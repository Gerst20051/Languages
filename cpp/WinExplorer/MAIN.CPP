/*
WinExplorer v1.30
Copyright (c) 1999 - 2003 Nir Sofer 

WinExplorer is a utility that shows all system's windows in hierarchical 
display. For every window in the hierarchy, you can view its properties,
like handle, class name, caption, size, position and more. You can also
modify some properties, like Caption and Visible/Enabled. 
You can use this utility in all Win32-based operating systems: 
Windows 95,98,ME,NT,2000,XP 


This utility is released as freeware with full source code. 
You can freely use, distribute, and modify the source code of this 
utility without restrictions. However, if you release to the public 
a modified version of this utility, you should specify the original 
copyright notice.

The source code is not well documented, but every advanced programmer
will be able to understand it after some time.
*/

#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <Commctrl.h>
#include "resource.h"
#include "general.h"
#include "tabs.h"
#include "main.h"
#include "options.h"
#include "about.h"

HWND hMainTree, hMainTab;
HWND hSubWins[NUM_OF_SUBWINS];
HIMAGELIST hImageList;
HINSTANCE hInst;
int iIndex;
HICON hMainIcon16, hMainIcon32;

//Find Dialog Variables
char szFindBuffer[MAX_FIND_BUFFER];
FINDREPLACE FindReplace;
UINT WM_FIND_REPLACE, uFindFlags = 0, uFindFlagsEx = 1;
HWND hFindReplace = NULL;
HTREEITEM hSelectedItem;
BOOL bSelectedItemPassed;
BOOL bAutoRefreshActive = FALSE;

int iImageWin, iImageButton, iImageWinHide, iImageCombo, iImageList,iImageEdit, iImageTreeView, iImageListView, 
	iImageStatic, iImageUnknown, iImageWinNoCaption, iImageWinNoCaptionHide, iImageEditPass, iImageTab,
	iImageDelphi, iImageVB, iImageProgMan, iImageCheckBox, iImageRadioButton, iImageFrame, iImageTimer, 
	iImageScroll, iImagePicture, iImageMSDos, iImageRegEdit, iImageMDIClient, iImageTaskBar, iImageIntExplorer, iImageRas, iImageNetscape;

int iImageBaseAppIcons, iAppIconsIndex;


