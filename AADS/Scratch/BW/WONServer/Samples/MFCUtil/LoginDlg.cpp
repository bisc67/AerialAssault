// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mfcutil.h"
#include "LoginDlg.h"
#include "CancelDlg.h"

#include "WONAuth/GetCertOp.h"
#include "WONCommon/StringUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


using namespace WONAPI;

/////////////////////////////////////////////////////////////////////////////
// LoginDlg dialog
LoginDlg::LoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(LoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(LoginDlg)
	m_Password = _T("");
	m_UserName = _T("");
	//}}AFX_DATA_INIT

	mGetDirCancelDlg = NULL;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
LoginDlg::~LoginDlg()
{
	if(mGetDirOp.get()!=NULL)
		mGetDirOp->Kill();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void LoginDlg::Init(AuthContext *theAuthContext, ServerContext *theDirServers)
{
	mAuthContext = theAuthContext;
	mDirServers = theDirServers;
	mGetDirStatus = WS_Success;
	CheckAuthDir();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void LoginDlg::CheckAuthDir()
{
	if(mDirServers.get()!=NULL)
	{
		ServerContext *anAuthAddrs = mAuthContext->GetServerContext();
		if(anAuthAddrs==NULL || anAuthAddrs->GetNumAddresses()==0) 
		{
			if(mGetDirOp.get()!=NULL) // already getting the directory
				return;

			mGetDirOp = new GetDirOp(mDirServers);
			mGetDirOp->SetPath(L"/TitanServers/Auth");
			mGetDirOp->SetFlags(DIR_GF_DECOMPSERVICES | DIR_GF_SERVADDNETADDR);
			mGetDirOp->SetCompletion(new ParamCompletion<AsyncOpPtr,LoginDlg*>(GetDirCompletion,this));
			mGetDirOp->RunAsync(OP_TIMEOUT_INFINITE);		
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void LoginDlg::GetDirCompletion(AsyncOpPtr theOp, LoginDlg *that)
{
	if(theOp->Killed())
		return;

	GetDirOp *anOp = (GetDirOp*)theOp.get();

	that->mGetDirOp = NULL;
	that->mGetDirStatus = anOp->GetStatus();
	if(anOp->Succeeded())
		that->mAuthContext->AddAddressesFromDir(anOp->GetDirEntityList());

	if(that->mGetDirCancelDlg!=NULL)
		that->mGetDirCancelDlg->EndDialog(IDOK);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void LoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LoginDlg)
	DDX_Control(pDX, IDC_USERNAME, m_UserNameCtrl);
	DDX_Control(pDX, IDC_PASSWORD, m_PasswordCtrl);
	DDX_Control(pDX, IDOK, m_LoginButton);
	DDX_Text(pDX, IDC_PASSWORD, m_Password);
	DDX_Text(pDX, IDC_USERNAME, m_UserName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(LoginDlg, CDialog)
	//{{AFX_MSG_MAP(LoginDlg)
	ON_EN_CHANGE(IDC_PASSWORD, OnChangePassword)
	ON_EN_CHANGE(IDC_USERNAME, OnChangeUsername)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// LoginDlg message handlers
void LoginDlg::CheckEnableLogin()
{
	if(m_PasswordCtrl.GetWindowTextLength()>0 && m_UserNameCtrl.GetWindowTextLength()>0)
		m_LoginButton.EnableWindow(TRUE);
	else
		m_LoginButton.EnableWindow(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void LoginDlg::OnChangePassword() 
{
	// TODO: Add your control notification handler code here
	CheckEnableLogin();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void LoginDlg::OnChangeUsername() 
{	
	// TODO: Add your control notification handler code here
	CheckEnableLogin();	
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void LoginDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	CheckAuthDir();

	if(mGetDirOp.get()!=NULL) // waiting for auth directory
	{
		mGetDirCancelDlg = new CancelDlg("Getting list of AuthServers.","Getting AuthDir...");
		int aResult = mGetDirCancelDlg->DoModal();
		delete mGetDirCancelDlg;
		mGetDirCancelDlg = NULL;
		
		if(aResult==IDCANCEL)
			return;
	}

	if(mGetDirStatus!=WS_Success) // Error getting auth directory
	{
		char aBuf[256];
		sprintf(aBuf,"Error getting auth dir: %s",WONStatusToString(mGetDirStatus));
		MessageBox(aBuf,"Dir Error.");
		return;
	}

	mAuthContext->SetUserName(StringToWString((const char*)m_UserName));
	mAuthContext->SetPassword(StringToWString((const char*)m_Password));

	GetCertOpPtr anOp = new GetCertOp(mAuthContext);
	CancelDlg aDlg("Logging in to AuthServer.","Logging in...");
	int aResult = aDlg.DoOp(anOp,"Login Error");

	if(aResult==IDCANCEL)
		return;

	CDialog::OnOK();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
BOOL LoginDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CheckEnableLogin();
	CheckAuthDir();
	
	// TODO: Add extra initialization here
	m_UserNameCtrl.SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
