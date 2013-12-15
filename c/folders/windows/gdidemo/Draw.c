/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples.
*       Copyright 1993 - 2000 Microsoft Corporation.
*       All rights reserved.
*       This source code is only intended as a supplement to
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the
*       Microsoft samples programs.
\******************************************************************************/

/*---------------------------------------------------------------------------*\
| RANDOM OBJECTS MODULE
\*---------------------------------------------------------------------------*/

#include <windows.h>
#include <stdio.h>
#include "gdidemo.h"
#include "draw.h"

/*---------------------------------------------------------------------------*\
| CREATE DRAW WINDOW PROCEDURE
\*---------------------------------------------------------------------------*/
HWND FAR CreateDrawWindow(HWND hWndClient, int nItem)
{
    HANDLE          hInstance;
    MDICREATESTRUCT mcs;
    static char buffer[256];


    hInstance = GETINSTANCE(hWndClient);
    LoadString (hInstance, DRAWTITLE, buffer, 256);

    /*
    ** Initialize the MDI create struct for creation of the
    ** test window.
    */
    mcs.szClass = DRAWCLASS;
    mcs.szTitle = buffer;
    mcs.hOwner  = hInstance;
    mcs.x       = CW_USEDEFAULT;
    mcs.y       = CW_USEDEFAULT;
    mcs.cx      = CW_USEDEFAULT;
    mcs.cy      = CW_USEDEFAULT;
    mcs.style   = 0l;
    mcs.lParam  = (LONG)nItem;

    return((HWND)SendMessage(hWndClient,WM_MDICREATE,0,(LPARAM)(LPMDICREATESTRUCT)&mcs));
}


/*---------------------------------------------------------------------------*\
| DRAW WINDOW PROCEDURE
\*---------------------------------------------------------------------------*/
LRESULT CALLBACK DrawProc(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
    switch(wMsg)
    {
        case WM_CREATE:
            DrawCreateProc(hWnd);
            break;


        case WM_COMMAND:
            DrawCommandProc(hWnd,wParam,lParam);
            break;


        case WM_TIMER:
            DrawObject(hWnd);
            break;


        case WM_PAINT:
            DrawPaintProc(hWnd);
            break;


        case WM_DESTROY:
            DrawDestroyProc(hWnd);
            break;


        default:
            return(DefMDIChildProc(hWnd,wMsg,wParam,lParam));
    }
    return(0l);
}


/*---------------------------------------------------------------------------*\
| DRAW CREATE PROCEDURE
\*---------------------------------------------------------------------------*/
BOOL DrawCreateProc(HWND hWnd)
{
    PDRAWDATA pdd;


    if(AllocWindowInfo(hWnd,sizeof(DRAWDATA)))
    {
        if(pdd = (PDRAWDATA)LockWindowInfo(hWnd))
        {
            pdd->nObject = 0;

            UnlockWindowInfo(hWnd);

            SetTimer(hWnd,1,50,NULL);
            return(TRUE);
        }
        FreeWindowInfo(hWnd);
    }
    return(FALSE);
}


/*---------------------------------------------------------------------------*\
| DRAW COMMAND PROCEDURE
\*---------------------------------------------------------------------------*/
BOOL DrawCommandProc(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    hWnd   = hWnd;
    wParam = wParam;
    lParam = lParam;

    return(TRUE);
}


/*---------------------------------------------------------------------------*\
| DRAW PAINT PROCEDURE
\*---------------------------------------------------------------------------*/
VOID DrawPaintProc(HWND hWnd)
{
    HDC         hDC;
    PAINTSTRUCT ps;


    if(hDC = BeginPaint(hWnd,&ps))
        EndPaint(hWnd,&ps);

    return;
}


/*---------------------------------------------------------------------------*\
| DRAW DESTROY PROCEDURE
\*---------------------------------------------------------------------------*/
VOID DrawDestroyProc(HWND hWnd)
{
    KillTimer(hWnd,1);
    FreeWindowInfo(hWnd);
    return;
}


VOID DrawObject(HWND hWnd)
{
    PDRAWDATA pdd;
    RECT      rect;
    HDC       hDC;
    int       x1,y1,x2,y2,x3,y3,x4,y4,r,g,b,nObject;
    HBRUSH    hBrush;
    char      szDebug[80];


    if(pdd = (PDRAWDATA)LockWindowInfo(hWnd))
    {
        if(hDC = GetDC(hWnd))
        {
            GetClientRect(hWnd,&rect);

            // avoid divide by zero errors when the window is small.
            if ( rect.right== 0) rect.right++;
            if ( rect.bottom== 0) rect.bottom++;

            r = lRandom() % 255;
            g = lRandom() % 255;
            b = lRandom() % 255;

            if(hBrush = SelectObject(hDC,CreateSolidBrush(RGB(r,g,b))))
            {
                x1 = lRandom() % rect.right;
                y1 = lRandom() % rect.bottom;
                x2 = lRandom() % rect.right;
                y2 = lRandom() % rect.bottom;
                x3 = lRandom() % rect.right;
                y3 = lRandom() % rect.bottom;
                x4 = lRandom() % rect.right;
                y4 = lRandom() % rect.bottom;


                nObject = lRandom() % 5;

                switch(nObject)
                {
                    default:
                    case OBJ_RECTANGLE:
                        snprintf(szDebug,sizeof(szDebug)-1,"Rectangle(%d,%d,%d,%d)\n",x1,y1,x2,y2);
                        szDebug[sizeof(szDebug)-1] = '\0';

                        DEBUGOUT(szDebug);
                        Rectangle(hDC,x1,y1,x2,y2);
                        break;

                    case OBJ_ELLIPSE:
                        snprintf(szDebug,sizeof(szDebug)-1,"Ellipse(%d,%d,%d,%d)\n",x1,y1,x2,y2);
                        szDebug[sizeof(szDebug)-1] = '\0';

                        DEBUGOUT(szDebug);
                        Ellipse(hDC,x1,y1,x2,y2);
                        break;



                    case OBJ_ROUNDRECT:
                        snprintf(szDebug,sizeof(szDebug)-1,"RoundRect(%d,%d,%d,%d,%d,%d)\n",x1,y1,x2,y2,x3,y3);
                        szDebug[sizeof(szDebug)-1] = '\0';

                        DEBUGOUT(szDebug);
                        RoundRect(hDC,x1,y1,x2,y2,x3,y3);
                        break;

                    case OBJ_CHORD:
                        snprintf(szDebug,sizeof(szDebug)-1,"Chord(%d,%d,%d,%d,%d,%d,%d,%d)\n",x1,y1,x2,y2,x3,y3,x4,y4);
                        szDebug[sizeof(szDebug)-1] = '\0';

                        DEBUGOUT(szDebug);
                        Chord(hDC,x1,y1,x2,y2,x3,y3,x4,y4);
                        break;

                    case OBJ_PIE:
                        snprintf(szDebug,sizeof(szDebug)-1,"Pie(%d,%d,%d,%d,%d,%d,%d,%d)\n",x1,y1,x2,y2,x3,y3,x4,y4);
                        szDebug[sizeof(szDebug)-1] = '\0';

                        DEBUGOUT(szDebug);
                        Pie(hDC,x1,y1,x2,y2,x3,y3,x4,y4);
                        break;

                }

                DeleteObject(SelectObject(hDC,hBrush));
            }
            ReleaseDC(hWnd,hDC);
        }
        UnlockWindowInfo(hWnd);
    }
    return;
}
