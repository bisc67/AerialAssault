
#include <time.h>
#pragma warning (disable : 4786)
#include "AdCache.h"
#include "AdUtils.h"
#include <direct.h>

using namespace WONAPI;

namespace CACHE_CONST
{
	const std::string MISC_DIR_NAME("Misc");
	const std::string REG_ROOT_KEY_NAME("AdAPI");
	const std::string REG_CACHE_KEY_NAME("Cache");
	const std::string REG_NAME_KEY_NAME("Name");
	const std::string REG_DOWNLOAD_KEY_NAME("DownLoad");
	const std::string REG_REQUEST_KEY_NAME("Request");

	const std::string REG_BACKSLASH("\\");
	const std::string REG_SOFTWARE_LEAF_NAME("SOFTWARE");
	const std::string REG_WON_LEAF_NAME("WON");
	const std::string REG_WON_AD_KEY_NAME(REG_SOFTWARE_LEAF_NAME+REG_BACKSLASH+REG_WON_LEAF_NAME+REG_BACKSLASH+REG_ROOT_KEY_NAME+REG_BACKSLASH+REG_CACHE_KEY_NAME);
};

// This class handles the saving of ad files to disk and maintaining the cache size
AdCache::AdCache(const std::string& theBaseCacheDirectory, 
				 unsigned long theFileUsageLifespan,     // Default 24 hours
				 unsigned long theFileExistenceLifespan, // Default 7 days
				 unsigned short theMaxFilesInCache) :    // Default 100 files
	mFileUsageLifespan(theFileUsageLifespan),
	mFileExistenceLifespan(theFileExistenceLifespan),
	mMaxFilesInCache(theMaxFilesInCache),
	mCleanupInProgress(false),
	ThreadBase()
{
	// Make sure we have a complete path
	if(theBaseCacheDirectory.find(":") == std::string::npos)
	{
		char aBuffer[256];
		mBaseCacheDirectory = getcwd(aBuffer, 256);
		mBaseCacheDirectory += "\\";
		mBaseCacheDirectory += theBaseCacheDirectory;
	}
	else
		mBaseCacheDirectory = theBaseCacheDirectory;

    mCleanupTriggerH = CreateEvent(NULL, false, false, NULL);
	CreateDirectory(mBaseCacheDirectory.c_str(), NULL);
	mRegistryKey.OpenNewKey(CACHE_CONST::REG_WON_AD_KEY_NAME, HKEY_LOCAL_MACHINE, true);
	LoadAllFromRegistry();
	CleanUpRegistry();
	startThread();
}

AdCache::~AdCache(void)
{
	stopThread();
	LOG("AdCache::~AdCache Ad Cache thread stopped");
	if(mCleanupTriggerH) CloseHandle(mCleanupTriggerH);
}

// If compiled to use single-threaded libraries,
// startThread() will fail to link.
#ifdef _MT
void AdCache::startThread()
{
	ThreadBase::startThread();
	ResetEvent(mCleanupTriggerH);
}
#endif

void AdCache::stopThread()
{
	SetEvent(mCleanupTriggerH);		
	ThreadBase::stopThread();
}

int
AdCache::threadProcess()
{
	HANDLE aWaitArray[2] = {getStopEvent(), getCleanupEvent()};
	while(true)
	{
		DWORD aResult = WaitForMultipleObjects(2, aWaitArray, false, INFINITE);
		if(aResult == WAIT_FAILED)
			return 1;

		switch(aResult - WAIT_OBJECT_0)
		{
		// Stop requested
		case 0:
			LOG("AdCache::threadProcess Stop thread requested.  Thread ID: " << getId());
			return 0;
			break;
		// Cleanup event
		case 1:
			LOG("AdCache::threadProcess Cleanup requested");
			if(!mCleanupInProgress)
				Cleanup();
			break;
		// Timer
		default:
			break;
		}
	}
}

bool 
AdCache::RequestFile(const std::string& theFileName, std::string& theFileAndPath)
{
	// Figure out just the file name
	int aPosition = theFileName.find_last_of("\\/");
	std::string aFileName;
	if(aPosition != std::string::npos)
		aFileName = theFileName.substr(aPosition+1);
	else
		aFileName = theFileName;

	// Crit section
	AdApiCommon::AutoCrit aCrit(mCrit);

	// Find if the file is in the cache
	bool isFileFound(false);
	FILE_MAP::iterator aFileNameIt = mFileNameMap.find(aFileName);
	if(aFileNameIt != mFileNameMap.end())
	{
		// Has it been in the cache too long?
		if(aFileNameIt->second.mTimeDownloaded + mFileExistenceLifespan < time(NULL))
		{
			LOG("AdCache::RequestFile File has been in Cache too long removing " << aFileName);
			// Find it in the time map and remove it
			for(TIME_MAP::iterator aSearchIt = mTimeFileRequestedMap.find(aFileNameIt->second.mTimeLastRequested); aSearchIt != mTimeFileRequestedMap.end(); aSearchIt++)
			{
				// We have a match
				if(aSearchIt->second == aFileNameIt->first)
				{
					mTimeFileRequestedMap.erase(aSearchIt);
					break;
				}
			}
			RemoveFromRegistry(aFileName);
			DeleteFile(aFileNameIt->second.mFullNameAndPath.c_str());
			mFileNameMap.erase(aFileNameIt);
		}
		else
		{
			// Make sure file has not been deleated by someone
			if (PathExists(aFileNameIt->second.mFullNameAndPath))
			{
				mCurrentlyRequestedFile = theFileAndPath = aFileNameIt->second.mFullNameAndPath;
				aFileNameIt->second.mTimeLastRequested = time(NULL);
				UpdateRegistryData(aFileName, aFileNameIt->second);
				isFileFound = true;
			}
			else
			{
				LOG("AdCache::RequestFile Alert - requested file has been deleted but not removed from registry.  Name: " << aFileName);
			}
		}
	}

	// File not in the cache
	if(!isFileFound)
		theFileAndPath = "";

	// Trigger a cleanup
	SetEvent(mCleanupTriggerH);
	return isFileFound;
}

