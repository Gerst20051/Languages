#ifndef _hyperlink_h_
#define _hyperlink_h_

typedef enum {
	ulHover, ulNone, ulAlways
} ULSTYLE;

typedef void (*callback) (DWORD HypCtrlID);

typedef struct {
	LPTSTR		szLink;		// link
	LPTSTR		szText;		// text displayed as link
	LPTSTR		szTooltip;	// tooltip text
	COLORREF	fgcNormal;  // foreground color normal state
	COLORREF	fgcHover;	// foreground color hover state
	COLORREF	bgcNormal;	// background color normal state
	COLORREF	bgcHover;	// background color hover state
	ULSTYLE		ulStyle;	// underline style
	UINT		dtStyle;	// DrawText() flags
	BOOL		bAutoSize;	// autosize control to fit the text
	BOOL		bPlaySound; // play default navigation sound defined by user
	callback	cbFn;		// callback function
	DWORD		cbID;		// callback control id
} HYPCTRL;

// public functions
BOOL InitHypCtrl(HYPCTRL* pHc);
HWND CreateHypCtrl(HWND hWndParent, HYPCTRL* pHc, int x, int y, int width, int height);

#endif
