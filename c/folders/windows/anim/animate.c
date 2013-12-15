/****************************************************************************
 *                                                                          *
 * Filename: animate.c                                                      *
 *                                                                          *
 * Purpose : DirectX sample for Pelles C for Windows.                       *
 *                                                                          *
 *           Original (?) source code from: www.codesampler.com             *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *           04-05-12  Corrected bug in message loop (WM_QUIT).             *
 *                                                                          *
 ****************************************************************************/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ddraw.h>
#include "ddutil.h"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "ddraw.lib")

/*
 ****************************************************************************
 * Globals.
 */
LPDIRECTDRAW7 g_lpdd7;                  // DirectDraw object
LPDIRECTDRAWSURFACE7 g_lpddsPrimary;    // DirectDraw primary surface
LPDIRECTDRAWSURFACE7 g_lpddsBack;       // DirectDraw back buffer surface
LPDIRECTDRAWSURFACE7 g_lpddsDonut;      // DirectDraw off-screen surface for the donut bitmap.

/*
 ****************************************************************************
 * Symbolic constants.
 */
const int FRAME_DELAY = 15;             // Frame delay or animation speed.
const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;
const int COLOR_DEPTH = 8;

/*
 ****************************************************************************
 * Prototypes.
 */
static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
static HRESULT init(HWND);
static HRESULT render(void);
static void shutdown(void);

