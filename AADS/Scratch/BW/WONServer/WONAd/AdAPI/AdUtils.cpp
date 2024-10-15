
// Contains utility functions and classes for AdApi library

#include <algorithm>
#include "Windows.h"
#include "AdUtils.h"

using namespace WONAPI;
using namespace std;
using namespace AdApiCommon;


////////////////////////////////////////////////////////////////////////////////////
// For logging
#ifdef ADAPI_LOGGING

// debugstringstream
std::streamsize debugstringstream::debugstringstreambuf::xsputn(const char* text, std::streamsize n)
{
	if (n)
		s.append(text, n);
	return n;
}


int debugstringstream::debugstringstreambuf::overflow(int ch)
{
	if (ch > 0x00FF) // ??
		return 0;
	
	s.append(1, (char)ch);
	
	return 1;
}


std::streamsize debugstringstream::debugstringstreambuf::xsgetn(char* text, std::streamsize n)
{
	std::streamsize readSize = 0;
	if (n)
	{
		std::streamsize remaining = s.size() - readPos;
		readSize = (n > remaining) ? remaining : n;
		if (readSize)
		{
			memcpy(text, s.data() + readPos, readSize);
			readPos += readSize;
		}
	}
	return readSize;
}


int debugstringstream::debugstringstreambuf::underflow()
{
	if (hasReadAheadChar)
		return readAheadChar;
	
	if (s.size() == readPos)
		return EOF;

	readAheadChar = *(s.data() + readPos);
	readPos++;
	hasReadAheadChar = true;

	return readAheadChar;
}


int debugstringstream::debugstringstreambuf::uflow()
{
	if (hasReadAheadChar)
	{
		hasReadAheadChar = false;
		return readAheadChar;
	}

	if (s.size() == readPos)
		return EOF;

	return *(s.data() + readPos++);
}


debugstringstream::debugstringstream()
	: sb(), iostream(0)
{
	init(&sb);
}

// Static member declarations
debugstringstream AdLog::mLogStream;
std::ofstream AdLog::mLogFile;
bool AdLog::mLogging = false;
bool AdLog::mInitialized = false;
//bool AdLog::mCritAllocated = false;
//AdApiCommon::CriticalSection*	AdLog::mCriticalSectionP; 

bool AdLog::mLogTime = false;

AdLog::~AdLog()
{
	//delete mCriticalSectionP;
	//mCriticalSectionP = NULL;
}

CriticalSection&
AdLog::GetCrit()
{
	// This done this way so that if ad log is statically inited that it will still work
	static CriticalSection crit;

	return crit;
	/*AutoCrit autoCrit(crit);

	if (!mCritAllocated)
	{
		mCriticalSectionP = new CriticalSection;
		mCritAllocated = true;
	}
	
	return *mCriticalSectionP;*/
}

bool
AdLog::GetInitialized()
{
	return mInitialized;
}

bool 
AdLog::Init( const char* theLogfilePath )
{	
	mLogStream.clear();

	if (!SetLogfile(theLogfilePath))
	{
		return false;
	}

	mInitialized = true;

	return true;
}

void 
AdLog::Shutdown()
{
	AutoCrit aAutoCrit(GetCrit());
	mInitialized = false;
	mLogFile.close();
	mLogging = false;
}

void 
AdLog::SetLogging(bool theLogging)
{
	AutoCrit aAutoCrit(GetCrit());
	mLogging = theLogging; 
}

void
AdLog::SetLoggingOption(AdLog::AdLogOption theAdLogOption)
{
	switch (theAdLogOption) 
	{
		case AdLog::AdLogOption_LogTime:
		{
			mLogTime = true;
		}
		break;
		default:
		break;
	}
}

