/****************************************************************************
 *                                                                          *
 * File    : main.c                                                         *
 *                                                                          *
 * Purpose : GPS sample for Windows Mobile 5.                               *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

#define WINCE_DEFAULT_LIBS

#define UNICODE
#include <windows.h>
#include <windowsx.h>
#include <aygshell.h>
#include <gpsapi.h>
#include <service.h>  /* SERVICE_STATE_* */
#include "main.h"

#define EVENT_NEWPOSITION  0
#define EVENT_NEWSTATE  1
#define NUM_EVENTS  2

#define NELEMS(a)  (sizeof(a) / sizeof((a)[0]))

/** Prototypes **************************************************************/

static LRESULT CALLBACK MainDlgProc(HWND, UINT, WPARAM, LPARAM);
static BOOL GPSInit(HANDLE *, HANDLE *, HANDLE *);
static void GPSDone(HANDLE, HANDLE, HANDLE);
static void GPSPosition(HANDLE, HWND);
static void GPSState(HANDLE, HWND);

/** Global variables ********************************************************/

static HANDLE ghInstance;

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

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpszCmdLine, int nCmdShow)
{
    WNDCLASS wc;
    HWND hwndDlg;
    HANDLE hDevice;
    HANDLE ahEvents[NUM_EVENTS];

    /* Always check first if program is already running */
    hwndDlg = FindWindow(L"GPSClass", NULL);
    if (hwndDlg)
    {
        /*
         * Set focus to the foremost child window. The "|0x01" is used to
         * bring any owned windows to the foreground and activate them.
         */
        SetForegroundWindow((HWND)((ULONG)hwndDlg|0x00000001));
        return 0;
    }

    ghInstance = hInstance;

    /* Get system dialog information */
    if (!GetClassInfo(NULL, L"Dialog", &wc))
        return 0;

    /* Register our own dialog */
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDR_ICO_MAIN));
    wc.lpszClassName = L"GPSClass";
    if (!RegisterClass(&wc))
        return 0;

    /* Open GPS handles */
    if (!GPSInit(&hDevice, &ahEvents[EVENT_NEWPOSITION], &ahEvents[EVENT_NEWSTATE]))
        return 0;

    // The user interface is a modeless dialog box.
    hwndDlg = CreateDialog(ghInstance, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)MainDlgProc);
    if (hwndDlg)
    {
        ShowWindow(hwndDlg, nCmdShow);
        UpdateWindow(hwndDlg);

        for (;;)
        {
            /* Wait for GPS or message queue events */
            switch (MsgWaitForMultipleObjects(NUM_EVENTS, ahEvents, FALSE, INFINITE, QS_ALLINPUT))
            {
                case WAIT_OBJECT_0 + EVENT_NEWPOSITION:
                {
                    /* Change in GPS position -- update dialog */
                    GPSPosition(hDevice, hwndDlg);
                    continue;
                }
                case WAIT_OBJECT_0 + EVENT_NEWSTATE:
                {
                    /* Change in GPS state -- update dialog */
                    GPSState(hDevice, hwndDlg);
                    continue;
                }
                case WAIT_OBJECT_0 + NUM_EVENTS:
                {
                    /* New message in queue */
                    MSG msg;
                    do
                    {
                        if (!GetMessage(&msg, NULL, 0, 0))
                            goto quit;
                        else if (!IsDialogMessage(hwndDlg, &msg))
                        {
                            TranslateMessage(&msg);
                            DispatchMessage(&msg);
                        }
                    } while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE));
                    continue;
                }
            }
        }
quit: ;
    }

    /* Close GPS handles (important!) */
    GPSDone(hDevice, ahEvents[EVENT_NEWPOSITION], ahEvents[EVENT_NEWSTATE]);
    return 0;
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
    switch (uMsg)
    {
        case WM_INITDIALOG:
        {
            SHINITDLGINFO shidi;

            shidi.dwMask = SHIDIM_FLAGS;
            shidi.dwFlags = SHIDIF_DONEBUTTON|SHIDIF_SIPDOWN|SHIDIF_SIZEDLGFULLSCREEN;
            shidi.hDlg = hwndDlg;
            SHInitDialog(&shidi);
            return TRUE;
        }

        case WM_COMMAND:
            switch (GET_WM_COMMAND_ID(wParam, lParam))
            {
                case IDOK:
                    PostMessage(hwndDlg, WM_CLOSE, 0, 0);
                    return TRUE;
            }
            break;

        case WM_CLOSE:
            DestroyWindow(hwndDlg);
            PostQuitMessage(0);
            return TRUE;
    }

    return FALSE;
}

