// AdApi.cpp : Defines the entry point for the DLL application.
//

#pragma warning(disable : 4786)

#include "AdApi.h"
#include "AdUtils.h"
#include "AdOptions.h"
#include "AdProtocol.h"
#include "AdDisplay.h"
#include "AdEngine.h"

using namespace WONAPI;
using namespace std;

// Constants
const char* Default_Logfile_Path = "C:\\logs\\AdApi.log";

#ifdef _USRDLL

// Note: This dll is designed to be implicitly loaded so nothing should 
// be put here unless that changes
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

#endif

AdApi::AdApi() :
	mAdDisplayP(NULL),
	mAdEngineP(NULL),
	mInitialized(false)
{
}

AdApi::~AdApi()
{
	// Make sure system is shut down if not already so
	Shutdown();
}

//////////////////////////////////////////////////////////////////////
// Public Interface for Ad API
//////////////////////////////////////////////////////////////////////

AdApi::AdResult 
AdApi::Init(const char* theProductName,
			NetCommInterface* theNetCommInterfaceP,
			const char* theCachePath,
			AdLocal theLocal)
{
	//AdLog::SetLoggingOption(AdLog::AdLogOption_LogTime);
	// Open log file
	// Need to do this here because of static construction issues
	INIT_LOG(Default_Logfile_Path);

	if (!theProductName) 
	{
		LOG("AdApi::Init Error:  No product name supplied");
		return AdResult_Failure;
	}
	if (!theNetCommInterfaceP)
	{
		LOG("AdApi::Init Error:  NetCommInterface is missing");
		return AdResult_Failure;
	}
	string aCachePath;
	if (theCachePath)
	{
		aCachePath = theCachePath;
	}
	else // Use default cache path
	{
		if (WONAPI::GetModulePath(aCachePath))
		{
			aCachePath += "AdCache";
		}
		else // Failed
		{
			LOG("AdApi::Init Error:  No Ad Cache path supplied and unable to get default");
			return AdResult_Failure;
		}
	}

	AdOptions::Init();

	mAdDisplayP = new AdDisplay();
	mAdEngineP = new AdEngine(theNetCommInterfaceP, mAdDisplayP);
	if (mAdEngineP->Init(theProductName, aCachePath, theLocal))
	{
		LOG("AdApi::Init Ad Engine intitialization completed successfully");
	}
	else // Failed
	{
		LOG("AdApi::Init Ad Engine intitialization failed");
		return AdResult_Failure;
	}
		
	LOG("AdApi::Init Initialization completed successfully");
	mInitialized = true;

	// Get display options
	bool aSceneSkip;
	AdOptions::GetBoolOption(OptionName_AllowSceneSkipping, OptionDefault_AllowSceneSkipping, aSceneSkip);
	mAdDisplayP->AllowSceneSkipping(aSceneSkip);

	mAdEngineP->StartAdDownload(); // start ad download process when options have been downloaded..
	return AdResult_Success;
}

	
AdApi::AdResult 
AdApi::Shutdown() 
{
	if (mInitialized)
	{
		if (mAdDisplayP)
		{
			// This should not be needed and can be removed in the future.. This was added
			// to help beginning game teams avoid deadlock issues.
			mAdDisplayP->StopAdDisplay(true);
		}
		mInitialized = false;
		AdOptions::Shutdown();
		
		delete mAdEngineP;
		mAdEngineP = NULL;
		delete mAdDisplayP;
		mAdDisplayP = NULL;

		// Close log file
		SHUTDOWN_LOG();
	}

	return AdResult_Success;
}

#ifdef _DEBUG
bool 
AdApi::SetLogFile(const char* theLogFilePath)
{
#ifdef ADAPI_LOGGING
	return SET_LOGFILE(theLogFilePath);
#else
	return false;
#endif
}
#endif
//////////////////////////////////////////////////////////////////////
// Setting callbacks
//////////////////////////////////////////////////////////////////////
	
AdApi::AdResult 
AdApi::SetPaintCallback(PaintCallback thePaintCallbackP, void* theDataPaintP)
{
	if (!mInitialized)
	{
		return AdResult_NotInitialized;
	}
	if (!mAdDisplayP)
	{
		LOG("AdApi::SetPaintCallback Error:  Display module not present");
		return AdResult_Failure;
	}
	if (!mAdEngineP)
	{
		LOG("AdApi::SetPaintCallback Error:  Engine module not present");
		return AdResult_Failure;
	}

	mAdDisplayP->SetPaintCallback(thePaintCallbackP, theDataPaintP);

	// Display default ad
	mAdEngineP->DisplayDefaultAd();

	return AdResult_Success;
}

//////////////////////////////////////////////////////////////////////
// Download functions
//////////////////////////////////////////////////////////////////////

