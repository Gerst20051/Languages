HWND GetSelectedWindow();
void SetSelectedWindow(HWND hSelected);
HWND GetMainDlg();
HICON GetWinIcon(HWND hWnd, WORD wIconType);

void SetMainDlg(HWND hWnd);
BOOL IsAllowToolhelp();
//BOOL FindModule(DWORD dwProcessID, HMODULE hModule, LPMODULEENTRY32 me);
//BOOL GetProcessModuleName(DWORD dwProcessID, HMODULE hModule, LPSTR lpszProcessName);
BOOL GetProcessModuleName(DWORD dwProcessID, HMODULE hModule, LPSTR lpszModuleName);


typedef HANDLE WINAPI FuncType_CreateToolhelp32Snapshot(DWORD dwFlags, DWORD th32ProcessID);
typedef BOOL WINAPI FuncType_Module32First(HANDLE hSnapshot, LPMODULEENTRY32 lpme);
typedef BOOL WINAPI FuncType_Process32First(HANDLE hSnapshot, LPPROCESSENTRY32 lppe);

typedef BOOL WINAPI FuncType_Process32First(HANDLE hSnapshot, LPPROCESSENTRY32 lppe);  

 
typedef DWORD WINAPI FuncType_GetModuleBaseName(
  HANDLE hProcess,    // handle to the process
  HMODULE hModule,    // handle to the module
  LPTSTR lpBaseName,  // buffer that receives the base name
  DWORD nSize         // size of the buffer
);


typedef BOOL WINAPI FuncType_EnumProcessModules(
  HANDLE hProcess,      // handle to the process
  HMODULE * lphModule,  // array to receive the module handles
  DWORD cb,             // size of the array
  LPDWORD lpcbNeeded    // receives the number of bytes returned
);
 


#define MAX_TEXT 1024
#define WM_REFRESH_WIN_TREE WM_USER + 1
#define WM_REFRESH_TREE_ITEM WM_USER + 2
#define CATCH_HOTKEY_ID 1


class CGeneralAppOptions
{
private:
	HKEY hKey;
	BOOL OpenKey();
	BOOL CloseKey();

public:
	BOOL m_bHideTopHiddenWindows;
	BOOL m_bFullTree;
	BOOL m_bCatchWinByMousePos;
	BOOL m_bUseAppIcons;
	DWORD m_dwCatchWinByMousePosKey;
	BOOL m_bAutoRefresh;
	DWORD m_dwRefreshSeconds;
	
	void SaveToReg();
	void LoadFromReg();
	CGeneralAppOptions();
	~CGeneralAppOptions();
};

CGeneralAppOptions *GetAppOptions();
void RefreshWindowsTree(HWND hNewWnd = NULL);
void RefreshCurrentTreeItem();

void SetCheck(HWND hwndDlg, int nIDDlgItem, BOOL bCheck);
BOOL GetCheck(HWND hwndDlg, int nIDDlgItem);
UINT GetRadioCheck(HWND hwndDlg, int nIDDlgItem1, int nIDDlgItem2);

int AppMessageBox(LPCTSTR lpText,LPCTSTR lpCaption, UINT uType);
void GeneralInit();
BOOL HandleCatchKey();
BOOL UnregisterCatchKey();

#define START_TEXT	"WinExplorer v1.30\r\n"\
					"Copyright (c) 1999 - 2003 Nir Sofer\r\n"\
					"Web site: http://nirsoft.mirrorz.com"


void GetStartText(LPSTR lpszStartText);


void FillFontStruct(LOGFONT *lpLogFont, LPSTR lpszName, int iSize, BOOL bBold, BOOL bUnderline);
void SetCurrInstance(HINSTANCE hInst);
HINSTANCE GetCurrInstance();
