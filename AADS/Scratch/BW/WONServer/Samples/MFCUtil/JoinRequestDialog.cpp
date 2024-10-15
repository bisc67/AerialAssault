// JoinRequestDialog.cpp : implementation file
//

#include "stdafx.h"
#include "mfcutil.h"
#include "JoinRequestDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// JoinRequestDialog dialog


///////////////////////////////////////////////////////////////////////////////
JoinRequestDialog::JoinRequestDialog(CWnd* pParent /*=NULL*/)
	: CDialog(JoinRequestDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(JoinRequestDialog)
	m_Comment = _T("");
	m_JoinText = _T("");
	//}}AFX_DATA_INIT
}


///////////////////////////////////////////////////////////////////////////////
void JoinRequestDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(JoinRequestDialog)
	DDX_Text(pDX, IDC_COMMENT, m_Comment);
	DDX_Text(pDX, IDC_JOIN_TEXT, m_JoinText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(JoinRequestDialog, CDialog)
	//{{AFX_MSG_MAP(JoinRequestDialog)
	ON_BN_CLICKED(IDC_ACCEPT_BUTTON, OnAcceptButton)
	ON_BN_CLICKED(IDC_REJECT_BUTTON, OnRejectButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// JoinRequestDialog message handlers

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void JoinRequestDialog::OnAcceptButton() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	EndDialog(IDOK);

}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void JoinRequestDialog::OnRejectButton() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	EndDialog(IDCANCEL);
}
