#ifndef _PRSHT_H
#define _PRSHT_H
#ifdef __cplusplus
extern "C" {
#endif

#define MAXPROPPAGES	100
#define PSP_DEFAULT	0
#define PSP_DLGINDIRECT	1
#define PSP_USEHICON	2
#define PSP_USEICONID	4
#define PSP_USETITLE	8
#define PSP_RTLREADING	16
#define PSP_HASHELP	32
#define PSP_USEREFPARENT	64
#define PSP_USECALLBACK	128
#define PSPCB_RELEASE	1
#define PSPCB_CREATE	2
#define PSH_DEFAULT	0
#define PSH_PROPTITLE	1
#define PSH_USEHICON	2
#define PSH_USEICONID	4
#define PSH_PROPSHEETPAGE	8
#define PSH_WIZARD	32
#define PSH_USEPSTARTPAGE	64
#define PSH_NOAPPLYNOW	128
#define PSH_USECALLBACK	256
#define PSH_HASHELP	512
#define PSH_MODELESS	1024
#define PSH_RTLREADING	2048
#define PSCB_INITIALIZED	1
#define PSCB_PRECREATE	2
#define PSM_GETTABCONTROL	1140
#define PSM_GETCURRENTPAGEHWND	1142
#define PSM_ISDIALOGMESSAGE	1141
#define PSM_PRESSBUTTON	1137
#define PSM_SETCURSELID	1138
#define PSM_SETFINISHTEXTW	1145
#define PSM_SETFINISHTEXTA	1139
#define PSN_FIRST	(-200)
#define PSN_LAST	(-299)
#define PSN_APPLY	(-202)
#define PSN_HELP	(-205)
#define PSN_KILLACTIVE	(-201)
#define PSN_QUERYCANCEL	(-209)
#define PSN_RESET	(-203)
#define PSN_SETACTIVE	(-200)
#define PSN_WIZBACK	(-206)
#define PSN_WIZFINISH	(-208)
#define PSN_WIZNEXT	(-207)
#define PSNRET_NOERROR	0
#define PSNRET_INVALID	1
#define PSNRET_INVALID_NOCHANGEPAGE	2
#define ID_PSRESTARTWINDOWS	2
#define ID_PSREBOOTSYSTEM 	3
#define WIZ_CXDLG	276
#define WIZ_CYDLG	140
#define WIZ_CXBMP	80
#define WIZ_BODYX	92
#define WIZ_BODYCX	184
#define PROP_SM_CXDLG	212
#define PROP_SM_CYDLG	188
#define PROP_MED_CXDLG	227
#define PROP_MED_CYDLG	215
#define PROP_LG_CXDLG	252
#define PROP_LG_CYDLG	218
#define PSBTN_MAX	6
#define PSBTN_BACK	0
#define PSBTN_NEXT	1
#define PSBTN_FINISH	2
#define PSBTN_OK	3
#define PSBTN_APPLYNOW	4
#define PSBTN_CANCEL	5
#define PSBTN_HELP	6
#define PSWIZB_BACK	1
#define PSWIZB_NEXT	2
#define PSWIZB_FINISH	4
#define PSWIZB_DISABLEDFINISH	8
#define PSM_SETWIZBUTTONS	(WM_USER+112)
#define PSM_APPLY	(WM_USER+110)
#define PSM_UNCHANGED	(WM_USER+109)
#define PSM_QUERYSIBLINGS	(WM_USER+108)
#define PSM_CANCELTOCLOSE	(WM_USER+107)
#define PSM_REBOOTSYSTEM	(WM_USER+106)
#define PSM_RESTARTWINDOWS	(WM_USER+105)
#define PSM_CHANGED	(WM_USER+104)
#define PSM_ADDPAGE	(WM_USER+103)
#define PSM_REMOVEPAGE	(WM_USER+102)
#define PSM_SETCURSEL	(WM_USER+101)
#define PSM_SETTITLEA	(WM_USER+111)
#define PSM_SETTITLEW	(WM_USER+120)

#ifndef RC_INVOKED

#pragma pack(push,8)
typedef struct _PROPSHEETPAGEA {
	DWORD	dwSize;
	DWORD	dwFlags;
	HINSTANCE	hInstance;
	union {
		LPCSTR	pszTemplate;
		LPCDLGTEMPLATE	pResource;
	} DUMMYUNIONNAME;
	union {
		HICON hIcon;
		LPCSTR pszIcon;
	} DUMMYUNIONNAME2;
	LPCSTR	pszTitle;
	DLGPROC	pfnDlgProc;
	LPARAM	lParam;
	UINT(CALLBACK *pfnCallback)(HWND,UINT,struct _PROPSHEETPAGEA*);
	UINT *pcRefParent;
} PROPSHEETPAGEA,*LPPROPSHEETPAGEA;
typedef const PROPSHEETPAGEA *LPCPROPSHEETPAGEA;
typedef struct _PROPSHEETPAGEW {
	DWORD	dwSize;
	DWORD	dwFlags;
	HINSTANCE	hInstance;
	union {
		LPCWSTR	pszTemplate;
		LPCDLGTEMPLATE	pResource;
	} DUMMYUNIONNAME;
	union {
		HICON hIcon;
		LPCWSTR pszIcon;
	} DUMMYUNIONNAME2;
	LPCWSTR	pszTitle;
	DLGPROC	pfnDlgProc;
	LPARAM	lParam;
	UINT(CALLBACK *pfnCallback)(HWND,UINT,struct _PROPSHEETPAGEW*);
	UINT *pcRefParent;
} PROPSHEETPAGEW,*LPPROPSHEETPAGEW;
typedef const PROPSHEETPAGEA *LPCPROPSHEETPAGEW;
typedef UINT(CALLBACK *LPFNPSPCALLBACKA)(HWND,UINT,LPPROPSHEETPAGEA);
typedef UINT(CALLBACK *LPFNPSPCALLBACKW)(HWND,UINT,LPPROPSHEETPAGEW);
typedef int(CALLBACK *PFNPROPSHEETCALLBACK)(HWND,UINT,LPARAM);
DECLARE_HANDLE(HPROPSHEETPAGE);
typedef struct _PROPSHEETHEADERA {
	DWORD	dwSize;
	DWORD	dwFlags;
	HWND	hwndParent;
	HINSTANCE	hInstance;
	union {
		HICON	hIcon;
		LPCSTR	pszIcon;
	}DUMMYUNIONNAME;
	LPCSTR	pszCaption;
	UINT	nPages;
	union {
		UINT	nStartPage;
		LPCSTR	pStartPage;
	}DUMMYUNIONNAME2;
	union {
		LPCPROPSHEETPAGEA ppsp;
		HPROPSHEETPAGE *phpage;
	}DUMMYUNIONNAME3;
	PFNPROPSHEETCALLBACK pfnCallback;
} PROPSHEETHEADERA,*LPPROPSHEETHEADERA;
typedef const PROPSHEETHEADERA *LPCPROPSHEETHEADERA;
typedef struct _PROPSHEETHEADERW {
	DWORD	dwSize;
	DWORD	dwFlags;
	HWND	hwndParent;
	HINSTANCE	hInstance;
	union {
		HICON	hIcon;
		LPCWSTR	pszIcon;
	}DUMMYUNIONNAME;
	LPCWSTR	pszCaption;
	UINT	nPages;
	union {
		UINT	nStartPage;
		LPCWSTR	pStartPage;
	}DUMMYUNIONNAME2;
	union {
		LPCPROPSHEETPAGEW ppsp;
		HPROPSHEETPAGE *phpage;
	}DUMMYUNIONNAME3;
	PFNPROPSHEETCALLBACK pfnCallback;
} PROPSHEETHEADERW,*LPPROPSHEETHEADERW;
typedef const PROPSHEETHEADERW *LPCPROPSHEETHEADERW;
typedef BOOL(CALLBACK *LPFNADDPROPSHEETPAGE)(HPROPSHEETPAGE,LPARAM);
typedef BOOL(CALLBACK *LPFNADDPROPSHEETPAGES)(LPVOID,LPFNADDPROPSHEETPAGE,LPARAM);
typedef struct _PSHNOTIFY {
	NMHDR hdr;
	LPARAM lParam;
} PSHNOTIFY,*LPPSHNOTIFY;

#pragma pack(pop)

HPROPSHEETPAGE WINAPI CreatePropertySheetPageA(LPCPROPSHEETPAGEA);
HPROPSHEETPAGE WINAPI CreatePropertySheetPageW(LPCPROPSHEETPAGEW);
BOOL WINAPI DestroyPropertySheetPage(HPROPSHEETPAGE);
int WINAPI PropertySheetA(LPCPROPSHEETHEADERA);
int WINAPI PropertySheetW(LPCPROPSHEETHEADERW);
#define PropSheet_AddPage(d,p) SendMessage(d,PSM_ADDPAGE,0,(LPARAM)p)
#define PropSheet_Apply(d) SendMessage(d,PSM_APPLY,0,0)
#define PropSheet_CancelToClose(d) SendMessage(d,PSM_CANCELTOCLOSE,0,0)
#define PropSheet_Changed(d,w) SendMessage(d,PSM_CHANGED,(WPARAM)w,0)
#define PropSheet_GetCurrentPageHwnd(d) (HWND)SendMessage(d,PSM_GETCURRENTPAGEHWND,0,0)
#define PropSheet_GetTabControl(d) (HWND)SendMessage(d,PSM_GETTABCONTROL,0,0)
#define PropSheet_IsDialogMessage(d,m) (BOOL)SendMessage(d,PSM_ISDIALOGMESSAGE,0,(LPARAM)m)
#define PropSheet_PressButton(d,i) SendMessage(d,PSM_PRESSBUTTON,i,0)
#define PropSheet_QuerySiblings(d,w,l) SendMessage(d,PSM_QUERYSIBLINGS,w,l)
#define PropSheet_RebootSystem(d) SendMessage(d,PSM_REBOOTSYSTEM,0,0)
#define PropSheet_RemovePage(d,i,p) SendMessage(d,PSM_REMOVEPAGE,i,(LPARAM)p)
#define PropSheet_RestartWindows(d) SendMessage(d,PSM_RESTARTWINDOWS,0,0)
#define PropSheet_SetCurSel(d,p,i) SendMessage(d,PSM_SETCURSEL,i,(LPARAM)p)
#define PropSheet_SetCurSelByID(d,i) SendMessage(d,PSM_SETCURSELID,0,i)
#define PropSheet_SetFinishText(d,s) SendMessage(d,PSM_SETFINISHTEXT,0,(LPARAM)s)
#define PropSheet_SetTitle(d,w,s) SendMessage(d,PSM_SETTITLE,w,(LPARAM)s)
#define PropSheet_SetWizButtons(d,f) PostMessage(d,PSM_SETWIZBUTTONS,0,(LPARAM)f)
#define PropSheet_UnChanged(d,w) SendMessage(d,PSM_UNCHANGED,(WPARAM)w,0)
#endif

#ifdef UNICODE
#define LPFNPSPCALLBACK	LPFNPSPCALLBACKW
#define PROPSHEETPAGE	PROPSHEETPAGEW
#define LPPROPSHEETPAGE	LPPROPSHEETPAGEW
#define LPCPROPSHEETPAGE	LPCPROPSHEETPAGEW
#define PROPSHEETHEADER	PROPSHEETHEADERW
#define LPPROPSHEETHEADER	LPPROPSHEETHEADERW
#define LPCPROPSHEETHEADER	LPCPROPSHEETHEADERW
#define PSM_SETTITLE PSM_SETTITLEW
#define PSM_SETFINISHTEXT PSM_SETFINISHTEXTW
#define CreatePropertySheetPage CreatePropertySheetPageW
#define PropertySheet PropertySheetW
#else
#define LPFNPSPCALLBACK	LPFNPSPCALLBACKA
#define PROPSHEETPAGE	PROPSHEETPAGEA
#define LPPROPSHEETPAGE	LPPROPSHEETPAGEA
#define LPCPROPSHEETPAGE	LPCPROPSHEETPAGEA
#define PROPSHEETHEADER	PROPSHEETHEADERA
#define LPPROPSHEETHEADER	LPPROPSHEETHEADERA
#define LPCPROPSHEETHEADER	LPCPROPSHEETHEADERA
#define PSM_SETTITLE PSM_SETTITLEA
#define PSM_SETFINISHTEXT PSM_SETFINISHTEXTA
#define CreatePropertySheetPage CreatePropertySheetPageA
#define PropertySheet PropertySheetA
#endif
#ifdef __cplusplus
}
#endif
#endif
