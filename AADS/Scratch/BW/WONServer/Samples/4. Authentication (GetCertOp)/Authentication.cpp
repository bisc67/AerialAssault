
/****************************************************************************
 * The starting point for all WON enabled products is the creation of the	*
 * WONAPICore object. This object also creates the required message threads.*
 ****************************************************************************/


/* Before you begin...
		- Include the path for, and link to, the WONAPI.lib. (../../debug)
		- Include the path for the WONAPI header files. (../../)
		- Be sure you are linking to the multithreaded run time libraries.
		- Link to either wsock32.lib or ws2_32.lib.
		- If you are going to be using platform specific code be sure to 
			include	WONCommon/Platform.h.
*/


#include "WONCommon/Platform.h"					// Platform specfic headers
#include "WONApi.h"								// API namespace. (WONAPICore defined)
#include "WONDir/GetDirOp.h"					// Get Directory Operation
#include "WONCommon/SmartPtr.h"					// Threadsafe Auto-deleting pointers (GetMOTDOpPtr)
#include "WONServer/ServerContext.h"			// Class for communicating with a Titan server
#include "WONAuth/AuthContext.h"				// ServerContext for the auth servers
#include "WONAuth/GetCertOp.h"					// Get Authenication Certificate Operation

#pragma warning(disable: 4786)					// STL bug gives off warning 4786
#include <string.h>								// STL basic string class

using namespace WONAPI;

// STATICS (server lists as globals for simplicity)
WONAPICore		 apiCore(true);					// Required by WONAPI
ServerContextPtr gDirContext;					// Used to speak to the dir server
AuthContextPtr	 gAuthContext;					//		"	"	 to the auth servers
DirEntityList	 gDirectoryContents;			// Stores the contents of the directory
const char*		PRODUCT_STRING		=	"Sample";					// Product string for CDKey Authentication
const wchar_t*	DIR_GAME			=	L"/API2Sample";				// Game directory. (/GameName)
const wchar_t*	DIR_AUTHSERVERS		=	L"/TitanServers/Auth";		// Directory for Authentication servers
const wchar_t*	AUTH_SERVER			=	L"AuthServer";				// Authenticates user information
const wchar_t*	GAME_SERVER			=	L"TitanRoutingServers";		// Game server. Runs on basic routing server.
const wchar_t*	COMMUNITY_SAMPLE	=	L"WON";
wchar_t gUserName[255];						// User name for Authentication
wchar_t gPassword[255];						// Password for Authentication
char gCDKeyString[255];						// CD Key for Authentication

// PROTOTYPES
bool LoadDirServerAddrs();						// Loads the DirServer addrs into the context
bool GetTitanServices();						// Retrieves the Auth and Firewall addrs
bool AuthenticateUser();						// Gets user info and validates with Auth servers
bool AuthenticateUserWithCDKey();				// Authenticates the user with a CD Key

//******************************************************************************
// Entry point
//******************************************************************************
int main()
{
	// The WON Directory Server acts much like a yellow pages, providing a categorized listings of 
	//	available services (servers). It maintains a hierarchical structure of directory entities, 
	//	much like a file system. Services may be added in any existing directory. A given directory 
	//	may contain any number of services and any number of subdirectories. Every service and 
	//	subdirectory must be unique within a given directory. 
	//
	//	(See api documentation for full desciption)
	gDirContext			= new ServerContext();			// Auto-deleting smart pointer (see above for var desc.)
	gAuthContext		= new AuthContext();			// Auto-deleting smart pointer (see above for var desc.)

	// The first thing to do is to obtain the IP addresses of the dir servers..
	// DirServer IP addresses should be loaded from an outside, modifiable source. (such as a config file)
	if (!LoadDirServerAddrs())
		return -1;//error

	// Now that the IP of a dir server is known, it can be queried for 
	// other services using a GetDirOp
	if (!GetTitanServices())							// just getting Auth servers in this sample
		return -1;//error

	// Authentication:
	//   For clarity I am not validating the input. (Whitespace, valid characters etc) 
	printf("UserName: "); _getws(gUserName);		// Get the username
	printf("Password: "); _getws(gPassword);		// Get the password

	// Store in the auth context
	gAuthContext->SetUserName( gUserName );			// Give the user name to the AuthContext
	gAuthContext->SetPassword( gPassword );			// Give the password to the AuthContext
	gAuthContext->AddCommunity( COMMUNITY_SAMPLE );		// Specify a community to login to

	bool bUseCDKey = false;							// Set to true to use CD Key value
	if (bUseCDKey)
	{
		// Authenticate the user with a CD Key but no CRC checking
		if (!AuthenticateUserWithCDKey())
			return -1;//error
	}
	else
	{
		// Authenticate the user (no CD Key validation or CRC checking)
		if (!AuthenticateUser())
			return -1;//error
	}

	// Success
	return 0;

}


