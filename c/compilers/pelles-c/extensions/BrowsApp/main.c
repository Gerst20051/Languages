// main.c
//
// Much of the functionality has not been tested.

#define  COBJMACROS

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <oleauto.h>
#include <exdisp.h>
#include "exdispid.h"
#include <stdio.h>
#include "iehelp.h"
#include "mainres.h"
#include "main.h"

//
// Link with ole32.lib oleaut32.lib and uuid.lib
//

HINSTANCE hInst;	// Instance handle
HWND hwndMain;	// Main window handle
IWebBrowser2 *iBrowser;	// Browser Interface pointer

#define NUM_TOOLBAR_BUTTONS 10
#define TBSTATE_DISABLED 0

char title[] = "Browser  ";
char bigtitle[MAX_PATH + sizeof(title) + 1];
char g_strFileFilter[90];	// filters for GetOpenFileName()
char buffer[MAX_PATH];	// for GetOpenFileName()

// forward declarations
HWND CreateToolBar(HWND hWndParent, HINSTANCE hInst);
HWND CreateStatusBar(HWND hWndParent, char *initialText, int nrOfParts);
void SizeStatusPanels(HWND hWndParent, int nrOfParts);
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int CallBack(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case DISPID_DOWNLOADBEGIN:
			SendMessage(GetDlgItem(hwndMain, ID_TOOLBAR), TB_ENABLEBUTTON, IDM_STOP, MAKELONG(TRUE, 0));
			break;

		case DISPID_TITLECHANGE:
			sprintf(bigtitle, "%s %s", title, "-> ");
			strncat(bigtitle, (char *)lParam, MAX_PATH - 1);	// strncat will not over-run buffer
			SetWindowText(hwndMain, bigtitle);
			break;

		case DISPID_NEWWINDOW2:
			return -1;	// stop a new Explorer window from openning.

		case DISPID_STATUSTEXTCHANGE:
			SendMessage(GetDlgItem(hwndMain, ID_STATUSBAR), SB_SETTEXT, 0, (long)lParam);
			break;

		case DISPID_PROGRESSCHANGE:
			if (wParam > 0)
			{
				sprintf(buffer, "%d%s", wParam, "%");
				SendMessage(GetDlgItem(hwndMain, ID_STATUSBAR), SB_SETTEXT, 1, (long)buffer);
			}
			else
			{	// clear the status bar text when the percentage returns to zero
				sprintf(buffer, "%s", "");
				SendMessage(GetDlgItem(hwndMain, ID_STATUSBAR), SB_SETTEXT, 1, (long)buffer);
			}
			break;

		case DISPID_DOCUMENTCOMPLETE:
			sprintf(bigtitle, "%s %s", title, "-> ");
			strncat(bigtitle, GetLocationURL(iBrowser), MAX_PATH - 1);	// strncat will not over-run buffer
			SetWindowText(hwndMain, bigtitle);
			SendMessage(GetDlgItem(hwndMain, ID_TOOLBAR), TB_ENABLEBUTTON, IDM_STOP, MAKELONG(FALSE, 0));
			break;
	}
	return 0;
}

/*-------------------------------------------------------------------------------

  FUNCTION		: OpenGetFileName(char* buffer, long buflen, char *filter)

  PURPOSE		: Displays GetOpenFileName() dialog after filling OFN struct

  RETURN     	: Return TRUE or FALSE depending if a file was selected

  PARAMS     	: *buffer - pointer to the buffer for filename
   				: buflen  - max length of string for buffer
	   			: *filter - pointer to the filletr string

-------------------------------------------------------------------------------*/
long OpenGetFileName(char *buffer, long buflen, char *filter)
{

	long i = 0;
	OPENFILENAME ofn;

	// Fill extensions ',' With NULL
	while (filter[i])
	{
		if (filter[i] == ',')
			filter[i] = 0;
		i++;
	}
	filter[i++] = 0;
	filter[i++] = 0;	// Two NULL's to end it

	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetActiveWindow();
	ofn.hInstance = GetModuleHandle(NULL);
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = buffer;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0L;
	ofn.nMaxFile = buflen;
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = buffer;
	ofn.lpstrTitle = "Open";

	ofn.lpstrDefExt = "txt";
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (TRUE == GetOpenFileName(&ofn))
	{
		return 1;
	}
	return 0;
}

