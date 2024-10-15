// ancelDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ChatSample.h"
#include "CancelDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CancelDialog dialog


CancelDialog::CancelDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CancelDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CancelDialog)
	m_Message = _T("");
	//}}AFX_DATA_INIT
	m_ButtonText = "Cancel";
}


void CancelDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CancelDialog)
	DDX_Control(pDX, IDCANCEL, m_Button);
	DDX_Text(pDX, IDC_MESSAGE, m_Message);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CancelDialog, CDialog)
	//{{AFX_MSG_MAP(CancelDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CancelDialog message handlers

BOOL CancelDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetWindowText(m_Caption);
	m_Button.SetWindowText(m_ButtonText);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
