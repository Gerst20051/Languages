#include <windows.h>
#include <tlhelp32.h>
#include <Commctrl.h>
#include "resource.h"
#include "general.h"
#include "about.h"

HFONT hLinkFont;
BOOL bOnLink = FALSE;

void SetLinkFont(HWND hWnd)
{
	LOGFONT lf;

	FillFontStruct(&lf, "MS Sans Serif", 10, TRUE, TRUE);
	hLinkFont = CreateFontIndirect(&lf);
	SendMessage(hWnd, WM_SETFONT, (WPARAM)hLinkFont, MAKELPARAM(0, 0));
}


void DeleteLinkFont()
{
	DeleteObject(hLinkFont);
}


//The DialogProc of "About" dialog box
BOOL CALLBACK AboutDialogProc(
  HWND hwndDlg,  // handle to dialog box
  UINT uMsg,     // message
  WPARAM wParam, // first message parameter
  LPARAM lParam  // second message parameter
)
{
	char szAboutText[1024];
	WORD wNotifyCode, wID;
	POINT pt;

	switch(uMsg)
	{
		case WM_INITDIALOG:
			GetStartText(szAboutText);
			SetDlgItemText(hwndDlg, IDC_MAIN_TEXT, szAboutText);
			SetDlgItemText(hwndDlg, IDC_URL, "http://nirsoft.mirrorz.com");
			SetLinkFont(GetDlgItem(hwndDlg, IDC_URL));
			break;


		case WM_LBUTTONUP:
			pt.x = LOWORD(lParam);    
			pt.y = HIWORD(lParam);    
			if (ChildWindowFromPoint(hwndDlg, pt) == GetDlgItem(hwndDlg, IDC_URL))
			{
				ShellExecute(hwndDlg, "open", "http://nirsoft.mirrorz.com", "", "", SW_SHOW);
				return TRUE;
			}

			break;

		case WM_MOUSEMOVE:
			pt.x = LOWORD(lParam);    
			pt.y = HIWORD(lParam);    
			if (ChildWindowFromPoint(hwndDlg, pt) == GetDlgItem(hwndDlg, IDC_URL))
			{
				SetCursor(LoadCursor(GetCurrInstance(), MAKEINTRESOURCE(IDD_HAND)));
				return TRUE;
			}
			break;
		
		case WM_CTLCOLORSTATIC:
			if ((HWND)lParam == GetDlgItem(hwndDlg, IDC_URL))
			{
				SetBkMode((HDC)wParam, TRANSPARENT);
				SetTextColor((HDC)wParam, RGB(0, 0, 192));
				return (DWORD)GetSysColorBrush(COLOR_3DFACE);
			}
			break; 

		case WM_COMMAND:
			wNotifyCode = HIWORD(wParam); // notification code 
			wID = LOWORD(wParam);         // item, control, or accelerator identifier 
			if (wID == IDOK && wNotifyCode == BN_CLICKED)
			{
				EndDialog(hwndDlg, wID);
				DeleteLinkFont();
				return TRUE;
			}

			break;

	}

	
	return FALSE;
}


void ShowAboutDialog(HWND hParent)
{
	DialogBoxParam(GetCurrInstance(), MAKEINTRESOURCE(IDD_ABOUT), hParent, AboutDialogProc, 0);
	

}