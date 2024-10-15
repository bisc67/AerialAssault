// ChatSampleDlg.cpp : implementation file
//
#pragma warning (disable : 4786)
#include "stdafx.h"
#include "ChatSample.h"
#include "ChatSampleDlg.h"
#include "LoginDlg.h"
#include "MOTDDialog.h"
#include "CancelDialog.h"
#include "ChatServerDialog.h"
#include "GameBoard.h"
#include "PasswordDialog.h"
#include "RoomListDialog.h"
#include "ProfileDialog.h"
#include "CreateChatDialog.h"
#include "JoinRequestDialog.h"

#include "WONMisc/GetMOTDOp.h"
#include "WONMisc/GetVersionOp.h"
#include "WONAuth/GetCertOp.h"
#include "WONDir/GetDirOp.h"
#include "WONRouting/AllRoutingOps.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace WONAPI;
using namespace std;
CChatSampleDlg* CChatSampleDlg::mInstance = NULL;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
static COLORREF COLOR_USER		= RGB(  0,255,  0);
static COLORREF COLOR_BROADCAST = RGB(255,255,255);
static COLORREF COLOR_WHISPER	= RGB(255,255,  0);
static COLORREF COLOR_EMOTE		= RGB(255,  0,255);
static COLORREF COLOR_MESSAGE	= RGB(255,255,255);
static COLORREF COLOR_ERROR		= RGB(255,  0,  0);
static COLORREF COLOR_SYSTEM	= RGB(255,255,255);
static COLORREF COLOR_MOTD		= RGB(255,255,  0);
static COLORREF COLOR_INVITE	= RGB(  0,255,255);
const wchar_t* SERVERNAME_AUTH		= L"AuthServer";
const wchar_t* SERVERNAME_PROFILE   = L"TitanProfileServer";
/////////////////////////////////////////////////////////////////////////////
// CChatSampleDlg dialog

CChatSampleDlg::CChatSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChatSampleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChatSampleDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	mInstance = this;
	mAPI = NULL;
	mPumpThread = NULL;

	srand(time(NULL));
	rand();
	
	mFilterDirtyWords = true;

	mAuthCancelDialog = NULL;
	mJoinChatServerCancelDialog = NULL;
	mJoinGameCancelDialog = NULL;
	mGameBoard = NULL;
	mImageList = NULL;

	mCurChatOutput = NULL;
	mAdBitmap = NULL;
	mNewAdImage = false;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
CChatSampleDlg::~CChatSampleDlg()
{
	delete mAuthCancelDialog;
	delete mJoinChatServerCancelDialog;
	delete mJoinGameCancelDialog;
	delete mGameBoard;
	delete mImageList;
	delete mPumpThread; 
	delete mAPI; 
}

void CChatSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChatSampleDlg)
	DDX_Control(pDX, IDC_AD_RECT, m_AdRect);
	DDX_Control(pDX, IDC_USERLIST, m_UserList);
	DDX_Control(pDX, IDC_SENDBUTTON, m_SendButton);
	DDX_Control(pDX, IDC_CHATINPUT, m_ChatInput);
	DDX_Control(pDX, IDC_CHATOUTPUT, m_ChatOutput);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CChatSampleDlg, CDialog)
	//{{AFX_MSG_MAP(CChatSampleDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SENDBUTTON, OnSendbutton)
	ON_COMMAND(ID_LOGIN_LOGININFO, OnLoginLogininfo)
	ON_COMMAND(ID_VIEW_CHATSERVERS, OnViewChatservers)
	ON_COMMAND(ID_VIEW_GAMEBOARD, OnViewGameboard)
	ON_COMMAND(ID_CHAT_FILTERDIRTYWORDS, OnChatFilterdirtywords)
	ON_COMMAND(ID_VIEW_GAMEROOMS, OnViewGamerooms)
	ON_COMMAND(ID_VIEW_CHATROOMS, OnViewChatrooms)
	ON_COMMAND(ID_LOGIN_DISCONNECT, OnLoginDisconnect)
	ON_COMMAND(ID_LOGIN_EXIT, OnLoginExit)
	ON_COMMAND(ID_OPTIONS_PROFILE, OnProfile)
	ON_COMMAND(ID_CREATE_CHATROOM, OnCreateChatroom)
	ON_COMMAND(ID_CREATE_GAME, OnCreateGame)
	ON_NOTIFY(NM_CLICK, IDC_USERLIST, OnClickUserlist)
	ON_NOTIFY(NM_RCLICK, IDC_USERLIST, OnRclickUserlist)
	ON_COMMAND(ID_PLAYER_IGNORE, OnPlayerIgnore)
	ON_COMMAND(ID_PLAYER_WHISPER, OnPlayerWhisper)
	ON_COMMAND(ID_PLAYER_INVITE, OnPlayerInvite)
	ON_COMMAND(ID_PLAYER_UNINVITE, OnPlayerUninvite)
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatSampleDlg message handlers

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
enum UserlistImages
{
	UserListImage_Normal = 0,
	UserListImage_Away = 1,
	UserListImage_Ignored = 2,
	UserListImage_Moderator = 3,
	UserListImage_Muted = 4
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
BOOL CChatSampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Initialize the WONAPICore and pump thread
	mAPI = new WONAPICore(false);
	mPumpThread = new PumpThread(m_hWnd,mAPI);
	mPumpThread->Start();

	// Initialize the game board
	mGameBoard = new GameBoard;
	mGameBoard->Create(GameBoard::IDD,NULL);
	mCurChatOutput = &m_ChatOutput;

	// Make sure the ad rect is the correct width and height
	CRect aClientRect;
	GetClientRect(&aClientRect);
	CRect anAdRect = GetRect(m_AdRect);
	m_AdRect.MoveWindow((aClientRect.Width() - 468)/2,anAdRect.top,468,60,FALSE);


	// Initialize the chat box
	m_ChatOutput.SetBackgroundColor(FALSE,RGB(0,0,0));
	CHARFORMAT aFormat;
	aFormat.cbSize = sizeof(aFormat);
	aFormat.dwMask = CFM_COLOR;
	aFormat.dwEffects = 0;
	aFormat.crTextColor = RGB(0,255,0);
	m_ChatOutput.SetDefaultCharFormat(aFormat);
	mCharFormat = aFormat;

	// Load images for the user list
	mImageList = new CImageList();
	CChatSampleApp *pApp = (CChatSampleApp *)AfxGetApp();
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// Get the message of the day
	AppendChat("Retrieving message of the day...\r\n",COLOR_SYSTEM);
	GetMOTDOpPtr anMOTDOp = new GetMOTDOp("API2Sample");
	anMOTDOp->SetCompletion(new DlgCompletion(MOTDCompletion));
	anMOTDOp->RunAsync(OP_TIMEOUT_INFINITE);

	// Initialize the list of directory servers
	mDirServerContext = new ServerContext;
	mDirServerContext->AddAddress("wontest.west.won.net:15101");
	mDirServerContext->AddAddress("wontest.central.won.net:15101");
	mDirServerContext->AddAddress("wontest.east.won.net:15101");