//The DialogProc of the main window:
BOOL CALLBACK MainDlgProc(
    HWND  hwndDlg,	// handle to dialog box
    UINT  uMsg,	// message
    WPARAM  wParam,	// first message parameter
    LPARAM  lParam 	// second message parameter
   )
{
	WORD wNotifyCode, wID;
	int idCtrl;
	LPNMHDR pnmh;
	UINT code;
	FINDREPLACE *FindInfo;
	HTREEITEM hTreeItem;
	POINT ptCursor;
	HWND hWnd;

	switch(uMsg)
	{
		case WM_INITDIALOG:

			//Load all icons to image list
			hImageList = ImageList_Create(16, 16, ILC_MASK | ILC_COLOR24 , 1, 1);
			iImageWin = ImageList_ReplaceIcon(hImageList, -1, LoadIcon(hInst, MAKEINTRESOURCE(IDI_WIN1)));
			iImageWinHide = ImageList_ReplaceIcon(hImageList, -1, LoadIcon(hInst, MAKEINTRESOURCE(IDI_WIN_HIDE)));
			iImageWinNoCaption = ImageList_ReplaceIcon(hImageList, -1, LoadIcon(hInst, MAKEINTRESOURCE(IDI_WIN1_NOCAPTION)));
			iImageWinNoCaptionHide = ImageList_ReplaceIcon(hImageList, -1, LoadIcon(hInst, MAKEINTRESOURCE(IDI_WIN_HIDE_NOCAPTION)));
			iImageButton = ImageList_ReplaceIcon(hImageList, -1, LoadIcon(hInst, MAKEINTRESOURCE(IDI_BUTTON)));
			iImageCombo = ImageList_ReplaceIcon(hImageList, -1, LoadIcon(hInst, MAKEINTRESOURCE(IDI_COMBO)));
			iImageList = ImageList_ReplaceIcon(hImageList, -1, LoadIcon(hInst, MAKEINTRESOURCE(IDI_LIST)));
			iImageEdit = ImageList_ReplaceIcon(hImageList, -1, LoadIcon(hInst, MAKEINTRESOURCE(IDI_EDIT)));
			iImageEditPass = ImageList_ReplaceIcon(hImageList, -1, LoadIcon(hInst, MAKEINTRESOURCE(IDI_EDIT_PASS)));
			

			iImageListView = ImageList_ReplaceIcon(hImageList, -1, LoadIcon(hInst, MAKEINTRESOURCE(IDI_LISTVIEW)));
			iImageTreeView = ImageList_ReplaceIcon(hImageList, -1, LoadIcon(hInst, MAKEINTRESOURCE(IDI_TREEVIEW)));
			iImageTab = ImageList_ReplaceIcon(hImageList, -1, LoadIcon(hInst, MAKEINTRESOURCE(IDI_TABCONTROL)));
			iImageStatic = ImageList_ReplaceIcon(hImageList, -1, LoadIcon(hInst, MAKEINTRESOURCE(IDI_STATIC)));
			iImageUnknown = ImageList_ReplaceIcon(hImageList, -1, LoadIcon(hInst, MAKEINTRESOURCE(IDI_UNKNOWN)));
			iImageDelphi = ImageList_ReplaceIcon(hImageList, -1, LoadIcon(hInst, MAKEINTRESOURCE(IDI_DELPHI2)));
			iImageVB = ImageList_ReplaceIcon(hImageList, -1, LoadIcon(hInst, MAKEINTRESOURCE(IDI_VB5)));
			iImageProgMan = ImageList_ReplaceIcon(hImageList, -1, LoadIcon(hInst, MAKEINTRESOURCE(IDI_PROGMAN)));
			iImageCheckBox  = ImageList_ReplaceIcon(hImageList, -1, LoadIcon(hInst, MAKEINTRESOURCE(IDI_CHECK)));
			iImageRadioButton = ImageList_ReplaceIcon(hImageList, -1, LoadIcon(hInst, MAKEINTRESOURCE(IDI_OPTION)));
			iImageFrame = ImageList_ReplaceIcon(hImageList, -1, LoadIcon(hInst, MAKEINTRESOURCE(IDI_FRAME)));
			iImageScroll = ImageList_ReplaceIcon(hImageList, -1, LoadIcon(hInst, MAKEINTRESOURCE(IDI_SCROLLBAR)));
			iImagePicture = ImageList_ReplaceIcon(hImageList, -1, LoadIcon(hInst, MAKEINTRESOURCE(IDI_PICTURE)));
			iImageMDIClient = ImageList_ReplaceIcon(hImageList,-1, LoadIcon(hInst, MAKEINTRESOURCE(IDI_MDICLIENT)));
			
			iImageBaseAppIcons = ImageList_GetImageCount(hImageList); 
			ImageList_SetImageCount(hImageList, iImageBaseAppIcons + MAX_APP_ICONS);
			
			hMainTree = GetDlgItem(hwndDlg, IDC_TREE1);
			hMainTab =  GetDlgItem(hwndDlg, IDC_PROPS_TAB);
			SendTreeMsg(TVM_SETIMAGELIST, TVSIL_NORMAL, (LPARAM)hImageList);
			FillWinTree();

			//Prepare all properties window:
			SetSubWin(CreatePropsGeneral(hInst, hMainTab), 0);
			SetSubWin(CreatePropsPosition(hInst, hMainTab), 1);
			SetSubWin(CreatePropsFont(hInst, hMainTab), 2);
			SetSubWin(CreatePropsClass(hInst, hMainTab), 3);
			SetSubWin(CreatePropsMore(hInst, hMainTab), 4);
			SetSubWin(CreatePropsStyle(hInst, hMainTab), 5);
			SetSubWin(CreatePropsExStyle(hInst, hMainTab), 6);
			//Insert the properties tabs into the tab control
			InsertTabs();
			SelectFirstItemInTree();
			//Activate the automatic refresh timer
			SetAutoRefreshTimer(hwndDlg);
			return TRUE;
		
		case WM_NOTIFY:
			idCtrl = (int) wParam; 
			pnmh = (LPNMHDR) lParam;
			code = pnmh->code;
			switch(idCtrl)
			{
				case IDC_TREE1:
					if (code == TVN_SELCHANGED) 
					{
						//Tree item selection has changed - 
						//Update all properties tab to the new selected item.
						TreeSelChanged();
					}
					return TRUE;

				case IDC_PROPS_TAB:
					ShowSubWin(SendTabMsg(TCM_GETCURSEL, 0, 0));
					return TRUE;
					break;

				default:
					return FALSE;
	
			}

		case WM_COMMAND:
			wNotifyCode = HIWORD(wParam); // notification code 
			wID = LOWORD(wParam);         // item, control, or accelerator identifier 
			
			switch(wID)
			{
				case IDC_CLOSE:
					//The user clicked the Close button:
					if (wNotifyCode == BN_CLICKED) 
					{
						UnregisterCatchKey();
						DestroyWindow(hwndDlg);
					}
					return TRUE;
				
				case IDC_REFRESH:
					//The user clicked the Refresh button:
					if (wNotifyCode == BN_CLICKED) RefreshWinTree();
					return TRUE;
					break;
				
				case IDC_ABOUT: 
					ShowAboutDialog(hwndDlg); 
					return TRUE;

				case IDC_OPTIONS:
					if (wNotifyCode == BN_CLICKED) 
					{
						//Display the options dialog:
						if (ShowOptionsDialog(hwndDlg) == IDOK)
						{
							RefreshWinTree();
							SetAutoRefreshTimer(hwndDlg);
						}
					}
					return TRUE;
				
				case IDC_FIND_WIN:
					if (wNotifyCode == BN_CLICKED)
					{
						if (hFindReplace == NULL)
						{
							//Display the find dialog-box:
							OpenFindReplace(hwndDlg);
						}
					}
					return TRUE;
				default:
					return FALSE;
			}
			
			break;

		case WM_HOTKEY:
			if (wParam == CATCH_HOTKEY_ID)
			{	
				//The user pressed the hot-key combination.
				if (GetCursorPos(&ptCursor))
				{
					//Find the window in the current mouse cursor position.
					hWnd = WindowFromPoint(ptCursor);
					RefreshWinTree();
					//Select the window in the windows tree.
					SelectWndInTree(hWnd);
				}
			}
			return TRUE;

		case WM_CLOSE:
			DestroyWindow(hwndDlg);
			return TRUE;

		case WM_DESTROY:
			PostQuitMessage(0);
			return TRUE;

		case WM_REFRESH_WIN_TREE:
			if (lParam != 0) SelectWndInTree((HWND)lParam); 
			RefreshWinTree();			
			return TRUE;
		
		case WM_REFRESH_TREE_ITEM:
			RefreshCurrTreeItem();
			return TRUE;

			
		case WM_SIZE:
			return FALSE;

		case WM_TIMER:
			RefreshWinTree();
			return TRUE;

		default:
			if (uMsg == WM_FIND_REPLACE)
			{
				//Message of Find dialog-box.
				FindInfo = (FINDREPLACE *)lParam;
				if ((FindInfo->Flags & FR_DIALOGTERM) == FR_DIALOGTERM)
				{
					uFindFlags = FindInfo->Flags & 7;
					CloseFindData();
				}
				if ((FindInfo->Flags & FR_FINDNEXT) == FR_FINDNEXT)
				{
					//Find a window according to find parameters:
					hTreeItem = FindNextWindow(FindInfo->lpstrFindWhat, FindInfo->Flags);
					if (hTreeItem != NULL) 
						SendTreeMsg(TVM_SELECTITEM, TVGN_CARET, (LPARAM)hTreeItem);
					else
						AppMessageBox("The search item was not found.", NULL, MB_OK | MB_ICONEXCLAMATION);
				}
				return TRUE;

			} else
			 return FALSE;

	}

}


