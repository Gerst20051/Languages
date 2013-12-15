/****************************************************************************
 *																			*
 * Filename: yatzy.c														*
 *																			*
 * Purpose : Yatzy score card for Windows CE.								*
 *																			*
 * History : Date	   Reason												*
 *			 00/00/00  Created												*
 *																			*
 ****************************************************************************/

#define UNICODE
#include <windows.h>
#include <windowsx.h>
#include <aygshell.h>
#include <commctrl.h>
#include "yatzy.h"

#define MAXPLAYERS  4
#define MAXPLAYERNAME  80
#define NOSCORE  -1

#ifndef PROOF_QUALITY
#define PROOF_QUALITY  2
#endif

#define LISTVIEWSTYLE	(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_SHOWSELALWAYS|LVS_OWNERDATA|LVS_OWNERDRAWFIXED)
#define MENU_HEIGHT  26

typedef struct _PLAYER *PPLAYER;
typedef struct _PLAYER {
	WCHAR *pszName;					// Name of player.
	int nWidth;						// Column width.
} PLAYER;

typedef struct _ROW *PROW;
typedef struct _ROW {
	WCHAR *pszText;					// Display text.
	int (*pfnDialog)(HWND, PROW);	// Score dialog procedure.
	int nFactor;					// Score or factor.
	int nScore[MAXPLAYERS];			// Score for each player.
} ROW;

/** Prototyper **************************************************************/

static LRESULT WINAPI MainWndProc(HWND, UINT, WPARAM, LPARAM);
static BOOL Main_OnCreate(HWND, CREATESTRUCT *);
static void CreatePlayerList(void);
static void Main_OnSize(HWND, UINT, int, int);
static void Main_OnSetFocus(HWND, HWND);
static void Main_OnCommand(HWND, int, HWND, UINT);
static void Main_OnMeasureItem(HWND, MEASUREITEMSTRUCT *);
static void Main_OnDrawItem(HWND, const DRAWITEMSTRUCT *);
static BOOL Main_OnEraseBkgnd(HWND, HDC);
static BOOL Main_OnNotify(HWND, int, LPNMHDR);
static void Main_OnSettingChange(HWND, UINT, PCTSTR);
static void Main_OnDestroy(HWND);
static void EnterScore(HWND);
static void NewGame(HWND);
static int DiceCount(HWND, PROW);
static int DiceOneKind(HWND, PROW);
static int DiceTwoKind(HWND, PROW);
static int DiceChance(HWND, PROW);
static int DiceConfirm(HWND, PROW);
static LRESULT WINAPI AboutDlgProc(HWND, UINT, WPARAM, LPARAM);
static LRESULT CALLBACK NewGameDlgProc(HWND, UINT, WPARAM, LPARAM);
static LRESULT CALLBACK DiceCountDlgProc(HWND, UINT, WPARAM, LPARAM);
static void DiceCountDlgEnable(HWND);
static LRESULT CALLBACK DiceOneKindDlgProc(HWND, UINT, WPARAM, LPARAM);
static void DiceOneKindDlgEnable(HWND);
static LRESULT CALLBACK DiceTwoKindDlgProc(HWND, UINT, WPARAM, LPARAM);
static void DiceTwoKindDlgEnable(HWND);
static LRESULT CALLBACK DiceChanceDlgProc(HWND, UINT, WPARAM, LPARAM);
static void DiceChanceDlgEnable(HWND);
static LRESULT CALLBACK DiceConfirmDlgProc(HWND, UINT, WPARAM, LPARAM);
static void CreateDiceBar(HWND, int);
static void CheckDigitBar(HWND, int, int);
static int GetDigitBarValue(HWND, int);
static void CreateDigitBar(HWND, int);
static void CheckDiceBar(HWND, int, int);
static int GetDiceBarValue(HWND, int);

/** Globala variabler *******************************************************/

static HANDLE ghInstance;
static HWND ghwndMB;
static HWND ghwndLV;
static HFONT ghFont;
static HFONT ghFontBold;

static WCHAR szAppName[] = L"Yatzy";
static WCHAR szTitle[] = L"Yatzy score card";
static WCHAR szGameOver[] = L"Game is over!";

static int giPlayer = 0;
static int gcPlayers = 0;
static int gnDescWidth = 70;

static WCHAR szName1[MAXPLAYERNAME] = L"";
static WCHAR szName2[MAXPLAYERNAME] = L"";
static WCHAR szName3[MAXPLAYERNAME] = L"";
static WCHAR szName4[MAXPLAYERNAME] = L"";

static PLAYER PlayerInfo[MAXPLAYERS] = {
	{ szName1, 0 },
	{ szName2, 0 },
	{ szName3, 0 },
	{ szName4, 0 }
};

static ROW RowInfo[] = {
	{ L"1's", DiceCount, 1 },
	{ L"2's", DiceCount, 2 },
	{ L"3's", DiceCount, 3 },
	{ L"4's", DiceCount, 4 },
	{ L"5's", DiceCount, 5 },
	{ L"6's", DiceCount, 6 },
	{ L"Total" },
	{ L"Bonus" },
	{ L"1 pair", DiceOneKind, 2 },
	{ L"2 pairs", DiceTwoKind, 2 },
	{ L"3 of a kind", DiceOneKind, 3 },
	{ L"4 of a kind", DiceOneKind, 4 },
	{ L"1 to 5", DiceConfirm, 15 },
	{ L"2 to 6", DiceConfirm, 20 },
	{ L"Full house", DiceTwoKind, 3 },
	{ L"Chance", DiceChance },
	{ L"Yatzy", DiceConfirm, 50 },
	{ L"Total" },
	{ NULL }
};

