/****************************************************************************
 *                                                                          *
 * File    : tipofday.c                                                     *
 *                                                                          *
 * Purpose : Tip-of-the-day custom control.                                 *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <time.h>
#include "tipofday.h"

PTIPNODE g_pTipHead = NULL;     // Head of linked list of tips.
PTIPNODE g_pTip;                // Pointer to current tip.
HANDLE g_hMod;                  // DLL's module handle.
LPCCSTYLE g_pccs;               // Pointer to a CCSTYLE structure.

CCSTYLEFLAGA aTipofdayStyleFlags[] =
{
    { TIP_RESERVED1, 0, "TIP_RESERVED1" },
    { TIP_RESERVED2, 0, "TIP_RESERVED2" }
};

// Static function prototypes.
static BOOL CALLBACK TipofdayStyle(HWND, LPCCSTYLE);
static int CALLBACK TipofdaySizeToText(DWORD, DWORD, HFONT, LPSTR);
static BOOL RegisterTipofdayClass(void);
static BOOL UnregisterTipofdayClass(void);
static LRESULT CALLBACK TipofdayWndProc(HWND, UINT, WPARAM, LPARAM);
static BOOL Tipofday_OnCreate(HWND, CREATESTRUCT *);
static UINT Tipofday_OnGetDlgCode(HWND, MSG *);
static BOOL Tipofday_OnEraseBkgnd(HWND, HDC);
static void Tipofday_OnPaint(HWND);
static void Tipofday_OnForceTip(HWND, PTSTR, int);
static void Tipofday_OnNextTip(HWND);
static BOOL CALLBACK TipofdayDlgProc(HWND, UINT, WPARAM, LPARAM);
static BOOL ScanForTipFiles(void);
static PTIPNODE AllocTipLink(LPCTSTR);
static void FreeTipLink(PTIPNODE);
static void FreeTipLinkList(void);
static BOOL InitTipFile(PTIPNODE);
static BOOL GetTipText(PTIPNODE);
static PTIPNODE GetRandomTipLink(void);
static PTIPNODE GetNextTipLink(PTIPNODE);
static void AddMostRecentTip(PTIPNODE);
static DWORD LoadMostRecentTips(PTIPNODE);
static DWORD SaveMostRecentTips(PTIPNODE);
static void ClearUnreferencedTipFiles(void);

/****************************************************************************
 *                                                                          *
 * Function: DllMain                                                        *
 *                                                                          *
 * Purpose : DLL entry and exit procedure.                                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

BOOL WINAPI DllMain(HANDLE hDLL, DWORD dwReason, LPVOID lpReserved)
{
    g_hMod = hDLL;

    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
            if (!RegisterTipofdayClass())
                return FALSE;

            if (!ScanForTipFiles())
                return FALSE;

            // Seed the random number generator.
            srand((UINT)time(NULL));

            return TRUE;

        case DLL_PROCESS_DETACH:
            if (!UnregisterTipofdayClass())
                return FALSE;

            // Remove unreferenced Registry values.
            ClearUnreferencedTipFiles();

            FreeTipLinkList();
            return TRUE;

        default:
            return TRUE;
    }
}

/****************************************************************************
 *                                                                          *
 * Function: CustomControlInfoA                                             *
 *                                                                          *
 * Purpose : Custom control interface.                                      *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

UINT __declspec(dllexport) CALLBACK CustomControlInfoA(LPCCINFOA acci)
{
    //
    // Pelles C IDE is querying the number of controls this DLL supports,
    // so return 1. Then we'll get called again with a valid "acci".
    //
    if (!acci)
        return 1;

    //
    // Fill in the constant values.
    //
    acci[0].flOptions         = 0;
    acci[0].cxDefault         = 150;    // Default width (dialog units).
    acci[0].cyDefault         = 100;    // Default height (dialog units).
    acci[0].flStyleDefault    = WS_CHILD|WS_VISIBLE|WS_BORDER|TIP_RESERVED2;
    acci[0].flExtStyleDefault = 0;
    acci[0].flCtrlTypeMask    = 0;
    acci[0].cStyleFlags       = NUM_TIPOFDAY_STYLES;
    acci[0].aStyleFlags       = aTipofdayStyleFlags;
    acci[0].lpfnStyle         = TipofdayStyle;
    acci[0].lpfnSizeToText    = TipofdaySizeToText;
    acci[0].dwReserved1       = 0;
    acci[0].dwReserved2       = 0;

    //
    // Copy the strings.
    //
    lstrcpyn(acci[0].szClass, TIPOFDAY_CLASS, CCHCCCLASS);
    lstrcpyn(acci[0].szDesc, TIPOFDAY_DESCRIPTION, CCHCCDESC);
    lstrcpyn(acci[0].szTextDefault, TIPOFDAY_DEFAULTTEXT, CCHCCTEXT);

    //
    // Return the number of controls that the DLL supports.
    //
    return 1;
}

/****************************************************************************
 *                                                                          *
 * Function: TipofdayStyle                                                  *
 *                                                                          *
 * Purpose : Custom control interface.                                      *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static BOOL CALLBACK TipofdayStyle(HWND hwndParent, LPCCSTYLE pccs)
{
    int ret;

    g_pccs = pccs;

    ret = DialogBox(g_hMod, _T("TipofdayStyle"), hwndParent, TipofdayDlgProc);
    if (ret == -1)
    {
        MessageBox(hwndParent, _T("TipofdayStyle(): DialogBox failed"),
            _T("TIPOFDAY.DLL"), MB_OK|MB_ICONEXCLAMATION|MB_APPLMODAL);

        ret = 0;
    }

    return (BOOL)ret;
}

/****************************************************************************
 *                                                                          *
 * Function: TipofdaySizeToText                                             *
 *                                                                          *
 * Purpose : Custom control interface.                                      *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static int CALLBACK TipofdaySizeToText(DWORD flStyle, DWORD flExtStyle, HFONT hFont, LPSTR pszText)
{
    /* We don't support "size to text" so we just return -1 */
    return -1;
}