bool 
AdLog::SetLogfile( const char* theLogfilePath )
{
	AutoCrit aAutoCrit(GetCrit());

	if (!CreateFullPath(theLogfilePath))
	{
		return false; // Unable to create path
	}

	if (mLogFile.is_open())
	{
		mLogFile.close();
	}
	if (!IsFileOpen(theLogfilePath))
	{
		mLogFile.open(theLogfilePath, ios::out | ios::ate);

		mLogging = mLogFile.is_open();
	}
	else
	{
#ifdef _DEBUG
		OutputDebugString("AdLog::SetLogfile Error: Unable to open log file. Logging stopped");
#endif
		mLogging = false;
	}

	if (mLogging)
	{
		SYSTEMTIME SystemTime;
		GetLocalTime(&SystemTime);

		GetLogStream() << "*** Ad API Test *** " << SystemTime.wMonth << "/" << SystemTime.wDay << " - " << SystemTime.wHour << ":" << SystemTime.wMinute << ":" << SystemTime.wSecond; 
		Flush();
	}

	return mLogging;
}

ostream& 
AdLog::GetLogStream()
{
	if (!mLogTime)
	{
		mLogStream << "AdApi: ";
	}
	else // log the time
	{
		SYSTEMTIME SystemTime;
		GetLocalTime(&SystemTime);

		mLogStream <<  SystemTime.wHour << ":" << SystemTime.wMinute << ":" << SystemTime.wSecond << ":" << SystemTime.wMilliseconds << " AdApi: ";
	}
	return mLogStream;
}

void 
AdLog::Flush()
{
	mLogStream << endl;

	if (mLogging && mLogFile.is_open())
	{
		mLogFile << mLogStream.str() << flush;
	}

#ifdef _DEBUG
	OutputDebugString(mLogStream.str().c_str());
#endif

	mLogStream.clear(); 
}

// Had to ad this because ofstream will open the same file for writing in two places
bool 
AdLog::IsFileOpen( const char* theFilePath )
{
	//_fopen
	HANDLE aAdFile = ::CreateFile(theFilePath, GENERIC_READ, 0, NULL, OPEN_ALWAYS, 0, NULL);
	if (aAdFile != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(aAdFile);
		return false;
	}
	return true;
}

#endif // ADAPI_LOGGING

////////////////////////////////////////////////////////////////////////////////////
// File manipulation routines

// Check for existance of path
bool 
WONAPI::PathExists(const string& thePath)
{
	WIN32_FIND_DATA aFindData;
	
	// Strip of any last \'s
	string::size_type aSlash = thePath.find_last_of('\\');
	if (aSlash == string::npos)
	{
		aSlash = thePath.find_last_of('/');
	}
	HANDLE aPathHandle;
	if (aSlash < thePath.size()-1) // \ is not the last item leave it
	{
		aPathHandle = ::FindFirstFile(thePath.c_str(), &aFindData);
	}
	else // Strip off last slash before compare
	{
		aPathHandle = ::FindFirstFile(thePath.substr(0, aSlash).c_str(), &aFindData);
	}
	
	
	if (aPathHandle != INVALID_HANDLE_VALUE)
	{
		::FindClose(aPathHandle);
		return true;
	}
	return false;
}

// GetPath
// 
// Gets a path from a filename

bool
WONAPI::FindPath(string& theFilePath)
{
	// Extract the path
	// Determine path by stripping off everything after the
	// final backslash or slash
	string::size_type aSlash = theFilePath.find_last_of('\\');
	if (aSlash == string::npos)
	{
		aSlash = theFilePath.find_last_of('/');
	}

	// No backslash or slash; this is not a path!
	if (aSlash == string::npos)
	{
		return false;
	}

	theFilePath.resize(aSlash+1);
	return true;
}

bool
WONAPI::FindFile(string& theFilePath)
{
	// Extract the file
	// Determine file by stripping off everything before the
	// final backslash or slash
	string::size_type aSlash = theFilePath.find_last_of('\\');
	if (aSlash == string::npos)
	{
		aSlash = theFilePath.find_last_of('/');
	}

	// No backslash or slash; this is not a path!
	if (aSlash == string::npos)
	{
		return false;
	}

	//theFilePath.resize(aSlash+1);
	theFilePath = theFilePath.substr(aSlash+1, string::npos);
	return true;
}