/****************************************************************************
 *																			*
 * Function: WinMain														*
 *																			*
 * Purpose : Initialize the application. Register window class, create		*
 *			 main window and enter message loop.							*
 *																			*
 * History : Date	   Reason												*
 *			 00/00/00  Created												*
 *																			*
 ****************************************************************************/

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpszCmdLine, int nCmdShow)
{
	WNDCLASS wc;
	MSG msg;
	HWND hwnd;

	/* Always check first if program is already running */
	hwnd = FindWindow(szAppName, NULL);
	if (hwnd)
	{
		/*
		 * Set focus to the foremost child window. The "|0x01" is used to
		 * bring any owned windows to the foreground and activate them.
		 */
		SetForegroundWindow((HWND)((ULONG)hwnd|0x00000001));
		return 0;
	}

	/* Register the main window class */
	if (!hPrevInstance)
	{
		wc.lpszClassName = szAppName;
		wc.lpfnWndProc = MainWndProc;
		wc.style = CS_VREDRAW|CS_HREDRAW;
		wc.hInstance = hInstance;
		wc.hIcon = NULL;
		wc.hCursor = NULL;
		wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wc.lpszMenuName = NULL;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;

		if (!RegisterClass(&wc))
			return 1;
	}

	ghInstance = hInstance;

	/* Create the main window */
	hwnd = CreateWindowEx(
		WS_EX_CAPTIONOKBTN,
		szAppName,
		szTitle,
		WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	if (!hwnd)
		return 1;

	/*
	 * When the main window is created using CW_USEDEFAULT the height of the menubar (if one
	 * is created is not taken into account). So we resize the window after creating it
	 * if a menubar is present.
	 */
	{
		RECT rc;
		GetWindowRect(hwnd, &rc);
		rc.bottom -= MENU_HEIGHT;
		if (ghwndMB)
			MoveWindow(hwnd, rc.left, rc.top, rc.right, rc.bottom, FALSE);
	}

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	/* Always start with a new game */
	FORWARD_WM_COMMAND(hwnd, IDM_GAME_NEW, ghwndMB, 0, PostMessage);

	/* Message loop */
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	/* It was fun, while it lasted... */
	return msg.wParam;
}

/****************************************************************************
 *																			*
 * Function: MainWndProc													*
 *																			*
 * Purpose : Process application messages.									*
 *																			*
 * History : Date	   Reason												*
 *			 00/00/00  Created												*
 *																			*
 ****************************************************************************/

static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		HANDLE_MSG(hwnd, WM_CREATE, Main_OnCreate);
		HANDLE_MSG(hwnd, WM_SIZE, Main_OnSize);
		HANDLE_MSG(hwnd, WM_SETFOCUS, Main_OnSetFocus);
		HANDLE_MSG(hwnd, WM_COMMAND, Main_OnCommand);
		HANDLE_MSG(hwnd, WM_MEASUREITEM, Main_OnMeasureItem);
		HANDLE_MSG(hwnd, WM_DRAWITEM, Main_OnDrawItem);
		HANDLE_MSG(hwnd, WM_ERASEBKGND, Main_OnEraseBkgnd);
		HANDLE_MSG(hwnd, WM_NOTIFY, Main_OnNotify);
		HANDLE_MSG(hwnd, WM_SETTINGCHANGE, Main_OnSettingChange);
		HANDLE_MSG(hwnd, WM_DESTROY, Main_OnDestroy);
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

/****************************************************************************
 *																			*
 * Function: Main_OnCreate													*
 *																			*
 * Purpose : Process WM_CREATE message.										*
 *																			*
 * History : Date	   Reason												*
 *			 00/00/00  Created												*
 *																			*
 ****************************************************************************/

static BOOL Main_OnCreate(HWND hwnd, CREATESTRUCT *pcs)
{
	SHMENUBARINFO mbi;
	LOGFONT lf;

	//
	// Create the menu bar.
	//
	memset(&mbi, 0, sizeof(mbi));
	mbi.cbSize = sizeof(mbi);
	mbi.hwndParent = hwnd;
	mbi.nToolBarId = IDR_MNU_MAIN;
	mbi.hInstRes = ghInstance;
	mbi.nBmpId = 0;
	mbi.cBmpImages = 0;
	if (!SHCreateMenuBar(&mbi))
		return FALSE;

	ghwndMB = mbi.hwndMB;

	//
	// Create two fonts, normal and bold.
	//
	memset(&lf, 0, sizeof(lf));
	wcscpy(lf.lfFaceName, L"MS Sans Serif");
	lf.lfHeight = -11;
	lf.lfWeight = FW_NORMAL;
	lf.lfQuality = PROOF_QUALITY;
	ghFont = CreateFontIndirect(&lf);

	memset(&lf, 0, sizeof(lf));
	wcscpy(lf.lfFaceName, L"MS Sans Serif");
	lf.lfHeight = -11;
	lf.lfWeight = FW_SEMIBOLD;
	lf.lfQuality = PROOF_QUALITY;
	ghFontBold = CreateFontIndirect(&lf);

	//
	// Create the Listview control.
	//
	ghwndLV = CreateWindow(WC_LISTVIEW, NULL, LISTVIEWSTYLE, 0, 0, 0, 0, hwnd, (HMENU)666, ghInstance, NULL);
	if (!ghwndLV)
		return FALSE;

	FORWARD_WM_SETFONT(ghwndLV, ghFont, FALSE, SendMessage);

	ListView_SetItemCount(ghwndLV, 18);
	ListView_EnsureVisible(ghwndLV, 0, TRUE);

	CreatePlayerList();

	return TRUE;
}

/****************************************************************************
 *																			*
 * Function: CreatePlayerList												*
 *																			*
 * Purpose : Initialize the Listview for the given players. 				*
 *																			*
 * History : Date	   Reason												*
 *			 00/00/00  Created												*
 *																			*
 ****************************************************************************/

static void CreatePlayerList(void)
{
	LVCOLUMN lvc;
	int i, j;

	/* Delete columns, if we have been here before */
	while (ListView_DeleteColumn(ghwndLV, 0))
		;

	/* Insert the 'Description' column */
	lvc.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	lvc.fmt = LVCFMT_LEFT;
	lvc.cx = gnDescWidth;
	lvc.pszText = L"";
	lvc.iSubItem = 0;
	ListView_InsertColumn(ghwndLV, 0, &lvc);

	/* Insert columns for each player */
	for (i = 0; i < gcPlayers; i++)
	{
		lvc.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
		lvc.fmt = LVCFMT_RIGHT;
		lvc.cx = PlayerInfo[i].nWidth;
		lvc.pszText = PlayerInfo[i].pszName;
		lvc.iSubItem = i+1;
		ListView_InsertColumn(ghwndLV, i+1, &lvc);

		for (j = 0; j <= 17; j++)
			RowInfo[j].nScore[i] = NOSCORE;
	}
}

/****************************************************************************
 *																			*
 * Function: Main_OnSize													*
 *																			*
 * Purpose : Process WM_SIZE message.										*
 *																			*
 * History : Date	   Reason												*
 *			 00/00/00  Created												*
 *																			*
 ****************************************************************************/

static void Main_OnSize(HWND hwnd, UINT state, int cx, int cy)
{
	if (state != SIZE_MINIMIZED)
		SetWindowPos(ghwndLV, NULL, 0, 0, cx, cy, SWP_NOACTIVATE|SWP_NOZORDER);
}

/****************************************************************************
 *																			*
 * Function: Main_OnSetFocus												*
 *																			*
 * Purpose : Process WM_SETFOCUS message.									*
 *																			*
 * History : Date	   Reason												*
 *			 00/00/00  Created												*
 *																			*
 ****************************************************************************/

static void Main_OnSetFocus(HWND hwnd, HWND hwndOldFocus)
{
	SetFocus(ghwndLV);
}

/****************************************************************************
 *																			*
 * Function: Main_OnCommand 												*
 *																			*
 * Purpose : Process WM_COMMAND message.									*
 *																			*
 * History : Date	   Reason												*
 *			 00/00/00  Created												*
 *																			*
 ****************************************************************************/

static void Main_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if (hwndCtl == ghwndMB)
	{
		switch (id)
		{
			case IDM_GAME_NEW:
				DialogBox(ghInstance, MAKEINTRESOURCE(DLG_NEWGAME), hwnd, (DLGPROC)NewGameDlgProc);
				break;

			case IDM_HELP_ABOUT:
				DialogBox(ghInstance, MAKEINTRESOURCE(DLG_ABOUT), hwnd, (DLGPROC)AboutDlgProc);
				break;
		}
	}
	else if (hwndCtl == NULL)
	{
		switch (id)
		{
			case IDOK:
				EnterScore(hwnd);
				break;
		}
	}
}

