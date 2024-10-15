
#include "HTTPUtil.h"
#include "HTTPGetReq.h"

using namespace std;
using namespace WONAPI;

////////////////////////////////////////////////////////////////////
// Constants

// 8 K download chunks
#define HTTP_DOWNLOAD_BLOCK_SIZE		(1024 * 8)


static const char* monthTable[] = {	"Jan", "Feb", "Mar",
									"Apr", "May", "Jun",
									"Jul", "Aug", "Sep",
									"Oct", "Nov", "Dec" };


static const unsigned long Jan_Value = ('J' * 0x00010000) + ('a' * 0x00000100) + 'n';
static const unsigned long JAN_Value = ('J' * 0x00010000) + ('A' * 0x00000100) + 'N';
static const unsigned long Feb_Value = ('F' * 0x00010000) + ('e' * 0x00000100) + 'b';
static const unsigned long FEB_Value = ('F' * 0x00010000) + ('E' * 0x00000100) + 'B';
static const unsigned long Mar_Value = ('M' * 0x00010000) + ('a' * 0x00000100) + 'r';
static const unsigned long MAR_Value = ('M' * 0x00010000) + ('A' * 0x00000100) + 'R';
static const unsigned long Apr_Value = ('A' * 0x00010000) + ('p' * 0x00000100) + 'r';
static const unsigned long APR_Value = ('A' * 0x00010000) + ('P' * 0x00000100) + 'R';
static const unsigned long May_Value = ('M' * 0x00010000) + ('a' * 0x00000100) + 'y';
static const unsigned long MAY_Value = ('M' * 0x00010000) + ('A' * 0x00000100) + 'Y';
static const unsigned long Jun_Value = ('J' * 0x00010000) + ('u' * 0x00000100) + 'n';
static const unsigned long JUN_Value = ('J' * 0x00010000) + ('U' * 0x00000100) + 'N';
static const unsigned long Jul_Value = ('J' * 0x00010000) + ('u' * 0x00000100) + 'l';
static const unsigned long JUL_Value = ('J' * 0x00010000) + ('U' * 0x00000100) + 'L';
static const unsigned long Aug_Value = ('A' * 0x00010000) + ('u' * 0x00000100) + 'g';
static const unsigned long AUG_Value = ('A' * 0x00010000) + ('U' * 0x00000100) + 'G';
static const unsigned long Sep_Value = ('S' * 0x00010000) + ('e' * 0x00000100) + 'p';
static const unsigned long SEP_Value = ('S' * 0x00010000) + ('E' * 0x00000100) + 'P';
static const unsigned long Oct_Value = ('O' * 0x00010000) + ('c' * 0x00000100) + 't';
static const unsigned long OCT_Value = ('O' * 0x00010000) + ('C' * 0x00000100) + 'T';
static const unsigned long Nov_Value = ('N' * 0x00010000) + ('o' * 0x00000100) + 'v';
static const unsigned long NOV_Value = ('N' * 0x00010000) + ('O' * 0x00000100) + 'V';
static const unsigned long Dec_Value = ('D' * 0x00010000) + ('e' * 0x00000100) + 'c';
static const unsigned long DEC_Value = ('D' * 0x00010000) + ('E' * 0x00000100) + 'C';

////////////////////////////////////////////////////////////////////
// Public methods

HTTPGetReq::HTTPGetReq(NetCommInterface* theNetCommInterfaceP) :
	mNetCommInterfaceP(theNetCommInterfaceP),
	mDestPath(),
	mGetCallbackP(NULL),	
	mGetCallbackDataP(NULL),
	mHTTPRequestResultP(NULL),		
	mHTTPRequestResultSize(0)
{}

HTTPGetReq::~HTTPGetReq()
{

}

bool
HTTPGetReq::HTTPGet(const char* theServer,							
					unsigned short thePort,							
					const char* theServerPath,						
					const char* theDestPath,						
					long theRequestTimeout,							
					NetCommInterface::NetCommCallback theNetCommCallbackP,				
					void* theCallbackDataP) 
{
	mDestPath = theDestPath;
	mGetCallbackP = theNetCommCallbackP;	
	mGetCallbackDataP = theCallbackDataP;

	string aRequest;
	if (!ConstructHTTPGetRequest(theServerPath, theServer, theDestPath, aRequest))
	{
		LOG("HTTPGetReq::HTTPGet Error:  Unable to construct request");
		return false;
	}
	// Request ad file name and image source
	if (!mNetCommInterfaceP->SendHTTPRequest(theServer,  thePort,  aRequest.c_str(), theRequestTimeout, &mHTTPRequestResultP, &mHTTPRequestResultSize, HTTPGetRequestCallback, this))
	{
		LOG("HTTPGet::SendAdRequest Error:  SendHTTPRequest Failed");
		return false;
	}
	return true;
}

