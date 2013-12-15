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

#include "config.h"

#include <event.h>

#include "tree.h"
#include "connection.h"
#include "http.h"
#include "robots.h"
#include "util.h"
#include "dns.h"

void http_robots_listfree(char **);
void http_robots_free(struct http_robots *);

#define ROBOTS_MAXLRUSIZE	32768

SPLAY_HEAD(rtree, http_robots) robottree;

struct stats {
	size_t robotfiles;
	size_t matches;
	size_t rejects;
	size_t replacements;
	size_t calls;
	size_t iterations;
} robot_stats;

static int
compare(struct http_robots *a, struct http_robots *b)
{
	robot_stats.iterations++;
	return (strcmp(a->host, b->host));
}

SPLAY_PROTOTYPE(rtree, http_robots, splay_next, compare);

SPLAY_GENERATE(rtree, http_robots, splay_next, compare);

TAILQ_HEAD(robots_list, http_robots) robotsqueue;
int robotsentries;

extern char *rawname;

void
http_robots_init(void)
{
	SPLAY_INIT(&robottree);
	TAILQ_INIT(&robotsqueue);
	robotsentries = 0;

	memset(&robot_stats, 0, sizeof(robot_stats));
}

void
http_robots_print_stats(void)
{
	fprintf(stdout, "Robots.txt inspected: %d\n", robot_stats.robotfiles);
	fprintf(stdout, "  matched useragent %d times\n", robot_stats.matches);
	fprintf(stdout, "  replaced %d entries\n", robot_stats.replacements);
	fprintf(stdout, "  rejected %d requests\n", robot_stats.rejects);
	fprintf(stdout, "  average list search: %f\n",
		(float)robot_stats.iterations/robot_stats.calls);
}

struct http_robots *
http_robots_new(char *host, u_short port)
{
	struct http_robots *htrobot;

	htrobot = calloc(1, sizeof(struct http_robots));
	if (htrobot == NULL) {
		warn("%s: calloc", __func__);
		return (NULL);
	}

	htrobot->host = strdup(host);
	if (htrobot->host == NULL) {
		free(htrobot);
		warn("%s: strdup", __func__);
		return (NULL);
	}
	htrobot->port = port;

	htrobot->flags = HTROBOT_PENDING;

	TAILQ_INSERT_HEAD(&robotsqueue, htrobot, robot_next);
	SPLAY_INSERT(rtree, &robottree, htrobot);

	if (robotsentries > ROBOTS_MAXLRUSIZE) {
		struct http_robots *tmp;

		tmp = TAILQ_LAST(&robotsqueue, robots_list);
		http_robots_free(tmp);

		robot_stats.replacements++;
	}

	robotsentries++;

	return (htrobot);
}

void
http_robots_free(struct http_robots *htrobot)
{
	TAILQ_REMOVE(&robotsqueue, htrobot, robot_next);
	SPLAY_REMOVE(rtree, &robottree, htrobot);

	if (htrobot->dislist)
		http_robots_listfree(htrobot->dislist);

	free(htrobot->host);
	free(htrobot);

	robotsentries--;
}

int
http_robots_get(char *host, u_short port)
{
	struct http_robots *htrobot;
	struct uri *uri;

	if ((uri = uri_new()) == NULL)
		return (-1);

	if ((uri->url_host = strdup(host)) == NULL) {
		uri_free(uri, 0);
		return (-1);
	}
	uri->url_port = port;
	if ((uri->url_file = strdup("/robots.txt")) == NULL) {
		uri_free(uri, 0);
		return (-1);
	}

	htrobot = http_robots_new(host, port);
	if (htrobot == NULL) {
		uri_free(uri, 0);
		return (-1);
	}

	if (http_newconnection(uri, host, port) == -1)
		goto fail;

	uri->flags = HTTP_REQUEST_GET;
	uri->depth = -1;

	return (0);

 fail:
	http_robots_free(htrobot);
	uri_free(uri, URI_CLEANCONNECT);
	return (-1);
}

struct http_robots *
http_robots_find(char *host, u_short port)
{
	struct http_robots *tmp, tmp2;

	tmp2.host = host;

	robot_stats.calls++;
	tmp = SPLAY_FIND(rtree, &robottree, &tmp2);

	return (tmp);
}

int
http_robots_allow(struct http_robots *htrobot, char *file)
{
	char **p = htrobot->dislist;

	if (p == NULL)
		return (1);

	while (*p) {
		if (!strncasecmp(file, *p, strlen(*p))) {
			robot_stats.rejects++;
			return (0);
		}

		p++;
	}

	return (1);
}