/****************************************************************************
 *																			*
 * Function: Main_OnMeasureItem 											*
 *																			*
 * Purpose : Process WM_MEASUREITEM message.								*
 *																			*
 * History : Date	   Reason												*
 *			 00/00/00  Created												*
 *																			*
 ****************************************************************************/

static void Main_OnMeasureItem(HWND hwnd, MEASUREITEMSTRUCT *pmis)
{
	HDC hdc;

	if (pmis->CtlType != ODT_LISTVIEW) return;

	hdc = GetDC(hwnd);
	if (hdc)
	{
		SIZE size;

		/* Calculate Listview row height from the current font */
		SelectFont(hdc, ghFont);
		GetTextExtentExPoint(hdc, L"O", 1, 0, NULL, NULL, &size);
		pmis->itemHeight = size.cy;

		ReleaseDC(hwnd, hdc);
	}
}

/****************************************************************************
 *																			*
 * Function: Main_OnDrawItem												*
 *																			*
 * Purpose : Process WM_DRAWITEM message.									*
 *																			*
 * History : Date	   Reason												*
 *			 00/00/00  Created												*
 *																			*
 ****************************************************************************/

static void Main_OnDrawItem(HWND hwnd, const DRAWITEMSTRUCT *pdis)
{
	RECT rcItem = pdis->rcItem;
	COLORREF clrText;

	if (pdis->CtlType != ODT_LISTVIEW) return;

	/* Draw the Listview state */
	if (pdis->itemState & ODS_SELECTED)
	{
		FillRect(pdis->hDC, &rcItem, GetSysColorBrush(COLOR_HIGHLIGHT));
		clrText = SetTextColor(pdis->hDC, GetSysColor(COLOR_HIGHLIGHTTEXT));
	}
	else
	{
		if (pdis->itemState & ODS_DISABLED)
		{
			FillRect(pdis->hDC, &rcItem, GetSysColorBrush(COLOR_BTNFACE));
			clrText = SetTextColor(pdis->hDC, GetSysColor(COLOR_GRAYTEXT));
		}
		else
		{
			FillRect(pdis->hDC, &rcItem, GetSysColorBrush(COLOR_WINDOW));
			clrText = SetTextColor(pdis->hDC, GetSysColor(COLOR_WINDOWTEXT));
		}
	}

	if (pdis->itemID != -1)
	{
		PROW pRow = &RowInfo[pdis->itemID];
		HFONT hFont;
		int i;

		/* Draw the Listview row description */
		rcItem.right = rcItem.left + gnDescWidth;
		hFont = SelectFont(pdis->hDC, pRow->pfnDialog ? ghFont : ghFontBold);
		DrawText(pdis->hDC, pRow->pszText, -1, &rcItem, DT_LEFT|DT_VCENTER);

		/* Draw the Listview row cell for each player */
		for (i = 0; i < gcPlayers; i++)
		{
			COLORREF clrSave;

			rcItem.left = rcItem.right;
			rcItem.right += PlayerInfo[i].nWidth;

			if (i == giPlayer)
			{
				FillRect(pdis->hDC, &rcItem, GetSysColorBrush(COLOR_HIGHLIGHT));
				clrSave = SetTextColor(pdis->hDC, GetSysColor(COLOR_HIGHLIGHTTEXT));
			}

			/* If the player has a score, display it */
			if (pRow->nScore[i] != NOSCORE)
			{
				WCHAR szText[10];
				wsprintf(szText, L"%d", pRow->nScore[i]);
				DrawText(pdis->hDC, szText, -1, &rcItem, DT_RIGHT|DT_VCENTER);
			}

			if (i == giPlayer)
			{
				SetTextColor(pdis->hDC, clrSave);
			}
		}

		SelectFont(pdis->hDC, hFont);
	}

	SetTextColor(pdis->hDC, clrText);

	if (pdis->itemState & ODS_FOCUS)
		DrawFocusRect(pdis->hDC, &pdis->rcItem);
}

/****************************************************************************
 *																			*
 * Function: Main_OnEraseBkgnd												*
 *																			*
 * Purpose : Process WM_ERASEBKGND message.									*
 *																			*
 * History : Date	   Reason												*
 *			 00/00/00  Created												*
 *																			*
 ****************************************************************************/

