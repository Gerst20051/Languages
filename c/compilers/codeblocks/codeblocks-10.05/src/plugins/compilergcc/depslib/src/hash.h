/* *** This file was borrowed from jam 2.5. The copyright statement from
 * *** jam.c appears below.
 */
/*
 * /+\
 * +\	Copyright 1993-2002 Christopher Seiwald and Perforce Software, Inc.
 * \+/
 *
 * This file is part of jam.
 *
 * License is hereby granted to use this software and distribute it
 * freely, as long as this copyright notice is retained and modifications 
 * are clearly marked.
 *
 * ALL WARRANTIES ARE HEREBY DISCLAIMED.
 */

/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*
 * hash.h - simple in-memory hashing routines 
 *
 * 11/04/02 (seiwald) - const-ing for string literals
 */

typedef struct hashdata HASHDATA;

struct hash *	hashinit( int datalen, const char *name );
int 		hashitem( struct hash *hp, HASHDATA **data, int enter );
void 		hashdone( struct hash *hp );

# define	hashenter( hp, data ) !hashitem( hp, data, !0 )
# define	hashcheck( hp, data ) hashitem( hp, data, 0 )
