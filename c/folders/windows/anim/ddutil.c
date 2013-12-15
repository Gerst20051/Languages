/****************************************************************************
 *                                                                          *
 * Filename: ddutil.c                                                       *
 *                                                                          *
 * Purpose : Routines for loading bitmap and palettes from resources.       *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include "ddutil.h"

/****************************************************************************
 *                                                                          *
 * Function: DDLoadBitmap                                                   *
 *                                                                          *
 * Purpose : Create a DirectDrawSurface from a bitmap resource.             *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

extern IDirectDrawSurface7 *DDLoadBitmap(IDirectDraw7 *pdd, LPCSTR szBitmap, int dx, int dy)
{
    HBITMAP hbm;
    BITMAP bm;
    DDSURFACEDESC2 ddsd;
    IDirectDrawSurface7 *pdds;

    //
    //  Try to load the bitmap as a resource, if that fails, try it as a file.
    //
    hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), szBitmap, IMAGE_BITMAP, dx, dy, LR_CREATEDIBSECTION);
    if (hbm == NULL)
        hbm = (HBITMAP)LoadImage(NULL, szBitmap, IMAGE_BITMAP, dx, dy, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
    if (hbm == NULL)
        return NULL;

    //
    // Get size of the bitmap.
    //
    GetObject(hbm, sizeof(bm), &bm);

    //
    // Create a DirectDrawSurface for this bitmap.
    //
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwWidth = bm.bmWidth;
    ddsd.dwHeight = bm.bmHeight;
    if (pdd->lpVtbl->CreateSurface(pdd, &ddsd, &pdds, NULL) != DD_OK)
        return NULL;

    DDCopyBitmap(pdds, hbm, 0, 0, 0, 0);
    DeleteObject(hbm);
    return pdds;
}

/****************************************************************************
 *                                                                          *
 * Function: DDReLoadBitmap                                                 *
 *                                                                          *
 * Purpose : Load a bitmap from a file or resource into a directdraw        *
 *           surface. Normally used to re-load a surface after a restore.   *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

HRESULT DDReLoadBitmap(IDirectDrawSurface7 *pdds, LPCSTR szBitmap)
{
    HBITMAP hbm;
    HRESULT hr;

    //
    //  Try to load the bitmap as a resource, if that fails, try it as a file.
    //
    hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), szBitmap, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
    if (hbm == NULL)
        hbm = (HBITMAP)LoadImage(NULL, szBitmap, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
    if (hbm == NULL)
    {
        OutputDebugString("handle is null\n");
        return E_FAIL;
    }
    hr = DDCopyBitmap(pdds, hbm, 0, 0, 0, 0);
    if (hr != DD_OK)
    {
        OutputDebugString("ddcopybitmap failed\n");
    }
    DeleteObject(hbm);
    return hr;
}

/****************************************************************************
 *                                                                          *
 * Function: DDCopyBitmap                                                   *
 *                                                                          *
 * Purpose : Draw a bitmap into a DirectDrawSurface.                        *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

extern HRESULT DDCopyBitmap(IDirectDrawSurface7 * pdds, HBITMAP hbm, int x, int y, int dx, int dy)
{
    HDC hdcImage;
    HDC hdc;
    BITMAP bm;
    DDSURFACEDESC2 ddsd;
    HRESULT hr;

    if (hbm == NULL || pdds == NULL)
        return E_FAIL;

    //
    // Make sure this surface is restored.
    //
    pdds->lpVtbl->Restore(pdds);

    //
    // Select bitmap into a memoryDC so we can use it.
    //
    hdcImage = CreateCompatibleDC(NULL);
    if (!hdcImage)
        OutputDebugString("createcompatible dc failed\n");
    SelectObject(hdcImage, hbm);

    //
    // Get size of the bitmap
    //
    GetObject(hbm, sizeof(bm), &bm);
    dx = (dx == 0) ? bm.bmWidth : dx;
    dy = (dy == 0) ? bm.bmHeight : dy;

    //
    // Get size of surface.
    //
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_HEIGHT|DDSD_WIDTH;
    pdds->lpVtbl->GetSurfaceDesc(pdds, &ddsd);

    if ((hr = pdds->lpVtbl->GetDC(pdds, &hdc)) == DD_OK)
    {
        StretchBlt(hdc, 0, 0, ddsd.dwWidth, ddsd.dwHeight, hdcImage, x, y, dx, dy, SRCCOPY);
        pdds->lpVtbl->ReleaseDC(pdds, hdc);
    }
    DeleteDC(hdcImage);
    return hr;
}

/****************************************************************************
 *                                                                          *
 * Function: DDLoadPalette                                                  *
 *                                                                          *
 * Purpose : Create a DirectDraw palette object from a bitmap resource.     *
 *           If the resource does not exist, or NULL is passed, create a    *
 *           default 332 palette.                                           *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

extern IDirectDrawPalette *DDLoadPalette(IDirectDraw7 *pdd, LPCSTR szBitmap)
{
    IDirectDrawPalette *ddpal;
    int i;
    int n;
    int fh;
    HRSRC h;
    LPBITMAPINFOHEADER lpbi;
    PALETTEENTRY ape[256];
    RGBQUAD *prgb;

    //
    // Build a 332 palette as the default.
    //
    for (i = 0; i < 256; i++)
    {
        ape[i].peRed = (BYTE) (((i >> 5) & 0x07) * 255 / 7);
        ape[i].peGreen = (BYTE) (((i >> 2) & 0x07) * 255 / 7);
        ape[i].peBlue = (BYTE) (((i >> 0) & 0x03) * 255 / 3);
        ape[i].peFlags = (BYTE) 0;
    }

    //
    // Get a pointer to the bitmap resource.
    //
    if (szBitmap && (h = FindResource(NULL, szBitmap, RT_BITMAP)))
    {
        lpbi = (LPBITMAPINFOHEADER) LockResource(LoadResource(NULL, h));
        if (!lpbi)
            OutputDebugString("lock resource failed\n");
        prgb = (RGBQUAD *)((BYTE *)lpbi + lpbi->biSize);
        if (lpbi == NULL || lpbi->biSize < sizeof(BITMAPINFOHEADER))
            n = 0;
        else if (lpbi->biBitCount > 8)
            n = 0;
        else if (lpbi->biClrUsed == 0)
            n = 1 << lpbi->biBitCount;
        else
            n = lpbi->biClrUsed;
        //
        //  A DIB color table has its colors stored BGR not RGB
        //  so flip them around.
        //
        for (i = 0; i < n; i++)
        {
            ape[i].peRed = prgb[i].rgbRed;
            ape[i].peGreen = prgb[i].rgbGreen;
            ape[i].peBlue = prgb[i].rgbBlue;
            ape[i].peFlags = 0;
        }
    }
    else if (szBitmap && (fh = _lopen(szBitmap, OF_READ)) != -1)
    {
        BITMAPFILEHEADER bf;
        BITMAPINFOHEADER bi;

        _lread(fh, &bf, sizeof(bf));
        _lread(fh, &bi, sizeof(bi));
        _lread(fh, ape, sizeof(ape));
        _lclose(fh);

        if (bi.biSize != sizeof(BITMAPINFOHEADER))
            n = 0;
        else if (bi.biBitCount > 8)
            n = 0;
        else if (bi.biClrUsed == 0)
            n = 1 << bi.biBitCount;
        else
            n = bi.biClrUsed;
        //
        //  A DIB color table has its colors stored BGR not RGB
        //  so flip them around.
        //
        for (i = 0; i < n; i++)
        {
            BYTE r = ape[i].peRed;

            ape[i].peRed = ape[i].peBlue;
            ape[i].peBlue = r;
        }
    }
    pdd->lpVtbl->CreatePalette(pdd, DDPCAPS_8BIT, ape, &ddpal, NULL);
    return ddpal;
}

/****************************************************************************
 *                                                                          *
 * Function: DDColorMatch                                                   *
 *                                                                          *
 * Purpose : Convert a RGB color to a pysical color. We do this by letting  *
 *           GDI SetPixel() do the color matching, then we lock the memory  *
 *           and see what it got mapped to.                                 *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

extern DWORD DDColorMatch(IDirectDrawSurface7 *pdds, COLORREF rgb)
{
    COLORREF rgbT;
    HDC hdc;
    DWORD dw = CLR_INVALID;
    DDSURFACEDESC2 ddsd;
    HRESULT hres;

    //
    //  Use GDI SetPixel to color match for us
    //
    if (rgb != CLR_INVALID && pdds->lpVtbl->GetDC(pdds, &hdc) == DD_OK)
    {
        rgbT = GetPixel(hdc, 0, 0);     // Save current pixel value
        SetPixel(hdc, 0, 0, rgb);       // Set our value
        pdds->lpVtbl->ReleaseDC(pdds, hdc);
    }
    //
    // Now lock the surface so we can read back the converted color
    //
    ddsd.dwSize = sizeof(ddsd);
    while ((hres = pdds->lpVtbl->Lock(pdds, NULL, &ddsd, 0, NULL)) == DDERR_WASSTILLDRAWING)
        ;
    if (hres == DD_OK)
    {
        dw = *(DWORD *)ddsd.lpSurface;                 // Get DWORD
        if (ddsd.ddpfPixelFormat.dwRGBBitCount < 32)
            dw &= (1 << ddsd.ddpfPixelFormat.dwRGBBitCount) - 1;  // Mask it to bpp
        pdds->lpVtbl->Unlock(pdds, NULL);
    }
    //
    //  Now put the color that was there back.
    //
    if (rgb != CLR_INVALID && pdds->lpVtbl->GetDC(pdds, &hdc) == DD_OK)
    {
        SetPixel(hdc, 0, 0, rgbT);
        pdds->lpVtbl->ReleaseDC(pdds, hdc);
    }
    return dw;
}

/****************************************************************************
 *                                                                          *
 * Function: DDSetColorKey                                                  *
 *                                                                          *
 * Purpose : Set a color key for a surface, given a RGB. If you pass        *
 *           CLR_INVALID as the color key, the pixel in the upper-left      *
 *           corner will be used.                                           *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

extern HRESULT DDSetColorKey(IDirectDrawSurface7 *pdds, COLORREF rgb)
{
    DDCOLORKEY ddck;

    ddck.dwColorSpaceLowValue = DDColorMatch(pdds, rgb);
    ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
    return pdds->lpVtbl->SetColorKey(pdds, DDCKEY_SRCBLT, &ddck);
}