static BOOL Main_OnEraseBkgnd(HWND hwnd, HDC hdc)
{
	// Don't touch my buddy - the background.
	return FALSE;
}

/****************************************************************************
 *																			*
 * Function: Main_OnNotify													*
 *																			*
 * Purpose : Process WM_NOTIFY message.										*
 *																			*
 * History : Date	   Reason												*
 *			 00/00/00  Created												*
 *																			*
 ****************************************************************************/

static BOOL Main_OnNotify(HWND hwnd, int id, LPNMHDR pnmh)
{
	switch (pnmh->code)
	{
		case LVN_KEYDOWN:
		{
			NMLVKEYDOWN *pnkd = (NMLVKEYDOWN *)pnmh;
			if (pnkd->wVKey == VK_RETURN)
				EnterScore(hwnd);
			break;
		}

		case HDN_ENDTRACK:
		{
			HD_NOTIFY *phdn = (HD_NOTIFY *)pnmh;
			if (phdn->iItem == 0)
				gnDescWidth = phdn->pitem->cxy;
			else
				PlayerInfo[phdn->iItem-1].nWidth = phdn->pitem->cxy;
			InvalidateRect(ghwndLV, NULL, TRUE);
			break;
		}
	}

	return 0;
}

/****************************************************************************
 *																			*
 * Function: Main_OnSettingChange											*
 *																			*
 * Purpose : Process WM_SETTINGCHANGE message.								*
 *																			*
 * History : Date	   Reason												*
 *			 00/00/00  Created												*
 *																			*
 ****************************************************************************/

static void Main_OnSettingChange(HWND hwnd, UINT uiAction, PCTSTR pszSection)
{
	switch (uiAction)
	{
		case SPI_SETSIPINFO:
		{
			SHACTIVATEINFO sai;
			memset(&sai, 0, sizeof(SHACTIVATEINFO));
			// Adjust the window size depending on the SIP panel.
			SHHandleWMSettingChange(hwnd, -1, 0, &sai);
			break;
		}
	}
}

/****************************************************************************
 *																			*
 * Function: Main_OnDestroy 												*
 *																			*
 * Purpose : Process WM_DESTROY message.									*
 *																			*
 * History : Date	   Reason												*
 *			 00/00/00  Created												*
 *																			*
 ****************************************************************************/

static void Main_OnDestroy(HWND hwnd)
{
	DestroyWindow(ghwndLV);
	DestroyWindow(ghwndMB);
	PostQuitMessage(0);
}

/****************************************************************************
 *																			*
 * Function: EnterScore 													*
 *																			*
 * Purpose : Allow entering score for the current player.					*
 *																			*
 * History : Date	   Reason												*
 *			 00/00/00  Created												*
 *																			*
 ****************************************************************************/

static void EnterScore(HWND hwnd)
{
	int iItem = ListView_GetNextItem(ghwndLV, -1, LVNI_SELECTED);
	if (iItem != -1)
	{
		PROW pRow = &RowInfo[iItem];
		/* Don't allow the player to enter two scores for the same entry */
		if (pRow->pfnDialog && pRow->nScore[giPlayer] == NOSCORE)
		{
			BOOL more = FALSE;
			int i, j, sum;

			/* Call the appropriate dialog, to get the new score */
			if (!pRow->pfnDialog(hwnd, pRow))
				return;

			/* Update totals */
			for (i = 0; i < gcPlayers; i++)
			{
				for (sum = 0, j = 0; j < 17; j++)
				{
					if (j == 6)
					{
						RowInfo[6].nScore[i] = sum;
						RowInfo[7].nScore[i] = sum >= 63 ? 50 : 0;
					}
					else if (RowInfo[j].nScore[i] != NOSCORE)
						sum += RowInfo[j].nScore[i];
					else
						more = TRUE;
				}
				RowInfo[17].nScore[i] = sum;
			}

			/* It's the next players turn! */
			if (++giPlayer >= gcPlayers)
				giPlayer = 0;

			InvalidateRect(ghwndLV, NULL, TRUE);

			if (!more)  /* Oops - we're done! */
				MessageBox(NULL, szGameOver, szTitle, MB_SETFOREGROUND|MB_OK);
		}
	}
}

/****************************************************************************
 *																			*
 * Function: *																*
 *																			*
 * Purpose : Dialog call wrappers.											*
 *																			*
 * History : Date	   Reason												*
 *			 00/00/00  Created												*
 *																			*
 ****************************************************************************/

static int DiceCount(HWND hwnd, PROW pRow)
{
	return DialogBoxParam(ghInstance, MAKEINTRESOURCE(DLG_COUNT), hwnd, (DLGPROC)DiceCountDlgProc, (LPARAM)pRow);
}

static int DiceOneKind(HWND hwnd, PROW pRow)
{
	return DialogBoxParam(ghInstance, MAKEINTRESOURCE(DLG_ONEKIND), hwnd, (DLGPROC)DiceOneKindDlgProc, (LPARAM)pRow);
}

static int DiceTwoKind(HWND hwnd, PROW pRow)
{
	return DialogBoxParam(ghInstance, MAKEINTRESOURCE(DLG_TWOKIND), hwnd, (DLGPROC)DiceTwoKindDlgProc, (LPARAM)pRow);
}

static int DiceChance(HWND hwnd, PROW pRow)
{
	return DialogBoxParam(ghInstance, MAKEINTRESOURCE(DLG_CHANCE), hwnd, (DLGPROC)DiceChanceDlgProc, (LPARAM)pRow);
}

static int DiceConfirm(HWND hwnd, PROW pRow)
{
	return DialogBoxParam(ghInstance, MAKEINTRESOURCE(DLG_CONFIRM), hwnd, (DLGPROC)DiceConfirmDlgProc, (LPARAM)pRow);
}

/****************************************************************************
 *																			*
 * Function: AboutDlgProc													*
 *																			*
 * Purpose : Process messages for the 'About' dialog. The dialog is shown	*
 *			 when the user selects About, in the Help menu.					*
 *																			*
 * History : Date	   Reason												*
 *			 00/00/00  Created												*
 *																			*
 ****************************************************************************/

