// Dispatch.c

#define  COBJMACROS

#include <objidl.h>
#include "exdispid.h"
#include "iehelp.h"
#include <stdio.h>

extern HWND hwndFrame;
extern DWORD dwCookie;
extern IConnectionPoint * pIConnectionPoint;

static int ref_cnt;

// user callback function
extern int (*pCallBack) (UINT msg, WPARAM wParam, LPARAM lParam);

static HRESULT STDMETHODCALLTYPE Dispatch_QueryInterface(IDispatch*,REFIID,void**);
static ULONG   STDMETHODCALLTYPE Dispatch_AddRef(IDispatch*);
static ULONG   STDMETHODCALLTYPE Dispatch_Release(IDispatch*);
static HRESULT STDMETHODCALLTYPE Dispatch_GetTypeInfoCount(IDispatch*,unsigned int*);
static HRESULT STDMETHODCALLTYPE Dispatch_GetTypeInfo(IDispatch*,unsigned int,LCID,ITypeInfo**);
static LONG    STDMETHODCALLTYPE Dispatch_GetIDsOfNames(IDispatch*,REFIID,LPOLESTR*, unsigned int,LCID,DISPID*);
static HRESULT STDMETHODCALLTYPE Dispatch_Invoke(IDispatch*,DISPID,REFIID,
				LCID,WORD,DISPPARAMS*,VARIANT*,EXCEPINFO*,UINT*);

static IDispatchVtbl dispatchVtbl ={
	Dispatch_QueryInterface,
	Dispatch_AddRef,
	Dispatch_Release,
	Dispatch_GetTypeInfoCount,
	Dispatch_GetTypeInfo,
	Dispatch_GetIDsOfNames,
	Dispatch_Invoke
};

IDispatch Dispatch = {&dispatchVtbl};


// IDispatch methods
static HRESULT STDMETHODCALLTYPE Dispatch_QueryInterface(IDispatch *this,REFIID riid,void ** ppvObject)
{
	if(IsEqualIID(riid, &IID_IUnknown) || IsEqualIID(riid , &IID_IDispatch))
		*ppvObject = (IDispatch*)this;
	else{
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}

	return S_OK;
}

static ULONG STDMETHODCALLTYPE Dispatch_AddRef(IDispatch *this)
{
//	// OutputDebugString ("AddRef");
	return ++ref_cnt;
}

static ULONG STDMETHODCALLTYPE Dispatch_Release(IDispatch *this)
{
//	// OutputDebugString ("Release");
	if(--ref_cnt == 0) {
		return 0;
	}
	return ref_cnt;
}

static HRESULT STDMETHODCALLTYPE Dispatch_GetTypeInfoCount(IDispatch * This, unsigned int *  pctinfo)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE Dispatch_GetTypeInfo(IDispatch * This,unsigned int iTInfo, LCID  lcid, ITypeInfo **  ppTInfo)
{
	return E_NOTIMPL;
}

static LONG STDMETHODCALLTYPE Dispatch_GetIDsOfNames(IDispatch * This,REFIID r, LPOLESTR* cNames, UINT i, LCID lcid, DISPID * rgDispId)
{
	return E_NOTIMPL;
}

#define VARIANT_TRUE ((VARIANT_BOOL)-1)

