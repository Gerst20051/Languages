/*****************************************************************************

Copyright (C) 1996, 2009, Innobase Oy. All Rights Reserved.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 51 Franklin
St, Fifth Floor, Boston, MA 02110-1301 USA

*****************************************************************************/

/********************************************************************//**
@file include/btr0types.h
The index tree general types

Created 2/17/1996 Heikki Tuuri
*************************************************************************/

#pragma once
#ifndef btr0types_h
#define btr0types_h

#include "univ.i"

#include "rem0types.h"
#include "page0types.h"

/** Persistent cursor */
typedef struct btr_pcur_struct		btr_pcur_t;
/** B-tree cursor */
typedef struct btr_cur_struct		btr_cur_t;
/** B-tree search information for the adaptive hash index */
typedef struct btr_search_struct	btr_search_t;

/** The size of a reference to data stored on a different page.
The reference is stored at the end of the prefix of the field
in the index record. */
#define BTR_EXTERN_FIELD_REF_SIZE	20

/** A BLOB field reference full of zero, for use in assertions and tests.
Initially, BLOB field references are set to zero, in
dtuple_convert_big_rec(). */
extern const byte field_ref_zero[BTR_EXTERN_FIELD_REF_SIZE];

#endif
