/****************************************************************************
 *                                                                          *
 * Filename: vertexce.c                                                     *
 *                                                                          *
 * Purpose : DirectX sample for Pelles C for Windows.                       *
 *                                                                          *
 *           Original source code from: www.codesampler.com                 *
 *           Port to Windows Mobile by Pelle Orinius.                       *
 *                                                                          *
 * History : Date          Reason                                           *
 *           06-08-26      Created                                          *
 *           Oct 06, 2006  Modified for Smartphone and Full Screen.         *
 *                         Tested to Work on Motorola Q Windows Mobile 5.0. *
 *                         Use center "Enter" button to exit.               *
 *                                                                          *
 ****************************************************************************/

/*
 * NOTE! The utility function library (D3DMX.DLL) *must* be installed on
 *       the device for this program to work. The file is located in
 *       the BIN folder, where Pelles C is installed. You may have to
 *       manually copy this DLL to your device.
 *
 *       (The utility function library contains math and texture support
 *       functions - see d3dmx.h header file).
 */
#ifndef WINCE_DEFAULT_LIBS
#define WINCE_DEFAULT_LIBS
#endif

#include <ceddraw.h>
#include <d3dm.h>
#include <d3dmx.h>
#include <mmsystem.h>
#include "vertexce.h"
//added aygshell.h for SHFullScreen
#include <aygshell.h>

#ifndef NDEBUG
#define CATCH(x) \
    do { \
        HRESULT hr = x; \
        if (FAILED(hr)) { \
            OutputDebugString(L"FAILED: " ## #x); \
            OutputDebugString(ErrorText(hr)); \
            exit(1); \
        } \
    } while (0)
#else
#define CATCH(x)  (x)
#endif

/*
 ****************************************************************************
 * Globals.
 */
HWND g_hwnd = NULL;
LPDIRECT3DMOBILE g_pD3DM = NULL;
LPDIRECT3DMOBILEDEVICE g_pd3dmDevice = NULL;
LPDIRECT3DMOBILEVERTEXBUFFER g_pVertexBuffer = NULL;
LPDIRECT3DMOBILETEXTURE g_pTexture = NULL;

struct Vertex {
    float x, y, z;
    float tu, tv;
};

struct Vertex g_cubeVertices[] = {
    {-1.0f, 1.0f,-1.0f,  0.0f,0.0f },
    { 1.0f, 1.0f,-1.0f,  1.0f,0.0f },
    {-1.0f,-1.0f,-1.0f,  0.0f,1.0f },
    { 1.0f,-1.0f,-1.0f,  1.0f,1.0f },

    {-1.0f, 1.0f, 1.0f,  1.0f,0.0f },
    {-1.0f,-1.0f, 1.0f,  1.0f,1.0f },
    { 1.0f, 1.0f, 1.0f,  0.0f,0.0f },
    { 1.0f,-1.0f, 1.0f,  0.0f,1.0f },

    {-1.0f, 1.0f, 1.0f,  0.0f,0.0f },
    { 1.0f, 1.0f, 1.0f,  1.0f,0.0f },
    {-1.0f, 1.0f,-1.0f,  0.0f,1.0f },
    { 1.0f, 1.0f,-1.0f,  1.0f,1.0f },

    {-1.0f,-1.0f, 1.0f,  0.0f,0.0f },
    {-1.0f,-1.0f,-1.0f,  0.0f,1.0f },
    { 1.0f,-1.0f, 1.0f,  1.0f,0.0f },
    { 1.0f,-1.0f,-1.0f,  1.0f,1.0f },

    { 1.0f, 1.0f,-1.0f,  0.0f,0.0f },
    { 1.0f, 1.0f, 1.0f,  1.0f,0.0f },
    { 1.0f,-1.0f,-1.0f,  0.0f,1.0f },
    { 1.0f,-1.0f, 1.0f,  1.0f,1.0f },

    {-1.0f, 1.0f,-1.0f,  1.0f,0.0f },
    {-1.0f,-1.0f,-1.0f,  1.0f,1.0f },
    {-1.0f, 1.0f, 1.0f,  0.0f,0.0f },
    {-1.0f,-1.0f, 1.0f,  0.0f,1.0f }
};

#define D3DMFVF_CUSTOMVERTEX  (D3DMFVF_XYZ_FLOAT|D3DMFVF_TEX1)

/*
 ****************************************************************************
 * Prototypes.
 */
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPWSTR, int);
static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
static BOOL IsScreenRotated(void);
static void Init(void);
static void Render(void);
static void ShutDown(void);
#ifndef NDEBUG
static const WCHAR const *ErrorText(HRESULT);
#endif /* NDEBUG */

