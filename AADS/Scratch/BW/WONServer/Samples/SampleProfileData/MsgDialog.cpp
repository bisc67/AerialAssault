// MsgDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SampleProfileData.h"
#include "MsgDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsgDialog dialog


CMsgDialog::CMsgDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMsgDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMsgDialog)
	m_Message = _T("");
	//}}AFX_DATA_INIT
}


void CMsgDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsgDialog)
	DDX_Text(pDX, IDC_MESSAGE, m_Message);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMsgDialog, CDialog)
	//{{AFX_MSG_MAP(CMsgDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgDialog message handlers