BOOL STDMETHODCALLTYPE AboutDlg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CLOSE:
			EndDialog(hwnd, 0);
			return 1;
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDOK:
					EndDialog(hwnd, 1);
					return 1;
			}
			break;
	}
	return 0;
}

void MainWndProc_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(DLG_ABOUT), hwnd, AboutDlg);
			break;

		case IDM_REFRESH:
			Refresh(iBrowser);
			break;

		case IDM_FORWARD:
			GoForward(iBrowser);
			break;

		case IDM_BACK:
			GoBack(iBrowser);
			break;

		case IDM_STOP:
			StopDownload(iBrowser);
			SendMessage(GetDlgItem(hwndMain, ID_TOOLBAR), TB_ENABLEBUTTON, IDM_STOP, MAKELONG(FALSE, 0));
			break;

		case IDM_JOHN:
			NavigateURL(iBrowser, L"http://www.johnfindlay.plus.com/pellesc/");
			break;

		case IDM_HOME:
			NavigateURL(iBrowser, L"http://www.johnfindlay.plus.com/pellesc/");
			break;

		case IDM_PELLESC:
			NavigateURL(iBrowser, L"http://www.smorgasbordet.com/pellesc/");
			break;

		case IDM_SEARCH:
			NavigateURL(iBrowser, L"http://www.google.com/");
			break;

		case IDM_LOCAL:
			strcpy(g_strFileFilter, "HTML Files,*.html;*.htm");
			if (OpenGetFileName(buffer, sizeof(buffer) - 1, g_strFileFilter))
			{
				NavigateURL(iBrowser, AnsiToUnicode(buffer));
			}
			break;

		case IDM_FORUM:
			NavigateURL(iBrowser, L"http://smorgasbordet.com/phpBB2/index.php");
			break;

		case IDM_EXIT:
			PostMessage(hwnd, WM_CLOSE, 0, 0);
			break;
	}
}

/*-------------------------------------------------------------------------------

  FUNCTION		: SizeStatusPanels(hWndParent, hWndStatusbar, nrOfParts)

  PURPOSE		: Sets the size and number of panels for statusbar.

  RETURN     	: NONE

  PARAMS     	: hWndParent    - handle of parent Window
	   			: hWndStatusbar - handle of statusbar
				: nrOfParts 	- numbar of panels

-------------------------------------------------------------------------------*/
void SizeStatusPanels(HWND hWndParent, int nrOfParts)
{

	int ptArray[40];	// Array defining the number of parts/sections
	RECT rect;
	HWND hStatus = GetDlgItem(hWndParent, ID_STATUSBAR);

	// Fill in the ptArray
	GetClientRect(hStatus, &rect);

	ptArray[nrOfParts - 1] = rect.right;

	ptArray[0] = rect.right - (rect.right / 3);
	ptArray[1] = rect.right - (rect.right / 6);
	ptArray[2] = rect.right;

	SendMessage(hStatus, SB_SETPARTS, nrOfParts, (LPARAM) (LPINT)ptArray);

}
/*-------------------------------------------------------------------------------

  FUNCTION		: CreateStatusBarWnd(HWND, char *initialText, int nrOfParts)

  PURPOSE		: Create the status bar

  RETURN     	: Handle of status bar. 0 on fail

  PARAMS     	: hWndParent   - handle of parent Window
	   			: *initialText - pointer to text that will be shown initially
	   			: nrOfParts    - how many panels for statusbar

-------------------------------------------------------------------------------*/
HWND CreateStatusBar(HWND hWndParent, char *initialText, int nrOfParts)
{

	HWND hWndStatusbar;

	hWndStatusbar = CreateStatusWindow(WS_CHILD | WS_VISIBLE | WS_BORDER | SBARS_SIZEGRIP, initialText, hWndParent, ID_STATUSBAR);
	if (hWndStatusbar)
	{
		SizeStatusPanels(hWndParent, nrOfParts);
		return hWndStatusbar;
	}

	return 0;
}

