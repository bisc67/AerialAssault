// SetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Set.h"
#include "SetDlg.h"
#include "SetBoard.h"
#include "SetCard.h"
#include <vector>
#include <algorithm>
#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;
using namespace WONAPI;

SetDlg* SetDlg::mInstance = NULL;


/////////////////////////////////////////////////////////////////////////////
// SetDlg dialog
SetDlg::SetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(SetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(SetDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	mImageList = NULL;
	mPumpThread = NULL;
	mLoggedIn = false;
	mRegistered = false;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
SetDlg::~SetDlg()
{
	delete mImageList;
	delete mPumpThread;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void SetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SetDlg)
	DDX_Control(pDX, IDC_PLAYER_LIST, m_UserList);
	DDX_Control(pDX, IDC_SEND_BUTTON, m_SendButton);
	DDX_Control(pDX, IDC_CHAT_OUTPUT, m_ChatOutput);
	DDX_Control(pDX, IDC_TIME_LABEL, m_TimeLabel);
	DDX_Control(pDX, IDC_ACTIVE_PLAYER, m_ActivePlayerLabel);
	DDX_Control(pDX, IDC_CALL_SET, m_CallSetButton);
	DDX_Control(pDX, IDC_CHAT_INPUT, m_ChatInput);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(SetDlg, CDialog)
	//{{AFX_MSG_MAP(SetDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_CALL_SET, OnCallSet)
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	ON_COMMAND(ID_LOGIN_EXIT, OnLoginExit)
	ON_COMMAND(ID_LOGIN_LOGIN, OnLoginLogin)
	ON_COMMAND(ID_SERVER_VIEWLIST, OnServerViewlist)
	ON_COMMAND(ID_SERVER_JOINBYIP, OnServerJoinbyip)
	ON_COMMAND(ID_GROUP_CREATE, OnGroupCreate)
	ON_COMMAND(ID_GROUP_VIEWLIST, OnGroupViewlist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SetDlg message handlers

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static CString UserListText(void *theMember, int theIndex, void *theParam)
{
	if(theIndex==0)
		return ChatManager::UserListText(theMember, theIndex, theParam);

	SetMemberInfo *aMember = (SetMemberInfo*)theMember;
	char aBuf[50];
	return itoa(aMember->mScore, aBuf, 10);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static int UserListSort(void *theM1, void *theM2, int theIndex, void *theParam)
{
	if(theIndex==0)
		return ChatManager::UserListSort(theM1,theM2,theIndex,theParam);

	SetMemberInfo *m1 = (SetMemberInfo*)theM1;
	SetMemberInfo *m2 = (SetMemberInfo*)theM2;
	return m1->mScore - m2->mScore;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
BOOL SetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	mInstance = this;
	mPumpThread = new PumpThread(m_hWnd);
	mPumpThread->Start();

	mDirServers = new ServerContext;
	mDirServers->AddAddress("wontest.west.won.net:15101");
	mDirServers->AddAddress("wontest.central.won.net:15101");
	mDirServers->AddAddress("wontest.east.won.net:15101");

	mAuthContext = new AuthContext;
	mAuthContext->SetCommunity(L"WON");
	mLoginDlg.Init(mAuthContext,mDirServers);
	mServerDlg.Init(mDirServers,L"/API2Sample");

	// TODO: Add extra initialization here
	HDC aDC = ::GetDC(m_hWnd);
	mBoard.SetRect(aDC,10,10, 380,260);
	::ReleaseDC(m_hWnd,aDC);


	DWORD style = m_UserList.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
	style |= LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP;
	m_UserList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, style);

	static const char *columnName[] = { "Player", "Score" };
	for(int i=0; i<2; i++)
		m_UserList.InsertColumn(i,columnName[i],LVCFMT_LEFT, 0);

	m_ChatOutput.SetBackgroundColor(FALSE,RGB(0,0,0));

	// Load images for the user list
	mImageList = new CImageList();
	SetApp *pApp = (SetApp *)AfxGetApp();
	mImageList->Create(16, 16, ILC_MASK, 0, 8);
	mImageList->Add(pApp->LoadIcon(IDI_NORMAL));
	mImageList->Add(pApp->LoadIcon(IDI_AWAY));
	mImageList->Add(pApp->LoadIcon(IDI_IGNORED));
	mImageList->Add(pApp->LoadIcon(IDI_MODERATOR));
	mImageList->Add(pApp->LoadIcon(IDI_MUTED));

	// Initialize the user display list
	m_UserList.SetBkColor(RGB(0,0,0));
	m_UserList.SetTextColor(RGB(0,255,0));
	m_UserList.SetTextBkColor(RGB(0,0,0));
	m_UserList.SetImageList(mImageList, LVSIL_SMALL);
	m_UserList.SetImageFunc(ChatManager::UserListImage);
	m_UserList.SetTextFunc(UserListText);
	m_UserList.SetSortFunc(UserListSort);

	mChatManager.SetControls(m_hWnd,&m_ChatInput, &m_ChatOutput, &m_UserList, &m_SendButton);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void SetDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this); // device context for painting
		mBoard.Draw(dc);
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR SetDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void SetDlg::InitRoutingConnection()
{
	KillRoutingConnection();
	mRoutingConnection = new SetRoutingConnection;
	mRoutingConnection->SetAuth(mAuthContext);
	mRoutingConnection->SetConnectCompletion(new DlgCompletion(ConnectCompletion));
	mRoutingConnection->SetCloseCompletion(new DlgCompletion(CloseCompletion));
	
	mRoutingConnection->SetDefaultRoutingCompletion(new RoutingCompletion(DefaultRoutingCompletion));
	mRoutingConnection->SetRoutingCompletion(RoutingOp_GroupCaptainChanged, new RoutingCompletion(CaptainChangedCompletion));
	mRoutingConnection->SetRoutingCompletion(RoutingOp_DerivedServerOp, new RoutingCompletion(DerivedServerCompletion)); 

	mRoutingConnection->AddCommandString(L"Invite",SmartRoutingConnection::InputCommand_Invite);
	mRoutingConnection->AddCommandString(L"Uninvite",SmartRoutingConnection::InputCommand_Uninvite);
	mRoutingConnection->SetServerWideCommands(true);

	SetWindowText("Connecting to routing server...");
	mRoutingConnection->ConnectAsync(mServerAddr);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
bool SetDlg::CheckError(const char *theStr, AsyncOp *theOp, bool killConnectionOnFailure)
{
	if(theOp->Succeeded())
		return false;

	if(killConnectionOnFailure)
		KillRoutingConnection();

	char aMsg[1024];
	sprintf(aMsg,"%s: %s",theStr,WONStatusToString(theOp->GetStatus()));
	MessageBox(aMsg,"Error.");
	return true;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void SetDlg::CloseCompletion(WONAPI::AsyncOp *theOp)
{
	SocketOp *anOp = (SocketOp*)theOp;
	if(anOp->GetSocket()!=mRoutingConnection)
		return;

	CheckError("Routing connection closed",anOp);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void SetDlg::ConnectCompletion(AsyncOp *theOp)
{
	PeerAuthOp *anOp = (PeerAuthOp*)theOp;
	if(anOp->GetSocket()!=mRoutingConnection) // not the current connection
		return;

	if(CheckError("Failed to connect to routing server",anOp))
		return;

	SetWindowText("Registering with server.");

	SetApp *pApp = (SetApp*)AfxGetApp();
	const char *aCommandLine = pApp->m_lpCmdLine;
	char aName[256];
	sscanf(aCommandLine,"%s",aName);

	mRoutingConnection->RegisterClient(StringToWString(aName),L"",0,0,
		RoutingRegisterClientFlag_LoginTypeCertificate | 
		RoutingRegisterClientFlag_GetClientList,
		0,
		new RoutingCompletion(RegisterCompletion));

//	mRoutingConnection->GetGroupList(RoutingGetGroupList_IncludeAll,new RoutingCompletion(GetGroupListCompletion));
//	mRoutingConnection->JoinGroup(0,L"",L"",0,0,new RoutingCompletion(JoinGroupCompletion));
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void SetDlg::RegisterCompletion(RoutingOp *theOp)
{
	RoutingRegisterClientOp *anOp = (RoutingRegisterClientOp*)theOp;
	if(CheckError("Failed to register with routing server",anOp))
		return;

	mRegistered = true;
	SetWindowText(mServerName);

	mJoinGroupDlg.Init(mRoutingConnection,RoutingGetGroupList_IncludeAll);
	mJoinGroupDlg.SetPreJoinCompletion(new RoutingCompletion(PreJoinGroupCompletion));
	mJoinGroupDlg.SetJoinCompletion(new RoutingCompletion(JoinGroupCompletion));

	mCreateGroupDlg.Init(mRoutingConnection);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void SetDlg::GetGroupListCompletion(WONAPI::RoutingOp *theOp)
{
//	RoutingRegisterClientOp *anOp = (RoutingRegisterClientOp*)theOp;
//	CheckError("Failed to get group list",anOp);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void SetDlg::PreJoinGroupCompletion(WONAPI::RoutingOp *theOp)
{
	LeaveGroup();
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void SetDlg::CreateGroupCompletion(WONAPI::RoutingOp *theOp)
{
	RoutingCreateGroupOp *anOp = (RoutingCreateGroupOp*)theOp;
	if(CheckError("Failed to create group",anOp,false))
		return;	

	HandleEnterGroup(anOp->GetGroupId());
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void SetDlg::JoinGroupCompletion(WONAPI::RoutingOp *theOp)
{	
	RoutingJoinGroupOp *anOp = (RoutingJoinGroupOp*)theOp;
	if(CheckError("Failed to join group",anOp,false))
		return;

	HandleEnterGroup(anOp->GetGroupId());
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void SetDlg::HandleEnterGroup(int theGroupId)
{
	mChatManager.Init(mRoutingConnection,theGroupId);
	mGroupId = theGroupId;
	mGameState = -1;

	mGroup = mRoutingConnection->GetGroupRef(mGroupId);

	CString aTitle;
	aTitle.Format("%s - %s",(const char*)mServerName,WStringToString(mGroup->mName).c_str());
	SetWindowText(aTitle);

	m_CallSetButton.EnableWindow(FALSE);

}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void SetDlg::CaptainChangedCompletion(WONAPI::RoutingOp *theOp)
{
	RoutingGroupCaptainChangedOp *anOp = (RoutingGroupCaptainChangedOp*)theOp;
	if(anOp->GetGroupId()!=mGroupId)
		return;

/*	RoutingMemberMap::iterator anItr = mGroup->mMemberMap.find(anOp->GetNewCaptainId());
	if(anItr!=mGroup->mMemberMap.end())
		m_UserList.UpdateItem(anItr->second);*/

	CheckCaptain();
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void SetDlg::HandleRoutingOp(WONAPI::RoutingOp *theOp)
{
	mChatManager.HandleRoutingOp(theOp);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void SetDlg::DefaultRoutingCompletion(WONAPI::RoutingOp *theOp)
{
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void SetDlg::CheckCaptain()
{
	if(mGameState==2) // Game not started
	{
		if(mGroup->mCaptainId==mRoutingConnection->GetMyClientId())
			m_CallSetButton.EnableWindow(TRUE);
		else
			m_CallSetButton.EnableWindow(FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void SetDlg::HandleGameState(ReadBuffer &theMsg)
{
	mBoard.Activate(false);
	mIControlBoard = false;
	m_ActivePlayerLabel.SetWindowText("");
		
	mGameState = theMsg.ReadByte();
	if(mGameState==0)
	{
		mBoard.Clear(true);
		mBoard.SetGrid(3,4);
		m_CallSetButton.SetWindowText("Call Set");
	}

	if(mGameState==0 || mGameState==1)
	{
		m_CallSetButton.EnableWindow(TRUE);
	}
	
	if(mGameState==2)
	{
		m_CallSetButton.SetWindowText("Start Game");
		CheckCaptain();
	}

	unsigned short aNumPlayers = theMsg.ReadShort();

	RoutingMemberMap &aMap = mGroup->mMemberMap;
	RoutingMemberMap::iterator anItr;

	int i;
	for(i=0; i<aNumPlayers; i++)
	{
		unsigned short anId = theMsg.ReadShort();
		short aScore = theMsg.ReadShort();
		anItr = aMap.find(anId);
		if(anItr!=aMap.end())
		{
			SetMemberInfo *aMember = (SetMemberInfo*)anItr->second.get();
			aMember->mScore = aScore;
			m_UserList.UpdateItem(aMember,false);
		}
	}
	m_UserList.Sort();

	mBoard.ResetSelection(mWasSet);

	unsigned char aNumCards = theMsg.ReadByte();
	for(i=0; i<aNumCards; i++)
	{
		unsigned char anId = theMsg.ReadByte();
		unsigned char aCard = theMsg.ReadByte();

		unsigned char attrib[4];
		for(int j=0; j<4; j++)
		{
			attrib[j] = (aCard&0x03) + 1;
			aCard>>=2;
		}

		mBoard.AddCard(anId,new SetCard(attrib[3],attrib[2],attrib[1],attrib[0]));
	}

	int aNumRows = mBoard.GetNumRows();
	int aNumCols = mBoard.GetNumCols();
	int aTotalCards = mBoard.GetNumCards();

	aNumCols = ceil((double)aTotalCards/aNumRows);
	if(aNumCols<4)
		aNumCols = 4;

	mBoard.SetGrid(aNumRows,aNumCols);

	InvalidateRect(mBoard,FALSE);

	mRoundNumber = theMsg.ReadShort();
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void SetDlg::HandlePlayerControlsBoard(ReadBuffer &theMsg)
{
	unsigned short anId = theMsg.ReadShort();

	RoutingClientInfoPtr aClient = mRoutingConnection->GetClientRef(anId);
	if(aClient.get()!=NULL)
		m_ActivePlayerLabel.SetWindowText(WStringToString(aClient->mName).c_str());

	m_CallSetButton.EnableWindow(false);
	mWasSet = false;
	if(anId==mRoutingConnection->GetMyClientId())
	{
		mBoard.Activate(true);
		mIControlBoard = true;
	}
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void SetDlg::HandlePlayerTogglesCard(ReadBuffer &theMsg)
{
	if(mIControlBoard)
		return;

	int anId = theMsg.ReadByte();
	if(mBoard.ToggleCardState(anId))
	{
		mWasSet = mBoard.CheckSelection()==1;
		InvalidateRect(mBoard,FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void SetDlg::DerivedServerCompletion(WONAPI::RoutingOp *theOp)
{
	RoutingDerivedServerOp *anOp = (RoutingDerivedServerOp*)theOp;
	int aMsgType = anOp->GetMsgType();
	ReadBuffer aMsg(anOp->GetRecvMsg()->data(),anOp->GetRecvMsg()->length());
	switch(aMsgType)
	{
		case 4: HandleGameState(aMsg); break;
		case 5: HandlePlayerControlsBoard(aMsg); break;
		case 6: HandlePlayerTogglesCard(aMsg); break;
	}

}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void SetDlg::KillRoutingConnection()
{
	SetWindowText("Set");

	mRegistered = false;
	if(mRoutingConnection.get()!=NULL)
	{
		mRoutingConnection->Kill();
		mRoutingConnection = NULL;
		LeaveGroup();
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetDlg::LeaveGroup()
{
	if(!mChatManager.LeaveGroup())
		return;

	mBoard.Clear(true);
	InvalidateRect(mBoard,FALSE);

	mGroupId = -1;
	mGameState = -1;
	mGroup = NULL;
	m_CallSetButton.EnableWindow(FALSE);

	SetWindowText(mServerName);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetDlg::CheckClick(CPoint &point)
{
	CRect anInvalidRect;
	int anId = mBoard.ButtonDown(point);
	if(anId!=-1)
	{
		WriteBuffer aMsg(2);
		aMsg.AppendByte(3);
		aMsg.AppendByte(8);
		aMsg.AppendByte(3); 
		aMsg.AppendByte(anId);
		mRoutingConnection->SendMsg(aMsg.ToByteBuffer());

		int aResult = mBoard.CheckSelection();
		if(aResult!=0)
		{
			mWasSet = aResult==1;
			mBoard.Activate(FALSE);
		}

		InvalidateRect(mBoard,FALSE);
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CheckClick(point);
	CDialog::OnLButtonDown(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CheckClick(point);
	CDialog::OnLButtonDblClk(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetDlg::OnCallSet() 
{
	// TODO: Add your control notification handler code here
	m_CallSetButton.EnableWindow(FALSE);
//	mBoard.Activate(true);
	WriteBuffer aMsg(2);
	aMsg.AppendByte(3);
	aMsg.AppendByte(8);

	if(mGameState==2) // Send Start Game
	{
		aMsg.AppendByte(1); 
	}
	else
	{
		aMsg.AppendByte(2);
		aMsg.AppendShort(mRoundNumber);
	}
		
	mRoutingConnection->SendMsg(aMsg.ToByteBuffer());
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
BOOL SetDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
/*	CPoint aPoint;
	GetCursorPos(&aPoint);
	ScreenToClient(&aPoint);

	if(mBoard.MouseOver(aPoint))
	{
		SetApp *pApp = (SetApp*)AfxGetApp();
		SetCursor(pApp->LoadCursor(IDC_HAND_CURSOR));
		return TRUE;
	}*/

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnTimer(nIDEvent);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
LRESULT SetDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(message)
	{
		case MSG_PUMP_API:
			if(mPumpThread!=NULL)
				mPumpThread->WindowProcNotify();
			return 0;

		case MSG_SIGNAL_PUMP_THREAD:
			if(mPumpThread!=NULL)
				mPumpThread->Signal();
			return 0;
	}
	
	mChatManager.WindowProc(message, wParam, lParam);
	return CDialog::WindowProc(message, wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
BOOL SetDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	KillRoutingConnection();
	delete mPumpThread;
	mPumpThread = NULL;
	mInstance = NULL;	
	return CDialog::DestroyWindow();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetDlg::OnLoginLogin() 
{
	// TODO: Add your command handler code here
	mLoggedIn = mLoginDlg.DoModal()==IDOK;
	if(mLoggedIn && mServerDlg.GetAuthContext()==NULL)
	{
		mServerDlg.SetAuthContext(mAuthContext);
		mServerDlg.GetDir();
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetDlg::OnLoginExit() 
{
	// TODO: Add your command handler code here
	EndDialog(IDOK);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool SetDlg::CheckLoggedIn()
{
	if(mLoggedIn)
		return true;

	OnLoginLogin();
	return mLoggedIn;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetDlg::OnServerViewlist() 
{

	// TODO: Add your command handler code here
	if(!CheckLoggedIn())
		return;

	if(mServerDlg.DoModal()!=IDOK)
		return;

	const DirEntity *anEntity = mServerDlg.GetDirEntity();
	mServerAddr = anEntity->GetNetAddrAsIP();
	mServerName = WStringToString(anEntity->mDisplayName).c_str();
	InitRoutingConnection();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetDlg::OnServerJoinbyip() 
{
	// TODO: Add your command handler code here
	if(!CheckLoggedIn())
		return;

	if(mJoinByIPDlg.DoModal()!=IDOK)
		return;

	mServerAddr = mJoinByIPDlg.GetAddr();
	mServerName = mJoinByIPDlg.m_IPAddrStr;
	InitRoutingConnection();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetDlg::OnGroupCreate() 
{
	// TODO: Add your command handler code here
	if(mCreateGroupDlg.DoModal()==IDCANCEL)
		return;

	LeaveGroup();

	RoutingCreateGroupOpPtr anOp = mCreateGroupDlg.GetCreateGroupOp();
	anOp->SetCompletion(new RoutingCompletion(CreateGroupCompletion));
	anOp->Run();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SetDlg::OnGroupViewlist() 
{
	// TODO: Add your command handler code here
	int aResult = mJoinGroupDlg.DoModal();
	if(aResult!=IDOK)
		return;
}
