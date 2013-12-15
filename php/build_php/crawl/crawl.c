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
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/queue.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <unistd.h>
#include <regex.h>

#include "config.h"

#include <event.h>

#include "tree.h"
#include "http.h"
#include "html.h"
#include "crawldb.h"
#include "util.h"
#include "dns.h"
#include "cfg.h"

#define CRAWL		"crawl"
#define CRAWL_VERSION	VERSION

void usage(void);
ssize_t atomicio(ssize_t (*f)(), int, void *, size_t);

int url_permitted(char *);
int img_permitted(char *);
int image_saver(struct uri *);

char *progname;

struct html_parse *parser;

/* Regular expression which determines if an url gets included */
regex_t reurlinc;
regex_t reurlexcl;
regex_t reimginc;
regex_t reimgexcl;

/* Path to directory where the saved images go to */
char *saveimgdir;

struct timeval tv_timeout;
int maxdepth = 0;

#define WAIT_DEFAULT	5

#define MEDIA_MINLEN	20000
#define MEDIA_MAXLEN	400000

int media_minlen = MEDIA_MINLEN;
int media_maxlen = MEDIA_MAXLEN;

/* An external filter that does special processing */
int extmatchw = -1;
int extmatchr = -1;

int debug = 1;

#define DFPRINTF(x,y)	if (debug >= x) fprintf y

void
usage(void)
{
	fprintf(stderr,
		"%s: [-u urlincl] [-e urlexcl] [-i imgincl] [-d dir] [-m depth] <url> ...\n"
		"\t for documentation of all options consult the man page\n",
		progname);
}

int
url_permitted(char *url)
{
	if (strncasecmp(url, HTTP_PREFIX, strlen(HTTP_PREFIX)))
		return (0);

	if (regexec(&reurlinc, url, 0, NULL, 0) != 0)
		return (0);

	if (regexec(&reurlexcl, url, 0, NULL, 0) == 0)
		return (0);

	/* Use an external filter */
	if (extmatchw != -1) {
		char ch;

		if (atomicio(write, extmatchw, url, strlen(url)) == -1)
			err(1, "write");
		if (atomicio(write, extmatchw, "\n", 1) == -1)
			err(1, "write");

		if (atomicio(read, extmatchr, &ch, 1) == -1)
			err(1, "read");

		if (ch != 'y')
			return (0);
	}

	return (1);
}

int
img_permitted(char *url)
{
	if (strncasecmp(url, HTTP_PREFIX, strlen(HTTP_PREFIX)))
		return (0);

	if (regexec(&reimginc, url, 0, NULL, 0) != 0)
		return (0);

	if (regexec(&reimgexcl, url, 0, NULL, 0) == 0)
		return (0);

	return (1);
}

int
image_saver(struct uri *uri)
{
	struct stat sb;
	char *path;
	char tmp[128], *p = "";
	char *url;

	url = http_make_url(&uri->url);

	/* 
	 * Sometimes we download something that should have been
	 * html, but is media instead.
	 */
	if (uri->save_fd == -1 && !img_permitted(url))
		return (-1);

	if (uri->length != -1 && uri->bdlen != uri->length) {
		snprintf(tmp, sizeof(tmp), " (%4.1f%%/%d)",
		    (float)uri->bdlen/uri->length*100, uri->length);
		p = tmp;
	}
	fprintf(stdout, "%s %s%s\n",
		uri->flags & HTTP_REQUEST_GET ? "GET" : "HEAD",
		url, uri->flags & HTTP_REQUEST_GET ? p : "");

	if (uri->flags & HTTP_REQUEST_HEAD) {
		int minlen, maxlen;

		if (uri->format == NULL) {
			minlen = media_minlen;
			maxlen = media_maxlen;
		} else {
			/* Get lengths depending on mime types */
			minlen = conf_get_num(uri->format, "Min-Length",
			    media_minlen);
			maxlen = conf_get_num(uri->format, "Max-Length",
			    media_maxlen);
		}

		/* See if it meets our extra constraints */
		if ((minlen != -1 && uri->length < minlen) ||
		    (maxlen != -1 && uri->length > maxlen))
			return (-1);

		/* Re-add request as GET */
		http_add(HTTP_REQUEST_GET, url, uri->depth);
		return (0);
	}

	if ((path = construct_path(url, 1)) == NULL)
		return (-1);
	if (stat(path, &sb) != -1) {
		if (sb.st_size >= uri->length)
			return (-1);
	}
	if (uri->save_fd == -1) {
		uri->save_fd = open(path, O_WRONLY|O_CREAT|O_TRUNC, 0644);
		if (uri->save_fd == -1)
			return (-1);
	}

	if (atomicio(write, uri->save_fd, uri->body, uri->bdread) == -1)
		return (-1);

	uri->bdread = 0;
	
	return (0);
}

