/*************************************************************
 *  Mpg Puzzle Game - Ver : 0.02 - PK4BCX2 ( 2004 )          *
 *                                                           *
 *  Public Domain, no warranty given, use at your own risk.  *
 *  This code must retain this message.                      *
 *************************************************************/

#include <windows.h>
#include <windowsx.h>
#include "mpgres.h"
#include "globals.h"


char szChildClass[] = "Mpg_Child" ;

//---------------------------------------------------------------------------------------------

static BOOL InitApplication(void)
{

     WNDCLASSEX  wndclass ;

     memset(&wndclass,0,sizeof(WNDCLASSEX));

	 wndclass.cbSize        = sizeof (wndclass) ;
     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = MainWndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInst ;
     wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = MAKEINTRESOURCE(IDMAINMENU);
     wndclass.lpszClassName = "mpgWndClass";
	 wndclass.hIconSm       = LoadIcon(hInst, MAKEINTRESOURCE(IDI_MPGICON));

     RegisterClassEx (&wndclass) ;

     wndclass.lpfnWndProc   = ChildWndProc ;
     wndclass.cbWndExtra    = sizeof (WORD) ;
     wndclass.hIcon         = NULL ;
     wndclass.lpszClassName = szChildClass ;
;

	 wndclass.hIconSm       = NULL ;

     RegisterClassEx (&wndclass) ;

	// ---TODO--- Call module specific initialization routines here

	return 1;
}

//---------------------------------------------------------------------------------------------

HWND CreatempgWndClassWnd(void)
{
	return CreateWindow("mpgWndClass"," MPG Puzzle Game",
		WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_CAPTION|WS_TILED|WS_SYSMENU,
		0,0,300,300,
		NULL,
		NULL,
		hInst,
		NULL);
}

//---------------------------------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
	MSG msg;
	HANDLE hAccelTable;

	hInst = hInstance;
	if (!InitApplication())
		return 0;
	hAccelTable = LoadAccelerators(hInst,MAKEINTRESOURCE(IDACCEL));
	if ((hwndMain = CreatempgWndClassWnd()) == (HWND)0)
		return 0;

	FormLoad();

	while (GetMessage(&msg,NULL,0,0)) {
		if (!TranslateAccelerator(msg.hwnd,hAccelTable,&msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}
