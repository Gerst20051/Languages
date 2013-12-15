// InPlaceFrame.c
#include <objidl.h>

static int ref_cnt;

extern HWND hwndFrame;

extern int (*pCallBack) (UINT msg, WPARAM wParam, LPARAM lParam);

static HRESULT STDMETHODCALLTYPE InPlaceFrame_QueryInterface(IOleInPlaceFrame*,REFIID,void**);
static ULONG STDMETHODCALLTYPE InPlaceFrame_AddRef(IOleInPlaceFrame*);
static ULONG STDMETHODCALLTYPE InPlaceFrame_Release(IOleInPlaceFrame*);
static HRESULT STDMETHODCALLTYPE InPlaceFrame_GetWindow(IOleInPlaceFrame*,HWND*);
static HRESULT STDMETHODCALLTYPE InPlaceFrame_ContextSensitiveHelp(IOleInPlaceFrame*,BOOL);
static HRESULT STDMETHODCALLTYPE InPlaceFrame_GetBorder(IOleInPlaceFrame*,LPRECT);
static HRESULT STDMETHODCALLTYPE InPlaceFrame_RequestBorderSpace(IOleInPlaceFrame*,LPCBORDERWIDTHS);
static HRESULT STDMETHODCALLTYPE InPlaceFrame_SetBorderSpace(IOleInPlaceFrame*,LPCBORDERWIDTHS);
static HRESULT STDMETHODCALLTYPE InPlaceFrame_SetActiveObject(IOleInPlaceFrame*,IOleInPlaceActiveObject*,LPCOLESTR);
static HRESULT STDMETHODCALLTYPE InPlaceFrame_InsertMenus(IOleInPlaceFrame*,HMENU,LPOLEMENUGROUPWIDTHS);
static HRESULT STDMETHODCALLTYPE InPlaceFrame_DoSetMenu(IOleInPlaceFrame*,HMENU,HOLEMENU,HWND);
static HRESULT STDMETHODCALLTYPE InPlaceFrame_RemoveMenus(IOleInPlaceFrame*,HMENU);
static HRESULT STDMETHODCALLTYPE InPlaceFrame_SetStatusText(IOleInPlaceFrame*,LPCOLESTR);
static HRESULT STDMETHODCALLTYPE InPlaceFrame_EnableModeless(IOleInPlaceFrame*,BOOL);
static HRESULT STDMETHODCALLTYPE InPlaceFrame_TranslateAccelerator(IOleInPlaceFrame*,LPMSG,WORD);

static IOleInPlaceFrameVtbl InPlaceFrameVtbl = {
	InPlaceFrame_QueryInterface,
	InPlaceFrame_AddRef,
	InPlaceFrame_Release,
	InPlaceFrame_GetWindow,
	InPlaceFrame_ContextSensitiveHelp,
	InPlaceFrame_GetBorder,
	InPlaceFrame_RequestBorderSpace,
	InPlaceFrame_SetBorderSpace,
	InPlaceFrame_SetActiveObject,
	InPlaceFrame_InsertMenus,
	InPlaceFrame_DoSetMenu,
	InPlaceFrame_RemoveMenus,
	InPlaceFrame_SetStatusText,
	InPlaceFrame_EnableModeless,
	InPlaceFrame_TranslateAccelerator
};

IOleInPlaceFrame InPlaceFrame = {&InPlaceFrameVtbl};

static HRESULT STDMETHODCALLTYPE InPlaceFrame_QueryInterface(IOleInPlaceFrame * this, REFIID riid, void ** ppvObject)
{
	if(IsEqualIID(riid, &IID_IUnknown) || IsEqualIID(riid , &IID_IOleInPlaceFrame))
		*ppvObject = (IOleInPlaceFrame*)this;
	else
	{
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}

	return S_OK;
}

static ULONG STDMETHODCALLTYPE InPlaceFrame_AddRef(IOleInPlaceFrame * this)
{
//	// OutputDebugString ("AddRef");
	return ++ref_cnt;
}

static ULONG STDMETHODCALLTYPE InPlaceFrame_Release(IOleInPlaceFrame * this)
{
//	// OutputDebugString ("Release");
	if(--ref_cnt == 0) {
	return 0;
	}
	return ref_cnt;
}

static HRESULT STDMETHODCALLTYPE InPlaceFrame_GetWindow(IOleInPlaceFrame * This,HWND *phwnd)
{
	// OutputDebugString ("GetWindow");
	*phwnd = hwndFrame;
	return S_OK;
}

