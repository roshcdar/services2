

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 04:14:07 2038
 */
/* Compiler settings for IServiceManager.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
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
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __IServiceManager_h__
#define __IServiceManager_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IServiceManager_FWD_DEFINED__
#define __IServiceManager_FWD_DEFINED__
typedef interface IServiceManager IServiceManager;

#endif 	/* __IServiceManager_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IServiceManager_INTERFACE_DEFINED__
#define __IServiceManager_INTERFACE_DEFINED__

/* interface IServiceManager */
/* [uuid][object] */ 


EXTERN_C const IID IID_IServiceManager;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("585f68f9-b9d4-4d3c-98b8-cc6ab18d5865")
    IServiceManager : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetService( 
            /* [in] */ const unsigned char *name,
            /* [in] */ DWORD access) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetStatus( 
            /* [retval][out] */ DWORD *status) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Start( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Stop( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Pause( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Continue( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Delete( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE WaitForStatus( 
            /* [in] */ const DWORD desiredStatus,
            /* [in] */ const unsigned int timeoutMs) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IServiceManagerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IServiceManager * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IServiceManager * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IServiceManager * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetService )( 
            IServiceManager * This,
            /* [in] */ const unsigned char *name,
            /* [in] */ DWORD access);
        
        HRESULT ( STDMETHODCALLTYPE *GetStatus )( 
            IServiceManager * This,
            /* [retval][out] */ DWORD *status);
        
        HRESULT ( STDMETHODCALLTYPE *Start )( 
            IServiceManager * This);
        
        HRESULT ( STDMETHODCALLTYPE *Stop )( 
            IServiceManager * This);
        
        HRESULT ( STDMETHODCALLTYPE *Pause )( 
            IServiceManager * This);
        
        HRESULT ( STDMETHODCALLTYPE *Continue )( 
            IServiceManager * This);
        
        HRESULT ( STDMETHODCALLTYPE *Delete )( 
            IServiceManager * This);
        
        HRESULT ( STDMETHODCALLTYPE *WaitForStatus )( 
            IServiceManager * This,
            /* [in] */ const DWORD desiredStatus,
            /* [in] */ const unsigned int timeoutMs);
        
        END_INTERFACE
    } IServiceManagerVtbl;

    interface IServiceManager
    {
        CONST_VTBL struct IServiceManagerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IServiceManager_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IServiceManager_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IServiceManager_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IServiceManager_SetService(This,name,access)	\
    ( (This)->lpVtbl -> SetService(This,name,access) ) 

#define IServiceManager_GetStatus(This,status)	\
    ( (This)->lpVtbl -> GetStatus(This,status) ) 

#define IServiceManager_Start(This)	\
    ( (This)->lpVtbl -> Start(This) ) 

#define IServiceManager_Stop(This)	\
    ( (This)->lpVtbl -> Stop(This) ) 

#define IServiceManager_Pause(This)	\
    ( (This)->lpVtbl -> Pause(This) ) 

#define IServiceManager_Continue(This)	\
    ( (This)->lpVtbl -> Continue(This) ) 

#define IServiceManager_Delete(This)	\
    ( (This)->lpVtbl -> Delete(This) ) 

#define IServiceManager_WaitForStatus(This,desiredStatus,timeoutMs)	\
    ( (This)->lpVtbl -> WaitForStatus(This,desiredStatus,timeoutMs) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IServiceManager_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


