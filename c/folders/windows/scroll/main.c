#include<windows.h>
#include<windowsx.h>
#include<stdio.h>
#include<string.h>
#include<Tchar.h>
#include "main.h"
#include "scroll.h"

static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow);

HWND hwndMain;

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;

	if (!hPrevInstance)
	{
		WNDCLASS wc;

		wc.lpszClassName = _T("winClass");
		wc.lpfnWndProc = MainWndProc;
		wc.style = CS_OWNDC|CS_VREDRAW|CS_HREDRAW;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDR_ICO_MAIN));
		wc.hCursor = LoadCursor(NULL, IDC_CROSS);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
		wc.lpszMenuName = MAKEINTRESOURCE(IDR_MNU_MAIN);
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;

		if (!RegisterClass(&wc))
			return 1;
	}
	for(int n=0;n<3;n++){
		hwndMain=hwnd = CreateWindow(_T("winClass"),
			_T("win Program"),
			WS_OVERLAPPEDWINDOW|WS_HSCROLL|WS_VSCROLL,
			n*200,
			0,
			200,
			200,
			NULL,
			NULL,
			hInstance,
			NULL
			    );
		if (!hwnd) return 1;
		ShowWindow(hwnd, nCmdShow);
		UpdateWindow(hwnd);
	}
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(0==HandleScrollMessages(hwnd,msg,wParam,lParam)){
		return 0;
	}
	switch (msg)
	{
	case WM_CREATE:
		{
			RECT rc;
			POINT pt;
			GetClientRect(hwnd,&rc);
			pt.x=100;
			pt.y=100;
			SetScrolls(hwnd,&rc,&pt);
		}
		return FALSE;
	case WM_SIZE:
		{
			RECT rc;
			POINT pt;
			GetScrolls(hwnd,NULL,&pt);
			GetClientRect(hwnd,&rc);
			InvalidateRect(hwnd,&rc,TRUE);
			SetScrolls(hwnd,&rc,&pt);
			UpdateWindow(hwnd);
		}
		return FALSE;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;

			RECT rcDraw,rcScroll;

			POINT pt;
			GetScrolls(hwnd,&rcScroll,&pt);

			rcDraw.left  =pt.x;
			rcDraw.top   =pt.y;
			rcDraw.right =rcScroll.right;
			rcDraw.bottom=rcScroll.bottom;

			HDC hdc=BeginPaint(hwnd,&ps);
			SelectObject(hdc,GetStockObject(GRAY_BRUSH));
			Ellipse(hdc,rcDraw.left-50,rcDraw.top-50,rcDraw.left+200,rcDraw.top+100);
			SetTextColor(hdc,RGB(0,0,255));
			DrawText(hdc,"Hello Scrolling!"
				    "\r\n - scroll with scrollbars"
				    "\r\n - or with the keyboard!"
				    "\r\n - or with the cursor on the"
				  	"\r\n     client area!",
				-1,&rcDraw,0);
			EndPaint(hwnd,&ps);
		}
		return FALSE;

	case WM_COMMAND:
		switch LOWORD(wParam)
		{


		}
		return FALSE;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		return FALSE;

	case WM_DESTROY:
		PostQuitMessage(0);
		return FALSE;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}
