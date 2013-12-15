/****************************************************************************
 *                                                                          *
 * File    : tipofday.h                                                     *
 *                                                                          *
 * Purpose : Constants and definitions.                                     *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

#include <custcntl.h>
#include "intrface.h"

//
// Constants ----------------------------------------------------------------
//

#define TIPOFDAY_DESCRIPTION    _T("Tip of the day")
#define TIPOFDAY_DEFAULTTEXT    _T(":-)")

#define LARGE_FONTNAME          _T("Comic Sans MS")
#define LARGE_FONTSIZE          14

#define SMALL_FONTNAME          _T("MS Sans Serif")
#define SMALL_FONTSIZE          10

#define REGISTRYKEY             _T("Software\\Pelle Orinius\\TipOfDay")
#define FILESELSTRING           _T("*.TIP")

#define CCHSTYLE                20  // size of style string.
#define CCHMAXTEXT              512

#define NUM_TIPOFDAY_STYLES     2

#define DID_RESERVED1           101 // Dialog control id's.
#define DID_RESERVED2           102
#define DID_OK                  103

#define CMAXRECENTTIPS          32

#define REGVERSION              0x0100

//
// Macros -------------------------------------------------------------------
//

#define NELEMS(a)  (sizeof(a) / sizeof(a[0]))
#define SWAP(type,a,b)  { type t; t = (a); (a) = (b); (b) = t; }

#define LogPixelsY(hdc)  GetDeviceCaps((hdc), LOGPIXELSY)

//
// Private message crackers.
//
#define HANDLE_TM_FORCETIP(hwnd,wParam, lParam, fn) \
    ((fn)((hwnd), (LPTSTR)(LPARAM)(lParam), (int)(WPARAM)(wParam)), 0)
#define HANDLE_TM_NEXTTIP(hwnd,wParam, lParam, fn) \
    ((fn)(hwnd), 0)

//
// Structures ---------------------------------------------------------------
//

typedef struct tagTIPNODE {
    struct tagTIPNODE *pNext;               // Ptr to next node.
    TCHAR szFileName[MAX_PATH];             // File name.
    int cTips;                              // Number of tips in file.
    int iTip;                               // Index of current tip (1-max).
    TCHAR szTipText[CCHMAXTEXT];            // Text of current tip.
    int aiRecentTip[CMAXRECENTTIPS];        // Most recently used tips.
} TIPNODE, *PTIPNODE;

typedef struct tagREGDATA {
    WORD wVersion;                          // REGVERSION.
    WORD awRecentTip[CMAXRECENTTIPS];       // Most recently used tips.
} REGDATA, *PREGDATA;
