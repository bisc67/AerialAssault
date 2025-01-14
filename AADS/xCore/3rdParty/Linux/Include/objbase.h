#ifndef _OBJBASE_H
#define _OBJBASE_H
#include <rpc.h>
#include <rpcndr.h>
#include <stdlib.h>
#pragma pack(push,8)
#include <basetyps.h>

#define WINOLEAPI STDAPI
#define WINOLEAPI_(type) STDAPI_(type)
#define FARSTRUCT
#define HUGEP
#define LISet32(li,v) ((li).HighPart=(v)<0?-1:0,(li).LowPart=(v))
#define ULISet32(li,v) ((li).HighPart=0,(li).LowPart=(v))
#define CLSCTX_ALL (CLSCTX_INPROC_SERVER|CLSCTX_INPROC_HANDLER|CLSCTX_LOCAL_SERVER)
#define CLSCTX_INPROC (CLSCTX_INPROC_SERVER|CLSCTX_INPROC_HANDLER)
#define CLSCTX_SERVER (CLSCTX_INPROC_SERVER|CLSCTX_LOCAL_SERVER)
#define MARSHALINTERFACE_MIN 500
#define CWCSTORAGENAME 32
#define STGM_DIRECT 0
#define STGM_TRANSACTED 0x10000L
#define STGM_SIMPLE 0x8000000L
#define STGM_READ 0
#define STGM_WRITE 1
#define STGM_READWRITE 2
#define STGM_SHARE_DENY_NONE 0x40
#define STGM_SHARE_DENY_READ 0x30
#define STGM_SHARE_DENY_WRITE 0x20
#define STGM_SHARE_EXCLUSIVE 0x10
#define STGM_PRIORITY 0x40000L
#define STGM_DELETEONRELEASE 0x4000000
#define STGM_NOSCRATCH 0x100000
#define STGM_CREATE 0x1000
#define STGM_CONVERT 0x20000
#define STGM_FAILIFTHERE 0
#define CWCSTORAGENAME 32
#define ASYNC_MODE_COMPATIBILITY	1
#define ASYNC_MODE_DEFAULT	0
#define STGTY_REPEAT	256
#define STG_TOEND	0xFFFFFFFF
#define STG_LAYOUT_SEQUENTIAL	0
#define STG_LAYOUT_INTERLEAVED	1
typedef enum tagREGCLS {
	REGCLS_SINGLEUSE = 0,
	REGCLS_MULTIPLEUSE = 1,
	REGCLS_MULTI_SEPARATE = 2
} REGCLS;
#include <wtypes.h>
#include <unknwn.h>
#include <objidl.h>

#ifdef __cplusplus
inline BOOL IsEqualGUID(REFGUID rguid1, REFGUID rguid2)
	{ return !memcmp(&rguid1, &rguid2, sizeof(GUID)); }
inline BOOL operator==(const GUID& guidOne, const GUID& guidOther)
	{ return !memcmp(&guidOne,&guidOther,sizeof(GUID)); }
inline BOOL operator!=(const GUID& g1, const GUID& g2)
	{ return !(g1 == g2); }
#else
#define IsEqualGUID(rguid1, rguid2) (!memcmp(rguid1, rguid2, sizeof(GUID)))
#endif
#define IsEqualIID(id1,id2) IsEqualGUID(id1,id2)
#define IsEqualCLSID(id1,id2) IsEqualGUID(id1,id2)
#include <cguid.h>