int WINAPI WinMain(
    HINSTANCE  hInstance,	// handle to current instance
    HINSTANCE  hPrevInstance,	// handle to previous instance
    LPSTR  lpCmdLine,	// pointer to command line
    int  nShowCmd 	// show state of window
   )
{
	MSG msg;
	
	InitCommonControls();
	SetCurrInstance(hInstance);
	GeneralInit();

	//Register find/replace dialog
	WM_FIND_REPLACE = RegisterWindowMessage(FINDMSGSTRING);
	hInst = hInstance;

	//Create the main dialog window.
	SetMainDlg(CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, (DLGPROC)MainDlgProc));

	//Load main icon into main dialog.
	hMainIcon16 = (HICON)LoadImage(hInst, MAKEINTRESOURCE(IDI_MAIN), IMAGE_ICON, 16, 16, 0);
	hMainIcon32 = (HICON)LoadImage(hInst, MAKEINTRESOURCE(IDI_MAIN), IMAGE_ICON, 32, 32, 0);
	SendMessage(GetMainDlg(), WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon32);
	SendMessage(GetMainDlg(), WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)hMainIcon16);
	
	//Show the dialog window
	ShowWindow(GetMainDlg(), SW_SHOW);
	HandleCatchKey();
	

	//The main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{

		int nIndex;

		BOOL bDialogFound = FALSE;
		for (nIndex = 0; nIndex < NUM_OF_SUBWINS; nIndex++)
			if (IsDialogMessage(hSubWins[nIndex], &msg))	
				bDialogFound = TRUE;

		if (hFindReplace != NULL)
			if (IsDialogMessage(hFindReplace, &msg))	
				bDialogFound = TRUE;

		if (!bDialogFound)
			if (!IsDialogMessage(GetMainDlg(), &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
	 }
	
	return 0;
}


