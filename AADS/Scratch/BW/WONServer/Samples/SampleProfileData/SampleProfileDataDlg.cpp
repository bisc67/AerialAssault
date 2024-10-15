// SampleProfileDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SampleProfileData.h"
#include "SampleProfileDataDlg.h"
#include "MsgDialog.h"

#include "WONDir/GetDirOp.h"
#include "WONAuth/GetCertOp.h"
#include "WONDB/GetProfileOp.h"
#include "WONDB/SetProfileOp.h"
#include "WONCommon/StringUtil.h"
#include "WONDB/ProfileData.h"
#include "WONDB/CreateAccountOp.h"

const wchar_t*	DIR_SAMPLE				=	L"/API2Sample";				// Game directory. (/GameName)
const wchar_t*	DIR_TITANSERVERS		=	L"/TitanServers";			// Directory for Auth and Profile servers
const wchar_t*	AUTH_SERVER_STRING		=	L"AuthServer";				// Authenticates user information
const wchar_t*	PROFILE_SERVER_STRING	=	L"TitanProfileServer";		// Stores user information
const wchar_t*	COMMUNITY_SAMPLE		=	L"WON";

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

WONAPICore apiCore(true);

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSampleProfileDataDlg dialog

CSampleProfileDataDlg::CSampleProfileDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSampleProfileDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSampleProfileDataDlg)
	m_UserName = _T("");
	m_Password = _T("");
	m_Email = _T("");
	m_Address1 = _T("");
	m_Address2 = _T("");
	m_City = _T("");
	m_Company = _T("");
	m_Email2 = _T("");
	m_Ext1 = _T("");
	m_Ext2 = _T("");
	m_FirstName = _T("");
	m_LastName = _T("");
	m_MiddleName = _T("");
	m_Phone1 = _T("");
	m_Phone2 = _T("");
	m_PostalCode = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSampleProfileDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSampleProfileDataDlg)
	DDX_Control(pDX, IDC_STATE, m_ListState);
	DDX_Control(pDX, IDCANCEL, m_ButtonCancel);
	DDX_Control(pDX, IDSETPROFILE, m_ButtonSetProfile);
	DDX_Control(pDX, IDCREATEACCOUNT, m_ButtonCreateAccount);
	DDX_Control(pDX, IDGETPROFILE, m_ButtonGetProfile);
	DDX_Control(pDX, IDLOGIN, m_ButtonLogin);
	DDX_Control(pDX, IDC_AGERANGE, m_ListAgeRange);
	DDX_Control(pDX, IDC_COUNTRY, m_ListCountry);
	DDX_Text(pDX, IDC_USERNAME, m_UserName);
	DDV_MaxChars(pDX, m_UserName, 20);
	DDX_Text(pDX, IDC_PASSWORD, m_Password);
	DDV_MaxChars(pDX, m_Password, 20);
	DDX_Text(pDX, IDC_EMAIL, m_Email);
	DDX_Text(pDX, IDC_ADDRESS1, m_Address1);
	DDX_Text(pDX, IDC_ADDRESS2, m_Address2);
	DDX_Text(pDX, IDC_CITY, m_City);
	DDX_Text(pDX, IDC_COMPANY, m_Company);
	DDX_Text(pDX, IDC_EMAIL2, m_Email2);
	DDX_Text(pDX, IDC_EXT1, m_Ext1);
	DDX_Text(pDX, IDC_EXT2, m_Ext2);
	DDX_Text(pDX, IDC_FIRSTNAME, m_FirstName);
	DDX_Text(pDX, IDC_LASTNAME, m_LastName);
	DDX_Text(pDX, IDC_MIDDLENAME, m_MiddleName);
	DDX_Text(pDX, IDC_PHONE1, m_Phone1);
	DDX_Text(pDX, IDC_PHONE2, m_Phone2);
	DDX_Text(pDX, IDC_POSTALCODE, m_PostalCode);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSampleProfileDataDlg, CDialog)
	//{{AFX_MSG_MAP(CSampleProfileDataDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDSETPROFILE, OnSetProfile)
	ON_BN_CLICKED(IDGETPROFILE, OnGetProfile)
	ON_BN_CLICKED(IDCREATEACCOUNT, OnCreateAccount)
	ON_BN_CLICKED(IDLOGIN, OnLogin)
	ON_EN_CHANGE(IDC_USERNAME, OnChangeUsername)
	ON_EN_CHANGE(IDC_PASSWORD, OnChangePassword)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSampleProfileDataDlg message handlers