WINOLEAPI_(DWORD) CoBuildVersion(void);
WINOLEAPI CoInitialize(PVOID);
WINOLEAPI_(void) CoUninitialize(void);
WINOLEAPI CoGetMalloc(DWORD,LPMALLOC*);
WINOLEAPI_(DWORD) CoGetCurrentProcess(void);
WINOLEAPI CoRegisterMallocSpy(LPMALLOCSPY);
WINOLEAPI CoRevokeMallocSpy(void);
WINOLEAPI CoCreateStandardMalloc(DWORD,IMalloc**);
#if DBG == 1
WINOLEAPI_(ULONG) DebugCoGetRpcFault(void);
WINOLEAPI_(void) DebugCoSetRpcFault(ULONG);
#endif
WINOLEAPI CoGetClassObject(REFCLSID,DWORD,PVOID,REFIID,PVOID*);
WINOLEAPI CoRegisterClassObject(REFCLSID,LPUNKNOWN,DWORD,DWORD,PDWORD);
WINOLEAPI CoRevokeClassObject(DWORD);
WINOLEAPI CoGetMarshalSizeMax(ULONG*,REFIID,LPUNKNOWN,DWORD,PVOID,DWORD);
WINOLEAPI CoMarshalInterface(LPSTREAM,REFIID,LPUNKNOWN,DWORD,PVOID,DWORD);
WINOLEAPI CoUnmarshalInterface(LPSTREAM,REFIID,PVOID*);
WINOLEAPI CoMarshalHresult(LPSTREAM,HRESULT);
WINOLEAPI CoUnmarshalHresult(LPSTREAM,HRESULT*);
WINOLEAPI CoReleaseMarshalData(LPSTREAM);
WINOLEAPI CoDisconnectObject(LPUNKNOWN,DWORD);
WINOLEAPI CoLockObjectExternal(LPUNKNOWN,BOOL,BOOL);
WINOLEAPI CoGetStandardMarshal(REFIID,LPUNKNOWN,DWORD,PVOID,DWORD,LPMARSHAL*);
WINOLEAPI_(BOOL) CoIsHandlerConnected(LPUNKNOWN);
WINOLEAPI_(BOOL) CoHasStrongExternalConnections(LPUNKNOWN);
WINOLEAPI CoMarshalInterThreadInterfaceInStream(REFIID,LPUNKNOWN,LPSTREAM*);
WINOLEAPI CoGetInterfaceAndReleaseStream(LPSTREAM,REFIID,PVOID*);
WINOLEAPI CoCreateFreeThreadedMarshaler(LPUNKNOWN,LPUNKNOWN*);
WINOLEAPI_(HINSTANCE) CoLoadLibrary(LPOLESTR,BOOL);
WINOLEAPI_(void) CoFreeLibrary(HINSTANCE);
WINOLEAPI_(void) CoFreeAllLibraries(void);
WINOLEAPI_(void) CoFreeUnusedLibraries(void);
WINOLEAPI CoCreateInstance(REFCLSID,LPUNKNOWN,DWORD,REFIID,PVOID*);
WINOLEAPI StringFromCLSID(REFCLSID,LPOLESTR*);
WINOLEAPI CLSIDFromString(LPOLESTR,LPCLSID);
WINOLEAPI StringFromIID(REFIID,LPOLESTR*);
WINOLEAPI IIDFromString(LPOLESTR,LPIID);
WINOLEAPI_(BOOL) CoIsOle1Class(REFCLSID);
WINOLEAPI ProgIDFromCLSID(REFCLSID,LPOLESTR*);
WINOLEAPI CLSIDFromProgID(LPCOLESTR,LPCLSID);
WINOLEAPI_(int) StringFromGUID2(REFGUID,LPOLESTR,int);
WINOLEAPI CoCreateGuid(GUID*);
WINOLEAPI_(BOOL) CoFileTimeToDosDateTime(FILETIME*,LPWORD,LPWORD);
WINOLEAPI_(BOOL) CoDosDateTimeToFileTime(WORD,WORD,FILETIME*);
WINOLEAPI CoFileTimeNow(FILETIME*);
WINOLEAPI CoRegisterMessageFilter(LPMESSAGEFILTER,LPMESSAGEFILTER*);
WINOLEAPI CoGetTreatAsClass(REFCLSID,LPCLSID);
WINOLEAPI CoTreatAsClass(REFCLSID,REFCLSID);
typedef HRESULT (STDAPICALLTYPE *LPFNGETCLASSOBJECT)(REFCLSID,REFIID,PVOID*);
typedef HRESULT (STDAPICALLTYPE *LPFNCANUNLOADNOW)(void);
STDAPI DllGetClassObject(REFCLSID,REFIID,PVOID*);
STDAPI DllCanUnloadNow(void);
WINOLEAPI_(PVOID) CoTaskMemAlloc(ULONG);
WINOLEAPI_(PVOID) CoTaskMemRealloc(PVOID,ULONG);
WINOLEAPI_(void) CoTaskMemFree(PVOID);
WINOLEAPI CreateDataAdviseHolder(LPDATAADVISEHOLDER*);
WINOLEAPI CreateDataCache(LPUNKNOWN,REFCLSID,REFIID,PVOID*);
WINOLEAPI StgCreateDocfile(const OLECHAR*,DWORD,DWORD,IStorage**);
WINOLEAPI StgCreateDocfileOnILockBytes(ILockBytes*,DWORD,DWORD,IStorage**);
WINOLEAPI StgOpenStorage(const OLECHAR*,IStorage*,DWORD,SNB,DWORD,IStorage**);
WINOLEAPI StgOpenStorageOnILockBytes(ILockBytes*,IStorage*,DWORD,SNB,DWORD,IStorage**);
WINOLEAPI StgIsStorageFile(const OLECHAR*);
WINOLEAPI StgIsStorageILockBytes(ILockBytes*);
WINOLEAPI StgSetTimes(OLECHAR const*,FILETIME const*,FILETIME const*,FILETIME const*);
WINOLEAPI BindMoniker(LPMONIKER,DWORD,REFIID,PVOID*);
WINOLEAPI MkParseDisplayName(LPBC,LPCOLESTR,ULONG*,LPMONIKER*);
WINOLEAPI MonikerRelativePathTo(LPMONIKER,LPMONIKER,LPMONIKER*,BOOL);
WINOLEAPI MonikerCommonPrefixWith(LPMONIKER,LPMONIKER,LPMONIKER*);
WINOLEAPI CreateBindCtx(DWORD,LPBC*);
WINOLEAPI CreateGenericComposite(LPMONIKER,LPMONIKER,LPMONIKER*);
WINOLEAPI GetClassFile (LPCOLESTR,CLSID*);
WINOLEAPI CreateFileMoniker(LPCOLESTR,LPMONIKER*);
WINOLEAPI CreateItemMoniker(LPCOLESTR,LPCOLESTR,LPMONIKER*);
WINOLEAPI CreateAntiMoniker(LPMONIKER*);
WINOLEAPI CreatePointerMoniker(LPUNKNOWN,LPMONIKER*);
WINOLEAPI GetRunningObjectTable(DWORD,LPRUNNINGOBJECTTABLE*);
#pragma pack(pop)
#endif
