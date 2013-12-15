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
  | Authors: Brian Shire <shire@php.net>                                 |
  +----------------------------------------------------------------------+

 */

/* $Id: apc_bin.h 300979 2010-07-04 10:15:05Z kalle $ */

#ifndef APC_BINDUMP_H
#define APC_BINDUMP_H

#include "apc.h"
#include "apc_php.h"
#include "ext/standard/basic_functions.h"

/* APC binload flags */
#define APC_BIN_VERIFY_MD5    1 << 0
#define APC_BIN_VERIFY_CRC32  1 << 1

typedef struct _apc_bd_entry_t {
    unsigned char type;
    uint num_functions;
    uint num_classes;
    apc_cache_entry_value_t val;
} apc_bd_entry_t;

typedef struct _apc_bd_t {
    unsigned int size;
    int swizzled;
    unsigned char md5[16];
    php_uint32 crc;
    unsigned int num_entries;
    apc_bd_entry_t *entries;
    int num_swizzled_ptrs;
    void ***swizzled_ptrs;
} apc_bd_t;

apc_bd_t* apc_bin_dump(HashTable *files, HashTable *user_vars TSRMLS_DC);
int apc_bin_load(apc_bd_t *bd, int flags TSRMLS_DC);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: expandtab sw=4 ts=4 sts=4 fdm=marker
 * vim<600: expandtab sw=4 ts=4 sts=4
 */
