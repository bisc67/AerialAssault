
/****************************************************************************
 * The starting point for all WON enabled products is the creation of the	*
 * WONAPICore object. This object also creates the required message threads.*
 ****************************************************************************/


/* Before you begin...
		- Include the path for, and link to, the WONAPI.lib. (../../debug)
		- Include the path for the WONAPI header files. (../../)
		- Be sure you are linking to the Multithread version run time libraries.
		- Link to either wsock32.lib or ws2_32.lib.
		- If you are going to be using platform specific code be sure to 
			include	WONCommon/Platform.h.
*/

#include "WONCommon/Platform.h"					// Platform specfic headers
#include "WONApi.h"								// API namespace. (WONAPICore defined)
#include "WONSocket/IPAddr.h"					// Encapsulated IP Addresses
#include "WONServer/PingOp.h"					// Perform a ping to an ip address
#include "WONCommon/SmartPtr.h"					// Auto-deleting pointers


using namespace WONAPI;

// STATICS (server lists as globals for simplicity)
WONAPICore	apiCore(true);						// Required by WONAPI
const int	NUM_SERVERS			= 3;			// Number of servers to ping
const int	PINGS_PER_SERVER	= 5;			// Number of pings per server
const int	PING_TIMEOUT		= 10000;		// Timeout per ping

int			gPingsDone =0;						// Number of pings completed
Event		gDoneEvent;							// Singnals the end of the application

// PROTOTYPES
void PingServer(IPAddr&);						// Pings the specified IP
void OnPingDone(AsyncOpPtr);					// Called after a ping attempt returns

//******************************************************************************
//  Entry point
//******************************************************************************
int main()
{
	// Initialize the IPAddr list
	printf("Initializing server list...\r\n");
	IPAddr theDirServers[NUM_SERVERS] = {
		IPAddr("wontest.east.won.net:15101"),
		IPAddr("wontest.central.won.net:15101"),
		IPAddr("wontest.west.won.net:15101")
	};

	// Iterate throught the list and ping each server twice
	printf("Pinging servers...\r\n");
	for (int nPing=0; nPing < PINGS_PER_SERVER; nPing++)
	{
		for (int nServer=0; nServer < NUM_SERVERS; nServer++)
			PingServer(theDirServers[nServer]);
	}

	// Idle until all pings are done
	gDoneEvent.WaitFor(INFINITE); 

	// Finished
	printf("Done...\r\n");
	return 0;

}



//******************************************************************************
//  Runs a PingOp on an IPAddr
//******************************************************************************
void PingServer(IPAddr& theIPAddr)
{
	// Construct the PingOp
	PingOpPtr thePingOp = new PingOp(theIPAddr,4);
	thePingOp->SetCompletion( new OpCompletion(OnPingDone) );

	// Run the op
	thePingOp->RunAsync(PING_TIMEOUT);

}


//******************************************************************************
//  Called after a completed PingOp on an IPAddr
//******************************************************************************
void OnPingDone(AsyncOpPtr theOp)
{
	// Cast into the PingOpPtr
	PingOp* thePingOp = (PingOp*)theOp.get();

	// Test for and print the result
	switch(theOp->GetStatus())
	{
	case WS_Success:
		printf("Ping time for %s, \t%d \r\n", thePingOp->GetAddr().GetHostAndPortString().c_str(), thePingOp->mLag);
		break;
	case WS_TimedOut:
		printf("Ping timed out for %s.\r\n", thePingOp->GetAddr().GetHostAndPortString().c_str());
		break;
	default:
		printf("Error on %s, could not complete ping op.\r\n", thePingOp->GetAddr().GetHostAndPortString());
	}

	// Increment our ping count
	gPingsDone++;

	if (gPingsDone == (NUM_SERVERS*PINGS_PER_SERVER))
		gDoneEvent.Set();

}
