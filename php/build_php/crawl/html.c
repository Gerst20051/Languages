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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

#include <event.h>

#include "http.h"
#include "html.h"
#include "util.h"

char **
html_attr_find(char **attr, char *name)
{
	while (*attr != NULL) {
		if (!strcasecmp(*attr, name))
			break;

		attr += 2;
	}

	return (attr);
}

void
html_free_cb(struct html_cb *cb)
{
	if (cb->name != NULL)
		free(cb->name);

	free(cb);
}

int
html_register_cb(struct html_parse *p, char *name,
		 void (*callback)(void *, char *, char **))
{
	struct html_cb *cb;

	cb = malloc(sizeof (struct html_cb));
	if (cb == NULL)
		return (-1);

	cb->cb = callback;
	if ((cb->name = strdup(name)) == NULL)
		goto out;

	TAILQ_INSERT_TAIL(&p->cbqueue, cb, next);

	return (0);

 out:
	html_free_cb(cb);
	return (-1);
}

struct html_parse *
html_newparser(void)
{
	struct html_parse *p;

	p = calloc(1, sizeof(struct html_parse));
	if (p == NULL)
		return (NULL);

	TAILQ_INIT(&p->cbqueue);

	return (p);
}

void
html_freeparser(struct html_parse *p)
{
	struct html_cb *cb;

	if (p->base != NULL)
		free(p->base);

	for (cb = TAILQ_FIRST(&p->cbqueue); cb;
	     cb = TAILQ_FIRST(&p->cbqueue)) {
		TAILQ_REMOVE(&p->cbqueue, cb, next);
		html_free_cb(cb);
	}
		
	free(p);
}

int
html_parse_setbase(struct html_parse *p, char *base)
{
	if (p->base != NULL)
		free(p->base);

	p->base = strdup(base);
	
	return (p->base == NULL ? -1 : 0);
}

void
tag_start(struct html_parse *p, char *el, char **attr)
{
	struct html_cb *cb;
	void *arg;

	arg = p->data != NULL ? p->data : p;

	TAILQ_FOREACH(cb, &p->cbqueue, next) {
		if (!strcasecmp(cb->name, el)) {
			cb->cb(arg, el, attr);
			break;
		}
	}
}

void
tag_end(struct html_parse *parser, char *el)
{

}

#define WHITESPACE	" \r\n\t"
#define WHITEEND	" \r\n\t>"
#define ATTRDELIM	" \r\n\t=>"

void
html_parsetag(struct html_parse *parser, char *start, char *end)
{
	char *element, *elend, *attr, *attrend;
	char **pattr, **pattrend;
	char *attrlist[MAXATTR*2 + 2];
	int endtag = 0, i;
	char quoted;

	start = start + 1 + strspn(start + 1, WHITESPACE);
	elend = strpbrk(start, WHITEEND);

	if (start >= end)
		return;

	if (*start == '/') {
		endtag = 1;
		start++;

		if (start >= end)
			return;
	}

	if ((element = strdupend(start, elend)) == NULL)
		return;

	memset(attrlist, 0, sizeof(attrlist));

	if (endtag) {
		tag_end(parser, element);
		goto out;
	}

	attr = elend;
	pattr = attrlist;
	pattrend = pattr + MAXATTR*2;
	while (attr < end && pattr < pattrend) {
		attr += strspn(attr, WHITESPACE);
		attrend = strpbrk(attr, ATTRDELIM);

		if (attrend >= end)
			break;

		*pattr = strdupend(attr, attrend);
		if (*pattr == NULL)
			goto out;
		pattr++;

		attr = attrend + strspn(attrend, ATTRDELIM);
		if (*attr == '"' || *attr == '\'') {
			char delim[5];

			quoted = *attr;
			attr++;
			sprintf(delim, "%c>\r\n", quoted);
			attrend = strpbrk(attr, delim);
		} else {
			quoted = '\0';
			attrend = strpbrk(attr, WHITEEND);
		}

		if (attrend == NULL)
			goto out;

		*pattr = strdupend(attr, attrend);
		if (*pattr == NULL)
			goto out;
		pattr++;

		if (*attrend == quoted)
			attrend++;

		attr = attrend;
	}
		
	tag_start(parser, element, attrlist);

 out:
	for (i = 0; i < MAXATTR * 2; i++)
		if (attrlist[i] != NULL)
			free(attrlist[i]);
	free(element);
}

int
html_parser(struct html_parse *parser, char *body, size_t len)
{
	int res = 0;
	char *p, *end;

	p = body;
	end = p + len;

	while (p < end) {
		char *tagend;

		p = strchr(p, '<');
		if (p == NULL)
			break;

		if (!strncmp(p, "<!--", 4)) {
			/* Skip comments */
			tagend = strstr(p, "-->");
			if (tagend == NULL)
				break;
			p = tagend + 3;
			continue;
		} else
			tagend = strchr(p, '>');

		if (tagend == NULL)
			break;

		html_parsetag(parser, p, tagend);
		p = tagend + 1;
	}

	return (res);
}