// Create Full path
//
// Creates directories along the full path specified

bool 
WONAPI::CreateFullPath(const string& thePath)
{
	string aOutputDir = thePath;

	// Not a path error
	if (!FindPath(aOutputDir) || aOutputDir.length() == 0)
	{
		return false;
	}

	// If directory exists, we're done
	if (WONAPI::PathExists(aOutputDir))
	{
		return true;
	}

	// Generate directories all the way down the path
	for (string::size_type aMarker = 0; aMarker != string::npos; )
	{
		++aMarker;
		string::size_type aBackSlash = aOutputDir.find('\\', aMarker);
		string::size_type aSlash = aOutputDir.find('/', aMarker);
		aMarker = min(aBackSlash, aSlash);
		string aDir = aOutputDir.substr(0, aMarker);
		if (!WONAPI::PathExists(aDir))
		{
			BOOL aSuccess = ::CreateDirectory(aDir.c_str(), NULL);

			// It's OK to have errors "creating" drives
			if (!aSuccess && aDir[1] != ':')
			{
				LOG("Unable to create " << aOutputDir.c_str() << " err " << ::GetLastError());
				return false;
			}
		}
	}
	return true;
}

bool
WONAPI::GetModulePath(string& theModulePath)
{
	char aPath[256];
	if (GetModuleFileName(NULL,aPath,256))
	{
		theModulePath = aPath;
		if (WONAPI::FindPath(theModulePath))
		{
			return true;
		}
	}
	return false;
}

// Same as string::find(), but case insensitive
string::size_type 
WONAPI::FindNoCase(const string& theSearch, const string& theFind, string::size_type theInitialPos, string::size_type theMaxChars)
{
	string::size_type aLen = theSearch.length();
	if (theMaxChars == string::npos)
		theMaxChars = theFind.length();

	if (theMaxChars == 0 && theInitialPos <= aLen)
		return (theInitialPos);

	const char *aSearchP = theSearch.c_str();
	const char *aFindP = theFind.c_str();
	string::size_type aScan = aLen - theInitialPos;
	if (theInitialPos < aLen && theMaxChars <= aScan)
	{
		const char *aXP;
		const char *aYP;
		const char *aAP;
		const char *aBP;

		for (aScan -= theMaxChars - 1, aYP = aSearchP + theInitialPos;
			 aAP = (const char*)::memchr(aYP, tolower(*aFindP), aScan),
			 aBP = (const char*)::memchr(aYP, toupper(*aFindP), aScan),
			 (aXP = (aAP == 0) ? aBP : ((aBP == 0) ? aAP : (min(aAP, aBP)))) != 0;
			aScan -= aXP - aYP + 1, aYP = aXP + 1)
		{
			if (::_strnicmp(aXP, aFindP, theMaxChars) == 0)
				return (aXP - aSearchP);
		}
	}
	return (string::npos);
}

string 
WONAPI::ToLower( const std::string &theString )
{
    string aString( theString );
    std::transform<std::string::iterator, std::string::iterator, int (__cdecl *)(int) >( aString.begin(),aString.end(),aString.begin(), tolower );
    return( aString );
}

// Convert all backslashes to forward slashes
void 
WONAPI::SlashesToForwardslashes(string& theConvert)
{
	// Replace all slashes with forward slashes
	for (string::size_type n = 0; n < theConvert.length(); ++n)
	{
		if (theConvert[n] == '\\')
			theConvert[n] = '/';
	}
}

void 
WONAPI::StripInvalidCharacters(string& theConvert)
{
	// Replace all invalid characters with -
	for (string::size_type n = 0; n < theConvert.length(); ++n)
	{
		if (theConvert[n] == '?') // Only invalid at this point is ?
			theConvert[n] = '-';
	}
}



	
