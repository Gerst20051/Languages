// webbrowser.c
//
// Much of the functionality has not been tested.
//
#define  COBJMACROS

#include <objidl.h>
#include <exdisp.h>
#include "iehelp.h"

HWND hwndFrame;

DWORD dwCookie;
IConnectionPoint * pIConnectionPoint;

// user call back func
int (*pCallBack) (UINT msg, WPARAM wParam, LPARAM lParam);

extern IOleClientSite  ClientSite;
extern IStorage        Storage;
extern IDispatch       Dispatch;
char   cbuff[256];

void Refresh(IWebBrowser2 * iBrowser)
{
	IWebBrowser2_Refresh(iBrowser);
}

void GoForward(IWebBrowser2 * iBrowser)
{
	IWebBrowser2_GoForward(iBrowser);
}

void GoBack(IWebBrowser2 * iBrowser)
{
	IWebBrowser2_GoBack(iBrowser);
}

char * GetLocationURL(IWebBrowser2 * iBrowser)
{
	BSTR wstr;
	IWebBrowser2_get_LocationURL(iBrowser, &wstr);
	return strncpy(cbuff, UnicodeToAnsi(wstr), 255);
}

void SetAsDropTarget(IWebBrowser2 * iBrowser, VARIANT_BOOL bReg)
{
	IWebBrowser2_put_RegisterAsDropTarget(iBrowser, bReg);
}

BOOL GetAsDropTarget(IWebBrowser2 * iBrowser, VARIANT_BOOL bReg)
{
	VARIANT_BOOL b;
	IWebBrowser2_get_RegisterAsDropTarget(iBrowser, &b);
	return b;
}

void SetAsTheaterMode(IWebBrowser2 * iBrowser, VARIANT_BOOL bReg)
{
	IWebBrowser2_put_TheaterMode(iBrowser, bReg);
}

BOOL GetAsTheaterMode(IWebBrowser2 * iBrowser, VARIANT_BOOL bReg)
{
	VARIANT_BOOL b;
	IWebBrowser2_get_TheaterMode(iBrowser, &b);
	return b;
}

void StopDownload(IWebBrowser2 * iBrowser)
{
	if(iBrowser)
		IWebBrowser2_Stop(iBrowser);
}

