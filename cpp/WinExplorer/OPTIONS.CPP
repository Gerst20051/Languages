#include <windows.h>
#include <Commctrl.h>
#include <tlhelp32.h>
#include "general.h"
#include "options.h"
#include "resource.h"

extern HINSTANCE hInst;

//This module handles the "Options" dialog box.

void EnableControls(HWND hwndDlg)
{
	EnableWindow(GetDlgItem(hwndDlg, IDC_CATCH_HOTKEY), GetCheck(hwndDlg, IDC_CATCHWIN));
	EnableWindow(GetDlgItem(hwndDlg, IDC_REFRESH_SECONDS), GetCheck(hwndDlg, IDC_AUTO_REFRESH));
}

void OptionsToDialog(HWND hwndDlg)
{
	CGeneralAppOptions *pAppOptions;
	pAppOptions = GetAppOptions();
	SetCheck(hwndDlg, IDC_HIDE_TOP_HIDDEN_WINDOWS, pAppOptions->m_bHideTopHiddenWindows);
	SetCheck(hwndDlg, IDC_FULL_TREE, pAppOptions->m_bFullTree);
	SetCheck(hwndDlg, IDC_CATCHWIN, pAppOptions->m_bCatchWinByMousePos);
	SetCheck(hwndDlg, IDC_USE_APP_ICONS, pAppOptions->m_bUseAppIcons);
	SetCheck(hwndDlg, IDC_AUTO_REFRESH, pAppOptions->m_bAutoRefresh);
	
	SetDlgItemInt(hwndDlg, IDC_REFRESH_SECONDS, pAppOptions->m_dwRefreshSeconds, FALSE);
	SendDlgItemMessage(hwndDlg, IDC_CATCH_HOTKEY, HKM_SETHOTKEY, pAppOptions->m_dwCatchWinByMousePosKey, 0); 
	EnableControls(hwndDlg);
}


void DialogToOptions(HWND hwndDlg)
{
	CGeneralAppOptions *pAppOptions;
	pAppOptions = GetAppOptions();
	pAppOptions->m_bHideTopHiddenWindows = GetCheck(hwndDlg, IDC_HIDE_TOP_HIDDEN_WINDOWS);
	pAppOptions->m_bFullTree = GetCheck(hwndDlg, IDC_FULL_TREE);
	pAppOptions->m_bCatchWinByMousePos = GetCheck(hwndDlg, IDC_CATCHWIN);
	pAppOptions->m_dwCatchWinByMousePosKey = SendDlgItemMessage(hwndDlg, IDC_CATCH_HOTKEY, HKM_GETHOTKEY, 0, 0); 
	pAppOptions->m_bUseAppIcons = GetCheck(hwndDlg, IDC_USE_APP_ICONS);
	pAppOptions->m_bAutoRefresh = GetCheck(hwndDlg, IDC_AUTO_REFRESH);
	pAppOptions->m_dwRefreshSeconds = GetDlgItemInt(hwndDlg, IDC_REFRESH_SECONDS, NULL, FALSE);
	if (pAppOptions->m_dwRefreshSeconds <= 0) pAppOptions->m_dwRefreshSeconds = 10; 
	pAppOptions->SaveToReg();
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////Options main dialog procedure///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

BOOL CALLBACK OptionsDialogProc(
    HWND  hwndDlg,	// handle to dialog box
    UINT  uMsg,	// message
    WPARAM  wParam,	// first message parameter
    LPARAM  lParam 	// second message parameter
   )
{
	WORD wNotifyCode, wID;

	switch(uMsg)
	{
		case WM_INITDIALOG:
			OptionsToDialog(hwndDlg);
			return TRUE;
		
		case WM_COMMAND:
			wNotifyCode = HIWORD(wParam); // notification code 
			wID = LOWORD(wParam);         // item, control, or accelerator identifier 
			if (wNotifyCode == BN_CLICKED)
				switch(wID)
				{
					case IDOK:
						DialogToOptions(hwndDlg);
						HandleCatchKey();

					case IDCANCEL:
						EndDialog(hwndDlg, wID);
						return TRUE;

					case IDC_AUTO_REFRESH:
					case IDC_CATCHWIN:
						EnableControls(hwndDlg);
						return TRUE;
					
					default:
						return FALSE;

				}
			else return FALSE;

		default:
			return FALSE;
	}

}


int ShowOptionsDialog(HWND hParent)
{
	return DialogBox(hInst, MAKEINTRESOURCE(IDD_OPTIONS), hParent, (DLGPROC)OptionsDialogProc);
}