void
http_dealimage(char *url, u_short depth)
{
	char *path;

	if (!img_permitted(url)) {
		DFPRINTF(3, (stderr, "Reject: %s\n", url));
		return;
	}

	/* Check if the file exists */
	if ((path = construct_path(url, 0)) == NULL)
		return;
	if (access(path, F_OK) != -1)
		return;

	if (http_mark_seen(url) != 0)
		return;

	http_add(HTTP_REQUEST_HEAD, url, depth);
}

void
html_foundimage(void *arg, char *el, char **attr)
{
	struct uri *uri = arg;
	char *base, *normal;

	if (!strcasecmp(el, "img"))
		attr = html_attr_find(attr, "src");
	else if (!strcasecmp(el, "body"))
		attr = html_attr_find(attr, "background");

	if (*attr == NULL)
		return;

	base = http_basename(uri);
	if (base == NULL)
		return;
       
	normal = http_make_uri(base, attr[1]);
	if (normal == NULL) {
		DFPRINTF(3, (stderr, "Failed on %s and %s\n", base, attr[1]));
		return;
	}

	http_dealimage(normal, uri->depth + 1);
}

void
html_foundanchor(void *arg, char *el, char **attr)
{
	struct uri *uri = arg;
	char *base, *normal;

	if (!strcasecmp(el, "a"))
		attr = html_attr_find(attr, "href");
	else if (!strcasecmp(el, "frame"))
		attr = html_attr_find(attr, "src");
	else if (!strcasecmp(el, "iframe"))
		attr = html_attr_find(attr, "src");
	else if (!strcasecmp(el, "area"))
		attr = html_attr_find(attr, "href");
	else if (!strcasecmp(el, "base"))
		attr = html_attr_find(attr, "link");
	else if (!strcasecmp(el, "link")) {
		char **p;
		p = html_attr_find(attr, "href");
		if (*p != NULL)
			attr = p;
		else 
			attr = html_attr_find(attr, "rel");
	} else
		return;

	if (*attr == NULL)
		return;

	base = http_basename(uri);
	if (base == NULL)
		return;
       
	normal = http_make_uri(base, attr[1]);
	if (normal == NULL) {
		DFPRINTF(3, (stderr, "Failed on %s and %s\n", base, attr[1]));
		return;
	}

	/* If this is an image, do to the image dealer */
	if (isMedia(normal))
		return (http_dealimage(normal, uri->depth + 1));

	if (maxdepth != -1 && uri->depth + 1 > maxdepth) {
		DFPRINTF(2, (stderr, "Max depth reached: %s\n", normal));
		return;
	}

	/* fprintf(stdout, "Anchor: %s\n", normal); */
	if (!url_permitted(normal)) {
		DFPRINTF(3, (stderr, "Reject: %s\n", normal));
		return;
	}

	if (http_mark_seen(normal) != 0)
		return;

	http_add(HTTP_REQUEST_GET, normal, uri->depth + 1);
}

int
html_follower(struct uri *uri)
{
	fprintf(stdout, "%s http://%s%s\n",
		uri->flags & HTTP_REQUEST_GET ? "GET" : "HEAD",
		uri->url_host, uri->url_file);

	/* We should never get a head request here */
	if (uri->flags & HTTP_REQUEST_HEAD)
		return (0);

	html_parse_setdata(parser, uri);
	html_parser(parser, uri->body, uri->bdlen);

	return (0);
}

/* Deal with relocations */

void
http_moved(struct uri *uri, char *location)
{
	if (!url_permitted(location)) {
		DFPRINTF(3, (stderr, "Reject: %s\n", location));
		return;
	}

	if (http_mark_seen(location) != 0)
		return;

	http_add(uri->flags & HTTP_REQUEST_GET ?
	    HTTP_REQUEST_GET : HTTP_REQUEST_HEAD,
	    location, uri->depth);
}

void
external_filter(char *program)
{
	int toext[2], fromext[2];
	int res;

	if (pipe(toext) == -1)
		err(1, "pipe");
	if (pipe(fromext) == -1)
		err(1, "pipe");
		
	res = fork();
	if (res == -1)
		err(1, "fork");
	if (res == 0) {
		/* Child */
		dup2(toext[0], fileno(stdin));
		close(toext[1]);
		dup2(fromext[1], fileno(stdout));
		close(fromext[0]);

		if (execlp(program, program, NULL) == -1)
			err(1, "execlp");
		exit(0);
	}

	/* Parent */
	extmatchw = toext[1];
	extmatchr = fromext[0];
	close(toext[0]);
	close(fromext[1]);
}

#define CONF_SAVE(w,f)	do { \
				char *p = f; \
				if (p != NULL) \
					(w) = p; \
			} while (0)

