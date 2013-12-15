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
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <err.h>

#include <event.h>

#include "config.h"
#include "tree.h"
#include "http.h"
#include "dns.h"

ssize_t atomicio(ssize_t (*f)(), int, void *, size_t);

extern int debug;
#define DFPRINTF(x,y)   if (debug >= x) fprintf y

#define DNS_MAXLRUSIZE	65536

struct dns_child {
	pid_t pid;
	struct dns_entry *current;

	int waits;

	struct event ev_read;
	struct event ev_write;
	int cmd_write;
	int res_read;

	struct dns_list waitqueue;
};

struct dns_child children[DNS_MAX_CHILDREN];
int childnr = 0;

struct stats {
	int hits;
	int replacements;
	int iterations;
	int calls;
} dns_stats;

SPLAY_HEAD(tree, dns_entry) root;

static int
compare(struct dns_entry *a, struct dns_entry *b)
{
	dns_stats.iterations++;
	return (strcasecmp(a->name, b->name));
}

SPLAY_PROTOTYPE(tree, dns_entry, splay_next, compare);

SPLAY_GENERATE(tree, dns_entry, splay_next, compare);

struct dns_transport {
	struct addrinfo ai;
	struct sockaddr_storage dns_addr;
};

void dns_child(int, int);
void dns_remove(struct dns_entry *);

struct dns_list dnsqueue;

int entries;

void
dns_makeaddrinfo(struct addrinfo **pai, struct dns_transport *dt)
{
	struct addrinfo *ai;

	ai = calloc(1, sizeof(struct addrinfo) + dt->ai.ai_addrlen);
	if (ai == NULL)
		err(1, "%s: malloc", __func__);

	*ai = dt->ai;
	ai->ai_addr = (struct sockaddr *)(ai + 1);
	memcpy(ai->ai_addr, &dt->dns_addr, ai->ai_addrlen);

	ai->ai_next = *pai;
	*pai = ai;
}

void
dns_read(int fd, short why, void *arg)
{
	struct dns_child *child = arg;
	struct dns_entry *dns;
	struct addrinfo *ai = NULL;
	struct timeval tv;
	u_char *buf;
	int size;

	dns = TAILQ_FIRST(&child->waitqueue);
	TAILQ_REMOVE(&child->waitqueue, dns, wait_next);

	/* Reschedule writes */
	child->waits--;
	if (child->waits)
		event_add(&child->ev_write, NULL);

	if (atomicio(read, fd, &size, sizeof(size)) != sizeof(size))
		err(1, "%s: read(%d)", __func__, sizeof(size));

	if (size == -1)
		goto fail;

	if (size > 0) {
		char *p;

		if ((buf = malloc(size)) == NULL)
			err(1, "%s: malloc", __func__);
		if (atomicio(read, fd, buf, size) != size)
			err(1, "%s: read(%d)", __func__, size);
		
		p = buf;
		while (size) {
			dns_makeaddrinfo(&ai, (struct dns_transport *)p);
			
			p += sizeof(struct dns_transport);
			size -= sizeof(struct dns_transport);
		}
		free(buf);
	} else
		ai = NULL;

	DFPRINTF(2, (stderr, "%s: return for %s: %p\n", __func__,
		     dns->name, ai));

	/* Remove pending or temporary status */
	dns->ai = ai;
	dns->flags = ai != NULL ? DNS_POSITIVE : 0;

	(*dns->cb)(ai, dns, dns->cbarg);

	return;
 fail:
	DFPRINTF(2, (stderr, "%s: error for %s\n", __func__, dns->name));

	dns->ai = NULL;

	dns->retries++;
	if (dns->retries > DNS_MAX_RETRY) {
		/* This is a negative cache entry, causes uri dequeue */
		dns->flags = 0;
	} else {
		dns->flags = DNS_TEMPORARY;

		/* Make them retry */
		gettimeofday(&dns->access, NULL);
		timerclear(&tv);
		tv.tv_sec = DNS_RETRY_TIME;
		timeradd(&tv, &dns->access, &dns->access);
	}

	(*dns->cb)(NULL, dns, dns->cbarg);
}

void
dns_write(int fd, short why, void *arg)
{
	struct dns_child *child = arg;
	struct dns_entry *dns;
	int size;

	dns = TAILQ_FIRST(&child->waitqueue);
	size = strlen(dns->name) + 1;

	if (atomicio(write, fd, &size, sizeof(size)) != sizeof(size))
		err(1, "write");
	if (atomicio(write, fd, dns->name, size) != size)
		err(1, "write");

	event_add(&child->ev_read, NULL);
}

