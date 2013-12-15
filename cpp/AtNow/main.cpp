#include <windows.h>
#include <Lmaccess.h> 
#include <lmremutl.h>
#include <lmat.h>
#include <stdio.h>
#include <lmerr.h>
#include "main.h"
#include "resource.h"
/*
AtNow v1.1
Copyright (c) 2002 - 2004 Nir Sofer  

Description
AtNow is a command-line utility that schedules programs and commands to run in the near future.
By default, the commands are executed within 70 seconds or less from the moment that you run the AtNow utility with the desired command. 

Syntax
AtNow [\\ComputerName] [Delay] [/interactive] "command" [Parameters] 



*/

static char szParams[MAX_PARAMS][MAX_PARAM_SIZE];
static int iParamLocation[MAX_PARAMS];
static char szError[ERROR_BUFFER_SIZE + 1];
static int iReturnValue = 0;

//Returns a pointer to the help string in IDR_HELP_TEXT resource
LPSTR GetHelpStr()
{
	HRSRC hRSrc = FindResource(NULL, MAKEINTRESOURCE(IDR_HELP_TEXT), "TEXT");
	if (hRSrc != NULL)
	{
		HGLOBAL hResource = LoadResource(NULL, hRSrc);
		if (hResource != NULL)
			return (LPSTR)LockResource(hResource);

	}

	return NULL;
}

//Safe strcpy - copy from lpszSource to lpszDest, up to iMax characters
char *strcpysf(LPSTR lpszDest, LPCSTR lpszSource, int iMax )
{
	int iLen = strlen(lpszSource);
	if (iLen >= iMax)
		iLen = iMax - 1;

	memcpy(lpszDest, lpszSource, iLen);
	lpszDest[iLen] = '\0';
	return lpszDest;
}


//Copy the error messenge represented by dwErr error code into szError.
void ErrorMsg(DWORD dwErr)
{
	HMODULE hModule = NULL;
	char *lpMessageBuffer; 
	DWORD dwResult;

	if(dwErr >= NERR_BASE && dwErr <= MAX_NERR) 
	{
		//Retrieve network releated message.
		hModule = LoadLibraryEx(TEXT("netmsg.dll"), NULL, LOAD_LIBRARY_AS_DATAFILE);
		dwResult = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE, hModule, dwErr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMessageBuffer, 0, NULL);
		
	}
	else
		dwResult = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwErr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMessageBuffer, 0, NULL);

	
	if (dwResult > 0)
	{
		strcpysf(szError, lpMessageBuffer, ERROR_BUFFER_SIZE);
		
		LocalFree(lpMessageBuffer); 					
	} else
	{
		strcpy(szError, "Unknown Error");
	}

	if (hModule != NULL) 
		FreeLibrary(hModule);

}

//Display the error messages represented by the the error code in dwError
void PrintLastError(DWORD dwError, LPCSTR lpszErrorInfo)
{
	ErrorMsg(dwError);
	printf("%s\r\n", lpszErrorInfo);
	printf("Error %d: %s ", dwError, szError);
	iReturnValue = 1;
}

//Load command-line parameters into array.
int LoadParams(LPCSTR lpCmdLine)
{
	int iParamIndex = 0;
	int iCharIndex = 0, iParamCharIndex = 0;
	BOOL bNewParam = TRUE;
	BOOL bQuotes = FALSE;
	int iNumOfParams;

	while (lpCmdLine[iCharIndex] != '\0')
	{
		if (lpCmdLine[iCharIndex] == '\x20' && bQuotes == FALSE)
		{
			if (!bNewParam)
			{
				szParams[iParamIndex][iParamCharIndex] = '\0';
				bNewParam = TRUE;
				if (iParamIndex < MAX_PARAMS) iParamIndex++;
				iParamCharIndex = 0;
			}
		} else
		{
			if (lpCmdLine[iCharIndex] == '\"')
				bQuotes ^= 1;
			if (iParamCharIndex != 0 || lpCmdLine[iCharIndex] != 32)
			{
				if (iParamCharIndex == 0) iParamLocation[iParamIndex] = iCharIndex;
				szParams[iParamIndex][iParamCharIndex] = lpCmdLine[iCharIndex];
				if (iParamCharIndex < MAX_PARAM_SIZE - 1) iParamCharIndex++;
			}
			bNewParam = FALSE;
		}
		iCharIndex++;
	}
	szParams[iParamIndex][iParamCharIndex] = '\0';

	if (iParamIndex == 0 && iParamCharIndex == 0)
		iNumOfParams = 0;
	else
		iNumOfParams = iParamIndex + 1;


	if (iNumOfParams > 0)
		if (szParams[iNumOfParams - 1][0] == '\0')
			iNumOfParams--;

	return iNumOfParams;
}

