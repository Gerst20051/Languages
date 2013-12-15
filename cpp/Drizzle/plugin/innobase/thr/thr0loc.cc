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
@file thr/thr0loc.c
The thread local storage

Created 10/5/1995 Heikki Tuuri
*******************************************************/

#include "thr0loc.h"
#ifdef UNIV_NONINL
#include "thr0loc.ic"
#endif

#include "sync0sync.h"
#include "hash0hash.h"
#include "mem0mem.h"
#include "srv0srv.h"

/*
	IMPLEMENTATION OF THREAD LOCAL STORAGE
	======================================

The threads sometimes need private data which depends on the thread id.
This is implemented as a hash table, where the hash value is calculated
from the thread id, to prepare for a large number of threads. The hash table
is protected by a mutex. If you need modify the program and put new data to
the thread local storage, just add it to struct thr_local_struct in the
header file. */

/** Mutex protecting thr_local_hash */
static mutex_t		thr_local_mutex;

/** The hash table. The module is not yet initialized when it is NULL. */
static hash_table_t*	thr_local_hash	= NULL;

/** Thread local data */
typedef struct thr_local_struct thr_local_t;

#ifdef UNIV_PFS_MUTEX
/* Key to register the mutex with performance schema */
UNIV_INTERN mysql_pfs_key_t	thr_local_mutex_key;
#endif /* UNIV_PFS_MUTEX */

/** @brief Thread local data.
The private data for each thread should be put to
the structure below and the accessor functions written
for the field. */
struct thr_local_struct{
	os_thread_id_t	id;	/*!< id of the thread which owns this struct */
	os_thread_t	handle;	/*!< operating system handle to the thread */
	ulint		slot_no;/*!< the index of the slot in the thread table
				for this thread */
	ibool		in_ibuf;/*!< TRUE if the the thread is doing an ibuf
				operation */
	hash_node_t	hash;	/*!< hash chain node */
	ulint		magic_n;/*!< magic number (THR_LOCAL_MAGIC_N) */
};

/** The value of thr_local_struct::magic_n */
#define THR_LOCAL_MAGIC_N	1231234

#ifdef UNIV_DEBUG
/*******************************************************************//**
Validates thread local data.
@return	TRUE if valid */
static
ibool
thr_local_validate(
/*===============*/
	const thr_local_t*	local)	/*!< in: data to validate */
{
	ut_ad(local->magic_n == THR_LOCAL_MAGIC_N);
	ut_ad(local->slot_no == ULINT_UNDEFINED
	      || local->slot_no < OS_THREAD_MAX_N);
	ut_ad(local->in_ibuf == FALSE || local->in_ibuf == TRUE);
	return(TRUE);
}
#endif /* UNIV_DEBUG */

/*******************************************************************//**
Returns the local storage struct for a thread.
@return	local storage */
static
thr_local_t*
thr_local_get(
/*==========*/
	os_thread_id_t	id)	/*!< in: thread id of the thread */
{
	thr_local_t*	local;

try_again:
	ut_ad(thr_local_hash);
	ut_ad(mutex_own(&thr_local_mutex));

	/* Look for the local struct in the hash table */

	local = NULL;

	HASH_SEARCH(hash, thr_local_hash, os_thread_pf(id),
		    thr_local_t*, local, ut_ad(thr_local_validate(local)),
		    os_thread_eq(local->id, id));
	if (local == NULL) {
		mutex_exit(&thr_local_mutex);

		thr_local_create();

		mutex_enter(&thr_local_mutex);

		goto try_again;
	}

	ut_ad(thr_local_validate(local));

	return(local);
}

/*******************************************************************//**
Gets the slot number in the thread table of a thread.
@return	slot number */
UNIV_INTERN
ulint
thr_local_get_slot_no(
/*==================*/
	os_thread_id_t	id)	/*!< in: thread id of the thread */
{
	ulint		slot_no;
	thr_local_t*	local;

	mutex_enter(&thr_local_mutex);

	local = thr_local_get(id);

	slot_no = local->slot_no;

	mutex_exit(&thr_local_mutex);

	return(slot_no);
}