void SetSubWin(HWND hWnd, int nIndex)
{
	hSubWins[nIndex] = hWnd;
}

LRESULT SendTreeMsg(UINT  Msg,WPARAM  wParam, LPARAM  lParam )
{
	return SendMessage(hMainTree, Msg, wParam, lParam);
}

LRESULT SendTabMsg(UINT  Msg,WPARAM  wParam, LPARAM  lParam )
{
	return SendMessage(hMainTab, Msg, wParam, lParam);
}

void AdjustTabWindow(HWND hSubWin)
{
	RECT rcWin;
	GetClientRect(hSubWin, &rcWin);
	SendTabMsg(TCM_ADJUSTRECT, (WPARAM)FALSE, (LPARAM)&rcWin);
	SetWindowPos(hSubWin, NULL, rcWin.left, rcWin.top, rcWin.right - rcWin.left + 1, rcWin.bottom - rcWin.top + 1, SWP_NOZORDER);
}

void GetTreeItemText(HWND hWnd, LPSTR szBuf)
{
	char szClass[32];
	char szText[255];
	DWORD dwResult;

	SendMessageTimeout(hWnd, WM_GETTEXT, 254, (LPARAM)szText, SMTO_ABORTIFHUNG, 300, &dwResult); 
	GetClassName(hWnd, szClass, 30);
	strcpy(szBuf, szText);
	strcat(szBuf, " [");
	strcat(szBuf, szClass);
	strcat(szBuf, "]");
}


void InsertTabs()
{
	int nIndex;

	//Insert all tabs to the tab control.
	InsertTab("General", 0, 0);
	InsertTab("Size && Position", 0, 0);
	InsertTab("Font", 0, 0);
	InsertTab("Class Information", 0, 0);
	InsertTab("More", 0, 0);
	InsertTab("Style", 0, 0);
	InsertTab("Extended Style", 0, 0);

	for (nIndex = 0; nIndex < NUM_OF_SUBWINS; nIndex++)
		AdjustTabWindow(hSubWins[nIndex]);
	SendTabMsg(TCM_SETCURSEL, 0, 0);
	ShowSubWin(0);

}

void ShowSubWin(int nIndex)
{

	int nWinIndex;
	HWND hSelected;

	hSelected = hSubWins[nIndex];

	//Display the selected properties tab, and hide the previous one.	
	for (nWinIndex = 0; nWinIndex < NUM_OF_SUBWINS; nWinIndex++)
		if (hSubWins[nWinIndex] == hSelected) ShowWindow(hSubWins[nWinIndex], SW_SHOW);

	for (nWinIndex = 0; nWinIndex < NUM_OF_SUBWINS; nWinIndex++)
		if (hSubWins[nWinIndex] != hSelected) ShowWindow(hSubWins[nWinIndex], SW_HIDE);

			
}

void InsertTab(LPSTR lpszText, int iImage, LPARAM lParam)
{
	TC_ITEM item;
	
	item.mask = TCIF_TEXT;
	item.pszText = lpszText;
	item.iImage = iImage;
	item.lParam = lParam;

	SendTabMsg(TCM_INSERTITEM, SendTabMsg(TCM_GETITEMCOUNT, 0, 0), (LPARAM)&item);
}

BOOL IsStartWith(LPCSTR lpszText, LPCSTR lpszStartText)
{
	char *tmp;
	int nLenStartText = strlen(lpszStartText);
	int nLenText = strlen(lpszText);
	BOOL bReturn;
	
	if (nLenStartText > nLenText)
		return FALSE;
	else
	{
		tmp = new char[nLenText + 1];
		strcpy(tmp, lpszText);
		tmp[nLenStartText] = '\0';
		bReturn = (!stricmp(tmp, lpszStartText));
		delete []tmp;
		return bReturn;
	}
}

BOOL IsEndWith(LPCSTR lpszText, LPCSTR lpszEndText)
{
	char *tmp;
	int nLenEndText = strlen(lpszEndText);
	int nLenText = strlen(lpszText);
	BOOL bReturn;

	if (nLenEndText > nLenText)
		return FALSE;
	else
	{	
		tmp = new char[nLenEndText + 1];
		strcpy(tmp, &lpszText[nLenText - nLenEndText]);
		bReturn = (!stricmp(tmp, lpszEndText));
		delete []tmp;
		return bReturn;
	}
}

BOOL IsClass(LPCSTR lpszClass, LPCSTR lpszStart, LPCSTR lpszEnd)
{
	return (IsEndWith(lpszClass, lpszEnd) && IsStartWith(lpszClass, lpszStart));
}

