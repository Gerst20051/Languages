/*
  +----------------------------------------------------------------------+
  | APC                                                                  |
  +----------------------------------------------------------------------+
  | Copyright (c) 2006-2010 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Authors: Daniel Cowgill <dcowgill@communityconnect.com>              |
  |          George Schlossnagle <george@omniti.com>                     |
  |          Rasmus Lerdorf <rasmus@php.net>                             |
  |          Arun C. Murthy <arunc@yahoo-inc.com>                        |
  |          Gopal Vijayaraghavan <gopalv@yahoo-inc.com>                 |
  +----------------------------------------------------------------------+

   This software was contributed to PHP by Community Connect Inc. in 2002
   and revised in 2005 by Yahoo! Inc. to add support for PHP 5.1.
   Future revisions and derivatives of this source code must acknowledge
   Community Connect Inc. as the original contributor of this module by
   leaving this note intact in the source code.

   All other licensing and usage conditions are those of the PHP Group.

 */

/* $Id: apc.h 304310 2010-10-11 12:19:24Z gopalv $ */

#ifndef APC_H
#define APC_H

/*
 * This module defines utilities and helper functions used elsewhere in APC.
 */

/* Commonly needed C library headers. */
#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* UNIX headers (needed for struct stat) */
#include <sys/types.h>
#include <sys/stat.h>
#ifndef PHP_WIN32
#include <unistd.h>
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "php.h"
#include "main/php_streams.h"

/* typedefs for extensible memory allocators */
typedef void* (*apc_malloc_t)(size_t TSRMLS_DC);
typedef void  (*apc_free_t)  (void * TSRMLS_DC);

/* wrappers for memory allocation routines */
extern void* apc_emalloc(size_t n TSRMLS_DC);
extern void* apc_erealloc(void* p, size_t n TSRMLS_DC);
extern void apc_efree(void* p TSRMLS_DC);
extern char* apc_estrdup(const char* s TSRMLS_DC);
extern void* apc_xstrdup(const char* s, apc_malloc_t f TSRMLS_DC);
extern void* apc_xmemcpy(const void* p, size_t n, apc_malloc_t f TSRMLS_DC);

/* console display functions */
extern void apc_error(const char *format TSRMLS_DC, ...);
extern void apc_warning(const char *format TSRMLS_DC, ...);
extern void apc_notice(const char *format TSRMLS_DC, ...);
extern void apc_debug(const char *format TSRMLS_DC, ...);

/* string and text manipulation */
extern char* apc_append(const char* s, const char* t TSRMLS_DC);
extern char* apc_substr(const char* s, int start, int length TSRMLS_DC);
extern char** apc_tokenize(const char* s, char delim TSRMLS_DC);

/* filesystem functions */

typedef struct apc_fileinfo_t 
{
    char *fullpath;
    char path_buf[MAXPATHLEN];
    php_stream_statbuf st_buf;
} apc_fileinfo_t;

extern int apc_search_paths(const char* filename, const char* path, apc_fileinfo_t* fileinfo TSRMLS_DC);

/* regular expression wrapper functions */
extern void* apc_regex_compile_array(char* patterns[] TSRMLS_DC);
extern void apc_regex_destroy_array(void* p TSRMLS_DC);
extern int apc_regex_match_array(void* p, const char* input);

/* apc_crc32: returns the CRC-32 checksum of the first len bytes in buf */
extern unsigned int apc_crc32(const char* buf, int len);

/* apc_flip_hash flips keys and values for faster searching */
extern HashTable* apc_flip_hash(HashTable *hash); 

#define APC_NEGATIVE_MATCH 1
#define APC_POSITIVE_MATCH 2

#define apc_time() \
    (APCG(use_request_time) ? sapi_get_request_time(TSRMLS_C) : time(0));

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim>600: expandtab sw=4 ts=4 sts=4 fdm=marker
 * vim<600: expandtab sw=4 ts=4 sts=4
 */