static LRESULT CALLBACK MainWndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	IWebBrowser2 * ipBrowser;
	RECT rc;
	switch (msg) {
	case WM_SIZE:
		ipBrowser = (IWebBrowser2 *)GetWindowLong(hwnd,GWL_USERDATA);
		if (ipBrowser) {
			GetClientRect(hwnd,&rc);
			ReSizeBrowserCtrl(ipBrowser, &rc);
		}
		break;
	case WM_CLOSE:
		ipBrowser = (IWebBrowser2 *)GetWindowLong(hwnd,GWL_USERDATA);
		if (ipBrowser) {
			IWebBrowser2_Quit(ipBrowser);
			IWebBrowser2_Release(ipBrowser);
		}
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

static HWND CreateDefaultWindow(void)
{
	static int registered;

	if (!registered) {
		WNDCLASS wc;

		memset(&wc,0,sizeof(WNDCLASS));
		wc.style 		 = CS_HREDRAW|CS_VREDRAW |CS_DBLCLKS ;
		wc.lpfnWndProc   = (WNDPROC)MainWndProc;
		wc.hInstance 	 = GetModuleHandle(NULL);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
		wc.lpszClassName = "browserhelperWndClass";
		wc.hCursor 		 = LoadCursor(NULL,IDC_ARROW);
		if (!RegisterClass(&wc))
			return 0;
		registered = 1;
	}
	return CreateWindow("browserhelperWndClass","browser",
		WS_MINIMIZEBOX|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_MAXIMIZEBOX|WS_CAPTION|WS_BORDER|WS_SYSMENU|WS_THICKFRAME,
		CW_USEDEFAULT,0,CW_USEDEFAULT,0,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL);
}

int ReSizeBrowserCtrl(IWebBrowser2 * iBrowser, RECT * rc)
{
	HRESULT hr;
	IOleInPlaceObject * pInPlaceObject;

	hr = IWebBrowser2_QueryInterface(iBrowser, &IID_IOleInPlaceObject, (void**)&pInPlaceObject);

	if (hr == S_OK){
		IOleInPlaceObject_SetObjectRects(pInPlaceObject, rc, rc);
		IOleInPlaceObject_Release(pInPlaceObject);
	}
	return hr;
}

HRESULT SetUserCallBack(IWebBrowser2 * iBrowser, int (*pUserFunc)(UINT msg, WPARAM wParam, LPARAM lParam))
{
	HRESULT hr;
	IConnectionPointContainer * pIConnectionContainer;

	pCallBack = NULL;

	// user callback function to receive event msgs.
	if(iBrowser && pUserFunc != NULL){

		hr = IWebBrowser2_QueryInterface(iBrowser, &IID_IConnectionPointContainer,
						(void**)&pIConnectionContainer);

		if (SUCCEEDED(hr)){

			hr = IConnectionPointContainer_FindConnectionPoint(pIConnectionContainer,
							&DIID_DWebBrowserEvents2, (IConnectionPoint**)&pIConnectionPoint);

			if (SUCCEEDED(hr)){
				IConnectionPoint_Advise(pIConnectionPoint, (IUnknown*)&Dispatch, &dwCookie);
				IConnectionPoint_Release(pIConnectionPoint);
				pCallBack = pUserFunc;
			}else{
				hr = S_FALSE;
			}

			IConnectionPointContainer_Release(pIConnectionContainer);

		}else{
			hr = S_FALSE;
		}

	}else{
		hr = S_FALSE;
	}
	return hr;
}

IWebBrowser2 * CreateEmbeddedWebControl(HWND hwnd)
{
	IOleObject *mpWebObject=NULL;
	HRESULT hr;
	RECT rc;

	IWebBrowser2* ipBrowser;
	int createdwindow = 0;

	if (hwnd == 0) {
		hwndFrame = hwnd = CreateDefaultWindow();
		ShowWindow(hwnd,SW_SHOW);
		createdwindow = 1;
	}else
		hwndFrame = hwnd;

	hr = OleCreate(&CLSID_WebBrowser, &IID_IOleObject, OLERENDER_DRAW, 0,
				&ClientSite, &Storage, (void**)&mpWebObject);

	if (hr != S_OK)
		return 0;

	GetClientRect(hwndFrame,&rc);
	IOleObject_SetHostNames(mpWebObject, L"Web Host", L"Web View");

	OleSetContainedObject((LPUNKNOWN)mpWebObject, TRUE);

	IOleObject_DoVerb(mpWebObject, OLEIVERB_SHOW,NULL, &ClientSite,-1, hwndFrame, &rc);
	IOleObject_QueryInterface(mpWebObject, &IID_IWebBrowser2, (void**)&ipBrowser);
	if (ipBrowser) {
		if (createdwindow) {
			SetWindowLong(hwndFrame,GWL_USERDATA,(long)ipBrowser);
		}
	}

	return ipBrowser;
}


int NavigateURL(IWebBrowser2 *iBrowser, LPWSTR Wurl)
{
	VARIANT vURL;
	VARIANT ve1, ve2, ve3, ve4;

	vURL.vt = VT_BSTR;
	vURL.bstrVal = SysAllocString(Wurl);
	ve1.vt = VT_EMPTY;
	ve2.vt = VT_EMPTY;
	ve3.vt = VT_EMPTY;
	ve4.vt = VT_EMPTY;

	IWebBrowser2_Navigate2(iBrowser, &vURL, &ve1, &ve2, &ve3, &ve4);

	VariantClear(&vURL);
	return 1;
}

int CloseEmbeddedWebControl(IWebBrowser2 *iBrowser)
{
	IWebBrowser2_Quit(iBrowser);
	IWebBrowser2_Release(iBrowser);
	return 1;
}

HWND GetEmbeddedWebControlWindow(void)
{
	return hwndFrame;
}