static LRESULT CALLBACK AboutDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_INITDIALOG:
			return TRUE;

		case WM_COMMAND:
			switch (wParam)
			{
				case IDOK:
					EndDialog(hwnd, TRUE);
					return TRUE;
			}
			break;
	}

	return FALSE;
}

/****************************************************************************
 *																			*
 * Function: NewGameDlgProc 												*
 *																			*
 * Purpose : New Game dialog procedure.										*
 *																			*
 * History : Date	   Reason												*
 *			 00/00/00  Created												*
 *																			*
 ****************************************************************************/

static LRESULT CALLBACK NewGameDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_INITDIALOG:
		{
			/* Initialize the silly dialog */
			SHINITDLGINFO shidi;
			shidi.dwMask = SHIDIM_FLAGS;
			shidi.dwFlags = SHIDIF_DONEBUTTON|SHIDIF_SIZEDLGFULLSCREEN;
			shidi.hDlg = hwnd;
			SHInitDialog(&shidi);
			/**/
			SetDlgItemText(hwnd, DID_PLAYER1, PlayerInfo[0].pszName);
			SetDlgItemText(hwnd, DID_PLAYER2, PlayerInfo[1].pszName);
			SetDlgItemText(hwnd, DID_PLAYER3, PlayerInfo[2].pszName);
			SetDlgItemText(hwnd, DID_PLAYER4, PlayerInfo[3].pszName);
			return TRUE;
		}

		case WM_COMMAND:
			switch (GET_WM_COMMAND_ID(wParam, lParam))
			{
				case IDOK:
				{
					WCHAR szPlayer[MAXPLAYERNAME];
					int cPlayers = 0;

					/* Remember the name of each player */
					GetDlgItemText(hwnd, DID_PLAYER1, szPlayer, MAXPLAYERNAME);
					if (*szPlayer) wcscpy(PlayerInfo[cPlayers++].pszName, szPlayer);
					GetDlgItemText(hwnd, DID_PLAYER2, szPlayer, MAXPLAYERNAME);
					if (*szPlayer) wcscpy(PlayerInfo[cPlayers++].pszName, szPlayer);
					GetDlgItemText(hwnd, DID_PLAYER3, szPlayer, MAXPLAYERNAME);
					if (*szPlayer) wcscpy(PlayerInfo[cPlayers++].pszName, szPlayer);
					GetDlgItemText(hwnd, DID_PLAYER4, szPlayer, MAXPLAYERNAME);
					if (*szPlayer) wcscpy(PlayerInfo[cPlayers++].pszName, szPlayer);
					if (cPlayers != 0)
					{
						if (cPlayers != gcPlayers)
						{
							/* Adjust column width, depending on number of players */
							int i;
							for (i = 0; i < cPlayers; i++)
								PlayerInfo[i].nWidth = 150 / cPlayers;
							for (; i < MAXPLAYERS; i++)
								PlayerInfo[i].pszName[0] = 0;
						}

						gcPlayers = cPlayers; giPlayer = 0;
						CreatePlayerList();

						EndDialog(hwnd, 1);
					}
					return TRUE;
				}
			}
			break;

		default:
			SHInputDialog(hwnd, msg, wParam);
			break;
	}

	return FALSE;
}

/****************************************************************************
 *																			*
 * Function: DiceCountDlgProc												*
 *																			*
 * Purpose : Enter Dice Count dialog procedure.								*
 *																			*
 * History : Date	   Reason												*
 *			 00/00/00  Created												*
 *																			*
 ****************************************************************************/

static LRESULT CALLBACK DiceCountDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_INITDIALOG:
		{
			/* Initialize the silly dialog */
			SHINITDLGINFO shidi;
			shidi.dwMask = SHIDIM_FLAGS;
			shidi.dwFlags = SHIDIF_SIPDOWN|SHIDIF_SIZEDLGFULLSCREEN;
			shidi.hDlg = hwnd;
			SHInitDialog(&shidi);
			/**/
			SetWindowLong(hwnd, DWL_USER, lParam);
			/**/
			CreateDigitBar(hwnd, DID_DIGITBAR);
			SetDlgItemText(hwnd, DID_CAPTION, ((PROW)lParam)->pszText);
			CheckDigitBar(hwnd, DID_DIGITBAR, DID_DIGITBAR+3);	/* default */
			DiceCountDlgEnable(hwnd);
			return TRUE;
		}

		case WM_COMMAND:
		{
			int id;
			switch (id = GET_WM_COMMAND_ID(wParam, lParam))
			{
				case DID_NOSCORE:
				{
					/* Enter 'no score' */
					PROW pRow = (PROW)GetWindowLong(hwnd, DWL_USER);
					pRow->nScore[giPlayer] = 0;
					EndDialog(hwnd, 1);
					return TRUE;
				}

				case DID_DIGITBAR+1:
				case DID_DIGITBAR+2:
				case DID_DIGITBAR+3:
				case DID_DIGITBAR+4:
				case DID_DIGITBAR+5:
					CheckDigitBar(hwnd, DID_DIGITBAR, id);
					DiceCountDlgEnable(hwnd);
					return TRUE;

				case DID_SCORE:
				{
					int cDice = GetDigitBarValue(hwnd, DID_DIGITBAR);
					if (cDice)
					{
						/* Enter score */
						PROW pRow = (PROW)GetWindowLong(hwnd, DWL_USER);
						pRow->nScore[giPlayer] = cDice * pRow->nFactor;
						EndDialog(hwnd, 1);
					}
					return TRUE;
				}

				case IDCANCEL:
					EndDialog(hwnd, 0);
					return TRUE;
			}
			break;
		}
	}

	return FALSE;
}

/****************************************************************************
 * Subfunction: DiceCountDlgEnable											*
 ****************************************************************************/

static void DiceCountDlgEnable(HWND hwnd)
{
	int cDice = GetDigitBarValue(hwnd, DID_DIGITBAR);
	EnableWindow(GetDlgItem(hwnd, DID_SCORE), cDice != 0);
}

/****************************************************************************
 *																			*
 * Function: DiceOneKindDlgProc 											*
 *																			*
 * Purpose : Enter One Dice Kind dialog procedure.							*
 *																			*
 * History : Date	   Reason												*
 *			 00/00/00  Created												*
 *																			*
 ****************************************************************************/