BOOL CSampleProfileDataDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Popup the status modeless dialog box
	CMsgDialog aMsgDialog;
	aMsgDialog.m_Message = "Initializing...";
	aMsgDialog.Create(IDD_MSGDIALOG);

	// Disable the buttons
	m_ButtonCreateAccount.EnableWindow(FALSE);
	m_ButtonGetProfile.EnableWindow(FALSE);
	m_ButtonSetProfile.EnableWindow(FALSE);
	m_ButtonLogin.EnableWindow(FALSE);

	// Set login state
	m_bLoggedIn = false;

	// Initialize the directory context
	m_DirContext = new ServerContext();
	//m_DirContext->SetNeedShuffle(TRUE);
	m_DirContext->AddAddress( IPAddr("wontest.west.won.net",15101) );
	m_DirContext->AddAddress( IPAddr("wontest.east.won.net",15101) );
	m_DirContext->AddAddress( IPAddr("wontest.central.won.net",15101) );

	// Load the auth servers
	aMsgDialog.m_Message = "Retrieving Auth servers...";
	GetDirOpPtr aGetDirOp = new GetDirOp(m_DirContext);
	aGetDirOp->SetPath( DIR_TITANSERVERS );
	aGetDirOp->SetFlags( DIR_GF_DECOMPRECURSIVE |
						 DIR_GF_SERVADDNAME |
						 DIR_GF_DECOMPSERVICES | 
						 DIR_GF_SERVADDNETADDR );

	WONStatus result = aGetDirOp->Run(OP_MODE_BLOCK, OP_TIMEOUT_INFINITE);

	if (result != WS_Success)
	{
		MessageBox("Fail to retrieve Authentication server list.", "ERROR", MB_ICONEXCLAMATION);
		PostQuitMessage(0);
		return TRUE;
	}

	// Create the auth and profile contexts
	m_AuthContext = new AuthContext();
	m_ProfileContext = new ServerContext();

	// Parse through the list for auth servers
	m_AuthContext->GetServerContext()->AddAddressesFromDir(aGetDirOp->GetDirEntityList(), AUTH_SERVER_STRING);
	
	// Parse through the list for contest servers
	m_ProfileContext->AddAddressesFromDir(aGetDirOp->GetDirEntityList(), PROFILE_SERVER_STRING);

	// Load Country values into the dialog
	ProfileCountryMap::const_iterator aCountryMapItr = ProfileData::GetCountryMap().begin();
	for (; aCountryMapItr != ProfileData::GetCountryMap().end(); ++aCountryMapItr)
	{
		m_ListCountry.AddString( aCountryMapItr->second.c_str() );
		m_ListCountry.SetItemData(m_ListCountry.GetCount()-1, aCountryMapItr->first);
	}

	// Load Age Range values into the dialog
	ProfileAgeRangeMap::const_iterator anAgeRangeMapItr = ProfileData::GetAgeRangeMap().begin();
	for (; anAgeRangeMapItr != ProfileData::GetAgeRangeMap().end(); ++anAgeRangeMapItr)
	{
		m_ListAgeRange.AddString( anAgeRangeMapItr->second.c_str() );
		m_ListAgeRange.SetItemData(m_ListAgeRange.GetCount()-1, anAgeRangeMapItr->first);
	}

	// Load State values into the dialog
	ProfileStateAbbrevMap::const_iterator aStateAbbrevMapItr = ProfileData::GetStateAbbrevMap().begin();
	for (; aStateAbbrevMapItr != ProfileData::GetStateAbbrevMap().end(); ++aStateAbbrevMapItr)
	{
		m_ListState.AddString( aStateAbbrevMapItr->second.c_str() );
		m_ListState.SetItemDataPtr(m_ListState.GetCount()-1, (void*)aStateAbbrevMapItr->first.c_str());
	}

	// Kill the message dialog box
	aMsgDialog.DestroyWindow();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSampleProfileDataDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSampleProfileDataDlg::OnPaint() 
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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSampleProfileDataDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSampleProfileDataDlg::OnSetProfile() 
{
	// Load the dialog data into the member variables
	UpdateData(TRUE);

	// Create a message window
	CMsgDialog aMsgDialog;
	aMsgDialog.m_Message = "Storing profile data...";
	aMsgDialog.Create(IDD_MSGDIALOG);

	// Construct the set profile op
	SetProfileOpPtr theSetProfileOp = new SetProfileOp(m_ProfileContext);
	theSetProfileOp->SetAuthType(AUTH_TYPE_SESSION, m_AuthContext);
	
	// Create the profile data structure from the dialog data
	ProfileDataPtr theProfileDataP = new ProfileData;
	theProfileDataP->Set(ProfileField_FirstName, StringToWString((LPCSTR)m_FirstName));
	theProfileDataP->Set(ProfileField_MiddleName, StringToWString((LPCSTR)m_MiddleName));
	theProfileDataP->Set(ProfileField_LastName, StringToWString((LPCSTR)m_LastName));
	theProfileDataP->Set(ProfileField_CompanyName, StringToWString((LPCSTR)m_Company));
	theProfileDataP->Set(ProfileField_Address1, StringToWString((LPCSTR)m_Address1));
	theProfileDataP->Set(ProfileField_Address2, StringToWString((LPCSTR)m_Address2));
	theProfileDataP->Set(ProfileField_City, StringToWString((LPCSTR)m_City));
	
	theProfileDataP->Set(ProfileField_Phone1, StringToWString((LPCSTR)m_Phone1));
	theProfileDataP->Set(ProfileField_Phone1Ext, StringToWString((LPCSTR)m_Ext1));
	theProfileDataP->Set(ProfileField_Phone2, StringToWString((LPCSTR)m_Phone2));
	theProfileDataP->Set(ProfileField_Phone2Ext, StringToWString((LPCSTR)m_Ext2));

	theProfileDataP->Set(ProfileField_PostalCode, StringToWString((LPCSTR)m_PostalCode));
	theProfileDataP->Set(ProfileField_Email1, StringToWString((LPCSTR)m_Email));
	theProfileDataP->Set(ProfileField_Email2, StringToWString((LPCSTR)m_Email2));

	// Gender
	if (GetCheckedRadioButton(IDC_MALE,IDC_FEMALE) == IDC_MALE)
		theProfileDataP->SetGender(WONAPI::ProfileGender_Male);
	else
		theProfileDataP->SetGender(WONAPI::ProfileGender_Female);

	// State
	wchar_t* theStateAbbrevP = (wchar_t*)m_ListState.GetItemDataPtr(m_ListState.GetCurSel());
	if (theStateAbbrevP != NULL)
		theProfileDataP->SetState( theStateAbbrevP );
		
	// AgeRange
	unsigned short theAgeRangeCode = m_ListAgeRange.GetItemData(m_ListAgeRange.GetCurSel());
	if (theProfileDataP->GetAgeRangeMap().find(theAgeRangeCode) != NULL)
		theProfileDataP->SetAgeRange( theAgeRangeCode );

	// Country
	unsigned short theCountryCode = m_ListCountry.GetItemData(m_ListCountry.GetCurSel());
	if (theCountryCode != CB_ERR)
		theProfileDataP->SetCountry( theCountryCode );


	// Store the profile data into the op
	theSetProfileOp->SetProfileData(theProfileDataP);

	// Run the Op
	WONStatus result = theSetProfileOp->Run(OP_MODE_BLOCK,OP_TIMEOUT_INFINITE);

	switch(result)
	{
	case WS_Success:
		break;
	default:
		MessageBox("Failed to Set Profile","ERROR", MB_ICONEXCLAMATION);
	}

	// Destroy the message window
	aMsgDialog.DestroyWindow();


}

