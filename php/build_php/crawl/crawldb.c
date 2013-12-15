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
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <limits.h>
#include <time.h>
#include <err.h>
#include <string.h>

#include "config.h"

#ifdef HAVE_DB_185_H
#define DB_LIBRARY_COMPATIBILITY_API
#include <db_185.h>
#elif HAVE_DB_H
#include <db.h>
#endif
#include <compat/md5.h>

DB *db;

DBT *
db_key(char *line)
{
	static DBT key;
	static u_char digest[16];
	MD5_CTX ctx;
	int i;

	MD5Init(&ctx);
	MD5Update(&ctx, line, strlen(line) + 1);
	MD5Final(digest, &ctx);

	for (i = 0; i < 8; i++)
		digest[i] ^= digest[i + 8];
	
	key.data = digest;
	key.size = 8;

	return (&key);
}

int
db_seen(char *line)
{
	DBT *key;
	DBT data;
	time_t now;
	int res;

	key = db_key(line);
	res = db->get(db, key, &data, 0);
	if (res != 1)
		return (1);

	now = time(NULL);

	data.data = &now;
	data.size = sizeof(now);

	res = db->put(db, key, &data, 0);

	return (0);
}

void
db_setup(char *file)
{
	int flags, mode;
	BTREEINFO binfo;

	flags = O_RDWR|O_CREAT;
	mode = S_IRUSR|S_IWUSR;

	memset(&binfo, 0, sizeof(binfo));
	binfo.cachesize = 10000000;
	db = dbopen(file, flags, mode, DB_BTREE, &binfo);
	if (db == NULL)
		errx(1, "dbopen");
}

void
db_close(void)
{
	db->close(db);
}
