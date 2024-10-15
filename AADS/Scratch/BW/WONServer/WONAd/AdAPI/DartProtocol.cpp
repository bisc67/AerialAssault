////////////////////////////////////////////////////////////////////////////////
// Dart ad delivery protocol
// 
// This protocol is used when getting ads from Doubleclick
//
// 


#include "DartProtocol.h"
#include "NetCommInterface.h"
#include "HTTPUtil.h"

using namespace WONAPI;
using namespace std;

// Constants
const long Request_Timeout = 20000; // 20 sec

// prototypes
void __stdcall HTTPRequestCallback(bool theOperationResult, void *theNetCommDataP);

DartProtocol::DartProtocol(NetCommInterface* theNetCommInterfaceP, const string& theCachePath, const string& theProductName, HANDLE theStopEvent) :
	AdProtocol(theNetCommInterfaceP, theCachePath, theProductName, theStopEvent),
	mHTTPRequestResultBuffer(),
	mHTTPRequestSuccess(false),
	mHTTPRequestDoneEvent(NULL)
{
	mHTTPRequestDoneEvent = CreateEvent(NULL, false, false, NULL);
}
	
DartProtocol::~DartProtocol()
{
	if (mHTTPRequestDoneEvent)
	{
		CloseHandle(mHTTPRequestDoneEvent);
		mHTTPRequestDoneEvent = NULL;
	}
}

// This method should be overriden for each protocol since each
// method of downloading will be different. 
// This function can block since ad download will not be done in the aplication's main thread
bool
DartProtocol::DownloadAd() 
{
	mAdReady = false;
	mCurrImageSource = "";
	mCookie = "";
	bool aSuccess = SendAdRequest(mAdTag, mAdServerName, mAdServerPort, Request_Timeout);

	if (!aSuccess)
	{
		LOG("DartProtocol::DownloadAd Error:  SendAdRequest Error");
		return false;
	}

	return true;
}

// Public virtual methods
bool
DartProtocol::LaunchAdDestination(AdInfoConstPtr theAdInfoPtr, int theShowCmd)
{
	const DartAdInfo* aDartAdInfoP = (const DartAdInfo*)(theAdInfoPtr.get()); 
	if (theAdInfoPtr->mAdDest.length() != 0)
	{
		LaunchBrowser(aDartAdInfoP->mAdDest, theShowCmd);
		return true;
	}
	else
	{
		LOG("AdProtocol::LaunchAdDestination Error: Launch of ad destination attemped on empty destination");
		return false;
	}
	LaunchBrowser(aDartAdInfoP->mAdDest, theShowCmd);
}

bool
DartProtocol::SendAdRequest(const string& theImageSource, const string& theHost, unsigned long thePort, long theTimeout)
{
	string aRequest;
	if (!ConstructRequest(theImageSource, theHost, "OPT_OUT", aRequest))
	{
		LOG("DartProtocol::SendAdRequest Error:  Unable to construct request");
		return false;
	}
	mHTTPRequestSuccess = false;

	// Create a reference counted object that holds data for the callback
	CallbackInfo* aCallbackInfoP = new CallbackInfo(HTTPRequestCallback, this, &mHTTPRequestResultBuffer);
	// Request ad file name and image source
	if (!mNetCommInterfaceP->SendHTTPRequest(theHost.c_str(),  thePort,  aRequest.c_str(), theTimeout, &(aCallbackInfoP->mRequestResultP), &(aCallbackInfoP->mRequestResultSize), CallbackInfoCallback, aCallbackInfoP))
	{
		LOG("DartProtocol::SendAdRequest Error:  SendHTTPRequest Failed");
		return false;
	}
		
	HANDLE aWaitArray[2] = {mStopEvent, mHTTPRequestDoneEvent};
	// Wait for an infinite amount of time for timeout to come back - Need to do this so that varable will be valid until callback is called.
	WaitForMultipleObjects(2, aWaitArray, false, INFINITE);

	// This means we will not get called back after this point
	aCallbackInfoP->InvalidateCallback();

	if (mHTTPRequestSuccess)
	{
		return ParseResponse();
	}
	LOG("DartProtocol::SendAdRequest Error:  SendAdRequest Failed");
	return false;
}


void 
DartProtocol::HTTPRequestDone(bool theOperationResult)
{
	mHTTPRequestSuccess = theOperationResult;
	SetEvent(mHTTPRequestDoneEvent);
}

