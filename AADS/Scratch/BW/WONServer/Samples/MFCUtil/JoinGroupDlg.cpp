// JoinGroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mfcutil.h"
#include "JoinGroupDlg.h"
#include "PasswordDlg.h"
#include "CancelDlg.h"
#include "WONCommon/StringUtil.h"
#include "WONRouting/RoutingJoinGroupOp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace WONAPI;

///////////////////////////////////////////////////////////////////////////////
// JoinGroupDlg dialog


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
JoinGroupDlg::JoinGroupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(JoinGroupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(JoinGroupDlg)
	//}}AFX_DATA_INIT
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void JoinGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(JoinGroupDlg)
	DDX_Control(pDX, IDC_GROUP_DETAILS, m_GroupDetails);
	DDX_Control(pDX, IDOK, m_JoinButton);
	DDX_Control(pDX, IDC_OBSERVE, m_ObserveButton);
	DDX_Control(pDX, IDC_GROUP_LIST, m_GroupList);
	//}}AFX_DATA_MAP
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void JoinGroupDlg::Init(WONAPI::SmartRoutingConnection *theConnection, unsigned char getGroupListFlags)
{
	mRoutingConnection = theConnection;
	if(getGroupListFlags!=0)
		mRoutingConnection->GetGroupList(getGroupListFlags);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void JoinGroupDlg::PopulateList()
{
	m_GroupList.ResetContent();
	ClearDetails();
	m_GroupDetails.SetWindowText("");
	mGroupMap = mRoutingConnection->GetGroupMapRef();

	RoutingGroupMap::iterator anItr = mGroupMap.begin();
	while(anItr!=mGroupMap.end())
	{
		RoutingGroupInfo *anInfo = anItr->second;
		int aPos = m_GroupList.AddString(WStringToString(anInfo->mName).c_str());
		m_GroupList.SetItemDataPtr(aPos,anInfo);
		++anItr;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(JoinGroupDlg, CDialog)
	//{{AFX_MSG_MAP(JoinGroupDlg)
	ON_LBN_SELCHANGE(IDC_GROUP_LIST, OnSelchangeGroupList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// JoinGroupDlg message handlers

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
BOOL JoinGroupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	int aSize = 8;
	m_GroupDetails.SetTabStops(aSize);
	PopulateList();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void JoinGroupDlg::ClearDetails()
{
	m_GroupDetails.SetWindowText("");
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void JoinGroupDlg::AddDetail(const char *theDetail, bool addLineFeed)
{
	m_GroupDetails.SetSel(m_GroupDetails.GetWindowTextLength(),m_GroupDetails.GetWindowTextLength());
	m_GroupDetails.ReplaceSel(theDetail);
	if(addLineFeed)
	{
		m_GroupDetails.SetSel(m_GroupDetails.GetWindowTextLength(),m_GroupDetails.GetWindowTextLength());
		m_GroupDetails.ReplaceSel("\r\n");
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void JoinGroupDlg::OnSelchangeGroupList() 
{
	// TODO: Add your control notification handler code here
	ClearDetails();

	int aPos = m_GroupList.GetCurSel();
	if(aPos==LB_ERR)
	{
		m_JoinButton.EnableWindow(FALSE);
		m_ObserveButton.EnableWindow(FALSE);
		return;
	}	

	mGroup = (RoutingGroupInfo*)m_GroupList.GetItemDataPtr(aPos);

	// Show captain
	int aCaptainId = mGroup->mCaptainId;
	if(aCaptainId!=RoutingId_Invalid)
	{
		RoutingClientInfo *aClient = mRoutingConnection->GetClientRef(aCaptainId);
		if(aClient!=NULL)
		{
			AddDetail("Captain: ",false);
			AddDetail(WStringToString(aClient->mName).c_str());
			AddDetail("");
		}
	}

	// Show group flags
	AddDetail("Group Settings:");
	unsigned long aFlags = mGroup->mFlags;
	if(aFlags & RoutingGroupFlag_Closed)
		AddDetail("\tGroup Closed");
	if(aFlags & RoutingGroupFlag_PasswordProtected)
		AddDetail("\tPassword Required");
	if(aFlags & RoutingGroupFlag_InviteOnly)
		AddDetail("\tInvite Only");
	if(aFlags & RoutingGroupFlag_AskCaptainToJoin)
		AddDetail("\tCaptain Must Approve");
	if(aFlags & RoutingGroupFlag_IsChatRoom)
		AddDetail("\tChat Room");
	AddDetail("");

	// Show members or member count
	const RoutingMemberMap &aMap = mGroup->mMemberMap;
	if(!aMap.empty())
	{
		AddDetail("Members:");
		RoutingMemberMap::const_iterator anItr = aMap.begin();
		while(anItr!=aMap.end())
		{
			RoutingMemberInfo *aMember = anItr->second;
			AddDetail("\t",false);
			AddDetail(WStringToString(aMember->mClientInfo->mName).c_str());
			++anItr;
		}
		AddDetail("");
	}
	else if(mGroup->mMemberCount>0)
	{
		CString aStr;
		aStr.Format("Num Members: %d",mGroup->mMemberCount);
		AddDetail(aStr);
		AddDetail("");
	}


	bool enableJoin = true;
	if(aFlags & RoutingGroupFlag_Closed)
		enableJoin = false;
	if(mGroup->mMaxPlayers!=0 && mGroup->mMemberCount>=mGroup->mMaxPlayers)
		enableJoin = false;
	m_JoinButton.EnableWindow(enableJoin);

	m_ObserveButton.EnableWindow(aFlags & RoutingGroupFlag_AllowObservers);

}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void JoinGroupDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(!mRoutingConnection->IsValid()) // not connected anymore
		return;		

	RoutingJoinGroupOpPtr anOp;

	bool triedPassword = false;
	bool needPassword = false;
	bool firstTime = true;
	std::wstring aPassword;
	if(mGroup->mFlags & RoutingGroupFlag_PasswordProtected)
		needPassword = true;

	while(true)
	{
		// Read password if necessary
		if(needPassword)
		{
			PasswordDlg aDlg(triedPassword);
			if(aDlg.DoModal()==IDCANCEL)
				return;

			triedPassword = true;
			aPassword = StringToWString((const char*)aDlg.m_Password);
		}

		// Try to join group
		anOp = new RoutingJoinGroupOp(mRoutingConnection);
		anOp->SetGroupId(mGroup->mId);
		anOp->SetJoinFlags(RoutingJoinGroupFlag_GetMembersOfGroup);
		anOp->SetGroupPassword(aPassword);
		if(firstTime && mPreJoinCompletion.get()!=NULL) // notify caller (e.g. in case he needs to leave his current group)
		{
			mPreJoinCompletion->Complete(anOp.get());
			firstTime = false;
		}

		CancelDlg aDlg("Waiting for join response.","Waiting...");
		aDlg.SetCompletion(mJoinCompletion);
		int aResult = aDlg.DoOp(anOp,NULL,OP_TIMEOUT_INFINITE,false);
		if(aResult==IDCANCEL)
		{
			mRoutingConnection->CancelJoinGroup(mGroup->mId,true);
			return;
		}

		if(anOp->Succeeded())
			break;

		if(anOp->GetStatus()!=WS_RoutingServG2_InvalidPassword)
		{
			WONStatus aStatus = anOp->GetStatus();
			CString aStr;
			aStr.Format("Error Joining: %s",WONStatusToString(aStatus));
			if(!anOp->GetJoinComment().empty())
			{
				aStr += "\r\n";
				aStr += "Comment: ";
				aStr += WStringToString(anOp->GetJoinComment()).c_str();
			}

			MessageBox(aStr,"Error.");
			return;
		}

		needPassword = true;
	}
	
	CDialog::OnOK();
}
