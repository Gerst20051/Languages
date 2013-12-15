// File: htmlColor.c
// Purpose: Select colors for using in HTML pages.
// Author: Philippe Berthault
// Free to use and to modify without any restrictions.

/*
 * Either define WIN32_LEAN_AND_MEAN, or one or more of NOCRYPT,
 * NOSERVICE, NOMCX and NOIME, to decrease compile time (if you
 * don't need these defines -- see windows.h).
 */

#define WIN32_LEAN_AND_MEAN
/* #define NOCRYPT */
/* #define NOSERVICE */
/* #define NOMCX */
/* #define NOIME */

#include <stdio.h>
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <commdlg.h>
#include <tchar.h>
#include "htmlColor.h"

// Prototypes
static LRESULT CALLBACK MainDlgProc(HWND, UINT, WPARAM, LPARAM);
static void RedrawViewer(HWND);
static void SelectColor(HWND, int id);
static void CopyHtmlValue(HWND, int);

// Global variables
static HWND		sliderTextR, sliderTextG, sliderTextB, sliderBgR, sliderBgG, sliderBgB;
static HWND		htmlTextRGB, htmlBgRGB, viewer;
static int		colorTextR, colorTextG, colorTextB, colorBgR, colorBgG, colorBgB;
static int		paintDone;
static COLORREF colorRef[16];

// Application entry point.
int PASCAL
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	WNDCLASSEX wcx;

	// Initialize common controls. Also needed for MANIFEST's.
	InitCommonControls();

	// Get system dialog information.
	ZeroMemory(&wcx, sizeof(wcx));
	wcx.cbSize = sizeof(wcx);
	if (!GetClassInfoEx(NULL, MAKEINTRESOURCE(32770), &wcx))
		return 0;

	// Add our own stuff.
	wcx.hInstance = hInstance;
	wcx.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDR_ICO_MAIN));
	wcx.lpszClassName = _T("htmlColorClass");
	if (!RegisterClassEx(&wcx))
		return 0;

	// The user interface is a modal dialog box.
	return DialogBox(hInstance, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)MainDlgProc);
}

// Process messages for the Main dialog
static LRESULT CALLBACK
MainDlgProc(HWND dialog, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hwScroll;

	switch (uMsg) {
	case WM_INITDIALOG:
		// Get handles on sliders
		sliderTextR = GetDlgItem(dialog, ID_TEXT_SLIDER_R);
		sliderTextG = GetDlgItem(dialog, ID_TEXT_SLIDER_G);
		sliderTextB = GetDlgItem(dialog, ID_TEXT_SLIDER_B);
		sliderBgR = GetDlgItem(dialog, ID_BG_SLIDER_R);
		sliderBgG = GetDlgItem(dialog, ID_BG_SLIDER_G);
		sliderBgB = GetDlgItem(dialog, ID_BG_SLIDER_B);
		// Get handles on text fields
		htmlTextRGB = GetDlgItem(dialog, ID_TEXT_VALUE_HTML);
		htmlBgRGB = GetDlgItem(dialog, ID_BG_VALUE_HTML);
		viewer = GetDlgItem(dialog, ID_VIEWER);
		// Set sliders min & max values
		SendMessage(sliderTextR, TBM_SETRANGE, TRUE, (LPARAM)MAKELONG(0, 255));
		SendMessage(sliderTextG, TBM_SETRANGE, TRUE, (LPARAM)MAKELONG(0, 255));
		SendMessage(sliderTextB, TBM_SETRANGE, TRUE, (LPARAM)MAKELONG(0, 255));
		SendMessage(sliderBgR, TBM_SETRANGE, TRUE, (LPARAM)MAKELONG(0, 255));
		SendMessage(sliderBgG, TBM_SETRANGE, TRUE, (LPARAM)MAKELONG(0, 255));
		SendMessage(sliderBgB, TBM_SETRANGE, TRUE, (LPARAM)MAKELONG(0, 255));
		// Initial positions
		colorBgR = colorBgG = colorBgB = 255;
		SendMessage(sliderBgR, TBM_SETPOS, TRUE, (LPARAM)255);
		SendMessage(sliderBgG, TBM_SETPOS, TRUE, (LPARAM)255);
		SendMessage(sliderBgB, TBM_SETPOS, TRUE, (LPARAM)255);
		return TRUE;

	case WM_PAINT:
		// This message permits to display the text area when the application starts
		// One message isn't sufficient but with two messages, it works well !
		if (paintDone < 2) {
			SendMessage(dialog, WM_HSCROLL, 0, 0);
			return TRUE;
		}
		return FALSE;

	case WM_ACTIVATE:
	case WM_SETFOCUS:
	case WM_EXITSIZEMOVE:
		paintDone = 0;
		UpdateWindow(dialog);
		return TRUE;

	case WM_HSCROLL:
		hwScroll = (HWND)lParam;
		if (hwScroll == sliderTextR)
			colorTextR = SendMessage(sliderTextR, TBM_GETPOS, FALSE, 0);
		else if (hwScroll == sliderTextG)
			colorTextG = SendMessage(sliderTextG, TBM_GETPOS, FALSE, 0);
		else if (hwScroll == sliderTextB)
			colorTextB = SendMessage(sliderTextB, TBM_GETPOS, FALSE, 0);
		else if (hwScroll == sliderBgR)
			colorBgR = SendMessage(sliderBgR, TBM_GETPOS, FALSE, 0);
		else if (hwScroll == sliderBgG)
			colorBgG = SendMessage(sliderBgG, TBM_GETPOS, FALSE, 0);
		else if (hwScroll == sliderBgB)
			colorBgB = SendMessage(sliderBgB, TBM_GETPOS, FALSE, 0);
		else
			paintDone++;
		RedrawViewer(dialog);
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_TEXT_CHOOSE:
		case ID_BG_CHOOSE:
			SelectColor(dialog, LOWORD(wParam));
			break;
		case ID_TEXT_COPY:
		case ID_BG_COPY:
			CopyHtmlValue(dialog, LOWORD(wParam));
			break;
		}
		return TRUE;

	case WM_CLOSE:
		EndDialog(dialog, 0);
		return TRUE;
	}

	return FALSE;
}