/****************************************************************************
 *                                                                          *
 * Function: GPSInit                                                        *
 *                                                                          *
 * Purpose : Open GPS handles.                                              *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static BOOL GPSInit(HANDLE *phDevice, HANDLE *phEventNewPosition, HANDLE *phEventNewState)
{
    *phDevice = *phEventNewPosition = *phEventNewState = NULL;

    *phEventNewPosition = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (*phEventNewPosition)
    {
        *phEventNewState = CreateEvent(NULL, FALSE, FALSE, NULL);
        if (*phEventNewState)
        {
            *phDevice = GPSOpenDevice(*phEventNewPosition, *phEventNewState, NULL, 0);
        }
    }

    return *phDevice && *phEventNewPosition && *phEventNewState;
}

/****************************************************************************
 *                                                                          *
 * Function: GPSDone                                                        *
 *                                                                          *
 * Purpose : Close GPS handles.                                             *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void GPSDone(HANDLE hDevice, HANDLE hEventNewPosition, HANDLE hEventNewState)
{
    if (hDevice)
        (void)GPSCloseDevice(hDevice);

    if (hEventNewPosition)
        (void)CloseHandle(hEventNewPosition);

    if (hEventNewState)
        (void)CloseHandle(hEventNewState);
}

/****************************************************************************
 *                                                                          *
 * Function: GPSPosition                                                    *
 *                                                                          *
 * Purpose : Retrieve the current GPS position (if possible).               *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void GPSPosition(HANDLE hDevice, HWND hwndDlg)
{
    GPS_POSITION Position = { .dwVersion = GPS_VERSION_1, .dwSize = sizeof(Position), .dwValidFields = 0 };
    DWORD err;
    WCHAR wchBuf[80];

    /* Try to retrieve GPS information */
    err = GPSGetPosition(hDevice, &Position, 10000, 0);
    if (err == ERROR_SUCCESS)
    {
        /* Longitude position */
        if (Position.dwValidFields & GPS_VALID_LONGITUDE)
        {
            _snwprintf(wchBuf, NELEMS(wchBuf), L"%+f", Position.dblLongitude);
            SetDlgItemText(hwndDlg, DID_LONGITUDE, wchBuf);
        }
        else
        {
            SetDlgItemText(hwndDlg, DID_LONGITUDE, L"\?");
        }

        /* Latitude position */
        if (Position.dwValidFields & GPS_VALID_LATITUDE)
        {
            _snwprintf(wchBuf, NELEMS(wchBuf), L"%+f", Position.dblLatitude);
            SetDlgItemText(hwndDlg, DID_LATITUDE, wchBuf);
        }
        else
        {
            SetDlgItemText(hwndDlg, DID_LATITUDE, L"\?");
        }

        /* Altitude */
        if (Position.dwValidFields & GPS_VALID_ALTITUDE_WRT_SEA_LEVEL)
        {
            _snwprintf(wchBuf, NELEMS(wchBuf), L"%+f", Position.flAltitudeWRTSeaLevel);
            SetDlgItemText(hwndDlg, DID_ALTITUDE, wchBuf);
        }
        else
        {
            SetDlgItemText(hwndDlg, DID_ALTITUDE, L"\?");
        }
    }
    else
    {
        _snwprintf(wchBuf, NELEMS(wchBuf), L"Error 0x%X from GPSGetPosition", err);
        MessageBox(hwndDlg, wchBuf, L"GPS Position", MB_OK|MB_ICONSTOP|MB_SETFOREGROUND);
        PostMessage(hwndDlg, WM_CLOSE, 0, 0);
    }
}

/****************************************************************************
 *                                                                          *
 * Function: GPSState                                                       *
 *                                                                          *
 * Purpose : Retrieve the current GPS state (if possible).                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void GPSState(HANDLE hDevice, HWND hwndDlg)
{
    GPS_DEVICE State = { .dwVersion = GPS_VERSION_1, .dwSize = sizeof(State) };
    DWORD err;
    WCHAR wchBuf[80];

    /* Try to retrieve GPS state */
    err = GPSGetDeviceState(&State);
    if (err == ERROR_SUCCESS)
    {
        switch (State.dwServiceState)
        {
            case SERVICE_STATE_OFF:
                SetDlgItemText(hwndDlg, DID_STATE, L"GPS service is OFF");
                break;
            case SERVICE_STATE_ON:
                SetDlgItemText(hwndDlg, DID_STATE, L"GPS service is ON");
                break;
            case SERVICE_STATE_STARTING_UP:
                SetDlgItemText(hwndDlg, DID_STATE, L"GPS service is starting up");
                break;
            case SERVICE_STATE_SHUTTING_DOWN:
                SetDlgItemText(hwndDlg, DID_STATE, L"GPS service is shutting down");
                break;
            case SERVICE_STATE_UNLOADING:
                SetDlgItemText(hwndDlg, DID_STATE, L"GPS service is unloading");
                break;
            case SERVICE_STATE_UNINITIALIZED:
                SetDlgItemText(hwndDlg, DID_STATE, L"GPS service is uninitialized");
                break;
            default:
                SetDlgItemText(hwndDlg, DID_STATE, L"GPS service in UNKNOWN state");
                break;
        }
    }
    else
    {
        _snwprintf(wchBuf, NELEMS(wchBuf), L"Error 0x%X from GPSGetState", err);
        MessageBox(hwndDlg, wchBuf, L"GPS Position", MB_OK|MB_ICONSTOP|MB_SETFOREGROUND);
        PostMessage(hwndDlg, WM_CLOSE, 0, 0);
    }
}