static LRESULT CALLBACK DiceOneKindDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_INITDIALOG:
		{
			/* Initialize the silly dialog */
			SHINITDLGINFO shidi;
			WCHAR sz[20];
			shidi.dwMask = SHIDIM_FLAGS;
			shidi.dwFlags = SHIDIF_SIPDOWN|SHIDIF_SIZEDLGFULLSCREEN;
			shidi.hDlg = hwnd;
			SHInitDialog(&shidi);
			/**/
			SetWindowLong(hwnd, DWL_USER, lParam);
			/**/
			CreateDiceBar(hwnd, DID_DICEBAR1);
			SetDlgItemText(hwnd, DID_CAPTION, ((PROW)lParam)->pszText);
			wsprintf(sz, L"x %d", ((PROW)lParam)->nFactor);
			SetDlgItemText(hwnd, DID_FACTOR1, sz);
			DiceOneKindDlgEnable(hwnd);
			return TRUE;
		}

		case WM_COMMAND:
		{
			int id;
			switch (id = GET_WM_COMMAND_ID(wParam, lParam))
			{
				case DID_NOSCORE:
				{
					/* Enter 'no score' */
					PROW pRow = (PROW)GetWindowLong(hwnd, DWL_USER);
					pRow->nScore[giPlayer] = 0;
					EndDialog(hwnd, 1);
					return TRUE;
				}

				case DID_DICEBAR1+1:
				case DID_DICEBAR1+2:
				case DID_DICEBAR1+3:
				case DID_DICEBAR1+4:
				case DID_DICEBAR1+5:
				case DID_DICEBAR1+6:
					CheckDiceBar(hwnd, DID_DICEBAR1, id);
					DiceOneKindDlgEnable(hwnd);
					return TRUE;

				case DID_SCORE:
				{
					int nDice = GetDiceBarValue(hwnd, DID_DICEBAR1);
					if (nDice)
					{
						/* Enter score */
						PROW pRow = (PROW)GetWindowLong(hwnd, DWL_USER);
						pRow->nScore[giPlayer] = nDice * pRow->nFactor;
						EndDialog(hwnd, 1);
					}
					return TRUE;
				}

				case IDCANCEL:
					EndDialog(hwnd, 0);
					return TRUE;
			}
			break;
		}
	}

	return FALSE;
}

/****************************************************************************
 * Subfunction: DiceOneKindDlgEnable										*
 ****************************************************************************/

static void DiceOneKindDlgEnable(HWND hwnd)
{
	int nDice = GetDiceBarValue(hwnd, DID_DICEBAR1);
	EnableWindow(GetDlgItem(hwnd, DID_SCORE), nDice != 0);
}

/****************************************************************************
 *																			*
 * Function: DiceTwoKindDlgProc 											*
 *																			*
 * Purpose : Enter Two Dice Kinds dialog procedure.							*
 *																			*
 * History : Date	   Reason												*
 *			 00/00/00  Created												*
 *																			*
 ****************************************************************************/

static LRESULT CALLBACK DiceTwoKindDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch (msg)
	{
		case WM_INITDIALOG:
		{
			/* Initialize the silly dialog */
			SHINITDLGINFO shidi;
			WCHAR sz[20];
			shidi.dwMask = SHIDIM_FLAGS;
			shidi.dwFlags = SHIDIF_SIPDOWN|SHIDIF_SIZEDLGFULLSCREEN;
			shidi.hDlg = hwnd;
			SHInitDialog(&shidi);
			/**/
			SetWindowLong(hwnd, DWL_USER, lParam);
			/**/
			CreateDiceBar(hwnd, DID_DICEBAR1);
			CreateDiceBar(hwnd, DID_DICEBAR2);
			SetDlgItemText(hwnd, DID_CAPTION, ((PROW)lParam)->pszText);
			wsprintf(sz, L"x %d", ((PROW)lParam)->nFactor);
			SetDlgItemText(hwnd, DID_FACTOR1, sz);
			SetDlgItemText(hwnd, DID_FACTOR2, L"x 2");
			DiceTwoKindDlgEnable(hwnd);
			return TRUE;
		}

		case WM_COMMAND:
		{
			int id;
			switch (id = GET_WM_COMMAND_ID(wParam, lParam))
			{
				case DID_NOSCORE:
				{
					/* Enter 'no score' */
					PROW pRow = (PROW)GetWindowLong(hwnd, DWL_USER);
					pRow->nScore[giPlayer] = 0;
					EndDialog(hwnd, 1);
					return TRUE;
				}

				case DID_DICEBAR1+1:
				case DID_DICEBAR1+2:
				case DID_DICEBAR1+3:
				case DID_DICEBAR1+4:
				case DID_DICEBAR1+5:
				case DID_DICEBAR1+6:
					CheckDiceBar(hwnd, DID_DICEBAR1, id);
					DiceTwoKindDlgEnable(hwnd);
					return TRUE;

				case DID_DICEBAR2+1:
				case DID_DICEBAR2+2:
				case DID_DICEBAR2+3:
				case DID_DICEBAR2+4:
				case DID_DICEBAR2+5:
				case DID_DICEBAR2+6:
					CheckDiceBar(hwnd, DID_DICEBAR2, id);
					DiceTwoKindDlgEnable(hwnd);
					return TRUE;

				case DID_SCORE:
				case IDOK:
				{
					/* Enter score */
					int nDice1 = GetDiceBarValue(hwnd, DID_DICEBAR1);
					int nDice2 = GetDiceBarValue(hwnd, DID_DICEBAR2);
					if (nDice1 && nDice2 && nDice1 != nDice2)
					{
						PROW pRow = (PROW)GetWindowLong(hwnd, DWL_USER);
						pRow->nScore[giPlayer] = nDice1 * pRow->nFactor + nDice2 * 2;
						EndDialog(hwnd, 1);
					}
					return TRUE;
				}

				case IDCANCEL:
					EndDialog(hwnd, 0);
					return TRUE;
			}
			break;
		}
	}

	return FALSE;
}

/****************************************************************************
 * Subfunction: DiceTwoKindDlgEnable										*
 ****************************************************************************/

