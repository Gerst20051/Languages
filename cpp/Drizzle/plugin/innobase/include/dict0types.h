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

/**************************************************//**
@file include/dict0types.h
Data dictionary global types

Created 1/8/1996 Heikki Tuuri
*******************************************************/

#pragma once
#ifndef dict0types_h
#define dict0types_h

typedef struct dict_sys_struct		dict_sys_t;
typedef struct dict_col_struct		dict_col_t;
typedef struct dict_field_struct	dict_field_t;
typedef struct dict_index_struct	dict_index_t;
typedef struct dict_table_struct	dict_table_t;
typedef struct dict_foreign_struct	dict_foreign_t;

/* A cluster object is a table object with the type field set to
DICT_CLUSTERED */

typedef dict_table_t			dict_cluster_t;

typedef struct ind_node_struct		ind_node_t;
typedef struct tab_node_struct		tab_node_t;

/* Space id and page no where the dictionary header resides */
#define	DICT_HDR_SPACE		0	/* the SYSTEM tablespace */
#define	DICT_HDR_PAGE_NO	FSP_DICT_HDR_PAGE_NO

typedef ib_id_t		table_id_t;
typedef ib_id_t		index_id_t;

#endif