void 
HTTPGetReq::HTTPRequestDone(bool theOperationResult)
{
	string aHTTPRequestResultBuffer;
	if (theOperationResult)
	{
		aHTTPRequestResultBuffer.assign((char*)mHTTPRequestResultP, mHTTPRequestResultSize);
	}
	else
	{
		LOG("HTTPGet::HTTPRequestDone Error:  Requested get operation failed");
		mHTTPRequestResultP = NULL;
		mHTTPRequestResultSize = 0;
		HTTPGetDone(false);
		return;
	}
	// Parse Response
	HTTPGetDone(ParseGetResponse(aHTTPRequestResultBuffer));
}

////////////////////////////////////////////////////////////////////
// Private methods

string 
HTTPGetReq::MakeInternetDateTime(time_t theTime)
{
	struct tm aTime = *localtime(&theTime);
	
	string result;

	switch (aTime.tm_wday)
	{
	case 0:
		result = "Sun";
		break;
	case 1:
		result = "Mon";
		break;
	case 2:
		result = "Tue";
		break;
	case 3:
		result = "Wed";
		break;
	case 4:
		result = "Thu";
		break;
	case 5:
		result = "Fri";
		break;
//	case 6:
	default:
		result = "Sat";
		break;
	};

	result += ", ";
	result += char((aTime.tm_mday / 10) % 10) + 48;
	result += char(aTime.tm_mday % 10) + 48;
	result += " ";
	result += monthTable[aTime.tm_mon % 12];
	result += " ";
	unsigned short year = 1900 + aTime.tm_year;
	result += char((year / 1000) % 10) + 48;
	result += char((year / 100) % 10) + 48;
	result += char((year / 10) % 10) + 48;
	result += char(year % 10) + 48;
	result += " ";
	result += char((aTime.tm_hour / 10) % 10) + 48;
	result += char(aTime.tm_hour % 10) + 48;
	result += ":";
	result += char((aTime.tm_min / 10) % 10) + 48;
	result += char(aTime.tm_min % 10) + 48;
	result += ":";
	result += char((aTime.tm_sec / 10) % 10) + 48;
	result += char(aTime.tm_sec % 10) + 48;
	result += " GMT";	// claims GMT for now

	return result;
}


// Potential internet date/time formats:
//
//      Sun, 06 Nov 1994 08:49:37 GMT    ; RFC 822, updated by RFC 1123
//      Sunday, 06-Nov-94 08:49:37 GMT   ; RFC 850, obsoleted by RFC 1036
//      Sun Nov  6 08:49:37 1994         ; ANSI C's asctime() format

