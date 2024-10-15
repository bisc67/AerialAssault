// AdEngine.cpp

#pragma warning(disable:4786)

#include "AdEngine.h"
#include "AdUtils.h"
#include "AdOptions.h"

using namespace WONAPI;
using namespace std;

const std::string AdEngine::Default_AdImageFileName = "DefaultAd.gif";
const std::string AdEngine::Default_AdLinkFileName = "DefaultLink.txt";
const std::string AdEngine::Default_AdLink = "www.won.net";
const unsigned short RemoteOptions_Port = 80;
const std::string RemoteOptions_Server = "ads.flipside.com";
const long RemoteOptions_Timeout = 30000; // 30 sec
const int NumRetrys_OnError = 5; 

// Constructor/Destructor
AdEngine::AdEngine(NetCommInterface* theNetCommInterfaceP,  AdDisplay* theAdDisplayP) :
	ThreadBase(),
	mNetCommInterfaceP(theNetCommInterfaceP),
	mAdDisplayP(theAdDisplayP),
	mAdProtocolP(NULL),
	mStartDownloadOptionsEvent(NULL),
	mAdClickedEvent(NULL),
	mStartDownloadEvent(NULL),
	mLastDisplayTime(0),
	mOptionsDoneEvent(NULL),
	mOptionsResult(false),
	mOptionsDownloaded(false),
	mShowingDefaultAd(false),
	mDownloadPaused(false),
	mCurrAdInfoP(NULL),
	mClickedAdInfoP(NULL),
	mClickedAdShowCmd(SW_SHOW),
	mAdDisplayDuration(0),
	mAdProvider(AdEngine::AdProvider_Default),
	mProductName(),
	mMaxDownloadRate(0), // Not used
	mCachePath()		
{
	mStartDownloadOptionsEvent = CreateEvent(NULL, true, false, NULL);
	mAdClickedEvent = CreateEvent(NULL, true, false, NULL);
	mStartDownloadEvent = CreateEvent(NULL, true, false, NULL);
	mOptionsDoneEvent = CreateEvent(NULL, false, false, NULL);
}

AdEngine::~AdEngine()
{
	// Do this here so item waiting does not have events closed it is waiting on...
	stopThread();
	if (mStartDownloadOptionsEvent)
	{
		CloseHandle(mStartDownloadOptionsEvent);
		mStartDownloadOptionsEvent = NULL;
	}
	if (mAdClickedEvent)
	{
		CloseHandle(mAdClickedEvent);
		mAdClickedEvent = NULL;
	}
	if (mStartDownloadEvent)
	{
		CloseHandle(mStartDownloadEvent);
		mStartDownloadEvent = NULL;
	}
	if (mOptionsDoneEvent)
	{
		CloseHandle(mOptionsDoneEvent);
		mOptionsDoneEvent = NULL;
	}
	LOG("AdEngine::AdEngine Download thread stopped");
	delete mAdProtocolP;
	mAdProtocolP = NULL;
}

bool
AdEngine::Init(const string& theProductName, const string& theCachePath, AdApi::AdLocal theLocal)
{
	mLastDisplayTime = 0;
	mOptionsDownloaded = false;
	mDownloadPaused = false;

	mProductName = theProductName;
	mCachePath = theCachePath;
	mLocal = theLocal;

	// Sets event that tells engine to download options
	// Note:  options can not be read until after configuration file has been downloaded
	SetEvent(mStartDownloadOptionsEvent);

	LOG("AdEngine::Init Thread starting");
	startThread(); // Start the download thread

	return true;
}

void
AdEngine::StartAdDownload()
{
	if (!mDownloadPaused)
	{
		// Sets event that starts downloading the ad
		SetEvent(mStartDownloadEvent);
	}
}

void 
AdEngine::PauseAdDownload(bool thePause)
{
	if (thePause) // Pause download
	{
		mDownloadPaused = true;
	}
	else // resume download
	{
		mDownloadPaused = false;
		StartAdDownload();
	}
}

