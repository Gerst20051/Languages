/****************************************************************************
 *                                                                          *
 * Filename: vertex.c                                                       *
 *                                                                          *
 * Purpose : DirectX sample for Pelles C for Windows.                       *
 *                                                                          *
 *           Original source code from: www.codesampler.com                 *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *           04-05-12  Corrected bug in message loop (WM_QUIT).             *
 *                                                                          *
 ****************************************************************************/

#define STRICT
#define WIN32_LEAN_AND_MEAN

#include <d3dx8.h>
#include "vertex.h"

/*
 ****************************************************************************
 * Globals.
 */
HWND g_hwnd = NULL;
LPDIRECT3D8 g_pD3D = NULL;
LPDIRECT3DDEVICE8 g_pd3dDevice = NULL;
LPDIRECT3DVERTEXBUFFER8 g_pVertexBuffer = NULL;
LPDIRECT3DTEXTURE8 g_pTexture = NULL;

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

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_TEX1)

/*
 ****************************************************************************
 * Prototypes.
 */
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
static void LoadTexture(void);
static void Init(void);
static void ShutDown(void);
static void Render(void);

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
    MSG msg;

    winClass.lpszClassName = "MY_WINDOWS_CLASS";
    winClass.cbSize = sizeof(WNDCLASSEX);
    winClass.style = CS_HREDRAW | CS_VREDRAW;
    winClass.lpfnWndProc = WindowProc;
    winClass.hInstance = hInstance;
    winClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDR_ICO_DIRECTX));
    winClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDR_ICO_DIRECTX));
    winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    winClass.lpszMenuName = NULL;
    winClass.cbClsExtra = 0;
    winClass.cbWndExtra = 0;

    if (!RegisterClassEx(&winClass))
        return 1;

    g_hwnd = CreateWindowEx(0, "MY_WINDOWS_CLASS", "DirectX Sample for Pelles C for Windows",
        WS_OVERLAPPEDWINDOW|WS_VISIBLE, 0, 0, 640, 480, NULL, NULL, hInstance, NULL);
    if (g_hwnd == NULL)
        return 1;

    ShowWindow(g_hwnd, nCmdShow);
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

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_KEYDOWN:
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                    break;
            }
            break;

        case WM_CLOSE:
            PostQuitMessage(0);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
            break;
    }

    return 0;
}

/****************************************************************************
 *                                                                          *
 * Function: LoadTexture                                                    *
 *                                                                          *
 * Purpose : Load texture from file.                                        *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void LoadTexture(void)
{
    D3DXCreateTextureFromFile(g_pd3dDevice, "logo.bmp", &g_pTexture);

    IDirect3DDevice8_SetTextureStageState(g_pd3dDevice, 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
    IDirect3DDevice8_SetTextureStageState(g_pd3dDevice, 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
}

/****************************************************************************
 *                                                                          *
 * Function: Init                                                           *
 *                                                                          *
 * Purpose : Initialize the application.                                    *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void Init(void)
{
    g_pD3D = Direct3DCreate8(D3D_SDK_VERSION);

    D3DDISPLAYMODE d3ddm;

    IDirect3D8_GetAdapterDisplayMode(g_pD3D, D3DADAPTER_DEFAULT, &d3ddm);

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));

    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = d3ddm.Format;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    IDirect3D8_CreateDevice(g_pD3D, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hwnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice);

    LoadTexture();

    IDirect3DDevice8_CreateVertexBuffer(g_pd3dDevice, 24*sizeof(struct Vertex),
        0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pVertexBuffer);

    void *pVertices = NULL;

    IDirect3DVertexBuffer8_Lock(g_pVertexBuffer, 0, sizeof(g_cubeVertices), (BYTE**)&pVertices, 0);
    memcpy(pVertices, g_cubeVertices, sizeof(g_cubeVertices));
    IDirect3DVertexBuffer8_Unlock(g_pVertexBuffer);

    IDirect3DDevice8_SetRenderState(g_pd3dDevice, D3DRS_LIGHTING, FALSE);
    IDirect3DDevice8_SetRenderState(g_pd3dDevice, D3DRS_ZENABLE, TRUE);

    D3DXMATRIX matProj;
    D3DXMatrixPerspectiveFovLH(&matProj, 45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
    IDirect3DDevice8_SetTransform(g_pd3dDevice, D3DTS_PROJECTION, &matProj);
}

/****************************************************************************
 *                                                                          *
 * Function: ShutDown                                                       *
 *                                                                          *
 * Purpose : Shut down the application.                                     *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void ShutDown(void)
{
    if (g_pTexture != NULL)
        IDirect3DTexture8_Release(g_pTexture);

    if (g_pVertexBuffer != NULL)
        IDirect3DVertexBuffer8_Release(g_pVertexBuffer);

    if (g_pd3dDevice != NULL)
        IDirect3DDevice8_Release(g_pd3dDevice);

    if (g_pD3D != NULL)
        IDirect3D8_Release(g_pD3D);
}

/****************************************************************************
 *                                                                          *
 * Function: Render                                                         *
 *                                                                          *
 * Purpose : Render the sample.                                             *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void Render(void)
{
    IDirect3DDevice8_Clear(g_pd3dDevice, 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
        D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,1.0f), 1.0f, 0);

    static float fXrot = 0.0f;
    static float fYrot = 0.0f;
    static float fZrot = 0.0f;

    fXrot += 0.1f;
    fYrot += 0.2f;
    fZrot += 0.3f;

    D3DXMATRIX matWorld;
    D3DXMATRIX matTrans;
    D3DXMATRIX matRot;

    D3DXMatrixTranslation(&matTrans, 0.0f, 0.0f, 4.0f);

    D3DXMatrixRotationYawPitchRoll(&matRot, D3DXToRadian(fXrot), D3DXToRadian(fYrot), D3DXToRadian(fZrot));

    D3DXMatrixMultiply(&matWorld, &matRot, &matTrans);
    IDirect3DDevice8_SetTransform(g_pd3dDevice, D3DTS_WORLD, &matWorld);

    IDirect3DDevice8_BeginScene(g_pd3dDevice);

    IDirect3DDevice8_SetTexture(g_pd3dDevice, 0, (LPDIRECT3DBASETEXTURE8)g_pTexture);
    IDirect3DDevice8_SetStreamSource(g_pd3dDevice,0, g_pVertexBuffer, sizeof(struct Vertex));
    IDirect3DDevice8_SetVertexShader(g_pd3dDevice, D3DFVF_CUSTOMVERTEX);

    IDirect3DDevice8_DrawPrimitive(g_pd3dDevice, D3DPT_TRIANGLESTRIP,  0, 2);
    IDirect3DDevice8_DrawPrimitive(g_pd3dDevice, D3DPT_TRIANGLESTRIP,  4, 2);
    IDirect3DDevice8_DrawPrimitive(g_pd3dDevice, D3DPT_TRIANGLESTRIP,  8, 2);
    IDirect3DDevice8_DrawPrimitive(g_pd3dDevice, D3DPT_TRIANGLESTRIP, 12, 2);
    IDirect3DDevice8_DrawPrimitive(g_pd3dDevice, D3DPT_TRIANGLESTRIP, 16, 2);
    IDirect3DDevice8_DrawPrimitive(g_pd3dDevice, D3DPT_TRIANGLESTRIP, 20, 2);

    IDirect3DDevice8_EndScene(g_pd3dDevice);
    IDirect3DDevice8_Present(g_pd3dDevice,NULL, NULL, NULL, NULL);
}
