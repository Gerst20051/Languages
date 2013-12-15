#include <windows.h>
#include <Commctrl.h>
#include <tlhelp32.h>
#include <stdio.h>
#include "general.h"

HWND hSelectedWin;
HWND hMainDlg;	
int DevLogPixelsY;
CGeneralAppOptions GeneralAppOptions;
BOOL bAllowToolhelp, bAllowPSAPI;
BOOL bAppDisabled = FALSE;
HINSTANCE hCurrInstance;

//Toolhelp functions (Windows 95/98/ME)
FuncType_CreateToolhelp32Snapshot *FuncCreateToolhelp32Snapshot;
FuncType_Module32First *FuncModule32First, *FuncModule32Next;
FuncType_Process32First *FuncProcess32First, *FuncProcess32Next;

//PSAPI functions (Windows NT/2000/XP)
FuncType_GetModuleBaseName *FuncGetModuleBaseName, *FuncGetModuleFileNameEx;
FuncType_EnumProcessModules *FuncEnumProcessModules;

OSVERSIONINFO osVersionInfo;

BOOL IsAllowToolhelp()
{
	return bAllowToolhelp;
}

void LoadToolhelp()
{
	HMODULE hToolhelp = (HMODULE)LoadLibrary("kernel32.dll");
	bAllowToolhelp = FALSE;
	if (hToolhelp != NULL)
	{
		if ((FuncCreateToolhelp32Snapshot = (FuncType_CreateToolhelp32Snapshot *)GetProcAddress(hToolhelp, "CreateToolhelp32Snapshot")) != NULL)
			if ((FuncModule32First = (FuncType_Module32First *)GetProcAddress(hToolhelp, "Module32First")) != NULL)
				if ((FuncModule32Next = (FuncType_Module32First *)GetProcAddress(hToolhelp, "Module32Next")) != NULL)
					if ((FuncProcess32First = (FuncType_Process32First *)GetProcAddress(hToolhelp, "Process32First")) != NULL)
						if ((FuncProcess32Next = (FuncType_Process32First *)GetProcAddress(hToolhelp, "Process32Next")) != NULL)
							bAllowToolhelp = TRUE;
		
		if (!bAllowToolhelp) FreeLibrary(hToolhelp);
	}
}

void LoadPSAPI()
{
	HMODULE hPSAPI = (HMODULE)LoadLibrary("psapi.dll");
	bAllowPSAPI = FALSE;
	if (hPSAPI != NULL)
	{
		if ((FuncGetModuleBaseName = (FuncType_GetModuleBaseName *)GetProcAddress(hPSAPI, "GetModuleBaseNameA")) != NULL)
			if ((FuncEnumProcessModules = (FuncType_EnumProcessModules *)GetProcAddress(hPSAPI, "EnumProcessModules")) != NULL)
				if ((FuncGetModuleFileNameEx = (FuncType_GetModuleBaseName *)GetProcAddress(hPSAPI, "GetModuleFileNameExA")) != NULL)
					bAllowPSAPI = TRUE;

		if (!bAllowPSAPI) FreeLibrary(hPSAPI);
	}
}

void InitProcessFuncs()
{
	if (osVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
		LoadPSAPI();
	else
		LoadToolhelp();

}

void GeneralInit()
{
	HDC hScreenDC = GetDC(0);
	DevLogPixelsY = GetDeviceCaps(hScreenDC, LOGPIXELSY);
	ReleaseDC(0, hScreenDC);
	osVersionInfo.dwOSVersionInfoSize = sizeof(osVersionInfo);
	GetVersionEx(&osVersionInfo);
	
	InitProcessFuncs();
	//LoadToolhelp();
	
}



BOOL FindModuleToolhelp(DWORD dwProcessID, HMODULE hModule, LPSTR lpszFilename)
{
	if (IsAllowToolhelp())
	{
		HANDLE hSnapshot;
		BOOL bResult;

		if (hModule == NULL)
			hSnapshot = FuncCreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		else
			hSnapshot = FuncCreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessID);
		
		if ((LONG)hSnapshot != -1)
		{
			if (hModule == NULL)
			{
				PROCESSENTRY32 pe32;

				pe32.dwSize = sizeof(pe32);
				bResult = FuncProcess32First(hSnapshot, &pe32);
				while (bResult)
				{
					if (pe32.th32ProcessID == dwProcessID)
					{
						strcpy(lpszFilename, pe32.szExeFile);
						CloseHandle(hSnapshot);
						return TRUE;
					}
					bResult = FuncProcess32Next(hSnapshot, &pe32);
				}

			}
			else
			{
				MODULEENTRY32 tempme;
				tempme.dwSize = sizeof(tempme);
				bResult = FuncModule32First(hSnapshot, &tempme);
				while (bResult)
				{
					if (tempme.hModule == hModule)
					{
						strcpy(lpszFilename, tempme.szExePath);
						CloseHandle(hSnapshot);
						return TRUE;
					}
					bResult = FuncModule32Next(hSnapshot, &tempme);
				}
			}

			CloseHandle(hSnapshot);
		}
	}
	return FALSE;
}

