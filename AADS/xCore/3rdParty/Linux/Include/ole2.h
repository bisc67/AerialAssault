#ifndef _OLE2_H
#define _OLE2_H
#pragma pack(push,8)
#include <winerror.h>
#include <objbase.h>
#include <olectlid.h>
#include <oleauto.h>

#ifdef __cplusplus
extern "C" {
#endif
#define E_DRAW VIEW_E_DRAW
#define DATA_E_FORMATETC DV_E_FORMATETC
#define OLEIVERB_PRIMARY (0L)
#define OLEIVERB_SHOW (-1L)
#define OLEIVERB_OPEN (-2L)
#define OLEIVERB_HIDE (-3L)
#define OLEIVERB_UIACTIVATE (-4L)
#define OLEIVERB_INPLACEACTIVATE (-5L)
#define OLEIVERB_DISCARDUNDOSTATE (-6L)
#define EMBDHLP_INPROC_HANDLER 0x0000L
#define EMBDHLP_INPROC_SERVER 0x0001L
#define EMBDHLP_CREATENOW 0x00000000L
#define EMBDHLP_DELAYCREATE 0x00010000L
#include <oleidl.h>

typedef struct _OLESTREAMVTBL *LPOLESTREAMVTBL;
typedef struct _OLESTREAM {
	LPOLESTREAMVTBL lpstbl;
} OLESTREAM,*LPOLESTREAM;
typedef struct _OLESTREAMVTBL {
	DWORD (CALLBACK* Get)(LPOLESTREAM,void*,DWORD);
	DWORD (CALLBACK* Put)(LPOLESTREAM,const void*,DWORD);
} OLESTREAMVTBL;

WINOLEAPI CreateDataAdviseHolder(LPDATAADVISEHOLDER*);
WINOLEAPI_(DWORD) OleBuildVersion(void);
WINOLEAPI ReadClassStg(LPSTORAGE,CLSID*);
WINOLEAPI WriteClassStg(LPSTORAGE,REFCLSID);
WINOLEAPI ReadClassStm(LPSTREAM,CLSID*);
WINOLEAPI WriteClassStm(LPSTREAM,REFCLSID);
WINOLEAPI WriteFmtUserTypeStg(LPSTORAGE,CLIPFORMAT,LPOLESTR);
WINOLEAPI ReadFmtUserTypeStg(LPSTORAGE,CLIPFORMAT*,LPOLESTR*);
WINOLEAPI OleInitialize(PVOID);
WINOLEAPI_(void) OleUninitialize(void);
WINOLEAPI OleQueryLinkFromData(LPDATAOBJECT);
WINOLEAPI OleQueryCreateFromData(LPDATAOBJECT);
WINOLEAPI OleCreate(REFCLSID,REFIID,DWORD,LPFORMATETC,LPOLECLIENTSITE,LPSTORAGE,PVOID*);
WINOLEAPI OleCreateFromData(LPDATAOBJECT,REFIID,DWORD,LPFORMATETC,LPOLECLIENTSITE,LPSTORAGE,PVOID*);
WINOLEAPI OleCreateLinkFromData(LPDATAOBJECT,REFIID,DWORD,LPFORMATETC,LPOLECLIENTSITE,LPSTORAGE,PVOID*);
WINOLEAPI OleCreateStaticFromData(LPDATAOBJECT,REFIID,DWORD,LPFORMATETC,LPOLECLIENTSITE,LPSTORAGE,PVOID*);
WINOLEAPI OleCreateLink(LPMONIKER,REFIID,DWORD,LPFORMATETC,LPOLECLIENTSITE,LPSTORAGE,PVOID*);
WINOLEAPI OleCreateLinkToFile(LPCOLESTR,REFIID,DWORD,LPFORMATETC,LPOLECLIENTSITE,LPSTORAGE,PVOID*);
WINOLEAPI OleCreateFromFile(REFCLSID,LPCOLESTR,REFIID,DWORD,LPFORMATETC,LPOLECLIENTSITE,LPSTORAGE,PVOID*);
WINOLEAPI OleLoad(LPSTORAGE,REFIID,LPOLECLIENTSITE,PVOID*);
WINOLEAPI OleSave(LPPERSISTSTORAGE,LPSTORAGE,BOOL);
WINOLEAPI OleLoadFromStream(LPSTREAM,REFIID,PVOID*);
WINOLEAPI OleSaveToStream(LPPERSISTSTREAM,LPSTREAM);
WINOLEAPI OleSetContainedObject(LPUNKNOWN,BOOL);
WINOLEAPI OleNoteObjectVisible(LPUNKNOWN,BOOL);
WINOLEAPI RegisterDragDrop(HWND,LPDROPTARGET);
WINOLEAPI RevokeDragDrop(HWND);
WINOLEAPI DoDragDrop(LPDATAOBJECT,LPDROPSOURCE,DWORD,PDWORD);
WINOLEAPI OleSetClipboard(LPDATAOBJECT);
WINOLEAPI OleGetClipboard(LPDATAOBJECT*);
WINOLEAPI OleFlushClipboard(void);
WINOLEAPI OleIsCurrentClipboard(LPDATAOBJECT);
WINOLEAPI_(HOLEMENU) OleCreateMenuDescriptor(HMENU,LPOLEMENUGROUPWIDTHS);
WINOLEAPI OleSetMenuDescriptor(HOLEMENU,HWND,HWND,LPOLEINPLACEFRAME,LPOLEINPLACEACTIVEOBJECT);
WINOLEAPI OleDestroyMenuDescriptor(HOLEMENU);
WINOLEAPI OleTranslateAccelerator(LPOLEINPLACEFRAME,LPOLEINPLACEFRAMEINFO,LPMSG);
WINOLEAPI_(HANDLE) OleDuplicateData(HANDLE,CLIPFORMAT,UINT);
WINOLEAPI OleDraw(LPUNKNOWN,DWORD,HDC,LPCRECT);
WINOLEAPI OleRun(LPUNKNOWN);
WINOLEAPI_(BOOL) OleIsRunning(LPOLEOBJECT);
WINOLEAPI OleLockRunning(LPUNKNOWN,BOOL,BOOL);
WINOLEAPI_(void) ReleaseStgMedium(LPSTGMEDIUM);
WINOLEAPI CreateOleAdviseHolder(LPOLEADVISEHOLDER*);
WINOLEAPI OleCreateDefaultHandler(REFCLSID,LPUNKNOWN,REFIID,PVOID*);
WINOLEAPI OleCreateEmbeddingHelper(REFCLSID,LPUNKNOWN,DWORD,LPCLASSFACTORY,REFIID,PVOID*);
WINOLEAPI_(BOOL) IsAccelerator(HACCEL,int,LPMSG,WORD*);
WINOLEAPI_(HGLOBAL) OleGetIconOfFile(LPOLESTR,BOOL);
WINOLEAPI_(HGLOBAL) OleGetIconOfClass(REFCLSID,LPOLESTR,BOOL);
WINOLEAPI_(HGLOBAL) OleMetafilePictFromIconAndLabel(HICON,LPOLESTR,LPOLESTR,UINT);
WINOLEAPI OleRegGetUserType(REFCLSID,DWORD,LPOLESTR*);
WINOLEAPI OleRegGetMiscStatus(REFCLSID,DWORD,DWORD*);
WINOLEAPI OleRegEnumFormatEtc (REFCLSID,DWORD,LPENUMFORMATETC*);
WINOLEAPI OleRegEnumVerbs (REFCLSID,LPENUMOLEVERB*);
WINOLEAPI OleConvertOLESTREAMToIStorage(LPOLESTREAM,LPSTORAGE,const DVTARGETDEVICE*);
WINOLEAPI OleConvertIStorageToOLESTREAM(LPSTORAGE,LPOLESTREAM);
WINOLEAPI GetHGlobalFromILockBytes(LPLOCKBYTES,HGLOBAL*);
WINOLEAPI CreateILockBytesOnHGlobal(HGLOBAL,BOOL,LPLOCKBYTES*);
WINOLEAPI GetHGlobalFromStream(LPSTREAM,HGLOBAL*);
WINOLEAPI CreateStreamOnHGlobal(HGLOBAL,BOOL,LPSTREAM*);
WINOLEAPI OleDoAutoConvert(LPSTORAGE,LPCLSID);
WINOLEAPI OleGetAutoConvert(REFCLSID,LPCLSID);
WINOLEAPI OleSetAutoConvert(REFCLSID,REFCLSID);
WINOLEAPI GetConvertStg(LPSTORAGE);
WINOLEAPI SetConvertStg(LPSTORAGE,BOOL);
WINOLEAPI OleConvertIStorageToOLESTREAMEx(LPSTORAGE,CLIPFORMAT,LONG,LONG,DWORD,LPSTGMEDIUM,LPOLESTREAM);
WINOLEAPI OleConvertOLESTREAMToIStorageEx(LPOLESTREAM,LPSTORAGE,CLIPFORMAT*,LONG*,LONG*,DWORD*,LPSTGMEDIUM);
#ifdef __cplusplus
}
#endif
#pragma pack(pop)
#endif
