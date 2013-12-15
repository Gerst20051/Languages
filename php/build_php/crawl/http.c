/*
 * Copyright 2001 Niels Provos <provos@citi.umich.edu>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by Niels Provos.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/types.h>
#include <sys/queue.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <ctype.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <err.h>
#include <errno.h>
#include <time.h>

#include "config.h"

#include <event.h>

#include "tree.h"
#include "connection.h"
#include "http.h"
#include "crawldb.h"
#include "robots.h"
#include "dns.h"
#include "util.h"

/* Proto-types */
ssize_t atomicio(ssize_t (*f)(), int, void *, size_t);
void http_insert_order(struct uri_list *, struct uri_small *);
void http_connectioncb(int, short, void *);
int http_isrobotstxt(struct uri *);
int http_robotstxt(struct uri *);

extern struct timeval tv_timeout;
extern int debug;
#define DFPRINTF(x,y)   if (debug >= x) fprintf y

/* Two separate queues, mediaqueue gets priority */
struct uri_list uriqueue;
struct uri_list mediaqueue;

TAILQ_HEAD(dispatch_list, dispatch) dispatchqueue;

u_char body[32768];
char *useragent = NULL;
char *rawname = NULL;
char *statefile = NULL;

void (*http_movecb)(struct uri *, char *) = NULL;

/* Timeout used for postevent */
struct event ev_timeout;

int http_maxconnects = HTTP_MAXCONNECTS;
int connections;
int dns_connections;

static int cacheable;	/* used by http_insertorder */

int use_robots = 1;
/* Set by signal handler if we are supposed to terminate */
int terminate = 0;

#define STATS_INCFIFO	\
	do { \
		http_stats.fifosize++; \
		if (http_stats.fifosize > http_stats.maxsize) \
			 http_stats.maxsize = http_stats.fifosize; \
	} while (0)

#define STATS_DECFIFO	http_stats.fifosize--;

static int in_httppostevent;

static struct stats {
	size_t bodybytes;
	size_t headerbytes;
	size_t gets;
	size_t heads;
	time_t start;
	size_t fifosize;
	size_t maxsize;
} http_stats;

struct uri *
uri_new(void)
{
	struct uri *uri;

	uri = calloc(1, sizeof (struct uri));
	if (uri == NULL)
		return (NULL);

	uri->fd = -1;
	uri->save_fd = -1;
	
	return (uri);
}

void
uri_free(struct uri *uri, int cleanconnect)
{
	if (event_initialized(&uri->ev))
		event_del(&uri->ev);
	
	/* If this is an aborted robots.txt, we treat it as no robots.txt */
	if (use_robots)
		http_robotstxt(uri);

	if (uri->fd != -1)
		close(uri->fd);

	if (cleanconnect || uri->fd != -1) {
		int n = 0;

		connections--;

		/* Try to start new connections */
		while (http_postevent() != -1)
			n++;
	}

	if (uri->dns != NULL)
		dns_unref(uri->dns);
	
	if (uri->save_fd != -1)
		close(uri->save_fd);

	if (uri->url_host != NULL)
		free(uri->url_host);
	if (uri->url_file != NULL)
		free(uri->url_file);
	if (uri->header != NULL)
		free(uri->header);
	if (uri->body != NULL)
		free(uri->body);
	if (uri->format != NULL)
		free(uri->format);
	free(uri);
}

struct uri_small *
uri_small_new(void)
{
	struct uri_small *uri;

	uri = malloc(sizeof (struct uri_small));
	memset(&uri->url, 0, sizeof(uri->url));

	uri->retry = 0;

	return (uri);
}

void
uri_small_free(struct uri_small *uri)
{
	if (uri->url_host != NULL)
		free(uri->url_host);
	if (uri->url_file != NULL)
		free(uri->url_file);
	free(uri);
}