// Launch browser with desination of ad currently being displayed.
// Called from main API thread
bool
AdEngine::LaunchAdDestination(int theShowCmd)
{
	string aAdDestination;
	if (mShowingDefaultAd)
	{
		// Use default destination
		if (!GetDefaultDestination(aAdDestination))
		{
			LOG("AdEngine::LaunchAdDestination Error: Unable to get default destination");
			return false;
		}
		if (aAdDestination.length() != 0)
		{
			// Launch default browser
			// ** Might want to make sure that string is a URL
			ShellExecute(NULL, NULL, aAdDestination.c_str(), NULL, NULL, theShowCmd);
		}
		else
		{
			LOG("AdEngine::LaunchAdDestination Error: Launch of ad destination attemped on empty destination");
			return false;
		}
	}
	else
	{
		// If this is a redirect we need to follow redirect now before launch.
		// Set current ad click information for later use by engine thread
		mClickedAdInfoP = mCurrAdInfoP;
		mClickedAdShowCmd = theShowCmd;
		SetEvent(mAdClickedEvent);
	}
	
	return true;
}


// Notifies the engine that an ad is ready to be displayed..
// Note: this function is designed to be called from another thread so must be thread safe
bool
AdEngine::DisplayAd(AdInfoPtr theAdInfoP)
{
	if (mAdDisplayP->LoadNewImage(theAdInfoP->mAdPath))
	{
		mShowingDefaultAd = false;
		mCurrAdInfoP = theAdInfoP;

		// reset timer for dowload of next ad
		mLastDisplayTime = GetTickCount();

		return true;
	}
	else
	{
		// Download new ad
		StartAdDownload();
		LOG("AdEngine::DisplayAd Error:  Error trying to display ad " << theAdInfoP->mAdPath);
		return false;	
	}
}

void
AdEngine::DisplayDefaultAd()
{
	string aDefaultAdPath;
	if (GetModulePath(aDefaultAdPath))
	{
		aDefaultAdPath += Default_AdImageFileName;
		if (WONAPI::PathExists(aDefaultAdPath) && mAdDisplayP->LoadNewImage(aDefaultAdPath))
		{
			mShowingDefaultAd = true;
		}
		else
		{
			LOG("AdEngine::DisplayDefaultAd Error:  Unable to find default ad");
		}
	}
	else
	{
		LOG("AdEngine::DisplayDefaultAd Error:  Unable to find default ad");
	}
}

bool
AdEngine::GetDefaultDestination(string& theDefaultDestination)
{
	string aDefaultAdPath; 
	string aDefaultAdLink = Default_AdLink; // set default if unable to get from file
	// Check for default link.txt
	if (GetModulePath(aDefaultAdPath))
	{
		aDefaultAdPath += Default_AdLinkFileName;
		char aBuffer[256];
		if (GetPrivateProfileString("AdDisplay", "DefaultLink", "", aBuffer, 256, aDefaultAdPath.c_str()))
		{
			aDefaultAdLink = aBuffer;
		}
	}

	// Get from option file
	// Order of Presidence - Option file, DefaultLink.txt, hard coded link
	AdOptions::GetStringOption(WONAPI::OptionName_DefaultLink, aDefaultAdLink, theDefaultDestination);

	return true;
}

void
AdEngine::OptionsDone(bool theResult)
{
	mOptionsResult = theResult;
	SetEvent(mOptionsDoneEvent);
}