void SetAutoRefreshTimer(HWND hwndDlg)
{
	CGeneralAppOptions *pAppOptions = GetAppOptions();

	if (bAutoRefreshActive)
	{
		KillTimer(hwndDlg, REFRESH_TIMER_ID);
		bAutoRefreshActive = FALSE;
	}

	if (pAppOptions->m_bAutoRefresh)
	{
		//Activate the refresh timer:
		SetTimer(hwndDlg, REFRESH_TIMER_ID, pAppOptions->m_dwRefreshSeconds * 1000, NULL);
		bAutoRefreshActive = TRUE;
	} 
	
}

UINT CALLBACK OpenFindHookProc(
  HWND hdlg,      // handle to the dialog box window
  UINT uiMsg,     // message identifier
  WPARAM wParam,  // message parameter
  LPARAM lParam   // message parameter
)
{
	switch(uiMsg)
	{

		case WM_INITDIALOG:
			return TRUE;

		default:
			return FALSE; 
	}

}
 
void OpenFindReplace(HWND hwndDlg)
{
	//Show find dialog-box:

	FindReplace.lStructSize = sizeof(FindReplace);
	FindReplace.hwndOwner = hwndDlg;
	FindReplace.Flags = FR_HIDEUPDOWN | FR_HIDEWHOLEWORD | FR_ENABLETEMPLATE | FR_ENABLEHOOK | uFindFlags;
	FindReplace.lpstrFindWhat = szFindBuffer;
	FindReplace.lpstrReplaceWith = NULL;
	FindReplace.wFindWhatLen = MAX_FIND_BUFFER;
	FindReplace.lpTemplateName = MAKEINTRESOURCE(IDD_FIND);
	FindReplace.hInstance = hInst;
	FindReplace.lpfnHook = OpenFindHookProc;
	hFindReplace = FindText(&FindReplace);
	SetCheck(hFindReplace, 500 + (uFindFlagsEx & FIND_IN_ALL), TRUE);
	SetCheck(hFindReplace, IDC_FIND_ANY_PART, ((uFindFlagsEx & FIND_ANY_PART) == FIND_ANY_PART));
}

void CloseFindData()
{
	uFindFlagsEx = GetRadioCheck(hFindReplace, 501, 500 + FIND_IN_ALL) | (GetCheck(hFindReplace, IDC_FIND_ANY_PART)?FIND_ANY_PART:0);
	
	hFindReplace = NULL;
}

HTREEITEM FindNextWindowRecurs(LPSTR lpszWinCaption, DWORD dwFlags, HTREEITEM hCurrItem)
{
	HTREEITEM hNextItem, hFoundItem; //, hCurrItem = (HTREEITEM)SendTreeMsg(TVM_GETNEXTITEM, TVGN_CARET, 0);
	HWND hCurrWnd;
	char szText[MAX_TREE_ITEM_TEXT];
	char szWinCaption[MAX_TREE_ITEM_TEXT] = "";
	BOOL bExists;
	int nItemID;
	
	if (strlen(lpszWinCaption) < MAX_TREE_ITEM_TEXT) 
		strcpy(szWinCaption, lpszWinCaption);
	if ((dwFlags & FR_MATCHCASE) == 0)
		strlwr(szWinCaption);
	
	while (hCurrItem != NULL)
	{
		hCurrWnd = GetWndInTreeItem(hCurrItem);
		if (bSelectedItemPassed)
		{
			if (GetCheck(hFindReplace, 500 + FIND_IN_WINTEXT))
				bExists = (GetWindowText(hCurrWnd, szText, MAX_TREE_ITEM_TEXT) != 0);
			else if (GetCheck(hFindReplace, 500 + FIND_IN_ITEMID))
			{
				
				nItemID = GetDlgCtrlID(hCurrWnd);
				itoa(nItemID, szText, 10);
				bExists = TRUE;
			}
			else if (GetCheck(hFindReplace, 500 + FIND_IN_CLASSNAME))
				bExists = (GetClassName(hCurrWnd, szText, MAX_TREE_ITEM_TEXT) != 0);

			if (bExists)
			{
				if (GetCheck(hFindReplace, IDC_FIND_ANY_PART))
				{
					if ((dwFlags & FR_MATCHCASE) == 0)
						strlwr(szText);
					if (strstr(szText, szWinCaption) != NULL)
						return hCurrItem;
				} else
					if ((dwFlags & FR_MATCHCASE) == FR_MATCHCASE)
					{
						if (strcmp(lpszWinCaption, szText) == 0) return hCurrItem;
					}
					else	
						if (stricmp(lpszWinCaption, szText) == 0) 
							return hCurrItem;
			}
		} else if (hCurrItem == hSelectedItem) bSelectedItemPassed = TRUE;
		
		hNextItem = (HTREEITEM)SendTreeMsg(TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hCurrItem);
		if (hNextItem != NULL)
		{
			hFoundItem = FindNextWindowRecurs(lpszWinCaption, dwFlags, hNextItem);				
			if (hFoundItem != NULL) return hFoundItem;
		}
				
		hCurrItem = (HTREEITEM)SendTreeMsg(TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)hCurrItem);

	}

	return NULL;
}


