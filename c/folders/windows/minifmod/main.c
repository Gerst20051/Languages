#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include "main.h"
#include ".\minifmod\interface.h"

#define TimerID		1000
#define dwWidth		240
#define lFontHeight	18

/** Prototypes **************************************************************/

static LRESULT CALLBACK MainDlgProc(HWND, UINT, WPARAM, LPARAM);

/** Global variables ********************************************************/

static HANDLE ghInstance;
FMUSIC_MODULE *mod;
HDC hDC;
HFONT hFont;
HDC hdcScroller;
HBITMAP hBmpScroller;
HDC hDC;
LOGFONT fnt = {lFontHeight, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial"};
LPCSTR szScroller="miniFMOD example for PellesC ... code: TBD ... music: waKa x ... thanks to: comrade (scroller), Pelle (C compiler), FairlightMultimedia (miniFMOD) ... press ESCAPE to exit";
RECT rcScrollerArea = {0, 0, dwWidth, 0};
RECT rcScroller;
PAINTSTRUCT ps;
signed long x=dwWidth;

/** WinMain *****************************************************************/

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	WNDCLASSEX wcx;

	ghInstance = hInstance;

	// Get system dialog information.
	wcx.cbSize = sizeof(wcx);
	if (!GetClassInfoEx(NULL, MAKEINTRESOURCE(32770), &wcx))
		return 0;

	// Add our own stuff.
	wcx.hInstance = hInstance;
	wcx.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDR_ICO_MAIN));
	wcx.lpszClassName = _T("MiniFMODClass");
	if (!RegisterClassEx(&wcx))
		return 0;

	// The user interface is a modal dialog box.
	DialogBox(hInstance, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)MainDlgProc);

	FMUSIC_StopSong(mod);
	FMUSIC_FreeSong(mod);
	FSOUND_Close();
	
	return(0);
}

static LRESULT CALLBACK MainDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
			case WM_INITDIALOG:
				// create font
				hFont = CreateFontIndirect(&fnt);
				hDC = GetDC(hwndDlg);
				// create back buffer
				hdcScroller = CreateCompatibleDC(hDC);
				SelectObject(hdcScroller, hFont);
				// get text height in pixels from device units
				rcScrollerArea.bottom = MulDiv(lFontHeight, GetDeviceCaps(hDC, LOGPIXELSY), 72); 				
				// calculate window size (client + caption + frame)
				int dwHeight = rcScrollerArea.bottom + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME);
				// resize window and center to screen
				MoveWindow(hwndDlg, (GetSystemMetrics(SM_CXSCREEN)-dwWidth)/2, (GetSystemMetrics(SM_CYSCREEN)-dwHeight)/2, dwWidth, dwHeight, 0);				
				SetBkMode(hdcScroller, OPAQUE);
				SetBkColor(hdcScroller, GetSysColor(COLOR_WINDOW));
				SetTextColor(hdcScroller, 0x000000);
				GetTextExtentPoint32(hdcScroller, szScroller, lstrlen(szScroller), (SIZE*)&rcScroller.right);
				hBmpScroller = CreateCompatibleBitmap(hDC, rcScroller.right, rcScrollerArea.bottom);
				SelectObject(hdcScroller, hBmpScroller);
				rcScroller.left = 0; rcScroller.top = 0; rcScroller.bottom = rcScrollerArea.bottom;
				// create black borders
				FillRect(hdcScroller, &rcScroller, GetStockObject(BLACK_BRUSH));
				// display texts
				DrawText(hdcScroller, szScroller, lstrlen(szScroller), &rcScroller, DT_SINGLELINE | DT_VCENTER | DT_EXPANDTABS);
				ReleaseDC(hwndDlg, hDC);
				SetTimer(hwndDlg, TimerID, 50, 0);

				// initialize miniFMOD file access routines (reading from resource)
				FSOUND_File_SetCallbacks(memopen, memclose, memread, memseek, memtell);

				// initialize miniFMOD
				if (!FSOUND_Init(44100, 0))
				{
					MessageBox(hwndDlg, "error:\t cannot init miniFmod engine","error", MB_ICONWARNING);
					return(FALSE);
				}

				// load song ( resource type: RCDATA name: "xm_file" )
				mod = FMUSIC_LoadSong("xm_file", NULL);
				if (!mod)
				{
					MessageBox(hwndDlg, "error:\t cannot load music file from resource","error", MB_ICONWARNING);
					FSOUND_Close();
					return(FALSE);
				}

				FMUSIC_PlaySong(mod);
				return TRUE;

			case WM_TIMER:
				x -= 2;
				if (x < 0 && abs(x) > rcScroller.right) x = rcScrollerArea.right;
				RedrawWindow(hwndDlg, 0, 0, RDW_INVALIDATE);
				break;

			case WM_PAINT:
				hDC = BeginPaint(hwndDlg, &ps);
				// erase background
				FillRect(hDC, &rcScrollerArea, GetSysColorBrush(COLOR_WINDOW));
				// display scroller
				BitBlt(hDC, 0, 0, dwWidth, rcScroller.bottom, hdcScroller, -x, 0, SRCCOPY);
				EndPaint(hwndDlg, &ps);
				break;

			case WM_COMMAND:
				switch (GET_WM_COMMAND_ID(wParam, lParam))
				{
					case IDCANCEL:
						EndDialog(hwndDlg, TRUE);
						return TRUE;
				}
				break;

			case WM_CLOSE:
				KillTimer(hwndDlg, TimerID);
				ReleaseDC(hwndDlg, hDC);
				EndDialog(hwndDlg, 0);
				return TRUE;
	}

	return FALSE;
}
