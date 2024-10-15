// DSAddIn.h : header file
//

#if !defined(AFX_DSADDIN_H__3C010DF4_89D0_4D72_AD37_85B5DA90D5F8__INCLUDED_)
#define AFX_DSADDIN_H__3C010DF4_89D0_4D72_AD37_85B5DA90D5F8__INCLUDED_

#include "commands.h"

// {AAA50EAF-582E-4161-9796-ED1BD08C65D1}
DEFINE_GUID(CLSID_DSAddIn,
0xaaa50eaf, 0x582e, 0x4161, 0x97, 0x96, 0xed, 0x1b, 0xd0, 0x8c, 0x65, 0xd1);

/////////////////////////////////////////////////////////////////////////////
// CDSAddIn

class CDSAddIn : 
	public IDSAddIn,
	public CComObjectRoot,
	public CComCoClass<CDSAddIn, &CLSID_DSAddIn>
{
public:
	DECLARE_REGISTRY(CDSAddIn, "xTB",
		"xTB Developer Studio Add-in", IDS_XTB_LONGNAME,
		THREADFLAGS_BOTH)

	CDSAddIn() {}
	BEGIN_COM_MAP(CDSAddIn)
		COM_INTERFACE_ENTRY(IDSAddIn)
	END_COM_MAP()
	DECLARE_NOT_AGGREGATABLE(CDSAddIn)

// IDSAddIns
public:
	STDMETHOD(OnConnection)(THIS_ IApplication* pApp, VARIANT_BOOL bFirstTime,
		long dwCookie, VARIANT_BOOL* pOnConnection);
	STDMETHOD(OnDisconnection)(THIS_ VARIANT_BOOL bLastTime);

	HRESULT AddCmd( LPCTSTR         szCommandMethod, 
                    int             stringID,
                    IApplication*   pApplication, 
                    VARIANT_BOOL    bFirstTime,
                    VARIANT_BOOL*   pOnConnection );

protected:
	CCommandsObj* m_pCommands;
	DWORD m_dwCookie;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSADDIN_H__3C010DF4_89D0_4D72_AD37_85B5DA90D5F8__INCLUDED)
