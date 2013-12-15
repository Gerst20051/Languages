/*

  RESPHEAD.H
  ==========
  (c) Copyright Paul Griffiths 1999
  Email: mail@paulgriffiths.net

  Interface to HTTP response header functions

*/


#ifndef PG_RESPHEAD_H
#define PG_RESPHEAD_H


#include "reqhead.h"          /*  for struct ReqInfo  */


/*  Function prototypes  */

int Output_HTTP_Headers(int conn, struct ReqInfo * reqinfo);


#endif  /*  PG_RESPHEAD_H  */
