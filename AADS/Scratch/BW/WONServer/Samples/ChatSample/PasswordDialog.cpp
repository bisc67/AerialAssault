// PasswordDialog.cpp : implementation file
//
#pragma warning (disable : 4786)
#include "stdafx.h"
#include "ChatSample.h"
#include "PasswordDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PasswordDialog dialog


PasswordDialog::PasswordDialog(CWnd* pParent /*=NULL*/)
	: CDialog(PasswordDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(PasswordDialog)
	m_Password = _T("");
	//}}AFX_DATA_INIT
}


void PasswordDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PasswordDialog)
	DDX_Control(pDX, IDC_PASSWORD, m_PasswordEdit);
	DDX_Text(pDX, IDC_PASSWORD, m_Password);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PasswordDialog, CDialog)
	//{{AFX_MSG_MAP(PasswordDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PasswordDialog message handlers

BOOL PasswordDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetWindowText(mCaption);
	m_PasswordEdit.SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	               // EXCEPTION: OCX Property Pages should return FALSE
}
