
#pragma warning(disable:4786)

#include <errno.h>
#include "AdUtils.h"
#include "AdOptions.h"

using namespace WONAPI;
using namespace std;
using namespace AdApiCommon;

// Constants

// Files
const char* WONAPI::LocalOptions_File = "AdApi.cfg";			// Local options file
const char* WONAPI::RemoteOptions_File = "AdApiRem.cfg";		// Remote options file

// Options
// Note: Options should not be retrieved until remote option file has been downloaded
const char* WONAPI::OptionName_FileUsageLifespan		= "FileUsageLifespan";
const char* WONAPI::OptionName_FileExistenceLifespan	= "FileExistenceLifespan";
const char* WONAPI::OptionName_MaxFilesInCache			= "MaxFilesInCache";
const char* WONAPI::OptionName_AdServer					= "AdServer";
const char* WONAPI::OptionName_AdServerPort				= "AdServerPort";
const char* WONAPI::OptionName_AdTag					= "AdTag";
const char* WONAPI::OptionName_AdRotation				= "AdRotation";
const char* WONAPI::OptionName_AdProvider				= "AdProvider";
const char* WONAPI::OptionName_AllowSceneSkipping		= "AllowSceneSkipping";
const char* WONAPI::OptionName_DefaultLink				= "DefaultLink";

// Default values
const unsigned long WONAPI::OptionDefault_FileUsageLifespan		= 24*3600;		// 1 day
const unsigned long WONAPI::OptionDefault_FileExistenceLifespan	= 7*24*3600;	// 7 days
const unsigned long WONAPI::OptionDefault_MaxFilesInCache		= 100;
const char*			WONAPI::OptionDefault_AdServer				= "";
const unsigned long	WONAPI::OptionDefault_AdServerPort			= 80;
const char*			WONAPI::OptionDefault_AdTag					= "";
const unsigned long WONAPI::OptionDefault_AdRotation			= 30;
const char*			WONAPI::OptionDefault_AdProvider			= "";			// Default
const bool			WONAPI::OptionDefault_AllowSceneSkipping	= false;

// Utils constants
const int Max_OptionSize = 256;

////////////////////////////////////////////////////////////////////////////////////
// Options

// Static member decl
RegKey AdOptions::mRegKey;
string AdOptions::mOptionsFileFullPath;
OptionsMap AdOptions::mOptionsMap;
//string AdOptions::mRemoteOptionsFileFullPath;

bool
AdOptions::Init()
{
	//mRemoteOptionsFileFullPath = theRemoteOptionsFileFullPath;

	GetModulePath(mOptionsFileFullPath);
	mOptionsFileFullPath += LocalOptions_File;

	return mRegKey.OpenNewKey("SOFTWARE\\WON\\AdAPI", HKEY_LOCAL_MACHINE, true) == RegKey::Ok;
}

void
AdOptions::Shutdown()
{
	mRegKey.CloseKey();
}

void
AdOptions::ReadOptionsFile(const std::string& theOptionFileName)
{
  HANDLE aOptionFile;
	if ((aOptionFile = CreateFile(theOptionFileName.c_str(),
		       	   		           GENERIC_READ,
			   			           FILE_SHARE_READ,
			   			           NULL,  
			   			           OPEN_EXISTING,
			   			           0,
			   			           NULL)) != INVALID_HANDLE_VALUE)
	{
		// Read entire file into a buffer
		// File is ascii
		string aFileBuffer;
		
		char aBuff[1024];
		DWORD aBytesRead;

		int aResult = ReadFile(aOptionFile, aBuff, 1024, &aBytesRead, NULL);
		while (aResult && aBytesRead)
		{
			aFileBuffer.append(aBuff, aBytesRead);
			aResult = ReadFile(aOptionFile, aBuff, 1024, &aBytesRead, NULL);
		}

		CloseHandle(aOptionFile);

		// Read whole file
		string::size_type aLineStart = 0;
		string::size_type aLineEnd = 0;
		while(aLineStart < aFileBuffer.size())
		{
			// Read an option - Read until end of line
			aLineEnd = aFileBuffer.find('\n', aLineStart);
			if (aLineEnd == string::npos) // end of file
			{
				aLineEnd = aFileBuffer.size() + 1;
			}

			string::size_type aEqualSign = aFileBuffer.find('=', aLineStart);
			
			if ((aEqualSign != string::npos) && (aEqualSign <= aLineEnd)) // This is an option line
			{
				string aOptionName = aFileBuffer.substr(aLineStart, aEqualSign - aLineStart);
				string aOptionValue = aFileBuffer.substr(aEqualSign + 1, aLineEnd - aEqualSign - 2);
				pair<OptionsMap::iterator, bool> aResult = mOptionsMap.insert(OptionsMap::value_type(aOptionName, aOptionValue));
				if (aResult.second = false) // already exists
				{
					aResult.first->second = aOptionValue;
				}

			}
			// Get ready for next line
			aLineStart = aLineEnd + 1;
		}
	}
}


