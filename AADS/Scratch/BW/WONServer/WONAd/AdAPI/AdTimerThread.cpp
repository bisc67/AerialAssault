
#pragma warning(disable : 4786)

#include "AdUtils.h"
#include "AdTimerThread.h"

using namespace WONAPI;

AdTimerThread::AdTimerThread() :
	ThreadBase(),
	mCriticalSection(),
	mStartTimerEvent(NULL),
	mTimerMap(),
	mTimerHandleMap(),
	mLastHandle(0)
{
	mStartTimerEvent = CreateEvent(NULL, false, false, NULL);

	LOG("AdTimerThread::AdTimerThread Thread starting");
	startThread(); // Start the timer
}

AdTimerThread::~AdTimerThread()
{
	// Do this here so item waiting does not have events closed it is waiting on...
	stopThread();

	if (mStartTimerEvent)
	{
		CloseHandle(mStartTimerEvent);
		mStartTimerEvent = NULL;
	}
}

TimerHandle 
AdTimerThread::StartTimer(TimerOp& theTimerOpR)
{
	AdApiCommon::AutoCrit aAutoCrit(mCriticalSection);
	DWORD aFinishTime = GetTickCount() + theTimerOpR.GetDuration();
	
	TimerHandle aTimerHandle = GetNextHandle();
	theTimerOpR.SetTimerHandle(aTimerHandle);

	TimerMap::iterator aItr = mTimerMap.insert(TimerMap::value_type(aFinishTime, theTimerOpR));
	if (aItr != mTimerMap.end())
	{
		mTimerHandleMap.insert(TimerHandleMap::value_type(aTimerHandle,aItr));
		LOG("AdTimerThread::StartTimer Timer added, Number of timers " << mTimerMap.size());
	}
	else
	{
		LOG("AdTimerThread::StartTimer Error - Failed to insert timer into map");
	}

	// Set event so timer thread will wake up if needed..
	SetEvent(mStartTimerEvent);

	return aTimerHandle;
}

TimerHandle
AdTimerThread::StartTimer(DWORD theDuration, TimerCallback theTimerCallback, void* theParamP)
{
	TimerOp aTimerOp;
	aTimerOp.SetTimer(theDuration, theTimerCallback, theParamP);
	
	return StartTimer(aTimerOp);
}

void
AdTimerThread::RemoveTimer(TimerHandle theTimerHandle)
{
	AdApiCommon::AutoCrit aAutoCrit(mCriticalSection);
	TimerHandleMap::iterator aItr = mTimerHandleMap.find(theTimerHandle);
	if (aItr != mTimerHandleMap.end())
	{
		mTimerMap.erase(aItr->second);
		mTimerHandleMap.erase(aItr);

		LOG("AdTimerThread::RemoveTimer Timer removed, Number of timers " << mTimerMap.size());

		// Wake up and wait for a different amount of time
		SetEvent(mStartTimerEvent);
	}
	else
	{
		LOG("AdTimerThread::RemoveTimer Failed Timer already removed " << theTimerHandle);
	}
}


int
AdTimerThread::threadProcess()
{
	HANDLE aWaitArray[2] = {getStopEvent(), getStartTimerEvent()};
	while(true)
	{
		// If multiple events get signaled
		DWORD aResult = WaitForMultipleObjects(2, aWaitArray, false, CalculateWaitTime());

		switch(aResult)
		{
			case WAIT_FAILED: // Error
			{
				LOG("AdTimerThread::threadProcess Error:  Wait failed");
				return 1;
			}
			break;
			case WAIT_TIMEOUT: // Time to execute operation we were waiting on
			{
				mCriticalSection.Enter();
				TimerMap::iterator aItr = mTimerMap.begin();
				TimerOp aTimerOp;
				if (aItr != mTimerMap.end())
				{
					aTimerOp = aItr->second;
					mTimerHandleMap.erase(aTimerOp.GetTimerHandle());
					mTimerMap.erase(aItr);
					LOG("AdTimerThread::threadProcess Timer removed, Number of timers " << mTimerMap.size());
				}
				mCriticalSection.Leave();

				if (aTimerOp.GetTimerHandle())
				{
					//LOG("AdTimerThread::threadProcess Timer goes off " << aTimerOp.GetTimerHandle());

					aTimerOp.Finished();
				}
			}
			break;
			// Stop requested
			case WAIT_OBJECT_0:
			{
				LOG("AdTimerThread::threadProcess Stop thread requested.  Thread ID: " << getId());
				return 0;
			}
			break;
			case WAIT_OBJECT_0 + 1:
			{
				//LOG("AdTimerThread::threadProcess Timer started updating list");
			}
			break;
		}	
	}
}

// calculate time until first timer goes off
DWORD 
AdTimerThread::CalculateWaitTime()
{
	AdApiCommon::AutoCrit aAutoCrit(mCriticalSection);
	TimerMap::iterator aItr = mTimerMap.begin();
	if (aItr != mTimerMap.end())
	{
		DWORD aCurrTime = GetTickCount();
		if (aCurrTime < aItr->first)
		{
			return aItr->first - aCurrTime;
		}
		else // We are at time or past time
		{
			return 0;
		}
	}
	else
	{
		return INFINITE;
	}
}

TimerHandle
AdTimerThread::GetNextHandle()
{
	return mLastHandle != 0xFFFFFFFF ? ++mLastHandle : 1; 
}