HTREEITEM FindNextWindow(LPSTR lpszWinCaption, DWORD dwFlags)
{
	HTREEITEM hRootItem = (HTREEITEM)SendTreeMsg(TVM_GETNEXTITEM, TVGN_ROOT, 0);
	hSelectedItem = (HTREEITEM)SendTreeMsg(TVM_GETNEXTITEM, TVGN_CARET, 0);	
	bSelectedItemPassed = (hSelectedItem == NULL);
	return FindNextWindowRecurs(lpszWinCaption, dwFlags, hRootItem);
}

int GetTreeItemImage(HWND hWnd)
{
	//This function determines which icon to use, by checking the class name
	//and styles.
	char szClass[32];
	int iImage, iStyle;

	GetClassName(hWnd, szClass, 30);
	iStyle = GetWindowLong(hWnd, GWL_STYLE);  
	//strupr(szClass);
	if ((stricmp(szClass,"BUTTON") == 0 && ((iStyle & 0x0f) == 0 || (iStyle & 0x0f) == BS_DEFPUSHBUTTON)) || IsClass(szClass, "THUNDER", "COMMANDBUTTON") || stricmp(szClass,"TBUTTON") == 0)
		iImage = iImageButton;
	else if ((!stricmp(szClass,"BUTTON") && ((iStyle & 0x0f) == BS_CHECKBOX || (iStyle & 0x0f) == BS_AUTOCHECKBOX || (iStyle & 0x0f) == BS_AUTO3STATE)) || IsClass(szClass, "THUNDER", "CHECKBOX") || !stricmp(szClass,"TCHECKBOX"))
		iImage = iImageCheckBox;
	else if ((!stricmp(szClass,"BUTTON") && ((iStyle & 0x0f) == BS_RADIOBUTTON || (iStyle & 0x0f) == BS_AUTORADIOBUTTON)) || IsClass(szClass, "THUNDER", "OPTIONBUTTON"))
		iImage = iImageRadioButton;
	else if ((!stricmp(szClass,"BUTTON") && (iStyle & 0x0f) == BS_GROUPBOX))
		iImage = iImageFrame;
	else if ((!stricmp(szClass, "BUTTON")))
		iImage = iImageButton;
	else if (!stricmp(szClass, "COMBOBOX") || IsClass(szClass, "THUNDER", "COMBOBOX") || !stricmp(szClass,"TCOMBOBOX"))
		iImage = iImageCombo;
	else if (!stricmp(szClass, "LISTBOX") || IsClass(szClass, "THUNDER", "LISTBOX") || !stricmp(szClass,"TLISTBOX"))
		iImage = iImageList;
	else if (!stricmp(szClass,"EDIT") || IsClass(szClass, "THUNDER","TEXTBOX") || !stricmp(szClass,"TEDIT"))
		if (iStyle & ES_PASSWORD)
			iImage = iImageEditPass;
		else
			iImage = iImageEdit;
	else if (!stricmp(szClass,"STATIC"))
		iImage = iImageStatic;
	else if (!stricmp(szClass, WC_TABCONTROL) || !stricmp(szClass, "SSTabCtlWndClass") || !stricmp(szClass, "TTabbedNotebook"))
		iImage = iImageTab;
	else if (!stricmp(szClass, WC_TREEVIEW))
		iImage = iImageTreeView;
	else if (!stricmp(szClass, WC_LISTVIEW))
		iImage = iImageListView;
	else if (!stricmp(szClass,"TAPPLICATION"))
			iImage = iImageDelphi;
	else if (stricmp(szClass,"THUNDERRT6MAIN") == 0 || stricmp(szClass,"THUNDERRT5MAIN") == 0 || stricmp(szClass,"THUNDERRT4MAIN") == 0 || stricmp(szClass,"THUNDERMAIN") == 0)
			iImage = iImageVB;
	else if (!stricmp(szClass,"SCROLLBAR") || IsClass(szClass, "THUNDER", "SCROLLBAR") || !stricmp(szClass,"TSCROLLBAR"))
			iImage = iImageScroll;
	else if (IsClass(szClass, "THUNDER", "PICTUREBOX"))
			iImage = iImagePicture;
	else if (IsClass(szClass, "THUNDER", "FRAME") || !stricmp(szClass,"TPANEL"))
			iImage = iImageFrame;
	else if (!stricmp(szClass, "MDICLIENT"))
			iImage = iImageMDIClient;
	else if (!stricmp(szClass,"PROGMAN"))
			iImage = iImageProgMan;
	else if ((iStyle & WS_CAPTION) != 0)
		if (IsWindowVisible(hWnd))
			iImage = iImageWin;
		else
			iImage = iImageWinHide;
	else if ((iStyle & WS_POPUP) != 0)
		if (IsWindowVisible(hWnd))
			iImage = iImageWinNoCaption;
		else
			iImage = iImageWinNoCaptionHide;
	else if (!stricmp(szClass, "THUNDERFORM"))
		iImage = iImageWinNoCaption;
	else 
		iImage = iImageUnknown;
	
	return iImage;
}