	// Querey the dir servers for the auth servers
	AppendChat("Retrieving AuthServer directory...\r\n",COLOR_SYSTEM);
	mAuthContext = new AuthContext;						// Stores the auth servers
	mProfileContext = new ServerContext;				// Stores the profile servers
	GetDirOpPtr aGetDir = new GetDirOp(mDirServerContext);
	aGetDir->SetPath(L"/TitanServers");
	aGetDir->SetFlags(DIR_GF_DECOMPRECURSIVE | DIR_GF_DECOMPSERVICES | DIR_GF_SERVADDNETADDR | DIR_GF_SERVADDNAME);
	aGetDir->SetCompletion(new DlgCompletion(GetDirCompletion));
	aGetDir->RunAsync(OP_TIMEOUT_INFINITE);

	// Initilize add API
	AdApi::AdResult result = mAdAPI.Init("TestProduct", NULL);

	if (result == AdApi::AdResult_Success)
	{
		result = mAdAPI.SetPaintCallback(StaticAdPaintCallback, this);
		if (result != AdApi::AdResult_Success)
			MessageBox("Error Initilizing Ad display", "Error", MB_ICONSTOP);
	}
	else
		MessageBox("Error Initilizing Ad display", "Error", MB_ICONSTOP);

	// Check for updates
	AppendChat("Checking for updates...\r\n",COLOR_SYSTEM);
	GetVersionOpPtr aGetVersionOp = new GetVersionOp(mDirServerContext,"API2Sample");
	aGetVersionOp->SetCompletion(new DlgCompletion(GetVersionCompletion));
	aGetVersionOp->RunAsync(OP_TIMEOUT_INFINITE);

