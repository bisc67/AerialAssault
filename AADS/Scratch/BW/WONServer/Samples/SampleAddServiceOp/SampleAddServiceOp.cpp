



// This is an example of registering services with the directory server and then 
// retrieving a list of registerd services.


#include "WONCommon/Platform.h"					// Platform specific headers
#include "WONAPI.h"								// API Core
#include "WONDir/GetDirOp.h"					// Get Directory Operation
#include "WONDir/AddEntityOp.h"					// AddEntity/AddService operations
#include "WONDir/RenewEntityOp.h"				// RenewEneity/RenewService operations
#include "WONCommon/SmartPtr.h"					// Threadsafe Auto-deleting pointers (GetMOTDOpPtr)
#include "WONServer/ServerContext.h"			// Class for communicating with a Titan server
#include "WONAuth/AuthContext.h"				// ServerContext for the auth servers

using namespace WONAPI;

// STATICS (server lists as globals for simplicity)
WONAPICore		 apiCore(true);					// Required by WONAPI
ServerContextPtr gDirContext;					// Used to hold the dir server list
const wchar_t*	DIR_GAME			=	L"/API2Sample";			// Game directory
const wchar_t*  DIR_AUTHSERVERS		=	L"/TitanServices/Auth";	// directory for auth servers
const wchar_t*	FAKESERVICE_NAME	=	L"Fake Server";			// a name for the fake servers
unsigned long SERVER_LIFESPAN		=	300;	// How long before the server needs a Keep Alive call (full seconds)
bool SERVER_SHUTDOWN				=	false;	// Signals fake servers to stop renewing the registrations
IPAddr gFakeServerIP("localhost",0);			// Default FakeServer IP (does not have to be valid)

// Prototypes
bool LoadDirServers();							// Loads the list of directory servers (hardcoded)
void StartFakeServer(void*);					// Spawn a fake server thread
bool ShowDirContents();							// Retrieves the list of registered services

//*****************************************************************************
// Starts up a thread that will create a dir registration and keep it alive
//*****************************************************************************
class FakeServer: public Thread					// Thread class declared in Thread.h
{
public:
	int m_nPort;

	// Override
	void ThreadFunc()
	{
		// Set the IPAddr for the fake server
		gFakeServerIP.SetThePort(m_nPort);

		// Construct the AddServiceOp to register a fake server with the directorys
		AddServiceOpPtr theAddServiceOp = new AddServiceOp(gDirContext);
		theAddServiceOp->SetLifespan(SERVER_LIFESPAN);
		theAddServiceOp->SetPath(DIR_GAME);
		theAddServiceOp->SetName(FAKESERVICE_NAME);
		theAddServiceOp->SetNetAddr(gFakeServerIP);

		if (theAddServiceOp->Run(OP_MODE_BLOCK,OP_TIMEOUT_INFINITE) != WS_Success)
			return;//error

		// Construct the RenewServiceOp for keep alive calls
		RenewServiceOpPtr theKeepAliveOp = new RenewServiceOp(gDirContext);
		theKeepAliveOp->SetLifespan(SERVER_LIFESPAN);
		theKeepAliveOp->SetPath(DIR_GAME);
		theKeepAliveOp->SetName(FAKESERVICE_NAME);
		theKeepAliveOp->SetNetAddr(gFakeServerIP);

		// Keep alive until SERVER_SHUTDOWN is set
		while (!mStopped) 
		{
			// Sleep for about 1/5th the lifespan. (convert seconds to milliseconds)
			mSignalEvent.WaitFor(SERVER_LIFESPAN *200);

			// Send a keep alive message
			if (!mStopped)
				theKeepAliveOp->Run(OP_MODE_BLOCK, OP_TIMEOUT_INFINITE);
		}
	}//end StartFakeServer()

public:
	FakeServer(int nPort) { m_nPort = nPort; }

};



