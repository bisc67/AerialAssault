
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

using namespace WONAPI;

/*
	The WONAPICoreEx class spawns and controls any threads required by the WON API and
	manages API operations. 
*/
WONAPICoreEx gAPI;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SingleThreadedGame()
{
	// If you don't have the API run its own PumpThread then you must call 
	// gAPI.Pump() at regular intervals in your own app.  The advantage of 
	// doing this is that all API completions happen in the thread which 
	// calls Pump, so you can have completions go off in your game thread by 
	// pumping the API in your game thread.
	gAPI.SetDoPumpThread(false);

	// If you want the API to be truly single threaded then you can tell it not
	// to handle blocked socket operations and timer operations in seperate
	// threads.  When these threads are enabled, blocked socket operations are
	// waited on and continued by a seperate API thread, and timer operations are
	// handled by yet another thread which is simply waiting on the earliest timer.
	// If you disable these threads then the API will handle these functions in
	// the call to gAPI.Pump().  This is probably overkill unless your game is linking
	// to the SingleThreaded libraries in which case you can't have threads.
	//
	// Note: For extreme single threading, link to the Debug SingleThreaded and
	// Release SingleThreaded versions of the WONAPI library and define __WON_SINGLETHREADED__
	// in your app.  
	gAPI.SetDoSocketAndTimerThreads(false);

	// Start the API (This starts necessary API threads and performs various other initializations)
	gAPI.Startup();

//	DoGameStuff
//	while(GameIsRunning())
//	{
//		DoSomeGameStuff();
		gAPI.Pump(10); // Pump API for 10 milliseconds 
		// Note: You can call Pump(0) if you don't want this call to wait at all.
		// Pump(0) will simply handle operations that are ready to go, whereas
		// Pump(10) will wait up to 10 milliseconds for operations to become
		// ready (i.e. data to come in from the network, timers to go off, operations
		// to finish, etc...)
//	}



	// Stop the API.  This kills all pending API operations.  The operations are 
	// still completed underneath this call.  The operations' AsyncOp::Killed() method will 
	// return true, though.
	gAPI.Shutdown();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void MultiThreadedGameWithCompletionsInGameThread()
{
	// If you don't have the API run its own PumpThread then you must call 
	// gAPI.Pump() at regular intervals in your own app.  The advantage of 
	// doing this is that all API completions happen in the thread which 
	// calls Pump, so you can have completions go off in your game thread by 
	// pumping the API in your game thread.
	gAPI.SetDoPumpThread(false);

	// Start the API (This performs various initializations)
	gAPI.Startup();

//	DoGameStuff
//	while(GameIsRunning())
//	{
//		DoSomeGameStuff();
		gAPI.Pump(10); // Pump API for 10 milliseconds
		// Note: You can call Pump(0) if you don't want this call to wait at all.
		// Pump(0) will simply handle operations that are ready to go, whereas
		// Pump(10) will wait up to 10 milliseconds for operations to become
		// ready (i.e. data to come in from the network, timers to go off, operations
		// to finish, etc...)
//	}



	// Stop the API.  This stops all API threads and kills all pending API operations.  
	// The operations are still completed underneath this call.  The operations' 
	// AsyncOp::Killed() method will return true, though.
	gAPI.Shutdown();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void MultiThreadedGame()
{
	// Let the API run and complete operations in its own thread. 
	gAPI.SetDoPumpThread(true); 

	// Start the API (This starts necessary API threads and performs various other initializations)
	gAPI.Startup();
	
	// Do game stuff
	// DoGameStuff();

	// Stop the API.  This stops all API threads and kills all pending API operations.  
	// The operations are still completed underneath this call.  The operations' 
	// AsyncOp::Killed() method will return true, though.
	gAPI.Shutdown();
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int main()
{
//	SingleThreadedGame();
	MultiThreadedGameWithCompletionsInGameThread();
//	MultiThreadedGame();

	return 0;
}
