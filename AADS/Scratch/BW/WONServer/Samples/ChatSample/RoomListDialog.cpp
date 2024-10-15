// RoomListDialog.cpp : implementation file
//
#pragma warning (disable : 4786)
#include "stdafx.h"
#include "ChatSample.h"
#include "RoomListDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;
using namespace WONAPI;

/////////////////////////////////////////////////////////////////////////////
// RoomListDialog dialog


RoomListDialog::RoomListDialog(CWnd* pParent /*=NULL*/)
	: CDialog(RoomListDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(RoomListDialog)
	//}}AFX_DATA_INIT
	mIsChat = true;
}


void RoomListDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RoomListDialog)
	DDX_Control(pDX, IDC_ROOM_LIST, m_RoomList);
	DDX_Control(pDX, IDC_MEMBER_LIST, m_MemberList);
	DDX_Control(pDX, IDC_OBSERVE_BUTTON, m_ObserveButton);
	DDX_Control(pDX, IDC_JOIN_BUTTON, m_JoinButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RoomListDialog, CDialog)
	//{{AFX_MSG_MAP(RoomListDialog)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_ROOM_LIST, OnColumnclickRoomList)
	ON_NOTIFY(NM_CLICK, IDC_ROOM_LIST, OnClickRoomList)
	ON_BN_CLICKED(IDC_JOIN_BUTTON, OnJoinButton)
	ON_BN_CLICKED(IDC_OBSERVE_BUTTON, OnObserveButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RoomListDialog message handlers


BOOL RoomListDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here

	DWORD style = m_RoomList.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
	style |= LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP;
	m_RoomList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, style);

	CRect aRect;
	m_RoomList.GetClientRect(&aRect);

	m_RoomList.InsertColumn(0,"Name",LVCFMT_LEFT,aRect.Width()-100);
	m_RoomList.InsertColumn(1,"Flags",LVCFMT_LEFT,50);
	m_RoomList.InsertColumn(2,"Users",LVCFMT_LEFT,50);

	RoutingGroupMap::iterator anItr = mGroupMap.begin();
	while(anItr!=mGroupMap.end())
	{
		RoutingGroupInfo *anInfo = anItr->second;
		bool isChat = anInfo->mFlags&RoutingGroupFlag_IsChatRoom?true:false;
		if(mIsChat==isChat)
		{
			LVITEM anItem;
			string aName = WStringToString(anInfo->mName);

			anItem.mask = LVIF_PARAM | LVIF_TEXT;
			anItem.lParam = (LPARAM)anInfo;
			anItem.iItem = 0;
			anItem.iSubItem = 0;
			anItem.pszText = (char*)aName.c_str();
			int anIndex = m_RoomList.InsertItem(&anItem);

			anItem.mask = LVIF_TEXT;
			anItem.iItem = anIndex;

			anItem.iSubItem = 1;
			string aFlagStr;
			if(anInfo->mFlags&RoutingGroupFlag_PasswordProtected)
				aFlagStr += "P";

			anItem.pszText = (char*)aFlagStr.c_str();

			m_RoomList.SetItem(&anItem);

			anItem.iSubItem = 2;
			char aBuf[50];
			sprintf(aBuf,"%d",anInfo->mMemberMap.size());
			anItem.pszText = aBuf;
			m_RoomList.SetItem(&anItem);
		}

		++anItr;
	}

	if(mIsChat)
	{
		SetWindowText("Chat rooms...");
		m_ObserveButton.ShowWindow(SW_HIDE);
	}
	else
	{
		SetWindowText("Game rooms...");
		m_ObserveButton.ShowWindow(SW_SHOW);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/*
void RoomListDialog::OnSelchangeList() 
{
	// TODO: Add your control notification handler code here
	int aSel = m_List.GetCurSel();
	if(aSel==LB_ERR)
		m_JoinButton.EnableWindow(FALSE);
	else
		m_JoinButton.EnableWindow(TRUE);
}

void RoomListDialog::OnOK() 
{	
	CDialog::OnOK();
}

void RoomListDialog::OnDblclkList() 
{
	// TODO: Add your control notification handler code here
	OnOK();	
}
*/

static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	RoutingGroupInfo *g1 = (RoutingGroupInfo*)lParam1;
	RoutingGroupInfo *g2 = (RoutingGroupInfo*)lParam2;

	switch(lParamSort)
	{
		case 0: return g1->mName.compare(g2->mName);
		case 1: return 0;
		case 2: return g2->mMemberMap.size() - g1->mMemberMap.size();
	}

	return 0;
}

void RoomListDialog::OnColumnclickRoomList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	m_RoomList.SortItems(CompareFunc,pNMListView->iSubItem);
	*pResult = 0;
}

void RoomListDialog::OnClickRoomList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_MemberList.ResetContent();
	mSelGroup = NULL;

	POSITION pos = m_RoomList.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		m_JoinButton.EnableWindow(FALSE);
		m_ObserveButton.EnableWindow(FALSE);
		return;
	}

	int aPos = m_RoomList.GetNextSelectedItem(pos);
	RoutingGroupInfo *aGroup = (RoutingGroupInfo*)m_RoomList.GetItemData(aPos);
	RoutingMemberMap::iterator anItr = aGroup->mMemberMap.begin();
	int aNumPlayers = 0;
	
	while(anItr!=aGroup->mMemberMap.end())
	{
		RoutingMemberInfo *aMember = anItr->second;
		string aName = WStringToString(aMember->mClientInfo->mName);

		if(!mIsChat)
		{
			if(aMember->mClientId==aGroup->mCaptainId) 
			{
				aName += "  (Captain)";
				aNumPlayers++;
			}
			else if(!(aMember->mFlags&RoutingMemberFlag_IsObserver))
			{
				aName += "  (Player)";
				aNumPlayers++;
			}
		}

		m_MemberList.AddString(aName.c_str());
		++anItr;
	}


	m_JoinButton.EnableWindow((aNumPlayers < aGroup->mMaxPlayers) || (aGroup->mMaxPlayers==0));
	m_ObserveButton.EnableWindow(aGroup->mFlags&RoutingGroupFlag_AllowObservers);
	
	mSelGroup = aGroup;
	*pResult = 0;
}

void RoomListDialog::OnJoinButton() 
{
	// TODO: Add your control notification handler code here
	EndDialog(RoomListResult_Join);
}

void RoomListDialog::OnObserveButton() 
{
	// TODO: Add your control notification handler code here
	EndDialog(RoomListResult_Observe);
}
