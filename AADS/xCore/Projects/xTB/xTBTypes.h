/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Jan 14 21:21:32 2003
 */
/* Compiler settings for C:\Projects\AADS\xCore\Projects\xTB\xTB.odl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __xTBTypes_h__
#define __xTBTypes_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __ICommands_FWD_DEFINED__
#define __ICommands_FWD_DEFINED__
typedef interface ICommands ICommands;
#endif 	/* __ICommands_FWD_DEFINED__ */


#ifndef __Commands_FWD_DEFINED__
#define __Commands_FWD_DEFINED__

#ifdef __cplusplus
typedef class Commands Commands;
#else
typedef struct Commands Commands;
#endif /* __cplusplus */

#endif 	/* __Commands_FWD_DEFINED__ */


#ifndef __ApplicationEvents_FWD_DEFINED__
#define __ApplicationEvents_FWD_DEFINED__

#ifdef __cplusplus
typedef class ApplicationEvents ApplicationEvents;
#else
typedef struct ApplicationEvents ApplicationEvents;
#endif /* __cplusplus */

#endif 	/* __ApplicationEvents_FWD_DEFINED__ */


#ifndef __DebuggerEvents_FWD_DEFINED__
#define __DebuggerEvents_FWD_DEFINED__

#ifdef __cplusplus
typedef class DebuggerEvents DebuggerEvents;
#else
typedef struct DebuggerEvents DebuggerEvents;
#endif /* __cplusplus */

#endif 	/* __DebuggerEvents_FWD_DEFINED__ */


void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 


#ifndef __XTB_LIBRARY_DEFINED__
#define __XTB_LIBRARY_DEFINED__

/* library XTB */
/* [helpstring][version][uuid] */ 


DEFINE_GUID(LIBID_XTB,0x1E241520,0x7843,0x485E,0x8B,0x44,0x45,0xEC,0xD9,0x4E,0x52,0x38);

#ifndef __ICommands_INTERFACE_DEFINED__
#define __ICommands_INTERFACE_DEFINED__

/* interface ICommands */
/* [object][dual][oleautomation][uuid] */ 


DEFINE_GUID(IID_ICommands,0x424A23B1,0xB766,0x4308,0x9E,0xDA,0xC6,0x17,0xB2,0x15,0xB1,0x03);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("424A23B1-B766-4308-9EDA-C617B215B103")
    ICommands : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE XTB_ConfigureXCL( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XTB_LaunchProgram( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XTB_DebugProgram( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XTB_AttachDebugger( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XTB_TargetManager( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICommandsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICommands __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICommands __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICommands __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ICommands __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ICommands __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ICommands __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ICommands __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XTB_ConfigureXCL )( 
            ICommands __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XTB_LaunchProgram )( 
            ICommands __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XTB_DebugProgram )( 
            ICommands __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XTB_AttachDebugger )( 
            ICommands __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XTB_TargetManager )( 
            ICommands __RPC_FAR * This);
        
        END_INTERFACE
    } ICommandsVtbl;

    interface ICommands
    {
        CONST_VTBL struct ICommandsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICommands_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICommands_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICommands_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICommands_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICommands_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICommands_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICommands_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICommands_XTB_ConfigureXCL(This)	\
    (This)->lpVtbl -> XTB_ConfigureXCL(This)

#define ICommands_XTB_LaunchProgram(This)	\
    (This)->lpVtbl -> XTB_LaunchProgram(This)

#define ICommands_XTB_DebugProgram(This)	\
    (This)->lpVtbl -> XTB_DebugProgram(This)

#define ICommands_XTB_AttachDebugger(This)	\
    (This)->lpVtbl -> XTB_AttachDebugger(This)

#define ICommands_XTB_TargetManager(This)	\
    (This)->lpVtbl -> XTB_TargetManager(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT STDMETHODCALLTYPE ICommands_XTB_ConfigureXCL_Proxy( 
    ICommands __RPC_FAR * This);


void __RPC_STUB ICommands_XTB_ConfigureXCL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICommands_XTB_LaunchProgram_Proxy( 
    ICommands __RPC_FAR * This);


void __RPC_STUB ICommands_XTB_LaunchProgram_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICommands_XTB_DebugProgram_Proxy( 
    ICommands __RPC_FAR * This);


void __RPC_STUB ICommands_XTB_DebugProgram_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICommands_XTB_AttachDebugger_Proxy( 
    ICommands __RPC_FAR * This);


void __RPC_STUB ICommands_XTB_AttachDebugger_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICommands_XTB_TargetManager_Proxy( 
    ICommands __RPC_FAR * This);


void __RPC_STUB ICommands_XTB_TargetManager_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICommands_INTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_Commands,0xE0F85A49,0xA880,0x4280,0x93,0x9E,0x9C,0x1C,0x25,0xA2,0x7F,0x47);

#ifdef __cplusplus

class DECLSPEC_UUID("E0F85A49-A880-4280-939E-9C1C25A27F47")
Commands;
#endif

DEFINE_GUID(CLSID_ApplicationEvents,0x442BA762,0x2E4A,0x44A0,0xB9,0x94,0x20,0x36,0x0A,0xFD,0x0D,0x93);

#ifdef __cplusplus

class DECLSPEC_UUID("442BA762-2E4A-44A0-B994-20360AFD0D93")
ApplicationEvents;
#endif

DEFINE_GUID(CLSID_DebuggerEvents,0x82C762B1,0x1279,0x4130,0xB6,0xFC,0xB1,0x58,0x06,0xCB,0x84,0x10);

#ifdef __cplusplus

class DECLSPEC_UUID("82C762B1-1279-4130-B6FC-B15806CB8410")
DebuggerEvents;
#endif
#endif /* __XTB_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