// Construct an HTTP request
bool 
DartProtocol::ConstructRequest(const string& theImageSource, const string& theHost, const string& theCookie, string& theRequestResult)
{
	// Construct the request
	theRequestResult = "GET ";
	theRequestResult += theImageSource;
	theRequestResult += " HTTP/1.1\r\n";
	theRequestResult += "Host: ";
	theRequestResult += theHost;
	theRequestResult += "\r\n";
	theRequestResult += "Accept: */*\r\n";
	theRequestResult += "User-Agent: ";
	theRequestResult += "WONAdApi - ";	
	theRequestResult += mProductName;
	theRequestResult += "\r\n";

	if (theCookie.length())
	{
		theRequestResult += "Cookie: ";
		theRequestResult += theCookie;
		theRequestResult += "\r\n";
	}

	theRequestResult += "Connection: close\r\n\r\n";

	LOG("DartProtocol::ConstructRequest Request constructed: " << theRequestResult);

	return true;
}

// ParseResponse
//
// Returns false if response is not HTTP or does not contain the expected info.
// Returns true if correctly parsed and next request issued.
bool 
DartProtocol::ParseResponse()
{
	// Parse out the key information.  We're looking for:
	//
	//		1) "200"					valid HTTP response
	//		2) "content-type:"			the type of the response

	// Locate the first linefeed; the status code comes before the first linefeed
	string::size_type aEndFirstLine = mHTTPRequestResultBuffer.find('\n');
	if (aEndFirstLine == string::npos)
	{
		LOG("DartProtocol::ParseResponse - Error:  Unable to find end of first line");
		return false;
	}

	// Determine the response code
	char aOkStr[MAX_STRNUM];
	char aMovedStr[MAX_STRNUM];
	char aRedirectStr[MAX_STRNUM];

	::itoa(HTTP_STATUS_OK,			aOkStr, RADIX);		// 200
	::itoa(HTTP_STATUS_MOVED,		aMovedStr, RADIX);		// 301
	::itoa(HTTP_STATUS_REDIRECT,	aRedirectStr, RADIX);	// 302

	bool aOK = mHTTPRequestResultBuffer.rfind(aOkStr, aEndFirstLine) != string::npos;
	bool aMoved = mHTTPRequestResultBuffer.rfind(aMovedStr, aEndFirstLine) != string::npos;
	bool aRedirect = mHTTPRequestResultBuffer.rfind(aRedirectStr, aEndFirstLine) != string::npos;

	// Bad response; log it
	if (!(aOK || aMoved || aRedirect))
	{
		// Make sure the error string is never too long in the unusual cases
		// where the response is total garbage
		string aError = mHTTPRequestResultBuffer.substr(0, MAX(100, aEndFirstLine - 1));
		LOG("DartProtocol::ParseResponse  Error getting ad from " << mAdServerName << " Error: " << aError.c_str());
		return false;
	}

	// If redirection or moved, send request to new location
	if (aMoved || aRedirect)
	{
		return (ParseRedirectionRequest());
	}

	// Parse out the cookie (if any)
	//ParseCookies(mHTTPRequestResultBuffer);

	// Parse out the content type (content-type:)
	string::size_type aContentStart = FindNoCase(mHTTPRequestResultBuffer, CONTENT_String.c_str());
	if (aContentStart == string::npos)
	{
		LOG("DartProtocol::ParseResponse - Error:  Unable to find content start");
		return false;
	}

	string::size_type aContentEnd = mHTTPRequestResultBuffer.find('\n', aContentStart);
	if (aContentEnd == string::npos)
	{
		LOG("DartProtocol::ParseResponse - Error:  Unable to find content end");
		return false;
	}

	string aCurrContentType = mHTTPRequestResultBuffer.substr(aContentStart + CONTENT_String.size(), aContentEnd - aContentStart - CONTENT_String.size() - 1);
	// Valid content types are stored in lower case
	ToLower(aCurrContentType);

	// We parse text/html responses, because we need to make another request
	if (aCurrContentType == TEXTHTML_String.c_str())
	{
		return ParseTextHtml();
	}
	
	// The content consists of everything from the dual linefeeds to the end of
	// the response.
	string::size_type aContentPos = GetContentStart(mHTTPRequestResultBuffer);
	if (aContentPos == string::npos)
	{
		LOG("DartProtocol::ParseResponse - Error:  GetContent failed");
		return false;
	}
	string aContent = mHTTPRequestResultBuffer.substr(aContentPos, string::npos);
	RawBuffer aContentBuffer((unsigned char*)aContent.c_str(), aContent.length());

	// We have valid content; write it to disk.
	MakeValidFileName(mCurrImageSource);
	if (!CreateAdFile(mCurrImageSource, aContentBuffer))
	{
		LOG("DartProtocol::ParseResponse - Error:  Create file failed " << mCurrImageSource);
		return false;
	}
	
	return true;
}

