/****************************************************************************
 *                                                                          *
 * File    : main.c                                                         *
 *                                                                          *
 * Purpose : Generic dialog based Win32 application.                        *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <tchar.h>
#include "main.h"
#include "hyperlink.h"

/** Prototypes **************************************************************/

static LRESULT CALLBACK MainDlgProc(HWND, UINT, WPARAM, LPARAM);
void HypCtrlCallback(DWORD HypCtrlID);

/** Global variables ********************************************************/

static HANDLE ghInstance;

/****************************************************************************
 *                                                                          *
 * Function: WinMain                                                        *
 *                                                                          *
 * Purpose : Initialize the application.  Register a window class,          *
 *           create and display the main window and enter the               *
 *           message loop.                                                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    WNDCLASSEX wcx;
    
    ghInstance = hInstance;

    wcx.cbSize = sizeof(wcx);
    if (!GetClassInfoEx(NULL, MAKEINTRESOURCE(32770), &wcx)) return 0;
    wcx.hInstance = hInstance;
    wcx.lpszClassName = _T("HLinkClass");
    if (!RegisterClassEx(&wcx)) return 0;

    return DialogBox(hInstance, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)MainDlgProc);
}

/****************************************************************************
 *                                                                          *
 * Function: MainDlgProc                                                    *
 *                                                                          *
 * Purpose : Process messages for the Main dialog.                          *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static LRESULT CALLBACK MainDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    switch (uMsg)
    {
        case WM_INITDIALOG: {

			HYPCTRL hc; // can be reused for new hyperlink controls
			
			InitHypCtrl(&hc);
			hc.ulStyle	 = ulHover;
			hc.szLink	 = _T("http://smorgasbordet.com/pellesc");
			hc.szTooltip = _T("Visit Pelles C Website");
			CreateHypCtrl(hwndDlg, &hc, 10, 10, 0, 0);

			InitHypCtrl(&hc);
			hc.ulStyle	 = ulHover;
			hc.szLink	 = _T("mailto:someone@somewhere.net");
			hc.szText	 = _T("someone@somewhere.net");
			hc.szTooltip = _T("Send program author a mail");
			CreateHypCtrl(hwndDlg, &hc, 10, 30, 0, 0);
			
			InitHypCtrl(&hc);
			hc.ulStyle	  = ulHover;
			hc.bPlaySound = FALSE;
			hc.szText     = _T("Callback Test 1");
			hc.cbFn       = HypCtrlCallback;
			hc.cbID       = 1;
			CreateHypCtrl(hwndDlg, &hc, 10, 50, 0, 0);

			InitHypCtrl(&hc);
			hc.ulStyle    = ulHover;
			hc.bPlaySound = FALSE;
			hc.szText     = _T("Callback Test 2");
			hc.cbFn	      = HypCtrlCallback;
			hc.cbID       = 2;
			CreateHypCtrl(hwndDlg, &hc, 10, 70, 0, 0);

            return TRUE;
        }
        case WM_COMMAND:
            switch (GET_WM_COMMAND_ID(wParam, lParam))
            {
                case IDOK:
                    EndDialog(hwndDlg, TRUE);
                    return TRUE;
            }
            break;
        case WM_CLOSE:
            EndDialog(hwndDlg, 0);
            return TRUE;
    }
    return FALSE;
}

/****************************************************************************
 *                                                                          *
 * Function: HypCtrlCallback                                                *
 *                                                                          *
 * Purpose : Will be called from second hyperlink control.                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/
void HypCtrlCallback(DWORD HypCtrlID)
{
	switch(HypCtrlID) {
		case 1:
			MessageBox(NULL, _T("Callback Test 1 clicked!"), _T("Test"), MB_OK);
			break;
		case 2:
			MessageBox(NULL, _T("Callback Test 2 clicked!"), _T("Test"), MB_OK);
			break;
	}
}
