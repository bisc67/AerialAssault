//----------------------------------------------------------------------------
// Surf.cpp
//
// This file was cribbed (and modified to avoid CStrings) from AdBanner.
//----------------------------------------------------------------------------
//#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "AdSurf.h"

using namespace WONAPI;


//----------------------------------------------------------------------------
// Start the user's browser and take them to the supplied URL.
//----------------------------------------------------------------------------
BOOL WONAPI::Browse(LPCSTR sURL, WORD wShow)
{
	char                sBrowser[MAX_PATH] = "";
	BOOL                bOK;
	STARTUPINFO         aStartupInfo;
	PROCESS_INFORMATION aProcessInfo;
	char                sCommandLine[4096];

	GetBrowserCommandLineFromRegistry(sBrowser, sizeof(sBrowser));
	if (sBrowser[0] == '\0')
		return FALSE;

	strcpy(sCommandLine, " ");
	strcat(sCommandLine, sURL);
	memset(&aStartupInfo, 0, sizeof(STARTUPINFO));
	aStartupInfo.cb = sizeof(STARTUPINFO);
	aStartupInfo.dwFlags = STARTF_USESHOWWINDOW;
	aStartupInfo.wShowWindow = wShow;

	bOK = CreateProcess(sBrowser, sCommandLine, NULL, NULL, FALSE, 0, NULL,
						NULL, &aStartupInfo, &aProcessInfo);

	if (bOK == TRUE)
	{
		CloseHandle(aProcessInfo.hThread);
		CloseHandle(aProcessInfo.hProcess);
	}

	return bOK;
}


//----------------------------------------------------------------------------
// Fetch the user's default browser.
//----------------------------------------------------------------------------
BOOL WONAPI::GetBrowserCommandLineFromRegistry(LPSTR sBrowser, int /*nBrowserSize*/)
{
	LONG  nLong;
	HKEY  hKey;
	char  sTemp[MAX_PATH];
	char  sValue[MAX_PATH];
	DWORD nValueLength = sizeof(sValue);
	char* pLocation;
	char  sShortPath[MAX_PATH];

	sBrowser[0] = '\0';

	// Find out what the preferred browser is by looking to see if .html is 
	// associated with NetscapeMarkup or htmlfile.
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, ".html", 0, KEY_QUERY_VALUE, &hKey)  != ERROR_SUCCESS)
		return FALSE;

	nLong = RegQueryValueEx(hKey, "", NULL, NULL, (unsigned char*) sValue, &nValueLength);
	RegCloseKey(hKey);
	if (nLong != ERROR_SUCCESS)
		return FALSE;

	strcpy(sTemp, sValue);
	strcat(sTemp, "\\shell\\open\\command");
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, sTemp, 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
		return FALSE;

	nValueLength = sizeof(sValue);
	nLong = RegQueryValueEx(hKey, "", NULL, NULL, (unsigned char*) sValue, &nValueLength);
	RegCloseKey(hKey);
	if (nLong != ERROR_SUCCESS)
		return FALSE;

	strcpy(sTemp, sValue);
	pLocation = strchr(sTemp, '\"');
	if (pLocation == sTemp)
	{
		pLocation = strchr(sTemp + 1, '\"');
		if (pLocation)
			pLocation[0] = '\0';

		DWORD nRetValue = GetShortPathName(sTemp + 1, sShortPath, MAX_PATH);
		if (nRetValue == 0 || nRetValue >= MAX_PATH)
			return FALSE;
		else
			strcpy(sBrowser, sShortPath);
	}
	else // Added to work for netscape
	{
		char* pEnd = strchr(sTemp, ' ');
		if (!pEnd)
		{
			pEnd = strchr(sTemp, '\"');
		}
		if (pEnd)
		{
			*pEnd = '\0';
			strcpy(sBrowser, sTemp);
		}
		else
		{
			return FALSE;
		}
	}
	/*if (pLocation)
	{
		strcpy(sBrowser, pLocation);
		int nLen = strlen(sBrowser);
		while (nLen > 0 && (sBrowser[nLen - 1] == ' ' || sBrowser[nLen - 1] == '\t' || 
			sBrowser[nLen - 1] == '\n'))
		{
			sBrowser[nLen - 1] = '\0';
			nLen = strlen(sBrowser);
		}
	}*/

	return strlen(sBrowser) > 0;
}