int GetTreeItemAppImage(HWND hWnd)
{
	HICON hIcon = GetWinIcon(hWnd, ICON_SMALL);
	if (hIcon != NULL)
	{
		iAppIconsIndex++;
		ImageList_ReplaceIcon(hImageList, iAppIconsIndex, hIcon);
		return iAppIconsIndex;
	} else
		return 0;

}

HTREEITEM AddWinTreeItem(HWND hWnd, HTREEITEM hParent)
{
	TV_INSERTSTRUCT is;
	char szItemText[MAX_TREE_ITEM_TEXT];
	int iImage;
	GetTreeItemText(hWnd, szItemText);
	
	if (hParent == TVI_ROOT && GetAppOptions()->m_bUseAppIcons)
	{
		iImage = GetTreeItemAppImage(hWnd);
		if (iImage == 0) iImage = GetTreeItemImage(hWnd);
	} 
	else
		iImage = GetTreeItemImage(hWnd); 

	is.hParent = hParent;
	is.hInsertAfter = TVI_SORT;
	is.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	is.item.iImage = iImage;
	is.item.iSelectedImage = iImage;
	is.item.pszText = szItemText;
	is.item.lParam = (LPARAM)hWnd;

	return (HTREEITEM)SendTreeMsg(TVM_INSERTITEM, 0, (LPARAM)&is);

}

BOOL CALLBACK EnumChildWinProc(
    HWND  hwnd,	
    LPARAM  lParam 	
   )
{
	if (GetAppOptions()->m_bFullTree)
	{
		HWND hParent = GetWndInTreeItem((HTREEITEM)lParam);
		if (hParent == GetParent(hwnd))
		{
			HTREEITEM hNewItem = AddWinTreeItem(hwnd, (HTREEITEM)lParam);
			EnumChildWindows(hwnd, (WNDENUMPROC)EnumChildWinProc, (LPARAM)hNewItem); 
		}
	}
	else
		AddWinTreeItem(hwnd, (HTREEITEM)lParam);
	return TRUE;
}

BOOL CALLBACK EnumWinProc(
    HWND  hwnd,	// handle to parent window
    LPARAM  lParam 	// application-defined value
   )
{
	HTREEITEM hNewItem;
	if (GetAppOptions()->m_bHideTopHiddenWindows)
		if (!IsWindowVisible(hwnd))
			return TRUE;
	hNewItem = AddWinTreeItem(hwnd, (HTREEITEM)lParam);

	//Enumerate child windows:
	EnumChildWindows(hwnd, (WNDENUMPROC)EnumChildWinProc, (LPARAM)hNewItem); 	
	return TRUE;
}

void TreeSelChanged()
{
	SetSelectedWindow(GetSelectedWndInTree());
	
	//Update all properties tab to the new selected window in the tree:
	WinToPropsGeneral(GetSelectedWindow());
	WinToPropsPosition(GetSelectedWindow());
	WinToPropsFont(GetSelectedWindow());
	WinToPropsClass(GetSelectedWindow());
	WinToPropsStyle(GetSelectedWindow());
	WinToPropsExStyle(GetSelectedWindow());
	WinToPropsMore(GetSelectedWindow());
}

void FillWinTree()
{
	iAppIconsIndex = iImageBaseAppIcons;  
	SendTreeMsg(TVM_DELETEITEM, 0, (LPARAM)TVI_ROOT);	//clear
	//Enumerate Windows
	EnumWindows((WNDENUMPROC)EnumWinProc, (LONG)TVI_ROOT);
	
}

