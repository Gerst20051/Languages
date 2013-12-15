
#ifdef _USRDLL
#ifndef MINIDLL
/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Sat Oct 09 20:41:58 2010
 */
/* Compiler settings for automcomserver.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __automcomserver_i_h__
#define __automcomserver_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ICOMServer_FWD_DEFINED__
#define __ICOMServer_FWD_DEFINED__
typedef interface ICOMServer ICOMServer;
#endif 	/* __ICOMServer_FWD_DEFINED__ */


#ifndef __CoCOMServer_FWD_DEFINED__
#define __CoCOMServer_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoCOMServer CoCOMServer;
#else
typedef struct CoCOMServer CoCOMServer;
#endif /* __cplusplus */

#endif 	/* __CoCOMServer_FWD_DEFINED__ */


/* header files for imported files */
#include "wtypes.h"

#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __LibCOMServer_LIBRARY_DEFINED__
#define __LibCOMServer_LIBRARY_DEFINED__

/* library LibCOMServer */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_LibCOMServer,0xa9863c65, 0x8cd4, 0x4069, 0x89, 0x3d, 0x3b, 0x5a, 0x3d, 0xdf, 0xae, 0x88);

#ifndef __ICOMServer_INTERFACE_DEFINED__
#define __ICOMServer_INTERFACE_DEFINED__

/* interface ICOMServer */
/* [object][oleautomation][dual][uuid] */ 


DEFINE_GUID(IID_ICOMServer,0x4ffe41b, 0x8fe9, 0x4479, 0x99, 0xa, 0xb1, 0x86, 0xec, 0x73, 0xf4, 0x9c);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("04FFE41B-8FE9-4479-990A-B186EC73F49C")
    ICOMServer : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ahktextdll( 
           /*in,optional*/VARIANT script,/*in,optional*/VARIANT options,/*in,optional*/VARIANT params, /* [retval][out] */unsigned int* hThread) = 0;
        
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ahkdll( 
           /*in,optional*/VARIANT filepath,/*in,optional*/VARIANT options,/*in,optional*/VARIANT params, /* [retval][out] */unsigned int* hThread) = 0;
        
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ahkPause( 
           /*in,optional*/VARIANT aChangeTo,/* [retval][out] */BOOL* paused) = 0;
        
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ahkReady( 
           /* [retval][out] */BOOL* ready) = 0;
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ahkFindLabel( 
           /*[in]*/ VARIANT aLabelName,/*[out, retval]*/ unsigned int* pLabel) = 0;
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ahkgetvar( 
           /*[in]*/ VARIANT name,/*[in,optional]*/ VARIANT getVar,/*[out, retval]*/ VARIANT *returnVal) = 0;
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ahkassign( 
           /*[in]*/ VARIANT name,/*[in,optional]*/ VARIANT value,/*[out, retval]*/ unsigned int* success) = 0;
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ahkExecuteLine( 
           /*[in,optional]*/ VARIANT line,/*[in,optional]*/ VARIANT aMode,/*[in,optional]*/ VARIANT wait,/*[out, retval]*/ unsigned int* pLine) = 0;
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ahkLabel( 
           /*[in]*/ VARIANT aLabelName,/*[in,optional]*/ VARIANT nowait,/*[out, retval]*/ BOOL* success) = 0;
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ahkFindFunc( 
           /*[in]*/ VARIANT FuncName,/*[out, retval]*/ unsigned int* pFunc) = 0;
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ahkFunction( 
           /*[in]*/ VARIANT FuncName,/*[in,optional]*/ VARIANT param1,/*[in,optional]*/ VARIANT param2,/*[in,optional]*/ VARIANT param3,/*[in,optional]*/ VARIANT param4,/*[in,optional]*/ VARIANT param5,/*[in,optional]*/ VARIANT param6,/*[in,optional]*/ VARIANT param7,/*[in,optional]*/ VARIANT param8,/*[in,optional]*/ VARIANT param9,/*[in,optional]*/ VARIANT param10,/*[out, retval]*/ VARIANT* returnVal) = 0;
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ahkPostFunction( 
           /*[in]*/ VARIANT FuncName,/*[in,optional]*/ VARIANT param1,/*[in,optional]*/ VARIANT param2,/*[in,optional]*/ VARIANT param3,/*[in,optional]*/ VARIANT param4,/*[in,optional]*/ VARIANT param5,/*[in,optional]*/ VARIANT param6,/*[in,optional]*/ VARIANT param7,/*[in,optional]*/ VARIANT param8,/*[in,optional]*/ VARIANT param9,/*[in,optional]*/ VARIANT param10,/*[out, retval]*/ unsigned int* returnVal) = 0;
	public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ahkKey( 
           /*[in]*/ VARIANT name,/*[out, retval]*/ BOOL* success) = 0;
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE addScript( 
           /*[in]*/ VARIANT script,/*[in,optional]*/ VARIANT replace,/*[out, retval]*/ unsigned int* success) = 0;
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE addFile( 
           /*[in]*/ VARIANT filepath,/*[in,optional]*/ VARIANT aAllowDuplicateInclude,/*[in,optional]*/ VARIANT aIgnoreLoadFailure,/*[out, retval]*/ unsigned int* success) = 0;
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ahkExec( 
           /*[in]*/ VARIANT script,/*[out, retval]*/ BOOL* success) = 0;
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ahkTerminate( 
           /*[in,optional]*/ VARIANT kill,/*[out, retval]*/ BOOL* success) = 0;
    };
    
#else 	/* C style interface */

    typedef struct ICOMServerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICOMServer * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICOMServer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICOMServer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICOMServer * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICOMServer * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICOMServer * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICOMServer * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Name )( 
            ICOMServer * This,
            /* [retval][out] */ BSTR *objectname);
        
        END_INTERFACE
    } ICOMServerVtbl;

    interface ICOMServer
    {
        CONST_VTBL struct ICOMServerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICOMServer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICOMServer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICOMServer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICOMServer_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICOMServer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICOMServer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICOMServer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ICOMServer_Name(This,objectname)	\
    ( (This)->lpVtbl -> Name(This,objectname) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICOMServer_INTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_CoCOMServer,0xc00bcc8c, 0x5a04, 0x4392, 0x87, 0xf, 0x20, 0xaa, 0xe1, 0xb9, 0x26, 0xb2);

#ifdef __cplusplus

class DECLSPEC_UUID("C00BCC8C-5A04-4392-870F-20AAE1B926B2")
CoCOMServer;
#endif
#endif /* __LibCOMServer_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


#endif
#endif