// returns 0 on failure
// ignores time zone for now
time_t 
HTTPGetReq::ParseInternetDateTime(const char* str, bool parseTime)
{
	const char* tmpStr = str;

	// skip leading spaces
	for (;*tmpStr == ' '; tmpStr++)
		;

	// skip day of week
	for (;*tmpStr != ' ' ; tmpStr++)
	{
		if (!*tmpStr)
			return 0;
	}

	// skip spaces
	for (;*tmpStr == ' '; tmpStr++)
		;

	bool gotDay = false;
	int day = 0;
	int month;

	// if the next char is numeric, then we're recving a day, else we're recving a month
	if (*tmpStr >= '0' && *tmpStr <= '9')
	{
		// read day of month
		for (;*tmpStr != ' ' && *tmpStr != '-'; tmpStr++)
		{
			if (!*tmpStr)
				return 0;

			day *= 10;
			day += *tmpStr - 48;
		}

		// skip spaces or dashes
		for (;*tmpStr == ' ' || *tmpStr == '-'; tmpStr++)
			;
		gotDay = true;
	}

	// read month

	//  Cheap trick here.  Since the month must be 3 chars, and 3 bytes fits in a long, we can make a switch statement
	unsigned long dateValue  = *(tmpStr++) * 0x00010000;
	dateValue += *(tmpStr++) * 0x00000100;
	dateValue += *(tmpStr++);
	switch (dateValue)
	{
	case Jan_Value:
	case JAN_Value:
		month = 0;
		break;
	case Feb_Value:
	case FEB_Value:
		month = 1;
		break;
	case Mar_Value:
	case MAR_Value:
		month = 2;
		break;
	case Apr_Value:
	case APR_Value:
		month = 3;
		break;
	case May_Value:
	case MAY_Value:
		month = 4;
		break;
	case Jun_Value:
	case JUN_Value:
		month = 5;
		break;
	case Jul_Value:
	case JUL_Value:
		month = 6;
		break;
	case Aug_Value:
	case AUG_Value:
		month = 7;
		break;
	case Sep_Value:
	case SEP_Value:
		month = 8;
		break;
	case Oct_Value:
	case OCT_Value:
		month = 9;
		break;
	case NOV_Value:
	case Nov_Value:
		month = 10;
		break;
	case Dec_Value:
	case DEC_Value:
		month = 11;
		break;
	default:
		return 0;
	}

	// skip spaces
	for (;*tmpStr == ' ' || *tmpStr == '-'; tmpStr++)
		;

	if (!gotDay)
	{
		// read day of month

		for (;*tmpStr != ' ' && *tmpStr != '-'; tmpStr++)
		{
			if (!*tmpStr)
				return 0;

			day *= 10;
			day += *tmpStr - 48;
		}

		// skip spaces or dashes
		for (;*tmpStr == ' ' || *tmpStr == '-'; tmpStr++)
			;
	}

	int year = 0;

	bool gotYear = false;
	if (*(tmpStr+2) != ':')	// Check to see if year preceeds time
	{
		// read year

		for (;*tmpStr != ' '; tmpStr++)
		{
			if (!*tmpStr)
			{
				if (year != 0)	// year might still be good
					break;
				return 0;
			}

			year *= 10;
			year += *tmpStr - 48;
		}
		gotYear = true;
	}

	int hour = 0;
	int mins = 0;
	int sec = 0;

	// skip spaces
	for (;*tmpStr == ' '; tmpStr++)
		;

	// parse 2 digit hour
	
	if (*tmpStr < '0' || *tmpStr > '9')
		return 0;
	
	hour = (*tmpStr - 48) * 10;
	
	tmpStr++;

	if (*tmpStr < '0' || *tmpStr > '9')
		return 0;
	
	hour += *tmpStr - 48;

	// skip ':'
	tmpStr++;

	if (*tmpStr != ':')
		return 0;

	// parse 2 digit min
	tmpStr++;

	if (*tmpStr < '0' || *tmpStr > '9')
		return 0;

	mins = (*tmpStr - 48) * 10;

	tmpStr++;

	if (*tmpStr < '0' || *tmpStr > '9')
		return 0;

	mins += *tmpStr - 48;

	// skip ':'
	tmpStr++;

	if (*tmpStr != ':')
		return 0;

	// parse 2 digit seconds
	tmpStr++;

	if (*tmpStr < '0' || *tmpStr > '9')
		return 0;

	sec = (*tmpStr - 48) * 10;

	tmpStr++;

	if (*tmpStr < '0' || *tmpStr > '9')
		return 0;

	sec += *tmpStr - 48;

	tmpStr++;

	// skip spaces
	for (;*tmpStr == ' '; tmpStr++)
		;

	if (!gotYear)
	{
		// read year

		for (;*tmpStr != ' '; tmpStr++)
		{
			if (!*tmpStr)
			{
				if (year != 0)	// year might still be good
					break;
				return 0;
			}

			year *= 10;
			year += *tmpStr - 48;
		}
	}

	if (year < 100)
		year += 1900;

	struct tm timeStruct;

	timeStruct.tm_sec = sec;
	timeStruct.tm_min = mins;
	timeStruct.tm_hour = hour - 1;
	timeStruct.tm_mday = day;
	timeStruct.tm_mon = month;
	timeStruct.tm_year = year - 1900;

	// dunno
	timeStruct.tm_wday = 0;
	timeStruct.tm_yday = 0;
	timeStruct.tm_isdst = 0;

	return mktime(&timeStruct);
}

bool
HTTPGetReq::ConstructHTTPGetRequest(const string& theRemotePath, const string& theHost, const string& theLocalPath, string& theRequestResult)
{
	theRequestResult = "GET " + theRemotePath + " HTTP/1.0\r\n";
	theRequestResult += "Host: " + theHost + "\r\n";

	// Currently we don't support getting partial files, we always get the full file
	//if(theLocalPath.size())
	//{

	//   struct _stat fileInfo;
	//   if(_stat(theLocalPath.c_str(), &fileInfo)==0)
	//   {
	//		char aBuf[20];
	//		theRequestResult += "If-Range: " + MakeInternetDateTime(fileInfo.st_mtime) + "\r\n";
	//		theRequestResult += "Range: bytes=";
	//		theRequestResult += ltoa(fileInfo.st_size,aBuf,10);
	//		theRequestResult += "-\r\n";
	//   }
	//}

	theRequestResult += "Connection: close\r\n\r\n";

	return true;
}

