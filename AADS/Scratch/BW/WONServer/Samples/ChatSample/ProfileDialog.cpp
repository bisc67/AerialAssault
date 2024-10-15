// ProfileDialog.cpp : implementation file
//

#include "stdafx.h"
#include "chatsample.h"
#include "CancelDialog.h"
#include "ProfileDialog.h"
#include "WONDB/GetProfileOp.h"
#include "WONDB/SetProfileOp.h"
#include "WONDB/ProfileData.h"
#include "WONCommon/StringUtil.h"
#include "WONCommon/Completion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace WONAPI;
/////////////////////////////////////////////////////////////////////////////
// ProfileDialog dialog


ProfileDialog::ProfileDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ProfileDialog::IDD, pParent)
{
}

ProfileDialog::ProfileDialog(AuthContextPtr& theAuthContext, ServerContextPtr& theProfileContext)
	: CDialog(ProfileDialog::IDD, NULL),
	  mAuthContext(theAuthContext),
	  mProfileContext(theProfileContext)
{
	//{{AFX_DATA_INIT(ProfileDialog)
	m_Address1 = _T("");
	m_Address2 = _T("");
	m_City = _T("");
	m_Company = _T("");
	m_Email = _T("");
	m_Email2 = _T("");
	m_Ext1 = _T("");
	m_Ext2 = _T("");
	m_FirstName = _T("");
	m_LastName = _T("");
	m_MiddleName = _T("");
	m_Password = WStringToString(mAuthContext->GetPassword()).c_str();
	m_Phone1 = _T("");
	m_Phone2 = _T("");
	m_PostalCode = _T("");
	m_UserName = WStringToString(mAuthContext->GetUserName()).c_str();
	//}}AFX_DATA_INIT
}

void ProfileDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ProfileDialog)
	DDX_Control(pDX, IDC_STATE, m_State);
	DDX_Control(pDX, IDC_AGERANGE, m_AgeRange);
	DDX_Control(pDX, IDC_COUNTRY, m_Country);
	DDX_Text(pDX, IDC_ADDRESS1, m_Address1);
	DDX_Text(pDX, IDC_ADDRESS2, m_Address2);
	DDX_Text(pDX, IDC_CITY, m_City);
	DDX_Text(pDX, IDC_COMPANY, m_Company);
	DDX_Text(pDX, IDC_EMAIL, m_Email);
	DDX_Text(pDX, IDC_EMAIL2, m_Email2);
	DDX_Text(pDX, IDC_EXT1, m_Ext1);
	DDX_Text(pDX, IDC_EXT2, m_Ext2);
	DDX_Text(pDX, IDC_FIRSTNAME, m_FirstName);
	DDX_Text(pDX, IDC_LASTNAME, m_LastName);
	DDX_Text(pDX, IDC_MIDDLENAME, m_MiddleName);
	DDX_Text(pDX, IDC_PASSWORD, m_Password);
	DDX_Text(pDX, IDC_PHONE1, m_Phone1);
	DDX_Text(pDX, IDC_PHONE2, m_Phone2);
	DDX_Text(pDX, IDC_POSTALCODE, m_PostalCode);
	DDX_Text(pDX, IDC_USERNAME, m_UserName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ProfileDialog, CDialog)
	//{{AFX_MSG_MAP(ProfileDialog)
	ON_BN_CLICKED(IDGETPROFILE, OnGetProfile)
	ON_BN_CLICKED(IDSETPROFILE, OnSetProfile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ProfileDialog message handlers

void ProfileDialog::OnGetProfile() 
{
	// Construct the get profile op
	GetProfileOpPtr theGetProfileOp = new GetProfileOp(mProfileContext);
	theGetProfileOp->SetAuthType(AUTH_TYPE_SESSION, mAuthContext);
	theGetProfileOp->AddAllFields();
	theGetProfileOp->SetCompletion(new ParamCompletion<AsyncOpPtr,ProfileDialog*>(GetProfileCompletion,this));

	// Construct the message dialog
	mGetProfileDialog = new CancelDialog;
	mGetProfileDialog->m_Caption = "Get Profile";
	mGetProfileDialog->m_Message = "Retrieving profile...";
	
	// Run the op
	theGetProfileOp->RunAsync(OP_TIMEOUT_INFINITE);

	// Display the cancel dialog box
	if (mGetProfileDialog->DoModal() == IDCANCEL)
		theGetProfileOp->Kill();

	delete mGetProfileDialog;
}

void ProfileDialog::OnSetProfile() 
{
	// Load the dialog data into the member variables
	UpdateData(TRUE);

	// Construct the set profile op
	SetProfileOpPtr theSetProfileOp = new SetProfileOp(mProfileContext);
	theSetProfileOp->SetAuthType(AUTH_TYPE_SESSION, mAuthContext);

	// Construct the message dialog
	mSetProfileDialog = new CancelDialog;
	mSetProfileDialog->m_Caption = "Set Profile";
	mSetProfileDialog->m_Message = "Setting profile...";

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
	if (GetCheckedRadioButton(IDC_FEMALE,IDC_MALE) == IDC_MALE)
		theProfileDataP->SetGender(WONAPI::ProfileGender_Male);
	else
		theProfileDataP->SetGender(WONAPI::ProfileGender_Female);

	// State
	wchar_t* theStateAbbrevP = (wchar_t*)m_State.GetItemDataPtr(m_State.GetCurSel());
	if (theStateAbbrevP != NULL)
		theProfileDataP->SetState( theStateAbbrevP );
		
	// AgeRange
	unsigned short theAgeRangeCode = m_AgeRange.GetItemData(m_AgeRange.GetCurSel());
	if (theProfileDataP->GetAgeRangeMap().find(theAgeRangeCode) != NULL)
		theProfileDataP->SetAgeRange( theAgeRangeCode );

	// Country
	unsigned short theCountryCode = m_Country.GetItemData(m_Country.GetCurSel());
	if (theCountryCode != CB_ERR)
		theProfileDataP->SetCountry( theCountryCode );


	// Store the profile data into the op
	theSetProfileOp->SetProfileData(theProfileDataP);

	// Set the completion
	theSetProfileOp->SetCompletion(new ParamCompletion<AsyncOpPtr,ProfileDialog*>(SetProfileCompletion,this));

	// Run the Op
	theSetProfileOp->RunAsync(OP_TIMEOUT_INFINITE);

	if (mSetProfileDialog->DoModal() != IDOK)
		theSetProfileOp->Kill();

	delete mSetProfileDialog;

}

BOOL ProfileDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Load Country values into the dialog
	ProfileCountryMap::const_iterator aCountryMapItr = ProfileData::GetCountryMap().begin();
	for (; aCountryMapItr != ProfileData::GetCountryMap().end(); ++aCountryMapItr)
	{
		m_Country.AddString( aCountryMapItr->second.c_str() );
		m_Country.SetItemData(m_Country.GetCount()-1, aCountryMapItr->first);
	}

	// Load Age Range values into the dialog
	ProfileAgeRangeMap::const_iterator anAgeRangeMapItr = ProfileData::GetAgeRangeMap().begin();
	for (; anAgeRangeMapItr != ProfileData::GetAgeRangeMap().end(); ++anAgeRangeMapItr)
	{
		m_AgeRange.AddString( anAgeRangeMapItr->second.c_str() );
		m_AgeRange.SetItemData(m_AgeRange.GetCount()-1, anAgeRangeMapItr->first);
	}

	// Load State values into the dialog
	ProfileStateAbbrevMap::const_iterator aStateAbbrevMapItr = ProfileData::GetStateAbbrevMap().begin();
	for (; aStateAbbrevMapItr != ProfileData::GetStateAbbrevMap().end(); ++aStateAbbrevMapItr)
	{
		m_State.AddString( aStateAbbrevMapItr->second.c_str() );
		m_State.SetItemDataPtr(m_State.GetCount()-1, (void*)aStateAbbrevMapItr->first.c_str());
	}

	OnGetProfile();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void ProfileDialog::GetProfileCompletion(AsyncOpPtr theOp, ProfileDialog* pThat)
{
	if (theOp->GetStatus()==WS_Killed)
		return;
	
	pThat->mGetProfileDialog->EndDialog(IDOK);
	pThat->GetProfileCompletion((GetProfileOp*)theOp.get());
}

void ProfileDialog::GetProfileCompletion(GetProfileOp* theOp)
{
	switch(theOp->GetStatus())
	{
	case WS_Success:
		{
		// Get the profile data
		ProfileData* theProfileDataP = theOp->GetProfileData();

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
			CheckRadioButton(IDC_FEMALE,IDC_MALE,IDC_FEMALE);
		else
			CheckRadioButton(IDC_FEMALE,IDC_MALE,IDC_MALE);

		//State
		ProfileStateAbbrevMap theStateAbbrevMap = theProfileDataP->GetStateAbbrevMap();
		std::wstring theStateAbbrev = theProfileDataP->GetStateAbbrev();

		if (!theStateAbbrev.empty())
			m_State.SelectString(0, theStateAbbrevMap[theStateAbbrev].c_str());
		else
			m_State.SetCurSel(-1);

		//AgeRange
		if (theProfileDataP->GetAgeRange() != 0)
			m_AgeRange.SetCurSel(m_AgeRange.FindString(0, theProfileDataP->GetAgeRangeMap().find(theProfileDataP->GetAgeRange())->second.c_str() ));
		else
			m_AgeRange.SetCurSel(-1);

		//Country
		ProfileCountryMap theCountryCodeMap = theProfileDataP->GetCountryMap();
		unsigned short theCountryCode = theProfileDataP->GetCountry();
		m_Country.SelectString(0, theCountryCodeMap.find(theCountryCode)->second.c_str() );
		
		// Display the new values in the dialog box
		UpdateData(FALSE);

		}
	case WS_Killed:
		return;
	default:
		MessageBox("Failed to get profile data.", "ERROR", MB_ICONEXCLAMATION);	
	}


}

void ProfileDialog::SetProfileCompletion(AsyncOpPtr theOp, ProfileDialog* pThat)
{
	if (theOp->GetStatus()==WS_Killed)
		return;

	pThat->mSetProfileDialog->EndDialog(IDOK);
	pThat->SetProfileCompletion((SetProfileOp*)theOp.get());
}

void ProfileDialog::SetProfileCompletion(SetProfileOp* theOp)
{
	if (theOp->GetStatus()!=WS_Success)
		MessageBox("Failed to Set Profile","ERROR", MB_ICONEXCLAMATION);
}

