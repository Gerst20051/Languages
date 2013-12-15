/*****************************************************************************

Copyright (C) 1995, 2009, Innobase Oy. All Rights Reserved.

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
@file include/thr0loc.h
The thread local storage

Created 10/5/1995 Heikki Tuuri
*******************************************************/

/* This module implements storage private to each thread,
a capability useful in some situations like storing the
OS handle to the current thread, or its priority. */

#pragma once
#ifndef thr0loc_h
#define thr0loc_h

#include "univ.i"
#include "os0thread.h"

/****************************************************************//**
Initializes the thread local storage module. */
UNIV_INTERN
void
thr_local_init(void);
/*================*/
 /****************************************************************//**
Close the thread local storage module. */
UNIV_INTERN
void
thr_local_close(void);
/*=================*/
/*******************************************************************//**
Creates a local storage struct for the calling new thread. */
UNIV_INTERN
void
thr_local_create(void);
/*==================*/
/*******************************************************************//**
Frees the local storage struct for the specified thread. */
UNIV_INTERN
void
thr_local_free(
/*===========*/
	os_thread_id_t	id);	/*!< in: thread id */
/*******************************************************************//**
Gets the slot number in the thread table of a thread.
@return	slot number */
UNIV_INTERN
ulint
thr_local_get_slot_no(
/*==================*/
	os_thread_id_t	id);	/*!< in: thread id of the thread */
/*******************************************************************//**
Sets in the local storage the slot number in the thread table of a thread. */
UNIV_INTERN
void
thr_local_set_slot_no(
/*==================*/
	os_thread_id_t	id,	/*!< in: thread id of the thread */
	ulint		slot_no);/*!< in: slot number */
/*******************************************************************//**
Returns pointer to the 'in_ibuf' field within the current thread local
storage.
@return	pointer to the in_ibuf field */
UNIV_INTERN
ibool*
thr_local_get_in_ibuf_field(void);
/*=============================*/

#ifndef UNIV_NONINL
#include "thr0loc.ic"
#endif

#endif
