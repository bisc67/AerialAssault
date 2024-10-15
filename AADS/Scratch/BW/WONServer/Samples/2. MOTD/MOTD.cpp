
/****************************************************************************
 * The starting point for all WON enabled products is the creation of the	*
 * WONAPICore object. This object also creates the required message threads.*
 ****************************************************************************/


/* Before you begin...
		- Include the path for the WONAPI header files. (../../)
		- Be sure you are linking to the multithreaded run time libraries.
		- Link to either wsock32.lib or ws2_32.lib.
*/

#include "WONAPI.h"								// API namespace. (WONAPICore defined)
#include "WONMisc/GetMOTDOp.h"					// Message of the Day Operation

using namespace WONAPI;

/*
	The WONAPICoreEx class spawns and controls any threads required by the WON API and
	manages API operations. 
*/
WONAPICoreEx gAPI;

#define PRODUCT_STRING			"Api2Sample"

int main()
{
	// Start the API.  It will automatically shutdown when the WONAPICoreEx destructs.
	gAPI.Startup();

	// We recommend that a product retrieve and display the MOTD (message of the day) before doing
	//	any other WONAPI processing. That way, in the event of a major issue, the players could be
	//	informed regardless of login or patching availability. The MOTD is specific for each game.
	//	(See api documentation for full desciption)
	
	// 1) Construct the MOTD Operation as a smart pointer. (passing the product name)
	GetMOTDOpPtr theMOTDOp = new GetMOTDOp( PRODUCT_STRING );			// will autodelete 

	// 2) Run the op. (sync calls wait until the op. completes)
	WONStatus result = theMOTDOp->Run(OP_MODE_BLOCK, OP_TIMEOUT_INFINITE);

	if (result != WS_Success)
	{
		// Your app may need more descriptive error handling
		printf("Could not retrieve MOTD: %s.\r\n",WONStatusToString(result));
		return 0;
	}

	// 3) Retrieve the MOTDs from the completed Op
	ByteBufferPtr sysMOTD = theMOTDOp->GetSysMOTD();
	ByteBufferPtr gameMOTD = theMOTDOp->GetGameMOTD();

	// 4) Display the MOTDs (trailing nulls are automatically appended by the GetMOTD() calls)
	// Note: These .txt files are also stored in the local path as _wonSamplemotd.txt and _wonsysmotd.txt
	// They are retrieved via an HTTP Get command.
	printf("%s", sysMOTD->data());
	printf("%s", gameMOTD->data());

	// Note:  If you want to check if the motd has changed since the last time you retrieved it,
	// then call theMOTDOp->SysMOTDIsNew() and theMOTDOp->GameMOTDIsNew().  The API knows whether
	// the motds are new because it stores a copy of the file and does an HTTP If/Get request
	// when getting them.  The webserver then tells them if there is a new motd available.  
	//
	// To control where the motd files are stored call theMOTDOp->SetGameMOTDPath() and 
	// theMOTDOp->SetSysMOTDPath().  The path should include the file name.  
	//
	// The API uses the first character of the MOTD to determine whether it should be displayed 
	// once or always. If the character is an 'A' then the API will always report that the MOTD 
	// is new.  If the character is a '1' then it will only report that the MOTD is new when it 
	// is actually new.  This first character is stripped from the motd in the calls to 
	// GetSysMOTD and GetGameMOTD.

	return 0;
}
