/****************************************************************************
 *                                                                          *
 * File    : winapp.c                                                       *
 *                                                                          *
 * Purpose : Windows GUI application wizard.                                *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <wizard.h>
#include "winapp.h"

#define NELEMS(a)  (sizeof(a) / sizeof(a[0]))

static TCHAR g_szProjname[9];
static TCHAR g_szPROJNAME[9];
static TCHAR g_szDispname[80];
static int cchProjname;
static int cchDispname;

static BOOL g_fSimple = FALSE;

// Static function prototypes.
static BOOL WINAPI OnStep1(HWND, enum WizAction);
static BOOL WINAPI OnStep2(HWND, enum WizAction);
static void WINAPI OnFileLine(PTSTR, int);

/****************************************************************************
 *                                                                          *
 * Function: WizMain                                                        *
 *                                                                          *
 * Purpose : Wizard entry point.                                            *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

BOOL WINAPI WizMain(void)
{
    TCHAR szBuffer[1024];
    TCHAR szTail[16];

    // Get the name of the current project.
    if (!WizGetProjectName(szBuffer, NELEMS(szBuffer)))
        return FALSE;

    // Build project names for the macro substitution.
    cchProjname = lstrlen(lstrcpyn(g_szProjname, szBuffer, NELEMS(g_szProjname)));
    CharUpper(lstrcpy(g_szPROJNAME, g_szProjname));

    // Build a default display name.
    LoadString(WizGetInstanceHandle(), IDS_APPLICATION, szTail, NELEMS(szTail));
    lstrcpyn(g_szDispname, szBuffer, NELEMS(g_szDispname) - lstrlen(szTail));
    lstrcat(g_szDispname, szTail);

    // Add the dialog pages for this wizard.
    if (!WizAddStep(MAKEINTRESOURCE(DLG_START), OnStep1) ||
        !WizAddStep(MAKEINTRESOURCE(DLG_FINISH), OnStep2))
        return FALSE;

    // Let the user interact with the wizard.
    if (!WizShowSteps())
        return FALSE;

    // Set the project type (GUI).
    if (!WizSetProjectType(Project_Win32_GUI))
        return FALSE;

    // Write the source file with macro substitution in OnFileLine().
    cchDispname = lstrlen(g_szDispname);
    if (!WizWriteTextFileFromResourceA("main.c", MAKEINTRESOURCE(IDR_MAIN_C), OnFileLine) ||
        !WizWriteTextFileFromResourceA("main.h", MAKEINTRESOURCE(IDR_MAIN_H), OnFileLine) ||
        !WizWriteTextFileFromResourceA("main.rc", MAKEINTRESOURCE(IDR_MAIN_RC), OnFileLine) ||
        !WizWriteFileFromResourceA("main.ico", MAKEINTRESOURCE(IDR_MAIN_ICO)))
        return FALSE;

    // Add the *source* files to the project. WizAddProjectFile() will only
    // accept .C and .RC files. Any files referenced from these files
    // (like .H, .ICO, ...) will be added automatically by the IDE).
    if (!WizAddProjectFile("main.c") ||
        !WizAddProjectFile("main.rc"))
        return FALSE;

    // Show who created this mess. (Not really needed.)
    if (!WizSetProjectSymbol("WizCreator", "Pelle Orinius"))
        return FALSE;

    return TRUE;
}

/****************************************************************************
 *                                                                          *
 * Function: OnStep1                                                        *
 *                                                                          *
 * Purpose : Handle events for step page #1.                                *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static BOOL WINAPI OnStep1(HWND hwnd, enum WizAction action)
{
    switch (action)
    {
        case Action_SetActive:
            // Set the display name.
            SetDlgItemText(hwnd, DID_DISPNAME, g_szDispname);
            // Set the program type.
            CheckRadioButton(hwnd, DID_SIMPLE, DID_HELLO, g_fSimple ? DID_SIMPLE : DID_HELLO);
            return TRUE;

        case Action_KillActive:
            // Remember the display name.
            GetDlgItemText(hwnd, DID_DISPNAME, g_szDispname, NELEMS(g_szDispname));
            // Remember the program type.
            g_fSimple = IsDlgButtonChecked(hwnd, DID_SIMPLE);
            return g_szDispname[0] != '\0';
    }

    return FALSE;
}

/****************************************************************************
 *                                                                          *
 * Function: OnStep2                                                        *
 *                                                                          *
 * Purpose : Handle events for step page #2.                                *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static BOOL WINAPI OnStep2(HWND hwnd, enum WizAction action)
{
    switch (action)
    {
        case Action_SetActive:
        {
            TCHAR szProjPath[260];

            // Fill the listbox with files we are about to create.
            SendDlgItemMessage(hwnd, DID_RESULTFILES, LB_RESETCONTENT, 0, 0);
            SendDlgItemMessage(hwnd, DID_RESULTFILES, LB_ADDSTRING, 0, (LPARAM)(LPSTR)"main.c");
            SendDlgItemMessage(hwnd, DID_RESULTFILES, LB_ADDSTRING, 0, (LPARAM)(LPSTR)"main.h");
            SendDlgItemMessage(hwnd, DID_RESULTFILES, LB_ADDSTRING, 0, (LPARAM)(LPSTR)"main.rc");
            SendDlgItemMessage(hwnd, DID_RESULTFILES, LB_ADDSTRING, 0, (LPARAM)(LPSTR)"main.ico");

            // File the editbox with the project path (=current directory).
            GetCurrentDirectory(NELEMS(szProjPath), szProjPath);
            SetDlgItemText(hwnd, DID_PROJFOLDER, szProjPath);
            return TRUE;
        }

        case Action_KillActive:
            return TRUE;
    }

    return FALSE;
}

/****************************************************************************
 *                                                                          *
 * Function: OnFileLine                                                     *
 *                                                                          *
 * Purpose : Perform simple macro substitution for the source files.        *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void WINAPI OnFileLine(PTSTR pszLine, int cchMaxLine /* normally 4096 chars */)
{
    PTSTR psz;

    for (;;)
    {
        if ((psz = _tcsstr(pszLine, TEXT("<Projname>"))) != NULL)
        {
            int cchLine = lstrlen(pszLine);
            _tmemmove(psz + cchProjname, psz + 10, pszLine + cchLine - (psz + 10) + 1);
            _tmemcpy(psz, g_szProjname, cchProjname);
        }
        else if ((psz = _tcsstr(pszLine, TEXT("<PROJNAME>"))) != NULL)
        {
            int cchLine = lstrlen(pszLine);
            _tmemmove(psz + cchProjname, psz + 10, pszLine + cchLine - (psz + 10) + 1);
            _tmemcpy(psz, g_szPROJNAME, cchProjname);
        }
        else if ((psz = _tcsstr(pszLine, TEXT("<Dispname>"))) != NULL)
        {
            int cchLine = lstrlen(pszLine);
            _tmemmove(psz + cchDispname, psz + 10, pszLine + cchLine - (psz + 10) + 1);
            _tmemcpy(psz, g_szDispname, cchDispname);
        }
        else if ((psz = _tcsstr(pszLine, TEXT("DrawText("))) != NULL && g_fSimple)
        {
            lstrcpy(pszLine, TEXT("    /* TODO: Enter any drawing code here */"));
        }
        else break;
    }
}

