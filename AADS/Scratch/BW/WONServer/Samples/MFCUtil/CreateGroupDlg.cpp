// CreateGroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mfcutil.h"
#include "CreateGroupDlg.h"
#include "WONCommon/StringUtil.h"

using namespace WONAPI;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CreateGroupDlg dialog


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
CreateGroupDlg::CreateGroupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CreateGroupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CreateGroupDlg)
	m_AllowObservers = TRUE;
	m_AskToJoin = FALSE;
	m_InviteOnly = FALSE;
	m_Name = _T("");
	m_Password = _T("");
	m_PasswordCheck = FALSE;
	//}}AFX_DATA_INIT
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CreateGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CreateGroupDlg)
	DDX_Control(pDX, IDC_NAME, m_NameCtrl);
	DDX_Control(pDX, IDC_PASSWORD, m_PasswordCtrl);
	DDX_Control(pDX, IDOK, m_CreateButton);
	DDX_Control(pDX, IDC_PASSWORD_LABEL, m_PasswordLabel);
	DDX_Check(pDX, IDC_ALLOW_OBSERVERS, m_AllowObservers);
	DDX_Check(pDX, IDC_ASK_TO_JOIN, m_AskToJoin);
	DDX_Check(pDX, IDC_INVITE_ONLY, m_InviteOnly);
	DDX_Text(pDX, IDC_NAME, m_Name);
	DDX_Text(pDX, IDC_PASSWORD, m_Password);
	DDX_Check(pDX, IDC_PASSWORD_CHECK, m_PasswordCheck);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CreateGroupDlg, CDialog)
	//{{AFX_MSG_MAP(CreateGroupDlg)
	ON_BN_CLICKED(IDC_PASSWORD_CHECK, OnPasswordCheck)
	ON_EN_CHANGE(IDC_NAME, OnChangeName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
RoutingCreateGroupOpPtr CreateGroupDlg::GetCreateGroupOp(bool release)
{
	RoutingCreateGroupOpPtr anOp = mCreateGroupOp;
	if(release)
		mCreateGroupOp = NULL;

	return anOp;
}


///////////////////////////////////////////////////////////////////////////////
// CreateGroupDlg message handlers

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
BOOL CreateGroupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	OnChangeName();
	m_NameCtrl.SetFocus();
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CreateGroupDlg::OnPasswordCheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int aCmd = m_PasswordCheck?SW_SHOW:SW_HIDE;

	m_PasswordLabel.ShowWindow(aCmd);
	m_PasswordCtrl.ShowWindow(aCmd);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CreateGroupDlg::OnChangeName() 
{	
	// TODO: Add your control notification handler code here
	m_CreateButton.EnableWindow(m_NameCtrl.GetWindowTextLength()>0);	
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CreateGroupDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	if(!mRoutingConnection->IsValid()) // not connected anymore
		return;		

	RoutingCreateGroupOpPtr anOp = new RoutingCreateGroupOp(mRoutingConnection);
	anOp->SetGroupId(RoutingId_Invalid); // create group
	anOp->SetGroupName(StringToWString((const char*)m_Name));
	if(m_PasswordCheck)
		anOp->SetGroupPassword(StringToWString((const char*)m_Password));

	anOp->SetJoinFlags(RoutingCreateGroupJoinFlag_JoinAsPlayer);

	// Figure out group flags
	unsigned long aGroupFlags = 0;
	if(m_AllowObservers)
		aGroupFlags |= RoutingGroupFlag_AllowObservers | RoutingGroupFlag_AllowObserversToChat | RoutingGroupFlag_DetailedObservers;

	if(m_AskToJoin)
		aGroupFlags |= RoutingGroupFlag_AskCaptainToJoin;

	if(m_InviteOnly)
		aGroupFlags |= RoutingGroupFlag_InviteOnly;;

	anOp->SetGroupFlags(aGroupFlags);

	mCreateGroupOp = anOp;
	CDialog::OnOK();
}
