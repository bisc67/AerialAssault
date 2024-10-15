// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mfcutil.h"
#include "ServerDlg.h"
#include "CancelDlg.h"
#include "WONCommon/StringUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace WONAPI;

///////////////////////////////////////////////////////////////////////////////
// ServerDlg dialog


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
ServerDlg::ServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ServerDlg)
	//}}AFX_DATA_INIT
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
ServerDlg::~ServerDlg()
{
	CancelGetDirOp();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ServerDlg::CancelGetDirOp()
{
	if(mGetDirOp.get()!=NULL)
	{
		mGetDirOp->Kill();
		mGetDirOp = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ServerDlg::SetDirEntityList(const WONAPI::DirEntityList &theList)
{
	mDirEntityList = theList;
	PopulateList();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ServerDlg::Init(ServerContext *theDirServers,  const std::wstring &thePath, const wchar_t *theNameFilter)
{
	mDirServers = theDirServers;
	mPath = thePath;
	if(theNameFilter!=NULL)
		mNameFilter = theNameFilter;
	else
		mNameFilter = L"";
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ServerDlg::PopulateList()
{
	if(m_hWnd==NULL)
		return;

	m_ServerList.ResetContent();
	DirEntityList::iterator anItr = mDirEntityList.begin();
	while(anItr!=mDirEntityList.end())
	{
		const DirEntity *anEntity = *anItr;
		if(anEntity->mName==mNameFilter)
		{
			int aPos = m_ServerList.AddString(WStringToString(anEntity->mDisplayName).c_str());
			m_ServerList.SetItemDataPtr(aPos,(void*)anEntity);
		}
		++anItr;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ServerDlg::GetDirCompletion(AsyncOpPtr theOp, ServerDlg *that)
{
	if(theOp->Killed())
		return;

	GetDirOp *anOp = (GetDirOp*)theOp.get();
	that->mGetDirOp = NULL;
	if(anOp->Succeeded())
		that->SetDirEntityList(anOp->GetDirEntityList());
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ServerDlg::GetDir(bool showCancelDlg)
{
	CancelGetDirOp();

	GetDirOpPtr anOp = new GetDirOp(mDirServers);
	anOp->SetPath(mPath);
	DWORD aFlags = DIR_GF_DECOMPSERVICES | DIR_GF_ADDDISPLAYNAME | DIR_GF_SERVADDNETADDR;
	if(!mNameFilter.empty())
		aFlags |= DIR_GF_SERVADDNAME;

	anOp->SetFlags(aFlags);
	if(mAuthContext.get()!=NULL)
		anOp->SetAuthType(AUTH_TYPE_SESSION,mAuthContext);

	if(showCancelDlg)
	{
		CancelDlg aDlg("Getting server directory.","Contacting DirServer...");
		int aResult = aDlg.DoOp(anOp,"DirServer Error");
		if(aResult==IDOK)
			SetDirEntityList(anOp->GetDirEntityList());
	}
	else
	{
		mGetDirOp = anOp;
		anOp->SetCompletion(new ParamCompletion<AsyncOpPtr,ServerDlg*>(GetDirCompletion,this));
		anOp->RunAsync(OP_TIMEOUT_INFINITE);
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ServerDlg)
	DDX_Control(pDX, IDOK, m_JoinButton);
	DDX_Control(pDX, IDC_SERVER_LIST, m_ServerList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ServerDlg, CDialog)
	//{{AFX_MSG_MAP(ServerDlg)
	ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
	ON_LBN_SELCHANGE(IDC_SERVER_LIST, OnSelchangeServerList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ServerDlg message handlers

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
BOOL ServerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	PopulateList();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ServerDlg::OnRefresh() 
{
	// TODO: Add your control notification handler code here
	GetDir(true);	
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ServerDlg::OnSelchangeServerList() 
{
	// TODO: Add your control notification handler code here
	int aPos = m_ServerList.GetCurSel();
	if(aPos==LB_ERR)
	{
		m_JoinButton.EnableWindow(FALSE);
		return;
	}

	m_JoinButton.EnableWindow(TRUE);
	mDirEntity = (const DirEntity*)m_ServerList.GetItemDataPtr(aPos);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