bool 
AdCache::AddFile(const std::string& theFileAndPath)
{
	// Figure out just the file name
	int aPosition = theFileAndPath.find_last_of("\\/");
	std::string aFileName;
	if(aPosition != std::string::npos)
		aFileName = theFileAndPath.substr(aPosition+1);
	else
		aFileName = theFileAndPath;

	// Figure out the subdirectory
	std::string aSubDirectory(GetSubdirectory(aFileName));

	// Construct the full file and path
	std::string aFileDirectory = mBaseCacheDirectory;
	if(!mBaseCacheDirectory.empty())
		aFileDirectory += "\\";
	aFileDirectory += aSubDirectory + "\\";
	CreateDirectory(aFileDirectory.c_str(), NULL);

	// Construct the file data object
	FILE_DATA aFileData;
	aFileData.mTimeDownloaded = aFileData.mTimeLastRequested = time(NULL);
	aFileData.mFullNameAndPath = aFileDirectory + aFileName;

	bool isSuccess(false);
	// Move file into cache, insert into map, and add to registry
	// Note: MoveFileEx does not work in Win9x
	//if(MoveFileEx(theFileAndPath.c_str(), aFileData.mFullNameAndPath.c_str(), MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) == TRUE)
	if(MoveFileToCache(theFileAndPath.c_str(), aFileData.mFullNameAndPath.c_str()))
	{
		mFileNameMap[aFileName] = aFileData;
		mTimeFileRequestedMap.insert(TIME_MAP::value_type(aFileData.mTimeLastRequested, aFileName));
		UpdateRegistryData(aFileName, aFileData);
		isSuccess = true;
	}
	

	// Trigger a cleanup
	SetEvent(mCleanupTriggerH);
	return isSuccess;
}

std::string 
AdCache::GetSubdirectory(const std::string& theFileName)
{
	// Find extension and use directory named same as extension
	int aPosition = theFileName.find_last_of(".");
	std::string aDirectoryName;
	if(aPosition != std::string::npos)
		aDirectoryName = theFileName.substr(aPosition+1);
	else
		aDirectoryName = CACHE_CONST::MISC_DIR_NAME;

	return aDirectoryName;
}

void 
AdCache::UpdateRegistryData(const std::string& theFileName, const FILE_DATA& theFileData)
{
	AdApiCommon::RegKey aSubKey;
	if(mRegistryKey.GetSubKey(theFileName, aSubKey, true))
	{
		aSubKey.SetValue(CACHE_CONST::REG_NAME_KEY_NAME, theFileData.mFullNameAndPath);
		aSubKey.SetValue(CACHE_CONST::REG_DOWNLOAD_KEY_NAME, theFileData.mTimeDownloaded);
		aSubKey.SetValue(CACHE_CONST::REG_REQUEST_KEY_NAME, theFileData.mTimeLastRequested);
	}
}

void 
AdCache::RemoveFromRegistry(const std::string& theFileName)
{
	mRegistryKey.DeleteSubKey(theFileName);
}

void 
AdCache::GetFileInfoFromRegistry(const AdApiCommon::RegKey& aSubKey)
{
	std::string aFileName = aSubKey.GetLeafName();

	FILE_DATA aFileData;
	aSubKey.GetValue(CACHE_CONST::REG_NAME_KEY_NAME, aFileData.mFullNameAndPath);

	unsigned long aTempValue;
	aSubKey.GetValue(CACHE_CONST::REG_DOWNLOAD_KEY_NAME, aTempValue);
	aFileData.mTimeDownloaded = aTempValue;

	aSubKey.GetValue(CACHE_CONST::REG_REQUEST_KEY_NAME, aTempValue);
	aFileData.mTimeLastRequested = aTempValue;

	mFileNameMap[aFileName] = aFileData;
	mTimeFileRequestedMap.insert(TIME_MAP::value_type(aFileData.mTimeLastRequested, aFileName));
}

