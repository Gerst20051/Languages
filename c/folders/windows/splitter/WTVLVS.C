//simple SDI-windows vertical splitter for example
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>

#define IDM_EXIT 100

LRESULT WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);

char szAppName[] = "WTVLVS";
char szFrameClass[] = "cFrame";
HANDLE hInst;
HWND hFrame, hWndLV, hWndTV;
HMENU hMenu, hMenuTS;
HTREEITEM hRoot;

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wc;
	MSG msg;

	hInst = hInstance;
	wc.style	= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc	= (WNDPROC) WndProc;
	wc.cbClsExtra	= 0;
	wc.cbWndExtra	= 0;
	wc.hInstance	= hInstance;
	wc.hIcon	= LoadIcon(NULL, IDI_APPLICATION);
	//wc.hCursor	= LoadCursor(NULL, IDC_ARROW);
	wc.hCursor	= LoadCursor(NULL, IDC_SIZEWE);	//for splitter
	wc.hbrBackground= (HBRUSH)COLOR_WINDOW;
	wc.lpszMenuName	= NULL;
	wc.lpszClassName= szFrameClass;

	if (!RegisterClass(&wc)) return 0;

	hFrame = CreateWindow(szFrameClass, szAppName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInst, NULL);
	if(!hFrame) return 0;
	ShowWindow(hFrame, nCmdShow);
	UpdateWindow(hFrame);

	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	 return msg.wParam;
}

LRESULT WINAPI WndProc(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	int i, rc;
	LV_COLUMN lvc;
	LV_ITEM lvi;
	TV_ITEM tvi;
	TV_INSERTSTRUCT tvins;
	static BOOL bDrag = FALSE;	//for splitter
	static UINT uDrag = 100;	//for splitter
	RECT rect;

	switch(wMsg) {
	case WM_COMMAND:
			switch(wParam) {
			case IDM_EXIT:
				PostMessage(hWnd, WM_CLOSE, 0, 0L);
				return(0);
		}
		break;
	//these are for splitter, begin
	case WM_SIZE:
		MoveWindow(hWndTV, 0, 0, uDrag-1, HIWORD(lParam), TRUE);
		MoveWindow(hWndLV, uDrag+1, 0, LOWORD(lParam)-101, HIWORD(lParam), TRUE);
		return 0;
	case WM_LBUTTONDOWN:
		SetCapture(hWnd);
		bDrag = TRUE;
		return 0;
	case WM_LBUTTONUP:
		ReleaseCapture();
		bDrag = FALSE;
		return 0;
	case WM_MOUSEMOVE:
		if (bDrag) {
			GetClientRect(hWnd, &rect);
			if ((LOWORD(lParam)>50) && (LOWORD(lParam)<(rect.right)-50)) {
				MoveWindow(hWndTV, 0, 0, LOWORD(lParam)-1, rect.bottom, TRUE);
				MoveWindow(hWndLV, LOWORD(lParam)+1, 0,
				rect.right-LOWORD(lParam)+1, rect.bottom, TRUE);
				uDrag = LOWORD(lParam);
			}
		}
		return 0;
	//these was for splitter, end
	case WM_CREATE:
		InitCommonControls();
		hMenu = CreateMenu();
		hMenuTS = CreatePopupMenu();
		AppendMenu(hMenuTS, MF_ENABLED | MF_STRING, IDM_EXIT, "&Exit");
		AppendMenu(hMenu, MF_ENABLED | MF_POPUP, (UINT)hMenuTS, "&File");
		SetMenu(hWnd, hMenu);
		hWndLV = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, NULL,
			WS_CHILD | WS_VISIBLE | WS_TABSTOP | LVS_REPORT
			,0, 0, 0, 0,
			hWnd, 0, hInst, NULL);
		ZeroMemory(&lvc, sizeof(lvc));
		lvc.mask = LVCF_TEXT;
		//lvc.fmt = LVCF_TEXT;
		lvc.cx = 20;
		lvc.cchTextMax = 11;
		lvc.iSubItem = 6;
		for (i=0; i<6; i++) {
			lvc.iSubItem = i;
			lvc.pszText = "C";
			ListView_InsertColumn(hWndLV, i, &lvc);
			ListView_SetColumnWidth(hWndLV, i, 50);
			//ListView_InsertItem(hWndLV, &lvi);
		}
		hWndTV = CreateWindowEx(WS_EX_CLIENTEDGE, WC_TREEVIEW, NULL,
			WS_CHILD | WS_VISIBLE | WS_TABSTOP |
			TVS_HASLINES | TVS_LINESATROOT |
			TVS_HASBUTTONS
			,0, 0, 0, 0,
			hWnd, 0, hInst, NULL);
		ZeroMemory(&tvins, sizeof(tvins));
		tvins.item.mask = TVIF_TEXT;
		//tvins.item.pszText = "Root";
		tvins.item.cchTextMax = 10;
		hRoot = TreeView_InsertItem(hWndTV, &tvins);
		SetFocus(hWndTV);
		return(0);
	case WM_DESTROY:
		PostQuitMessage(0);
		return(0);
    default:
		return DefWindowProc(hWnd, wMsg, wParam, lParam);
    }
	return 0;
}
