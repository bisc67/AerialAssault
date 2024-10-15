// MOTDDialog.cpp : implementation file
//
#pragma warning (disable : 4786)
#include "stdafx.h"
#include "ChatSample.h"
#include "MOTDDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MOTDDialog dialog


MOTDDialog::MOTDDialog(CWnd* pParent /*=NULL*/)
	: CDialog(MOTDDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(MOTDDialog)
	m_MOTDText = _T("");
	//}}AFX_DATA_INIT
}


void MOTDDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MOTDDialog)
	DDX_Text(pDX, IDC_MOTD_EDIT, m_MOTDText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MOTDDialog, CDialog)
	//{{AFX_MSG_MAP(MOTDDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MOTDDialog message handlers