// Overloaded Methods
// Note:  We used to download ads in the back ground and show them every 30 seconds but this caused us to be 
// off on the impression count because we had downloaded and ad we had not displayed.  Now we download another ad
// only after the previous ad has been displayed for the aloted time.
int
AdEngine::threadProcess()
{
	HANDLE aWaitArray[4] = {getStopEvent(), getStartDownloadOptionsEvent(), getAdClickedEvent(), getStartDownloadEvent()};
	int aNumRetrys = 0; // Used to set how many times in a row we try to get ads if we have a failures
	while(true)
	{
		// If multiple events get signaled
		DWORD aResult = WaitForMultipleObjects(4, aWaitArray, false, CalculateTimeUntilAdDisplay());

		switch(aResult)
		{
			case WAIT_FAILED: // Error
			{
				LOG("AdEngine::threadProcess Error:  Wait failed");
				return 1;
			}
			break;
			case WAIT_TIMEOUT: // Time to download a new ad
			{
				StartAdDownload();
			}
			break;
			// Stop requested
			case WAIT_OBJECT_0:
			{
				LOG("AdEngine::threadProcess Stop thread requested.  Thread ID: " << getId());
				return 0;
			}
			break;
			// Download options..
			case WAIT_OBJECT_0+1:
			{
				LOG("AdEngine::threadProcess Downloading options");
				if (!GetAdServOptions())
				{
					LOG("AdEngine::threadProcess Error:  Get options failed");
					return 1; // Might not want to be this harsh.. might want let them go ahead without remote options
				}
				LOG("AdEngine::threadProcess Download options succeded");
				ReadOptions();
				if (!LoadProtocol())
				{
					LOG("AdEngine::threadProcess Error:  Unable to load protocol");
					return 1;
				}
				ResetEvent(getStartDownloadOptionsEvent());
				StartAdDownload();
			}
			break;
			// Ad clicked on
			case WAIT_OBJECT_0+2:
			{
				LOG("AdEngine::threadProcess Ad click process started");
				if (!mAdProtocolP->LaunchAdDestination(mClickedAdInfoP, mClickedAdShowCmd))
				{
					LOG("AdEngine::threadProcess Error launching ad destination");
				}
				ResetEvent(getAdClickedEvent());
			}
			break;
			// Download the ad..
			case WAIT_OBJECT_0+3:
			{
				LOG("AdEngine::threadProcess Ad download started");
				if (mAdProtocolP->DownloadAd())
				{
					LOG("AdEngine::threadProcess Ad download complete");
					
					// Display ad
					// Ad is ready and it is time to display it
					if (mAdProtocolP->IsAdReady())
					{
						if (DisplayAd(mAdProtocolP->GetCurrAdInfo()))
						{
							aNumRetrys = 0; // Reset Retry counter
							// Ad display succeded display ad for display amount of time
							ResetEvent(getStartDownloadEvent());
						}
						else
						{
							LOG("AdEngine::threadProcess Ad display failed downloading new ad");
							aNumRetrys++;
							if (aNumRetrys >= NumRetrys_OnError)
							{
								LOG("AdEngine::threadProcess Error:  Max number of retrys reached shut down ad download process");
								return 1;
							}
						}
					}
					else
					{
						LOG("AdEngine::threadProcess Ad is not ready downloading new ad");
					}
				}
				else
				{
					LOG("AdEngine::threadProcess Error:  Ad download failed");
					aNumRetrys++;
					if (aNumRetrys >= NumRetrys_OnError)
					{
						LOG("AdEngine::threadProcess Error:  Max number of retrys reached shut down ad download process");
						return 1;
					}
				}
			}
			break;	
			default:
				LOG("AdEngine::threadProcess Error: Unexpected error has occured");
			break;
		}
	}
}

// Private methods

DWORD
AdEngine::CalculateTimeUntilAdDisplay()
{
	// This will be infinite until options and ad are downloaded...
	if (!mOptionsDownloaded || mDownloadPaused)
	{
		return INFINITE;
	}

	DWORD aElapsed = GetTickCount() - mLastDisplayTime;

	// This is done this way so that the 49 day wrap arround will not affect the calculation
	if (aElapsed >= mAdDisplayDuration)
	{
		return 0; // It is time or past time to show ad
	}
	else
	{
		return mAdDisplayDuration - aElapsed;
	}
}

// Get ad serving options from Won server
// This function makes blocking network calls
bool
AdEngine::GetAdServOptions()
{
	if (!mOptionsDownloaded)
	{
		string aDestPath;
		string aServerPath = "/AdApi/";

		if (GetRemoteOptionsPath(aDestPath))
		{
			aServerPath += mProductName;
			string aLocalStr = GetLocalString();
			if (aLocalStr.size())
			{
				aServerPath += "/";
				aServerPath += aLocalStr;
			}
			aServerPath += "/";
			aServerPath += RemoteOptions_File;
			// Currently there is no mechanism that alows us to read this directly into memory..
			// At some point this should be changed to do that
			// Warning:  There will be a problem here if AdEngine is deleted before the callback is called.
			mOptionsResult = false;
			
			if ( mNetCommInterfaceP->HTTPGet(RemoteOptions_Server.c_str(), RemoteOptions_Port, aServerPath.c_str(), aDestPath.c_str(), RemoteOptions_Timeout,GetOptionsCallback, this))
			{
				// Wait for file to come back
				HANDLE aWaitArray[2] = {getStopEvent(), mOptionsDoneEvent};
				WaitForMultipleObjects(2, aWaitArray, false, INFINITE);
			}

			AdOptions::ReadOptionsFile(aDestPath);

			// Delete the temp file
			DeleteFile(aDestPath.c_str());

			if(mOptionsResult)
			{
				mOptionsDownloaded = true;
			}
		}
	}
	return mOptionsDownloaded;
}