void CSampleProfileDataDlg::OnGetProfile() 
{
	// Retrieve the dialog data so far
	UpdateData(TRUE);

	// Create a message window
	CMsgDialog aMsgDialog;
	aMsgDialog.m_Message = "Loading profile data...";
	aMsgDialog.Create(IDD_MSGDIALOG);

	// Construct the get profile op
	GetProfileOpPtr theGetProfileOp = new GetProfileOp(m_ProfileContext);
	theGetProfileOp->SetAuthType(AUTH_TYPE_SESSION, m_AuthContext);
	theGetProfileOp->AddAllFields();

	WONStatus result = theGetProfileOp->Run(OP_MODE_BLOCK,OP_TIMEOUT_INFINITE);
	
	if (result == WS_Success)
	{
		// Get the profile data
		ProfileData* theProfileDataP = theGetProfileOp->GetProfileData();

		// Update the dialog box from the profile data
		m_FirstName		= WStringToString(theProfileDataP->GetWString(ProfileField_FirstName)).c_str();
		m_MiddleName	= WStringToString(theProfileDataP->GetWString(ProfileField_MiddleName)).c_str();
		m_LastName		= WStringToString(theProfileDataP->GetWString(ProfileField_LastName)).c_str();
		m_Company		= WStringToString(theProfileDataP->GetWString(ProfileField_CompanyName)).c_str();
		m_Address1		= WStringToString(theProfileDataP->GetWString(ProfileField_Address1)).c_str();
		m_Address2		= WStringToString(theProfileDataP->GetWString(ProfileField_Address2)).c_str();
		m_City			= WStringToString(theProfileDataP->GetWString(ProfileField_City)).c_str();
		m_Phone1		= WStringToString(theProfileDataP->GetWString(ProfileField_Phone1)).c_str();
		m_Ext1			= WStringToString(theProfileDataP->GetWString(ProfileField_Phone1Ext)).c_str();
		m_Phone2		= WStringToString(theProfileDataP->GetWString(ProfileField_Phone2)).c_str();
		m_Ext2			= WStringToString(theProfileDataP->GetWString(ProfileField_Phone2Ext)).c_str();
		m_PostalCode	= WStringToString(theProfileDataP->GetWString(ProfileField_PostalCode)).c_str();
		m_Email			= WStringToString(theProfileDataP->GetWString(ProfileField_Email1)).c_str();
		m_Email2		= WStringToString(theProfileDataP->GetWString(ProfileField_Email2)).c_str();

		// Gender
		if (theProfileDataP->GetGender() == WONAPI::ProfileGender_Female)
			CheckRadioButton(IDC_MALE,IDC_FEMALE,IDC_FEMALE);
		else
			CheckRadioButton(IDC_MALE,IDC_FEMALE,IDC_MALE);

		//State
		ProfileStateAbbrevMap theStateAbbrevMap = theProfileDataP->GetStateAbbrevMap();
		std::wstring theStateAbbrev = theProfileDataP->GetStateAbbrev();

		if (!theStateAbbrev.empty())
			m_ListState.SelectString(0, theStateAbbrevMap[theStateAbbrev].c_str());
		else
			m_ListState.SetCurSel(-1);

		//AgeRange
		if (theProfileDataP->GetAgeRange() != 0)
			m_ListAgeRange.SetCurSel(m_ListAgeRange.FindString(0, theProfileDataP->GetAgeRangeMap().find(theProfileDataP->GetAgeRange())->second.c_str() ));
		else
			m_ListAgeRange.SetCurSel(-1);

		//Country
		ProfileCountryMap theCountryCodeMap = theProfileDataP->GetCountryMap();
		unsigned short theCountryCode = theProfileDataP->GetCountry();
		m_ListCountry.SelectString(0, theCountryCodeMap.find(theCountryCode)->second.c_str() );

		
		// Display the new values in the dialog box
		UpdateData(FALSE);

	}
	else
		MessageBox("Failed to get profile data.", "ERROR", MB_ICONEXCLAMATION);

	// Destroy the message window
	aMsgDialog.DestroyWindow();
}

