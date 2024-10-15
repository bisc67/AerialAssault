
#include "AdUtils.h"
#include "HTTPUtil.h"

using namespace std;
using namespace WONAPI;

// HTTP constants
const string WONAPI::HTTP_String			= "http://";
const string WONAPI::URLCLICKTHRU_String	= "href=\"";
const string WONAPI::URLCLICKTHRU2_String	= "href=";
const string WONAPI::ADFILE_String			= "<img src=\"";
const string WONAPI::TOOLTIP1_String		= "alt=\"";
const string WONAPI::TOOLTIP2_String		= "alt='";
const string WONAPI::SETCOOKIE_String		= "set-cookie: ";
const string WONAPI::COOKIEDOMAIN_String	= "domain=";
const string WONAPI::COOKIEPATH_String		= "path=";
const string WONAPI::CONTENT_String			= "content-type: ";
const string WONAPI::LOCATION_String		= "location: ";
const string WONAPI::TEXTHTML_String		= "text/html";
const string WONAPI::TEMP_PREFIX_String		= "WON";
const string WONAPI::APPLSTREAM_String		= "application/octet-stream";

// Return the position where the HTTP content starts (immediately after the dual
// CRLF).  Returns string::npos if no dual CRLF detected.
string::size_type 
WONAPI::GetContentStart(const string& theBuffer) 
{
	const char* sCRLF[] =
	{
		"\r\n\r\n",			// from most to least likely
		"\n\n",
		"\n\r\n\r",
		"\r\r",
		NULL
	};
	
	for (int i = 0; sCRLF[i] != NULL; ++i)
	{
		string::size_type nContent = theBuffer.find(sCRLF[i]);
		if (nContent != string::npos)
			return (nContent + ::lstrlen(sCRLF[i]));
	}

	return (string::npos);
}

// Chop the given URL into its two components, the server and file.
bool 
WONAPI::SplitURL(const string& theURLtoSplit, string& theServer, string& theFile, int& thePort) 
{
	theServer.erase();
	theFile.erase();

	// For ease in parsing, we convert slashes to a common format
	string aURLStr = theURLtoSplit;
	SlashesToForwardslashes(aURLStr);

	// A URL beginning with a slash does not have a server
	if (aURLStr[0] == '/' || aURLStr[0] == '\\')
	{
		theFile = theURLtoSplit;
		return true;
	}

	// We assume the following format: "http://www.server.com/path/name"
	// The third slash indicates the parse point.  Everything before the slash
	// is the server and everything after is the file.
	// If the URL doesn't match the format, we assert and fail.

	string::size_type aHTTP = FindNoCase(aURLStr, HTTP_String.c_str());
	if (aHTTP == string::npos)
	{
		LOG("AdForceProtocol::SplitURL - Error:  Unable to find HTTP string");
		return false;
	}

	// find 1st
	string::size_type aSlash = aURLStr.find('/');

	// find 2nd
	aSlash = aURLStr.find('/', aSlash+1);

	// find 3rd
	aSlash = aURLStr.find('/', aSlash+1);

	// validate
	if (aSlash == string::npos)
	{
		LOG("AdForceProtocol::SplitURL - Error:  Unable to find 3rd slash");
		return false;
	}

	string::size_type aServerStart = ::lstrlen(HTTP_String.c_str());
	string::size_type aServerLen = aSlash - aServerStart;
	theServer = theURLtoSplit.substr(aServerStart, aServerLen);

	// Parse off the port number from the server (if any)
	string::size_type aColon = theServer.rfind(':');
	if (aColon == string::npos)
	{
		thePort = INTERNET_DEFAULT_HTTP_PORT;
	}
	else
	{
		string aPortStr = theServer.substr(aColon+1);
		thePort = ::atoi(aPortStr.c_str());
		theServer.resize(aColon);
	}

	theFile = theURLtoSplit.substr(aSlash, string::npos);
	return true;
}

