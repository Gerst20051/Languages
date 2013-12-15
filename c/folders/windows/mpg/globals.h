#ifndef __globals_h__
#define __globals_h__

// mpg.c
void FormLoad(void);
LRESULT CALLBACK MainWndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);
LRESULT CALLBACK ChildWndProc (HWND, UINT, WPARAM, LPARAM) ;
BOOL ValidMove(int PosClicked,int EmptyPos);
void DoSwitch(HWND hwndClicked,int WinwOld, int EmptyPos);
BOOL LevelDone(void);
void SetField(HWND hwnd,int w,int h);
void DestroyChilds(void);
HBITMAP LoadJPG( char* );
HBITMAP LoadGIF (char *);

// main.c
static BOOL InitApplication(void);
HWND   CreatempgWndClassWnd(void);
int    WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                                                  LPSTR lpCmdLine, INT nCmdShow);

// about.c
BOOL _stdcall AboutDlg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// tools.c
void    UpdateStatusBar(LPSTR lpszStatusString, WORD partNumber, WORD displayFlags);
void    InitializeStatusBar(HWND hwndParent,int nrOfParts);
BOOL    CreateSBar(HWND hwndParent,char *initialText,int nrOfParts);
LRESULT MsgMenuSelect(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam);

// globals
HINSTANCE hInst;		    // Instance handle
HWND      hwndMain;		    // Main window handle
HWND hWndStatusbar;


#endif