/*-------------------------------------------------------------------------------

  FUNCTION		: TBButton(TBBUTTON *, iBitmap, idCommand, fsState, fsStyle)

  PURPOSE		: Fills in a TBBUTTON struct with passed params for use with.
				  a toolbar.

  RETURN     	: NONE

  PARAMS     	: *tbb      - pointer to the struct
	   			: iBitmap   - Zero-based index of the button image
	   			: idCommand - Command identifier associated with the button.
							  This identifier is used in a WM_COMMAND message
							  when the button is chosen.
				: fsState   - Button state flags
				: fsStyle   - Button style flags

-------------------------------------------------------------------------------*/
void TBButton(TBBUTTON * tbb, int iBitmap, int idCommand, int fsState, int fsStyle, char *str)
{

	// Button
	tbb->iBitmap = iBitmap;
	tbb->idCommand = idCommand;
	tbb->fsState = fsState;
	tbb->fsStyle = fsStyle;
	tbb->iString = (int)str;
}

/*-------------------------------------------------------------------------------

  FUNCTION		: CreateToolBarWnd(HWND hWndParent, HINSTANCE hInst)

  PURPOSE		: Create a toolbar.

  RETURN     	: Toolbar's handle on success, '0' on fail

  PARAMS     	: hWndParent - handle of Parent Window
	   			: hInst	 	 - Instqnce handle of app.

-------------------------------------------------------------------------------*/
HWND CreateToolBar(HWND hWndParent, HINSTANCE hInst)
{
	HWND hWndTB;
	TBADDBITMAP tbab;
	TBBUTTON tbb[NUM_TOOLBAR_BUTTONS];

	// clean memory before using it
	memset(&tbb[0], 0, sizeof(TBBUTTON) * NUM_TOOLBAR_BUTTONS);
	// make buttons
	TBButton(&tbb[0], 0, 0, 0, TBSTYLE_SEP, "");
	TBButton(&tbb[1], TD_BACK, IDM_BACK, TBSTATE_ENABLED, TBSTYLE_BUTTON, "Back");
	TBButton(&tbb[2], TD_FORWARD, IDM_FORWARD, TBSTATE_ENABLED, TBSTYLE_BUTTON, "  Forward  ");
	TBButton(&tbb[3], TD_STOP, IDM_STOP, TBSTATE_DISABLED, TBSTYLE_BUTTON, "Stop");
	TBButton(&tbb[4], 0, 0, 0, TBSTYLE_SEP, "");
	TBButton(&tbb[5], TD_REFRESH, IDM_REFRESH, TBSTATE_ENABLED, TBSTYLE_BUTTON, "Refresh");
	TBButton(&tbb[6], 0, 0, 0, TBSTYLE_SEP, "");
	TBButton(&tbb[7], TD_HOME, IDM_HOME, TBSTATE_ENABLED, TBSTYLE_BUTTON, "Home");
	TBButton(&tbb[8], TD_SEARCH, IDM_SEARCH, TBSTATE_ENABLED, TBSTYLE_BUTTON, "Google");
	TBButton(&tbb[9], 0, 0, 0, TBSTYLE_SEP, "");

	// Create the toolbar.
	hWndTB = CreateWindowEx(0, TOOLBARCLASSNAME, (LPSTR)NULL, WS_CHILD | WS_BORDER | CCS_TOP | TBSTYLE_TOOLTIPS, 0, 0, 0, 0, hWndParent, (HMENU)ID_TOOLBAR, hInst, NULL);

	if (!hWndTB)
		return 0;

	// Send the TB_BUTTONSTRUCTSIZE message, which is required for backward compatibility.
	SendMessage(hWndTB, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

	tbab.hInst = 0;
	tbab.nID = (UINT)LoadImage(hInst, MAKEINTRESOURCE(ID_BITMAP), IMAGE_BITMAP, 0, 0, LR_LOADMAP3DCOLORS);

	// Tell the toolbar the bitmap size
	SendMessage(hWndTB, TB_SETBITMAPSIZE, 0, MAKELONG(18, 16));

	// Add the bitmap containing button images to the toolbar.
	SendMessage(hWndTB, TB_ADDBITMAP, (WPARAM)1, (LPARAM) & tbab);

	SendMessage(hWndTB, TB_ADDBUTTONS, NUM_TOOLBAR_BUTTONS, (LPARAM) & tbb);

	SetWindowLong(hWndTB, GWL_STYLE, GetWindowLong(hWndTB, GWL_STYLE) | TBSTYLE_FLAT);

	ShowWindow(hWndTB, SW_SHOW);
	return hWndTB;
}

static BOOL InitApplication(void)
{
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_WIN95_CLASSES;

	InitCommonControlsEx(&icex);

	WNDCLASS wc;

	memset(&wc, 0, sizeof(WNDCLASS));
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wc.lpfnWndProc = (WNDPROC)MainWndProc;
	wc.hInstance = hInst;
	wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wc.lpszClassName = "WebBrowserClass";
	wc.lpszMenuName = MAKEINTRESOURCE(IDMAINMENU);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(1));
	if (!RegisterClass(&wc))
		return 0;

	return 1;
}

