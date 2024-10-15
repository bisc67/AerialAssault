// CancelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mfcutil.h"
#include "CancelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace WONAPI;
///////////////////////////////////////////////////////////////////////////////
// CancelDlg dialog


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
CancelDlg::CancelDlg(const char *theText, const char *theCaption, CWnd* pParent /*=NULL*/)
	: CDialog(CancelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CancelDlg)
	m_Text = _T("");
	//}}AFX_DATA_INIT

	if(theCaption!=NULL)
		m_Caption =  theCaption;

	if(theText!=NULL)
		m_Text = theText;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CancelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CancelDlg)
	DDX_Text(pDX, IDC_TEXT, m_Text);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CancelDlg, CDialog)
	//{{AFX_MSG_MAP(CancelDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CancelDlg message handlers

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CancelDlg::CancelOpCompletion(WONAPI::AsyncOpPtr theOp, CancelDlg *that)
{	
	if(!theOp->Killed())
	{
		if(theOp->Succeeded() && that->mCompletion.get()!=NULL)
			that->mCompletion->Complete(theOp);

		that->EndDialog(IDOK);
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void CancelDlg::ShowError(AsyncOp *theOp, const char *theErrorStr)
{
	WONStatus aStatus = theOp->GetStatus();
	char aBuf[256];
	sprintf(aBuf,"%s: %s",theErrorStr,WONStatusToString(aStatus));
	MessageBox(aBuf,"Error.");
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int CancelDlg::DoOp(AsyncOp *theOp, const char *theErrorStr, DWORD theTimeout, bool finishOnCancel)
{
	theOp->SetCompletion(new ParamCompletion<AsyncOpPtr,CancelDlg*>(CancelOpCompletion,this));
	theOp->RunAsync(theTimeout);
	int aResult = DoModal();

	if(aResult==IDCANCEL)
		theOp->Kill(finishOnCancel);
	else if(theErrorStr!=NULL && !theOp->Succeeded())
	{
		ShowError(theOp,theErrorStr);
		return IDCANCEL;
	}

	return aResult;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
BOOL CancelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_Caption.GetLength()!=0)
		SetWindowText(m_Caption);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
