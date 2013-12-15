
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright 1993 - 2000 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/


#ifdef WIN16
#define APIENTRY FAR PASCAL
typedef WORD WPARAM;
//#else
//typedef DWORD WPARAM;
#endif

#define DRAWCLASS  "DRAWDEMO"


#define OBJ_RECTANGLE       0
#define OBJ_ELLIPSE         1
#define OBJ_ROUNDRECT       2
#define OBJ_CHORD           3
#define OBJ_PIE             4


typedef struct _DRAWDATA
{
    int    nObject;
} DRAWDATA;
typedef DRAWDATA      *PDRAWDATA;
typedef DRAWDATA NEAR *NPDRAWDATA;
typedef DRAWDATA FAR  *LPDRAWDATA;


/*
** DRAW WINDOW ROUTINES (draw.c)
*/
HWND  FAR      CreateDrawWindow(HWND,int);
LRESULT  CALLBACK DrawProc(HWND,UINT,WPARAM,LPARAM);
BOOL           DrawCreateProc(HWND);
VOID           DrawDestroyProc(HWND);
BOOL           DrawCommandProc(HWND,WPARAM,LPARAM);
VOID           DrawPaintProc(HWND);


VOID           DrawObject(HWND);