static HRESULT STDMETHODCALLTYPE Dispatch_Invoke(IDispatch * This, DISPID dispidMember,
				REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS * pDispParams,
				VARIANT * pvarResult, EXCEPINFO *  pExcepInfo,  UINT * puArgErr)
{
	float f;

	switch (dispidMember){
	case DISPID_DOWNLOADBEGIN: // Fires when a navigation operation is beginning
		pCallBack(DISPID_DOWNLOADBEGIN, 0, 0);
		break;

	case DISPID_DOCUMENTCOMPLETE: // Fires when a document has been completely loaded and initialized
		pCallBack(DISPID_DOCUMENTCOMPLETE, 0, 0);
		break;

	case DISPID_PROGRESSCHANGE:  // sent when download progress is updated
		// this seems to be not good. the max number appears to be 10000
		// although MS Doc says 100. Strange large numbers sometimes (3762%)
		f = (float)pDispParams->rgvarg[1].lVal;
		f = (f/10000.0)*100.0;
		if(f < 100.0f) // only send it if below 100%
			pCallBack(DISPID_PROGRESSCHANGE, (int)f, 0);
		break;

	case DISPID_BEFORENAVIGATE2: // Fires before navigation occurs in the given object (on either a window or frameset element)
		pCallBack(DISPID_BEFORENAVIGATE2, pDispParams->cArgs, (long)pDispParams);
		break;

	case DISPID_NAVIGATECOMPLETE2: // Fires after a navigation to a link is completed on either a window or frameSet element
		pCallBack(DISPID_NAVIGATECOMPLETE2, 0, 0);
		break;

	case DISPID_NEWWINDOW2: // Fires when a new window is to be created.
		// stop a new Explorer window from openning.
		if(pDispParams->cArgs == 2){
			if( -1 == pCallBack(DISPID_NEWWINDOW2, 0, 0)){
				if((pDispParams->rgvarg[0].vt & VT_BYREF) && (pDispParams->rgvarg[0].vt & VT_BOOL))
					*(pDispParams->rgvarg[0].pboolVal)  = VARIANT_TRUE;
					*(pDispParams->rgvarg[1].ppdispVal) = &Dispatch;
			}
		}
		break;

	case DISPID_STATUSTEXTCHANGE: // Fires when the status bar text of the object has changed.
		pCallBack(DISPID_STATUSTEXTCHANGE, 0, (long)UnicodeToAnsi(pDispParams->rgvarg[0].bstrVal));
		break;

	case DISPID_TITLECHANGE: // Fires when the title of a document in the object becomes available or changes
		if (pDispParams->cArgs > 0 && pDispParams->rgvarg[0].vt == VT_BSTR){
			pCallBack(DISPID_TITLECHANGE, 0, (long)UnicodeToAnsi(pDispParams->rgvarg[0].bstrVal));
		}
		break;

	case DISPID_CLIENTTOHOSTWINDOW: // Fires to request that the client window size be converted to the host window size

		break;

	case DISPID_COMMANDSTATECHANGE: // Fires when the enabled state of a command changes

		break;

	case DISPID_DOWNLOADCOMPLETE: // Fires when a navigation operation finishes, is halted, or fails

		break;

	case DISPID_FILEDOWNLOAD: // Fires to indicate that a file download is about to occur.
							  // If a file download dialog is to be displayed, this event
							  // is fired prior to the display of the dialog.
		break;

	case DISPID_NAVIGATEERROR:{ // Fires when an error occurs during navigation
       // Extract the status code from the DISPPARAMS structure
//        VARIANT * vt_statuscode = pDispParams->rgvarg[1].pvarVal;
//        DWORD  dwStatusCode =  vt_statuscode->lVal;
		// ...
        // Extract the event's IDispatch pointer
//        IDispatch *pdispFiredEvent = pDispParams->rgvarg[4].pdispVal;
		}
		break;

	case DISPID_ONTOOLBAR: // Fires when the IWebBrowser2_ToolBar property is changed
		MessageBeep(16);
		break;

	case DISPID_ONVISIBLE:

		break;

	case DISPID_PRIVACYIMPACTEDSTATECHANGE:

		break;

	case DISPID_PROPERTYCHANGE:

		break;

	case DISPID_SETSECURELOCKICON:

		break;

	case DISPID_TITLEICONCHANGE:

		break;

	case DISPID_WINDOWACTIVATE:

		break;

	case DISPID_WINDOWCLOSING:

		break;

	case DISPID_WINDOWMOVE:

		break;

	case DISPID_WINDOWRESIZE:

		break;

	case DISPID_WINDOWSETHEIGHT:

		break;

	case DISPID_WINDOWSETLEFT:

		break;

	case DISPID_WINDOWSETRESIZABLE:

		break;

	case DISPID_WINDOWSETTOP:

		break;

	case DISPID_WINDOWSETWIDTH:

		break;


	default:
		return DISP_E_MEMBERNOTFOUND;
   }

   return S_OK;
}

//	DISPID_AMBIENT_DLCONTROL
//	DISPID_FRAMEBEFORENAVIGATE
//	DISPID_FRAMENAVIGATECOMPLETE
//	DISPID_FRAMENEWWINDOW

//	DISPID_PRINTTEMPLATEINSTANTIATION   225   // Fired to indicate that a print template is instantiated
//	DISPID_PRINTTEMPLATETEARDOWN        226   // Fired to indicate that a print templete is completely gone
//	DISPID_UPDATEPAGESTATUS             227   // Fired to indicate that the spooling status has changed



