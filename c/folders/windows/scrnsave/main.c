/****************************************************************************
 *                                                                          *
 * Filename: main.c                                                         *
 *                                                                          *
 * Purpose : Windows sample for Pelles C for Windows.                       *
 *                                                                          *
 *           Very "beautiful" screensaver.                                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

#include <windows.h>
#include <windowsx.h>
#include <process.h>
#include <stdlib.h>
#include <scrnsave.h>
#include <time.h>
#include "main.h"

#define BACKGROUND_BRUSH  LTGRAY_BRUSH

void __cdecl StartSample(void *);  // prototype defined by _beginthread()
HPALETTE CreateDIBPalette(PINFO, BITMAPINFO *);
BOOL IsPaletteDevice(void);
BYTE *FindDIBBits(BITMAPINFOHEADER *);
int GetDIBPaletteSize(BITMAPINFOHEADER *);
int GetDIBPaletteEntries(BITMAPINFOHEADER *);
void InitInfo(PINFO);
void LoadStrings(void);

/****************************************************************************
 *                                                                          *
 * Function: ScreenSaverProc                                                *
 *                                                                          *
 * Purpose : Creates a new execution thread that handles painting.          *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

LONG APIENTRY ScreenSaverProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_CREATE:
            //
            // Load the resource strings and initialize our private structure.
            //
            LoadStrings();
            InitInfo(&gInfo);
            break;

        case WM_ERASEBKGND:
        {
            RECT rcClient;

            //
            // Erase the background of the window.
            //
            GetClientRect(hwnd, &rcClient);
            FillRect((HDC)wParam, &rcClient, GetStockBrush(BACKGROUND_BRUSH));

            //
            // Create the painting thread, if not created before.
            //
            if (gInfo.hThrd == NULL)
            {
                gInfo.hwnd = hwnd;

                if (gInfo.hThrd = (HANDLE)_beginthread(StartSample, 0, &gInfo))
                    SetThreadPriority(gInfo.hThrd, THREAD_PRIORITY_BELOW_NORMAL);
            }
            return 0;
        }

        case WM_DESTROY:
            //
            // Destroy any created painting thread.
            //
            if (gInfo.hThrd != NULL)
            {
                gInfo.fDestroy = TRUE;  /* tell the thread to quit */
                WaitForSingleObject(gInfo.hThrd, INFINITE);  /* wait for the thread to end */
                CloseHandle(gInfo.hThrd);  /* close the thread handle */
                gInfo.hThrd = NULL;
            }
            break;
    }

    // Important to call the screensaver default message handler.
    return DefScreenSaverProc(hwnd, message, wParam, lParam);
}

