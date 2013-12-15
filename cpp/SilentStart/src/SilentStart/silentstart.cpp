// silentstart.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <windows.h>

int InformUser(LPCWSTR *s)
{
	HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if(INVALID_HANDLE_VALUE != outputHandle)
	{
		unsigned long cChars;
		int numToWrite = _mbstrlen(s);

		//I'm probably missing something here about the stdout handle, 
		//	as it doesn't find it for me, will correct when I figure it out.
		if(0 != WriteConsoleA(outputHandle, s, numToWrite, &cChars, NULL))
		{
			return 0;
		}
	}
	MessageBoxA(NULL, s, "SilentStart", MB_OK);
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	DWORD exitCode = 1;
	size_t cmdLength = _mbstrlen(lpCmdLine);
	char *outCommandLine;
	outCommandLine = malloc(cmdLength);
	strcpy(outCommandLine, lpCmdLine);

	if( outCommandLine != NULL ) //Could catch this fail-condition better.
	{
		STARTUPINFOA si;
		PROCESS_INFORMATION pi;

		ZeroMemory( &si, sizeof(si) );
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;
		si.cb = sizeof(si);
		ZeroMemory( &pi, sizeof(pi) );

		if(!CreateProcessA(NULL,
			outCommandLine,
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi )           // Pointer to PROCESS_INFORMATION structure
		)
		{
			InformUser("CreateProcess failed, app to start does not exist.");
			return 1;
		}
		WaitForSingleObject( pi.hProcess, INFINITE );
		GetExitCodeProcess(pi.hProcess, &exitCode);
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );
	}
	else
	{
		InformUser("No app to start, please provide the full executable path as first parameter.");
		return 1;
	}
	//InformUser("Finished.");
	return exitCode;
}