//*****************************************************************************
// Entry Point
//*****************************************************************************
int main()
{
	// Initialization
	gDirContext = new ServerContext;			// Auto-deleting smart ptr
	std::list<FakeServer*> mServerList;			// list of the threads (for cleanup only)

	// State notification
	printf("Initializing...\r\n");

	// Load the dir server ips
	if (!LoadDirServers())
	{
		// error
		printf("Could not load directory server IP's.\r\n");
		return -1; 
	} else
		// addrs loaded
		printf("Directory server IPs loaded...\r\n");

	// Prepare the main loop
	bool EXIT_APP = false;						// True = exit application
	char ch;									// stores input character

	while (!EXIT_APP)
	{
		// Prompt
		printf("\r\n(1 = Spawn new server thread, 2 = Show directory contents, 3 = Exit): ");
		
		// input a character, skip over returns
		do 
		{
			ch = getchar();
		}
		while (ch == 10 || ch == 13) ;

		switch(ch)
		{
			case '1':
				{
				// Startup a dummy server in a seperate thread 
				FakeServer* aFakeServer = new FakeServer(mServerList.size());
				mServerList.push_back( aFakeServer );	// Store in the list for later cleanup
				aFakeServer->Start();					// Begin the thread
				printf("Number of spawned servers: %d\r\n", mServerList.size() );
				break;
				}
			case '2':
				{
				// Retrieve the directory contents and display
				printf("Retrieving directory contents...\r\n");
				ShowDirContents();
				break;
				}
			case '3':
				{
				// Exit the Application
				printf("Exiting...\r\n");
				EXIT_APP = true;
				break;
				}
			default:
				{
				printf("Not a recognized option.\r\n");
				break;
				}
		}//end switch(ch)
	}//end while(!EXIT_APP)

	// Cleanup
	while (mServerList.size() != 0)
	{
		FakeServer* aFakeServer = mServerList.front();
		mServerList.front()->Stop();
		mServerList.pop_front();
		delete aFakeServer;
	}

	return 0;
}//end main()



//*****************************************************************************
// Load the directory server IPs. (Usually from a config file.)
//*****************************************************************************
bool LoadDirServers()
{
	// These should be loaded from a file in retail applications
	//gDirContext->SetNeedShuffle(true);			// shuffle the ips to balance the load
	gDirContext->AddAddress( IPAddr("wontest.west.won.net",15101) );
	gDirContext->AddAddress( IPAddr("wontest.central.won.net",15101) );
	gDirContext->AddAddress( IPAddr("wontest.east.won.net",15101) );
	return true;
}//end LoadDirServers()



//*****************************************************************************
// Retrieve a list of registered services
//*****************************************************************************
bool ShowDirContents()
{
	// Construct the GetDirOp as a smart pointer
	GetDirOpPtr aGetDirOp = new GetDirOp(gDirContext);
	aGetDirOp->SetPath(DIR_GAME);
	aGetDirOp->SetFlags(DIR_GF_DECOMPSERVICES |		// Include services
						DIR_GF_SERVADDNAME |		// Include service names
						DIR_GF_SERVADDNETADDR);		// Include service addresses

	if (aGetDirOp->Run(OP_MODE_BLOCK,OP_TIMEOUT_INFINITE) != WS_Success)
		return false;
	else
		printf("Directory retrieved...\r\n");

	// Get a ptr to the entity list
	DirEntityList theDirContents = aGetDirOp->GetDirEntityList();
	
	// Parse through the list and display the registered fake servers
	DirEntityList::const_iterator aDirEntityItr = theDirContents.begin();
	for (; aDirEntityItr != theDirContents.end(); ++aDirEntityItr)
	{
		// Display the name and ip address.
		printf("%S", (*aDirEntityItr)->mName.c_str());
		printf("\t\t%s\r\n", (*aDirEntityItr)->GetNetAddrAsIP().GetHostAndPortString().c_str() );
	}
	return true;
}//end ShowDirContents()
