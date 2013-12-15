// Storage.c

#include <objidl.h>

static int ref_cnt;
static HRESULT STDMETHODCALLTYPE Storage_QueryInterface( IStorage*,REFIID,void**);
static ULONG STDMETHODCALLTYPE Storage_AddRef(IStorage*);
static ULONG STDMETHODCALLTYPE Storage_Release(IStorage*);
static HRESULT STDMETHODCALLTYPE Storage_CreateStream(IStorage*,const OLECHAR*,DWORD,DWORD,DWORD,IStream**);
static HRESULT STDMETHODCALLTYPE Storage_OpenStream(IStorage*,const OLECHAR*,void*,DWORD,DWORD,IStream**);
static HRESULT STDMETHODCALLTYPE Storage_CreateStorage( IStorage*,const OLECHAR*,DWORD,DWORD,DWORD,IStorage**);
static HRESULT STDMETHODCALLTYPE Storage_OpenStorage(IStorage*,const OLECHAR*,IStorage*,DWORD,SNB,DWORD,IStorage**);
static HRESULT STDMETHODCALLTYPE Storage_CopyTo(IStorage*,DWORD,const IID*,SNB,IStorage*);
static HRESULT STDMETHODCALLTYPE Storage_MoveElementTo(IStorage*,const OLECHAR*,IStorage*,const OLECHAR*,DWORD);
static HRESULT STDMETHODCALLTYPE Storage_Commit( IStorage*,DWORD);
static HRESULT STDMETHODCALLTYPE Storage_Revert(IStorage*);
static HRESULT STDMETHODCALLTYPE Storage_EnumElements(IStorage*,DWORD,void*,DWORD,IEnumSTATSTG**);
static HRESULT STDMETHODCALLTYPE Storage_DestroyElement(IStorage*,const OLECHAR*);
static HRESULT STDMETHODCALLTYPE Storage_RenameElement(IStorage*,const WCHAR *,const WCHAR *);
static HRESULT STDMETHODCALLTYPE Storage_SetElementTimes(IStorage*,const OLECHAR*,const FILETIME*,const FILETIME*,const FILETIME*);
static HRESULT STDMETHODCALLTYPE Storage_SetClass(IStorage*,REFCLSID);
static HRESULT STDMETHODCALLTYPE Storage_SetStateBits(IStorage*,DWORD,DWORD);
static HRESULT STDMETHODCALLTYPE Storage_Stat(IStorage*,STATSTG*,DWORD);

static IStorageVtbl stgVtbl = {
	Storage_QueryInterface,
	Storage_AddRef,
	Storage_Release,
	Storage_CreateStream,
	Storage_OpenStream,
	Storage_CreateStorage,
	Storage_OpenStorage,
	Storage_CopyTo,
	Storage_MoveElementTo,
	Storage_Commit,
	Storage_Revert,
	Storage_EnumElements,
	Storage_DestroyElement,
	Storage_RenameElement,
	Storage_SetElementTimes,
	Storage_SetClass,
	Storage_SetStateBits,
	Storage_Stat,
};

IStorage Storage = {&stgVtbl};

static HRESULT STDMETHODCALLTYPE Storage_QueryInterface( IStorage* st,REFIID riid,void ** pp)
{
	*pp = NULL;
	return E_NOTIMPL;
}

static ULONG STDMETHODCALLTYPE Storage_AddRef(IStorage *this)
{
	return ++ref_cnt;
}

static ULONG STDMETHODCALLTYPE Storage_Release(IStorage *this)
{
	if(--ref_cnt == 0) {
		return 0;
	}
	return ref_cnt;
}

static HRESULT STDMETHODCALLTYPE Storage_CreateStream(IStorage* i,const OLECHAR* o,DWORD a,DWORD b,DWORD c,IStream **ps)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE Storage_OpenStream(IStorage * i,const OLECHAR* o,void* a,DWORD b,DWORD c ,IStream **s)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE Storage_CreateStorage( IStorage* i,const OLECHAR* o,DWORD a,DWORD b,DWORD c,IStorage ** d)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE Storage_OpenStorage(IStorage* i,const OLECHAR* o,IStorage* a,DWORD b,SNB n,DWORD aa,IStorage **is)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE Storage_CopyTo(IStorage* i,DWORD a,const IID* b,SNB c,IStorage* is)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE Storage_MoveElementTo(IStorage* i,const OLECHAR* o,IStorage* s,const OLECHAR* n,DWORD m)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE Storage_Commit( IStorage* a,DWORD b)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE Storage_Revert(IStorage* i)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE Storage_EnumElements(IStorage* i,DWORD d,void* a,DWORD dd,IEnumSTATSTG **is)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE Storage_DestroyElement(IStorage* i,const OLECHAR* o)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE Storage_RenameElement(IStorage* i,const WCHAR * a,const WCHAR * b)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE Storage_SetElementTimes(IStorage* i,const OLECHAR* a,const FILETIME* f,const FILETIME* m,const FILETIME* z)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE Storage_SetClass(IStorage* i,REFCLSID r)
{
	return S_OK;
}

static HRESULT STDMETHODCALLTYPE Storage_SetStateBits(IStorage* i,DWORD a,DWORD b)
{
	return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE Storage_Stat(IStorage* i,STATSTG* a,DWORD b)
{
	return E_NOTIMPL;
}