/****************************************************************************
 *                                                                          *
 * Function: WinMain                                                        *
 *                                                                          *
 * Purpose : The application's entry point.                                 *
 *                                                                          *
 * History : Date      Reason                                               *
 *           06-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    WNDCLASS winClass;
    MSG msg;

    if (IsScreenRotated())
        return 1;

    winClass.style = CS_HREDRAW|CS_VREDRAW;
    winClass.lpfnWndProc = WindowProc;
    winClass.cbClsExtra = 0;
    winClass.cbWndExtra = 0;
    winClass.hInstance = hInstance;
    winClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDR_ICO_DIRECTX));
    winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    winClass.lpszMenuName = NULL;
    winClass.lpszClassName = L"MY_WINDOWS_CLASS";

    if (!RegisterClass(&winClass))
        return 1;

//   g_hwnd = CreateWindowEx(0, L"MY_WINDOWS_CLASS", L"D3DM Sample",
//        WS_OVERLAPPEDWINDOW|WS_VISIBLE, 0, 0, 320, 240, NULL, NULL, hInstance, NULL);
//    if (g_hwnd == NULL)
//        return 1;

// removed WS_OVERLAPPEDWINDOW

    g_hwnd = CreateWindowEx(0, L"MY_WINDOWS_CLASS", L"D3DM Sample",
        WS_VISIBLE, 0, 0, 320, 240, NULL, NULL, hInstance, NULL);
    if (g_hwnd == NULL)
        return 1;


	//Added code for full screen
	SHFullScreen(g_hwnd, SHFS_HIDETASKBAR | SHFS_HIDESTARTICON | SHFS_HIDESIPBUTTON);
	ShowWindow(g_hwnd, SW_MAXIMIZE); 


//    ShowWindow(g_hwnd, nCmdShow);
    UpdateWindow(g_hwnd);

    Init();

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
            Render();
        }
    }

    ShutDown();

    UnregisterClass(L"MY_WINDOWS_CLASS", winClass.hInstance);

    return msg.wParam;
}

/****************************************************************************
 *                                                                          *
 * Function: WindowProc                                                     *
 *                                                                          *
 * Purpose : The window's message handler.                                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           06-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

#define TID_SINGALONG  100
static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_CREATE:
            SetTimer(hwnd, TID_SINGALONG, 3000, NULL);
            break;

        case WM_TIMER:
            KillTimer(hwnd, TID_SINGALONG);
            PlaySound(MAKEINTRESOURCE(IDR_WAV_SINGALONG), GetModuleHandle(NULL), SND_ASYNC|SND_RESOURCE);
            break;

        case WM_LBUTTONUP: 
            PostMessage(hwnd, WM_CLOSE, 0, 0);
            break;
// added return key to quit for smartphone
        case WM_KEYDOWN:
            if (wParam == VK_RETURN)
                PostMessage(hwnd, WM_CLOSE, 0, 0);
            break;

        case WM_SETTINGCHANGE:
            if (IsScreenRotated())
                PostMessage(hwnd, WM_CLOSE, 0, 0);
            break;

        case WM_CLOSE:
            PostQuitMessage(0);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}
#undef TID_SINGALONG

/****************************************************************************
 *                                                                          *
 * Function: IsScreenRotated                                                *
 *                                                                          *
 * Purpose : Currently the D3DM runtime does not support a rotated screen.  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           06-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

static BOOL IsScreenRotated(void)
{
    DEVMODE dm = { .dmSize = sizeof(DEVMODE), .dmFields = DM_DISPLAYORIENTATION };

    ChangeDisplaySettingsEx(NULL, &dm, NULL, CDS_TEST, NULL);

    if (dm.dmDisplayOrientation != DMDO_0)
    {
        MessageBox(NULL, L"D3DM does not work on a rotated screen.", L"Bummer!",
            MB_OK|MB_ICONEXCLAMATION|MB_SETFOREGROUND);
        return TRUE;
    }

    return FALSE;
}

/****************************************************************************
 *                                                                          *
 * Function: Init                                                           *
 *                                                                          *
 * Purpose : Initialize the application.                                    *
 *                                                                          *
 * History : Date      Reason                                               *
 *           06-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void Init(void)
{
    if ((g_pD3DM = Direct3DMobileCreate(D3DM_SDK_VERSION)) == NULL)
    {
        OutputDebugString(L"Direct3DMobileCreate failed!");
        exit(1);
    }

    D3DMPRESENT_PARAMETERS d3dmpp = {
        .Windowed = TRUE,
        .SwapEffect = D3DMSWAPEFFECT_DISCARD,
        .BackBufferFormat = D3DMFMT_UNKNOWN
    };

    CATCH(IDirect3DMobile_CreateDevice(g_pD3DM, D3DMADAPTER_DEFAULT, D3DMDEVTYPE_DEFAULT, g_hwnd, 0, &d3dmpp, &g_pd3dmDevice));

    if (FAILED(D3DMXCreateTextureFromResourceEx(g_pd3dmDevice, GetModuleHandle(NULL),
            MAKEINTRESOURCE(IDR_BMP_LOGO), D3DMX_DEFAULT, D3DMX_DEFAULT, 1, 0, D3DMFMT_UNKNOWN,
            D3DMPOOL_SYSTEMMEM, D3DMX_FILTER_POINT, D3DMX_FILTER_POINT, 0, NULL, NULL, &g_pTexture)))
    {
        OutputDebugString(L"CreateTextureFromResource failed!");
        exit(1);
    }

    CATCH(IDirect3DMobileDevice_CreateVertexBuffer(g_pd3dmDevice, sizeof(g_cubeVertices),
        0, D3DMFVF_CUSTOMVERTEX, D3DMPOOL_SYSTEMMEM, &g_pVertexBuffer));

    void *pVertices = NULL;

    CATCH(IDirect3DMobileVertexBuffer_Lock(g_pVertexBuffer, 0, sizeof(g_cubeVertices), &pVertices, 0));
    memcpy(pVertices, g_cubeVertices, sizeof(g_cubeVertices));
    CATCH(IDirect3DMobileVertexBuffer_Unlock(g_pVertexBuffer));

    CATCH(IDirect3DMobileDevice_SetRenderState(g_pd3dmDevice, D3DMRS_LIGHTING, FALSE));
    CATCH(IDirect3DMobileDevice_SetRenderState(g_pd3dmDevice, D3DMRS_ZENABLE, D3DMZB_TRUE));

    D3DMXMATRIX matProj;
    D3DMXMatrixPerspectiveFovLH(&matProj, 45.0f, 240.0f / 320.0f, 0.1f, 100.0f);
    CATCH(IDirect3DMobileDevice_SetTransform(g_pd3dmDevice, D3DMTS_PROJECTION, (D3DMMATRIX *)&matProj, D3DMFMT_D3DMVALUE_FLOAT));
}

/****************************************************************************
 *                                                                          *
 * Function: Render                                                         *
 *                                                                          *
 * Purpose : Render the sample.                                             *
 *                                                                          *
 * History : Date      Reason                                               *
 *           06-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void Render(void)
{
    CATCH(IDirect3DMobileDevice_Clear(g_pd3dmDevice, 0, NULL,
        D3DMCLEAR_TARGET|D3DMCLEAR_ZBUFFER, D3DMCOLOR_XRGB(0,0,255), 1.0f, 0));

    static float fXrot = 0.0f;
    static float fYrot = 0.0f;
    static float fZrot = 0.0f;

    fXrot += 0.1f;
    fYrot += 0.2f;
    fZrot += 0.3f;

    D3DMXMATRIX matWorld;
    D3DMXMATRIX matTrans;
    D3DMXMATRIX matRot;

    D3DMXMatrixTranslation(&matTrans, 0.0f, 0.0f, 4.0f);
    D3DMXMatrixRotationYawPitchRoll(&matRot, D3DMXToRadian(fXrot), D3DMXToRadian(fYrot), D3DMXToRadian(fZrot));
    D3DMXMatrixMultiply(&matWorld, &matRot, &matTrans);

    CATCH(IDirect3DMobileDevice_SetTransform(g_pd3dmDevice, D3DMTS_WORLD, (D3DMMATRIX *)&matWorld, D3DMFMT_D3DMVALUE_FLOAT));

    CATCH(IDirect3DMobileDevice_BeginScene(g_pd3dmDevice));

    CATCH(IDirect3DMobileDevice_SetTexture(g_pd3dmDevice, 0, (LPDIRECT3DMOBILEBASETEXTURE)g_pTexture));
    CATCH(IDirect3DMobileDevice_SetStreamSource(g_pd3dmDevice, 0, g_pVertexBuffer, sizeof(struct Vertex)));

    CATCH(IDirect3DMobileDevice_DrawPrimitive(g_pd3dmDevice, D3DMPT_TRIANGLESTRIP,  0, 2));
    CATCH(IDirect3DMobileDevice_DrawPrimitive(g_pd3dmDevice, D3DMPT_TRIANGLESTRIP,  4, 2));
    CATCH(IDirect3DMobileDevice_DrawPrimitive(g_pd3dmDevice, D3DMPT_TRIANGLESTRIP,  8, 2));
    CATCH(IDirect3DMobileDevice_DrawPrimitive(g_pd3dmDevice, D3DMPT_TRIANGLESTRIP, 12, 2));
    CATCH(IDirect3DMobileDevice_DrawPrimitive(g_pd3dmDevice, D3DMPT_TRIANGLESTRIP, 16, 2));
    CATCH(IDirect3DMobileDevice_DrawPrimitive(g_pd3dmDevice, D3DMPT_TRIANGLESTRIP, 20, 2));

    CATCH(IDirect3DMobileDevice_EndScene(g_pd3dmDevice));
    CATCH(IDirect3DMobileDevice_Present(g_pd3dmDevice, NULL, NULL, NULL, NULL));
}

/****************************************************************************
 *                                                                          *
 * Function: ShutDown                                                       *
 *                                                                          *
 * Purpose : Shut down the application.                                     *
 *                                                                          *
 * History : Date      Reason                                               *
 *           06-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void ShutDown(void)
{
    if (g_pTexture != NULL)
        IDirect3DMobileTexture_Release(g_pTexture);

    if (g_pVertexBuffer != NULL)
        IDirect3DMobileVertexBuffer_Release(g_pVertexBuffer);

    if (g_pd3dmDevice != NULL)
        IDirect3DMobileDevice_Release(g_pd3dmDevice);

    if (g_pD3DM != NULL)
        IDirect3DMobile_Release(g_pD3DM);
}

/****************************************************************************
 *                                                                          *
 * Function: ErrorText                                                      *
 *                                                                          *
 * Purpose : Map D3DM errors (HRESULT) to readable text.                    *
 *                                                                          *
 * History : Date      Reason                                               *
 *           06-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

#ifndef NDEBUG
static const WCHAR const *ErrorText(HRESULT hr)
{
    switch (hr)
    {
        case D3DMERR_DRIVERINTERNALERROR: return L"D3DMERR: Driver internal error";
        case D3DMERR_INVALIDDEVICE: return L"D3DMERR: Invalid device";
        case D3DMERR_INVALIDCALL: return L"D3DMERR: Invalid call";
        case D3DMERR_DRIVERINVALIDCALL: return L"D3DMERR: Driver invalid call";
        case D3DMERR_DRIVERUNSUPPORTED: return L"D3DMERR: Driver unsupported";
        case D3DMERR_WRONGTEXTUREFORMAT: return L"D3DMERR: Wrong texture format";
        case D3DMERR_UNSUPPORTEDCOLOROPERATION: return L"D3DMERR: Unsupported color operation";
        case D3DMERR_UNSUPPORTEDCOLORARG: return L"D3DMERR: Unsupported color arg";
        case D3DMERR_UNSUPPORTEDALPHAOPERATION: return L"D3DMERR: Unsupported alpha operation";
        case D3DMERR_UNSUPPORTEDALPHAARG: return L"D3DMERR: Unsupported alpha arg";
        case D3DMERR_TOOMANYOPERATIONS: return L"D3DMERR: To many operations";
        case D3DMERR_CONFLICTINGTEXTUREFILTER: return L"D3DMERR: Conflicting texture filter";
        case D3DMERR_UNSUPPORTEDFACTORVALUE: return L"D3DMERR: Unsupported factor value";
        case D3DMERR_CONFLICTINGRENDERSTATE: return L"D3DMERR: Conflicting render state";
        case D3DMERR_UNSUPPORTEDTEXTUREFILTER: return L"D3DMERR: Unsupported texture filter";
        case D3DMERR_CONFLICTINGTEXTUREPALETTE: return L"D3DMERR: Conflicting texture palette";
        case D3DMERR_NOTFOUND: return L"D3DMERR: Not found";
        case D3DMERR_MOREDATA: return L"D3DMERR: More data";
        case D3DMERR_DEVICELOST: return L"D3DMERR: Device lost (in space?!)";
        case D3DMERR_DEVICENOTRESET: return L"D3DMERR: Device not reset";
        case D3DMERR_NOTAVAILABLE: return L"D3DMERR: Not available";
        case D3DMERR_MEMORYPOOLEMPTY: return L"D3DMERR: Memory pool empty";
        case D3DMERR_ALREADYLOCKED: return L"D3DMERR: Already locked";
        default: return L"D3DMERR: unknown... :-(";
    }
}
#endif /* NDEBUG */