// Parse Get Response
bool 
HTTPGetReq::ParseGetResponse(const string& theBuffer)
{
	// Locate the first linefeed; the status code comes before the first linefeed
	string::size_type aEndFirstLine = theBuffer.find('\n');
	if (aEndFirstLine == string::npos)
	{
		LOG("HTTPGetReq::ParseResponse - Error:  Unable to find end of first line");
		return false;
	}

	// Determine the response code
	char aOkStr[MAX_STRNUM];
	char aRedirectStr[MAX_STRNUM];

	::itoa(HTTP_STATUS_OK,			aOkStr, RADIX);			// 200
	::itoa(HTTP_STATUS_REDIRECT,	aRedirectStr, RADIX);	// 302

	bool aOK = theBuffer.rfind(aOkStr, aEndFirstLine) != string::npos;
	bool aRedirect = theBuffer.rfind(aRedirectStr, aEndFirstLine) != string::npos;

	// Bad response; log it
	if (!(aOK /*|| aRedirect*/))
	{
		// Make sure the error string is never too long in the unusual cases
		// where the response is total garbage
		string aError = theBuffer.substr(0, MAX(100, aEndFirstLine - 1));
		LOG("HTTPGetReq::ParseGetResponse  Error unexpected response type " << " Error: " << aError.c_str());
		return false;
	}

	// Don't support redirect at the moment
	// If redirection or moved, send request to new location
	//if (aRedirect)
	//{
	//	return (ParseRedirectionRequest());
	//}

	// Parse out the content type (content-type:)
	string::size_type aContentStart = FindNoCase(theBuffer, CONTENT_String.c_str());
	if (aContentStart == string::npos)
	{
		LOG("HTTPGetReq::ParseGetResponse - Error:  Unable to find content start");
		return false;
	}

	string::size_type aContentEnd = theBuffer.find('\n', aContentStart);
	if (aContentEnd == string::npos)
	{
		LOG("HTTPGetReq::ParseGetResponse - Error:  Unable to find content end");
		return false;
	}

	string aCurrContentType = theBuffer.substr(aContentStart + CONTENT_String.size(), aContentEnd - aContentStart - CONTENT_String.size() - 1);
	ToLower(aCurrContentType);

	if (aCurrContentType != APPLSTREAM_String.c_str())
	{
		LOG("HTTPGetReq::ParseGetResponse - Error:  Unknown content type " << aCurrContentType);
		return false;
	}

	// The content consists of everything from the dual linefeeds to the end of
	// the response.
	string::size_type aContentPos = GetContentStart(theBuffer);
	if (aContentPos == string::npos)
	{
		LOG("HTTPGetReq::ParseGetResponse - Error:  GetContent failed");
		return false;
	}
	string aContent = theBuffer.substr(aContentPos, string::npos);
	RawBuffer aContentBuffer((unsigned char*)aContent.c_str(), aContent.length());

	// We have valid content; write it to disk.
	if (!CreateFile(mDestPath, aContentBuffer))
	{
		LOG("HTTPGetReq::ParseGetResponse - Error:  Create file failed " << mDestPath);
		return false;
	}

	return true;
}

void
HTTPGetReq::HTTPGetDone(bool theGetResult)
{
	mGetCallbackP(theGetResult, mGetCallbackDataP);
}

bool 
HTTPGetReq::CreateFile(const string& theDestPath, const RawBuffer& theContentBuffer)
{
	if (theDestPath.length() == 0)
	{
		return false;
	}

	// Create the folder first
	if (!CreateFullPath(theDestPath))
	{
		LOG("HTTPGetReq::CreateFile Error:  Unable to create full path " << theDestPath.c_str());
		return false;
	}

	// Create the file
	HANDLE aFile = ::CreateFile(theDestPath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (aFile == INVALID_HANDLE_VALUE)
	{
		LOG("HTTPGetReq::CreateFile Error:  Failed to create file " << theDestPath.c_str() << " Err: "<< ::GetLastError());
		return false;
	}

	// Write the file
	DWORD aBytesWritten = 0;
	BOOL aSuccess = ::WriteFile(aFile, theContentBuffer.c_str(), theContentBuffer.length(), &aBytesWritten, NULL);
	::CloseHandle(aFile);

	if (!aSuccess || aBytesWritten != theContentBuffer.length())
	{
		LOG("HTTPGetReq::CreateFile Failed to write data to file " << theDestPath.c_str() << " Err: " << ::GetLastError());
		::DeleteFile(theDestPath.c_str());
		return false;
	}
	return true;	
}

// Callbacks

void
__stdcall WONAPI::HTTPGetRequestCallback(bool theOperationResult, void *theNetCommDataP)
{
	HTTPGetReq* This = (HTTPGetReq*)theNetCommDataP;
	This->HTTPRequestDone(theOperationResult);
}