/****************************************************************************
 *                                                                          *
 * Function: WinMain                                                        *
 *                                                                          *
 * Purpose : The application's entry point.                                 *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX winClass;
    HWND hwnd;
    MSG msg;

    winClass.lpszClassName = "MY_WINDOWS_CLASS";
    winClass.cbSize = sizeof(WNDCLASSEX);
    winClass.style = CS_DBLCLKS|CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
    winClass.lpfnWndProc = WindowProc;
    winClass.hInstance = hInstance;
    winClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    winClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    winClass.lpszMenuName = NULL;
    winClass.cbClsExtra = 0;
    winClass.cbWndExtra = 0;

    if (!RegisterClassEx(&winClass))
        return 0;

    hwnd = CreateWindowEx(0, "MY_WINDOWS_CLASS", "Sprite Animation",
        WS_POPUP|WS_VISIBLE, 0, 0, 640, 480, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
        return 1;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    init(hwnd);

    for (;;)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            render();
        }
    }

    shutdown();

    UnregisterClass("MY_WINDOWS_CLASS", winClass.hInstance);

    return msg.wParam;
}

/****************************************************************************
 *                                                                          *
 * Function: WindowProc                                                     *
 *                                                                          *
 * Purpose : The window's message handler.                                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_KEYDOWN:
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                    break;
            }
            break;

        case WM_SETCURSOR:
            SetCursor(NULL);
            return 1;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
            break;
    }

    return 0;
}

/****************************************************************************
 *                                                                          *
 * Function: init                                                           *
 *                                                                          *
 * Purpose : Initializes everything.                                        *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

static HRESULT init(HWND hwnd)
{
    DDSURFACEDESC2 ddsd;                // DirectDraw surface description structure
    DDSCAPS2 ddscaps;                   // DirectDraw surface capabilities structure.
    LPDIRECTDRAWPALETTE lpddPalette;    // DirectDraw palette.

    // Create the main DirectDraw object
    DirectDrawCreateEx(NULL, (LPVOID *)&g_lpdd7, &IID_IDirectDraw7, NULL);

    // Set our cooperative level
    g_lpdd7->lpVtbl->SetCooperativeLevel(g_lpdd7, hwnd, DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN);

    // Set the display mode.
    g_lpdd7->lpVtbl->SetDisplayMode(g_lpdd7, SCREEN_WIDTH, SCREEN_HEIGHT, COLOR_DEPTH, 0, 0);

    // Create the primary surface with one back buffer.
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(DDSURFACEDESC2);
    ddsd.dwFlags = DDSD_CAPS|DDSD_BACKBUFFERCOUNT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE|DDSCAPS_FLIP|DDSCAPS_COMPLEX|DDSCAPS_VIDEOMEMORY;
    ddsd.dwBackBufferCount = 1;
    ddsd.dwWidth = SCREEN_WIDTH;
    ddsd.dwHeight = SCREEN_HEIGHT;

    g_lpdd7->lpVtbl->CreateSurface(g_lpdd7, &ddsd, &g_lpddsPrimary, NULL);

    // Get a pointer to the back buffer.
    ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
    ddscaps.dwCaps2 = 0;
    ddscaps.dwCaps3 = 0;
    ddscaps.dwCaps4 = 0;
    g_lpddsPrimary->lpVtbl->GetAttachedSurface(g_lpddsPrimary, &ddscaps, &g_lpddsBack);

    // Create and set the palette
    lpddPalette = DDLoadPalette(g_lpdd7, "donut.bmp");
    g_lpddsPrimary->lpVtbl->SetPalette(g_lpddsPrimary, lpddPalette);

    // Create a surface and load the bitmap containing our sprite's frames into it.
    g_lpddsDonut = DDLoadBitmap(g_lpdd7, "donut.bmp", 0, 0);

    // Use CLR_INVALID to set the color key for this bitmap
    // by using the color of the pixel located at X=0,Y=0.
    // In most cases, this works fine.
    DDSetColorKey(g_lpddsDonut, CLR_INVALID);

    // If you want to set it manualy, pass in the RGB color value
    // of the background pixels you want to drop during the blit
    // like this...
    //DDSetColorKey(lpDDSDonut, RGB(0,0,0)); // RGB(0,0,0) = black

    return S_OK;
}

/****************************************************************************
 *                                                                          *
 * Function: render                                                         *
 *                                                                          *
 * Purpose : Draw all 3D Objects to the screen...                           *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

static HRESULT render(void)
{
    DDBLTFX ddbltfx;
    RECT rcFrame;
    RECT rcDest;
    double dTickCount;

    static double dLastTickCount;   // Last frame time.
    static int nFrame;              // Current sprite frame.
    static int nPosition = 0;       // Current position of sprite.

    dTickCount = GetTickCount();

    if ((dTickCount - dLastTickCount) <= FRAME_DELAY)
        return S_OK;

    dLastTickCount = dTickCount;

    // Clear the back buffer to blue using the blitter.
    ddbltfx.dwSize = sizeof(ddbltfx);
    ddbltfx.dwFillColor = 0;
    g_lpddsBack->lpVtbl->Blt(g_lpddsBack, NULL, NULL, NULL, DDBLT_COLORFILL|DDBLT_WAIT, &ddbltfx);

    // Calculate which donut frame to use next from the bitmap!
    rcFrame.top = ((nFrame / 5) * 64);
    rcFrame.left = ((nFrame % 5) * 64);
    rcFrame.bottom = rcFrame.top + 64;
    rcFrame.right = rcFrame.left + 64;

    ++nFrame;

    if (nFrame > 29)
        nFrame = 0;

    // Calculate the donuts new position as it moves across the screen
    nPosition += 1;

    if (nPosition > 640)
        nPosition = 0;

    rcDest.left = nPosition;
    rcDest.top = 200;
    rcDest.bottom = rcDest.top  + 64;
    rcDest.right = rcDest.left + 64;

    // Blit the donut's next frame from the off-screen surface called, lpDDSDonut
    // to the back buffer surface called, lpDDSBack!
    g_lpddsBack->lpVtbl->Blt(g_lpddsBack, &rcDest, g_lpddsDonut, &rcFrame, DDBLT_WAIT|DDBLT_KEYSRC, NULL);

    // Finally, perform the flip.
    g_lpddsPrimary->lpVtbl->Flip(g_lpddsPrimary, NULL, DDFLIP_WAIT);

    return S_OK;
}

/****************************************************************************
 *                                                                          *
 * Function: shutdown                                                       *
 *                                                                          *
 * Purpose : Release and cleanup everything.                                *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void shutdown(void)
{
    if (g_lpddsDonut)
    {
        g_lpddsDonut->lpVtbl->Release;
        g_lpddsDonut = NULL;
    }

    if (g_lpddsPrimary)
    {
        g_lpddsPrimary->lpVtbl->Release;
        g_lpddsPrimary = NULL;
    }

    if (g_lpdd7)
    {
        g_lpdd7->lpVtbl->Release;
        g_lpdd7 = NULL;
    }
}