static void DiceTwoKindDlgEnable(HWND hwnd)
{
	int nDice1 = GetDiceBarValue(hwnd, DID_DICEBAR1);
	int nDice2 = GetDiceBarValue(hwnd, DID_DICEBAR2);
	EnableWindow(GetDlgItem(hwnd, DID_SCORE), nDice1 && nDice2 && nDice1 != nDice2);
}

/****************************************************************************
 *																			*
 * Function: DiceChanceDlgProc												*
 *																			*
 * Purpose : Enter Chance Score dialog procedure.							*
 *																			*
 * History : Date	   Reason												*
 *			 00/00/00  Created												*
 *																			*
 ****************************************************************************/

static LRESULT CALLBACK DiceChanceDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_INITDIALOG:
		{
			/* Initialize the silly dialog */
			SHINITDLGINFO shidi;
			shidi.dwMask = SHIDIM_FLAGS;
			shidi.dwFlags = SHIDIF_SIPDOWN|SHIDIF_SIZEDLGFULLSCREEN;
			shidi.hDlg = hwnd;
			SHInitDialog(&shidi);
			/**/
			SetWindowLong(hwnd, DWL_USER, lParam);
			/**/
			CreateDiceBar(hwnd, DID_DICEBAR1);
			CreateDiceBar(hwnd, DID_DICEBAR2);
			CreateDiceBar(hwnd, DID_DICEBAR3);
			CreateDiceBar(hwnd, DID_DICEBAR4);
			CreateDiceBar(hwnd, DID_DICEBAR5);
			SetDlgItemText(hwnd, DID_CAPTION, ((PROW)lParam)->pszText);
			DiceChanceDlgEnable(hwnd);
			return TRUE;
		}

		case WM_COMMAND:
		{
			int id;
			switch (id = GET_WM_COMMAND_ID(wParam, lParam))
			{
				case DID_DICEBAR1+1:
				case DID_DICEBAR1+2:
				case DID_DICEBAR1+3:
				case DID_DICEBAR1+4:
				case DID_DICEBAR1+5:
				case DID_DICEBAR1+6:
					CheckDiceBar(hwnd, DID_DICEBAR1, id);
					DiceChanceDlgEnable(hwnd);
					return TRUE;

				case DID_DICEBAR2+1:
				case DID_DICEBAR2+2:
				case DID_DICEBAR2+3:
				case DID_DICEBAR2+4:
				case DID_DICEBAR2+5:
				case DID_DICEBAR2+6:
					CheckDiceBar(hwnd, DID_DICEBAR2, id);
					DiceChanceDlgEnable(hwnd);
					return TRUE;

				case DID_DICEBAR3+1:
				case DID_DICEBAR3+2:
				case DID_DICEBAR3+3:
				case DID_DICEBAR3+4:
				case DID_DICEBAR3+5:
				case DID_DICEBAR3+6:
					CheckDiceBar(hwnd, DID_DICEBAR3, id);
					DiceChanceDlgEnable(hwnd);
					return TRUE;

				case DID_DICEBAR4+1:
				case DID_DICEBAR4+2:
				case DID_DICEBAR4+3:
				case DID_DICEBAR4+4:
				case DID_DICEBAR4+5:
				case DID_DICEBAR4+6:
					CheckDiceBar(hwnd, DID_DICEBAR4, id);
					DiceChanceDlgEnable(hwnd);
					return TRUE;

				case DID_DICEBAR5+1:
				case DID_DICEBAR5+2:
				case DID_DICEBAR5+3:
				case DID_DICEBAR5+4:
				case DID_DICEBAR5+5:
				case DID_DICEBAR5+6:
					CheckDiceBar(hwnd, DID_DICEBAR5, id);
					DiceChanceDlgEnable(hwnd);
					return TRUE;

				case DID_SCORE:
				case IDOK:
				{
					int nDice1 = GetDiceBarValue(hwnd, DID_DICEBAR1);
					int nDice2 = GetDiceBarValue(hwnd, DID_DICEBAR2);
					int nDice3 = GetDiceBarValue(hwnd, DID_DICEBAR3);
					int nDice4 = GetDiceBarValue(hwnd, DID_DICEBAR4);
					int nDice5 = GetDiceBarValue(hwnd, DID_DICEBAR5);
					if (nDice1 && nDice2 && nDice3 && nDice4 && nDice5)
					{
						PROW pRow = (PROW)GetWindowLong(hwnd, DWL_USER);
						pRow->nScore[giPlayer] = nDice1 + nDice2 + nDice3 + nDice4 + nDice5;
						EndDialog(hwnd, 1);
					}
					return TRUE;
				}

				case IDCANCEL:
					EndDialog(hwnd, 0);
					return TRUE;
			}
			break;
		}
	}

	return FALSE;
}

/****************************************************************************
 * Subfunction: DiceChanceDlgEnable											*
 ****************************************************************************/

static void DiceChanceDlgEnable(HWND hwnd)
{
	int nDice1 = GetDiceBarValue(hwnd, DID_DICEBAR1);
	int nDice2 = GetDiceBarValue(hwnd, DID_DICEBAR2);
	int nDice3 = GetDiceBarValue(hwnd, DID_DICEBAR3);
	int nDice4 = GetDiceBarValue(hwnd, DID_DICEBAR4);
	int nDice5 = GetDiceBarValue(hwnd, DID_DICEBAR5);
	EnableWindow(GetDlgItem(hwnd, DID_SCORE), nDice1 && nDice2 && nDice3 && nDice4 && nDice5);
}

/****************************************************************************
 *																			*
 * Function: DiceConfirmDlgProc 											*
 *																			*
 * Purpose : Confirm a fixed score, for example 'Small Straight'			*
 *																			*
 * History : Date	   Reason												*
 *			 00/00/00  Created												*
 *																			*
 ****************************************************************************/