AdApi::AdResult
AdApi::PauseDownload(bool thePause)
{
	if (!mInitialized)
	{
		return AdResult_NotInitialized;
	}
	if (!mAdEngineP)
	{
		LOG("AdApi::PauseDownload Error:  Engine module not present");
		return AdResult_Failure;
	}

	if (thePause)
	{
		LOG("AdApi::PauseDownload Ad download paused");
	}
	else
	{
		LOG("AdApi::PauseDownload Ad download unpaused");
	}
	mAdEngineP->PauseAdDownload(thePause);
	return AdResult_Success;
}

AdApi::AdResult
AdApi::PauseAdDelivery(bool thePause)
{
	if (!mInitialized)
	{
		return AdResult_NotInitialized;
	}
	if (!mAdEngineP)
	{
		LOG("AdApi::PauseDownload Error:  Engine module not present");
		return AdResult_Failure;
	}
	if (!mAdDisplayP)
	{
		LOG("AdApi::StopAdDisplay Error:  Display module not present");
		return AdResult_Failure;
	}
	if (thePause)
	{
		LOG("AdApi::PauseAdDelivery Ad delivery paused");
	}
	else
	{
		LOG("AdApi::PauseAdDelivery Ad delivery unpaused");
	}
	mAdDisplayP->StopAdDisplay(thePause);
	mAdEngineP->PauseAdDownload(thePause);
	return AdResult_Success;
}

//////////////////////////////////////////////////////////////////////
// Display functions
//////////////////////////////////////////////////////////////////////

AdApi::AdResult 
AdApi::AdClicked(AdLaunchWndShowCmd theShowCmd)
{
	if (!mInitialized)
	{
		return AdResult_NotInitialized;
	}
	if (!mAdEngineP)
	{
		LOG("AdApi::AdClicked Error:  Engine Module is not present");
		return AdResult_Failure;
	}

	// Launch browser
	if (!mAdEngineP->LaunchAdDestination(theShowCmd))
	{
		LOG("AdApi::AdClicked Error: Unable to launch default browser");
		return AdResult_BrowserLaunchFailure;
	}

	return AdResult_Success;
}

AdApi::AdResult 
AdApi::GetToolTip(char*& theToolTipR)
{
	if (!mInitialized)
	{
		return AdResult_NotInitialized;
	}
	if (!mAdEngineP)
	{
		LOG("AdApi::GetToolTip Error:  Engine Module is not present");
		return AdResult_Failure;
	}

	string aToolTip;
	mAdEngineP->GetToolTip(aToolTip);
	if (aToolTip.size() > 0)
	{
		theToolTipR = new char[aToolTip.size()+1];
		memcpy(theToolTipR, aToolTip.c_str(), aToolTip.size()+1);
		return AdResult_Success;
	}
	else
	{
		theToolTipR = NULL;
		return AdResult_Failure;
	}
}

void 
AdApi::ReleaseToolTip(char* theToolTip)
{
	delete [] theToolTip;
}

AdApi::AdResult 
AdApi::GetDisplayImage(HDC theDC, HBITMAP& theBitmapHR)
{
	if (!mInitialized)
	{
		return AdResult_NotInitialized;
	}
	if (!mAdDisplayP)
	{
		LOG("AdApi::GetDisplayImage Error:  Display module not present");
		return AdResult_Failure;
	}

	// Get current bitmap image to display
	if (mAdDisplayP->GetDisplayImage(theDC, theBitmapHR))
	{
		return AdResult_Success;
	}
	else
	{
		LOG("AdApi::GetDisplayImage Error:  Unable to get Display image");
		return AdResult_Failure;
	}
}

void
AdApi::ReleaseDisplayImage(HBITMAP theBitmap)
{
	if (!mInitialized)
	{
		return;
	}
	if(mAdDisplayP)
		mAdDisplayP->ReleaseDisplayImage(theBitmap);
	else
		LOG("AdApi::ReleaseDisplayImage Error:   Display module not present");
}

AdApi::AdResult 
AdApi::GetDisplayImage(BYTE* &theRawPR, int& theWidthR, int& theHeightR)
{
	if (!mInitialized)
	{
		theRawPR = NULL;
		return AdResult_NotInitialized;
	}
	if (!mAdDisplayP)
	{
		theRawPR = NULL;
		LOG("AdApi::GetDisplayImage Error:  Display module not present");
		return AdResult_Failure;
	}

	// Get current bitmap image to display
	if (mAdDisplayP->GetDisplayImage(theRawPR, theWidthR, theHeightR))
	{
		return AdResult_Success;
	}
	else
	{
		theRawPR = NULL;
		LOG("AdApi::GetDisplayImage Error:  Unable to get Display image");
		return AdResult_Failure;
	}
}

void
AdApi::ReleaseDisplayImage(BYTE* theRawP)
{
	if (!mInitialized)
	{
		return;
	}
	if(mAdDisplayP)
		mAdDisplayP->ReleaseDisplayImage(theRawP);
	else
		LOG("AdApi::ReleaseDisplayImage Error:  Display module not present");
}



