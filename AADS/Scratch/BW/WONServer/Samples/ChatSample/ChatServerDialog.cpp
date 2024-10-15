// ChatServerDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ChatSample.h"
#include "ChatServerDialog.h"
#include "WONCommon/StringUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace WONAPI;
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// ChatServerDialog dialog


ChatServerDialog::ChatServerDialog(const DirEntityList &theEntityList, CWnd* pParent)   // standard constructor
	: CDialog(ChatServerDialog::IDD, pParent), mEntityList(theEntityList)
{
	//{{AFX_DATA_INIT(ChatServerDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void ChatServerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ChatServerDialog)
	DDX_Control(pDX, IDC_CHAT_SERVER_LIST, m_ServerList);
	DDX_Control(pDX, IDOK, m_JoinButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ChatServerDialog, CDialog)
	//{{AFX_MSG_MAP(ChatServerDialog)
	ON_LBN_SELCHANGE(IDC_CHAT_SERVER_LIST, OnSelchangeChatServerList)
	ON_LBN_DBLCLK(IDC_CHAT_SERVER_LIST, OnDblclkChatServerList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ChatServerDialog message handlers

void ChatServerDialog::OnSelchangeChatServerList() 
{
	// TODO: Add your control notification handler code here
	m_JoinButton.EnableWindow(m_ServerList.GetCurSel()!=LB_ERR);
}

void ChatServerDialog::OnOK() 
{
	// TODO: Add extra validation here
	int aSel = m_ServerList.GetCurSel();
	if(aSel<0)
		return;

	mSelEntity = (const DirEntity*)m_ServerList.GetItemDataPtr(aSel);
	CDialog::OnOK();
}

BOOL ChatServerDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	DirEntityList::iterator anItr = mEntityList.begin();
	while(anItr!=mEntityList.end())
	{
		int anIndex = m_ServerList.AddString(WStringToString((*anItr)->mDisplayName).c_str());
		m_ServerList.SetItemDataPtr(anIndex,(void*)anItr->get());
		++anItr;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void ChatServerDialog::OnDblclkChatServerList() 
{
	// TODO: Add your control notification handler code here
	OnOK();
}