/****************************************************************************
 *                                                                          *
 * Function: ScreenSaverConfigureDialog                                     *
 *                                                                          *
 * Purpose : Handles the configuration dialog.                              *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

BOOL APIENTRY ScreenSaverConfigureDialog(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_INITDIALOG:
            LoadStrings();
            return TRUE;

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDOK:
                    EndDialog(hwndDlg, LOWORD(wParam) == IDOK);
                    return TRUE;

                default:
                    break;
            }

        default:
            break;
    }

    return FALSE;
}

/****************************************************************************
 *                                                                          *
 * Function: RegisterDialogClasses                                          *
 *                                                                          *
 * Purpose : Registers any classes for the configuration dialog.            *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

BOOL WINAPI RegisterDialogClasses(HANDLE hInst)
{
    return TRUE;
}

/****************************************************************************
 *                                                                          *
 * Function: StartSample                                                    *
 *                                                                          *
 * Purpose : Thread function that handles painting.                         *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

void __cdecl StartSample(void *pv)
{
    HGLOBAL hgRes = NULL;
    HRSRC hrsrc;
    PINFO pInfo;
    int cxClient;
    int cyClient;
    RECT rc;

    pInfo = (PINFO)pv;

    // Initialize the random number generator.
    srand ((UINT)time(NULL));

    // Calculate size of the client area.
    GetClientRect(pInfo->hwnd, &rc);
    cxClient = (rc.right - rc.left);
    cyClient = (rc.bottom - rc.top);

    // Load the correct bitmap (DIB) from the resources.
    if ((hrsrc = FindResource(hMainInstance, RESOURCEID(fChildPreview), RT_BITMAP)) &&
        (hgRes = LoadResource(hMainInstance, hrsrc)))
    {
        BITMAPINFO *pbmi = LockResource(hgRes);
        HPALETTE hpal;
        HPALETTE hpalSave1;
        PBYTE pBits;
        int cxBitmap;
        int cyBitmap;
        int x;
        int y;
        int xStep;
        int yStep;
        HDC hdc;

        hdc = GetDC(pInfo->hwnd);

        hpal = CreateDIBPalette(pInfo, pbmi);
        if (hpal)
        {
            hpalSave1 = SelectPalette(hdc, hpal, FALSE);
            RealizePalette(hdc);
        }

        pBits = FindDIBBits((BITMAPINFOHEADER *)pbmi);

        cxBitmap = pbmi->bmiHeader.biWidth;
        cyBitmap = pbmi->bmiHeader.biHeight;

        // Generate a random startup position on the screen.
        x = (rand() % (cxClient - cxBitmap));
        y = (rand() % (cyClient - cyBitmap));

        // Step size (decides the animation speed together with the Sleep amount).
        xStep = 1;
        yStep = 1;

        // No position to remember on the first loop.
        SetRectEmpty(&rc);

        // Loop until the main thread says 'drop dead'.
        while (!pInfo->fDestroy)
        {
            BOOL fSound = FALSE;

            x += xStep;
            y += yStep;

            // Bounce here...
            if (x < 0 || x + cxBitmap > cxClient)
                xStep = -xStep, fSound = TRUE;

            // ...or bounce there...
            if (y < 0 || y + cyBitmap > cyClient)
                yStep = -yStep, fSound = TRUE;

            // ...and make some noice when needed...
            if (fSound && !fChildPreview)
            {
                PlaySound(MAKEINTRESOURCE(IDR_WAV_DING),
                    hMainInstance, SND_RESOURCE|SND_ASYNC);
            }

            // Draw the bitmap in the new position...
            SetDIBitsToDevice(hdc, x, y, cxBitmap, cyBitmap,
                0, 0, 0, cyBitmap, pBits, pbmi, DIB_RGB_COLORS);

            // ...and remove it from the old position...
            if (!IsRectEmpty(&rc))
            {
                ExcludeClipRect(hdc, x, y, x + cxBitmap, y + cyBitmap);
                FillRect(hdc, &rc, GetStockObject(BACKGROUND_BRUSH));
                SelectClipRgn(hdc, NULL);
            }

            // Remember position for the next loop.
            SetRect(&rc, x, y, x + cxBitmap, y + cyBitmap);

            // Zzzz...
            Sleep(fChildPreview ? 250 : 50);
        }

        if (hpal)
        {
            SelectPalette(hdc, hpalSave1, FALSE);
            DeletePalette(hpal);
        }

        ReleaseDC(pInfo->hwnd, hdc);
    }
}

/****************************************************************************
 *                                                                          *
 * Function: CreateDIBPalette                                               *
 *                                                                          *
 * Purpose : Return a logical palette from a DIB (bitmap).                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

HPALETTE CreateDIBPalette(PINFO pInfo, BITMAPINFO *pbmi)
{
    //
    // Is the screen palette-based?
    //
    if (!IsPaletteDevice())
        return NULL;

    //
    // The screen is palette based; Create a palette for 2^8 colors.
    //
    if (pbmi->bmiHeader.biBitCount == 8)
    {
        int cEntries = GetDIBPaletteEntries(&pbmi->bmiHeader);
        PLOGPALETTE ppal;
        HPALETTE hpal;
        int i;

        ppal = (PLOGPALETTE)malloc(SIZEOF_LOGPALETTE(cEntries));
        if (ppal)
        {
            ppal->palVersion = PALVERSION;
            ppal->palNumEntries = cEntries;

            for (i = 0; i < cEntries; i++)
            {
                ppal->palPalEntry[i].peRed = pbmi->bmiColors[i].rgbRed;
                ppal->palPalEntry[i].peGreen = pbmi->bmiColors[i].rgbGreen;
                ppal->palPalEntry[i].peBlue = pbmi->bmiColors[i].rgbBlue;
                ppal->palPalEntry[i].peFlags = 0;
            }

            hpal = CreatePalette(ppal);
            free(ppal);

            return hpal;
        }
    }

    //
    // The screen is palette based; Create a palette for 2^16 or more colors.
    //
    if (pbmi->bmiHeader.biBitCount >= 16)
    {
        HDC hdc;

        hdc = GetDC(pInfo->hwnd);
        if (hdc)
        {
            HPALETTE hpal = CreateHalftonePalette(hdc);
            ReleaseDC(pInfo->hwnd, hdc);
            return hpal;
        }
    }

    return NULL;
}

/****************************************************************************
 *                                                                          *
 * Function: IsPaletteDevice                                                *
 *                                                                          *
 * Purpose : Return TRUE if the screen is palette based.                    *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

BOOL IsPaletteDevice(void)
{
    HDC hdc;
    int iRasterCaps;

    hdc = GetDC(NULL);
    iRasterCaps = GetDeviceCaps(hdc, RASTERCAPS);
    ReleaseDC(NULL, hdc);

    return (iRasterCaps & RC_PALETTE) ? TRUE : FALSE;
}

/****************************************************************************
 *                                                                          *
 * Function: FindDIBBits                                                    *
 *                                                                          *
 * Purpose : Return a pointer to the DIB pixels.                            *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

BYTE *FindDIBBits(BITMAPINFOHEADER *pbmih)
{
    return (BYTE *)pbmih + pbmih->biSize + GetDIBPaletteSize(pbmih);
}

/****************************************************************************
 *                                                                          *
 * Function: GetDIBPaletteSize                                              *
 *                                                                          *
 * Purpose : Return the size of the DIB palette.                            *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

int GetDIBPaletteSize(BITMAPINFOHEADER *pbmih)
{
    return GetDIBPaletteEntries(pbmih) * sizeof(RGBQUAD);
}

/****************************************************************************
 *                                                                          *
 * Function: GetDIBPaletteEntries                                           *
 *                                                                          *
 * Purpose : Return the number of colors in the DIB palette.                *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

int GetDIBPaletteEntries(BITMAPINFOHEADER *pbmih)
{
    if (pbmih->biClrUsed)
    {
        return (int)pbmih->biClrUsed;
    }

    switch (pbmih->biBitCount)
    {
        case 1:
            return 2;
        case 4:
            return 16;
        case 8:
            return 256;
        case 16:
        case 24:
            return 0;
        default:
            return -1;  // OK, and then...?!
    }
}

/****************************************************************************
 *                                                                          *
 * Function: InitInfo                                                       *
 *                                                                          *
 * Purpose : Initialize our silly little structure.                         *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

void InitInfo(PINFO pInfo)
{
    pInfo->hwnd = NULL;
    pInfo->hThrd = NULL;
    pInfo->fDestroy = FALSE;
}

/****************************************************************************
 *                                                                          *
 * Function: LoadStrings                                                    *
 *                                                                          *
 * Purpose : Initialize default strings from the resources.                 *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

void LoadStrings(void)
{
    LoadString(hMainInstance, IDS_DESCRIPTION, szName, TITLEBARNAMELEN);
    LoadString(hMainInstance, idsAppName, szAppName, APPNAMEBUFFERLEN);
    LoadString(hMainInstance, idsIniFile, szIniFile, MAXFILELEN);
    LoadString(hMainInstance, idsScreenSaver, szScreenSaver, 22);
    LoadString(hMainInstance, idsHelpFile, szHelpFile, MAXFILELEN);
    LoadString(hMainInstance, idsNoHelpMemory, szNoHelpMemory, BUFFLEN);
}

