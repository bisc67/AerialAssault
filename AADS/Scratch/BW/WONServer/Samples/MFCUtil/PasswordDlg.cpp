// PasswordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mfcutil.h"
#include "PasswordDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PasswordDlg dialog


PasswordDlg::PasswordDlg(bool retry, CWnd* pParent /*=NULL*/)
	: CDialog(PasswordDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(PasswordDlg)
	m_Password = _T("");
	//}}AFX_DATA_INIT
	mRetry = retry;
}


void PasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PasswordDlg)
	DDX_Control(pDX, IDC_PASSWORD, m_PasswordCtrl);
	DDX_Text(pDX, IDC_PASSWORD, m_Password);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PasswordDlg, CDialog)
	//{{AFX_MSG_MAP(PasswordDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// PasswordDlg message handlers

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
BOOL PasswordDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if(mRetry)
		SetWindowText("Invalid password.  Try Again.");

	m_PasswordCtrl.SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
