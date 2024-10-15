// Threadbase.cpp - nice encapsulation for threads
//

#include "AdThreadbase.h"
#include <process.h>

// private namespace for constants and using
namespace {
	using AdApiCommon::ThreadBase;

	const char* THREADNAME_DEF = "Unnamed_Thread";
};

ThreadBase::ThreadBase(const char* theName) :
	hThread(0),
	threadId(0),
	mLastError(0),
	priority(THREAD_PRIORITY_NORMAL),
	hStop(0),
	hExceptionNotify(0),
	mName(theName ? theName : THREADNAME_DEF)
{
   hStop  = CreateEvent(NULL, true, false, NULL);
}


ThreadBase::~ThreadBase()
{
   stopThread();   
   if (hStop) CloseHandle( hStop ); hStop = NULL;
}


// If compiled to use single-threaded libraries,
// startThread() will fail to link.
#ifdef _MT
void ThreadBase::startThread()
{
   if (!hThread)
   {
      ResetEvent( hStop );                      //force to non-signaled
      hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadRoutine, this, CREATE_SUSPENDED, (unsigned int*)&threadId );

      if (hThread)
      {
         SetThreadPriority( hThread, priority );
         ResumeThread( hThread );   
      }
   }
}
#endif


void ThreadBase::stopThread()
{
   if (hThread)   
   {
      SetEvent( hStop );                        //signal hStop so threadProcess can return.

      WaitForSingleObject(hThread,INFINITE);    //wait for threadProcess to sucessfully exit
      CloseHandle(hThread);                     //close the thread
      hThread  = 0;
	  threadId = 0;
   }
}


unsigned int __stdcall ThreadBase::ThreadRoutine( void* param ) 
{
	ThreadBase *pTB = (ThreadBase *)param;

	// anything not caught in the derived thread will eventually end up here.
	// this is the bottom of our threads execution stack
	int aRet = 0;
	try
	{
		aRet = pTB->threadProcess();
	}

#ifndef _NO_TOPLEVEL_CATCH
	catch (...)
	{
		// Error code is unknown, set to (-1)
		if(! pTB->mLastError) pTB->mLastError = -1;

		// Set the exception event if needed
		if (pTB->hExceptionNotify) SetEvent(pTB->hExceptionNotify);
		aRet = pTB->mLastError;
	}
#endif

	return aRet;
}


int ThreadBase::threadProcess()
{
   while (1)
   {
      //default behavior: sleep until it's time to stop the thread
      switch (WaitForSingleObject( getStopEvent(), INFINITE ))
      {
         case WAIT_OBJECT_0:  return (1);
         case WAIT_TIMEOUT:   
         default:             return (0);
      }
   }
}