// Finds a valid remote options path
bool
AdEngine::GetRemoteOptionsPath(string& thePath)
{
	char aTempPath[MAX_PATH];
	if (GetTempPath(MAX_PATH, aTempPath)) 
	{
		thePath = aTempPath;
	}
	else // no temp path
	{
		if (!GetModulePath(thePath))
		{
			return false;
		}
	}

	// Find a file name that is unused...
	int n;
	char aNum[6];
	string aTempFile;
	HANDLE aFile;
	for (n = 0; n<10000; n++)
	{
		aTempFile = thePath;
		aTempFile += "ada";
		aTempFile += ltoa(n,aNum,10);
		aTempFile += ".tmp";

		// Check for existance
		aFile = CreateFile(aTempFile.c_str(), /*GENERIC_READ*/0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED | FILE_FLAG_SEQUENTIAL_SCAN, NULL); 
		if (aFile != INVALID_HANDLE_VALUE) 
		{
			CloseHandle(aFile);
		}
		else // Found one that does not exist
		{
			thePath = aTempFile;
			break;
		}
	}
	
	return n<10000;
}

const string
AdEngine::GetLocalString()
{
	switch (mLocal)
	{
		case AdApi::AdLocal_UK:
			return "Uk";
		break;
		case AdApi::AdLocal_French:
			return "Frn";
		break;
		case AdApi::AdLocal_German:
			return "Grm";
		break;
		case AdApi::AdLocal_Spanish:
			return "Spn";
		break;
		case AdApi::AdLocal_Korean:
			return "Kor";
		break;
		case AdApi::AdLocal_Japan:
			return "Jap";
		break;
		default:
			return "";
		break;
	}
}

bool
AdEngine::ReadOptions()
{
	AdOptions::GetLongOption(WONAPI::OptionName_AdRotation, WONAPI::OptionDefault_AdRotation, mAdDisplayDuration);
	mAdDisplayDuration *= 1000; // Convert from seconds to milli seconds
	
	// Super secret option for changing to old ad provider
	string aAdProvider;
	AdOptions::GetStringOption(WONAPI::OptionName_AdProvider, WONAPI::OptionDefault_AdProvider, aAdProvider);
	if (!strcmp(ToLower(aAdProvider).c_str(), "24x7") )
	{
		mAdProvider = AdEngine::AdProvider_24x7;
	}
	else if (!strcmp(ToLower(aAdProvider).c_str(), "dart") )
	{
		mAdProvider = AdEngine::AdProvider_Dart;
	}
	else // Use default
	{
		mAdProvider = AdEngine::AdProvider_Default;
	}
	return true;
}

bool
AdEngine::LoadProtocol()
{
	switch (mAdProvider)
	{
		case AdEngine::AdProvider_24x7:
		{
			mAdProtocolP = new AdForceProtocol(mNetCommInterfaceP, mCachePath, mProductName, getStopEvent());
		}
		break;
		case AdEngine::AdProvider_Default:
		case AdEngine::AdProvider_Dart:
		{
			mAdProtocolP = new DartProtocol(mNetCommInterfaceP, mCachePath, mProductName, getStopEvent());
		}
		break;
		case AdEngine::AdProvider_Invalid:
		default:
			LOG("AdEngine::Init Error:  Invalid AdProvider supplied");
			return false;
		break;
	}
	return true;
}

// Callbacks

void
__stdcall WONAPI::GetOptionsCallback(bool theResult, void* theDataP)
{
	((AdEngine*)theDataP)->OptionsDone(theResult);
}



