// JoinByIPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mfcutil.h"
#include "JoinByIPDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// JoinByIPDlg dialog


JoinByIPDlg::JoinByIPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(JoinByIPDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(JoinByIPDlg)
	m_IPAddrStr = _T("");
	//}}AFX_DATA_INIT
}


void JoinByIPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(JoinByIPDlg)
	DDX_Control(pDX, IDOK, m_JoinButton);
	DDX_Control(pDX, IDC_IP_ADDR, m_IPAddrCtrl);
	DDX_Text(pDX, IDC_IP_ADDR, m_IPAddrStr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(JoinByIPDlg, CDialog)
	//{{AFX_MSG_MAP(JoinByIPDlg)
	ON_EN_CHANGE(IDC_IP_ADDR, OnChangeIpAddr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// JoinByIPDlg message handlers

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
BOOL JoinByIPDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	m_JoinButton.EnableWindow(m_IPAddrCtrl.GetWindowTextLength()>0);
	m_IPAddrCtrl.SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void JoinByIPDlg::OnChangeIpAddr() 
{	
	// TODO: Add your control notification handler code here
	m_JoinButton.EnableWindow(m_IPAddrCtrl.GetWindowTextLength()>0);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void JoinByIPDlg::OnOK() 
{
	// TODO: Add extra validation here
	CString anIPStr;
	m_IPAddrCtrl.GetWindowText(anIPStr);

	if(!mAddr.Set((const char*)anIPStr))
	{
		MessageBox("Invalid IP Address.","Invalid IP");
		return;
	}

	CDialog::OnOK();
}
