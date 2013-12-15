#ifndef __RMTDEBUG_H__
#define __RMTDEBUG_H__

#include <windows.h>
#pragma comment(lib, "debuglib.lib")

// Functions required for debugging
VOID DbgPrint00(PWCHAR, ...);
BOOL StartDebugging (DWORD);
VOID StopDebugging ();


// WinCE undeclared kernel functions.
HANDLE GetCallerProcess (VOID);
LPVOID MapPtrToProcess (LPVOID, HANDLE);

#ifdef RMTDEBUG

#define DBG_MSG				DbgPrint00
#define START_DEBUG(_x)		StartDebugging(_x)
#define STOP_DEBUG()		StopDebugging()
#else //RMTDEBUG

#define DBG_MSG
#define START_DEBUG(_x)
#define STOP_DEBUG(_x)

#endif //RMTREBUG


#define DBG_SERVER	1
#define DBG_CLIENT	0

#endif //__RMTDEBUG_H__