// Parse out the redirection response and issue the redirection request
bool 
DartProtocol::ParseRedirectionRequest() 
{
	string::size_type aLocationStart = FindNoCase(mHTTPRequestResultBuffer, LOCATION_String.c_str());
	if (aLocationStart == string::npos)
	{
		LOG("DartProtocol::ParseRedirectionRequest - Error:  Unable to find Location start");
		return false;
	}

	string::size_type aLocationEnd = mHTTPRequestResultBuffer.find('\n', aLocationStart);
	if (aLocationEnd == string::npos)
	{
		LOG("DartProtocol::ParseRedirectionRequest - Error:  Unable to find Location end");
		return false;
	}

	string aLocationStr = mHTTPRequestResultBuffer.substr(aLocationStart + LOCATION_String.size(), aLocationEnd - aLocationStart - LOCATION_String.size() - 1);

	// Split the URL into its server and file components
	string aServerName;
	string aImageSource;
	int aPort;
	if (!SplitURL(aLocationStr, aServerName, aImageSource, aPort))
	{
		LOG("DartProtocol::ParseRedirectionRequest - Error:  Split URL failed");
		return false;
	}

	// Update the current image source
	mCurrImageSource = aImageSource;

	// Get file from Cache if it is there.
	string aFileName = aImageSource;
	MakeValidFileName(aFileName);
	if (IsValidFile(aFileName))
	{
		// Check to see if it is in cache
		string aAdPath;
		if (mAdCacheP->RequestFile(aFileName, aAdPath))
		{
			// It is in Cache
			SetCurrAdPath(aAdPath);
			return true;
		}
	}

	LOG("DartProtocol::ParseRedirectionRequest Redirecton followed " << mCurrImageSource);

	// Request the image source at the new location
	return (SendAdRequest(aImageSource, aServerName, aPort, Request_Timeout));
}

// Parse out any cookies from the response
/*
void 
DartProtocol::ParseCookies(const string& theResponse)
{
	string::size_type aCookieStart = FindNoCase(theResponse, SETCOOKIE_String.c_str());
	if (aCookieStart == string::npos)
	{
		return; // no cookie
	}

	string::size_type aCookieEnd = theResponse.find('\n', aCookieStart);
	string aCookieStr = theResponse.substr(aCookieStart + SETCOOKIE_String.size(), aCookieEnd - aCookieStart - SETCOOKIE_String.size() - 1);

	// Parse out the components of the cookie.  The only components we're interested
	// in are the name/value pair, the "Domain=", and the "Path=".  We ignore
	// everything else (e.g. Comment=, Expires=, Secure)

	string::size_type aCookieSize = aCookieStr.length() + 1;
	string::size_type aNameValueEnd = aCookieStr.find(';');
	aNameValueEnd = min(aNameValueEnd, aCookieSize);

	// Name/value pair
	string aNameValueStr = aCookieStr.substr(0, aNameValueEnd);

	// Domain (defaults to current host)
	string aDomainStr(mAdServerName); 
	string::size_type aDomainStart = FindNoCase(aCookieStr, COOKIEDOMAIN_String.c_str(), aNameValueEnd);
	if (aDomainStart != string::npos)
	{
		string::size_type aDomainEnd = aCookieStr.find(';', aDomainStart);
		aDomainStr = aCookieStr.substr(aDomainStart + COOKIEDOMAIN_String.size(), min(aDomainEnd, aCookieSize) - aDomainStart - COOKIEDOMAIN_String.size());
	}

	// Path (if specified)
	string aPathStr;
	string::size_type aPathStart = FindNoCase(aCookieStr, COOKIEPATH_String.c_str(), aNameValueEnd);
	if (aPathStart != string::npos)
	{
		string::size_type aPathEnd = aCookieStr.find(';', aPathStart);
		aPathStr = aCookieStr.substr(aPathStart + COOKIEPATH_String.size(), min(aPathEnd, aCookieSize) - aPathStart - COOKIEPATH_String.size());
	}

	// Append path to domain
	aDomainStr += aPathStr; // Domain is only for debug purposes at the moment.. 

	// Update cookie 
	mCookie = aNameValueStr;
}*/


// Parse out a text/html response.
//
// The response has an <img src=%s> tag that indicates the next request we need
// to make.  We also pull out the clickthru tag <a href=%s> and tooltip tag
// (alt=%s).  Then we issue a new request.

