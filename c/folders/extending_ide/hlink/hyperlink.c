/****************************************************************************
 *                                                                          *
 * File    : hyperlink.c                                                    *
 *                                                                          *
 * Author  : Alexander Stoica (Berlin, Germany) nitex@freenet.de            *
 *                                                                          *
 * Version : 0.2                                                            *
 *                                                                          *
 * Purpose : Hyperlink Control                                              *
 *                                                                          *
 * History : The first version was written for LCC-Win32, i've updated      *
 *           the following stuff:                                           *
 *                                                                          * 
 *           - Added unicode support.                                       *
 *           - Hand cursor will not be loaded from "winhlp32.exe" anymore,  *
 *             instead i use "LoadCursor(NULL, IDC_HAND);", which requires  *
 *             Win98. Read "hyperlink.html" if you need to support Win95.   *
 *           - The click sound is loaded from registry now.                 *
 *           - Fixed bug with WinXP designs activated.                      *
 *           - Fixed bug where hyperlink control stayed in hover mode.      *
 *           - Support for user defined callback function.                  *
 *                                                                          *
 * Special thanks to John Findlay, who helped me with the first version of  *
 * the hyperlink control. Visit his site at:                                *
 *                                                                          *
 * http://www.john.findlay1.btinternet.co.uk                                *
 *                                                                          *
 ****************************************************************************/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <tchar.h>
#include <shellapi.h>
#include <mmsystem.h>
#include "hyperlink.h"

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "winmm.lib")

// offsets for the window extra space
#define WND_HYPSTRUCT 0
#define WND_ISHOVER 4
#define WND_LEFT 6
#define WND_TOP 10
#define WND_RIGHT 14
#define WND_BOTTOM 18
#define WND_FONTN 22
#define WND_FONTH 26

// used for calculating the size of a string in bytes including the terminating null character
#define STRBYTESIZE(a) (GetStringLength(a) * sizeof(TCHAR) + sizeof(TCHAR))

/** Prototypes **************************************************************/

static LRESULT CALLBACK HypCtrlWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
static UINT GetStringLength(LPTSTR szString);
static void PlayNavigatingSound(void);

/** Global variables ********************************************************/

static HINSTANCE hInst = NULL;
static LPTSTR lpszClassname = _T("HypCtrlClass");

