#pragma warning(disable:4786)

#include <assert.h>
#include "AdProtocol.h"
#include "AdDisplay.h"
#include "AdUtils.h"
#include "AdOptions.h"
#include "AdSurf.h"

using namespace WONAPI;
using namespace std;

// callback that uses callback info
void
__stdcall WONAPI::CallbackInfoCallback(bool theResult, void* theDataP)
{
	((CallbackInfo*)theDataP)->Callback(theResult);
}

AdProtocol::AdProtocol(NetCommInterface* theNetCommInterfaceP, const string& theCachePath, const string& theProductName, HANDLE theStopEvent) :
	mNetCommInterfaceP(theNetCommInterfaceP),
	mMaxDownloadRate(0), // Not used currently
	mStopEvent(theStopEvent),
	mAdCacheP(NULL),
	mAdReady(false),
	mCurrAdDest(),
	mCurrAdPath(),
	mCurrToolTip(),
	mProductName(theProductName),
	mAdServerName(),
	mAdServerPort(0),
	mAdTag()
{
	if (mNetCommInterfaceP && mStopEvent)
	{
		unsigned long aFileUsageLifespan;
		unsigned long aFileExistenceLifespan;
		unsigned long aMaxFilesInCache;

		// Load ad cache options
		AdOptions::GetLongOption(OptionName_FileUsageLifespan, OptionDefault_FileUsageLifespan, aFileUsageLifespan);
		AdOptions::GetLongOption(OptionName_FileExistenceLifespan, OptionDefault_FileExistenceLifespan, aFileExistenceLifespan);
		AdOptions::GetLongOption(OptionName_MaxFilesInCache, OptionDefault_MaxFilesInCache, aMaxFilesInCache);

		mAdCacheP = new AdCache(theCachePath, aFileUsageLifespan, aFileExistenceLifespan, aMaxFilesInCache);

		// Get other options
		AdOptions::GetStringOption(OptionName_AdServer, OptionDefault_AdServer, mAdServerName);
		AdOptions::GetLongOption(OptionName_AdServerPort, OptionDefault_AdServerPort, mAdServerPort);
		AdOptions::GetStringOption(OptionName_AdTag, OptionDefault_AdTag, mAdTag);
	}
	else
	{
		LOG("AdProtocol::AdProtocol Error:  Null pointers passed into contstructor");
		assert(0);
	}
}

AdProtocol::~AdProtocol()
{
	delete mAdCacheP;
	mAdCacheP = NULL;
}


// Public virtual methods
bool
AdProtocol::LaunchAdDestination(AdInfoConstPtr theAdInfoPtr, int theShowCmd)
{
	return LaunchBrowser(theAdInfoPtr->mAdDest, theShowCmd);
}

// Private methods
bool
AdProtocol::LaunchBrowser(const string& theAdDest, int theShowCmd)
{
	if (theAdDest.length() != 0)
	{
		// Launch default browser
		// ** Might want to make sure that string is a URL
		LOG("AdProtocol::LaunchBrowser Launching browser to location " << theAdDest.c_str());
		//ShellExecute(NULL, NULL, theAdDest.c_str(), NULL, NULL, theShowCmd);
		Browse(theAdDest.c_str(), (WORD)theShowCmd);
		return true;
	}
	else
	{
		LOG("AdProtocol::LaunchBrowser Error: Launch of ad destination attemped on empty destination");
		return false;
	}
}

bool 
AdProtocol::CreateAdFile(const string& theFileName, const RawBuffer& theContent)
{
	// Might want to do cache cleanup here

	if (theFileName.length() == 0)
	{
		return false;
	}

	string aAdPath = theFileName;
	if (!FindFile(aAdPath))
	{
		return false;
	}

	// Find a temporary path to put file in
	aAdPath = mAdCacheP->GetCachePath() + "\\" + aAdPath;

	// Create the folder first
	if (!CreateFullPath(aAdPath))
	{
		LOG("AdProtocol::CreateAdFile Error:  Unable to create full path " << aAdPath);
		return false;
	}

	// Create the file
	HANDLE aAdFile = ::CreateFile(aAdPath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (aAdFile == INVALID_HANDLE_VALUE)
	{
		LOG("AdProtocol::CreateAdFile Error:  Failed to create ad file " << aAdPath.c_str() << " Err: "<< ::GetLastError());
		return false;
	}

	// Write the file
	DWORD aBytesWritten = 0;
	BOOL aSuccess = ::WriteFile(aAdFile, theContent.c_str(), theContent.length(), &aBytesWritten, NULL);
	::CloseHandle(aAdFile);

	if (!aSuccess || aBytesWritten != theContent.length())
	{
		LOG("AdProtocol::CreateAdFile Error: Failed to write data to ad file " << aAdPath.c_str() << " Err: " << ::GetLastError());
		::DeleteFile(aAdPath.c_str());
		return false;
	}

	// Ad file to cache
	if (!mAdCacheP->AddFile(aAdPath))
	{
		LOG("AdProtocol::CreateAdFile Error:  Unable to add file to cache " << aAdPath);
		return false;
	}
	// Check file out from cashe for use
	if (!mAdCacheP->RequestFile(theFileName, aAdPath))
	{
		LOG("AdProtocol::CreateAdFile Error:  Unable to get file from cache " << aAdPath);
		return false;
	}

	SetCurrAdPath(aAdPath);

	return true;
}

// Create full filename with path
const string 
AdProtocol::GenerateAdPath(const string& theFileName)
{
	string aFileName = theFileName;
	AddReplaceFileExt(aFileName);

	// Get base directory
	string aAdPath = mAdCacheP->GetCachePath();

	// Ad filename
	aAdPath += aFileName;

	return aAdPath; 
}

// true if image source is a file and file extension matches an allowed extension
bool 
AdProtocol::IsValidFile(const string& theImageSource)
{
	// Get the extension
	string aExtStr = GetExtension(theImageSource);
	if (aExtStr.length() == 0)
	{
		return false;
	}

	// Currently the only acceptable extension is .gif
	// May need to add more later
	return (aExtStr == "gif");
}

void 
AdProtocol::AddReplaceFileExt(string& theFilePath)
{
	// Replace the extension if it exists with the proper extension
	string::size_type aDot = theFilePath.rfind('.');
	if (aDot != string::npos) // There currently exists an extention
	{
		theFilePath.resize(aDot+1);
	}
	else // Currently no extention
	{
		theFilePath += ".";
	}
	
	// ** Currently we only support one file type.  This will have to be changed when we support more
	theFilePath += "gif";
}

// Returns an empty string if the given file name does not have an extension
string 
AdProtocol::GetExtension(const string& theFile)
{
	string::size_type aDot = theFile.rfind('.');
	if (aDot == string::npos)
	{
		return string();
	}

	return (theFile.substr(aDot+1));
}

void
AdProtocol::MakeValidFileName(string& theFileName)
{
	StripInvalidCharacters(theFileName);
	if (!IsValidFile(theFileName))
	{
		AddReplaceFileExt(theFileName);
	}
}





