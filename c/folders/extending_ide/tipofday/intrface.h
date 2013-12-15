/****************************************************************************
 *                                                                          *
 * File    : intrface.h                                                     *
 *                                                                          *
 * Purpose : Tip-of-the-day custom control interface.                       *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

#ifndef _INTRFACE_H
#define _INTRFACE_H

// Tip-of-the-day window class name.
#define TIPOFDAY_CLASS  _T("Tipofday32")

// Tip-of-the-day window styles.
#define TIP_RESERVED1  0x0001
#define TIP_RESERVED2  0x0002

// Tip-of-the-day control messages.
#define TM_FORCETIP  (WM_USER + 0x100)
#define TM_NEXTTIP   (WM_USER + 0x101)

// Tip-of-the-day control message APIs.
#define Tipofday_ForceTip(hwndCtl,lpszfile,index)  ((void)SendMessage((hwndCtl), TM_FORCETIP, (WPARAM)(int)(index), (LPARAM)(LPTSTR)(lpszfile)))
#define Tipofday_NextTip(hwndCtl)                  ((void)SendMessage((hwndCtl), TM_NEXTTIP, 0, 0))

#endif // _INTRFACE_H
