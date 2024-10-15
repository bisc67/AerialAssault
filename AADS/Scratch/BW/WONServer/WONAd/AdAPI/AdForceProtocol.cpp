////////////////////////////////////////////////////////////////////////////////
// Ad force ad delivery protocol
// 
// This protocol is used when getting ads from Adforce
//
// Serving process:
//
//		Request an ad file (img src) and URL target (a href) from server
//		Parse response
//		Get content-type
//		If content-type is text/html
//			If img src is a file with a valid extension
//				If ad is not in the cache
//					Dowload new ad
//					Start timer for next ad download
//				Else if ad in cache
//					Display it
//					Start timer for next ad download
//				Else
//					Request img src and continue with parse response above
//		else if content-type is allowable file type
//			Copy request data to ad cache
//			Dowload new ad
//			Start timer for next ad download
//		else 
//			Request a new ad


#include "AdForceProtocol.h"
#include "NetCommInterface.h"
#include "HTTPUtil.h"

using namespace WONAPI;
using namespace std;

// Constants
const long Request_Timeout = 20000; // 20 sec

// Prototypes
void __stdcall HTTPRequestCallback(bool theOperationResult, void *theNetCommDataP);

AdForceProtocol::AdForceProtocol(NetCommInterface* theNetCommInterfaceP, const string& theCachePath, const string& theProductName, HANDLE theStopEvent) :
	AdProtocol(theNetCommInterfaceP, theCachePath, theProductName, theStopEvent),
	mHTTPRequestResultBuffer(),
	mHTTPRequestSuccess(false),
	mHTTPRequestDoneEvent(NULL)
{
	mHTTPRequestDoneEvent = CreateEvent(NULL, false, false, NULL);
}
	
AdForceProtocol::~AdForceProtocol()
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
AdForceProtocol::DownloadAd() 
{
	mAdReady = false;
	mCurrImageSource = "";
	mCookie = "";
	bool aSuccess = SendAdRequest(mAdTag, mAdServerName, mAdServerPort, mCookie, Request_Timeout);

	if (!aSuccess)
	{
		LOG("AdForceProtocol::DownloadAd Error:  SendAdRequest Error");
		return false;
	}

	return true;
}

// Public virtual methods
bool
AdForceProtocol::LaunchAdDestination(AdInfoConstPtr theAdInfoPtr, int theShowCmd)
{
	const AdForceAdInfo* aAdForceAdInfoP = (const AdForceAdInfo*)(theAdInfoPtr.get()); 
	if (theAdInfoPtr->mAdDest.length() != 0)
	{
		string aServerName;
		string aImageSource;
		int aPort;
		if (!SplitURL(aAdForceAdInfoP->mAdDest, aServerName, aImageSource, aPort))
		{
			LOG("AdForceProtocol::LaunchAdDestination - Error:  Split URL failed");
			LaunchBrowser(aAdForceAdInfoP->mAdDest, theShowCmd);
			return false;
		}
		string aRequest;
		if (!ConstructRequest(aImageSource, aServerName, aAdForceAdInfoP->mCookie, aRequest))
		{
			LOG("AdForceProtocol::LaunchAdDestination Error:  Unable to construct request");
			LaunchBrowser(aAdForceAdInfoP->mAdDest, theShowCmd);
			return false;
		}
		mHTTPRequestSuccess = false;

		// Create a reference counted object that holds data for the callback
		CallbackInfo* aCallbackInfoP = new CallbackInfo(HTTPRequestCallback, this, &mHTTPRequestResultBuffer);
		// Request ad file name and image source
		if (!mNetCommInterfaceP->SendHTTPRequest(aServerName.c_str(),  aPort,  aRequest.c_str(), Request_Timeout, &(aCallbackInfoP->mRequestResultP), &(aCallbackInfoP->mRequestResultSize), CallbackInfoCallback, aCallbackInfoP))
		{
			LOG("AdForceProtocol::LaunchAdDestination Error:  SendHTTPRequest Failed");
			LaunchBrowser(aAdForceAdInfoP->mAdDest, theShowCmd);
			return false;
		}
			
		HANDLE aWaitArray[2] = {mStopEvent, mHTTPRequestDoneEvent};
		// Wait for an infinite amount of time for timeout to come back - Need to do this so that varable will be valid until callback is called.
		WaitForMultipleObjects(2, aWaitArray, false, INFINITE);

		// This means we will not get called back after this point
		aCallbackInfoP->InvalidateCallback();

		if (mHTTPRequestSuccess)
		{
			if (ParseLaunchDestResponse(theShowCmd))
			{
				return true;
			}
			else
			{
				LaunchBrowser(aAdForceAdInfoP->mAdDest, theShowCmd);
				return false;
			}
		}
		LOG("AdForceProtocol::LaunchAdDestination Error:  SendAdRequest Failed");
		LaunchBrowser(aAdForceAdInfoP->mAdDest, theShowCmd);
		return false;
	}
	else
	{
		LOG("AdProtocol::LaunchAdDestination Error: Launch of ad destination attemped on empty destination");
		return false;
	}
}

