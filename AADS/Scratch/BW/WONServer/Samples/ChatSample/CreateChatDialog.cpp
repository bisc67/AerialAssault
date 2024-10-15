// CreateChatDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ChatSample.h"
#include "CreateChatDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CreateChatDialog dialog


CreateChatDialog::CreateChatDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CreateChatDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CreateChatDialog)
	m_Name = _T("");
	m_PasswordCheck = FALSE;
	m_Password = _T("");
	m_AllowObservers = TRUE;
	m_AskToJoin = FALSE;
	m_InviteOnly = FALSE;
	//}}AFX_DATA_INIT

	mIsChat = true;
}


void CreateChatDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CreateChatDialog)
	DDX_Control(pDX, IDC_NAME, m_NameEdit);
	DDX_Control(pDX, IDC_INVITE_ONLY, m_InviteOnlyCtrl);
	DDX_Control(pDX, IDC_ASK_TO_JOIN, m_AskToJoinCtrl);
	DDX_Control(pDX, IDC_ALLOW_OBSERVERS, m_AllowObserversCtrl);
	DDX_Control(pDX, IDC_PASSWORD, m_PasswordEdit);
	DDX_Control(pDX, IDC_PASSWORD_LABEL, m_PasswordLabel);
	DDX_Text(pDX, IDC_NAME, m_Name);
	DDX_Check(pDX, IDC_PASSWORD_CHECK, m_PasswordCheck);
	DDX_Text(pDX, IDC_PASSWORD, m_Password);
	DDX_Check(pDX, IDC_ALLOW_OBSERVERS, m_AllowObservers);
	DDX_Check(pDX, IDC_ASK_TO_JOIN, m_AskToJoin);
	DDX_Check(pDX, IDC_INVITE_ONLY, m_InviteOnly);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CreateChatDialog, CDialog)
	//{{AFX_MSG_MAP(CreateChatDialog)
	ON_BN_CLICKED(IDC_PASSWORD_CHECK, OnPasswordCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CreateChatDialog message handlers


void CreateChatDialog::OnPasswordCheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int aCmd = m_PasswordCheck?SW_SHOW:SW_HIDE;
	m_PasswordEdit.ShowWindow(aCmd);
	m_PasswordLabel.ShowWindow(aCmd);
}

BOOL CreateChatDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int aShowFlag = mIsChat?SW_HIDE:SW_SHOW;
	m_AllowObserversCtrl.ShowWindow(aShowFlag);
	m_AskToJoinCtrl.ShowWindow(aShowFlag);
	m_InviteOnlyCtrl.ShowWindow(aShowFlag);

	if(mIsChat)
		SetWindowText("Create Chat Room");
	else
		SetWindowText("Create Game");
	
	this->m_NameEdit.SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
				   // EXCEPTION: OCX Property Pages should return FALSE
}