void CSampleProfileDataDlg::OnCreateAccount() 
{
	// Load the dialog data
	UpdateData(TRUE);

	// Create a message window
	CMsgDialog aMsgDialog;
	aMsgDialog.m_Message = "Creating account...";
	aMsgDialog.Create(IDD_MSGDIALOG);

	// Construct the Op
	CreateAccountOpPtr aCreateAccountOp = new CreateAccountOp(m_ProfileContext);
	aCreateAccountOp->SetPassword(StringToWString(LPCSTR(m_Password)));
	aCreateAccountOp->SetUserName(StringToWString(LPCSTR(m_UserName)));

	WONStatus result = aCreateAccountOp->Run(OP_MODE_BLOCK,OP_TIMEOUT_INFINITE);

	switch(result)
	{
	case WS_Success:
		MessageBox("Account created.", "ERROR", MB_ICONEXCLAMATION);
		break;
	case WS_CommServ_AlreadyExists:
		MessageBox("Account already exists.", "ERROR", MB_ICONEXCLAMATION);
		break;
	case WS_CommServ_NotAllowed:
		MessageBox("Account creation not allowed.", "ERROR", MB_ICONEXCLAMATION);
		break;
	case WS_CommServ_InvalidParameters:
		MessageBox("Invalid Parameters.", "ERROR", MB_ICONEXCLAMATION);
		break;
	default:
		MessageBox("Failed to create account.", "ERROR", MB_ICONEXCLAMATION);
	}

	// Destroy the message window
	aMsgDialog.DestroyWindow();
}

