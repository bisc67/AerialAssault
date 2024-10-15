
#include <string>
#include "HTTPGetReq.h"
#include "NetCommInterface.h"

using namespace WONAPI;
using namespace std;

////////////////////////////////////////////////////////////////////
// Public methods

NetCommInterface::NetCommInterface() :
	mHTTPGetReqP(NULL)
{}
	
NetCommInterface::~NetCommInterface()
{
	delete mHTTPGetReqP;
}

// Here we provide a default implimentation of HTTPGet for products that do not want to implement it.
bool
NetCommInterface::HTTPGet(	const char* theServer,							
							unsigned short thePort,							
							const char* theServerPath,						
							const char* theDestPath,						
							long theRequestTimeout,							
							NetCommCallback theNetCommCallbackP,				
							void* theCallbackDataP) 
{
	if (mHTTPGetReqP == NULL)
	{
		// This is dynamically allocated because we may not need one of these
		mHTTPGetReqP = new HTTPGetReq(this);
	}

	return mHTTPGetReqP->HTTPGet(theServer, thePort, theServerPath, theDestPath, theRequestTimeout, theNetCommCallbackP, theCallbackDataP);
}