void
terminate_handler(int sig)
{
	int s = errno;

	signal(SIGINT, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	terminate = 1;

	errno = s;
}

void
http_posteventcb(int fd, short which, void *arg)
{
	/* Reschedule events if possible */
	while (http_postevent() != -1)
		;
}

int
http_init(char *filename)
{
	TAILQ_INIT(&uriqueue);
	TAILQ_INIT(&mediaqueue);

	TAILQ_INIT(&dispatchqueue);

	timeout_set(&ev_timeout, http_posteventcb, NULL);

	if (use_robots)
		http_robots_init();

	connections = 0;

	memset(&http_stats, 0, sizeof (http_stats));
	http_stats.start = time(NULL);

	if (filename) {
		statefile = strdup(filename);
		if (statefile == NULL) {
			perror("strdup");
			return (-1);
		}

		/* Setup signal handler */
		if (signal(SIGINT, terminate_handler) == SIG_ERR) {
			perror("signal");
			return (-1);
		}
		if (signal(SIGPIPE, terminate_handler) == SIG_ERR) {
			perror("signal");
			return (-1);
		}
	}
	
	return (0);
}

void
http_print_stats(void)
{
	time_t spent;
	size_t bytes;

	spent = time(NULL) - http_stats.start;
	bytes = http_stats.headerbytes + http_stats.bodybytes;

	fprintf(stdout, "%d GET for body %d Kbytes\n",
		http_stats.gets, http_stats.bodybytes / 1024);
	fprintf(stdout, "%d HEAD for header %d Kbytes\n",
		http_stats.heads, http_stats.headerbytes / 1024);
	fprintf(stdout, "% 8.3f Requests/sec\n",
		(float)(http_stats.gets + http_stats.heads)/ spent);
	fprintf(stdout, "Throughput %f KBytes/sec\n",
		(float) bytes / 1024 / spent);
	fprintf(stdout, "Max fifo size: %d, %f Kbyte, current: %d/%d\n",
		http_stats.maxsize,
		(float) http_stats.maxsize *
		(sizeof(struct uri_small) + 80)/1024,
		http_stats.fifosize, connections);

	if (use_robots)
		http_robots_print_stats();
}

int
http_setuseragent(char *name)
{
	char agent[1024];

	if (useragent != NULL)
		free(useragent);
	if (rawname != NULL)
		free(rawname);

	/* Safe the raw name for robots.txt handling */
	rawname = strdup(name);
	if (rawname == NULL) {
		useragent = NULL;
		return (-1);
	}

	snprintf(agent, sizeof (agent), "User-Agent: %s %s/%s\r\n",
		 name, HTTP_NAME, HTTP_VERSION);
	useragent = strdup(agent);

	return (useragent != NULL ? 0 : -1);
}

int
http_register_dispatch(char *type, int (*cb)(struct uri *))
{
	struct dispatch *dispatch;

	dispatch = malloc(sizeof (struct dispatch));
	if (dispatch == NULL) {
		warn("%s: malloc", __func__);
		return (-1);
	}
	dispatch->type = type;
	dispatch->cb = cb;

	TAILQ_INSERT_TAIL(&dispatchqueue, dispatch, next);

	return (0);
}


/* The file descriptor needs to be connected */

void
http_fetch(struct uri *uri)
{
	char request[1024];
	char sport[NI_MAXSERV];
	struct timeval tv;

	/* fprintf(stdout, "Fetching: %s:%d %s\n", host, port, file); */
	snprintf(sport, sizeof(sport), "%d", uri->url_port);

	snprintf(request, sizeof(request),
	    "%s %s HTTP/1.0\r\n"
	    "%s"
	    "Host: %s%s%s\r\n\r\n",
	    uri->flags & HTTP_REQUEST_GET ? "GET" : "HEAD", uri->url_file,
	    useragent != NULL ? useragent : "",
	    uri->url_host,
	    uri->url_port != HTTP_DEFAULTPORT ? ":" : "",
	    uri->url_port != HTTP_DEFAULTPORT ? sport : "");

	atomicio(write, uri->fd, request, strlen(request));

	event_set(&uri->ev, uri->fd, EV_READ, http_readheader, uri);
	timerclear(&tv);
	tv.tv_sec = HTTP_READTIMEOUT;
	event_add(&uri->ev, &tv);
}

void
http_collect(char *name, struct uri_list *dsthead, struct uri_list *srchead)
{
	struct uri_small *tmp, *next;

	for (tmp = TAILQ_FIRST(srchead); tmp; tmp = next) {
		next = TAILQ_NEXT(tmp, next);

		if (strcasecmp(name, tmp->url_host))
			continue;

		/* Remove from generic queue, and put on dns queue */
		TAILQ_REMOVE(srchead, tmp, next);
		http_insert_order(dsthead, tmp);
	}
}

/* A negative dns result, causes us to remove all entries for this host */

void
http_decollect(char *name, struct uri_list *srchead)
{
	struct uri_small *tmp, *next;

	for (tmp = TAILQ_FIRST(srchead); tmp; tmp = next) {
		next = TAILQ_NEXT(tmp, next);

		if (strcasecmp(name, tmp->url_host))
			continue;

		/* Remove from generic queue, and free */
		TAILQ_REMOVE(srchead, tmp, next);
		uri_small_free(tmp);
	}
}

void
http_dns_calcdepth(struct dns_entry *dns)
{
	struct uri_small *suri;
	int depth = 0;

	suri = TAILQ_FIRST(&dns->mediaqueue);
	if (suri != NULL && suri->depth > depth)
		    depth = suri->depth;

	suri = TAILQ_FIRST(&dns->uriqueue);
	if (suri != NULL && suri->depth > depth)
		    depth = suri->depth;

	if (dns->depth != dns->depth)
		dns_setdepth(dns, depth);
}

void
http_newconnection_dnscb(struct addrinfo *ai, struct dns_entry *dns, void *arg)
{
	struct uri *uri = arg;
	struct timeval tv;
	int fd;

	dns_connections--;

	/* See if we should queue addresses on the dns object */
	if (dns != NULL) {
		if (DNS_NEGATIVE(dns)) {
			DFPRINTF(1, (stderr, "%s: decollecting %s\n",
				     __func__, dns->name));
			/* the DNS object might have uris queued on it */
			http_decollect(dns->name, &dns->mediaqueue);
			http_decollect(dns->name, &dns->uriqueue);
			/* Search generic list */
			http_decollect(dns->name, &mediaqueue);
			http_decollect(dns->name, &uriqueue);
			goto fail;
		} else if (timercmp(&dns->creat, &dns->access, =)) {
			/* First time this dns object has been used */
			http_collect(dns->name, &dns->mediaqueue, &mediaqueue);
			http_collect(dns->name, &dns->uriqueue, &uriqueue);
		}

		http_dns_calcdepth(dns);
	}

	/* DNS lookup failed */
	if (ai == NULL)
		goto fail;

	/* Complete sillyness of the API */
	fd = connection_new(ai, uri->url_port);
	if (fd == -1)
		goto fail;

	event_set(&uri->ev, fd, EV_WRITE, http_connectioncb, uri);
	timerclear(&tv);
	tv.tv_sec = HTTP_CONNECT_TIMEOUT;
	event_add(&uri->ev, &tv);

	/* Reference counting for host accesses */
	if (uri->dns != NULL)
		dns_unref(uri->dns);
	dns_ref(dns);
	uri->dns = dns;

	/* Record next time that we may access this host */
	gettimeofday(&dns->access, NULL);
	timeradd(&dns->access, &tv_timeout, &dns->access);

	uri->fd = fd;
	return;

 fail:
	uri_free(uri, URI_CLEANCONNECT);
}

int
http_newconnection(struct uri *uri, char *host, u_short port)
{
	int res;

	connections++;
	dns_connections++;

	res = dns_resolve_cb(host, port, http_newconnection_dnscb, uri);
	if (res == -1) {
		dns_connections--;
		fprintf(stderr, "%s: http_newconnection(%s,%d) failed\n", 
		    __func__, host, port);
	}
	return (res);
}

void
http_connection_retry(struct uri *uri)
{
	uri->retry++;

	if (uri->retry > HTTP_MAXRETRY)
		goto fail;

	/* As we are closing a file descriptor here, try to start
	 * a new connection on failure
	 */
	close(uri->fd);
	connections--;	/* http_newconnection is going to increase for us */

	if (http_newconnection(uri, uri->url_host, uri->url_port) == -1)
		goto fail;

	return;
 fail:
	warnx("%s: failed retry: http://%s%s", __func__, 
	    uri->url_host, uri->url_file);
	uri_free(uri, URI_CLEANCONNECT);
}

void
http_connectioncb(int fd, short which, void *arg)
{
	int error;
	socklen_t errsz = sizeof(error);
	struct uri *uri = arg;

	if (which == EV_TIMEOUT)
		goto retry;

	/* Check if the connection completed */
	if (getsockopt(uri->fd, SOL_SOCKET, SO_ERROR, &error,
		       &errsz) == -1) {
		warn("%s: getsockopt for http://%s%s", __func__,
		    uri->url_host, uri->url_file);
		uri_free(uri, URI_CLEANCONNECT);
		return;
	}

	if (error) {
		if (error == ETIMEDOUT || error == ECONNREFUSED)
			goto retry;

		fprintf(stderr, "%s: %s failed: http://%s%s (%d)\n", __func__,
			uri->url_host, uri->url_file, strerror(error), error);
		uri_free(uri, URI_CLEANCONNECT);
		return;
	}

	http_fetch(uri);

	return;

 retry:
	http_connection_retry(uri);
}

/* Separated host, port and file from URI */

int
http_hostportfile(char *url, char **phost, u_short *pport, char **pfile)
{
	static char host[1024];
	static char file[1024];
	char *p, *p2;
	int len;
	u_short port;

	len = strlen(HTTP_PREFIX);
	if (strncasecmp(url, HTTP_PREFIX, len))
		return (-1);

	url += len;

	/* We might overrun */
	if (strlcpy(host, url, sizeof (host)) >= sizeof(host))
		return (-1);

	p = strchr(host, '/');
	if (p != NULL) {
		*p = '\0';
		p2 = p + 1;
	} else
		p2 = NULL;

	if (pfile != NULL) {
		/* Generate request file */
		if (p2 == NULL)
			p2 = "";
		snprintf(file, sizeof(file), "/%s", p2);
	}

	p = strchr(host, ':');
	if (p != NULL) {
		*p = '\0';
		port = atoi(p + 1);

		if (port == 0)
			return (-1);
	} else
		port = HTTP_DEFAULTPORT;

	if (phost != NULL)
		*phost = host;
	if (pport != NULL)
		*pport = port;
	if (pfile != NULL)
		*pfile = file;

	return (0);
}

int
http_restore_state(char *filename)
{
	FILE *fp;
	char line[2048], *p, *p2;
	int i, type, depth, linenum;

	fp = fopen(filename, "r");
	if (fp == NULL)
		err(1, "fopen");

	linenum = 1;
	fprintf(stdout, "Resuming from saved state...\n");
	cacheable = 1;
	while (fgets(line, sizeof (line), fp) != NULL) {
		for (i = strlen(line) - 1; i >= 0; i--)
			if (line[i] == '\r' || line[i] == '\n')
				line[i] = '\0';

		p2 = line;
		p = strsep(&p2, " ");
		if (*p != 'g' && *p != 'h')
			goto fail;
		type = *p == 'g' ? HTTP_REQUEST_GET : HTTP_REQUEST_HEAD;

		p = strsep(&p2, " ");
		if (p == NULL || p2 == NULL ||
		    (depth = atoi(p)) < 0)
			goto fail;

		if (strncasecmp(p2, HTTP_PREFIX, strlen(HTTP_PREFIX)))
			goto fail;

		http_add(type, p2, depth);

		linenum++;
	}
	cacheable = 0;
	fclose(fp);

	fprintf(stdout, "Continuing with %d urls.\n", linenum - 1);
	return (0);

 fail:
	fclose(fp);
	fprintf(stderr, "Line %d in state file corrupt.\n", linenum);
	return (-1);
}

void
http_savequeue(FILE *fp, struct uri_list *head, int *num)
{
	struct uri_small *uri;
	int linenum = *num;

	while ((uri = TAILQ_FIRST(head)) != NULL) {
		TAILQ_REMOVE(head, uri, next);

		fprintf(fp, "%c %d %s\n",
		    uri->flags & HTTP_REQUEST_GET ? 'g' : 'h',
		    uri->depth, http_make_url(&uri->url));

		linenum++;
	}

	*num = linenum;
}

void
http_savestate(void)
{
	extern struct dns_list dnsqueue;
	struct dns_entry *dns;

	FILE *fpstate = NULL;
	int linenum = 0;

	timeout_del(&ev_timeout);

	fprintf(stdout, "Terminating...\n");

	if ((fpstate = fopen(statefile, "w")) == NULL)
		err(1, "fopen");

	TAILQ_FOREACH(dns, &dnsqueue, next) {
		http_savequeue(fpstate, &dns->mediaqueue, &linenum);
		http_savequeue(fpstate, &dns->uriqueue, &linenum);
	}

	http_savequeue(fpstate, &mediaqueue, &linenum);
	http_savequeue(fpstate, &uriqueue, &linenum);

	fclose(fpstate);

	fprintf(stdout, "Terminated with %d saved urls.\n", linenum);
}

#define HTTP_RBTCHK_PROCEED	0	/* Okay to go ahead */
#define HTTP_RBTCHK_DENY	1	/* Not okay */
#define HTTP_RBTCHK_ASKING	2	/* Retrieving robots.txt now */
#define HTTP_RBTCHK_PENDING	3	/* Still asking */

int
http_robot_check(struct uri_list *head, struct uri_small *suri,
    char *host, short port)
{
	struct http_robots *htrobot;

	if ((htrobot = http_robots_find(host, port)) == NULL) {
		/* Generates a new connection */
		if (http_robots_get(host, port) != -1)
			return (HTTP_RBTCHK_ASKING);
	} else {
		if (htrobot->flags & HTROBOT_PENDING)
			return (HTTP_RBTCHK_PENDING);
		if (!http_robots_allow(htrobot, suri->url_file)) {
			fprintf(stderr,
			    "%s: robots diallowed url: http://%s%s\n",
			    __func__, suri->url_host, suri->url_file);
				
			TAILQ_REMOVE(head, suri, next);
			STATS_DECFIFO;

			uri_small_free(suri);
			return (HTTP_RBTCHK_DENY);
		}
	}

	return (HTTP_RBTCHK_PROCEED);
}

/*
 * Schedule a new request of a given dns object or the generic list
 */

int
http_schedule(struct dns_entry *dns, struct uri_list *head)
{
	struct uri *uri;
	struct uri_small *suri, *next;
	int res = -1;

	if ((uri = uri_new()) == NULL)
		return (-1);

	for (suri = TAILQ_FIRST(head); suri; suri = next) {
		next = TAILQ_NEXT(suri, next);

		/* Only up to third outstanding dns queries */
		if (dns == NULL && (dns_connections > http_maxconnects/3 ||
			dns_connections > (DNS_MAX_CHILDREN*2)))
			return (-1);

		if (use_robots) {
			int res = http_robot_check(head, suri,
			    suri->url_host, suri->url_port);

			/* Everything okay */
			switch (res) {
			case HTTP_RBTCHK_ASKING:
				if (connections >= http_maxconnects) {
					res = 0;
					goto out;
				}
				/* FALLTHROUGH */
			case HTTP_RBTCHK_PENDING:
			case HTTP_RBTCHK_DENY:
				continue;
			case HTTP_RBTCHK_PROCEED:
				/* Everything is okay */
				break;
			}
		}

		TAILQ_REMOVE(head, suri, next);
		STATS_DECFIFO;

		/* Copy small to large uri */
		uri->url = suri->url;
		uri->flags = suri->flags;
		uri->depth = suri->depth;

		/* suri still needs freeing */
		suri->url_host = NULL;
		suri->url_file = NULL;

		if (http_newconnection(uri,uri->url_host,uri->url_port)== -1) {
			connections--;

			/* Swap back to small state */
			suri->url = uri->url;
			uri->url_host = NULL;
			uri->url_file = NULL;

			/* These errors might be transient */
			suri->retry++;
			if (suri->retry >= HTTP_MAXRETRY) {
				uri_small_free(suri);
			} else {
				http_insert_order(head, suri);
				STATS_INCFIFO;
			}
			continue;
		}

		/* Everything is fine */
		break;
	}

	/* Nothing to do, go away */
	if (suri == NULL)
		goto out;
	uri_small_free(suri);

	return (0);

 out:
	if (uri != NULL)
		uri_free(uri, 0);
	return (res);
}

int
dns_schedule(int media)
{
	extern struct dns_list dnsqueue;
	struct uri_list *head;
	struct uri_small *suri;
	struct dns_entry *dns;
	struct timeval now, tv_ready, tv_min;
	int res = -1;
	int depth = 0;

	gettimeofday(&now, NULL);

	head = media ? &mediaqueue : &uriqueue;
	suri = TAILQ_FIRST(head);
	if (suri != NULL)
		depth = suri->depth;

	timerclear(&tv_min);
	TAILQ_FOREACH(dns, &dnsqueue, next) {
		if (head != NULL && dns->depth < depth) {
			/* Schedule from generic queue */
			res = http_schedule(NULL, head);
			if (res != -1) {
				dns = NULL;
				break;
			}

			/*
			 * We failed to get one of the generic queue,
			 * so we may continue with dns entries
			 */
			head = NULL;
		}

		/* Either pending or no time yet */
		if (!dns_ready(dns, &now, &tv_ready, HTTP_MAXPARALLEL)) {
			if (timerisset(&tv_ready) &&
			    (!timerisset(&tv_min) ||
				timercmp(&tv_ready, &tv_min, <))) {
				tv_min = tv_ready;
			}
			continue;
		}

		res = http_schedule(dns,
		    media ? &dns->mediaqueue : &dns->uriqueue);
		if (res != -1)
			break;
	}

	/* Potentially remove an element */
	if (dns != NULL) {
		if (res != -1) {
			http_dns_calcdepth(dns);
			return (0);
		}
	} else if (head != NULL)
		res = http_schedule(NULL, head);

	if (res == -1 && timerisset(&tv_min) && 
	    !timeout_pending(&ev_timeout, NULL)) {
		struct timeval tv;
		timersub(&tv_min, &now, &tv);

		timeout_add(&ev_timeout, &tv);
	}

	return (res);
}

int
http_postevent(void)
{
	int res;

	if (connections >= http_maxconnects) {
		DFPRINTF(4, (stderr, "%s: %d >= http_maxconnects\n", __func__,
			     connections));
		return (-1);
	}

	if (terminate) {
		if (connections == 0) {
			http_savestate();
			return (-1);
		}

		fprintf(stdout,
			"Terminating: %3d(%3d) of %3d connections pending.\n",
			connections, dns_connections, http_maxconnects);
		return (-1);
	}

	/* Prevent endless loop on some errors */
	if (in_httppostevent)
		return (-1);
	in_httppostevent = 1;

	/* We are calling schedule, schedule adds a new timeout if necessary */
	timeout_del(&ev_timeout);

	/* media list first */
	res = dns_schedule(1);
	if (res == 0)
		goto out;

	/* uri list */
	res = dns_schedule(0);

 out:
	in_httppostevent = 0;

	return (res);
}

/* Inserts a new url into descending order on the list.
 * When we resume from a saved state file, we will remember
 * a tail pointer to avoid O(n**2) behaviour.
 */

void
http_insert_order(struct uri_list *head, struct uri_small *uri)
{
	struct uri_small *tmp;
	static struct uri_list *cache_head;
	static struct uri_small *cache_uri;
	static int cache_depth;

	if (cacheable && cache_head == head && uri->depth <= cache_depth)
		tmp = cache_uri;
	else
		tmp = TAILQ_FIRST(head);

	for (; tmp; tmp = TAILQ_NEXT(tmp, next)) {
		if (tmp->depth <= uri->depth)
			break;
	}

	if (tmp == NULL) {
		TAILQ_INSERT_HEAD(head, uri, next);
	} else {
		TAILQ_INSERT_BEFORE(tmp, uri, next);
	}

	if (cacheable) {
		/* Do some quick caching */
		cache_head = head;
		cache_uri = uri;
		cache_depth = uri->depth;
	}
}

int
http_add(u_short type, char *url, u_short depth)
{
	struct uri_small *uri;
	struct uri_list *head;
	struct dns_entry *dns;
	u_short port;
	char *host, *file;

	if (http_hostportfile(url, &host, &port, &file) == -1) {
		fprintf(stderr, "%s: illegal url: %s\n", __func__, url);
		return (-1);
	}

	dns = dns_find(host);
	/* On a negative cache hit, we just throw the url away */
	if (dns != NULL && DNS_NEGATIVE(dns)) {
		DFPRINTF(1, (stderr, 
			     "%s: dropping %s on negative dns cache hit\n",
			     __func__, url));
		return (0);
	}
	
	uri = uri_small_new();

	if (uri == NULL) {
		warn("%s: malloc", __func__);
		return (-1);
	}

	uri->url_host = strdup(host);
	uri->url_file = strdup(file);
	uri->url_port = port;
	if (uri->url_host == NULL || uri->url_file == NULL) {
		warn("%s: malloc", __func__);
		uri_small_free(uri);
		return (-1);
	}

	/* GET or HEAD */
	uri->flags = type;
	uri->depth = depth;

	/* If we have an entry for the hostname queue url up there */
	if (dns != NULL) {
		head = isMedia(url) ? &dns->mediaqueue : &dns->uriqueue;
	} else
		head = isMedia(url) ? &mediaqueue : &uriqueue;

	http_insert_order(head, uri);
	STATS_INCFIFO;

	return (0);
}

void
http_readheader(int fd, short event, void *arg)
{
	char line[2048], *p;
	ssize_t n, offset;
	struct timeval tv;
	struct uri *uri = arg;

	if (event == EV_TIMEOUT) {
		fprintf(stderr, "%s: timeout http://%s%s\n", __func__,
		    uri->url_host, uri->url_file);
		uri_free(uri, URI_CLEANCONNECT);

		return;
	}

	n = read(uri->fd, line, sizeof(line));
	if (n == -1) {
		if (errno == EINTR || errno == EAGAIN)
			goto readmore;
		warn("%s: read", __func__);
		uri_free(uri, URI_CLEANCONNECT);

		return;
	} else if (n == 0) {
		/* Uhm dum */
		fprintf(stderr, "%s: finished read on http://%s%s?\n",
		    __func__, uri->url_host, uri->url_file);

		uri_free(uri, URI_CLEANCONNECT);
		return;
	}
	
	p = realloc(uri->header, uri->hdlen + n + 1);
	if (p == NULL) {
		warn("%s: realloc", __func__);
		uri_free(uri, URI_CLEANCONNECT);

		return;
	}

	uri->header = p;
	memcpy(uri->header + uri->hdlen, line, n);
	uri->hdlen += n;
	uri->header[uri->hdlen] = '\0';

	p = strstr(uri->header, HTTP_HEADEREND);
	if (p == NULL)
		goto readmore;

	offset = p + strlen(HTTP_HEADEREND) - uri->header;

	if (offset < uri->hdlen) {
		uri->bdlen = uri->hdlen - offset;
		uri->body = malloc(uri->bdlen + 1);
		if (uri->body == NULL) {
			warn("%s: malloc", __func__);
			uri_free(uri, URI_CLEANCONNECT);

			return;
		}
		memcpy(uri->body, uri->header + offset, uri->bdlen);
		uri->body[uri->bdlen] = '\0';

		/* Adjust header */
		uri->hdlen = offset;
		uri->header[offset] = '\0';
	}

	http_parseheader(uri);
	return;

 readmore:
	timerclear(&tv);
	tv.tv_sec = HTTP_READTIMEOUT;
	event_add(&uri->ev, &tv);
	return;

}

void
http_parseheader(struct uri *uri)
{
	char *p, *end;
	int major, minor, code;
	char *type = NULL, *length = NULL, *location = NULL;
	struct header parse[] = {
		{"Content-Type: ", &type},
		{"Content-Length: ", &length},
		{"Location: ", &location},
		{NULL, NULL}
	};
	struct header *hdr;
	struct timeval tv;

	if (sscanf(uri->header, "HTTP/%d.%d %d",
		   &major, &minor, &code) != 3 ||
	    major != 1 ||
	    (minor != 0 && minor != 1)) {
		fprintf(stderr, "%s: illegal header in http://%s%s\n",
		    __func__, uri->url_host, uri->url_file);

		uri_free(uri, URI_CLEANCONNECT);
		return;
	}

	/* Parse header */
	p = uri->header;
	end = p + uri->hdlen;
	while(p < end) {
		char *lend;

		lend = strstr(p, "\r\n");
		if (lend == NULL) {
			fprintf(stderr, "%s: illegal header in http://%s%s\n",
			    __func__, uri->url_host, uri->url_file);

			uri_free(uri, URI_CLEANCONNECT);
			return;
		}

		for (hdr = &parse[0]; hdr->name; hdr++) {
			if (strncasecmp(p, hdr->name,
					strlen(hdr->name)) == 0) {
				*hdr->where = p + strlen(hdr->name);
				break;
			}
		}

		p = lend + 2;
	}

	if (length == NULL)
		uri->length = -1;
	else
		uri->length = atoi(length);

	if (type != NULL) {
		end = strstr(type, "\r\n");

		uri->format = malloc(end - type + 1);
		if (uri->format == NULL) {
			warn("%s: malloc", __func__);
			uri_free(uri, URI_CLEANCONNECT);
			return;
		}

		memcpy(uri->format, type, end - type);
		uri->format[end - type] = '\0';
	}

	uri->code = code;

	switch (code) {
	case HTTP_OK:
		break;

	case HTTP_MOVETEMP:
	case HTTP_MOVEPERM:
		if (location == NULL || http_movecb == NULL ||
		    (use_robots && http_isrobotstxt(uri)))
			goto error;

		p = location;
		end = strstr(p, "\r\n");

		if ((location = malloc(end - p + 1)) == NULL) {
			warn("%s: malloc", __func__);
			uri_free(uri, URI_CLEANCONNECT);
			return;
		}

		memcpy(location, p, end - p);
		location[end - p] = '\0';

		/* User call back for move */
		(*http_movecb)(uri, location);

		free(location);
		uri_free(uri, URI_CLEANCONNECT);
		return;

	default:
		goto error;
	}

	/* If we just had a HEAD request, we are done now */
	if (uri->flags & HTTP_REQUEST_HEAD) {
		http_dispatch(uri, HTTPDIS_FREE);
		return;
	}

	/*
	 * Check if we have a dispatcher for this format.
	 * If we not, we are not going to continue with this.
	 */
	if (uri->format != NULL) {
		struct dispatch *dispatch;
		char *format = uri->format;

		if (use_robots && http_isrobotstxt(uri))
			goto conturi;

		/* Find a dispatch for this format */
		TAILQ_FOREACH(dispatch, &dispatchqueue, next) {
			if (strncasecmp(format, dispatch->type,
				strlen(dispatch->type)) == 0)
				break;
		}

		/* We do not need to download this */
		if (dispatch == NULL)
			goto error;
	}
 conturi:
	
	if (uri->length != -1) {
		size_t len;
		u_char *p;

		len = uri->length;
		if (len > HTTP_MAXMEM)
			len = HTTP_MAXMEM;
		
		p = realloc(uri->body, len + 1);
		if (p == NULL) {
			warn("%s: malloc", __func__);
			uri_free(uri, URI_CLEANCONNECT);
			return;
		}
		uri->body = p;
		uri->body[len] = '\0';
		uri->bdmemlen = len;
		uri->bdread = uri->bdlen;
	}

	event_set(&uri->ev, uri->fd, EV_READ,
		  http_readbody, uri);
	timerclear(&tv);
	tv.tv_sec = HTTP_READTIMEOUT;
	event_add(&uri->ev, &tv);
	return;

 error:
	http_stats.headerbytes += uri->hdlen;

	DFPRINTF(1, (stderr, "%s: abort http://%s%s: %d\n", __func__,
		     uri->url_host, uri->url_file, code));
	uri_free(uri, URI_CLEANCONNECT);
	return;
}

void
http_readbody(int fd, short event, void *arg)
{
	struct uri *uri = arg;
	struct timeval tv;
	ssize_t n;
	u_char *where;
	ssize_t len;

	if (event == EV_TIMEOUT) {
		fprintf(stderr, "%s: timeout http://%s%s\n", __func__,
		    uri->url_host, uri->url_file);
		uri_free(uri, URI_CLEANCONNECT);
		return;
	}

	if (uri->length == -1) {
		where = body;
		len = sizeof(body);
	} else {
		where = uri->body + uri->bdread;
		len = uri->length - uri->bdlen;
		if (len > uri->bdmemlen - uri->bdread)
			len = uri->bdmemlen - uri->bdread;
	}

	n = read(uri->fd, where, len);
	if (n == -1) {
		if (errno == EINTR || errno == EAGAIN)
			goto readmore;
		warn("%s: read", __func__);
		uri_free(uri, URI_CLEANCONNECT);

		return;
	} else if (n == 0) {
		if (uri->length != -1 &&
		    uri->length != uri->bdlen) {
			fprintf(stderr, "%s: short read on http://%s%s\n",
			    __func__, uri->url_host, uri->url_file);
			uri_free(uri, URI_CLEANCONNECT);
			return;
		}

		uri->length = uri->bdlen;
		goto done;
	}

	if (uri->length == -1) {
		u_char *p;

		p = realloc(uri->body, uri->bdlen + n + 1);
		if (p == NULL) {
			warn("%s: realloc", __func__);
			uri_free(uri, URI_CLEANCONNECT);

			return;
		}

		uri->body = p;
		memcpy(uri->body + uri->bdlen, body, n);
		uri->bdlen += n;
		uri->body[uri->bdlen] = '\0';
	} else {
		uri->bdlen += n;
		uri->bdread += n;
	}

	if (uri->length == -1 || uri->bdlen < uri->length)
		goto readmore;

	/* We are done with this document */

 done:
	http_dispatch(uri, HTTPDIS_FREE);
	return;

 readmore:
	timerclear(&tv);
	tv.tv_sec = HTTP_READTIMEOUT;
	event_add(&uri->ev, &tv);

	if (uri->length != -1 && uri->bdmemlen == uri->bdread)
		http_dispatch(uri, HTTPDIS_KEEP);
	
	return;
}

int
http_isrobotstxt(struct uri *uri)
{
	return (uri->url_file != NULL &&
	    strcmp(uri->url_file, "/robots.txt") == 0);
}

int
http_robotstxt(struct uri *uri)
{
	if (uri->url_file == NULL)
		return (0);

	if (strcmp(uri->url_file, "/robots.txt") == 0) {
		http_robots_response(uri);
		return (1);
	}

	return (0);
}



void
http_dispatch(struct uri *uri, int mustfree)
{
	struct dispatch *dispatch;
	char *format;

	format = uri->format;
	if (format == NULL)
		format = "text/html";

	if (uri->length < HTTP_MAXMEM || uri->bdread == 0) {
		http_stats.headerbytes += uri->hdlen;
		http_stats.bodybytes += uri->bdlen;
	} else {
		if (uri->bdlen == uri->bdmemlen)
			http_stats.headerbytes += uri->hdlen;
		http_stats.bodybytes += uri->bdread;
	}

	if (uri->flags & HTTP_REQUEST_GET)
		http_stats.gets++;
	else
		http_stats.heads++;

	if (use_robots && http_robotstxt(uri)) {
		uri_free(uri, URI_CLEANCONNECT);
		return;
	}
		
	TAILQ_FOREACH(dispatch, &dispatchqueue, next) {
		if (strncasecmp(format, dispatch->type,
				strlen(dispatch->type)) == 0) {
			/* Found an interested party */
			if (dispatch->cb(uri) != -1)
				break;
		}
	}

	/* caller decides */
	if (mustfree || dispatch == NULL) {
		uri_free(uri, URI_CLEANCONNECT);
		return;
	}

	/* object is still alive */
}

char *
http_normalize_uri(char *uri)
{
	static char normal[1024];
	char *host, *file, *p;
	u_short port;

	if (http_hostportfile(uri, &host, &port, &file) == -1)
		return (NULL);

	if ((p = strchr(file, '#')) != NULL)
		*p = '\0';
	
	/* Remove identities */
	p = file;
	while ((p = strstr(p, "/./")) != NULL) {
		memmove(p, p + 2, strlen(p + 2) + 1);
	}

	p = file;
	while ((p = strstr(p, "//")) != NULL) {
		char *p2 = p + strspn(p, "/");

		memmove(p + 1, p2, strlen(p2) + 1);
	}
	
	/* Deal with ../ */
	while ((p = strstr(file, "/..")) != NULL) {
		char *p2;

		for (p2 = p - 1; p2 > file; p2--)
			if (*p2 == '/')
				break;

		if (p2 <= file)
			memmove(file, p + 3, strlen(p + 3) + 1);
		else
			memmove(p2, p + 3, strlen(p + 3) + 1);
	}

	if (port != HTTP_DEFAULTPORT)
		snprintf(normal, sizeof(normal), "http://%s:%d", host, port);
	else
		snprintf(normal, sizeof(normal), "http://%s", host);

	/* Unix file names should not be lowered */
	p = normal;
	while (*p) {
		*p = tolower(*p);
		p++;
	}

	/* Append file and check for overrun */
	if (strlcat(normal, file, sizeof (normal)) >= sizeof (normal))
		return (NULL);

	return (normal);
}

/* Depends on http_normalize_uri */

char *
http_basename(struct uri *uri)
{
	char *p, *url, *normal;

	url = http_make_url(&uri->url);
	normal = http_normalize_uri(url);

	if (normal == NULL)
		return (NULL);

	p = strrchr(normal, '/');

	/* This should never happen */
	if (p == NULL)
		return (NULL);

	if (p[1] != '\0')
		p[1] = '\0';

	return (normal);
}

char *
http_make_uri(char *base, char *rel)
{
	static char normal[1024];

	if (!strncasecmp(rel, HTTP_PREFIX, strlen(HTTP_PREFIX)) ||
	    strchr(rel, ':') != NULL)
		return (rel);

	if (rel[0] == '/') {
		char *host, *file;
		u_short port;

		if (http_hostportfile(base, &host, &port, &file) == -1)
			return (NULL);

		if (port != HTTP_DEFAULTPORT)
			snprintf(normal, sizeof (normal), "http://%s:%d%s",
				 host, port, rel);
		else
			snprintf(normal, sizeof (normal), "http://%s%s",
				 host, rel);
	} else if (rel[0] == '#')
		return (NULL);
	else
		snprintf(normal, sizeof (normal), "%s%s", base, rel);

	return (http_normalize_uri(normal));
}

int
http_mark_seen(char *line)
{
	char *normal = http_normalize_uri(line);

	return (normal != NULL ? db_seen(normal) : 1);
}

char *
http_make_url(struct url *url)
{
	static char output[1024];

	if (url->port != HTTP_DEFAULTPORT)
		snprintf(output, sizeof(output), "http://%s:%d%s",
		    url->host, url->port, url->file);
	else
		snprintf(output, sizeof(output), "http://%s%s",
		    url->host, url->file);

	return (output);
}