void 
AdCache::LoadAllFromRegistry(void)
{
	// Crit section
	AdApiCommon::AutoCrit aCrit(mCrit);
	AdApiCommon::RegKey aSubKey;
	// Cycle thru and load all keys
	if(mRegistryKey.GetFirstSubKey(aSubKey) == AdApiCommon::RegKey::Ok)
	{
		GetFileInfoFromRegistry(aSubKey);
		while(mRegistryKey.GetNextSubKey(aSubKey) == AdApiCommon::RegKey::Ok)
			GetFileInfoFromRegistry(aSubKey);
	}
}

// Remove cache entries from registry that do not really exist
void 
AdCache::CleanUpRegistry()
{
	// Crit section
	AdApiCommon::AutoCrit aCrit(mCrit);

	for (FILE_MAP::iterator aItr = mFileNameMap.begin();aItr != mFileNameMap.end();)
	{
		// Check to see if file really does exist in cache directory
		if (!PathExists(aItr->second.mFullNameAndPath))
		{
			// This file does not exist deleate it from registry
			LOG("AdCache::CleanUpRegistry Alert - Removing file from registry that has been deleted by other means.  Name: " << aItr->first);
			RemoveFromRegistry(aItr->first);
			TIME_MAP::iterator aTimeItr = mTimeFileRequestedMap.find(aItr->second.mTimeLastRequested);
			mTimeFileRequestedMap.erase(aTimeItr);
			// This is done due to differences in ANSI STL
			FILE_MAP::iterator aEraseItr = aItr++;
			mFileNameMap.erase(aEraseItr);
		}
		else
		{
			aItr++;
		}
	}
}

void 
AdCache::Cleanup(void)
{
	// Is there any reason to perform cleanup
	if(mFileNameMap.size() == 0 || (mMaxFilesInCache == 0 && mFileUsageLifespan == 0))
		return;

	// To prevent cleanup requests from queing up while we are doing cleanup
	mCleanupInProgress = true;

	int aMinimumRemovedFiles = (mMaxFilesInCache > 0) ? (mFileNameMap.size() - mMaxFilesInCache) : 0;
	if(aMinimumRemovedFiles < 0) 
		aMinimumRemovedFiles = 0;
	time_t anOldestFileTimeAllowedForUsage = time(NULL) - mFileUsageLifespan;

	FILE_MAP::iterator aFileNameMapIt = mFileNameMap.begin();
	// Are we removing any files
	if(aMinimumRemovedFiles > 0 || (mFileUsageLifespan > 0 && (aFileNameMapIt->second.mTimeLastRequested < anOldestFileTimeAllowedForUsage)))
	{
		int aNumberOfFilesRemoved(0);
		bool DeleteThisFile(false);
		// Go thru files in time order
		for(TIME_MAP::iterator aFileTimeIt = mTimeFileRequestedMap.begin(); aFileTimeIt != mTimeFileRequestedMap.end();)
		{
			aFileNameMapIt = mFileNameMap.find(aFileTimeIt->second);
			// If file is older than allowed lifespan
			if(aFileNameMapIt != mFileNameMap.end() && aFileNameMapIt->second.mTimeLastRequested < anOldestFileTimeAllowedForUsage)
				DeleteThisFile = true;

			// If too many files
			if(aNumberOfFilesRemoved < aMinimumRemovedFiles)
				DeleteThisFile = true;

			// Don't remove if it is the file currently being used
			if(DeleteThisFile && aFileTimeIt->second != mCurrentlyRequestedFile)
			{
				// Crit section
				AdApiCommon::AutoCrit aCrit(mCrit);

				// Remove file
				if(aFileNameMapIt != mFileNameMap.end())
				{
					LOG("AdCache::Cleanup File removed from cache.  Name:  " << aFileNameMapIt->first);
					RemoveFromRegistry(aFileNameMapIt->first);
					DeleteFile(aFileNameMapIt->second.mFullNameAndPath.c_str());
					mFileNameMap.erase(aFileNameMapIt);
				}
				// Done this way due to differences in ANSI STL
				TIME_MAP::iterator aEraseItr = aFileTimeIt++;
				mTimeFileRequestedMap.erase(aEraseItr);
				aNumberOfFilesRemoved++;
			}
			else
				aFileTimeIt++;

			DeleteThisFile = false;

			// Break early if we have met all the minimum storage requirements
			if(aFileTimeIt->first >= anOldestFileTimeAllowedForUsage && aNumberOfFilesRemoved >= aMinimumRemovedFiles)
				break;
		}
	}
	mCleanupInProgress = false;
}

bool
AdCache::MoveFileToCache(const char* theExistingFilePath, const char* theNewFilePath)
{
	if (!MoveFile(theExistingFilePath, theNewFilePath))
	{
		LOG("AdCache::MoveFileToCache Alert - MoveFile failed From: " << theExistingFilePath << " To: " << theNewFilePath << " Error: " << GetLastError());
		// Try to copy and delete file
		if(!CopyFile(theExistingFilePath, theNewFilePath, false))
		{
			LOG("AdCache::MoveFileToCache Error - Copy file failed Error: " << GetLastError());
			return false;
		}
		DeleteFile(theExistingFilePath);
	}
	return true;
}