void
http_robots_listfree(char **list)
{
	char **p = list;
	
	while (*p) {
		free(*p);
		p++;
	}

	free (list);
}

#define FLD_USERAGENT	"User-Agent:"
#define FLD_DISALLOW	"Disallow:"

#define FLG_SKIPDISALLOW	0x0001
#define FLG_EXPLICITMATCH	0x0002

void
http_robots_response(struct uri *uri)
{
	char *p, *end, *lend;
	int flag, ualen = 0, tmpentries;
	struct http_robots *htrobot;
	char **tmplist;

	htrobot = http_robots_find(uri->url_host, uri->url_port);
	if (htrobot == NULL || !(htrobot->flags & HTROBOT_PENDING))
		return;

	robot_stats.robotfiles++;

	htrobot->flags &= ~HTROBOT_PENDING;

	if (uri->bdlen == 0 || uri->code == HTTP_NOTFOUND)
		return;

	/* Determine the match length */
	if (rawname != NULL) {
		char *where;
		where = strchr(rawname, '/');
		if (where != NULL)
			ualen = where - rawname;
		else
			ualen = strlen(rawname);
	}
		

	p = uri->body;
	end = p + uri->bdlen;

	flag = FLG_SKIPDISALLOW;
	tmplist = NULL;
	tmpentries = 1;
	for (; p < end; p = lend + strspn(lend, "\r\n")) {
		char *pa, *pe, **tmp;

		/* Find line end */
		lend = strpbrk(p, "\r\n");
		if (lend == NULL)
			lend = end;

		/* Deal with User-Agent field */
		if (!strncasecmp(p, FLD_USERAGENT, strlen(FLD_USERAGENT))) {
			pa = p + strlen(FLD_USERAGENT);
			pa += strspn(pa, " \t");

			/* Insert the current list */
			if (tmplist != NULL) {
				if (htrobot->dislist != NULL)
					http_robots_listfree(htrobot->dislist);
				htrobot->dislist = tmplist;

				tmplist = NULL;
				tmpentries = 1;
			}

			/* Wildcard match? */
			if (*pa != '*') {
				if (rawname == NULL ||
				    strncasecmp(pa, rawname, ualen)) {
					flag |= FLG_SKIPDISALLOW;
					continue;
				}

				flag |= FLG_EXPLICITMATCH;
			} else if (flag & FLG_EXPLICITMATCH) {
				/* If we had an explicit match, ignore wild */
				flag |= FLG_SKIPDISALLOW;
				continue;
			}

			/* The disallow fields are for us */
			flag &= ~FLG_SKIPDISALLOW;

			/* And we may destroy what we have right now */
			if (htrobot->dislist != NULL) {
				http_robots_listfree(htrobot->dislist);
				htrobot->dislist = NULL;
			}

			robot_stats.matches++;
		}

		/* Skip disallow field handling */
		if (flag & FLG_SKIPDISALLOW)
			continue;

		/* This is not a disallow field, skip */
		if (strncasecmp(p, FLD_DISALLOW, strlen(FLD_DISALLOW)))
			continue;

		pa = p + strlen(FLD_DISALLOW);
		pa += strspn(pa, " \t");
		pe = strpbrk(pa, " \t\r\n");

		/* Empty field means allow everyting */
		if (pe == pa) {
			flag |= FLG_SKIPDISALLOW;
			if (tmplist != NULL) {
				http_robots_listfree(tmplist);
				tmplist = NULL;
				tmpentries = 1;
			}

			/* Free the current disallow list */
			if (htrobot->dislist != NULL) {
				http_robots_listfree(htrobot->dislist);
				htrobot->dislist = NULL;
			}
			continue;
		}

		tmpentries++;
		tmp = realloc(tmplist, tmpentries * sizeof(char *));
		if (tmp != NULL) {
			tmplist = tmp;
			tmplist[tmpentries - 1] = NULL;
		}
		if (tmp == NULL ||
		    (tmp[tmpentries - 2] = strdupend(pa, pe)) == NULL) {
			/* On memory problems, we just take what we have */
			if (tmplist != NULL) {
				http_robots_listfree(tmplist);
				tmplist = NULL;
				tmpentries = 0;
			}
			return;
		}
	}

	if (tmplist != NULL) { 
		if (!(flag & FLG_SKIPDISALLOW)) {
			/* Insert the current list */
			if (htrobot->dislist != NULL)
				http_robots_listfree(htrobot->dislist);
			htrobot->dislist = tmplist;
		} else
			http_robots_listfree(tmplist);
	}
}