int
main(int argc, char **argv)
{
	extern char *optarg;
	extern int optind;
	char *urlinclude = "http://.*\\.citi\\.umich\\.edu";
	char *urlexclude = "\\.(ps|gz|c|h|tar|exe|doc|pdf|ppt|txt|diff)$";
	char *imginclude = "\\.(jpg|jpeg)";
	char *imgexclude = "thumbs\\.";
	char *agent = CRAWL"/"CRAWL_VERSION;
	extern int use_robots, http_maxconnects;
	char *state = "crawl.state";
	char *external = NULL;
	char *resume = NULL; 
	int ch;

	progname = argv[0];
	saveimgdir = ".";
	timerclear(&tv_timeout);
	tv_timeout.tv_sec = WAIT_DEFAULT;

	conf_init();
	CONF_SAVE(urlinclude, conf_get_str("General", "Url-Include"));
	CONF_SAVE(urlexclude, conf_get_str("General", "Url-Exclude"));
	CONF_SAVE(imginclude, conf_get_str("General", "Img-Include"));
	CONF_SAVE(imgexclude, conf_get_str("General", "Img-Exclude"));
	CONF_SAVE(saveimgdir, conf_get_str("General", "Img-Directory"));
	CONF_SAVE(external, conf_get_str("General", "External-Filter"));
	CONF_SAVE(agent, conf_get_str("HTTP", "Agent"));
	CONF_SAVE(state, conf_get_str("HTTP", "State-File"));
	use_robots = conf_get_num("HTTP", "Use-Robots", 1);
	http_maxconnects = conf_get_num("HTTP", "Connections", HTTP_MAXCONNECTS);
	maxdepth = conf_get_num("General", "Max-Depth", 0);
	media_minlen = conf_get_num("General", "Min-Length", MEDIA_MINLEN);
	media_maxlen = conf_get_num("General", "Max-Length", MEDIA_MAXLEN);

	while ((ch = getopt(argc, argv, "t:v:u:e:i:d:m:RA:E:I:c:")) != -1)
		switch(ch) {
		case 't': {
			char *p;
			float val;
			val = strtod(optarg, &p);
			if (p == NULL && *p != '\0') {
				usage();
				exit(1);
			}

			tv_timeout.tv_sec = val;
			tv_timeout.tv_usec = (val - (int)val) * 1000000L;
			break;
		}
		case 'v':
			debug = atoi(optarg);
			break;
		case 'c':
			resume = optarg;
			break;
		case 'u':
			urlinclude = optarg;
			break;
		case 'e':
			urlexclude = optarg;
			break;
		case 'i':
			imginclude = optarg;
			break;
		case 'I':
			imgexclude = optarg;
			break;
		case 'd':
			saveimgdir = optarg;
			break;
		case 'm':
			maxdepth = atoi(optarg);
			break;
		case 'A':
			agent = optarg;
			break;
		case 'R':
			use_robots = 0;
			break;
		case 'E':
			external = optarg;
			break;
		default:
			usage();
			exit(1);
		}

	argc -= optind;
	argv += optind;

	if (resume == NULL && argc < 1) {
		usage();
		exit(1);
	}

	if (regcomp(&reurlinc, urlinclude, REG_EXTENDED|REG_ICASE) == -1)
		errx(1, "Error compiling regexp: '%s'\n", urlinclude);
	if (regcomp(&reurlexcl, urlexclude, REG_EXTENDED|REG_ICASE) == -1)
		errx(1, "Error compiling regexp: '%s'\n", urlinclude);
	if (regcomp(&reimginc, imginclude, REG_EXTENDED|REG_ICASE) == -1)
		errx(1, "Error compiling regexp: '%s'\n", imginclude);
	if (regcomp(&reimgexcl, imgexclude, REG_EXTENDED|REG_ICASE) == -1)
		errx(1, "Error compiling regexp: '%s'\n", imgexclude);

	if (external != NULL)
		external_filter(external);

	db_setup("crawl.db");

	event_init();
	/* dns_init has to go before http_init because of signal handlers */
	dns_init();
	http_init(state);

	if (resume == NULL) {
		while (argc) {
			if (http_add(HTTP_REQUEST_GET, argv[0], 0) != -1)
				http_mark_seen(argv[0]);
			
			argc--;
			argv++;
		}
	} else
		if (http_restore_state(resume) == -1)
			exit(1);

	/* Schedule connections */
	while (http_postevent() != -1)
		;

	if (strlen(agent))
		http_setuseragent(agent);

	http_register_dispatch("text/html", html_follower);
	http_register_dispatch("image/", image_saver);
	http_register_dispatch("audio/", image_saver);
	http_register_dispatch("video/", image_saver);

	/* XXX - bad cludge */
	http_setcallback(http_movecb, http_moved);

	if ((parser = html_newparser()) == NULL)
		return (-1);

	html_register_cb(parser, "img", html_foundimage);
	html_register_cb(parser, "body", html_foundimage);
	html_register_cb(parser, "a", html_foundanchor);
	html_register_cb(parser, "frame", html_foundanchor);
	html_register_cb(parser, "area", html_foundanchor);
	html_register_cb(parser, "base", html_foundanchor);
	html_register_cb(parser, "link", html_foundanchor);

	setvbuf(stdout, NULL, _IONBF, 0);

	event_dispatch();

	/* Print some informative stats */
	http_print_stats();
	dns_print_stats();
	dns_end();

	/* Done with everything */
	html_freeparser(parser);
	db_close();

	exit (0);
}