static LRESULT CALLBACK DiceConfirmDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_INITDIALOG:
		{
			/* Initialize the silly dialog */
			SHINITDLGINFO shidi;
			shidi.dwMask = SHIDIM_FLAGS;
			shidi.dwFlags = SHIDIF_SIPDOWN|SHIDIF_SIZEDLGFULLSCREEN;
			shidi.hDlg = hwnd;
			SHInitDialog(&shidi);
			/**/
			SetWindowLong(hwnd, DWL_USER, lParam);
			/**/
			SetDlgItemText(hwnd, DID_CAPTION, ((PROW)lParam)->pszText);
			return TRUE;
		}

		case WM_COMMAND:
		{
			switch (GET_WM_COMMAND_ID(wParam, lParam))
			{
				case DID_NOSCORE:
				{
					/* Enter 'no score' */
					PROW pRow = (PROW)GetWindowLong(hwnd, DWL_USER);
					pRow->nScore[giPlayer] = 0;
					EndDialog(hwnd, 1);
					return TRUE;
				}

				case DID_SCORE:
				case IDOK:
				{
					/* Enter score */
					PROW pRow = (PROW)GetWindowLong(hwnd, DWL_USER);
					pRow->nScore[giPlayer] = pRow->nFactor;
					EndDialog(hwnd, 1);
					return TRUE;
				}

				case IDCANCEL:
					EndDialog(hwnd, 0);
					return TRUE;
			}
			break;
		}
	}

	return FALSE;
}

/****************************************************************************
 *																			*
 * Function: CreateDigitBar 												*
 *																			*
 * Purpose : Create digit bar, for selecting number of dices.				*
 *																			*
 * History : Date	   Reason												*
 *			 00/00/00  Created												*
 *																			*
 ****************************************************************************/

static void CreateDigitBar(HWND hwnd, int id)
{
	HWND hwndTB = GetDlgItem(hwnd, id);
	TBADDBITMAP tbab;
	int i;

	//
	// Version control and size of a single button.
	//
	ToolBar_ButtonStructSize(hwndTB);
	ToolBar_SetBitmapSize(hwndTB, 32, 32);

	//
	// Add a bitmap, with all buttons, to the toolbar.
	//
	tbab.hInst = ghInstance;
	tbab.nID = IDR_BMP_DIGITBAR;
	ToolBar_AddBitmap(hwndTB, 6, &tbab);

	//
	// Add buttons to the toolbar.
	//
	for (i = 0; i < 5; i++)
	{
		TBBUTTON tbb;
		tbb.iBitmap = i;
		tbb.idCommand = id + 1 + i;
		tbb.fsState = TBSTATE_ENABLED;
		tbb.fsStyle = TBSTYLE_BUTTON;
		tbb.iString = 0;
		ToolBar_AddButtons(hwndTB, 1, &tbb);
	}

	ToolBar_AutoSize(hwndTB);
}

/****************************************************************************
 *																			*
 * Function: CheckDigitBar													*
 *																			*
 * Purpose : Select a dice in a digit bar.									*
 *																			*
 * History : Date	   Reason												*
 *			 00/00/00  Created												*
 *																			*
 ****************************************************************************/

static void CheckDigitBar(HWND hwnd, int id, int idCheck)
{
	HWND hwndTB = GetDlgItem(hwnd, id);
	int i;

	for (i = 0; i < 5; i++)
	{
		int idDigit = id + 1 + i;
		ToolBar_CheckButton(hwndTB, idDigit, idDigit == idCheck);
	}
}

/****************************************************************************
 *																			*
 * Function: GetDigitBarValue												*
 *																			*
 * Purpose : Return the selected digit in a digit bar.						*
 *																			*
 * History : Date	   Reason												*
 *			 00/00/00  Created												*
 *																			*
 ****************************************************************************/

static int GetDigitBarValue(HWND hwnd, int id)
{
	HWND hwndTB = GetDlgItem(hwnd, id);
	int i;

	for (i = 0; i < 5; i++)
	{
		if (ToolBar_IsButtonChecked(hwndTB, id + 1 + i))
			return 1 + i;
	}

	return 0;
}

/****************************************************************************
 *																			*
 * Function: CreateDiceBar													*
 *																			*
 * Purpose : Create a dice bar, for selecting a dice.						*
 *																			*
 * History : Date	   Reason												*
 *			 00/00/00  Created												*
 *																			*
 ****************************************************************************/

static void CreateDiceBar(HWND hwnd, int id)
{
	HWND hwndTB = GetDlgItem(hwnd, id);
	TBADDBITMAP tbab;
	int i;

	//
	// Version control and size of a single button.
	//
	ToolBar_ButtonStructSize(hwndTB);
	ToolBar_SetBitmapSize(hwndTB, 26, 26);

	//
	// Add a bitmap, with all buttons, to the toolbar.
	//
	tbab.hInst = ghInstance;
	tbab.nID = IDR_BMP_DICEBAR;
	ToolBar_AddBitmap(hwndTB, 6, &tbab);

	//
	// Add buttons to the toolbar.
	//
	for (i = 0; i < 6; i++)
	{
		TBBUTTON tbb;
		tbb.iBitmap = i;
		tbb.idCommand = id + 1 + i;
		tbb.fsState = TBSTATE_ENABLED;
		tbb.fsStyle = TBSTYLE_BUTTON;
		tbb.iString = 0;
		ToolBar_AddButtons(hwndTB, 1, &tbb);
	}

	ToolBar_AutoSize(hwndTB);
}

/****************************************************************************
 *																			*
 * Function: CheckDiceBar													*
 *																			*
 * Purpose : Select a dice in a dice bar.									*
 *																			*
 * History : Date	   Reason												*
 *			 00/00/00  Created												*
 *																			*
 ****************************************************************************/

static void CheckDiceBar(HWND hwnd, int id, int idCheck)
{
	HWND hwndTB = GetDlgItem(hwnd, id);
	int i;

	for (i = 0; i < 6; i++)
	{
		int idDice = id + 1 + i;
		ToolBar_CheckButton(hwndTB, idDice, idDice == idCheck);
	}
}

/****************************************************************************
 *																			*
 * Function: GetDiceBarValue												*
 *																			*
 * Purpose : Return the selected dice in a dice bar.						*
 *																			*
 * History : Date	   Reason												*
 *			 00/00/00  Created												*
 *																			*
 ****************************************************************************/

static int GetDiceBarValue(HWND hwnd, int id)
{
	HWND hwndTB = GetDlgItem(hwnd, id);
	int i;

	for (i = 0; i < 6; i++)
	{
		if (ToolBar_IsButtonChecked(hwndTB, id + 1 + i))
			return 1 + i;
	}

	return 0;
}