/****************************************************************************
 *                                                                          *
 * Function: InitHypCtrl                                                    *
 *                                                                          *
 * Purpose : Register Hyperlink class and initialize hyperlink structure    *
 *           with default values.                                           *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/
BOOL InitHypCtrl(HYPCTRL* pHc)
{
    WNDCLASS wc;
    
	hInst = GetModuleHandle(NULL);
	
	if (!GetClassInfo(hInst, lpszClassname, &wc)) {

		ZeroMemory(&wc, sizeof(wc));
	    wc.style         = CS_HREDRAW | CS_VREDRAW;
    	wc.lpfnWndProc   = (WNDPROC) HypCtrlWndProc;
    	wc.cbClsExtra    = 0;
    	wc.cbWndExtra    = 32;
    	wc.hInstance     = hInst;
    	wc.hbrBackground = (HBRUSH) (HOLLOW_BRUSH);
    	wc.lpszMenuName  = NULL;
    	wc.hIcon         = NULL;
    	wc.hCursor       = LoadCursor(NULL, IDC_HAND); //requires win98 or higher
    	wc.lpszClassName = lpszClassname;

    	if (!RegisterClass(&wc)) return FALSE;

		// initialize common controls for the tooltips
    	INITCOMMONCONTROLSEX icex;
		icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
		icex.dwICC  = ICC_BAR_CLASSES;
		InitCommonControlsEx(&icex); 
	}

	pHc->szLink	    = NULL;
	pHc->szText	    = NULL;
	pHc->szTooltip  = NULL;
	pHc->fgcNormal  = RGB(0, 0, 255);
	pHc->fgcHover   = RGB(255, 0, 0);
	pHc->bgcNormal  = (COLORREF) GetSysColor(COLOR_3DFACE);
	pHc->bgcHover   = (COLORREF) GetSysColor(COLOR_3DFACE);
	pHc->ulStyle    = ulAlways;
	pHc->dtStyle    = DT_LEFT | DT_TOP;
	pHc->bAutoSize 	= TRUE;
	pHc->bPlaySound = TRUE;
	pHc->cbFn 		= NULL;
	pHc->cbID		= 0;
	
    return TRUE;
}

/****************************************************************************
 *                                                                          *
 * Function: CreateHypCtrl                                                  *
 *                                                                          *
 * Purpose : Create Hyperlink control.                                      *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/
HWND CreateHypCtrl(HWND hWndParent, HYPCTRL* pHc, int x, int y, int width, int height)
{
	HYPCTRL* pHcWnd;
	HWND hWndHyperlink;
	RECT rect;
	LOGFONT lf;
	HFONT hFontParent, hFontNormal, hFontHover;

	// some error checking
	if (hInst == NULL) return (HWND) NULL;
	if (hWndParent == NULL) return (HWND) NULL;

	if (pHc->cbFn == NULL) {
		// if the hyperlink opens a normal link, require the szLink member
		if (pHc->szLink == NULL) return (HWND) NULL;
	} else {
		// if the hyperlink calls a callback function, require the szText member
		if (pHc->szText == NULL) return (HWND) NULL;
	}

	hWndHyperlink = CreateWindow(lpszClassname,
						NULL,
						WS_CHILD,
						x, y, width, height,
						hWndParent,
						NULL,
						hInst,
						NULL);

	if (!hWndHyperlink) return (HWND) NULL;

	// alloc some space to hold all hyperlink stuff
	if ((pHcWnd = GlobalAllocPtr(GPTR, sizeof(HYPCTRL))) == NULL) {
		DestroyWindow(hWndHyperlink);
		return (HWND) NULL;
	}
	CopyMemory(pHcWnd, pHc, sizeof(HYPCTRL));

	// copy the text entries from HYPCTRL
	if (pHcWnd->cbFn == NULL) {
		if (pHcWnd->szLink != NULL) {
			if ((pHcWnd->szLink = (LPTSTR) GlobalAllocPtr(GPTR, STRBYTESIZE(pHc->szLink))) == NULL) {
				if (pHcWnd) GlobalFreePtr(pHcWnd);
				DestroyWindow(hWndHyperlink);
				return (HWND) NULL;
			}
			CopyMemory(pHcWnd->szLink, pHc->szLink, STRBYTESIZE(pHc->szLink));
		}
	}

	if (pHcWnd->szText != NULL) {
		if ((pHcWnd->szText = GlobalAllocPtr(GPTR, STRBYTESIZE(pHc->szText))) == NULL) {
			if (pHcWnd->szLink) GlobalFreePtr(pHcWnd->szLink);
			if (pHcWnd) GlobalFreePtr(pHcWnd);
			DestroyWindow(hWndHyperlink);
			return (HWND) NULL;
		}
		CopyMemory(pHcWnd->szText, pHc->szText, STRBYTESIZE(pHc->szText));
	} else {
		// no callback function and no szText given so we use szLink as text
		if(pHcWnd->cbFn == NULL) pHcWnd->szText = pHcWnd->szLink;
	}

	if (pHcWnd->szTooltip != NULL) {
		if ((pHcWnd->szTooltip = GlobalAllocPtr(GPTR, STRBYTESIZE(pHc->szTooltip))) == NULL) {
			if (pHcWnd->szLink) GlobalFreePtr(pHcWnd->szLink);
			if (pHcWnd->szText) GlobalFreePtr(pHcWnd->szText);
			if (pHcWnd) GlobalFreePtr(pHcWnd);
			DestroyWindow(hWndHyperlink);
			return (HWND) NULL;
		}
		CopyMemory(pHcWnd->szTooltip, pHc->szTooltip, STRBYTESIZE(pHc->szTooltip));
	}

	// save a pointer to the structure in the window extra space
	SetWindowLong(hWndHyperlink, WND_HYPSTRUCT, (LONG) (pHcWnd));
	SetWindowWord(hWndHyperlink, WND_ISHOVER, (WORD) FALSE);

	// create the fonts for the control
	hFontParent = (HFONT) SendMessage(GetParent(hWndHyperlink), WM_GETFONT, 0, 0);

	if (hFontParent != NULL) {
		GetObject(hFontParent, sizeof(LOGFONT), &lf);

		switch (pHcWnd->ulStyle) {
			case ulHover:
				hFontNormal = CreateFontIndirect(&lf);
				lf.lfUnderline = (BYTE) TRUE;
				hFontHover  = CreateFontIndirect(&lf);
				break;
			case ulAlways:
				lf.lfUnderline = (BYTE) TRUE;
				hFontNormal = CreateFontIndirect(&lf);
				hFontHover  = CreateFontIndirect(&lf);
				break;
			case ulNone:
				hFontNormal = CreateFontIndirect(&lf);
				hFontHover  = CreateFontIndirect(&lf);
				break;
			}
			// save the fonts in the window extra space
			SetWindowLong(hWndHyperlink, WND_FONTN, (LONG) hFontNormal);
			SetWindowLong(hWndHyperlink, WND_FONTH, (LONG) hFontHover);
	} else {
		// use the system font
		SetWindowLong(hWndHyperlink, WND_FONTN, (LONG) NULL);
		SetWindowLong(hWndHyperlink, WND_FONTH, (LONG) NULL);
	}

	GetClientRect(hWndHyperlink, &rect);

	// adjust window size to fit the text
	if (pHcWnd->bAutoSize) {
		PAINTSTRUCT ps;
		HDC hdc;
		SIZE size;

		hdc	= BeginPaint(hWndHyperlink, &ps);
 		SelectObject(hdc, (HFONT) GetWindowLong(hWndHyperlink, WND_FONTN));
		GetTextExtentPoint32(hdc, pHcWnd->szText, GetStringLength(pHcWnd->szText), &size);
		rect.right	= size.cx - rect.left;
		rect.bottom	= size.cy - rect.top;
		EndPaint(hWndHyperlink, &ps);

		SetWindowPos(hWndHyperlink,
			0,
			0, 0, size.cx, size.cy,
			SWP_NOMOVE | SWP_NOZORDER);
	}

	// save window size in the window extra space
	SetWindowLong(hWndHyperlink, WND_LEFT, (LONG) (rect.left));
	SetWindowLong(hWndHyperlink, WND_TOP, (LONG) (rect.top));
	SetWindowLong(hWndHyperlink, WND_RIGHT, (LONG) (rect.right));
	SetWindowLong(hWndHyperlink, WND_BOTTOM, (LONG) (rect.bottom));

	// create tooltip if requested
	if (pHcWnd->szTooltip != NULL) {
    	HWND hWndTT;
    	TOOLINFO ti;

    	hWndTT = CreateWindowEx(WS_EX_TOPMOST,
    		TOOLTIPS_CLASS,
    		NULL,
    		WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
    		CW_USEDEFAULT,
    		CW_USEDEFAULT,
    		CW_USEDEFAULT,
    		CW_USEDEFAULT,
    		hWndHyperlink,
    		NULL,
    		hInst,
    		NULL);
    				
    	if (!hWndTT) return FALSE;

    	SetWindowPos(hWndTT,
    		HWND_TOPMOST,
    		0, 0, 0, 0,
    		SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

    	ti.cbSize      = sizeof(TOOLINFO);
    	ti.uFlags      = TTF_SUBCLASS;
    	ti.hwnd        = hWndHyperlink;
    	ti.hinst       = hInst;
    	ti.uId         = 0;
    	ti.lpszText    = pHcWnd->szTooltip; // get text for tooltip
    	ti.rect.left   = (LONG) GetWindowLong(hWndHyperlink, WND_LEFT);
    	ti.rect.top    = (LONG) GetWindowLong(hWndHyperlink, WND_TOP);
    	ti.rect.right  = (LONG) GetWindowLong(hWndHyperlink, WND_RIGHT);
    	ti.rect.bottom = (LONG) GetWindowLong(hWndHyperlink, WND_BOTTOM);

    	// add tooltip
    	SendMessage(hWndTT, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ti);
	}

	// show the window
	ShowWindow(hWndHyperlink, SW_NORMAL);
	UpdateWindow(hWndHyperlink);

	return hWndHyperlink;
}

/****************************************************************************
 *                                                                          *
 * Function: HypCtrlWndProc                                                 *
 *                                                                          *
 * Purpose : Process Hyperlink control messages.                            *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/
static LRESULT CALLBACK HypCtrlWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg){
		case WM_LBUTTONUP: {
			HYPCTRL* pHcWnd = (HYPCTRL*) GetWindowLong(hWnd, WND_HYPSTRUCT);

			if (pHcWnd->bPlaySound) PlayNavigatingSound();
		
			if (pHcWnd->cbFn != NULL) {
				// call user function (callback)
				pHcWnd->cbFn(pHcWnd->cbID);
			} else {
				// jump to link
				ShellExecute(NULL, _T("open"), pHcWnd->szLink, NULL, NULL, SW_SHOW);
			}
			return FALSE;
		}
		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc;
			RECT rect;

			HYPCTRL* pHcWnd     = (HYPCTRL*) GetWindowLong(hWnd, WND_HYPSTRUCT);
			BOOL bIsOverControl = (BOOL) GetWindowWord(hWnd, WND_ISHOVER);

			rect.left	= (LONG) GetWindowLong(hWnd, WND_LEFT);
			rect.top	= (LONG) GetWindowLong(hWnd, WND_TOP);
			rect.right	= (LONG) GetWindowLong(hWnd, WND_RIGHT);
			rect.bottom	= (LONG) GetWindowLong(hWnd, WND_BOTTOM);

			hdc = BeginPaint(hWnd, &ps);

			if (bIsOverControl) {
				// draws hover state
     		    SetBkColor(hdc, (COLORREF) pHcWnd->bgcHover);
				SelectObject(hdc, (HFONT) GetWindowLong(hWnd, WND_FONTH));
				SetTextColor(hdc, pHcWnd->fgcHover);
			} else {
				// draws normal state
       		    SetBkColor(hdc, (COLORREF) pHcWnd->bgcNormal);
				SelectObject(hdc, (HFONT) GetWindowLong(hWnd, WND_FONTN));
				SetTextColor(hdc, pHcWnd->fgcNormal);
			}
			// draw the text
			DrawText(hdc, pHcWnd->szText, -1, &rect, pHcWnd->dtStyle);
            
			EndPaint(hWnd, &ps);
			return FALSE;
		}
		case WM_CAPTURECHANGED:
			SetWindowWord(hWnd, WND_ISHOVER, (WORD) FALSE);
			InvalidateRect(hWnd, NULL, TRUE);
			return FALSE;
		case WM_MOUSEMOVE: {
			RECT rect;

			rect.left	= (LONG) GetWindowLong(hWnd, WND_LEFT);
			rect.top	= (LONG) GetWindowLong(hWnd, WND_TOP);
			rect.right	= (LONG) GetWindowLong(hWnd, WND_RIGHT);
			rect.bottom	= (LONG) GetWindowLong(hWnd, WND_BOTTOM);
			
			// check if mouse is over control
			if ((BOOL) GetWindowWord(hWnd, WND_ISHOVER)) {
				POINT ptMousePos;
				DWORD dwMousePos;

				dwMousePos = GetMessagePos();

				ptMousePos.x = LOWORD(dwMousePos);
				ptMousePos.y = HIWORD(dwMousePos);

				ScreenToClient(hWnd, &ptMousePos);

				if (!(BOOL) PtInRect(&rect, ptMousePos)) {
					SetWindowWord(hWnd, WND_ISHOVER, (WORD) FALSE);
					InvalidateRect(hWnd, &rect, TRUE);
				   	ReleaseCapture();
				}
			} else {
				SetWindowWord(hWnd, WND_ISHOVER, (WORD) TRUE);
				InvalidateRect(hWnd, &rect, TRUE);
				SetCapture(hWnd);
			}
			return FALSE;
		}
		case WM_CLOSE: {
			HYPCTRL* pHcWnd = (HYPCTRL*) GetWindowLong(hWnd, WND_HYPSTRUCT);
			HFONT hFontTemp;

			// clean up
			if (pHcWnd->szLink) GlobalFreePtr(pHcWnd->szLink);
			if (pHcWnd->szText) GlobalFreePtr(pHcWnd->szText);
			if (pHcWnd->szTooltip) GlobalFreePtr(pHcWnd->szTooltip);
			if (pHcWnd->cbFn) GlobalFreePtr(pHcWnd->cbFn);
			
			hFontTemp = (HFONT) GetWindowLong(hWnd, WND_FONTN);
			if (hFontTemp) DeleteObject(hFontTemp);

			hFontTemp = (HFONT) GetWindowLong(hWnd, WND_FONTH);
			if (hFontTemp) DeleteObject(hFontTemp);

			if (pHcWnd) GlobalFreePtr(pHcWnd);
			DestroyWindow(hWnd);
			return FALSE;
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/****************************************************************************
 *                                                                          *
 * Function: GetStringLength                                                *
 *                                                                          *
 * Purpose : Get string length in characters.                               *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/
static UINT GetStringLength(LPTSTR szString)
{
	// check for terminating null character and return length in characters
	if (_tcschr(szString, 0)) return lstrlen(szString);
	return 0;
}

/****************************************************************************
 *                                                                          *
 * Function: PlayNavigatingSound                                            *
 *                                                                          *
 * Purpose : Play system navigating sound.                                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/
static void PlayNavigatingSound(void)
{
	HKEY hKey = NULL;
	ULONG ulBufferSize = MAX_PATH + sizeof(TCHAR);
	LPTSTR lpszBuffer = GlobalAllocPtr(GPTR, ulBufferSize);
	LPTSTR lpszSoundPath = GlobalAllocPtr(GPTR, ulBufferSize);

	if (RegOpenKeyEx(HKEY_CURRENT_USER, _T("AppEvents\\Schemes\\Apps\\Explorer\\Navigating\\.Current"), 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS) {
		if (RegQueryValueEx(hKey, NULL, 0, NULL, (LPBYTE) lpszBuffer, &ulBufferSize) == ERROR_SUCCESS) {
			ExpandEnvironmentStrings(lpszBuffer, lpszSoundPath, ulBufferSize);
			PlaySound(lpszSoundPath, NULL, SND_ASYNC | SND_NODEFAULT | SND_NOWAIT);
		}
   		if(hKey) RegCloseKey(hKey);
	}

	if(lpszBuffer) GlobalFreePtr(lpszBuffer);
	if(lpszSoundPath) GlobalFreePtr(lpszSoundPath);
}
