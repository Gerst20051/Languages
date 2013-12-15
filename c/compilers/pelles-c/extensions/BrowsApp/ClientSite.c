// ClientSite.c

#include <objidl.h>
#include <objbase.h>

static int ref_cnt = 0;

// user call back func
extern int (*pCallBack) (UINT msg, WPARAM wParam, LPARAM lParam);

static HRESULT STDMETHODCALLTYPE ClientSite_QueryInterface(IOleClientSite*,REFIID,void**);
static ULONG STDMETHODCALLTYPE ClientSite_AddRef(IOleClientSite*);
static ULONG STDMETHODCALLTYPE ClientSite_Release(IOleClientSite*);
static HRESULT STDMETHODCALLTYPE ClientSite_SaveObject(IOleClientSite*);
static HRESULT STDMETHODCALLTYPE ClientSite_GetMoniker(IOleClientSite*,DWORD,DWORD,IMoniker**);
static HRESULT STDMETHODCALLTYPE ClientSite_GetContainer(IOleClientSite*,IOleContainer**);
static HRESULT STDMETHODCALLTYPE ClientSite_ShowObject(IOleClientSite*);
static HRESULT STDMETHODCALLTYPE ClientSite_OnShowWindow(IOleClientSite*,BOOL);
static HRESULT STDMETHODCALLTYPE ClientSite_RequestNewObjectLayout(IOleClientSite*);

static IOleClientSiteVtbl siteVtbl = {
	ClientSite_QueryInterface,
	ClientSite_AddRef,
	ClientSite_Release,
	ClientSite_SaveObject,
	ClientSite_GetMoniker,
	ClientSite_GetContainer,
	ClientSite_ShowObject,
	ClientSite_OnShowWindow,
	ClientSite_RequestNewObjectLayout
};

IOleClientSite ClientSite = {&siteVtbl};

extern IDispatch Dispatch;
extern IOleInPlaceFrame InPlaceFrame;
extern IOleInPlaceSite InPlacesite;

static HRESULT STDMETHODCALLTYPE ClientSite_QueryInterface(IOleClientSite *this,REFIID riid,void ** ppvObject)
{
	if(IsEqualIID(riid, &IID_IUnknown) || IsEqualIID(riid , &IID_IOleClientSite))
		*ppvObject = (IOleClientSite*)this;
	else if (IsEqualIID(riid, &IID_IOleInPlaceSite))
		*ppvObject = (void *)&InPlacesite;
	else if (IsEqualIID(riid, &IID_IDispatch))
		*ppvObject = (void *)&Dispatch;
	else if (IsEqualIID(riid, &IID_IOleInPlaceFrame))
		*ppvObject = (void *)&InPlaceFrame;
	else
	{
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}
	return S_OK;
}

static ULONG STDMETHODCALLTYPE ClientSite_AddRef(IOleClientSite *this)
{
	return ++ref_cnt;
}

static ULONG STDMETHODCALLTYPE ClientSite_Release(IOleClientSite *this)
{
	if(--ref_cnt == 0) {
		return 0;
	}
	return ref_cnt;
}

static HRESULT STDMETHODCALLTYPE ClientSite_SaveObject(IOleClientSite *this)
{
	// OutputDebugString ("SaveObject");
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE ClientSite_GetMoniker(IOleClientSite *this,DWORD dwAssign,DWORD dwWhichMoniker,IMoniker **ppmk)
{
	// OutputDebugString ("GetMoniker");
	*ppmk = NULL;
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE ClientSite_GetContainer(IOleClientSite *this,IOleContainer **ppContainer)
{
	// OutputDebugString ("GetContainer");
	*ppContainer = NULL;
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE ClientSite_ShowObject(IOleClientSite *this)
{
	// OutputDebugString ("ShowObject");
	return S_OK;
}

static HRESULT STDMETHODCALLTYPE ClientSite_OnShowWindow(IOleClientSite *this,BOOL flag)
{
	// OutputDebugString ("OnShowWindow");
	return S_OK;
}

static HRESULT STDMETHODCALLTYPE ClientSite_RequestNewObjectLayout(IOleClientSite *this)
{
	// OutputDebugString ("RequestNewObjectLayout");
	return E_NOTIMPL;
}