void CSampleProfileDataDlg::OnLogin() 
{
	// Retrieve the dialog data so far
	UpdateData(TRUE);

	// Create a message window
	CMsgDialog aMsgDialog;
	aMsgDialog.m_Message = "Authenticating...";
	aMsgDialog.Create(IDD_MSGDIALOG);

	// Convert username and password to widestring
	// Store user info the auth context
	m_AuthContext->SetUserName( StringToWString(LPCSTR(m_UserName)) );
	m_AuthContext->SetPassword( StringToWString(LPCSTR(m_Password)) );
	m_AuthContext->AddCommunity( COMMUNITY_SAMPLE );		// Specify a community to login to

	// Construct the GetCertOp to get the authentication certificate
	GetCertOpPtr theGetCertOp = new GetCertOp(m_AuthContext);

	WONStatus result = theGetCertOp->Run(OP_MODE_BLOCK, OP_TIMEOUT_INFINITE);

	switch(result)
	{
	case WS_Success:
		{
		m_bLoggedIn = true;

		// Update the window caption
		CString wndtext = "SampleProfileData -"; wndtext += m_UserName;
		SetWindowText(wndtext);
	
		m_ButtonSetProfile.EnableWindow(TRUE);
		m_ButtonGetProfile.EnableWindow(TRUE);
		break;
		}
	case WS_AuthServ_InvalidUserName:
	case WS_AuthServ_BadPassword:
		MessageBox("Invalid UserName/Password combo.", "ERROR", MB_ICONEXCLAMATION);
		break;
	default:
		MessageBox("Failed to authenticate.", "ERROR", MB_ICONEXCLAMATION);
	}

	// Destroy the message dialog
	aMsgDialog.DestroyWindow();
}

void CSampleProfileDataDlg::OnChangeUsername() 
{
	// Retrieve the dialog data so far
	UpdateData(TRUE);

	// If both the Login and Password 
	if (m_UserName.GetLength() > 0 && m_Password.GetLength() > 0)
	{
		m_ButtonLogin.EnableWindow(TRUE);
		m_ButtonCreateAccount.EnableWindow(TRUE);
	} else
	{
		m_ButtonLogin.EnableWindow(FALSE);
		m_ButtonCreateAccount.EnableWindow(FALSE);
	}
}

void CSampleProfileDataDlg::OnChangePassword() 
{
	// Retrieve the dialog data so far
	UpdateData(TRUE);

	// If both the Login and Password 
	if (m_UserName.GetLength() > 0 && m_Password.GetLength() > 0)
	{
		m_ButtonLogin.EnableWindow(TRUE);
		m_ButtonCreateAccount.EnableWindow(TRUE);
	} else
	{
		m_ButtonLogin.EnableWindow(FALSE);
		m_ButtonCreateAccount.EnableWindow(FALSE);
	}
}
