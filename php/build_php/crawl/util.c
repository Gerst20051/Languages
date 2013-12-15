/*
 * Copyright (c) 1983, 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
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
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
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
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <err.h>
#include <fcntl.h>

#include "config.h"

#ifdef HAVE_LIBGEN_H 
#include <libgen.h>
#endif

#define HTTP_PREFIX	"http://"

char *Media_list[] = {
	".aiff", ".aif",
	".asf", ".asx",
	".avi",
	".au",
	".bmp",
	".dl", ".fli",
	".gif",
	".gl",
	".jpg",	".jpeg", ".jpe",
	".mid", ".midi",
	".mp3", "m3a",
	".mpg", ".mpa", ".mp2", ".m2a",
	".mpeg", ".mpe",
	".pcx",
	".pct", ".pict",
	".png",
	".qt", ".mov", ".moov",
	".ra", ".ram", ".rm", ".rmp",
	".tiff", ".tif",
	".voc",
	".viv",
	".wav",
	".wmv",
	".xbm",
	NULL
};

int
isMedia(char *name)
{
	char **listwalk;
	int len;

	for (listwalk = Media_list; *listwalk; listwalk++) {
		len = strlen(*listwalk);
		if (strlen(name) > len &&
		    !strcasecmp(name + strlen(name) - len, *listwalk))
			break;
	}

	return (*listwalk != NULL);
}

/*
 * mkpath -- create directories.  
 *	path     - path
 *	mode     - file mode of terminal directory
 *	dir_mode - file mode of intermediate directories
 */

int
mkpath(path, mode, dir_mode)
	char *path;
	mode_t mode;
	mode_t dir_mode;
{
	struct stat sb;
	register char *slash;
	int done = 0;

	path = slash = strdup(path);
	if (path == NULL) {
		warn("strdup");
		return (-1);
	}

	while (!done) {
		slash += strspn(slash, "/");
		slash += strcspn(slash, "/");

		done = (*slash == '\0');
		*slash = '\0';

		if (stat(path, &sb)) {
			if (errno != ENOENT ||
			    (mkdir(path, done ? mode : dir_mode) &&
			    errno != EEXIST)) {
				warn("%s", path);
				goto err;
			}
		} else if (!S_ISDIR(sb.st_mode)) {
			warnx("%s: %s", path, strerror(ENOTDIR));
			goto err;
		}

		*slash = '/';
	}

	free(path);
	return (0);

 err:
	free(path);
	return (-1);
}

char *
strdupend(char *s, char *end)
{
	char *p, *str;
	int len;

	for (p = s; *p && p < end; p++)
		;

	len = p - s;
	str = malloc(len + 1);
	if (str == NULL)
		return (NULL);

	memcpy(str, s, len);
	str[len] = '\0';

	return (str);
}

char *
construct_path(char *uri, int mkdir)
{
	static char path[1024];
	extern char *saveimgdir;
	char *dir;
	mode_t mode, dir_mode;

	mode = 0777 & ~umask(0);
	dir_mode = mode | S_IWUSR | S_IXUSR;

	dir = dirname(uri);
	strlcpy(path, saveimgdir, sizeof(path) - 1);
	if (path[strlen(path) - 1] != '/')
		strlcat(path, "/", sizeof(path));
	if (strlcat(path, dir + sizeof(HTTP_PREFIX) - 1,
		sizeof(path)) > sizeof (path))
		return (NULL);

	/* Build the directory path in the file system */
	if (mkdir && mkpath(path, mode, dir_mode) == -1)
		return (NULL);

	if (path[strlen(path) - 1] != '/')
		strlcat(path, "/", sizeof (path));

	if (strlcat(path, basename(uri), sizeof (path)) > sizeof (path))
		return (NULL);

	return (path);
}