/*******************************************************************//**
Sets the slot number in the thread table of a thread. */
UNIV_INTERN
void
thr_local_set_slot_no(
/*==================*/
	os_thread_id_t	id,	/*!< in: thread id of the thread */
	ulint		slot_no)/*!< in: slot number */
{
	thr_local_t*	local;

	mutex_enter(&thr_local_mutex);

	local = thr_local_get(id);

	local->slot_no = slot_no;

	mutex_exit(&thr_local_mutex);
}

/*******************************************************************//**
Returns pointer to the 'in_ibuf' field within the current thread local
storage.
@return	pointer to the in_ibuf field */
UNIV_INTERN
ibool*
thr_local_get_in_ibuf_field(void)
/*=============================*/
{
	thr_local_t*	local;

	mutex_enter(&thr_local_mutex);

	local = thr_local_get(os_thread_get_curr_id());

	mutex_exit(&thr_local_mutex);

	return(&(local->in_ibuf));
}

/*******************************************************************//**
Creates a local storage struct for the calling new thread. */
UNIV_INTERN
void
thr_local_create(void)
/*==================*/
{
	thr_local_t*	local;

	if (thr_local_hash == NULL) {
		thr_local_init();
	}

	local = static_cast<thr_local_t *>(mem_alloc(sizeof(thr_local_t)));

	local->id = os_thread_get_curr_id();
	local->handle = os_thread_get_curr();
	local->magic_n = THR_LOCAL_MAGIC_N;
	local->slot_no = ULINT_UNDEFINED;
	local->in_ibuf = FALSE;

	mutex_enter(&thr_local_mutex);

	HASH_INSERT(thr_local_t, hash, thr_local_hash,
		    os_thread_pf(os_thread_get_curr_id()),
		    local);

	mutex_exit(&thr_local_mutex);
}

/*******************************************************************//**
Frees the local storage struct for the specified thread. */
UNIV_INTERN
void
thr_local_free(
/*===========*/
	os_thread_id_t	id)	/*!< in: thread id */
{
	thr_local_t*	local;

	mutex_enter(&thr_local_mutex);

	/* Look for the local struct in the hash table */

	HASH_SEARCH(hash, thr_local_hash, os_thread_pf(id),
		    thr_local_t*, local, ut_ad(thr_local_validate(local)),
		    os_thread_eq(local->id, id));
	if (local == NULL) {
		mutex_exit(&thr_local_mutex);

		return;
	}

	HASH_DELETE(thr_local_t, hash, thr_local_hash,
		    os_thread_pf(id), local);

	mutex_exit(&thr_local_mutex);

	ut_a(local->magic_n == THR_LOCAL_MAGIC_N);
	ut_ad(thr_local_validate(local));

	mem_free(local);
}

/****************************************************************//**
Initializes the thread local storage module. */
UNIV_INTERN
void
thr_local_init(void)
/*================*/
{

	ut_a(thr_local_hash == NULL);

	thr_local_hash = hash_create(OS_THREAD_MAX_N + 100);

	mutex_create(thr_local_mutex_key,
		     &thr_local_mutex, SYNC_THR_LOCAL);
}

/********************************************************************
Close the thread local storage module. */
UNIV_INTERN
void
thr_local_close(void)
/*=================*/
{
	ulint		i;

	ut_a(thr_local_hash != NULL);

	/* Free the hash elements. We don't remove them from the table
	because we are going to destroy the table anyway. */
	for (i = 0; i < hash_get_n_cells(thr_local_hash); i++) {
		thr_local_t*	local;

		local = static_cast<thr_local_t *>(HASH_GET_FIRST(thr_local_hash, i));

		while (local) {
			thr_local_t*	prev_local = local;

			local = static_cast<thr_local_t *>(HASH_GET_NEXT(hash, prev_local));
			ut_a(prev_local->magic_n == THR_LOCAL_MAGIC_N);
			ut_ad(thr_local_validate(prev_local));
			mem_free(prev_local);
		}
	}

	hash_table_free(thr_local_hash);
	thr_local_hash = NULL;
}