RECT rc;
RECT rcT;
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CREATE:
			CreateToolBar(hwnd, hInst);
			CreateStatusBar(hwnd, "Ready", 3);
			return 0;

		case WM_SIZE:
			SendMessage(GetDlgItem(hwnd, ID_TOOLBAR), WM_SIZE, LOWORD(lParam), HIWORD(lParam));
			SendMessage(GetDlgItem(hwnd, ID_STATUSBAR), WM_SIZE, LOWORD(lParam), HIWORD(lParam));
			SizeStatusPanels(hwnd, 3);
			if (iBrowser)
			{
				GetClientRect(hwnd, &rc);
				GetClientRect(GetDlgItem(hwnd, ID_TOOLBAR), &rcT);
				rc.top = rcT.bottom;
				GetClientRect(GetDlgItem(hwnd, ID_STATUSBAR), &rcT);
				rc.bottom -= rcT.bottom;
				ReSizeBrowserCtrl(iBrowser, &rc);
			}
			break;

		case WM_COMMAND:
			HANDLE_WM_COMMAND(hwnd, wParam, lParam, MainWndProc_OnCommand);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
	MSG msg;
	HANDLE hAccelTable;

	hInst = hInstance;
	if (!InitApplication())
		return 0;

	hAccelTable = LoadAccelerators(hInst, MAKEINTRESOURCE(IDACCEL));

	hwndMain = CreateWindowEx(WS_EX_WINDOWEDGE, "WebBrowserClass", title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInst, NULL);

	if (!hwndMain)
		return 0;

	ShowWindow(hwndMain, SW_SHOW);

	OleInitialize(NULL);	// use this instead of CoInitialize

	// Creation of a window, RegisterClass, etc
	// Window where the browser will be displayed is hwndMain
	iBrowser = CreateEmbeddedWebControl(hwndMain);
	if (iBrowser)
		SetUserCallBack(iBrowser, CallBack);
	else
	{
		MessageBox(0, "Could not create Embedded Control!", "Error!", MB_OK);
		goto ende;
	}

	SetAsDropTarget(iBrowser, TRUE);

	SendMessage(hwndMain, WM_SIZE, 0, 0);	// pseudo WM_SIZE to resize Browser and toolbar etc.

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(hwndMain, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	if (iBrowser)
		CloseEmbeddedWebControl(iBrowser);
  ende:
	OleUninitialize();
	return 0;
}
