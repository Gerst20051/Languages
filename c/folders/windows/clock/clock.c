////////////////////////////////////////////////////////////////////////////
// CLOCK:  Simple example demonstrating use of the Timer        by Athlor //
////////////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include "clock.h"

// Globals
static HANDLE ghInstance;

// Prototypes
static LRESULT CALLBACK MainDlgProc(HWND, UINT, WPARAM, LPARAM);

////////////////////////////////////////////////////////////////////////////
int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                                            LPSTR lpszCmdLine, int nCmdShow)
{
  WNDCLASSEX wcx;
  ghInstance = hInstance;
  wcx.cbSize = sizeof(wcx); // Get system dialog information
  if (!GetClassInfoEx(NULL, MAKEINTRESOURCE(32770), &wcx))  return 0;
  wcx.hInstance = hInstance;
  wcx.lpszClassName = _T("ClockClass");
  if (!RegisterClassEx(&wcx)) return 0;
  return DialogBox(hInstance, MAKEINTRESOURCE(DLG_MAIN), NULL,
          (DLGPROC)MainDlgProc); // The user interface is a modal dialog box
}

////////////////////////////////////////////////////////////////////////////
static LRESULT CALLBACK MainDlgProc(HWND hwndDlg, UINT uMsg,
                                               WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
    case WM_INITDIALOG:
      SetTimer(hwndDlg, 1, 1000, NULL); // Set 1 second intervals
      FORWARD_WM_TIMER(hwndDlg, 1, SendMessage); // Force the 1st update
      return TRUE;
    case WM_TIMER:
    {
      TCHAR Time[32]; // Temp string for time
      GetTimeFormat(LOCALE_USER_DEFAULT, 0, NULL, NULL, Time,
                                        (sizeof(Time) / sizeof((Time)[0])));
      SetDlgItemText(hwndDlg, txt_Time, Time); // Store time in textbox
      return TRUE;
    }
    case WM_CLOSE:
      KillTimer(hwndDlg, 1); // Turn timer off
      EndDialog(hwndDlg, 0); // Destroy Dialog
      return TRUE;
  }
  return FALSE;
}