bool 
DartProtocol::ParseTextHtml()
{
	// Parse out the key information.  We're looking for:
	//
	//		1) href="URL target"			click-thru URL
	//		2) src="Ad File Name"			next request
	//		3) alt="Tool Tip"				tool tip

	// Parse out the click-thru URL (href=)
	string::size_type aURLStart = FindNoCase(mHTTPRequestResultBuffer, URLCLICKTHRU_String.c_str());
	if (aURLStart == string::npos)
	{
		LOG("DartProtocol::ParseTextHtml - Error:  Unable to find Url Click through start");
		return false;
	}

	string::size_type aURLEnd = mHTTPRequestResultBuffer.find('\"', aURLStart+URLCLICKTHRU_String.size()+1);
	if (aURLEnd == string::npos)
	{
		LOG("DartProtocol::ParseTextHtml - Error:  Unable to find Url Click through end");
		return false;
	}

	string::size_type aAdFileStart;
	string::size_type aAdFileEnd;

	
	// Parse out the ad file or new request (img src=)
	aAdFileStart = FindNoCase(mHTTPRequestResultBuffer, ADFILE_String.c_str());
	if (aAdFileStart == string::npos)
	{
		LOG("DartProtocol::ParseTextHtml - Error:  Unable to find Ad file start");
		return false;
	}

	aAdFileEnd = mHTTPRequestResultBuffer.find('\"', aAdFileStart+ADFILE_String.size()+1);
	if (aAdFileEnd == string::npos)
	{
		LOG("DartProtocol::ParseTextHtml - Error:  Unable to find Ad file end");
		return false;
	}

	// If we made it this far, we have all the information we need

	// The tool tip is not required (alt=)
	// Tool tip may be enclosed by single quotes or double quotes
	string::size_type aToolTipStart = FindNoCase(mHTTPRequestResultBuffer, TOOLTIP1_String.c_str(), aAdFileEnd+ADFILE_String.size()+1);
	string::size_type aToolTipEnd = string::npos;
	if (aToolTipStart != string::npos)
	{
		aToolTipEnd = mHTTPRequestResultBuffer.find('\"', aToolTipStart+TOOLTIP1_String.size()+1); 
	}
	else aToolTipStart = FindNoCase(mHTTPRequestResultBuffer, TOOLTIP2_String.c_str(), aAdFileEnd+ADFILE_String.size()+1);
	{
		if (aToolTipStart != string::npos)
		{
			aToolTipEnd = mHTTPRequestResultBuffer.find("'", aToolTipStart+TOOLTIP2_String.size()+1); 
		}
	}

	// Parse it out the elements
	string aCurrAdDest = mHTTPRequestResultBuffer.substr(aURLStart + URLCLICKTHRU_String.size(), aURLEnd - aURLStart - URLCLICKTHRU_String.size());
	mCurrImageSource = mHTTPRequestResultBuffer.substr(aAdFileStart + ADFILE_String.size(), aAdFileEnd - aAdFileStart - ADFILE_String.size());
	string aToolTipStr;
	if (aToolTipEnd != string::npos)
	{
		aToolTipStr = mHTTPRequestResultBuffer.substr(aToolTipStart + TOOLTIP1_String.size(), aToolTipEnd - aToolTipStart - TOOLTIP1_String.size());
		SetCurrToolTip(aToolTipStr);
	}

	// Prepend the ad server name to the front of the URL if we need to
	if (aCurrAdDest[0] == '\\' || aCurrAdDest[0] == '/')
	{
		aCurrAdDest.insert(0, mAdServerName);
		aCurrAdDest.insert(0, HTTP_String.c_str());
	}

	SetCurrAdDest(aCurrAdDest);

	// Split the source image URL into its server and file components
	string aServerName;
	string aImageSource;
	int aPort;
	if (!SplitURL(mCurrImageSource, aServerName, aImageSource, aPort))
	{
		LOG("DartProtocol::ParseTextHtml - Error:  SplitUrl failed "<< mCurrImageSource);
		return false;
	}

	// If the ad file is a file with a valid extension, do some cache checking
	// to avoid additional downloading
	string aFileName = aImageSource;
	MakeValidFileName(aFileName);
	if (IsValidFile(aFileName))
	{
		// Check to see if it is in cache
		string aAdPath;
		if (mAdCacheP->RequestFile(aFileName, aAdPath))
		{
			// It is in Cache
			SetCurrAdPath(aAdPath);
			return true;
		}
	}	

	// Update the current image source
	mCurrImageSource = aImageSource;

	// Request the image source at the new location
	return (SendAdRequest(aImageSource, aServerName, aPort, Request_Timeout));
}

// Callbacks

static void
__stdcall HTTPRequestCallback(bool theOperationResult, void *theNetCommDataP)
{
	DartProtocol* This = (DartProtocol*)theNetCommDataP;
	This->HTTPRequestDone(theOperationResult);
}