void RefreshWinTree()
{
	HWND hCurrWin = GetSelectedWndInTree();
	FillWinTree();
	if (!SelectWndInTree(hCurrWin))
		SelectFirstItemInTree();
}

void RefreshTreeItem(HTREEITEM hItem)
{
	TV_ITEM item;
	char szItemText[MAX_TREE_ITEM_TEXT];
	int iImage;
	HWND hWnd;

	hWnd = GetWndInTreeItem(hItem);
	GetTreeItemText(hWnd, szItemText);

	iImage = GetTreeItemImage(hWnd); 
	item.hItem = hItem;
	item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	item.iImage = iImage;
	item.iSelectedImage = iImage;
	item.pszText = szItemText;
	SendTreeMsg(TVM_SETITEM, 0, (LPARAM)&item);
}

void RefreshCurrTreeItem()
{
	HTREEITEM hTreeItem = (HTREEITEM)SendTreeMsg(TVM_GETNEXTITEM, TVGN_CARET, 0);
	if (hTreeItem != NULL)
	{
		RefreshTreeItem(hTreeItem);	
	}
}

HTREEITEM FindTreeItemByWnd(HWND hWnd, HTREEITEM hStartItem)
{
	HTREEITEM hNextItem;
	HTREEITEM hFoundItem = NULL;
	HTREEITEM hCurrItem;
	HWND hItemWnd;

	hCurrItem = hStartItem;
	while (hCurrItem != NULL)
	{
		hItemWnd = GetWndInTreeItem(hCurrItem);
		if (hItemWnd == hWnd) return hCurrItem;
		hNextItem = (HTREEITEM)SendTreeMsg(TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hCurrItem);
		if (hNextItem != NULL) 
			hFoundItem = FindTreeItemByWnd(hWnd, hNextItem);

		if (hFoundItem == NULL)
			hCurrItem = (HTREEITEM)SendTreeMsg(TVM_GETNEXTITEM, TVGN_CHILD,  (LPARAM)hCurrItem);
		else
			return hFoundItem;
	}

	return NULL;
}

HWND GetWndInTreeItem(HTREEITEM hItem)
{
	TV_ITEM item;

	item.mask = TVIF_HANDLE;
	item.hItem = hItem;

	SendTreeMsg(TVM_GETITEM, 0, (LPARAM)&item);

	return (HWND)item.lParam;
}

void SelectFirstItemInTree()
{
	HTREEITEM hNext = (HTREEITEM)SendTreeMsg(TVM_GETNEXTITEM, TVGN_FIRSTVISIBLE, 0);
	if (hNext != NULL) 
	{
		SendTreeMsg(TVM_SELECTITEM, TVGN_CARET, (LPARAM)hNext );
		TreeSelChanged();	
	}
}

BOOL SelectWndInTree(HWND hWnd, HTREEITEM hItem)
{
	HTREEITEM hChild, hCurrTreeItem;

	hCurrTreeItem = hItem;
	if (hCurrTreeItem == TVI_ROOT) hCurrTreeItem = (HTREEITEM)SendTreeMsg(TVM_GETNEXTITEM, TVGN_ROOT, 0);  
	while (hCurrTreeItem != NULL)
	{
		HWND hTreeWnd = GetWndInTreeItem(hCurrTreeItem);	
		if (hTreeWnd == hWnd) 
		{
			SendTreeMsg(TVM_SELECTITEM, TVGN_CARET, (LPARAM)hCurrTreeItem);		
			return TRUE;
		} else
		{
			hChild = (HTREEITEM)SendTreeMsg(TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hCurrTreeItem);
			if (hChild != NULL) 
				if (SelectWndInTree(hWnd, hChild))
					return TRUE;
		}
		hCurrTreeItem = (HTREEITEM)SendTreeMsg(TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)hCurrTreeItem);
	}
	return FALSE;
}

HWND GetSelectedWndInTree()
{
	HWND hWnd;
	TV_ITEM it;

	HTREEITEM hItem = (HTREEITEM)SendTreeMsg(TVM_GETNEXTITEM, TVGN_CARET, 0);
	if (hItem != NULL)
	{
		it.mask = TVIF_HANDLE;
		it.hItem = hItem;

		SendTreeMsg(TVM_GETITEM, 0, (LPARAM)&it);
		hWnd = (HWND)it.lParam;
		return hWnd;
	} else
		return NULL;

}

LRESULT SendMainDlgMessage(int nIDDlgItem, UINT  Msg,WPARAM  wParam, LPARAM  lParam)
{
	return SendDlgItemMessage(GetMainDlg(), nIDDlgItem, Msg, wParam, lParam);
}