bool
AdOptions::GetStringOption(const string& theOptionNameR, const string& theDefaultValR, string& theOptionValR)
{
	char aBuff[Max_OptionSize];

	// Get from options map
	OptionsMap::const_iterator aItr = mOptionsMap.find(theOptionNameR);
	if (aItr != mOptionsMap.end())
	{
		theOptionValR = aItr->second;
		return true;
	}
		
	// Check local options file
	GetPrivateProfileString("AdDisplay", theOptionNameR.c_str(),"WON_ERROR", aBuff, Max_OptionSize, mOptionsFileFullPath.c_str());
	if (strcmp(aBuff, "WON_ERROR"))
	{
		// Succeeded
		theOptionValR = aBuff;
		return true;
	}

	// Check registry
	if (mRegKey.GetValue(theOptionNameR, theOptionValR) != RegKey::Ok) 
	{
		theOptionValR = theDefaultValR;
		return true;
	}

	// Unable to find option
	return false;
}

bool
AdOptions::GetLongOption(const string& theOptionNameR, unsigned long theDefaultValR, unsigned long& theOptionValR)
{
	UINT aTemp;

	// Get from options map
	OptionsMap::const_iterator aItr = mOptionsMap.find(theOptionNameR);
	if (aItr != mOptionsMap.end())
	{
		char* aEndP = NULL;
		theOptionValR = strtoul(aItr->second.c_str(), &aEndP, 10);
		return ((*aEndP == '\0') && ((theOptionValR != ULONG_MAX) || (errno != ERANGE)));
	}
		
	// Check local options file
	aTemp = GetPrivateProfileInt("AdDisplay", theOptionNameR.c_str(),-99999, mOptionsFileFullPath.c_str());
	if (aTemp != -99999)
	{
		// Succeeded
		theOptionValR = (long)aTemp;
		return true;
	}

	// Check Registry
	if (mRegKey.GetValue(theOptionNameR, theOptionValR) != RegKey::Ok)
	{
		theOptionValR = theDefaultValR;
	}
	return false;
}

bool
AdOptions::GetBoolOption(const string& theOptionNameR, bool theDefaultValR, bool& theOptionValR)
{
	char aBuff[Max_OptionSize];

	// Get from options map
	OptionsMap::const_iterator aItr = mOptionsMap.find(theOptionNameR);
	if (aItr != mOptionsMap.end())
	{
		 theOptionValR = IsTrue(aItr->second.c_str());
		 return true;
	}

	// Check local options file
	GetPrivateProfileString("AdDisplay", theOptionNameR.c_str(),"WON_ERROR", aBuff, Max_OptionSize, mOptionsFileFullPath.c_str());
	if (strcmp(aBuff, "WON_ERROR"))
	{
		// Succeeded
		theOptionValR = IsTrue(aBuff);
		return true;
	}

	string aString;
	// Check registry
	if (mRegKey.GetValue(theOptionNameR, aString) == RegKey::Ok) 
	{
		theOptionValR = IsTrue(aString.c_str());
		return true;
	}
	else
	{
		theOptionValR = theDefaultValR;
		return true;
	}

	// Unable to find option
	return false;
}

// Remember to delete binary data when done
bool
AdOptions::GetBinaryOption(const string& theOptionNameR, unsigned char* &theOptionValR, unsigned long& theLengthR)
{
	// Check registry only
	return mRegKey.GetValue(theOptionNameR, theOptionValR, theLengthR) == RegKey::Ok;
}

bool
AdOptions::IsTrue(const char* theCmpString)
{
	string aString = ToLower(theCmpString);

	return ((aString == "1") || 
			(aString == "true") ||
			(aString == "yes"));
}