int
dns_set_child(struct dns_child *child)
{
	int cmd[2];
	int res[2];

	memset(child, 0, sizeof(struct dns_child));

	if (pipe(cmd) == -1)
		err(1, "pipe");
	if (pipe(res) == -1)
		err(1, "pipe");

	if (signal(SIGCHLD, SIG_IGN) == SIG_ERR)
		err(1, "signal");

	child->pid = fork();
	if (child->pid == 0) {
		close(cmd[1]);
		close(res[0]);
		dns_child(cmd[0], res[1]);
		exit(0);
	} else if (child->pid == -1)
		err(1, "fork");

	close(cmd[0]);
	close(res[1]);

	child->cmd_write = cmd[1];
	child->res_read = res[0];

	child->waits = 0;

	TAILQ_INIT(&child->waitqueue);

	event_set(&child->ev_read, child->res_read, EV_READ, dns_read, child);
	event_set(&child->ev_write, child->cmd_write, EV_WRITE, dns_write, child);

	return (0);
}

void
dns_init(void)
{
	int i;

	TAILQ_INIT(&dnsqueue);
	SPLAY_INIT(&root);

	memset(&dns_stats, 0, sizeof(dns_stats));

	for (i = 0; i < DNS_MAX_CHILDREN; i++)
		dns_set_child(&children[i]);

	entries = 0;
}

void
dns_end(void)
{
	int i;

	for (i = 0; i < DNS_MAX_CHILDREN; i++)
		kill(children[i].pid, SIGTERM);
}

void
dns_print_stats(void)
{
	fprintf(stdout, "DNS queries: %d, entries: %d\n",
	    dns_stats.calls, entries);
	fprintf(stdout, "DNS cache hits: %d, replacements: %d\n",
		dns_stats.hits, dns_stats.replacements);
	fprintf(stdout, "DNS average list search: %f\n",
		(float)dns_stats.iterations/dns_stats.calls);
}

/* Reference counting on host names */

void
dns_unref(struct dns_entry *dns)
{
	if (dns->ref == 0) {
		fprintf(stderr, "%s: %s(%p) already unref\n", __func__,
		    dns->name, dns);
		return;
	}

	dns->ref--;
}

void
dns_ref(struct dns_entry *dns)
{
	dns->ref++;
}

void
dns_remove(struct dns_entry *dns)
{
	TAILQ_REMOVE(&dnsqueue, dns, next);
	SPLAY_REMOVE(tree, &root, dns);

	if (dns->name)
		free(dns->name);
	if (dns->ai)
		freeaddrinfo(dns->ai);
	memset(dns, 0, sizeof (struct dns_entry));
}

void
dns_set(struct dns_entry *dns, char *name,
    void (*cb)(struct addrinfo *, struct dns_entry *, void *), void *arg)
{
	/* Record creation and access time */
	gettimeofday(&dns->creat, NULL);
	dns->access = dns->creat;

	dns->name = name;
	dns->flags = DNS_PENDING;

	dns->cb = cb;
	dns->cbarg = arg;
}

struct addrinfo *
dns_roundrobin(struct dns_entry *dns)
{
	struct addrinfo *first = dns->ai;
	struct addrinfo *tmp;

	if (first == NULL || first->ai_next == NULL)
		return (first);

	tmp = first->ai_next;
	dns->ai = tmp;

	/* Detach at the beginning and put at the end */
	first->ai_next = NULL;
	while (tmp->ai_next != NULL)
		tmp = tmp->ai_next;

	tmp->ai_next = first;

	return (first);
}

void
dns_setdepth(struct dns_entry *dns, int depth)
{
	struct dns_entry *tmp;

	/* Fast path */
	if (depth == 0) {
		TAILQ_REMOVE(&dnsqueue, dns, next);
		TAILQ_INSERT_TAIL(&dnsqueue, dns, next);
		return;
	}
	
	while ((tmp = TAILQ_PREV(dns, dns_list, next))) {
		if (tmp->depth >= dns->depth)
			break;
	}

	if (tmp == NULL)
		return;
	if (tmp == TAILQ_PREV(dns, dns_list, next)) {
		while ((tmp = TAILQ_NEXT(dns, next))) {
			if (tmp->depth <= dns->depth)
				break;
		}

		if (tmp == NULL || tmp == TAILQ_NEXT(dns, next))
			return;
	}

	TAILQ_REMOVE(&dnsqueue, dns, next);
	TAILQ_INSERT_AFTER(&dnsqueue, tmp, dns, next);
	return;
}

struct dns_entry *
dns_find(char *host)
{
	struct dns_entry *dns, tmp;

	tmp.name = host;

	dns_stats.calls++;
	dns = SPLAY_FIND(tree, &root, &tmp);
	if (dns != NULL)
		dns_stats.hits++;

	return (dns);
}

struct dns_entry *
dns_get(void)
{
	struct dns_entry *dns = NULL;

	if (entries >= DNS_MAXLRUSIZE) {
		dns_stats.replacements++;

		/* Recycle an old entry */
		dns = TAILQ_LAST(&dnsqueue, dns_list);
		while (dns != NULL && DNS_UNUSED(dns))
			dns = TAILQ_PREV(dns, dns_list, next);
		if (dns != NULL)
			dns_remove(dns);
		/* 
		 * If there is no old unused entry, we fall through and
		 * allocate a new one.
		 */
	}

	if (dns == NULL) {
		dns = calloc(1, sizeof(struct dns_entry));
		entries++;
	}

	/* Initalize queues */
	TAILQ_INIT(&dns->uriqueue);
	TAILQ_INIT(&dns->mediaqueue);

	return (dns);
}