	// Set the focus to the chat input box	
	m_ChatInput.SetFocus();
	return FALSE;  // return TRUE  unless you set the focus to a control
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if (IsIconic())
	{
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
		AutoCrit aCrit(mAdCrit);
		HDC hWindowDC = dc.m_hDC;

		if(mNewAdImage)
		{
			if(mAdBitmap!=NULL)
			{
				mAdAPI.ReleaseDisplayImage(mAdBitmap);
				mAdBitmap = NULL;
			}

			AdApi::AdResult result = mAdAPI.GetDisplayImage(hWindowDC, mAdBitmap);	// Get Bitmap
			if(result!=AdApi::AdResult_Success)
				mAdBitmap = NULL;

			mNewAdImage = false;
		}
		aCrit.Leave();

		// create a memory device context
		if(mAdBitmap!=NULL)
		{
			HDC hDC = CreateCompatibleDC(hWindowDC);

			RECT nDlgClientRect;
			GetClientRect(&nDlgClientRect);

			HBITMAP hOldBmp = (HBITMAP)SelectObject(hDC, mAdBitmap);		// select Bitmap into memory DC
			CRect aRect = GetRect(m_AdRect);
			BitBlt(dc, aRect.left, aRect.top, aRect.Width(), aRect.Height(), hDC, 0, 0, SRCCOPY);			// blt the Bitmap onto the screen
			SelectObject(hDC, hOldBmp);
				
			// delete the memory device context
			DeleteDC(hDC);
		}

		CDialog::OnPaint();
	}
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
CRect CChatSampleDlg::GetRect(const CWnd &theWnd)
{
	CRect aRect;
	theWnd.GetWindowRect(&aRect);
	ScreenToClient(&aRect);
	return aRect;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::AdPaintCallback()
{
	// Set Tool tip here
	RECT theDlgClientRect;
	GetClientRect(&theDlgClientRect);
	
	AutoCrit aCrit(mAdCrit);
	mNewAdImage = true;
	aCrit.Leave();

	CRect aRect = GetRect(m_AdRect);
	InvalidateRect(&aRect, FALSE);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::StaticAdPaintCallback(void *data)
{
	CChatSampleDlg *dlg = reinterpret_cast<CChatSampleDlg*>(data);
	dlg->AdPaintCallback();
}


// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CChatSampleDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::KillRoutingConnection()
{
	if(mRoutingConnection.get()!=NULL)
	{
		mRoutingConnection->Kill();
		mRoutingConnection = NULL;

		LeaveChatGroup();
		LeaveGameGroup();

		CMenu *aMenu = GetMenu();
		aMenu->EnableMenuItem(ID_VIEW_GAMEROOMS,MF_GRAYED);
		aMenu->EnableMenuItem(ID_VIEW_CHATROOMS,MF_GRAYED);
		aMenu->EnableMenuItem(ID_LOGIN_DISCONNECT,MF_GRAYED);
		aMenu->EnableMenuItem(ID_CREATE_CHATROOM,MF_GRAYED);
		aMenu->EnableMenuItem(ID_CREATE_GAME,MF_GRAYED);
	}

	SetWindowText("ChatSample");
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
static string GetEditFormat(const char *theStr)
{
	string aStr;
	const char *aPtr = theStr;
	while(*aPtr!='\0')
	{
		char aChar = *aPtr;
		if(aChar=='\n')
			aStr+="\r\n";
		else
			aStr+=aChar;

		++aPtr;
	}

	return aStr;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::MOTDCompletion(WONAPI::AsyncOp *theOp)
{
	AppendChat("Done retrieving message of the day.\r\n",COLOR_SYSTEM);

	GetMOTDOp *anOp = (GetMOTDOp*)theOp;
	if(!anOp->Succeeded())
	{
		char aBuf[1024];
		sprintf(aBuf,"Failed to get message of the day: %s",WONStatusToString(anOp->GetStatus()));
		MessageBox(aBuf,"Failed to get MOTD");
		return;
	}

	MOTDDialog aDlg;
	if(anOp->SysMOTDIsNew())
	{
		ByteBufferPtr aBuf = anOp->GetSysMOTD();
		aDlg.m_MOTDText += "System Message:\r\n";
		aDlg.m_MOTDText += GetEditFormat(aBuf->data()).c_str();
		aDlg.m_MOTDText += "\r\n";
	}

	if(anOp->GameMOTDIsNew())
	{
		ByteBufferPtr aBuf = anOp->GetGameMOTD();
		aDlg.m_MOTDText += "Game Message:\r\n";
		aDlg.m_MOTDText += GetEditFormat(aBuf->data()).c_str();
		aDlg.m_MOTDText += "\r\n";
	}

	if(aDlg.m_MOTDText.GetLength()>0)
	{
		AppendChat("\r\nMessage of the day:\r\n",COLOR_MOTD);
		AppendChat((const char*)aDlg.m_MOTDText,COLOR_MOTD);
		AppendChat("\r\n");
	}

//	aDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::GetDirCompletion(WONAPI::AsyncOp *theOp)
{
	AppendChat("Done retrieving auth directory.\r\n",COLOR_SYSTEM);

	GetDirOp *anOp = (GetDirOp*)theOp;
	if(!anOp->Succeeded())
	{
		char aMsg[1024];
		sprintf(aMsg,"Failed to get titan server directory: %s",WONStatusToString(anOp->GetStatus()));
		MessageBox(aMsg,"Get Titan Dir Failed.");
		return;
	}

	mAuthContext->AddAddressesFromDir(anOp->GetDirEntityList(), SERVERNAME_AUTH);
	mProfileContext->AddAddressesFromDir(anOp->GetDirEntityList(), SERVERNAME_PROFILE);

	CMenu *aMenu = GetMenu();
	aMenu->EnableMenuItem(ID_LOGIN_LOGININFO,MF_ENABLED);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::GetChatServerListCompletion(WONAPI::AsyncOp *theOp)
{
	AppendChat("Done retrieving chat server list.\r\n",COLOR_SYSTEM);

	GetDirOp *anOp = (GetDirOp*)theOp;
	if(!anOp->Succeeded())
	{
		char aMsg[1024];
		sprintf(aMsg,"Failed to get chat server directory: %s",WONStatusToString(anOp->GetStatus()));
		MessageBox(aMsg,"Get Chat Dir Failed.");
		return;
	}

	mChatServerList = anOp->GetDirEntityList();
	CMenu *aMenu = GetMenu();
	aMenu->EnableMenuItem(ID_VIEW_CHATSERVERS,MF_ENABLED);
}	

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::GetChatServerList()
{
	GetDirOpPtr anOp = new GetDirOp(mDirServerContext);
	anOp->SetAuthType(AUTH_TYPE_SESSION,mAuthContext);
	anOp->SetPath(L"/API2Sample");
	anOp->SetFlags(DIR_GF_DECOMPSERVICES | DIR_GF_ADDDISPLAYNAME | DIR_GF_SERVADDNETADDR);
	anOp->SetCompletion(new DlgCompletion(GetChatServerListCompletion));
	anOp->RunAsync(OP_TIMEOUT_INFINITE);

	AppendChat("Getting chat server list...\r\n",COLOR_SYSTEM);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::AuthLoginCompletion(WONAPI::AsyncOp *theOp)
{
	GetCertOp *anOp = (GetCertOp*)theOp;

	if(anOp!=mGetCertOp) // not waiting on this op anymore
		return;
	
	AppendChat("Finished AuthServer login.\r\n");
	mAuthCancelDialog->EndDialog(IDOK);

	if(!anOp->Succeeded())
	{
		char aMsg[1024];
		sprintf(aMsg,"Failed to get certificate: %s",WONStatusToString(anOp->GetStatus()));
		MessageBox(aMsg,"Auth Failure");
		return;
	}
	
	GetMenu()->EnableMenuItem(ID_OPTIONS_PROFILE,MF_ENABLED);
	GetChatServerList();
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::CloseCompletion(WONAPI::AsyncOp *theOp)
{
	SocketOp *anOp = (SocketOp*)theOp;
	if(anOp->GetSocket()!=mRoutingConnection)
		return;

	char aMsg[1024];
	sprintf(aMsg,"Routing connection closed: %s",WONStatusToString(anOp->GetStatus()));
	MessageBox(aMsg,"Connection closed.");
	KillRoutingConnection();
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::ConnectCompletion(AsyncOp *theOp)
{
	PeerAuthOp *anOp = (PeerAuthOp*)theOp;
	if(anOp->GetSocket()!=mRoutingConnection) // not the current connection
		return;

	AppendChat("Done connecting to chat server.\r\n",COLOR_SYSTEM);
	mJoinChatServerCancelDialog->EndDialog(IDOK);

	CMenu *aMenu = GetMenu();
	aMenu->EnableMenuItem(ID_LOGIN_DISCONNECT,MF_ENABLED);

	if(!anOp->Succeeded())
	{
		KillRoutingConnection();

		char aMsg[1024];
		sprintf(aMsg,"Failed to connect to routing server: %s",WONStatusToString(anOp->GetStatus()));
		MessageBox(aMsg,"Connect failed.");
		return;
	}

	AppendChat("Registering client on chat server...\r\n");
	mRoutingConnection->RegisterClient(L"",L"",0,0,
		RoutingRegisterClientFlag_LoginTypeCertificate | 
		RoutingRegisterClientFlag_GetClientList,
		mFilterDirtyWords?0:RoutingClientFlag_DirtyWordFilteringOff, 
		new RoutingCompletion(RegisterCompletion));

	mRoutingConnection->GetGroupList(RoutingGetGroupList_IncludeAll,new RoutingCompletion(GetGroupListCompletion));
	SetWindowText(mChatServerName);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::RegisterCompletion(RoutingOp *theOp)
{
	AppendChat("Done registering client on chat server...\r\n",COLOR_SYSTEM);

	RoutingRegisterClientOp *anOp = (RoutingRegisterClientOp*)theOp;

	if(!anOp->Succeeded())
	{
		KillRoutingConnection();

		char aMsg[1024];
		sprintf(aMsg,"Failed to register with routing server: %s",WONStatusToString(anOp->GetStatus()));
		MessageBox(aMsg,"Register failed.");
		return;
	}
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::GetGroupListCompletion(WONAPI::RoutingOp *theOp)
{
	RoutingRegisterClientOp *anOp = (RoutingRegisterClientOp*)theOp;

	CMenu *aMenu = GetMenu();
	aMenu->EnableMenuItem(ID_VIEW_GAMEROOMS,MF_ENABLED);
	aMenu->EnableMenuItem(ID_VIEW_CHATROOMS,MF_ENABLED);
	aMenu->EnableMenuItem(ID_CREATE_CHATROOM,MF_ENABLED);
	aMenu->EnableMenuItem(ID_CREATE_GAME,MF_ENABLED);

	JoinChatGroup(0);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
int CChatSampleDlg::IndexOfMember(unsigned short theId)
{
	for(int i=0; i<m_UserList.GetItemCount(); i++)
	{
		RoutingMemberInfo *aMember = (RoutingMemberInfo*)m_UserList.GetItemData(i);
		if(aMember->mClientId==theId)
			return i;
	}

	return -1;

}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
RoutingMemberInfoPtr CChatSampleDlg::GetSelectedMember()
{
	if(mRoutingConnection.get()==NULL)
		return NULL;

	POSITION pos = m_UserList.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return NULL;

	int aPos = m_UserList.GetNextSelectedItem(pos);
	return (RoutingMemberInfo*)m_UserList.GetItemData(aPos);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
int CChatSampleDlg::GetItemImage(WONAPI::RoutingMemberInfo *theMember)
{
	SmartRoutingClientInfo *theClient = (SmartRoutingClientInfo*)theMember->mClientInfo.get();

	bool muted = theMember->IsMuted();
	bool moderator = theMember->IsModerator();
	bool away = (theClient->mFlags&RoutingClientFlag_IsAway)?true:false;
	bool ignored = theClient->mIgnored;

	if(ignored) return UserListImage_Ignored;
	else if(muted) return UserListImage_Muted;
	else if(away) return 	UserListImage_Away;
	else if(moderator) return UserListImage_Moderator;
	else return UserListImage_Normal;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::AddMember(WONAPI::RoutingMemberInfo *theMember)
{
	
	// check if user is already in list
	if(IndexOfMember(theMember->mClientId)!=-1)
		return;

	string aName = WStringToString(theMember->mClientInfo->mName);
	
	LV_ITEM aItem;
	aItem.iItem = 0;
	aItem.iSubItem = 0;
	aItem.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;
	aItem.pszText = (char*)aName.c_str();
	aItem.lParam = (DWORD)theMember;
	aItem.iImage = GetItemImage(theMember);
	m_UserList.InsertItem(&aItem);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::UpdateMember(WONAPI::RoutingMemberInfo *theMember)
{
	if(theMember==NULL)
		return;

	int anIndex = IndexOfMember(theMember->mClientId);
	if(anIndex==-1)
		return;

	LV_ITEM aItem;
	aItem.iItem = anIndex;
	aItem.iSubItem = 0;
	aItem.mask = LVIF_IMAGE;
	aItem.iImage = GetItemImage(theMember);
	m_UserList.SetItem(&aItem);
}

	
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::RemoveMember(unsigned short theId)
{
	int anIndex = IndexOfMember(theId);
	if(anIndex>=0)
		m_UserList.DeleteItem(anIndex);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::HandleEnterGameGroup(WONAPI::RoutingGroupInfo *theGroup)
{
	mGameGroupId = theGroup->mId;

	CMenu *aMenu = GetMenu();
	aMenu->EnableMenuItem(ID_VIEW_GAMEBOARD,MF_ENABLED);
	mGameBoard->InitializeGroup(theGroup);
	mGameBoard->ShowWindow(SW_NORMAL);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::LeaveGameGroup()
{
	if(mRoutingConnection.get()!=NULL)
	{
		if(mPendingGameGroupId!=-1)
			mRoutingConnection->CancelJoinGroup(mPendingGameGroupId, true);

		if(mGameGroupId!=-1)
			mRoutingConnection->LeaveGroup(mGameGroupId);
	}

	mPendingGameGroupId = -1;
	mGameGroupId = -1;
	CMenu *aMenu = GetMenu();
	aMenu->EnableMenuItem(ID_VIEW_GAMEBOARD,MF_GRAYED);

	mGameBoard->ShowWindow(SW_HIDE);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::JoinGameGroup(unsigned short theGroupId, const char *thePassword)
{
	if(mRoutingConnection.get()==NULL)
		return;
	
	LeaveGameGroup();
	mPendingGameGroupId = theGroupId;

	wstring aPassword;
	if(thePassword!=NULL)
		aPassword = StringToWString(thePassword);

	mRoutingConnection->JoinGroup(theGroupId,aPassword,L"",
		0, mObserveGame?RoutingMemberFlag_IsObserver:0,
		new RoutingCompletion(JoinGameGroupCompletion));

	mJoinGameCancelDialog = new CancelDialog;
	mJoinGameCancelDialog->m_Caption = "Joining game...";
	mJoinGameCancelDialog->m_Message = ("Requesting to join " + WStringToString(mRoutingConnection->GetGroupRef(theGroupId)->mName) + ".").c_str();
	
	int aResult = mJoinGameCancelDialog->DoModal();
	if(aResult==IDCANCEL)
	{
		mRoutingConnection->CancelJoinGroup(theGroupId, true);
		mPendingGameGroupId = -1;
	}

	delete mJoinGameCancelDialog;
	mJoinGameCancelDialog = NULL;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::CreateGameGroupCompletion(WONAPI::RoutingOp *theOp)
{
	RoutingCreateGroupOp *anOp = (RoutingCreateGroupOp*)theOp;

	if(!anOp->Succeeded())
	{
		char aMsg[1024];
		sprintf(aMsg,"Create game room failed: %s",WONStatusToString(anOp->GetStatus()));
		MessageBox(aMsg,"Create Failed.");
		return;
	}

	RoutingGroupInfoPtr aGroup = mRoutingConnection->GetGroupRef(anOp->GetGroupId());
	HandleEnterGameGroup(aGroup);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::JoinGameGroupCompletion(WONAPI::RoutingOp *theOp)
{
	RoutingJoinGroupOp *anOp = (RoutingJoinGroupOp*)theOp;

	if(anOp->GetGroupId()!=mPendingGameGroupId)
	{
		if(anOp->Succeeded())
			mRoutingConnection->LeaveGroup(anOp->GetGroupId());

		return;
	}

	if(mJoinGameCancelDialog!=NULL)
		mJoinGameCancelDialog->EndDialog(IDOK);
	
	mPendingGameGroupId = -1;

	if(!anOp->Succeeded())
	{
		if(anOp->GetStatus()==WS_RoutingServG2_InvalidPassword)
		{
			PasswordDialog aDlg;
			aDlg.mCaption = "Game password required...";
			if(aDlg.DoModal()==IDOK)
				JoinGameGroup(anOp->GetGroupId(),aDlg.m_Password);

			return;
		}


		char aMsg[1024];
		string aStr;
		sprintf(aMsg,"Failed to join game group: %s",WONStatusToString(anOp->GetStatus()));
		aStr = aMsg;
		
		if(!anOp->GetJoinComment().empty())
		{
			aStr += "\r\n";
			aStr += "Comment: ";
			aStr += WStringToString(anOp->GetJoinComment());
		}

		MessageBox(aStr.c_str(),"Join Failed.");
		return;
	}

	RoutingGroupInfoPtr aGroup = mRoutingConnection->GetGroupRef(anOp->GetGroupId());
	HandleEnterGameGroup(aGroup);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::CaptainChangedCompletion(WONAPI::RoutingOp *theOp)
{
	RoutingGroupCaptainChangedOp *anOp = (RoutingGroupCaptainChangedOp*)theOp;

	if(anOp->GetGroupId()==mGameGroupId)
		mGameBoard->SetAmCaptain(anOp->GetNewCaptainId()==mRoutingConnection->GetMyClientId());
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::HandleEnterChatGroup(WONAPI::RoutingGroupInfo *theGroup)
{
	m_SendButton.EnableWindow(TRUE);
	m_UserList.DeleteAllItems();

	RoutingMemberMap::iterator anItr = theGroup->mMemberMap.begin();
	while(anItr!=theGroup->mMemberMap.end())
	{
		AddMember(anItr->second);
		++anItr;
	}

	mChatGroupId = theGroup->mId;
	CString aTitle = mChatServerName + " - ";
	aTitle += WStringToString(theGroup->mName).c_str();
	SetWindowText(aTitle);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::LeaveChatGroup()
{
	if(mRoutingConnection.get()!=NULL)
	{
		if(mPendingChatGroupId!=-1)
			mRoutingConnection->CancelJoinGroup(mPendingChatGroupId, true);

		if(mChatGroupId!=-1)
			mRoutingConnection->LeaveGroup(mChatGroupId);
	}

	mPendingChatGroupId = -1;
	mChatGroupId = -1;

	m_SendButton.EnableWindow(FALSE);
	m_UserList.DeleteAllItems();
	CalcPlayerMenuState();

	SetWindowText(mChatServerName);

}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::JoinChatGroup(unsigned short theGroupId, const char *thePassword)
{
	if(mRoutingConnection.get()==NULL)
		return;

	RoutingGroupInfoPtr aGroup = mRoutingConnection->GetGroupRef(theGroupId);
	if(aGroup.get()==NULL)
		return;

	string aName = WStringToString(aGroup->mName);
	AppendChat("Joining chat room: " + aName + "...\r\n",COLOR_SYSTEM);
	
	LeaveChatGroup();
	mPendingChatGroupId = theGroupId;

	wstring aPassword;
	if(thePassword!=NULL)
		aPassword = StringToWString(thePassword);

	mRoutingConnection->JoinGroup(theGroupId,aPassword,L"",0,0,new RoutingCompletion(JoinChatGroupCompletion));
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::CreateChatGroupCompletion(WONAPI::RoutingOp *theOp)
{
	RoutingCreateGroupOp *anOp = (RoutingCreateGroupOp*)theOp;

	if(!anOp->Succeeded())
	{
		char aMsg[1024];
		sprintf(aMsg,"Create chat room failed: %s",WONStatusToString(anOp->GetStatus()));
		MessageBox(aMsg,"Create Failed.");
		return;
	}

	RoutingGroupInfoPtr aGroup = mRoutingConnection->GetGroupRef(anOp->GetGroupId());
	HandleEnterChatGroup(aGroup);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::JoinChatGroupCompletion(WONAPI::RoutingOp *theOp)
{
	RoutingJoinGroupOp *anOp = (RoutingJoinGroupOp*)theOp;

	if(anOp->GetGroupId()!=mPendingChatGroupId)
	{
		if(anOp->Succeeded())
			mRoutingConnection->LeaveGroup(anOp->GetGroupId());

		return;
	}
	mPendingChatGroupId = -1;

	if(!anOp->Succeeded())
	{
		if(anOp->GetStatus()==WS_RoutingServG2_InvalidPassword)
		{
			PasswordDialog aDlg;
			aDlg.mCaption = "Chat password required...";
			if(aDlg.DoModal()==IDOK)
				JoinChatGroup(anOp->GetGroupId(),aDlg.m_Password);

			return;
		}


		char aMsg[1024];
		sprintf(aMsg,"Failed to join chat group: %s",WONStatusToString(anOp->GetStatus()));
		MessageBox(aMsg,"Join Failed.");
		return;
	}

	RoutingGroupInfoPtr aGroup = mRoutingConnection->GetGroupRef(anOp->GetGroupId());
	HandleEnterChatGroup(aGroup);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::ClientJoinedCompletion(WONAPI::RoutingOp *theOp)
{
	RoutingClientJoinedGroupOp *anOp = (RoutingClientJoinedGroupOp*)theOp;
	unsigned short aGroupId = anOp->GetGroupId();
	unsigned short aClientId = anOp->GetClientId();

	if(aGroupId!=mChatGroupId && aGroupId!=mGameGroupId)
		return;

	RoutingMemberInfoPtr aMember = mRoutingConnection->GetMemberRef(aClientId,aGroupId);
	if(aMember.get()!=NULL)
	{
		if(aGroupId==mChatGroupId)
			AddMember(aMember);
		else
			mGameBoard->AddMember(aMember);
	}
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::ClientLeftCompletion(WONAPI::RoutingOp *theOp)
{
	RoutingClientLeftGroupOp *anOp = (RoutingClientLeftGroupOp*)theOp;

	unsigned short aClientId = anOp->GetClientId();
	bool isMe = aClientId==mRoutingConnection->GetMyClientId();


	if(anOp->GetGroupId()==mChatGroupId)
	{
		if(isMe)
			LeaveChatGroup();
		else
			RemoveMember(anOp->GetClientId());
	}
	else if(anOp->GetGroupId()==mGameGroupId)
	{
		if(isMe)
			LeaveGameGroup();
		else
			mGameBoard->RemoveMember(anOp->GetClientId());
	}
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::ClientFlagsChangedCompletion(WONAPI::RoutingOp *theOp)
{
	RoutingClientFlagsChangedOp *anOp = (RoutingClientFlagsChangedOp*)theOp;
	
	RoutingMemberInfoPtr aMember = mRoutingConnection->GetMemberRef(anOp->GetClientId(),mChatGroupId);
	UpdateMember(aMember);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::MemberFlagsChangedCompletion(WONAPI::RoutingOp *theOp)
{
	RoutingMemberFlagsChangedOp *anOp = (RoutingMemberFlagsChangedOp*)theOp;

	int aGroupId = anOp->GetGroupId();
	RoutingMemberInfoPtr aMember = mRoutingConnection->GetMemberRef(anOp->GetClientId(),aGroupId);
	UpdateMember(aMember);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::InviteCompletion(WONAPI::RoutingOp *theOp)
{
	RoutingGroupInvitationOp *anOp = (RoutingGroupInvitationOp*)theOp;

	int aGroupId = anOp->GetGroupId();
	int aClientId = anOp->GetCaptainId();

	RoutingClientInfoPtr aClient = mRoutingConnection->GetClientRef(aClientId);
	if(aClient.get()==NULL)
		return;

	RoutingGroupInfoPtr aGroup = mRoutingConnection->GetGroupRef(aGroupId);
	if(aGroup.get()==NULL)
		return;

	AppendChat(WStringToString(aClient->mName),COLOR_USER);
	if(anOp->GetAmInvited())
		AppendChat(" invites you to join ",COLOR_INVITE);
	else
		AppendChat(" uninvites you from ",COLOR_INVITE);

	AppendChat(WStringToString(aGroup->mName) + ".\r\n",COLOR_INVITE);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::GetVersionCompletion(WONAPI::AsyncOp *theOp)
{
	GetVersionOp *anOp = (GetVersionOp*)theOp;

	// For this sample I am using a simple .cfg file for the version.
	// This makes it easy to "roll back" versions as the only
	// change is the version number.
	FILE* pVersionFile = fopen("version.cfg","r");
	char VersionString = 0;
	
	// Load in the version number from the file
	if (pVersionFile != NULL)
		VersionString = fgetc(pVersionFile);

	// Is this the latest version
	if (anOp->VersionIsLatest(&VersionString))
		return;

	// Get the patch
	

}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::JoinRequestCompletion(WONAPI::RoutingOp *theOp)
{
	RoutingGroupJoinAttemptOp *anOp = (RoutingGroupJoinAttemptOp*)theOp;
	if(mGameGroupId!=anOp->GetGroupId())
		return;

	RoutingClientInfoPtr aClient = mRoutingConnection->GetClientRef(anOp->GetClientId());
	if(aClient.get()==NULL)
		return;

	string aStr = "Allow " + WStringToString(aClient->mName) + " to join game?";

	JoinRequestDialog aDlg;
	aDlg.m_JoinText = aStr.c_str();
	int aResult = aDlg.DoModal();

	if(mRoutingConnection.get()==NULL)
		return;

	mRoutingConnection->AcceptClient(anOp->GetGroupId(),anOp->GetClientId(),aResult==IDC_ACCEPT_BUTTON, StringToWString((const char*)aDlg.m_Comment));
		
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::AppendChat(const std::string &theStr)
{
	if(mCurChatOutput==NULL)
	{
		mCurChatOutput = &m_ChatOutput;
		AppendChat(theStr);
		mCurChatOutput = &(mGameBoard->m_ChatOutput);
		AppendChat(theStr);
		mCurChatOutput = NULL;
		return;
	}

	// Remember selection
	CHARRANGE aSelRange;
	mCurChatOutput->GetSel(aSelRange);

	// Get the scroll bar position values
	bool needScroll = false;
	int aNumLines = mCurChatOutput->GetLineCount();
	int aTopLine = mCurChatOutput->GetFirstVisibleLine();
	int aNumVisibleLines = 0;

	TEXTMETRIC tm;
	CDC* aDC = mCurChatOutput->GetDC();
	aDC->GetTextMetrics(&tm);
	mCurChatOutput->ReleaseDC(aDC);
	CRect aRect; 
	mCurChatOutput->GetRect(&aRect);
	if(tm.tmAscent>0)
	{
		aNumVisibleLines = aRect.Height()/tm.tmAscent;
		if(aNumLines>=aNumVisibleLines && aNumLines-aNumVisibleLines <= aTopLine+1)
			needScroll = true;
	}


	// Store the line count
	int nBeginLineCount = mCurChatOutput->GetLineCount();

	// Freeze the display of the edit control
	mCurChatOutput->SetRedraw(FALSE);	

	// Append the chat text
	mCurChatOutput->SetSel(mCurChatOutput->GetWindowTextLength(),mCurChatOutput->GetWindowTextLength());
	mCurChatOutput->SetSelectionCharFormat(mCharFormat);
	mCurChatOutput->ReplaceSel(theStr.c_str());
	CHARFORMAT noFormat;
	noFormat.cbSize = sizeof(CHARFORMAT);
	noFormat.dwMask = 0;
	mCurChatOutput->SetSelectionCharFormat(noFormat);

	// Restore Selection
	if (aSelRange.cpMin != aSelRange.cpMax)
		mCurChatOutput->SetSel(aSelRange);
	
	int aNewNumLines = mCurChatOutput->GetLineCount();
	int aNewTopLine = mCurChatOutput->GetFirstVisibleLine();

	if(needScroll)
		mCurChatOutput->LineScroll((aNumLines-aNewTopLine)-aNumVisibleLines);
	else
		mCurChatOutput->LineScroll(aTopLine - aNewTopLine);


	// Resume the display of the edit control
	mCurChatOutput->SetRedraw(TRUE);
	mCurChatOutput->RedrawWindow();

}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::AppendChat(const std::string &theStr, COLORREF theColor)
{
	CHARFORMAT aFormat;
	aFormat.cbSize = sizeof(aFormat);
	aFormat.dwMask = CFM_COLOR;
	aFormat.dwEffects = 0;
	aFormat.crTextColor = theColor;
	mCharFormat = aFormat;

	AppendChat(theStr);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::SendGameData(WONAPI::ByteBufferPtr theData)
{
	mRoutingConnection->SendData(theData,0,mGameGroupId);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::SendGameDataTo(WONAPI::ByteBufferPtr theData, unsigned short theClientId)
{
	mRoutingConnection->SendData(theData,0,theClientId);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::PeerDataCompletion(WONAPI::RoutingOp *theOp)
{
	RoutingPeerDataOp *anOp = (RoutingPeerDataOp*)theOp;

	unsigned short aSenderId = anOp->GetSenderId();
	if(aSenderId==mRoutingConnection->GetMyClientId())
		return;

	mGameBoard->HandleMessage(anOp->GetData(),aSenderId);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CChatSampleDlg::PeerChatCompletion(WONAPI::RoutingOp *theOp)
{
	RoutingPeerChatOp *anOp = (RoutingPeerChatOp*)theOp;

	RoutingClientInfoPtr aSendClient = mRoutingConnection->GetClientRef(anOp->GetSenderId());
	if(aSendClient.get()==NULL)
		return;

	const RoutingRecipientList &aList = anOp->GetRecipients();
	if(aList.empty())
		return;

	int aDestId = aList.front();
	if(aDestId==mChatGroupId) 
		mCurChatOutput = &m_ChatOutput;
	else if(aDestId==mGameGroupId) 
		mCurChatOutput = &(mGameBoard->m_ChatOutput);
	else 
		mCurChatOutput = NULL;

	if(anOp->IsEmote())
	{
		AppendChat(WStringToString(aSendClient->mName),COLOR_USER);
		AppendChat(WStringToString(anOp->GetText()),COLOR_EMOTE);
	}		
	else if(anOp->IsWhisper())
	{
		if(aSendClient->mId==mRoutingConnection->GetMyClientId()) // Whisper from me
		{
			RoutingClientInfoPtr aDestClient = mRoutingConnection->GetClientRef(aDestId);
			if(aDestClient.get()==NULL)
				return;

			AppendChat("You whisper to ",COLOR_USER);
			AppendChat(WStringToString(aDestClient->mName),COLOR_USER);
			AppendChat(": ");
			AppendChat(WStringToString(anOp->GetText()),COLOR_WHISPER);
		}
		else
		{
			AppendChat(WStringToString(aSendClient->mName),COLOR_USER);
			AppendChat(" whispers to you: ",COLOR_USER);
			AppendChat(WStringToString(anOp->GetText()),COLOR_WHISPER);
		}
	}
	else
	{
		AppendChat(WStringToString(aSendClient->mName),COLOR_USER);
		AppendChat(": ");
		AppendChat(WStringToString(anOp->GetText()),COLOR_BROADCAST);
	}

	AppendChat("\r\n");

	mCurChatOutput = &m_ChatOutput;
}

void CChatSampleDlg::HandleInput(const char *theInput, bool fromGame)
{
	if(mRoutingConnection.get()==NULL)
		return;

	wstring anInput = StringToWString(theInput);
	if(anInput.length()==0)
		return;

	unsigned short aGroupId;
	if(fromGame) 
	{
		mCurChatOutput = &(mGameBoard->m_ChatOutput);
		aGroupId = mGameGroupId;
	}
	else 
	{
		mCurChatOutput = &m_ChatOutput;
		aGroupId = mChatGroupId;
	}

	if(mRoutingConnection->HandleInput(anInput,aGroupId))
	{
		SmartRoutingConnection::InputCommand aCommand = mRoutingConnection->GetLastInputCommand();
		if(aCommand==SmartRoutingConnection::InputCommand_Ignore)
			UpdateMember(mRoutingConnection->GetLastTargetMemberRef());
	}
	else
	{
		switch(mRoutingConnection->GetLastInputError())
		{
			case SmartRoutingConnection::InputError_ClientNotFound:
				AppendChat("Client not found: ",COLOR_ERROR);
				AppendChat(WStringToString(mRoutingConnection->GetLastMatchName()));
				AppendChat("\r\n");
				break;

			case SmartRoutingConnection::InputError_ClientAmbiguous:
				AppendChat("Name could match multiple clients: ",COLOR_ERROR);
				AppendChat(WStringToString(mRoutingConnection->GetLastMatchName()));
				AppendChat("\r\n");
				break;

			case SmartRoutingConnection::InputError_InvalidCommand:
				AppendChat("InvalidCommand.\r\n",COLOR_ERROR);
				break;

			case SmartRoutingConnection::InputError_BadTime:
				AppendChat("Bad time specification.\r\n",COLOR_ERROR);
				break;
		}
	}

	mCurChatOutput = &m_ChatOutput;
}

void CChatSampleDlg::OnSendbutton() 
{
	// TODO: Add your control notification handler code here

	CString aChatStr;
	m_ChatInput.GetWindowText(aChatStr);
	HandleInput(aChatStr, false);
	m_ChatInput.SetWindowText("");
}

LRESULT CChatSampleDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	switch(message)
	{
		case MSG_PUMP_API: 
			PostMessage(MSG_SIGNAL_PUMP_THREAD); // need this since pump can get stuck in a modal dialog (used to signal right after pumping)
			if(mAPI!=NULL) mAPI->Pump(0);
			return 0;

		case MSG_SIGNAL_PUMP_THREAD:
			if(mPumpThread!=NULL)
				mPumpThread->Signal();
			return 0;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}


void CChatSampleDlg::OnLoginDisconnect() 
{
	// TODO: Add your command handler code here
	KillRoutingConnection();
}

void CChatSampleDlg::OnLoginLogininfo() 
{
	// TODO: Add your command handler code here
	LoginDlg aLoginDlg;
	aLoginDlg.m_UserName = mAuthContext->GetUserName().c_str();
	aLoginDlg.m_Password = mAuthContext->GetPassword().c_str();
	int aResult = aLoginDlg.DoModal();
	if(aResult!=IDOK)
		return;

	mAuthContext->SetUserName(StringToWString((const char*)aLoginDlg.m_UserName));
	mAuthContext->SetPassword(StringToWString((const char*)aLoginDlg.m_Password));
	mAuthContext->AddCommunity(L"WON");
	
	mAuthCancelDialog = new CancelDialog;
	mAuthCancelDialog->m_Caption = "Logging in...";
	mAuthCancelDialog->m_Message = "Logging in to AuthServer.";
	
	mGetCertOp = new GetCertOp(mAuthContext);
	mGetCertOp->SetCompletion(new DlgCompletion(AuthLoginCompletion));
	if (aLoginDlg.m_CreateAccount == TRUE)
		mGetCertOp->SetCreateAccount(true);

	mGetCertOp->RunAsync(OP_TIMEOUT_INFINITE);

	AppendChat("Logging in to AuthServer...\r\n",COLOR_SYSTEM);

	if(mAuthCancelDialog->DoModal()==IDCANCEL)
	{
		mGetCertOp->Kill();
		AppendChat("AuthServer login canceled.\r\n",COLOR_SYSTEM);
	}

	delete mAuthCancelDialog;
	mAuthCancelDialog = NULL;

	mGetCertOp = NULL;
}

void CChatSampleDlg::OnViewChatservers() 
{
	// TODO: Add your command handler code here
	ChatServerDialog aDlg(mChatServerList);

	int aResult = aDlg.DoModal();
	if(aResult!=IDOK)
		return;

	DirEntityPtr anEntity = aDlg.mSelEntity;
	if(anEntity.get()==NULL)
		return;

	mJoinChatServerCancelDialog = new CancelDialog;
	mJoinChatServerCancelDialog->m_Caption = "Connecting chat server.";
	mJoinChatServerCancelDialog->m_Message = "Joining " + CString(WStringToString(anEntity->mDisplayName).c_str()) + "...";

	AppendChat("Joining chat server...\r\n",COLOR_SYSTEM);

	KillRoutingConnection();
	mRoutingConnection = new SmartRoutingConnection;
	mRoutingConnection->SetAuth(mAuthContext);
	mRoutingConnection->SetConnectCompletion(new DlgCompletion(ConnectCompletion));
	mRoutingConnection->SetCloseCompletion(new DlgCompletion(CloseCompletion));
	
	mRoutingConnection->SetRoutingCompletion(RoutingOp_PeerChat,new RoutingCompletion(PeerChatCompletion));
	mRoutingConnection->SetRoutingCompletion(RoutingOp_ClientJoinedGroup, new RoutingCompletion(ClientJoinedCompletion));
	mRoutingConnection->SetRoutingCompletion(RoutingOp_ClientLeftGroup, new RoutingCompletion(ClientLeftCompletion));
	mRoutingConnection->SetRoutingCompletion(RoutingOp_ClientFlagsChanged, new RoutingCompletion(ClientFlagsChangedCompletion));
	mRoutingConnection->SetRoutingCompletion(RoutingOp_MemberFlagsChanged, new RoutingCompletion(MemberFlagsChangedCompletion));
	mRoutingConnection->SetRoutingCompletion(RoutingOp_GroupCaptainChanged, new RoutingCompletion(CaptainChangedCompletion));
	mRoutingConnection->SetRoutingCompletion(RoutingOp_PeerData, new RoutingCompletion(PeerDataCompletion));
	mRoutingConnection->SetRoutingCompletion(RoutingOp_GroupInvitation, new RoutingCompletion(InviteCompletion));
	mRoutingConnection->SetRoutingCompletion(RoutingOp_GroupJoinAttempt, new RoutingCompletion(JoinRequestCompletion));

	SetWindowText("Connecting...");
	mChatServerName = WStringToString(anEntity->mDisplayName).c_str();

	mRoutingConnection->ConnectAsync(anEntity->GetNetAddrAsIP());

	if(mJoinChatServerCancelDialog->DoModal()==IDCANCEL)
	{
		KillRoutingConnection();
		AppendChat("Chat server login canceled.\r\n",COLOR_SYSTEM);
	}

	delete mJoinChatServerCancelDialog;
	mJoinChatServerCancelDialog = NULL;
}

void CChatSampleDlg::OnViewGameboard() 
{
	// TODO: Add your command handler code here
	mGameBoard->ShowWindow(SW_NORMAL);	
}

void CChatSampleDlg::OnChatFilterdirtywords() 
{
	// TODO: Add your command handler code here
	CMenu *aMenu = GetMenu();
	mFilterDirtyWords = !mFilterDirtyWords;
	aMenu->CheckMenuItem(ID_CHAT_FILTERDIRTYWORDS,mFilterDirtyWords?MF_CHECKED:MF_UNCHECKED);

	if(mRoutingConnection.get()!=NULL)
		mRoutingConnection->SetClientFlags(RoutingClientFlag_DirtyWordFilteringOff,mFilterDirtyWords?0:RoutingClientFlag_DirtyWordFilteringOff, 0, 0);
}

void CChatSampleDlg::OnViewGamerooms() 
{
	// TODO: Add your command handler code here
	RoomListDialog aDlg;
	aDlg.mGroupMap = mRoutingConnection->GetGroupMapRef();
	aDlg.mIsChat = false;

	int aResult = aDlg.DoModal();
	if(aResult==RoomListResult_Cancel || aDlg.mSelGroup.get()==NULL)
		return;

	mObserveGame = (aResult==RoomListResult_Observe);
	JoinGameGroup(aDlg.mSelGroup->mId,"");

}

void CChatSampleDlg::OnViewChatrooms() 
{
	// TODO: Add your command handler code here
	RoomListDialog aDlg;
	aDlg.mGroupMap = mRoutingConnection->GetGroupMapRef();
	aDlg.mIsChat = true;

	int aResult = aDlg.DoModal();
	if(aResult==RoomListResult_Cancel || aDlg.mSelGroup.get()==NULL)
		return;

	JoinChatGroup(aDlg.mSelGroup->mId);
}


void CChatSampleDlg::OnLoginExit() 
{
	// TODO: Add your command handler code here
	EndDialog(IDOK);	
}

void CChatSampleDlg::OnProfile()
{
	ProfileDialog aDlg(mAuthContext, mProfileContext);
	
	if(aDlg.DoModal()!=IDOK)
		return;
	

}

void CChatSampleDlg::OnCreateChatroom() 
{
	// TODO: Add your command handler code here
	CreateChatDialog aDlg;
	aDlg.mIsChat = true;
	if(aDlg.DoModal()!=IDOK)
		return;

	LeaveChatGroup();
	wstring aName = StringToWString((const char*)aDlg.m_Name);
	wstring aPassword;
	if(aDlg.m_PasswordCheck)
		aPassword = StringToWString((const char*)aDlg.m_Password);
	
	if(mRoutingConnection.get()!=NULL)
	{
		mRoutingConnection->CreateGroup(RoutingId_Invalid,aName,aPassword,0,
			RoutingGroupFlag_IsChatRoom | RoutingGroupFlag_NoCaptain,
			RoutingCreateGroupJoinFlag_JoinAsPlayer, 
			0,
			new RoutingCompletion(CreateChatGroupCompletion));
	}
}


void CChatSampleDlg::OnCreateGame() 
{
	// TODO: Add your command handler code here
	CreateChatDialog aDlg;
	aDlg.mIsChat = false;
	if(aDlg.DoModal()!=IDOK)
		return;

	LeaveGameGroup();
	wstring aName = StringToWString((const char*)aDlg.m_Name);
	wstring aPassword;
	if(aDlg.m_PasswordCheck)
		aPassword = StringToWString((const char*)aDlg.m_Password);

	unsigned long flags = 0;
	if(aDlg.m_AskToJoin)
		flags |= RoutingGroupFlag_AskCaptainToJoin;

	if(aDlg.m_InviteOnly)
		flags |= RoutingGroupFlag_InviteOnly;

	if(aDlg.m_AllowObservers)
		flags |= RoutingGroupFlag_AllowObservers;

	mObserveGame = false;
	if(mRoutingConnection.get()!=NULL)
	{
		mRoutingConnection->CreateGroup(RoutingId_Invalid,aName,aPassword,2,
			flags,
			RoutingCreateGroupJoinFlag_JoinAsPlayer, 
			0,
			new RoutingCompletion(CreateGameGroupCompletion));
	}
}

void CChatSampleDlg::CalcPlayerMenuState()
{
	CMenu *aMenu = GetMenu();
	aMenu->EnableMenuItem(ID_PLAYER_IGNORE,MF_GRAYED);
	aMenu->EnableMenuItem(ID_PLAYER_WHISPER,MF_GRAYED);
	aMenu->EnableMenuItem(ID_PLAYER_INVITE,MF_GRAYED);
	aMenu->EnableMenuItem(ID_PLAYER_UNINVITE,MF_GRAYED);

	RoutingMemberInfoPtr aMember = GetSelectedMember();
	if(aMember.get()==NULL)
		return;

	SmartRoutingClientInfo *aClient = (SmartRoutingClientInfo*)aMember->mClientInfo.get();

	if(aClient->mIgnored)
		aMenu->ModifyMenu(ID_PLAYER_IGNORE, MF_STRING, ID_PLAYER_IGNORE, "Unignore");
	else
		aMenu->ModifyMenu(ID_PLAYER_IGNORE, MF_STRING, ID_PLAYER_IGNORE, "Ignore");

	aMenu->EnableMenuItem(ID_PLAYER_IGNORE,MF_ENABLED);
	aMenu->EnableMenuItem(ID_PLAYER_WHISPER,MF_ENABLED);

	if(mGameBoard->GetAmCaptain())
	{
		aMenu->EnableMenuItem(ID_PLAYER_INVITE,MF_ENABLED);
		aMenu->EnableMenuItem(ID_PLAYER_UNINVITE,MF_ENABLED);
	}
}

void CChatSampleDlg::OnClickUserlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CalcPlayerMenuState();
	
	*pResult = 0;
}

void CChatSampleDlg::OnRclickUserlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CalcPlayerMenuState();

	CMenu* aMenu = GetMenu();
	aMenu = aMenu->GetSubMenu(4);


	POINT point;
	DWORD dwPos;

	dwPos = GetMessagePos();

	point.x = LOWORD (dwPos);
	point.y = HIWORD (dwPos);

	// Convert the point from screen to client coordinates,
	// relative to the listview
	//ScreenToClient(&point);

	aMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	
	*pResult = 0;
}

void CChatSampleDlg::OnPlayerIgnore() 
{
	// TODO: Add your command handler code here
	RoutingMemberInfoPtr aMember = GetSelectedMember();	
	if(aMember.get()==NULL)
		return;

	SmartRoutingClientInfo *aClient = (SmartRoutingClientInfo*)aMember->mClientInfo.get();
	mRoutingConnection->SetIgnore(aClient,!aClient->mIgnored);
	UpdateMember(aMember);
}

void CChatSampleDlg::OnPlayerWhisper() 
{
	// TODO: Add your command handler code here
	RoutingMemberInfoPtr aMember = GetSelectedMember();
	if(aMember.get()==NULL)
		return;

	CString aCommand = "/w \"";
	aCommand += WStringToString(aMember->mClientInfo->mName).c_str();
	aCommand += "\" ";

	m_ChatInput.SetWindowText(aCommand);
	m_ChatInput.SetSel(m_ChatInput.GetWindowTextLength(),m_ChatInput.GetWindowTextLength());
	m_ChatInput.SetFocus();
}

void CChatSampleDlg::OnPlayerInvite() 
{
	// TODO: Add your command handler code here
	if(mRoutingConnection.get()==NULL)
		return;

	RoutingMemberInfoPtr aMember = GetSelectedMember();
	if(aMember.get()==NULL)
		return;

	mRoutingConnection->InviteClient(mGameGroupId,aMember->mClientId, true, L"");
}

void CChatSampleDlg::OnPlayerUninvite() 
{
	// TODO: Add your command handler code here
	if(mRoutingConnection.get()==NULL)
		return;

	RoutingMemberInfoPtr aMember = GetSelectedMember();
	if(aMember.get()==NULL)
		return;

	mRoutingConnection->InviteClient(mGameGroupId,aMember->mClientId, false, L"");	
}

void CChatSampleDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect dlgClientRect;
	GetClientRect(&dlgClientRect);

	CRect aRect = GetRect(m_AdRect);
	if (aRect.PtInRect(point))
	{
		AdApi::AdResult result = mAdAPI.AdClicked();
		if (result != AdApi::AdResult_Success)
		{
			MessageBox("Error launching browser", "Error", MB_ICONSTOP);
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CChatSampleDlg::OnDestroy() 
{
	
	CDialog::OnDestroy();

}

BOOL CChatSampleDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	KillRoutingConnection();
	if(mAdBitmap!=NULL)
	{
		mAdAPI.ReleaseDisplayImage(mAdBitmap);
		mAdBitmap = NULL;
		mNewAdImage = false;
	}

	AdApi::AdResult result = mAdAPI.Shutdown();
	if (result != AdApi::AdResult_Success)
	{
		MessageBox("Error on shutdown", "Error", MB_ICONSTOP);
	}

	mPumpThread->Stop();
	mInstance = NULL;
	
	return CDialog::DestroyWindow();
}

BOOL CChatSampleDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	CChatSampleApp *pApp = (CChatSampleApp *)AfxGetApp();
	CPoint aPoint;
	GetCursorPos(&aPoint);
	ScreenToClient(&aPoint);
	
	CRect aRect = GetRect(m_AdRect);
	if(aRect.PtInRect(aPoint) && mAdBitmap!=NULL)
	{
		SetCursor(pApp->LoadCursor(IDC_HAND_CURSOR));
		return TRUE;
	}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}	

