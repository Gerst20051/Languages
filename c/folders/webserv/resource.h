/*

  RESOURCE.H
  ==========
  (c) Copyright Paul Griffiths 1999
  Email: mail@paulgriffiths.net

  Interface to functions for returning a resource.

*/


#ifndef PG_RESOURCE_H
#define PG_RESOURCE_H


#include "reqhead.h"         /*  for struct ReqInfo  */


/*  Function prototypes  */

int Return_Resource (int conn, int resource, struct ReqInfo * reqinfo);
int Check_Resource  (struct ReqInfo * reqinfo);
int Return_Error_Msg(int conn, struct ReqInfo * reqinfo);


#endif  /*  PG_RESOURCE_H  */