int
dns_ready(struct dns_entry *dns, struct timeval *tv, struct timeval *ready,
    int max)
{
	int res;

	timerclear(ready);

	if (dns->flags & DNS_PENDING)
		return (0);

	/* Limit number of parallel connections */
	if (max && dns->ref >= max)
		return (0);

	/* See if we may access this host yet */
	res = timercmp(&dns->access, tv, <=);

	if (!res)
		*ready = dns->access;

	return (res);
}

void
dns_send(struct dns_entry *dns)
{
	struct dns_child *child;

	/* Round robin on the children */
	child = &children[++childnr % DNS_MAX_CHILDREN];

	TAILQ_INSERT_TAIL(&child->waitqueue, dns, wait_next);
	
	if (child->waits == 0)
		event_add(&child->ev_write, NULL);

	child->waits++;
}

/*
 * If tv != NULL, it records the gap between access times.
 */

int
dns_resolve_cb(char *ip, u_short port,
    void (*cb)(struct addrinfo *, struct dns_entry *, void *), void *arg)
{
	char *name;
	struct dns_entry *dns;
	struct addrinfo *ai = NULL;
	int positive;

	dns = dns_find(ip);

	if (dns != NULL) {
		/* 
		 * Return an error if we have a pending access for this
		 * host already.
		 */
		if (dns->flags & DNS_PENDING)
			return (-1);

		if (dns->flags & DNS_TEMPORARY) {
			/* XXX - need to have retry limit */
			dns->cb = cb;
			dns->cbarg = arg;
			dns->flags |= DNS_PENDING;
			dns_send(dns);
			return (0);
		}

		if (dns->flags & DNS_POSITIVE)
			ai = dns_roundrobin(dns);
		else
			ai = NULL;

		(*cb)(ai, dns, arg);

		return (0);
	}

	name = strdup(ip);
	if (name == NULL) {
		warn("%s: strdup", __func__);
		return (-1);
	}

	/* Only use IP addresses with this interface, otherwise we
	 * will block on DNS.
	 */
	positive = 0;

	/* Get a slot for a dns entry */
	if ((dns = dns_get()) == NULL) {
		free(name);

		return (-1);
	}

	dns_set(dns, name, cb, arg);
	TAILQ_INSERT_TAIL(&dnsqueue, dns, next);
	SPLAY_INSERT(tree, &root, dns);

	dns_send(dns);
	return (0);
}

int
dns_resolve(char *ip, struct addrinfo **pai)
{
	struct addrinfo hints, *ai;
	int res;

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
	res = getaddrinfo(ip, NULL, &hints, &ai);
        if (res != 0) {
                fprintf(stderr, "%s: getaddrinfo(%s): %s\n", __func__,
		    ip, gai_strerror(res));
		if (res != EAI_NODATA)
			return (-1);

		/* Negative caching */
		ai = NULL;
        }

	*pai = ai;

	return (0);
}

void
dns_child_error(int fd)
{
	int size = -1;

	if (atomicio(write, fd, &size, sizeof(size)) != sizeof(size))
		exit(1);
}

void
dns_child_success(int fd, struct addrinfo *ai)
{
	int size = 0;
	char *buf = NULL, *p;
	struct dns_transport *dt;

	while (ai != NULL) {
		size += sizeof(struct dns_transport);

		if ((p = realloc(buf, size)) == NULL)
			break;
		buf = p;

		dt = (struct dns_transport *)(buf + size - sizeof(struct dns_transport));
		dt->ai = *ai;
		dt->ai.ai_canonname = NULL;
		dt->ai.ai_next = NULL;
		dt->ai.ai_addr = NULL;
		memcpy(&dt->dns_addr, ai->ai_addr, ai->ai_addrlen);

		ai = ai->ai_next;
	}

	if (atomicio(write, fd, &size, sizeof(size)) != sizeof(size))
		exit(1);
	if (size > 0 && atomicio(write, fd, buf, size) != size)
		exit(1);
	free(buf);
}

void
dns_child(int rfd, int wfd)
{
	struct addrinfo *ai;
	char line[1024];
	int i, size;

	/* Clean up file descriptors */
	for (i = 0; i < DNS_MAX_CHILDREN && children[i].pid; i++) {
		close(children[i].cmd_write);
		close(children[i].res_read);
	}

	/* Setup signal handler */
	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
		err(1, "signal");

	while (atomicio(read, rfd, &size, sizeof(size)) == sizeof(size)) {
		/* Error - no handling */
		if (size > sizeof(line) || size < 0)
			break;
		
		if (atomicio(read, rfd, line, size) != size)
			exit(1);
		if (dns_resolve(line, &ai) == -1)
			dns_child_error(wfd);
		else {
			dns_child_success(wfd, ai);
			if (ai != NULL)
				freeaddrinfo(ai);
		}
	}

	exit(1);
}