/****************************************************************************
 *                                                                          *
 * Function: RegisterTipofdayClass                                          *
 *                                                                          *
 * Purpose : Register tip-of-the-day class.                                 *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static BOOL RegisterTipofdayClass(void)
{
    WNDCLASS wc;

    wc.style         = CS_GLOBALCLASS;
    wc.lpfnWndProc   = TipofdayWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = g_hMod;
    wc.hIcon         = NULL;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = TIPOFDAY_CLASS;

    if (!RegisterClass(&wc))
        return FALSE;

    return TRUE;
}

/****************************************************************************
 *                                                                          *
 * Function: UnregisterTipofdayClass                                        *
 *                                                                          *
 * Purpose : Unregister tip-of-the-day class.                               *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static BOOL UnregisterTipofdayClass(void)
{
    return UnregisterClass(TIPOFDAY_CLASS, g_hMod);
}

/****************************************************************************
 *                                                                          *
 * Function: TipofdayWndProc                                                *
 *                                                                          *
 * Purpose : Tipofday window procedure.                                     *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static LRESULT CALLBACK TipofdayWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        // Windows messages.
        HANDLE_MSG(hwnd, WM_CREATE, Tipofday_OnCreate);
        HANDLE_MSG(hwnd, WM_GETDLGCODE, Tipofday_OnGetDlgCode);
        HANDLE_MSG(hwnd, WM_ERASEBKGND, Tipofday_OnEraseBkgnd);
        HANDLE_MSG(hwnd, WM_PAINT, Tipofday_OnPaint);

        // Private messages.
        HANDLE_MSG(hwnd, TM_FORCETIP, Tipofday_OnForceTip);
        HANDLE_MSG(hwnd, TM_NEXTTIP, Tipofday_OnNextTip);

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

/****************************************************************************
 *                                                                          *
 * Function: Tipofday_OnCreate                                              *
 *                                                                          *
 * Purpose : Handle WM_CREATE message.                                      *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static BOOL Tipofday_OnCreate(HWND hwnd, CREATESTRUCT *pcs)
{
    g_pTip = GetRandomTipLink();

    return GetTipText(g_pTip);
}

/****************************************************************************
 *                                                                          *
 * Function: Tipofday_OnGetDlgCode                                          *
 *                                                                          *
 * Purpose : Handle WM_GETDLGCODE message.                                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static UINT Tipofday_OnGetDlgCode(HWND hwnd, MSG *pmsg)
{
    return DLGC_STATIC;
}

/****************************************************************************
 *                                                                          *
 * Function: Tipofday_OnEraseBkgnd                                          *
 *                                                                          *
 * Purpose : Handle WM_ERASEBKGND message.                                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static BOOL Tipofday_OnEraseBkgnd(HWND hwnd, HDC hdc)
{
    // I do it myself.
    return TRUE;
}

/****************************************************************************
 *                                                                          *
 * Function: Tipofday_OnPaint                                               *
 *                                                                          *
 * Purpose : Handle WM_PAINT message.                                       *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void Tipofday_OnPaint(HWND hwnd)
{
    PAINTSTRUCT ps;
    LOGFONT lf;
    TEXTMETRIC tm;
    HFONT hFontLarge;
    HFONT hFontSmall;
    HFONT hFontSave;
    RECT rcClient;
    RECT rcBounds;
    int cxIcon;
    int cyIcon;
    TCHAR szText[80];

    BeginPaint(hwnd, &ps);

    memset(&lf, 0, sizeof(lf));

    //
    // Create large font.
    //
    lstrcpy(lf.lfFaceName, LARGE_FONTNAME);
    lf.lfHeight = -MulDiv(LARGE_FONTSIZE, LogPixelsY(ps.hdc), 72);
    lf.lfWeight = FW_NORMAL;
    lf.lfQuality = PROOF_QUALITY;
    hFontLarge = CreateFontIndirect(&lf);

    //
    // Create small font.
    //
    lstrcpy(lf.lfFaceName, SMALL_FONTNAME);
    lf.lfHeight = -MulDiv(SMALL_FONTSIZE, LogPixelsY(ps.hdc), 72);
    lf.lfWeight = FW_NORMAL;
    lf.lfQuality = PROOF_QUALITY;
    hFontSmall = CreateFontIndirect(&lf);

    GetClientRect(hwnd, &rcClient);

    cxIcon = GetSystemMetrics(SM_CXICON);
    cyIcon = GetSystemMetrics(SM_CYICON);

    //
    // Draw left pane with icon.
    //
    rcBounds = rcClient; rcBounds.right = cxIcon * 2;
    FillRect(ps.hdc, &rcBounds, GetSysColorBrush(COLOR_APPWORKSPACE));

    DrawIcon(ps.hdc, cxIcon/2, cyIcon/2,
        LoadIcon(g_hMod, MAKEINTRESOURCE(1)));

    //
    // Draw right pane with text.
    //
    rcClient.left = rcBounds.right;
    FillRect(ps.hdc, &rcClient, GetSysColorBrush(COLOR_INFOBK));

    InflateRect(&rcClient, -8, -8);

    SetTextColor(ps.hdc, GetSysColor(COLOR_INFOTEXT));
    SetBkMode(ps.hdc, TRANSPARENT);

    hFontSave = SelectFont(ps.hdc, hFontLarge);
    GetTextMetrics(ps.hdc, &tm);

    LoadString(g_hMod, 1, szText, NELEMS(szText));

    rcBounds = rcClient; rcBounds.bottom = rcBounds.top + tm.tmHeight;
    DrawText(ps.hdc, szText, -1, &rcBounds, DT_LEFT|DT_SINGLELINE|DT_NOPREFIX);

    SelectFont(ps.hdc, hFontSmall);

    rcBounds.top = rcBounds.bottom + 8; rcBounds.bottom = rcClient.bottom;
    DrawText(ps.hdc, g_pTip->szTipText, -1, &rcBounds, DT_LEFT|DT_WORDBREAK|DT_NOPREFIX);

    //
    // Clean up the mess.
    //
    SelectFont(ps.hdc, hFontSave);
    DeleteFont(hFontLarge);
    DeleteFont(hFontSmall);

    EndPaint(hwnd, &ps);
}

/****************************************************************************
 *                                                                          *
 * Function: Tipofday_OnForceTip                                            *
 *                                                                          *
 * Purpose : Handle private TM_FORCETIP message.                            *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void Tipofday_OnForceTip(HWND hwnd, PTSTR pszFileName, int iTip)
{
    PTIPNODE pTip;

    for (pTip = g_pTipHead; pTip != NULL; pTip = pTip->pNext)
    {
        if (lstrcmpi(pTip->szFileName, pszFileName) == 0)
        {
            pTip->iTip = max(min(iTip, pTip->cTips), 1);

            if (GetTipText(pTip))
                g_pTip = pTip;

            InvalidateRect(hwnd, NULL, TRUE);
            return;
        }
    }
}

/****************************************************************************
 *                                                                          *
 * Function: Tipofday_OnNextTip                                             *
 *                                                                          *
 * Purpose : Handle private TM_NEXTTIP message.                             *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void Tipofday_OnNextTip(HWND hwnd)
{
    g_pTip = GetNextTipLink(g_pTip);
    GetTipText(g_pTip);

    InvalidateRect(hwnd, NULL, TRUE);
}

/****************************************************************************
 *                                                                          *
 * Function: TipofdayDlgProc                                                *
 *                                                                          *
 * Purpose : Custom control dialog procedure.                               *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static BOOL CALLBACK TipofdayDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_INITDIALOG:
        {
            if (g_pccs->flStyle & TIP_RESERVED1)
                CheckDlgButton(hDlg, DID_RESERVED1, 1);

            if (g_pccs->flStyle & TIP_RESERVED2)
                CheckDlgButton(hDlg, DID_RESERVED2, 1);

            break;
        }

        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
                case DID_RESERVED1:
                    if (IsDlgButtonChecked(hDlg, DID_RESERVED1))
                        g_pccs->flStyle |= TIP_RESERVED1;
                    else
                        g_pccs->flStyle &= ~TIP_RESERVED1;
                    break;

                case DID_RESERVED2:
                    if (IsDlgButtonChecked(hDlg, DID_RESERVED2))
                        g_pccs->flStyle |= TIP_RESERVED2;
                    else
                        g_pccs->flStyle &= ~TIP_RESERVED2;
                    break;

                case DID_OK:
                    EndDialog(hDlg, 1);
                    break;
            }
            break;
        }
    }

    return FALSE;
}

/****************************************************************************
 *                                                                          *
 * Function: ScanForTipFiles                                                *
 *                                                                          *
 * Purpose : Scan the startup directory for tip-of-the-day files.           *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static BOOL ScanForTipFiles(void)
{
    TCHAR szFileName[MAX_PATH];
    PTSTR psz;
    WIN32_FIND_DATA wfd;
    HANDLE hff;

    GetModuleFileName(g_hMod, szFileName, NELEMS(szFileName));
    psz = _tcsrchr(szFileName, '\\') + 1;
    lstrcpy(psz, FILESELSTRING);

    hff = FindFirstFile(szFileName, &wfd);
    if (hff == INVALID_HANDLE_VALUE)
        return FALSE;

    do
    {
        PTIPNODE pTip;

        // Re-use the buffer which contains the path.
        lstrcpy(psz, wfd.cFileName);

        // Allocate a node for this file.
        if (!(pTip = AllocTipLink(szFileName)))
        {
            FreeTipLinkList();
            break;
        }

        // Free file node if error (but don't quit!).
        if (!InitTipFile(pTip))
            FreeTipLink(pTip);

    } while (FindNextFile(hff, &wfd));
    FindClose(hff);

    // Return SUCCESS if we found something.
    return g_pTipHead != NULL;
}

/****************************************************************************
 *                                                                          *
 * Function: AllocTipLink                                                   *
 *                                                                          *
 * Purpose : Allocate TIPNODE structure. This is a node in a linked list.   *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static PTIPNODE AllocTipLink(LPCTSTR pszFileName)
{
    PTIPNODE pTip;
    PTIPNODE pTipT;

    //
    // Allocate a new node.
    //
    if (!(pTip = malloc(sizeof(TIPNODE))))
        return NULL;

    memset(pTip, 0, sizeof(TIPNODE));
    lstrcpy(pTip->szFileName, pszFileName);

    //
    // Insert link in the list.
    //
    if (!g_pTipHead)
    {
        //
        // This is the first node. Start the list.
        //
        g_pTipHead = pTip;
    }
    else
    {
        //
        // Find the end of the list and tack on the new node.
        //
        for (pTipT = g_pTipHead;
             pTipT->pNext;
             pTipT = pTipT->pNext)
            ;

        pTipT->pNext = pTip;
    }

    //
    // Load most recently used tips from the Registry.
    //
    (void)LoadMostRecentTips(pTip);

    return pTip;
}

/****************************************************************************
 *                                                                          *
 * Function: FreeTipLink                                                    *
 *                                                                          *
 * Purpose : Free given node and close up the linked list.                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void FreeTipLink(PTIPNODE pTipFree)
{
    PTIPNODE pTip;
    PTIPNODE pTipPrev;

    //
    // Find the existing node and get it's previous link.
    //
    for (pTip = g_pTipHead, pTipPrev = NULL;
         pTip && pTip != pTipFree;
         pTipPrev = pTip, pTip = pTip->pNext)
        ;

    // Did we find the node?
    if (pTip)
    {
        //
        // Close up the linked list.
        //
        if (pTipPrev)
            pTipPrev->pNext = pTipFree->pNext;
        else
            g_pTipHead = pTipFree->pNext;

        //
        // Save most recently used tips in the Registry.
        //
        (void)SaveMostRecentTips(pTipFree);

        //
        // Free the link itself.
        //
        free(pTipFree);
    }
}

/****************************************************************************
 *                                                                          *
 * Function: FreeTipLinkList                                                *
 *                                                                          *
 * Purpose : Free all nodes in a linked list (if any).                      *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void FreeTipLinkList(void)
{
    while (g_pTipHead != NULL)
        FreeTipLink(g_pTipHead);
}

/****************************************************************************
 *                                                                          *
 * Function: InitTipFile                                                    *
 *                                                                          *
 * Purpose : Open and parse a tip-of-the-day file.                          *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static BOOL InitTipFile(PTIPNODE pTip)
{
    TCHAR szText[CCHMAXTEXT];
    FILE *fd;

    if (!pTip)
        return FALSE;

    fd = fopen(pTip->szFileName, "r");
    if (fd == NULL) return FALSE;

    // Maximize buffering.
    setvbuf(fd, NULL, _IOFBF, 2048);

    // Read through the tip file and collect information.
    while ((fgets(szText, CCHMAXTEXT, fd)) != NULL)
    {
        PTSTR psz;

        // Skip comments and blank lines.
        for (psz = szText; isspace(*psz); ++psz)
            ;

        if (*psz == ';' || *psz == '\0')
            continue;

        // Update tip count.
        pTip->cTips++;
    }

    fclose(fd);

    return TRUE;
}

/****************************************************************************
 *                                                                          *
 * Function: GetTipText                                                     *
 *                                                                          *
 * Purpose : Return a tip-of-the-day text.                                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static BOOL GetTipText(PTIPNODE pTip)
{
    TCHAR szText[CCHMAXTEXT];
    FILE *fd;
    int iTip;

    if (!pTip)
        return FALSE;

    fd = fopen(pTip->szFileName, "r");
    if (fd == NULL) return FALSE;

    // Maximize buffering.
    setvbuf(fd, NULL, _IOFBF, 2048);

    iTip = pTip->iTip;
    while ((fgets(szText, CCHMAXTEXT, fd)) != NULL)
    {
        PTSTR psz;

        // Skip comments and blank lines.
        for (psz = szText; isspace(*psz); ++psz)
            ;

        if (*psz == ';' || *psz == '\0')
            continue;

        if (--iTip == 0)
        {
            PTSTR pszT = pTip->szTipText;

            while (*psz != '\0')
            {
                // Convert "\n" to line feed.
                if (*psz == '\\' && *(psz+1) == 'n')
                    *pszT++ = '\n', psz += 2;
                else
                    *pszT++ = *psz++;
            }
            *pszT = '\0';

            AddMostRecentTip(pTip);

            fclose(fd);
            return TRUE;
        }
    }

    fclose(fd);

    // Clear current tip text buffer.
    pTip->szTipText[0] = '\0';

    return FALSE;
}

/****************************************************************************
 *                                                                          *
 * Function: GetRandomTipLink                                               *
 *                                                                          *
 * Purpose : Return a tip-of-the-day node.                                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static PTIPNODE GetRandomTipLink(void)
{
    PTIPNODE pTip;
    int cFiles;
    int iFile;

    cFiles = 0;
    for (pTip = g_pTipHead; pTip; pTip = pTip->pNext)
        cFiles++;

    iFile = 1 + (rand() % cFiles);

    for (pTip = g_pTipHead; pTip != NULL && --iFile > 0; pTip = pTip->pNext)
        ;

    if (pTip)
    {
        int nPrioBest = -1;
        int iTipBest;
        int i;

        for (i = 0; i < CMAXRECENTTIPS && nPrioBest != 255; i++)
        {
            int nPrio = 255;
            int iTip;
            int i;

            iTip = 1 + (rand() % pTip->cTips);

            for (i = 0; i < CMAXRECENTTIPS; i++)
            {
                if (iTip == pTip->aiRecentTip[i])
                {
                    nPrio = i;
                    break;
                }
            }

            if (nPrio > nPrioBest)
            {
                nPrioBest = nPrio;
                iTipBest = iTip;
            }
        }

        pTip->iTip = iTipBest;
    }

    return pTip;
}

/****************************************************************************
 *                                                                          *
 * Function: GetNextTipLink                                                 *
 *                                                                          *
 * Purpose : Return a tip-of-the-day node.                                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static PTIPNODE GetNextTipLink(PTIPNODE pTip)
{
    if (pTip)
    {
        //
        // Try bump tip index in current file.
        //
        if (++pTip->iTip <= pTip->cTips)
            return pTip;

        //
        // Out of tips in this file, go to next or first file.
        //
        if ((pTip = pTip->pNext) || (pTip = g_pTipHead))
            pTip->iTip = 1;
    }

    return pTip;
}

/****************************************************************************
 *                                                                          *
 * Function: AddMostRecentTip                                               *
 *                                                                          *
 * Purpose : Add tip to list of most recently used tips.                    *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void AddMostRecentTip(PTIPNODE pTip)
{
    int iTip;
    int i;

    iTip = pTip->iTip;

    for (i = 0; i < CMAXRECENTTIPS; i++)
    {
        SWAP(int, iTip, pTip->aiRecentTip[i]);

        if (iTip == pTip->iTip)
            break;
    }
}

/****************************************************************************
 *                                                                          *
 * Function: LoadMostRecentTips                                             *
 *                                                                          *
 * Purpose : Load a files most recently used tips from the Registry.        *
 *                                                                          *
 * History : Date      Reason                                               *
 *           97-04-16  Created                                              *
 *                                                                          *
 ****************************************************************************/