BOOL GetProcessModuleName(DWORD dwProcessID, HMODULE hModule, LPSTR lpszModuleName)
{
	BOOL bReturn = FALSE;

	lpszModuleName[0] = '\0';
	if (osVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{

		if (bAllowPSAPI)
		{
			HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessID);
			
			if (hProcess != NULL)
			{
				if (hModule == NULL)
				{
					DWORD dwSize;
					HMODULE hModules[1024];

					if (FuncEnumProcessModules(hProcess, hModules, sizeof(hModules), &dwSize) != 0)
						if (FuncGetModuleFileNameEx(hProcess, hModules[0], lpszModuleName, MAX_PATH) != 0)
							bReturn = TRUE;				
				}
				else
					if (FuncGetModuleFileNameEx(hProcess, hModule, lpszModuleName, MAX_PATH) != 0)
						bReturn = TRUE;
				
				CloseHandle(hProcess);
			}
		}
	}
	else
	{
		if (FindModuleToolhelp(dwProcessID, hModule, lpszModuleName))
		{
			bReturn = TRUE;
		}

		
	}

	return bReturn;
}

void SetCheck(HWND hwndDlg, int nIDDlgItem, BOOL bCheck)
{
	SendMessage(GetDlgItem(hwndDlg, nIDDlgItem), BM_SETCHECK, bCheck, 0);
}

BOOL GetCheck(HWND hwndDlg, int nIDDlgItem)
{
	return SendDlgItemMessage(hwndDlg, nIDDlgItem, BM_GETCHECK, 0, 0);
}


UINT GetRadioCheck(HWND hwndDlg, int nIDDlgItem1, int nIDDlgItem2)
{
	int nItem;

	for (nItem = nIDDlgItem1; nItem <= nIDDlgItem2; nItem++)
		if (GetCheck(hwndDlg, nItem)) return nItem - nIDDlgItem1 + 1;
	return 0;
}

HWND GetMainDlg()
{
	return hMainDlg;
}

void SetMainDlg(HWND hWnd)
{
	hMainDlg = hWnd;
}

int AppMessageBox(LPCTSTR lpText,LPCTSTR lpCaption, UINT uType)
{
	return MessageBox(GetMainDlg(), lpText, lpCaption, uType);
}

HICON GetWinIcon(HWND hWnd, WORD wIconType)
{
	HICON hIcon;

	SendMessageTimeout(hWnd, WM_GETICON, wIconType, 0, SMTO_ABORTIFHUNG, 300, (DWORD *)&hIcon);
	//hIcon = (HICON)SendMessage(hWnd, WM_GETICON, wIconType, 0);

	if (hIcon == NULL) 
		if (wIconType == ICON_SMALL)
			hIcon = (HICON)GetClassLong(hWnd, GCL_HICONSM);
		else
			hIcon = (HICON)GetClassLong(hWnd, GCL_HICON);

	return hIcon;

}

HWND GetSelectedWindow()
{
	return hSelectedWin;
}

void SetSelectedWindow(HWND hSelected)
{
	hSelectedWin = hSelected; 
}

void RefreshWindowsTree(HWND hNewWnd)
{
	SendMessage(GetMainDlg(), WM_REFRESH_WIN_TREE, 0, (LPARAM)hNewWnd);	
}

void RefreshCurrentTreeItem()
{
	SendMessage(GetMainDlg(), WM_REFRESH_TREE_ITEM, 0, 0);
}

CGeneralAppOptions *GetAppOptions()
{
	return &GeneralAppOptions;
}



DWORD RegQueryValueDWord(HKEY hKey, LPTSTR lpValueName, DWORD dwDefault = 0)
{
	DWORD dwType, dwValue, dwSize;

	dwSize = 4;
	if (RegQueryValueEx(hKey, lpValueName, NULL, &dwType, (LPBYTE)&dwValue, &dwSize) == ERROR_SUCCESS)
		if (dwType == REG_DWORD) return dwValue;

	return dwDefault;

}

DWORD RegSetValueDWord(HKEY hKey, LPCTSTR lpValueName, DWORD dwValue)
{
	
	return RegSetValueEx(hKey, lpValueName, 0, REG_DWORD, (BYTE *)&dwValue, 4);
}



UINT TranslateHotKey(UINT uKey)
{
	UINT uHotKeyMod = 0;

	if ((uKey & HOTKEYF_ALT) != 0) uHotKeyMod |= MOD_ALT;	
	if ((uKey & HOTKEYF_CONTROL) != 0) uHotKeyMod |= MOD_CONTROL;	
	if ((uKey & HOTKEYF_SHIFT) != 0) uHotKeyMod |= MOD_SHIFT;	
	return uHotKeyMod;
}

