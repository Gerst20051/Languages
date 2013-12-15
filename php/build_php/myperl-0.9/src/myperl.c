/* Copyright (C) 2003 Brian Aker
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

/*
** This is allows you to store raw perl in a MySQL database and 
** have it executed when called.
**
** The syntax to add this in is:
** CREATE FUNCTION myperl RETURNS STRING SONAME "myperl.so";
** To remove it you need to do this:
** DROP FUNCTION myperl;
** For the Aggregate Function
** The syntax to add this in is:
** CREATE AGGREGATE FUNCTION myperlgroup RETURNS STRING SONAME "myperl.so";
** To remove it you need to do this:
** DROP FUNCTION myperlgroup;
**
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <my_global.h>
#include <mysql.h>
#include <EXTERN.h>
#include <perl.h>
#include "version.h"

#define USAGE "myperl() calls require at least one parameter, which is the perl code, all other parameters are passed to the script as ARGV. Version "VERSION
#define WATCHPOINT fprintf(stderr, "WATCHPOINT %s %d\n", __FILE__, __LINE__);

#ifdef HAVE_DLOPEN
#include <dlfcn.h>
#include "libperl_path.h"
static int lib_loaded = 0;
#endif


my_bool myperl_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
void myperl_deinit(UDF_INIT *initid);
char *myperl(UDF_INIT *initid, UDF_ARGS *args, char *result, 
			unsigned long *length, char *is_null, char *error);

EXTERN_C void xs_init (pTHXo);

extern char **environ;

typedef struct {
	char *returnable;
	size_t  size;
	PerlInterpreter *myperl;
	size_t  messagesize;
} myperl_passable;

my_bool myperl_global_init(UDF_INIT *initid, UDF_ARGS *args, char *message) 
{
	myperl_passable *pass = NULL;
	int exitstatus = 0;
	char *embedding[] = { "perl", "-MMyPerl::Request", "-e", "1" };
	PerlInterpreter *my_perl;
	uint i =0;

#ifdef HAVE_DLOPEN
	/* And yes there should be some locking going on here */
	if (!lib_loaded) {
		dlopen(LIBPERL_PATH"/CORE/libperl.so", RTLD_NOW | RTLD_GLOBAL);
	}
#endif
	/* We just set this above 255 so that we can return whatever we want */
	initid->max_length = 256;
	initid->maybe_null=1;

	/* If no arguements are passed it is an error */
	if (args->arg_count == 0 || args->arg_type[0] != STRING_RESULT) {
		strncpy(message,USAGE, MYSQL_ERRMSG_SIZE);
		return 1;
	}
	for (i=0 ; i < args->arg_count; i++)
		args->arg_type[i]=STRING_RESULT;

	pass = (myperl_passable *) malloc(sizeof(myperl_passable));;
	if (!pass) {
		strncpy(message, "Could not allocate memory", MYSQL_ERRMSG_SIZE);
		return 1;
	}

	if((my_perl = perl_alloc()) == NULL) {
		strncpy(message, "Could not allocate perl", MYSQL_ERRMSG_SIZE);
		return 1;
	}
	perl_construct(my_perl);
#ifdef HAVE_DLOPEN
	exitstatus = perl_parse(my_perl, xs_init, 4, embedding, environ);
#else
	exitstatus = perl_parse(my_perl, NULL, 4, embedding, environ);
#endif
	PL_exit_flags |= PERL_EXIT_DESTRUCT_END;
	if (exitstatus) {
		strncpy(message, "Error in creating perl parser", MYSQL_ERRMSG_SIZE);
		goto error;
	}

	exitstatus = perl_run(my_perl);
	if (exitstatus) {
		strncpy(message, "Error in parsing your perl", MYSQL_ERRMSG_SIZE);
		goto error;
	}

	pass->returnable = NULL;
	pass->size = 0;
	pass->messagesize = 0;
	pass->myperl = my_perl;
	initid->ptr = (char*)pass;

	return 0;

	error:

	perl_destruct(my_perl);
	perl_free(my_perl);
	free(pass);
	pass = NULL;

	return 1;
}

void myperl_global_add(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error, int groupby) 
{
	char *returnable = NULL;
	unsigned long x = 0;
	size_t size = 0;
	char *newspot = NULL;
	char *string = NULL;
	myperl_passable *pass = (myperl_passable *)initid->ptr ;
	STRLEN n_a; //Return strings length
	PerlInterpreter *my_perl = pass->myperl;

	dSP;   
	ENTER;
	SAVETMPS; 
	PUSHMARK(SP); 

	// First we push the script
	XPUSHs(sv_2mortal(newSVpvn(args->args[0], args->lengths[0])));

	// Now we push the value that we are tracking (if this is a group by)
	if (groupby)
		XPUSHs(sv_2mortal(newSVpvn(pass->returnable, pass->messagesize)));

	// Now we push the additional values into ARGV
fprintf(stderr, "ARGS %d\n", args->arg_count);
	for(x = 1; x < args->arg_count  ; x++) {
		XPUSHs(sv_2mortal(newSVpvn(args->args[x], args->lengths[x])));
	}
	PUTBACK;
	call_pv("MyPerl::Request::handler", G_SCALAR);
	SPAGAIN;
	string = POPpx;
	size = (size_t)n_a;
	if (size) {
		if(pass->size < size) {
			newspot = (char *)realloc(pass->returnable, size);
			if(!newspot) {
				error[0] = '1';
				returnable =  NULL;
				goto error;
			}
			pass->size = size;
			pass->returnable = newspot;
		}
		// Always know the current size, it may be less then the full size
		pass->messagesize = size;
		memcpy(pass->returnable, string, size);
	} else {
		is_null[0] = '1';
	}

	error:
	PUTBACK;
	FREETMPS;
	LEAVE;
}

my_bool myperl_init(UDF_INIT *initid, UDF_ARGS *args, char *message) 
{
	return myperl_global_init(initid, args, message);
}

void myperl_deinit(UDF_INIT *initid) 
{
	myperl_passable *pass = (myperl_passable *)initid->ptr ;

	perl_destruct(pass->myperl);
	perl_free(pass->myperl);
	free(pass->returnable);
	free(initid->ptr);
}

char * myperl(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error) 
{
	myperl_passable *pass = (myperl_passable *)initid->ptr ;

	//Set the message to zero
	pass->messagesize = 0;

	myperl_global_add(initid, args, is_null, error, 0);
	*length = pass->messagesize;

	return pass->returnable;
}

/* Average functions for GROUP BY */
my_bool myperlgroup_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
void myperlgroup_deinit(UDF_INIT *initid);
void myperlgroup_reset(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error);
void myperlgroup_add(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error);
char *myperlgroup(UDF_INIT *initid, UDF_ARGS *args, char *result, 
			unsigned long *length, char *is_null, char *error);

my_bool myperlgroup_init(UDF_INIT *initid, UDF_ARGS *args, char *message) 
{
	return myperl_global_init(initid, args, message);
}

void myperlgroup_deinit(UDF_INIT *initid) {
	myperl_deinit(initid);
}

void myperlgroup_reset(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error) 
{
	myperl_passable *pass = (myperl_passable *)initid->ptr ;

	fprintf(stderr, "myperlgroup_reset\n");

	pass->messagesize = 0;
	if(pass->returnable)
		memset(pass->returnable, pass->size, sizeof(char));
}

void myperlgroup_add(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error) 
{
	myperl_global_add(initid, args, is_null, error, 1);
}

char * myperlgroup(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error) 
{
	myperl_passable *pass = (myperl_passable *)initid->ptr;

	*length = pass->messagesize;

	return pass->returnable;
}