static HRESULT STDMETHODCALLTYPE InPlaceFrame_ContextSensitiveHelp(IOleInPlaceFrame * This,BOOL fEnterMode)
{
	// OutputDebugString ("ContextSensitiveHelp");
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE InPlaceFrame_GetBorder(IOleInPlaceFrame * This,LPRECT lprectBorder)
{
	// OutputDebugString ("GetBorder");
	return INPLACE_E_NOTOOLSPACE;
}

static HRESULT STDMETHODCALLTYPE InPlaceFrame_RequestBorderSpace(IOleInPlaceFrame * This,LPCBORDERWIDTHS pborderwidths)
{
	// OutputDebugString ("RequestBorderSpace");
	return INPLACE_E_NOTOOLSPACE;
}

static HRESULT STDMETHODCALLTYPE InPlaceFrame_SetBorderSpace(IOleInPlaceFrame * This,LPCBORDERWIDTHS pborderwidths)
{
	// OutputDebugString ("SetBorderSpace");
	return OLE_E_INVALIDRECT;
}
/*
	IOleInPlaceUIWindow::SetActiveObject
	Provides a direct channel of communication between the object and each of the frame and
	document windows.

	Parameters

	pActiveObject
	[in] Pointer to the IOleInPlaceActiveObject interface on the active in-place object.

	pszObjName
	[in] Pointer to a string containing a name that describes the object an embedding container
	can use in composing its window title. It can be NULL if the object does not require the
	container to change its window titles.
	The Microsoft Windows User Interface Design Guide recommends that containers ignore this
	parameter and always use their own name in the title bar.

	Return Values
	This method supports the standard return values E_INVALIDARG and E_UNEXPECTED,
	as well as the following:

	S_OK
	The method completed successfully.

	Remarks
	Generally, an embedded object should pass NULL for the pszObjName parameter (see
	"Notes to Implementers" below). However, if you are working in conjunction with a container
	that does display the name of the in-place active object in its title bar, then you should
	compose a string in the following form:
	<application name> - <object short-type name>

	Notes to Callers
	IOleInPlaceUIWindow::SetActiveObject is called by the object to establish a direct
	communication link between itself and the document and frame windows.

	When deactivating, the object calls IOleInPlaceUIWindow::SetActiveObject, passing NULL
	for the pActiveObject and pszObjName parameters.

	An object must call IOleInPlaceUIWindow::SetActiveObject before calling
	IOleInPlaceFrame::SetMenu to give the container the pointer to the active object. The
	container then uses this pointer in processing IOleInPlaceFrame::SetMenu and to pass to
	OleSetMenuDescriptor.
*/
static IOleInPlaceActiveObject *ActiveObject;
static HRESULT STDMETHODCALLTYPE InPlaceFrame_SetActiveObject(IOleInPlaceFrame * This,
		IOleInPlaceActiveObject *pActiveObject, LPCOLESTR pszObjName)
{
	// OutputDebugString ("SetActiveObject");
	ActiveObject = pActiveObject; // Save a pointer to the active object interface
	return S_OK;
}

static HRESULT STDMETHODCALLTYPE InPlaceFrame_InsertMenus(IOleInPlaceFrame * This,
			HMENU hmenuShared, LPOLEMENUGROUPWIDTHS lpMenuWidths)
{
	// OutputDebugString ("InsertMenus");
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE InPlaceFrame_DoSetMenu(IOleInPlaceFrame * This,HMENU hmenuShared,
				HOLEMENU holemenu, HWND hwndActiveObject)
{
	// OutputDebugString ("DoSetMenu");

	if (hmenuShared)
		SetMenu(hwndFrame,hmenuShared);

	return S_OK;
}

static HRESULT STDMETHODCALLTYPE InPlaceFrame_RemoveMenus(IOleInPlaceFrame * This,HMENU hmenuShared)
{
	// OutputDebugString ("RemoveMenus");
	return S_OK;
}

OLECHAR * AnsiToUnicode(char * szA)
{
	static OLECHAR szW[512];
	szW[0] = L'\0';

	MultiByteToWideChar(CP_ACP, 0, szA, -1, szW, 512);
	return szW;
}

char * UnicodeToAnsi( OLECHAR * szW )
{
	static char szA[512];
	szA[0] = 0;

	if ( szW == NULL )
		return "<none>";

	WideCharToMultiByte(CP_ACP, 0, szW, -1, szA, 512, NULL, NULL);
	return szA;
}
char mes[256];
// not needed because DISPID_STATUSTEXTCHANGE in IDispatch
static HRESULT STDMETHODCALLTYPE InPlaceFrame_SetStatusText(IOleInPlaceFrame * This, LPCOLESTR pszStatusText)
{
//	if(pCallBack){
//		pCallBack(SB_SETTEXT, (WPARAM)0, (LPARAM)UnicodeToAnsi(pszStatusText));
//	}
	// OutputDebugString ("SetStatusText");
	return S_OK;
}

static HRESULT STDMETHODCALLTYPE InPlaceFrame_EnableModeless(IOleInPlaceFrame * This,BOOL fEnable)
{
	// OutputDebugString ("EnableModeless");
	return S_OK;
}

static HRESULT STDMETHODCALLTYPE InPlaceFrame_TranslateAccelerator(IOleInPlaceFrame * This,LPMSG lpmsg,WORD wID)
{
	// OutputDebugString ("TranslateAccelerator");
	return S_OK;
}



