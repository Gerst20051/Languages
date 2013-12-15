/****************************************************************************
 *                                                                          *
 * File    : main.c                                                         *
 *                                                                          *
 * Purpose : Dialog based Win32 application with tray icon support.         *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                               																						*
 * Shamelessly stolen from ICZELION'S ASSEMBLER TUTORIAL                    *
 * This example was converted to PellesC from lesson 24 off                 *
 * ICZELION'S ASSEMBLER TUTORIAL, definitively a must for anyone involved in*
 * Windows Programming                                                      *
 * I've lost the URL but a Web Search will do..                             *
 * I recommed reading lesson 24 to understand waths happening here, don't   *
 * worry if you don't handle assembler, is not necessary to grasp the       *
 * concepts...																															*
 * 																																					*
 * Thanks to...																															*
 *  Iczelion for his awesome tutorial ....																	*
 *  Pelles Orinius for Pelles C																							*
 ****************************************************************************/

/* 
 * Either define WIN32_LEAN_AND_MEAN, or one or more of NOCRYPT,
 * NOSERVICE, NOMCX and NOIME, to decrease compile time (if you
 * don't need these defines -- see windows.h).
 */

#define WIN32_LEAN_AND_MEAN
/* #define NOCRYPT */
/* #define NOSERVICE */
/* #define NOMCX */
/* #define NOIME */

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <tchar.h>
#include <shellapi.h>
#include "main.h"

// These defines are for the popup menu
#define IDM_RESTORE 1000
#define IDM_EXIT 1001
#define IDI_TRAY 1002
// To exchange message between tray and dialog
#define WM_SHELLNOTIFY  WM_USER+5 


/** Prototypes **************************************************************/

static LRESULT CALLBACK MainDlgProc(HWND, UINT, WPARAM, LPARAM);

/** Global variables ********************************************************/

//Handle to application
static HANDLE ghInstance;
//handle to Tray Icon PopUp menu;
static HANDLE hPopupMenu ;

/****************************************************************************
 *                                                                          *
 * Function: WinMain                                                        *
 *                                                                          *
 * Purpose : Initialize the application.  Register a window class,          *
 *           create and display the main window and enter the               *
 *           message loop.                                                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    WNDCLASSEX wcx;
    
    ghInstance = hInstance;

    // Initialize common controls. Also needed for MANIFEST's.
    InitCommonControls();

    // Get system dialog information.
    wcx.cbSize = sizeof(wcx);
    if (!GetClassInfoEx(NULL, MAKEINTRESOURCE(32770), &wcx))
        return 0;

    // Add our own stuff.
    wcx.hInstance = hInstance;
    wcx.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDR_ICO_MAIN));
    wcx.lpszClassName = _T("TrayIconClass");
    if (!RegisterClassEx(&wcx))
        return 0;

    // The user interface is a modal dialog box.
    return DialogBox(hInstance, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)MainDlgProc);
}

/****************************************************************************
 *                                                                          *
 * Function: MainDlgProc                                                    *
 *                                                                          *
 * Purpose : Process messages for the Main dialog.                          *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static LRESULT CALLBACK MainDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
		NOTIFYICONDATA nid;
 		POINT pt;

    switch (uMsg)
    {
       case WM_INITDIALOG:
				  //Create PopUp menu for tray icon
	 				hPopupMenu=CreatePopupMenu();
 	  	    AppendMenu(hPopupMenu,MF_STRING,IDM_RESTORE,"Restore");
  	      AppendMenu(hPopupMenu,MF_STRING,IDM_EXIT,"Exit");
   	 		  return TRUE;
				case WM_DESTROY:
					//Release PopUp menu before leaving
					DestroyMenu(hPopupMenu);
        	PostQuitMessage(0);
        case WM_SIZE:
					// If Minimized go to tray
          if (wParam == SIZE_MINIMIZED) {
					  //Fill a NOTIFYICONDATA structures
						nid.cbSize=sizeof(NOTIFYICONDATA); //Most API Structs require this
						nid.hWnd=hwndDlg; //Handle to App Window
						nid.uID=IDI_TRAY; //ID sent to WM_SHELLNOTIFY when the pointer enters Tray icon area
						nid.uFlags=NIF_ICON+NIF_MESSAGE+NIF_TIP; //Flags to set requires fields
						nid.uCallbackMessage=WM_SHELLNOTIFY; // Message ID sent when the pointer enters Tray icon area
						nid.hIcon=LoadIcon(ghInstance, MAKEINTRESOURCE(IDR_ICO_MAIN)); //Load Icon for tray
						lstrcpy(nid.szTip,"Pelles C Rocks..."); //Tray Icon Tool Tip
						ShowWindow(hwndDlg, SW_HIDE);//Hide window
						Shell_NotifyIcon(NIM_ADD,&nid);//Show the Icon
					}
          return TRUE;
        case WM_COMMAND:
						//Menu Options
						if (lParam==0 ){
            	Shell_NotifyIcon(NIM_DELETE,&nid);
            	
            	if (LOWORD(wParam)==IDM_RESTORE) {
							  //Restore Windows
             	  ShowWindow(hwndDlg,SW_RESTORE);
             	}
						 	else {
								//Exit App
        	      DestroyWindow(hwndDlg);
						 	}
        		}
						else {
            switch (GET_WM_COMMAND_ID(wParam, lParam))
            {
								//Button press 
                case IDOK:
                    EndDialog(hwndDlg, TRUE);
                    return TRUE;
            }
						}
            break;
				case WM_SHELLNOTIFY:
				//Called when pointer entry tray icon area
					if (wParam==IDI_TRAY ) {
						//Show PopUp menu if right button down
            if (lParam==WM_RBUTTONDOWN ) {
                GetCursorPos(&pt );
                SetForegroundWindow(hwndDlg);
                TrackPopupMenu(hPopupMenu,TPM_RIGHTALIGN,pt.x,pt.y,0,hwndDlg,0 );
                PostMessage(hwndDlg,WM_NULL,0,0 );
            } 
						else if (lParam==WM_LBUTTONDBLCLK ) {
						  //Restore Window
              SendMessage(hwndDlg,WM_COMMAND,IDM_RESTORE,0); 
            }
        	}
          break;


        case WM_CLOSE:
            EndDialog(hwndDlg, 0);
            return TRUE;

        /*
         * TODO: Add more messages, when needed.
         */
    }

    return FALSE;
}