// Update the viewer text area
static void
RedrawViewer(HWND dialog)
{
	static const char	*line1= "Example of black text.";
	static const char	*line2= "Example of white text.";
	static const char	*line3= "Example of colored text.";
	HDC			dc = GetDC(viewer);
	COLORREF	textColor = RGB(colorTextR, colorTextG, colorTextB);
	COLORREF	bgColor = RGB(colorBgR, colorBgG, colorBgB);
	RECT		rect;
	HBRUSH		brush;
	char		htmlRGB[10];

	snprintf(htmlRGB, sizeof(htmlRGB), "#%02X%02X%02X", colorTextR, colorTextG, colorTextB);
	SetDlgItemText(dialog, ID_TEXT_VALUE_HTML, htmlRGB);
	snprintf(htmlRGB, sizeof(htmlRGB), "#%02X%02X%02X", colorBgR, colorBgG, colorBgB);
	SetDlgItemText(dialog, ID_BG_VALUE_HTML, htmlRGB);

	GetClientRect(viewer, &rect);
	brush = CreateSolidBrush(bgColor);
	FillRect(dc, &rect, brush);
	SetBkColor(dc, bgColor);
	rect.left += 40;
	// line of black text
	rect.top += 5;
	DrawText(dc, line1, strlen(line1), &rect, 0);
	// line of white text
	rect.top += 20;
	SetTextColor(dc, RGB(255, 255, 255));
	DrawText(dc, line2, strlen(line2), &rect, 0);
	// line of colored text
	rect.top += 20;
	SetTextColor(dc, textColor);
	DrawText(dc, line3, strlen(line3), &rect, 0);
	ReleaseDC(dialog, dc);
}

// Choose text/background color by using a ChooseColor
static void
SelectColor(HWND dialog, int id)
{
	static CHOOSECOLOR	cc;

	cc.lStructSize    = sizeof(CHOOSECOLOR);
	cc.hwndOwner      = dialog;
	cc.hInstance      = NULL;
	cc.lpCustColors   = colorRef;
	cc.Flags          = CC_RGBINIT | CC_FULLOPEN;
	cc.lCustData      = 0;
	cc.lpfnHook       = NULL;
	cc.lpTemplateName = NULL;
	if (id == ID_TEXT_CHOOSE)
		cc.rgbResult = RGB(colorTextR, colorTextG, colorTextB);
	else
		cc.rgbResult = RGB(colorBgR, colorBgG, colorBgB);

	if (!ChooseColor(&cc))
		return;

	if (id == ID_TEXT_CHOOSE) {
		colorTextR = GetRValue(cc.rgbResult);
		colorTextG = GetGValue(cc.rgbResult);
		colorTextB = GetBValue(cc.rgbResult);
		SendMessage(sliderTextR, TBM_SETPOS, TRUE, colorTextR);
		SendMessage(sliderTextG, TBM_SETPOS, TRUE, colorTextG);
		SendMessage(sliderTextB, TBM_SETPOS, TRUE, colorTextB);
	}
	else {
		colorBgR = GetRValue(cc.rgbResult);
		colorBgG = GetGValue(cc.rgbResult);
		colorBgB = GetBValue(cc.rgbResult);
		SendMessage(sliderBgR, TBM_SETPOS, TRUE, colorBgR);
		SendMessage(sliderBgG, TBM_SETPOS, TRUE, colorBgG);
		SendMessage(sliderBgB, TBM_SETPOS, TRUE, colorBgB);
	}

	RedrawViewer(dialog);
}

// Copy the text/background RGB string to the clipboard
static void
CopyHtmlValue(HWND dialog, int id)
{
	char	htmlRGB[10], *memBlock;
	HGLOBAL hg;

	if (id == ID_TEXT_COPY)
		snprintf(htmlRGB, sizeof(htmlRGB), "#%02X%02X%02X", colorTextR, colorTextG, colorTextB);
	else
		snprintf(htmlRGB, sizeof(htmlRGB), "#%02X%02X%02X", colorBgR, colorBgG, colorBgB);

	hg = GlobalAlloc(GHND|GMEM_SHARE, strlen(htmlRGB) + 1);
	memBlock = (char *)GlobalLock(hg);
	strcpy(memBlock, htmlRGB);
	GlobalUnlock(hg);
	OpenClipboard(dialog);
	EmptyClipboard();
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
}
