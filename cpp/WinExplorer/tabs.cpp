#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <Commctrl.h>
#include "resource.h"
#include "general.h"
#include "tabs.h"


HWND hSubWinGeneral, hSubWinPosition, hSubWinFont, hSubWinClass, hSubWinStyle, hSubWinExStyle, hSubWinMore;

HBRUSH hTextBrush;
extern int DevLogPixelsY;

BYTE CharsetCodes[] = {ANSI_CHARSET, DEFAULT_CHARSET, SYMBOL_CHARSET, SHIFTJIS_CHARSET, GB2312_CHARSET,
HANGEUL_CHARSET, CHINESEBIG5_CHARSET, OEM_CHARSET, JOHAB_CHARSET, HEBREW_CHARSET,
ARABIC_CHARSET, GREEK_CHARSET, TURKISH_CHARSET, THAI_CHARSET, EASTEUROPE_CHARSET,
RUSSIAN_CHARSET, MAC_CHARSET, BALTIC_CHARSET};

char *Charsets[] = {"Ansi", "Default", "Symbol", "SHIFTJIS", "GB2312", "Hangeul", "Chinese", "OEM", 
"Johab", "Hebrew", "Arabic", "Greek", "Turkish", "Thai", "East Europe", "Russian", "MAC", "Baltic"};

BOOL SetDlgItemHex(HWND  hDlg, int nIDDlgItem, int nNumber, int nDigits)
{
	//Format a number into hexadecimal format and put it in a dialog text-box
	char szBuff[16];
	char szTemp1[8], szTemp2[8];
	
	sprintf(szTemp1, "%d", nDigits);
	strcpy(szTemp2, "%");
	strcat(szTemp2, szTemp1);
	strcat(szTemp2, ".");
	strcat(szTemp2, szTemp1);
	strcat(szTemp2, "X");

	sprintf(szBuff, szTemp2, nNumber);
	return SetDlgItemText(hDlg, nIDDlgItem, szBuff);

}