//******************************************************************************
// Loads the directory server addresses
//******************************************************************************
bool LoadDirServerAddrs()
{
	// I'm just hardcoding them in for simplicity
	gDirContext->AddAddress( IPAddr("wontest.west.won.net",15101) );
	gDirContext->AddAddress( IPAddr("wontest.east.won.net",15101) );
	gDirContext->AddAddress( IPAddr("wontest.central.won.net",15101) );

	return true;
}


//******************************************************************************
//Authenticate the user (without cd key)
//******************************************************************************
bool AuthenticateUser()
{
	// Construct the GetCertOp to get the authentication certificate
	GetCertOpPtr theGetCertOp = new GetCertOp(gAuthContext);
	
	WONStatus result = theGetCertOp->Run(OP_MODE_BLOCK, OP_TIMEOUT_INFINITE);

	switch(result)
	{
	case WS_Success:
		{
		printf("\r\nLogged in as %S.\r\n", gUserName);
		return true;
		}
		//anything else
	case WS_AuthServ_BadCommunity:			// invalid community (case sensitive)
	case WS_AuthServ_BadPassword:			// incorrect password
	case WS_AuthServ_UserNotFound:			// user name does not exist
	default:
		return false;
	}

}


//******************************************************************************
//Authenticate the user (with cd key)
//******************************************************************************
bool AuthenticateUserWithCDKey()
{
	// Get the CD Key
	printf("\r\nCDKey for Authentication: "); gets(gCDKeyString);

	// Construct a CD Key object
	CDKey aCDKey( PRODUCT_STRING );
	aCDKey.Init( gCDKeyString );

	// Set the CD Key into the Auth Context
	gAuthContext->SetCDKey( COMMUNITY_SAMPLE, aCDKey );

	// Construct the GetCertOp to get the authentication certificate
	GetCertOpPtr theGetCertOp = new GetCertOp(gAuthContext);
	WONStatus result = theGetCertOp->Run(OP_MODE_BLOCK, OP_TIMEOUT_INFINITE);

	// Since CD Key's are not required for the "Sample" community (server side switch)
	// any value for the CD Key string should authenticate successfully.
	switch(result)
	{
	case WS_Success:
		{
		printf("\r\nLogged in as %S.\r\n", gUserName);
		return true;
		}
		//anything else
	case WS_AuthServ_BadCommunity:			// invalid community (case sensitive)
	case WS_AuthServ_BadPassword:			// incorrect password
	case WS_AuthServ_UserNotFound:			// user name does not exist
	case WS_AuthServ_InvalidCDKey:			// CD Key is invalid
	case WS_AuthServ_CDKeyBanned:			// CD Key has been banned (reported stolen etc)
	case WS_AuthServ_CDKeyInUse:			// CD Key is already being used
	case WS_AuthServ_CRCFailed:				// CRC Check failed. (.exe has been modified)
	default:
		return false;
	}

	return false;
}


//******************************************************************************
// Retrieves the available Authentication servers
//******************************************************************************
bool GetTitanServices()
{
	// Construct the Get Directory Operation
	GetDirOpPtr theGetDirOp = new GetDirOp(gDirContext);// Auto deleting smart pointer
	theGetDirOp->SetPath( DIR_AUTHSERVERS );			// Directory for Authentication servers
	theGetDirOp->SetFlags(  DIR_GF_DECOMPSERVICES | 
							DIR_GF_SERVADDNETADDR );
	
	// Run the constructed OP synchronously (wait until finished)
	WONStatus result = theGetDirOp->Run(OP_MODE_BLOCK, OP_TIMEOUT_INFINITE);
	if (result != WS_Success)
	{
		printf("Could not access directory servers.\r\n");
		return false;//error
	}

	// Gain access to the retrieved directory listing
	gDirectoryContents = theGetDirOp->GetDirEntityList();

	// Parse through the list for auth servers (don't need contest or firewall)
	gAuthContext->GetServerContext()->AddAddressesFromDir(gDirectoryContents);

	return true;
}