static DWORD LoadMostRecentTips(PTIPNODE pTip)
{
    DWORD err;
    HKEY hKey;

    err = RegOpenKeyEx(HKEY_CURRENT_USER, REGISTRYKEY, 0, KEY_READ, &hKey);
    if (err == ERROR_SUCCESS)
    {
        PTSTR pszValueName = _tcsrchr(pTip->szFileName, '\\') + 1;
        REGDATA Data;
        DWORD cbData;

        cbData = sizeof(REGDATA);

        err = RegQueryValueEx(hKey, pszValueName, NULL, NULL,
            (PBYTE)(PREGDATA)&Data, &cbData);

        if (err == ERROR_SUCCESS && cbData == sizeof(REGDATA))
        {
            if (Data.wVersion == REGVERSION)
            {
                int i;

                for (i = 0; i < CMAXRECENTTIPS; i++)
                    pTip->aiRecentTip[i] = Data.awRecentTip[i];
            }
        }

        RegCloseKey(hKey);
    }

    return err;
}

/****************************************************************************
 *                                                                          *
 * Function: SaveMostRecentTips                                             *
 *                                                                          *
 * Purpose : Save a files most recently used tips to the Registry.          *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static DWORD SaveMostRecentTips(PTIPNODE pTip)
{
    DWORD err;
    DWORD dwDisp;
    HKEY hKey;

    err = RegCreateKeyEx(HKEY_CURRENT_USER, REGISTRYKEY, 0, _T(""),
        REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDisp);
    if (err == ERROR_SUCCESS)
    {
        PTSTR pszValueName = _tcsrchr(pTip->szFileName, '\\') + 1;
        REGDATA Data;
        int i;

        for (i = 0; i < CMAXRECENTTIPS; i++)
            Data.awRecentTip[i] = pTip->aiRecentTip[i];

        Data.wVersion = REGVERSION;

        err = RegSetValueEx(hKey, pszValueName, 0, REG_BINARY,
            (PBYTE)(PREGDATA)&Data, sizeof(REGDATA));

        RegCloseKey(hKey);
    }

    return err;
}

/****************************************************************************
 *                                                                          *
 * Function: ClearUnreferencedTipFiles                                      *
 *                                                                          *
 * Purpose : Delete unreferenced values in the Registry.                    *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void ClearUnreferencedTipFiles(void)
{
    DWORD err;
    HKEY hKey;

    err = RegOpenKeyEx(HKEY_CURRENT_USER, REGISTRYKEY, 0, KEY_ALL_ACCESS, &hKey);
    if (err == ERROR_SUCCESS)
    {
        DWORD iValue;

        for (iValue = 0; err == ERROR_SUCCESS; iValue++)
        {
            TCHAR szValue[MAX_PATH] = "";
            DWORD cchValue = MAX_PATH;

            err = RegEnumValue(hKey, iValue, szValue, &cchValue,
                NULL, NULL, NULL, NULL);
            if (err == ERROR_SUCCESS)
            {
                PTIPNODE pTip;

                for (pTip = g_pTipHead; pTip; pTip = pTip->pNext)
                {
                    PTSTR pszValue = _tcsrchr(pTip->szFileName, '\\') + 1;

                    if (lstrcmpi(pszValue, szValue) == 0)
                        break;
                }

                if (!pTip)
                {
                    // Delete unreferenced value.
                    err = RegDeleteValue(hKey, szValue);
                }
            }
        }

        RegCloseKey(hKey);
    }
}
