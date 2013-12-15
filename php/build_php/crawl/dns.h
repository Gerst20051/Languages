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

#ifndef _DNS_H_
#define _DNS_H_

#define DNS_MAX_CHILDREN	3

#define DNS_POSITIVE	0x0001
#define DNS_TEMPORARY	0x0002		/* temporary error */
#define DNS_PENDING	0x8000

#define DNS_MAX_RETRY	3		/* try resolution three times */
#define DNS_RETRY_TIME	(1*60)		/* retry DNS in that time */

struct dns_entry {
	TAILQ_ENTRY(dns_entry) next;		/* LRU list */
	TAILQ_ENTRY(dns_entry) wait_next;	/* Wait list for dns child */
	SPLAY_ENTRY(dns_entry) splay_next;	/* Lookup splay */

	struct timeval creat;		/* Creation time */
	struct timeval access;		/* Last access */

	int retries;			/* resolution retries */

	u_int16_t flags;
	short ref;			/* reference counter for downloads */

	char *name;
	struct addrinfo *ai;

	/* Callbacks when DNS is done */
	void (*cb)(struct addrinfo *, struct dns_entry *, void *);
	void *cbarg;

	/* Storage for documents yet to fetch */
	int depth;
	struct uri_list uriqueue;
	struct uri_list mediaqueue;
};

TAILQ_HEAD(dns_list, dns_entry);

void dns_init(void);
void dns_end(void);
void dns_print_stats(void);

struct addrinfo;
int dns_ready(struct dns_entry *, struct timeval *, struct timeval *, int);
int dns_resolve_cb(char *, u_short,
    void (*)(struct addrinfo *, struct dns_entry *, void *), void *);
void dns_ref(struct dns_entry *);
void dns_unref(struct dns_entry *);
struct dns_entry *dns_find(char *);

void dns_setdepth(struct dns_entry *, int);

#define DNS_UNUSED(x)	((x)->ref || TAILQ_FIRST(&(x)->mediaqueue) != NULL || \
			   TAILQ_FIRST(&(x)->uriqueue) != NULL)

/* Negative cache hit */
#define DNS_NEGATIVE(x)	((x) == 0)
#endif /* _DNS_H */
