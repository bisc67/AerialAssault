// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChatSample.h"
#include "LoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// LoginDlg dialog


LoginDlg::LoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(LoginDlg::IDD, pParent),
	  m_CreateAccount(FALSE)
{
	//{{AFX_DATA_INIT(LoginDlg)
	m_Password = _T("");
	m_UserName = _T("");
	//}}AFX_DATA_INIT
}


void LoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LoginDlg)
	DDX_Control(pDX, IDC_USERNAME, m_UserNameCtrl);
	DDX_Text(pDX, IDC_PASSWORD, m_Password);
	DDX_Text(pDX, IDC_USERNAME, m_UserName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(LoginDlg, CDialog)
	//{{AFX_MSG_MAP(LoginDlg)
	ON_BN_CLICKED(IDCREATEACCOUNT, OnCreateAccount)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LoginDlg message handlers

BOOL LoginDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_UserNameCtrl.SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void LoginDlg::OnCreateAccount() 
{
	// TODO: Add your control notification handler code here
	m_CreateAccount = TRUE;
	OnOK();
	return;
}