BOOL IsDisabledEdit(HWND hWnd)
{
	char szClassName[32];
	GetClassName(hWnd, szClassName, 30);
	if (stricmp(szClassName, "Edit") == 0)
		if ((GetWindowLong(hWnd,GWL_STYLE) & ES_READONLY) == ES_READONLY)
			return TRUE;

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////Size & Position tab/////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ClearWinState()
{
	SetCheck(hSubWinPosition, IDC_WINSTATE_MAX, FALSE);
	SetCheck(hSubWinPosition, IDC_WINSTATE_MIN, FALSE);
	SetCheck(hSubWinPosition, IDC_WINSTATE_NORMAL, FALSE);
}


void ChangeWinState(HWND hWnd)
{
	int nWinState;

	if (GetCheck(hSubWinPosition, IDC_WINSTATE_MAX))
		nWinState = SW_SHOWMAXIMIZED;
	else if (GetCheck(hSubWinPosition, IDC_WINSTATE_MIN))
		nWinState = SW_SHOWMINIMIZED;
	else
		nWinState = SW_SHOWNORMAL;

	ShowWindow(hWnd, nWinState);
}

void EnableWinState(BOOL bEnable)
{
	EnableWindow(GetDlgItem(hSubWinPosition, IDC_WINSTATE_MAX), bEnable);
	EnableWindow(GetDlgItem(hSubWinPosition, IDC_WINSTATE_MIN), bEnable);
	EnableWindow(GetDlgItem(hSubWinPosition, IDC_WINSTATE_NORMAL), bEnable);

}

void WinToPropsPosition(HWND hWnd)
{
	RECT rcWin;
	HWND hParent;
	int nWidth, nHeight;

	if (GetWindowRect(hWnd, &rcWin))
	{
		nWidth = rcWin.right - rcWin.left;
		nHeight = rcWin.bottom - rcWin.top;
		hParent = GetParent(hWnd); 
		if (hParent != NULL) 
		{
			POINT pt;
			pt.x = rcWin.left;
			pt.y = rcWin.top;
			ScreenToClient(hParent, &pt);
			rcWin.left = pt.x;
			rcWin.top = pt.y;

		}

		SetDlgItemInt(hSubWinPosition, IDC_LEFT, rcWin.left, TRUE);
		SetDlgItemInt(hSubWinPosition, IDC_TOP, rcWin.top, TRUE);
		SetDlgItemInt(hSubWinPosition, IDC_WIDTH, nWidth, TRUE);
		SetDlgItemInt(hSubWinPosition, IDC_HEIGHT, nHeight, TRUE);
		WINDOWPLACEMENT wp;
		wp.length = sizeof(wp);
		if (GetWindowPlacement(hWnd, &wp))
		{
			ClearWinState();
			if (wp.showCmd == SW_SHOWMAXIMIZED)
				SetCheck(hSubWinPosition, IDC_WINSTATE_MAX, TRUE);
			else if (wp.showCmd == SW_SHOWMINIMIZED) 
				SetCheck(hSubWinPosition, IDC_WINSTATE_MIN, TRUE);
			else
				SetCheck(hSubWinPosition, IDC_WINSTATE_NORMAL, TRUE);

		}

	}

	int nStyle = GetWindowLong(hWnd, GWL_STYLE);
	EnableWinState((nStyle & WS_CAPTION) != 0);

}

BOOL DlgItemInt(HWND hWnd, int nIDDlgItem, int *lpValue, LPCSTR lpszText)
{
	BOOL bResult;
	int nValue;

	nValue = GetDlgItemInt(hWnd, nIDDlgItem, &bResult, TRUE);
	if (bResult)
	{
		*lpValue = nValue;
		return TRUE;
	} else
	{
		AppMessageBox(lpszText, NULL, MB_OK | MB_ICONSTOP);
		return FALSE;
	}
}

void PropsToWinPosition(HWND hWnd)
{
	int nLeft, nTop, nWidth, nHeight;

	if (DlgItemInt(hSubWinPosition, IDC_LEFT, &nLeft, "The value of the left position is invalid"))
		if (DlgItemInt(hSubWinPosition, IDC_TOP, &nTop, "The value of the top position is invalid"))
			if (DlgItemInt(hSubWinPosition, IDC_WIDTH, &nWidth, "The value of the width is invalid"))
				if (DlgItemInt(hSubWinPosition, IDC_HEIGHT, &nHeight, "The value of the height position is invalid"))
				{
					SetWindowPos(hWnd, NULL, nLeft, nTop, nWidth, nHeight, SWP_NOZORDER | SWP_NOACTIVATE);
				
				}

}


///////////DialogProc of Size & Position tab//////////////////////
BOOL CALLBACK PositionPropsProc(
    HWND  hwndDlg,	// handle to dialog box
    UINT  uMsg,	// message
    WPARAM  wParam,	// first message parameter
    LPARAM  lParam 	// second message parameter
   )
{
	WORD wNotifyCode, wID;
	HWND hSelectedWin;
	hSelectedWin = GetSelectedWindow();

	switch(uMsg)
	{
		case WM_INITDIALOG:
			return TRUE;

		case WM_COMMAND:
			wNotifyCode = HIWORD(wParam); // notification code 
			wID = LOWORD(wParam);         // item, control, or accelerator identifier 
			
			switch(wID)
			{
				case IDC_MODIFY:
					if (wNotifyCode == BN_CLICKED)
						PropsToWinPosition(hSelectedWin);
					return TRUE;
					break;
				
				case IDC_WINTOPMOST:
					if (wNotifyCode == BN_CLICKED)
					{
						SetWindowPos(hSelectedWin, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
					}

					return TRUE;

				case IDC_WINNOTOPMOST:
					if (wNotifyCode == BN_CLICKED)
					{
						SetWindowPos(hSelectedWin, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
						SetForegroundWindow(GetMainDlg());
					}

					return TRUE;

				case IDC_WINBOTTOM:
					if (wNotifyCode == BN_CLICKED)
					{
						SetWindowPos(hSelectedWin, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
					}
					return TRUE;

				case IDC_WINTOP:
					if (wNotifyCode == BN_CLICKED)
					{
						SetWindowPos(hSelectedWin, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
					}
					return TRUE;

				case IDC_WINSTATE_NORMAL:
				case IDC_WINSTATE_MIN:
				case IDC_WINSTATE_MAX:
					if (wNotifyCode == BN_CLICKED)
					{
						ChangeWinState(hSelectedWin);	
						WinToPropsPosition(hSelectedWin);
						return TRUE;
					} 
					return FALSE;
				default:
					return FALSE;
			}

		default:
			return FALSE;
	}

}

HWND CreatePropsPosition(HINSTANCE hInst, HWND hParent)
{
	return (hSubWinPosition = CreateDialog(hInst, MAKEINTRESOURCE(IDD_PROPS_POSITION), hParent, (DLGPROC)PositionPropsProc));
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////Class Information Tab///////////////////////////
///////////////////////////////////////////////////////////////////////////////

BOOL CALLBACK ClassPropsProc(
    HWND  hwndDlg,	// handle to dialog box
    UINT  uMsg,	// message
    WPARAM  wParam,	// first message parameter
    LPARAM  lParam 	// second message parameter
   )
{
	WORD wNotifyCode, wID;
	HWND hSelectedWin;
	hSelectedWin = GetSelectedWindow();

	switch(uMsg)
	{
		case WM_INITDIALOG:
			return TRUE;
		
		case WM_CTLCOLORSTATIC:
			if (IsDisabledEdit((HWND)lParam))
			{
				SetBkColor((HDC)wParam, GetSysColor(COLOR_WINDOW));
				SetTextColor((HDC)wParam, RGB(0, 0,255));
				return (BOOL)GetSysColorBrush(COLOR_WINDOW);
			} else return FALSE;

			

		case WM_COMMAND:
			wNotifyCode = HIWORD(wParam); // notification code 
			wID = LOWORD(wParam);         // item, control, or accelerator identifier 
			
			switch(wID)
			{
				case IDC_MODIFY:
					return TRUE;
					break;
				
				default:
					return FALSE;
			}

		default:
			return FALSE;
	}

}

void WinToPropsClass(HWND hWnd)
{
	char szClass[64];	
	char szModule[MAX_PATH];
	DWORD dwProcessID;

	GetClassName(hWnd, szClass, 63);
	SetDlgItemText(hSubWinClass, IDC_CLASS_NAME, szClass);
	SetDlgItemInt(hSubWinClass, IDC_CLASS_EXTRA, GetClassLong(hWnd, GCL_CBCLSEXTRA), FALSE);
	SetDlgItemInt(hSubWinClass, IDC_WIN_EXTRA, GetClassLong(hWnd, GCL_CBWNDEXTRA), FALSE);
	SetDlgItemHex(hSubWinClass, IDC_ATOM, GetClassLong(hWnd, GCW_ATOM), 4);
	SetDlgItemHex(hSubWinClass, IDC_INSTANCE, GetClassLong(hWnd, GCL_HMODULE), 8);
	SetDlgItemHex(hSubWinClass, IDC_WNDPROC, GetClassLong(hWnd, GCL_WNDPROC), 8);
	SetDlgItemHex(hSubWinClass, IDC_ICON1, GetClassLong(hWnd, GCL_HICON), 8);
	SetDlgItemHex(hSubWinClass, IDC_CLASS_STYLE, GetClassLong(hWnd, GCL_STYLE), 8);


	SendDlgItemMessage(hSubWinClass, IDC_CLASS_ICON, STM_SETICON, (WPARAM)GetClassLong(hWnd, GCL_HICON), 0); 
	
	GetWindowThreadProcessId(hWnd, &dwProcessID);

	if (GetProcessModuleName(dwProcessID, (HMODULE)GetClassLong(hWnd, GCL_HMODULE), szModule))
		SetDlgItemText(hSubWinClass, IDC_MODULE, szModule);
	else
		SetDlgItemText(hSubWinClass, IDC_MODULE, "");

}

HWND CreatePropsClass(HINSTANCE hInst, HWND hParent)
{
	return (hSubWinClass = CreateDialog(hInst, MAKEINTRESOURCE(IDD_PROPS_CLASS), hParent, (DLGPROC)ClassPropsProc));
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////ExStyle Tab/////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void ModifyExWindowStyle(HWND hWnd, DWORD nExStyle, BOOL bValue)
{
	DWORD dwExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
	if (bValue)
		dwExStyle |= nExStyle;
	else
		dwExStyle &= ~nExStyle;

	SetWindowLong(hWnd, GWL_EXSTYLE, dwExStyle);
}

void WinToPropsExStyle(HWND hWnd)
{
	DWORD dwIndex, dwExStyleMask;
	DWORD dwExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);

	for (dwIndex = 0; dwIndex < 19; dwIndex++)
	{
		dwExStyleMask = 1L << dwIndex;
		SetCheck(hSubWinExStyle, BASE_WIN_EXSTYLE + dwIndex, ((dwExStyle & dwExStyleMask) != 0));
	}

}

BOOL CALLBACK ExStylePropsProc(
    HWND  hwndDlg,	// handle to dialog box
    UINT  uMsg,	// message
    WPARAM  wParam,	// first message parameter
    LPARAM  lParam 	// second message parameter
   )
{
	WORD wNotifyCode, wID;
	HWND hSelectedWin;
	hSelectedWin = GetSelectedWindow();

	switch(uMsg)
	{
		case WM_INITDIALOG:
			return TRUE;
		
		case WM_CTLCOLORSTATIC:
			if (IsDisabledEdit((HWND)lParam))
			{
				SetBkColor((HDC)wParam, GetSysColor(COLOR_WINDOW));
				SetTextColor((HDC)wParam, RGB(0, 0,255));
				return (BOOL)GetSysColorBrush(COLOR_WINDOW);
			} else return FALSE;

			

		case WM_COMMAND:
			wNotifyCode = HIWORD(wParam); // notification code 
			wID = LOWORD(wParam);         // item, control, or accelerator identifier 
			
			if (wID >= BASE_WIN_EXSTYLE && wID <= BASE_WIN_EXSTYLE + 18 && wNotifyCode == BN_CLICKED)
			{
				ModifyExWindowStyle(hSelectedWin, 1L << (wID - BASE_WIN_EXSTYLE), GetCheck(hwndDlg, wID));	
				WinToPropsExStyle(hSelectedWin);
			} 
			else
			{
				switch(wID)
				{
					case IDC_MODIFY:
						return TRUE;
						break;
					
					default:
						return FALSE;
				}
			}
		default:
			return FALSE;
	}

}

HWND CreatePropsExStyle(HINSTANCE hInst, HWND hParent)
{
	return (hSubWinExStyle = CreateDialog(hInst, MAKEINTRESOURCE(IDD_PROPS_EXSTYLE), hParent, (DLGPROC)ExStylePropsProc));
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////Style Tab //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void WinToPropsStyle(HWND hWnd)
{
	 
	DWORD dwIndex, dwStyleMask;
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	char szValue[5];

	for (dwIndex = 0; dwIndex < 16; dwIndex++)
	{
		sprintf(szValue, "%4.4X", (DWORD)1 << dwIndex);
		SetDlgItemText(hSubWinStyle, BASE_WIN_SPECIFIC_STYLE + dwIndex, szValue);
	}

	if ((dwStyle & WS_CAPTION) == 0)
	{
		SetDlgItemText(hSubWinStyle, BASE_WIN_STYLE, "WS_TABSTOP");	
		SetDlgItemText(hSubWinStyle, BASE_WIN_STYLE + 1, "WS_GROUP");	
		
	} else
	{
		SetDlgItemText(hSubWinStyle, BASE_WIN_STYLE, "WS_MAXIMIZEBOX");	
		SetDlgItemText(hSubWinStyle, BASE_WIN_STYLE + 1, "WS_MINIMIZEBOX");	
	}

	for (dwIndex = 0; dwIndex < 16; dwIndex++)
	{
		dwStyleMask = 1L << (dwIndex + 16);
		SetCheck(hSubWinStyle, BASE_WIN_STYLE + dwIndex, ((dwStyle & dwStyleMask) != 0));
	}

	for (dwIndex = 0; dwIndex < 16; dwIndex++)
	{
		dwStyleMask = 1L << dwIndex;
		SetCheck(hSubWinStyle, BASE_WIN_SPECIFIC_STYLE + dwIndex, ((dwStyle & dwStyleMask) != 0));
	}

}

void ModifyWindowStyle(HWND hWnd, DWORD nStyle, BOOL bValue)
{
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	if (bValue)
		dwStyle |= nStyle;
	else
		dwStyle &= ~nStyle;

	SetWindowLong(hWnd, GWL_STYLE, dwStyle);
}

BOOL CALLBACK StylePropsProc(
    HWND  hwndDlg,	// handle to dialog box
    UINT  uMsg,	// message
    WPARAM  wParam,	// first message parameter
    LPARAM  lParam 	// second message parameter
   )
{
	WORD wNotifyCode, wID;
	HWND hSelectedWin;
	hSelectedWin = GetSelectedWindow();

	switch(uMsg)
	{
		case WM_INITDIALOG:
			return TRUE;
		
		case WM_CTLCOLORSTATIC:
			if (IsDisabledEdit((HWND)lParam))
			{
				SetBkColor((HDC)wParam, GetSysColor(COLOR_WINDOW));
				SetTextColor((HDC)wParam, RGB(0, 0,255));
				return (BOOL)GetSysColorBrush(COLOR_WINDOW);
			} else return FALSE;

			

		case WM_COMMAND:
			wNotifyCode = HIWORD(wParam); // notification code 
			wID = LOWORD(wParam);         // item, control, or accelerator identifier 
			
			if (wID >= BASE_WIN_STYLE && wID <= BASE_WIN_STYLE + 15 && wNotifyCode == BN_CLICKED)
			{
				ModifyWindowStyle(hSelectedWin, 1L << (16 + (wID - BASE_WIN_STYLE)), GetCheck(hwndDlg, wID));	
				WinToPropsStyle(hSelectedWin);
			} else
			if (wID >= BASE_WIN_SPECIFIC_STYLE && wID <= BASE_WIN_SPECIFIC_STYLE + 15 && wNotifyCode == BN_CLICKED)
			{
				ModifyWindowStyle(hSelectedWin, 1L << (wID - BASE_WIN_SPECIFIC_STYLE), GetCheck(hwndDlg, wID));	
				WinToPropsStyle(hSelectedWin);
			} 

			else
			{
				switch(wID)
				{
					case IDC_MODIFY:
						return TRUE;
						break;
					
					default:
						return FALSE;
				}
			}
		default:
			return FALSE;
	}

}

HWND CreatePropsStyle(HINSTANCE hInst, HWND hParent)
{
	return (hSubWinStyle = CreateDialog(hInst, MAKEINTRESOURCE(IDD_PROPS_STYLE), hParent, (DLGPROC)StylePropsProc));
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////Font Tab////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int FindFontCharSet(BYTE charset)
{
	for (int i = 0; i < NUM_OF_CHARSET; i++)
		if (CharsetCodes[i] == charset) return i;

	return -1;
}

void PropsToWinFont(HWND hWnd)
{

}

void WinToPropsFont(HWND hWnd)
{
	LOGFONT lf;
	HFONT hFont = NULL;

	SendMessageTimeout(hWnd, WM_GETFONT, 0, 0, SMTO_ABORTIFHUNG, 300, (DWORD *)&hFont); 
	//HFONT hFont = (HFONT)SendMessage(hWnd, WM_GETFONT, 0, 0);
	if (hFont != NULL)
	{
		if (GetObject(hFont, sizeof(lf), &lf) != 0)
		{
			SetDlgItemText(hSubWinFont, IDC_FONT_NAME, lf.lfFaceName);
			SetDlgItemInt(hSubWinFont, IDC_FONT_SIZE, abs(lf.lfHeight * 72 / DevLogPixelsY), TRUE);
			SetCheck(hSubWinFont, IDC_FONT_ITALIC, lf.lfItalic);
			SetCheck(hSubWinFont, IDC_FONT_UNDERLINE, lf.lfUnderline);
			SetCheck(hSubWinFont, IDC_FONT_STRIKEOUT, lf.lfStrikeOut);
			SetCheck(hSubWinFont, IDC_FONT_BOLD, (lf.lfWeight >= 700));
			int charset = FindFontCharSet(lf.lfCharSet);
			if (charset == -1) 
				SetDlgItemText(hSubWinFont, IDC_FONT_CHARSET, "Unknown");
			else
				SetDlgItemText(hSubWinFont, IDC_FONT_CHARSET, Charsets[charset]);

			return;
		}
	} 

	SetDlgItemText(hSubWinFont, IDC_FONT_NAME, "Unknown");
	SetDlgItemText(hSubWinFont, IDC_FONT_SIZE, "");
	SetDlgItemText(hSubWinFont, IDC_FONT_CHARSET, "Unknown");
	SetCheck(hSubWinFont, IDC_FONT_ITALIC, FALSE);
	SetCheck(hSubWinFont, IDC_FONT_UNDERLINE, FALSE);
	SetCheck(hSubWinFont, IDC_FONT_STRIKEOUT, FALSE);
	SetCheck(hSubWinFont, IDC_FONT_BOLD, FALSE);

}

BOOL CALLBACK FontPropsProc(
    HWND  hwndDlg,	// handle to dialog box
    UINT  uMsg,	// message
    WPARAM  wParam,	// first message parameter
    LPARAM  lParam 	// second message parameter
   )
{
	WORD wNotifyCode, wID;
	HWND hSelectedWin;
	hSelectedWin = GetSelectedWindow();

	switch(uMsg)
	{
		case WM_INITDIALOG:
			return TRUE;
		
		case WM_CTLCOLORSTATIC:
			if (IsDisabledEdit((HWND)lParam))
			{
				SetBkColor((HDC)wParam, GetSysColor(COLOR_WINDOW));
				SetTextColor((HDC)wParam, RGB(0, 0,255));
				return (BOOL)GetSysColorBrush(COLOR_WINDOW);
			} else return FALSE;

			

		case WM_COMMAND:
			wNotifyCode = HIWORD(wParam); // notification code 
			wID = LOWORD(wParam);         // item, control, or accelerator identifier 
			
			switch(wID)
			{
				case IDC_MODIFY:
					if (wNotifyCode == BN_CLICKED)
						PropsToWinFont(hSelectedWin);
					return TRUE;
					break;
				
				default:
					return FALSE;
			}

		default:
			return FALSE;
	}

}

HWND CreatePropsFont(HINSTANCE hInst, HWND hParent)
{
	return (hSubWinFont = CreateDialog(hInst, MAKEINTRESOURCE(IDD_PROPS_FONT), hParent, (DLGPROC)FontPropsProc));
}


///////////////////////////////////////////////////////////////////////////////
////////////////////////////// More Tab////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BOOL CALLBACK MorePropsProc(
    HWND  hwndDlg,	// handle to dialog box
    UINT  uMsg,	// message
    WPARAM  wParam,	// first message parameter
    LPARAM  lParam 	// second message parameter
   )
{
	
	HWND hSelectedWin;
	hSelectedWin = GetSelectedWindow();

	switch(uMsg)
	{
		case WM_INITDIALOG:
			return TRUE;
		
		case WM_CTLCOLORSTATIC:
			if (IsDisabledEdit((HWND)lParam))
			{
				SetBkColor((HDC)wParam, GetSysColor(COLOR_WINDOW));
				SetTextColor((HDC)wParam, RGB(0, 0,255));
				return (BOOL)GetSysColorBrush(COLOR_WINDOW);
			} else 
				return FALSE;

		default:
			return FALSE;
	}


}


HWND CreatePropsMore(HINSTANCE hInst, HWND hParent)
{
	return (hSubWinMore = CreateDialog(hInst, MAKEINTRESOURCE(IDD_PROPS_MORE), hParent, (DLGPROC)MorePropsProc));
}

void WinToPropsMore(HWND hWnd)
{
	static BOOL bFirstPropsMore = TRUE;

	if (hWnd != NULL)
	{
		DWORD dwProcessID, dwThreadID;
		//char szTemp[20];

		dwThreadID = GetWindowThreadProcessId(hWnd, &dwProcessID);
		SetDlgItemHex(hSubWinMore, IDC_THREADID, dwThreadID, 8);
		SetDlgItemHex(hSubWinMore, IDC_PROCESSID, dwProcessID, 8);

		char szFilename[MAX_PATH];
		if (GetProcessModuleName(dwProcessID, NULL, szFilename))
			SetDlgItemText(hSubWinMore, IDC_PROCESS_PATH, szFilename);
		else
			SetDlgItemText(hSubWinMore, IDC_PROCESS_PATH, "");

		if (GetProcessModuleName(dwProcessID, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), szFilename))
			SetDlgItemText(hSubWinMore, IDC_MODULE_PATH, szFilename);
		else
			SetDlgItemText(hSubWinMore, IDC_MODULE_PATH, "");


		if (bFirstPropsMore) 
		{
			char szStartText[1024];
			GetStartText(szStartText);
			bFirstPropsMore = FALSE;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////General Tab/////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

LRESULT SendDlgMessageGeneral(int nIDDlgItem, UINT  Msg,WPARAM  wParam, LPARAM  lParam)
{
	return SendDlgItemMessage(hSubWinGeneral, nIDDlgItem, Msg, wParam, lParam);
}


BOOL CALLBACK GeneralPropsProc(
    HWND  hwndDlg,	// handle to dialog box
    UINT  uMsg,	// message
    WPARAM  wParam,	// first message parameter
    LPARAM  lParam 	// second message parameter
   )
{
	WORD wNotifyCode, wID;
	HWND hSelectedWin;
	hSelectedWin = GetSelectedWindow();

	switch(uMsg)
	{
		case WM_INITDIALOG:
			return TRUE;

		case WM_CTLCOLORSTATIC:
			if (IsDisabledEdit((HWND)lParam))
			{
				SetBkColor((HDC)wParam, GetSysColor(COLOR_WINDOW));
				SetTextColor((HDC)wParam, RGB(0, 0,255));
				return (BOOL)GetSysColorBrush(COLOR_WINDOW);
			} else return FALSE;

		case WM_COMMAND:
			wNotifyCode = HIWORD(wParam); // notification code 
			wID = LOWORD(wParam);         // item, control, or accelerator identifier 
			
			switch(wID)
			{
				case IDC_WIN_VISIBLE:
					if (wNotifyCode == BN_CLICKED) 
					{
						if (SendDlgMessageGeneral(IDC_WIN_VISIBLE, BM_GETCHECK, 0, 0)) 
							ShowWindow(hSelectedWin, SW_SHOW);
						else
							ShowWindow(hSelectedWin, SW_HIDE);
					}
					return TRUE;

				case IDC_WIN_ENABLED:
					if (wNotifyCode == BN_CLICKED) 
						EnableWindow(hSelectedWin, SendDlgMessageGeneral(IDC_WIN_ENABLED, BM_GETCHECK, 0, 0));
					
					return TRUE;

				case IDC_MODIFY:
					if (wNotifyCode == BN_CLICKED) PropsToWinGeneral(hSelectedWin);
					return TRUE;
				
				case IDC_DESTROY:
					if (wNotifyCode == BN_CLICKED) 
					{
						if (MessageBox(hwndDlg, "Are you sure you want to destroy the selected window ?", "Destroy Window", MB_ICONQUESTION | MB_YESNO) == IDYES)
						{
							
							HWND hChildWin = GetParent(hSelectedWin);
							DestroyWin(hSelectedWin);
							RefreshWindowsTree(hChildWin);
						}
					}
					return TRUE;
				
				default:
					return FALSE;
			}
			
			break;

		default:
			return FALSE;

	}

}


HWND CreatePropsGeneral(HINSTANCE hInst, HWND hParent)
{
	return (hSubWinGeneral = CreateDialog(hInst, MAKEINTRESOURCE(IDD_PROPS_GENERAL), hParent, (DLGPROC)GeneralPropsProc));
}



void PropsToWinGeneral(HWND hWnd)
{
	char buf[MAX_TEXT];

	if (hWnd != NULL)
	{
		SendDlgMessageGeneral(IDC_WIN_CAPTION, WM_GETTEXT, MAX_TEXT - 1, (LPARAM)buf);
		SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)buf);
		SetWindowText(hWnd, buf);
		RefreshCurrentTreeItem(); 
	}
}

void WinToPropsGeneral(HWND hWnd)
{
	const MAX_EXTRA = 1024;
	char buf[MAX_TEXT];

	if (hWnd != NULL)
	{
		SetCheck(hSubWinGeneral, IDC_WIN_VISIBLE, IsWindowVisible(hWnd));
		SetCheck(hSubWinGeneral, IDC_WIN_ENABLED, IsWindowEnabled(hWnd));
		SetDlgItemHex(hSubWinGeneral, IDC_WIN_HANDLE, (LONG)hWnd, 8);
		SetDlgItemHex(hSubWinGeneral, IDC_WIN_HINSTANCE, GetWindowLong(hWnd, GWL_HINSTANCE), 8);

		
		SetDlgItemHex(hSubWinGeneral, IDC_WIN_USER_DATA, GetWindowLong(hWnd, GWL_USERDATA), 8);


		DWORD dwResult;
		SendMessageTimeout(hWnd, WM_GETTEXT, MAX_TEXT - 1, (LPARAM)buf, SMTO_ABORTIFHUNG, 300, &dwResult);
		SendDlgItemMessage(hSubWinGeneral, IDC_WIN_CAPTION, WM_SETTEXT, 0, (LPARAM)buf);
		GetClassName(hWnd, buf, MAX_TEXT - 1);
		SendDlgItemMessage(hSubWinGeneral, IDC_WIN_CLASSNAME, WM_SETTEXT, 0, (LPARAM)buf);
		SetDlgItemInt(hSubWinGeneral, IDC_WIN_ITEMID, GetDlgCtrlID(hWnd), FALSE);	

		DWORD dwExtra = GetClassLong(hWnd, GCL_CBWNDEXTRA);
		if (dwExtra >=4)
		{
			char szExtra[MAX_EXTRA] = "";
			char szTemp[20];
			DWORD dwLen = 0;

			for (DWORD dwIndex = 0; dwIndex <= dwExtra - 4; dwIndex+=4)
			{
				sprintf(szTemp, "%8.8X", GetWindowLong(hWnd, dwIndex));
				dwLen += strlen(szTemp) + 1;
			
				if (dwLen < MAX_EXTRA - 20) 
				{
					strcat(szExtra, szTemp);
					strcat(szExtra, " ");
				}
			}

			SetDlgItemText(hSubWinGeneral, IDC_WIN_EXTRA_BYTES, szExtra);
		}
		else
			SetDlgItemText(hSubWinGeneral, IDC_WIN_EXTRA_BYTES, "");

	}
}

void DestroyWin(HWND hwnd)
{
	PostMessage(hwnd, WM_CLOSE, 0, 0);
}