bool
AdForceProtocol::SendAdRequest(const string& theImageSource, const string& theHost, unsigned long thePort, const string& theCookie, long theTimeout)
{
	string aRequest;
	if (!ConstructRequest(theImageSource, theHost, theCookie, aRequest))
	{
		LOG("AdForceProtocol::SendAdRequest Error:  Unable to construct request");
		return false;
	}
	mHTTPRequestSuccess = false;

	// Create a reference counted object that holds data for the callback
	CallbackInfo* aCallbackInfoP = new CallbackInfo(HTTPRequestCallback, this, &mHTTPRequestResultBuffer);
	// Request ad file name and image source
	if (!mNetCommInterfaceP->SendHTTPRequest(theHost.c_str(),  thePort,  aRequest.c_str(), theTimeout, &(aCallbackInfoP->mRequestResultP), &(aCallbackInfoP->mRequestResultSize), CallbackInfoCallback, aCallbackInfoP))
	{
		LOG("AdForceProtocol::SendAdRequest Error:  SendHTTPRequest Failed");
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
	LOG("AdForceProtocol::SendAdRequest Error:  SendAdRequest Failed");
	return false;
}


void 
AdForceProtocol::HTTPRequestDone(bool theOperationResult)
{
	mHTTPRequestSuccess = theOperationResult;
	SetEvent(mHTTPRequestDoneEvent);
}

// Construct an HTTP request
bool 
AdForceProtocol::ConstructRequest(const string& theImageSource, const string& theHost, const string& theCookie, string& theRequestResult)
{
	// Construct the request
	theRequestResult = "GET ";
	theRequestResult += theImageSource;
	theRequestResult += " HTTP/1.0\r\n";
	theRequestResult += "Host: ";
	theRequestResult += theHost;
	theRequestResult += "\r\n";
	theRequestResult += "From: webmaster@won.net\r\n";
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

	LOG("AdForceProtocol::ConstructRequest Request constructed: " << theRequestResult);

	return true;
}

// ParseResponse
//
// Returns false if response is not HTTP or does not contain the expected info.
// Returns true if correctly parsed and next request issued.
bool 
AdForceProtocol::ParseResponse()
{
	// Parse out the key information.  We're looking for:
	//
	//		1) "200"					valid HTTP response
	//		2) "content-type:"			the type of the response

	// Locate the first linefeed; the status code comes before the first linefeed
	string::size_type aEndFirstLine = mHTTPRequestResultBuffer.find('\n');
	if (aEndFirstLine == string::npos)
	{
		LOG("AdForceProtocol::ParseResponse - Error:  Unable to find end of first line");
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
		LOG("AdForceProtocol::ParseResponse  Error getting ad from " << mAdServerName << " Error: " << aError.c_str());
		return false;
	}

	// If redirection or moved, send request to new location
	if (aMoved || aRedirect)
	{
		return (ParseRedirectionRequest());
	}

	// Parse out the cookie (if any)
	ParseCookies(mHTTPRequestResultBuffer);

	// Parse out the content type (content-type:)
	string::size_type aContentStart = FindNoCase(mHTTPRequestResultBuffer, CONTENT_String.c_str());
	if (aContentStart == string::npos)
	{
		LOG("AdForceProtocol::ParseResponse - Error:  Unable to find content start");
		return false;
	}

	string::size_type aContentEnd = mHTTPRequestResultBuffer.find('\n', aContentStart);
	if (aContentEnd == string::npos)
	{
		LOG("AdForceProtocol::ParseResponse - Error:  Unable to find content end");
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
	
	// If we get to here this means that the ad was just sent to us after our initial request **

	// At this point, the content-type is most likely the ad we've been looking for
	// Should be .gif

	// If the content is not what we're looking for, we're done
	// ** Looking for file type... We assume it is .gif
	//FileTypeList::const_iterator i = m_FileType.find(aCurrContentType);
	//bool aMatch = (i != m_FileType.end());
	//if (!aMatch)
	//	return (false);

	// The content consists of everything from the dual linefeeds to the end of
	// the response.
	string::size_type aContentPos = GetContentStart(mHTTPRequestResultBuffer);
	if (aContentPos == string::npos)
	{
		LOG("AdForceProtocol::ParseResponse - Error:  GetContent failed");
		return false;
	}
	string aContent = mHTTPRequestResultBuffer.substr(aContentPos, string::npos);
	RawBuffer aContentBuffer((unsigned char*)aContent.c_str(), aContent.length());

	// We have valid content; write it to disk.
	MakeValidFileName(mCurrImageSource);
	if (!CreateAdFile(mCurrImageSource, aContentBuffer))
	{
		LOG("AdForceProtocol::ParseResponse - Error:  Create file failed " << mCurrImageSource);
		return false;
	}

	// Filter the ad if necessary *****
	
	return true;
}

// Parse out the redirection response and issue the redirection request
bool 
AdForceProtocol::ParseRedirectionRequest() 
{
	string::size_type aLocationStart = FindNoCase(mHTTPRequestResultBuffer, LOCATION_String.c_str());
	if (aLocationStart == string::npos)
	{
		LOG("AdForceProtocol::ParseRedirectionRequest - Error:  Unable to find Location start");
		return false;
	}

	string::size_type aLocationEnd = mHTTPRequestResultBuffer.find('\n', aLocationStart);
	if (aLocationEnd == string::npos)
	{
		LOG("AdForceProtocol::ParseRedirectionRequest - Error:  Unable to find Location end");
		return false;
	}

	string aLocationStr = mHTTPRequestResultBuffer.substr(aLocationStart + LOCATION_String.size(), aLocationEnd - aLocationStart - LOCATION_String.size() - 1);

	// Split the URL into its server and file components
	string aServerName;
	string aImageSource;
	int aPort;
	if (!SplitURL(aLocationStr, aServerName, aImageSource, aPort))
	{
		LOG("AdForceProtocol::ParseRedirectionRequest - Error:  Split URL failed");
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

	LOG("AdForceProtocol::ParseRedirectionRequest Redirecton followed " << mCurrImageSource);

	// Request the image source at the new location
	return (SendAdRequest(aImageSource, aServerName, aPort, mCookie, Request_Timeout));
}

// Parse out any cookies from the response
void 
AdForceProtocol::ParseCookies(const string& theResponse)
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
}


// Parse out a text/html response.
//
// The response has an <img src=%s> tag that indicates the next request we need
// to make.  We also pull out the clickthru tag <a href=%s> and tooltip tag
// (alt=%s).  Then we issue a new request.

bool 
AdForceProtocol::ParseTextHtml()
{
	// Parse out the key information.  We're looking for:
	//
	//		1) "<a href="URL target""				click-thru URL
	//		2) "<img src="Ad File Name""			next request
	//		3) "<img src=... alt="Tool Tip" ... >	tool tip

	// Parse out the click-thru URL (a href=)
	string::size_type aURLStart = FindNoCase(mHTTPRequestResultBuffer, URLCLICKTHRU_String.c_str());
	if (aURLStart == string::npos)
	{
		LOG("AdForceProtocol::ParseTextHtml - Error:  Unable to find Url Click through start");
		return false;
	}

	string::size_type aURLEnd = mHTTPRequestResultBuffer.find('\"', aURLStart+URLCLICKTHRU_String.size()+1);
	if (aURLEnd == string::npos)
	{
		LOG("AdForceProtocol::ParseTextHtml - Error:  Unable to find Url Click through end");
		return false;
	}

	string::size_type aAdFileStart;
	string::size_type aAdFileEnd;

	//SWF HACK
	/*
	string::size_type aSwfExtStart = mHTTPRequestResultBuffer.find(".swf");
	if (nSwfExtStart != string::npos)
	{
		nAdFileEnd = nSwfExtStart + 4;
		nAdFileStart = mHTTPRequestResultBuffer.rfind(HTTP_String.c_str(), nSwfExtStart) - ADFILE_String.size();
		if (nAdFileStart == string::npos)
			return (false);

		nURLStart = WONutil::FindNoCase(mHTTPRequestResultBuffer, URLCLICKTHRU_String.c_str()2);
		nURLEnd = mHTTPRequestResultBuffer.find('>', nURLStart+URLCLICKTHRU2_String.size()+1);
		
		nURLStart += URLCLICKTHRU2_String.size()-URLCLICKTHRU_String.size();
	}
	else
	{*/
		// Parse out the ad file or new request (img src=)
		aAdFileStart = FindNoCase(mHTTPRequestResultBuffer, ADFILE_String.c_str());
		if (aAdFileStart == string::npos)
		{
			LOG("AdForceProtocol::ParseTextHtml - Error:  Unable to find Ad file start");
			return false;
		}

		aAdFileEnd = mHTTPRequestResultBuffer.find('\"', aAdFileStart+ADFILE_String.size()+1);
		if (aAdFileEnd == string::npos)
		{
			LOG("AdForceProtocol::ParseTextHtml - Error:  Unable to find Ad file end");
			return false;
		}
	//}

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
		LOG("AdForceProtocol::ParseTextHtml - Error:  SplitUrl failed "<< mCurrImageSource);
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
	return (SendAdRequest(aImageSource, aServerName, aPort, mCookie, Request_Timeout));
}


// Parse out the redirection response and launch the browser
bool 
AdForceProtocol::ParseLaunchDestRedirectionRequest(int theShowCmd) 
{
	string::size_type aLocationStart = FindNoCase(mHTTPRequestResultBuffer, LOCATION_String.c_str());
	if (aLocationStart == string::npos)
	{
		LOG("AdForceProtocol::ParseLaunchDestRedirectionRequest Error unable to find location beginning");
		return false;
	}

	string::size_type aLocationEnd = mHTTPRequestResultBuffer.find('\n', aLocationStart);
	if (aLocationEnd == string::npos)
	{
		LOG("AdForceProtocol::ParseLaunchDestRedirectionRequest Error unable to find location end");
		return false;
	}

	string aLocationStr = mHTTPRequestResultBuffer.substr(aLocationStart + LOCATION_String.size(), aLocationEnd - aLocationStart - LOCATION_String.size() - 1);

	LaunchBrowser(aLocationStr, theShowCmd);
	return true;
}

// ParseResponse
//
// Returns false if response is not HTTP or does not contain the expected info.
// Returns true if correctly parsed and next request issued.
bool 
AdForceProtocol::ParseLaunchDestResponse(int theShowCmd)
{
	// Parse out the key information.  We're looking for:
	//
	//		1) "200"					valid HTTP response
	//		2) "content-type:"			the type of the response

	// Locate the first linefeed; the status code comes before the first linefeed
	string::size_type aEndFirstLine = mHTTPRequestResultBuffer.find('\n');
	if (aEndFirstLine == string::npos)
	{
		LOG("AdForceProtocol::ParseLaunchDestResponse - Error:  Unable to find end of first line");
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
		LOG("AdForceProtocol::ParseLaunchDestResponse  Error getting ad from " << mAdServerName << " Error: " << aError.c_str());
		return false;
	}

	// If redirection or moved, send request to new location
	if (aMoved || aRedirect)
	{
		return (ParseLaunchDestRedirectionRequest(theShowCmd));
	}

	// Parse out the content type (content-type:)
	string::size_type aContentStart = FindNoCase(mHTTPRequestResultBuffer, CONTENT_String.c_str());
	if (aContentStart == string::npos)
	{
		LOG("AdForceProtocol::ParseLaunchDestResponse - Error:  Unable to find content start");
		return false;
	}

	string::size_type aContentEnd = mHTTPRequestResultBuffer.find('\n', aContentStart);
	if (aContentEnd == string::npos)
	{
		LOG("AdForceProtocol::ParseLaunchDestResponse - Error:  Unable to find content end");
		return false;
	}

	string aCurrContentType = mHTTPRequestResultBuffer.substr(aContentStart + CONTENT_String.size(), aContentEnd - aContentStart - CONTENT_String.size() - 1);
	// Valid content types are stored in lower case
	ToLower(aCurrContentType);

	// We parse text/html responses, because we need to make another request
	if (aCurrContentType == TEXTHTML_String.c_str())
	{
		return ParseLaunchDestTextHtml(theShowCmd);
	}

	LOG("AdForceProtocol::ParseLaunchDestResponse - Error:  Invalid response contents");
	return false;
}

// Parse out a text/html response and launch the browser.
// The response has an <img src=%s> tag that indicates the browser URL
bool 
AdForceProtocol::ParseLaunchDestTextHtml(int theShowCmd)
{
	// Parse out the URL (img src=)
	string::size_type aAdFileStart = FindNoCase(mHTTPRequestResultBuffer, ADFILE_String.c_str());
	if (aAdFileStart == string::npos)
	{
		LOG("AdForceProtocol::ParseLaunchDestTextHtml Error Unable to find Adfile start");
		return false;
	}

	string::size_type aAdFileEnd = mHTTPRequestResultBuffer.find('\"', aAdFileStart+ADFILE_String.size()+1);
	if (aAdFileEnd == string::npos)
	{
		LOG("AdForceProtocol::ParseLaunchDestTextHtml Error Unable to find Adfile end");
		return false;
	}

	string aURLStr = mHTTPRequestResultBuffer.substr(aAdFileStart + ADFILE_String.size(), aAdFileEnd - aAdFileStart - ADFILE_String.size());
	LaunchBrowser(aURLStr, theShowCmd);
	return true;
}

// Callbacks

static void
__stdcall HTTPRequestCallback(bool theOperationResult, void *theNetCommDataP)
{
	AdForceProtocol* This = (AdForceProtocol*)theNetCommDataP;
	This->HTTPRequestDone(theOperationResult);
}



