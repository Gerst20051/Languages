// InPlaceSite.c

#include <objidl.h>

static int ref_cnt;

extern HWND hwndFrame;

static HRESULT STDMETHODCALLTYPE InPlaceSite_QueryInterface(IOleInPlaceSite*,REFIID,void**);
static ULONG STDMETHODCALLTYPE InPlaceSite_AddRef(IOleInPlaceSite*);
static ULONG STDMETHODCALLTYPE InPlaceSite_Release(IOleInPlaceSite*);
static HRESULT STDMETHODCALLTYPE InPlaceSite_GetWindow(IOleInPlaceSite*,HWND*);
static HRESULT STDMETHODCALLTYPE InPlaceSite_ContextSensitiveHelp(IOleInPlaceSite*,BOOL);
static HRESULT STDMETHODCALLTYPE InPlaceSite_CanInPlaceActivate(IOleInPlaceSite*);
static HRESULT STDMETHODCALLTYPE InPlaceSite_OnInPlaceActivate(IOleInPlaceSite*);
static HRESULT STDMETHODCALLTYPE InPlaceSite_OnUIActivate( IOleInPlaceSite*);
static HRESULT STDMETHODCALLTYPE InPlaceSite_GetWindowContext(IOleInPlaceSite*,
			IOleInPlaceFrame**,IOleInPlaceUIWindow**,LPRECT,LPRECT,LPOLEINPLACEFRAMEINFO);
static HRESULT STDMETHODCALLTYPE InPlaceSite_Scroll(IOleInPlaceSite*,SIZE);
static HRESULT STDMETHODCALLTYPE InPlaceSite_OnUIDeactivate(IOleInPlaceSite*,BOOL);
static HRESULT STDMETHODCALLTYPE InPlaceSite_OnInPlaceDeactivate(IOleInPlaceSite*);
static HRESULT STDMETHODCALLTYPE InPlaceSite_DiscardUndoState(IOleInPlaceSite*);
static HRESULT STDMETHODCALLTYPE InPlaceSite_DeactivateAndUndo(IOleInPlaceSite*);
static HRESULT STDMETHODCALLTYPE InPlaceSite_OnPosRectChange(IOleInPlaceSite*,LPCRECT);
static HRESULT STDMETHODCALLTYPE AddRef(IOleClientSite*);
static HRESULT STDMETHODCALLTYPE Release(IOleClientSite*);

static IOleInPlaceSiteVtbl InPlaceSiteVtbl ={
	InPlaceSite_QueryInterface,
	InPlaceSite_AddRef,
	InPlaceSite_Release,
	InPlaceSite_GetWindow,
	InPlaceSite_ContextSensitiveHelp,
	InPlaceSite_CanInPlaceActivate,
	InPlaceSite_OnInPlaceActivate,
	InPlaceSite_OnUIActivate,
	InPlaceSite_GetWindowContext,
	InPlaceSite_Scroll,
	InPlaceSite_OnUIDeactivate,
	InPlaceSite_OnInPlaceDeactivate,
	InPlaceSite_DiscardUndoState,
	InPlaceSite_DeactivateAndUndo,
	InPlaceSite_OnPosRectChange
};

IOleInPlaceSite InPlacesite = {&InPlaceSiteVtbl};


// IOleInPlaceSite methods
static HRESULT STDMETHODCALLTYPE InPlaceSite_QueryInterface(IOleInPlaceSite *this,REFIID riid,void ** ppvObject)
{
	if(IsEqualIID(riid, &IID_IOleInPlaceSite))
		*ppvObject = (void *)&InPlacesite;
	else
	{
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}

	return S_OK;
}

static ULONG STDMETHODCALLTYPE InPlaceSite_AddRef(IOleInPlaceSite *this)
{
	return ++ref_cnt;
}

static ULONG STDMETHODCALLTYPE InPlaceSite_Release(IOleInPlaceSite *this)
{
	if(--ref_cnt == 0) {
	return 0;
	}
	return ref_cnt;
}

static HRESULT STDMETHODCALLTYPE InPlaceSite_GetWindow(IOleInPlaceSite * This,HWND *phwnd)
{
	*phwnd = hwndFrame;
	return S_OK;
}

static HRESULT STDMETHODCALLTYPE InPlaceSite_ContextSensitiveHelp( IOleInPlaceSite * This,BOOL fEnterMode)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE InPlaceSite_CanInPlaceActivate( IOleInPlaceSite * This)
{
	return S_OK;
}

static HRESULT STDMETHODCALLTYPE InPlaceSite_OnInPlaceActivate( IOleInPlaceSite * This)
{
	return S_OK;
}

static HRESULT STDMETHODCALLTYPE InPlaceSite_OnUIActivate( IOleInPlaceSite * This)
{
	return S_OK;
}

extern IOleInPlaceFrame InPlaceFrame;

static HRESULT STDMETHODCALLTYPE InPlaceSite_GetWindowContext(IOleInPlaceSite * This,
			IOleInPlaceFrame **ppFrame,
			IOleInPlaceUIWindow **ppDoc,
			LPRECT lprcPosRect,
			LPRECT lprcClipRect,
			LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
//	OutputDebugString ("GetWindowContext");

	*ppFrame = &InPlaceFrame;
	*ppDoc = NULL;
	GetClientRect(hwndFrame,lprcPosRect);
	GetClientRect(hwndFrame,lprcClipRect);

	lpFrameInfo->fMDIApp = FALSE;
	lpFrameInfo->hwndFrame = hwndFrame;
	lpFrameInfo->haccel = NULL;
	lpFrameInfo->cAccelEntries = 0;

	return S_OK;

}

static HRESULT STDMETHODCALLTYPE InPlaceSite_Scroll(IOleInPlaceSite * This,SIZE scrollExtant)
{
	// OutputDebugString ("Scroll");
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE InPlaceSite_OnUIDeactivate(IOleInPlaceSite * This,BOOL fUndoable)
{
	// OutputDebugString ("OnUIDeactivate");
	return S_OK;
}

static HRESULT STDMETHODCALLTYPE InPlaceSite_OnInPlaceDeactivate( IOleInPlaceSite * This)
{
	// OutputDebugString ("OnInPlaceDeactivate");
	return S_OK;
}

static HRESULT STDMETHODCALLTYPE InPlaceSite_DiscardUndoState (IOleInPlaceSite * This)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE InPlaceSite_DeactivateAndUndo(IOleInPlaceSite * This)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE InPlaceSite_OnPosRectChange(IOleInPlaceSite * This,LPCRECT lprcPosRect)
{
	return S_OK;
}