//Convert string from ascii to unicode.
void ToWideChar(LPCSTR str, LPWSTR wstr)
{
	if (MultiByteToWideChar(CP_ACP, 0, str, -1, wstr, WIDECHAR_BUFF_SIZE) == 0)
		wstr[0] = L'\0';
}

//Check if the lpszText is a valid number
BOOL IsNumeric(LPCSTR lpszText)
{
	int nIndex = 0;

	if (lpszText[nIndex] == '\0') return FALSE;

	while (lpszText[nIndex] != '\0')
	{
		if (lpszText[nIndex] > '9' || lpszText[nIndex] < '0')
			return FALSE;

		nIndex++;
	}

	return TRUE;
}

int main(int argc, char *argv[])
{

	BOOL bShowInfo = FALSE;
	DWORD dwJobID;
	SYSTEMTIME stLocal;
	AT_INFO atinfo;
	WCHAR ServerName[WIDECHAR_BUFF_SIZE], AtCommand[WIDECHAR_BUFF_SIZE];
	LPWSTR lpServerName = NULL;
	LPSTR lpszCommandLine = GetCommandLine();
	BOOL bInteractive = FALSE, bRemote = FALSE, bCommandFound = FALSE;
	BOOL bSyntaxError = FALSE;
	NET_API_STATUS  nStatus;
	int nDelay = 0;

	int nIndex;

	//Load command-line parameters into szParams array.
	int nParams = LoadParams(lpszCommandLine);
	
	if (strcmpi("/?", szParams[1]) == 0)
	{
		//Print extended help information from IDR_HELP_TEXT resource.
		LPSTR lpHelpText = GetHelpStr();

		if (lpHelpText != NULL)
			printf(lpHelpText);

		return 0;
	}

	if (nParams >= 1)
	{
		TIME_OF_DAY_INFO *todi;
		
		//Enumerate command-line parameters.
		for (nIndex = 1; nIndex < nParams; nIndex++)
		{
			if (strcmpi("/interactive", szParams[nIndex]) == 0)
				bInteractive = TRUE;
			else if (strncmp("/", szParams[nIndex], 1) == 0)
				bSyntaxError = TRUE;

			else if (strncmp("\\\\", szParams[nIndex], 2) == 0)
			{
				bRemote = TRUE;
				//Convert remote server name to unicode.
				ToWideChar(szParams[nIndex], ServerName);
				lpServerName = ServerName;
			} else if (IsNumeric(szParams[nIndex]))
				nDelay = atoi(szParams[nIndex]);
			else
			{
				if (szParams[nIndex][0] != '\0')
				{
					//Convert command string to unicode.
					bCommandFound = TRUE;
					ToWideChar(&lpszCommandLine[iParamLocation[nIndex]], AtCommand); 
					break;
				}
			}
		}
		
		if (!bCommandFound || bSyntaxError)
			bShowInfo = TRUE;
		else
		{

			if (bRemote) 
				//Retrieve "Time Of Day" information from the remote computer
				nStatus = NetRemoteTOD(lpServerName, (LPBYTE *)&todi); 
			else
				GetLocalTime(&stLocal);

			if (nStatus == 0 || bRemote == FALSE)
			{
				int nTime;
				
				//If the delay value is not specified, set it to 70 seconds.
				if (nDelay == 0) nDelay = 70;
				
				if (bRemote)
					//Create the time value according to the "Time Of Day" information in the remote computer.
					nTime = todi->tod_hours * 3600 + todi->tod_mins * 60 + todi->tod_secs - todi->tod_timezone * 60 + nDelay; 
				else
					//Create the time value according to the local time of the computer.
					nTime = stLocal.wHour * 3600L + stLocal.wMinute * 60 + stLocal.wSecond + nDelay;

				//Fix the time value if there is an overflow to the next or previous day.
				if (nTime < 0) 
					//Add one day
					nTime += 3600 * 24;
				else if (nTime >= 3600 * 24)
					//Subtract one day
					nTime -= 3600 * 24;

				//Fill the AT_INFO structure 
				atinfo.JobTime = (DWORD)nTime * 1000;
				atinfo.DaysOfMonth = NULL;
				atinfo.DaysOfWeek = NULL;
				if (!bInteractive) 
					atinfo.Flags = JOB_NONINTERACTIVE;
				else
					atinfo.Flags = 0;

				atinfo.Command = AtCommand;
				//Call NetScheduleJobAdd API to add a new schedule job.
				nStatus = NetScheduleJobAdd(lpServerName, (LPBYTE)&atinfo, &dwJobID);
				if (nStatus == 0)
					printf("Added a new job with job ID = %d", dwJobID);
				else
				{
					PrintLastError(nStatus, "Cannot add a new job in the target computer.");
				}
				
			} else
			{
				PrintLastError(nStatus, "Cannot retrieve the date/time information from the target computer.");
			}
		}
	} else
	{
		bShowInfo = TRUE;
	}

	if (bShowInfo)
	{
		printf(ATNOW_INFO);
	}

	return iReturnValue;

}
 