#ifndef __windows_h__
#include <windows.h>
#endif
#include <objbase.h>
#include <exdisp.h>
IWebBrowser2 *iBrowser;
IWebBrowser2 *CreateEmbeddedWebControl(HWND hwnd);
int 	NavigateURL(IWebBrowser2 *iBrowser,LPWSTR Wurl);
HWND 	GetEmbeddedWebControlWindow(void);
int 	CloseEmbeddedWebControl(IWebBrowser2 *iBrowser);
void 	Refresh(IWebBrowser2 * iBrowser);
void 	GoForward(IWebBrowser2 * iBrowser);
void 	GoBack(IWebBrowser2 * iBrowser);
char *  GetLocationURL(IWebBrowser2 * iBrowser);
void 	SetAsDropTarget(IWebBrowser2 * iBrowser, VARIANT_BOOL bReg);
BOOL 	GetAsDropTarget(IWebBrowser2 * iBrowser, VARIANT_BOOL bReg);
void 	StopDownload(IWebBrowser2 * iBrowser);
OLECHAR * AnsiToUnicode(char * szA);
char *  UnicodeToAnsi( OLECHAR * szW );
int 	ReSizeBrowserCtrl(IWebBrowser2 * iBrowser, RECT * rc);
HRESULT SetUserCallBack(IWebBrowser2 * iBrowser, int (*pUserFunc)(UINT msg, WPARAM wParam, LPARAM lParam));