BOOL RegisterCatchKey()
{
	DWORD dwHotKey = GetAppOptions()->m_dwCatchWinByMousePosKey;
	return RegisterHotKey(hMainDlg, CATCH_HOTKEY_ID, TranslateHotKey(HIBYTE(dwHotKey)), LOBYTE(dwHotKey));
}

BOOL UnregisterCatchKey()
{
	return UnregisterHotKey(hMainDlg, CATCH_HOTKEY_ID);
}

BOOL HandleCatchKey()
{
	if (GetAppOptions()->m_bCatchWinByMousePos)
	{
		UnregisterCatchKey();
		return RegisterCatchKey();
	}
	else
		return UnregisterCatchKey();
}



void GetStartText(LPSTR lpszStartText)
{
	strcpy(lpszStartText, START_TEXT);
}




void FillFontStruct(LOGFONT *lpLogFont, LPSTR lpszName, int iSize, BOOL bBold, BOOL bUnderline)
{
	lpLogFont->lfHeight = iSize;
	lpLogFont->lfWidth = 0;
	lpLogFont->lfEscapement = 0;
	lpLogFont->lfOrientation = 0;
	lpLogFont->lfWeight = 400 + 300 * (bBold == TRUE);
	lpLogFont->lfItalic = FALSE;
	lpLogFont->lfUnderline = bUnderline;
	lpLogFont->lfStrikeOut = FALSE;
	lpLogFont->lfCharSet = DEFAULT_CHARSET;
	lpLogFont->lfOutPrecision = OUT_DEFAULT_PRECIS;
	lpLogFont->lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lpLogFont->lfQuality = DEFAULT_QUALITY;
	lpLogFont->lfPitchAndFamily = DEFAULT_PITCH;
	strcpy(lpLogFont->lfFaceName, lpszName);
}


void SetCurrInstance(HINSTANCE hInst)
{
	hCurrInstance = hInst;
}

HINSTANCE GetCurrInstance()
{
	return hCurrInstance;
}



///////////////////////////////////////////////////////////////////////////
//CGeneralAppOptions Class
//
//This class is responsible for saving and loading the application options
//to the Registry.
///////////////////////////////////////////////////////////////////////////

BOOL CGeneralAppOptions::OpenKey()
{
	DWORD dwDisposition;
	return (RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\NirSoft\\WinExplorer", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL ,&hKey, &dwDisposition) == ERROR_SUCCESS);
}

BOOL CGeneralAppOptions::CloseKey()
{
	return (RegCloseKey(hKey) == ERROR_SUCCESS);
}

void CGeneralAppOptions::SaveToReg()
{
	if (OpenKey())
	{
		RegSetValueDWord(hKey, "HideTopHiddenWindows", m_bHideTopHiddenWindows);
		RegSetValueDWord(hKey, "FullTree", m_bFullTree);
		RegSetValueDWord(hKey, "CatchWinByMousePos", m_bCatchWinByMousePos);
		RegSetValueDWord(hKey, "CatchWinByMousePosKey", m_dwCatchWinByMousePosKey);
		RegSetValueDWord(hKey, "UseAppIcons", m_bUseAppIcons);
		RegSetValueDWord(hKey, "AutoRefresh", m_bAutoRefresh);
		RegSetValueDWord(hKey, "RefreshSeconds", m_dwRefreshSeconds);
		CloseKey();
	}
}

void CGeneralAppOptions::LoadFromReg()
{
	if (OpenKey())
	{
		m_bHideTopHiddenWindows = RegQueryValueDWord(hKey, "HideTopHiddenWindows", TRUE);
		m_bFullTree = RegQueryValueDWord(hKey, "FullTree", TRUE);
		m_bCatchWinByMousePos = RegQueryValueDWord(hKey, "CatchWinByMousePos", FALSE);
		m_dwCatchWinByMousePosKey = RegQueryValueDWord(hKey, "CatchWinByMousePosKey", MAKEWORD(65, HOTKEYF_ALT | HOTKEYF_CONTROL));
		m_bUseAppIcons = RegQueryValueDWord(hKey, "UseAppIcons", TRUE);
		m_bAutoRefresh = RegQueryValueDWord(hKey, "AutoRefresh", FALSE);
		m_dwRefreshSeconds = RegQueryValueDWord(hKey, "RefreshSeconds", 10);
		CloseKey();
	}
}

CGeneralAppOptions::CGeneralAppOptions()
{
	LoadFromReg();
}

CGeneralAppOptions::~CGeneralAppOptions()
{


}

//////////////////////////////////////////////////////////////////////////
