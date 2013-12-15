/////////////////////////////////////////////////////////////////////////////
//  Microsoft's DirectDraw Demo 'Wormhole'
//  Simple conversion from C++ to Pelles C   by Athlor
/////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <ddraw.h>

static HWND hWnd;
static PALETTEENTRY pe[256];
static BOOL bActive;
static BOOL bIsInitialized;

static LPDIRECTDRAW lpDD;
static LPDIRECTDRAWSURFACE lpDDSPrimary;
static LPDIRECTDRAWSURFACE lpDDSOne;
static LPDIRECTDRAWPALETTE lpDDPal;

int     WINAPI WinMain (HINSTANCE, HINSTANCE, LPSTR, int);
int     RestoreAll (void);
void    UpdateFrame (void);
void    FiniObjects (void);
LRESULT CALLBACK WindowProc (HWND, UINT, WPARAM, LPARAM);
int     initFail (HWND);
int     doInit (HINSTANCE, int);
void    CyclePalette (void);
int     ReadBMPIntoSurfaces (void);

/////////////////////////////////////////////////////////////////////////////
int WINAPI WinMain (HINSTANCE hInst, HINSTANCE hPrev, LPSTR CmdLine,
                                                                 int CmdShow)
{
  MSG Msg;
  memset(&Msg,0,sizeof(Msg));
  if(!doInit(hInst,CmdShow))
  {
    return 0;
  }
  ReadBMPIntoSurfaces();
  UpdateFrame();
  while((1))
    {
      if(PeekMessage(&Msg,NULL,0,0,PM_NOREMOVE))
        {
          if(!GetMessage(&Msg,NULL,0,0))
            {
              return Msg.wParam;
            }
          TranslateMessage(&Msg);
          DispatchMessage(&Msg);
        }
        else if(bActive)
        {
          CyclePalette();
        }
        else
        {
          WaitMessage();
        }
    }
  return TRUE;
} 


int RestoreAll (void)
{
  BOOL  bResult;
  bResult=IDirectDrawSurface_Restore(lpDDSPrimary) == DD_OK &&
                               IDirectDrawSurface_Restore(lpDDSOne) == DD_OK;
  ReadBMPIntoSurfaces();
  return bResult;
}

void UpdateFrame (void)
{
  RECT  rcRect;
  RECT  destRect;
  HRESULT  ddrval;
  POINT  pt;
  rcRect.left=0;
  rcRect.top=0;
  rcRect.right=640;
  rcRect.bottom=480;
  GetClientRect(hWnd,&destRect);
  pt.x=pt.y=0;
  ClientToScreen(hWnd,&pt);
  OffsetRect(&destRect,pt.x,pt.y);
  while(1)
  {
    ddrval=IDirectDrawSurface_Blt(lpDDSPrimary,&destRect,lpDDSOne,&rcRect,0,0);
    if(ddrval==DD_OK)
    {
      break;
    }
    if(ddrval==DDERR_SURFACELOST)
    {
      if(!RestoreAll())
      {
        return;
      }
      continue;
    }
    if(ddrval!=DDERR_WASSTILLDRAWING)
    {
      return;
    }
  }
}

void FiniObjects (void)
{
  if(lpDD!=NULL)
  {
    if(lpDDSPrimary!=NULL)
    {
      IDirectDrawSurface_Release(lpDDSPrimary);
      lpDDSPrimary=NULL;
    }
    if(lpDDSOne!=NULL)
    {
      IDirectDrawSurface_Release(lpDDSOne);
      lpDDSOne=NULL;
    }
    if(lpDDPal!=NULL)
    {
      IDirectDrawSurface_Release(lpDDPal);
      lpDDPal=NULL;
    }
    IDirectDrawSurface_Release(lpDD);
    lpDD=NULL;
  }
}

LRESULT CALLBACK WindowProc (HWND hWnd, UINT Msg, WPARAM wParam,
                                                               LPARAM lParam)
{
  switch(Msg)
  {
  case WM_ACTIVATE:
    bActive=wParam;
    break;
  case WM_CREATE:
    break;
  case WM_SETCURSOR:
    SetCursor(NULL);
    if(bIsInitialized)
    {
      UpdateFrame();
      IDirectDrawPalette_GetEntries(lpDDPal,0,0,256,pe);
    }
    break;
  case WM_KEYDOWN:
    switch(wParam)
    {
      case VK_ESCAPE:
      case VK_F12:
        PostMessage(hWnd,WM_CLOSE,0,0);
        break;
    }
    break;
  case WM_DESTROY:
    FiniObjects();
    PostQuitMessage(0);
    break;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}

int initFail (HWND hWnd)
{
  FiniObjects();
  MessageBox(hWnd,"DirectDraw Init FAILED","WormHole",MB_OK);
  DestroyWindow(hWnd);
  return FALSE;
}

int doInit (HINSTANCE hInst, int CmdShow)
{
  static  WNDCLASS  wc;
  memset(&wc,0,sizeof(wc));
  static  DDSURFACEDESC  ddsd;
  memset(&ddsd,0,sizeof(ddsd));
  static  HRESULT  ddrval;
  memset(&ddrval,0,sizeof(ddrval));
  wc.style=CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc=WindowProc;
  wc.cbClsExtra=0;
  wc.cbWndExtra=0;
  wc.hInstance=hInst;
  wc.hIcon=LoadIcon(hInst,IDI_APPLICATION);
  wc.hCursor=LoadCursor(NULL,IDC_ARROW);
  wc.hbrBackground=NULL;
  wc.lpszMenuName=NULL;
  wc.lpszClassName="WormHole";
  RegisterClass(&wc);
  hWnd=CreateWindowEx(0,"WormHole","WormHole",WS_POPUP,0,0,
  GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),0,(HMENU)0,
                                                                    hInst,0);
  if(!hWnd)
  {
    return 0;
  }
  ShowWindow(hWnd,CmdShow);
  UpdateWindow(hWnd);
  ddrval=DirectDrawCreate(NULL,&lpDD,NULL);
  if(ddrval!=DD_OK)
  {
    return initFail(hWnd);
  }
  ddrval=IDirectDraw_SetCooperativeLevel(lpDD,hWnd,DDSCL_EXCLUSIVE|
                                                           DDSCL_FULLSCREEN);
  ddrval=IDirectDraw_SetDisplayMode(lpDD,640,480,8);
  if(ddrval!=DD_OK)
  {
    return initFail(hWnd);
  }
  ddsd.dwSize=sizeof(ddsd);
  ddsd.dwFlags=DDSD_CAPS;
  ddsd.ddsCaps.dwCaps=DDSCAPS_PRIMARYSURFACE;
  ddrval=IDirectDraw_CreateSurface(lpDD,&ddsd,&lpDDSPrimary,NULL);
  if(ddrval!=DD_OK)
  {
    return initFail(hWnd);
  }
  ddsd.dwSize=sizeof(ddsd);
  ddsd.dwFlags=DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH;
  ddsd.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN;
  ddsd.dwWidth=640;
  ddsd.dwHeight=480;
  IDirectDraw_CreateSurface(lpDD,&ddsd,&lpDDSOne,NULL);
  if(lpDDSOne==NULL)
  {
    return initFail(hWnd);
  }
  bIsInitialized=TRUE;
  return TRUE;
}


void CyclePalette (void)
{
  int      Reg[15]; // WAS16
  int      K;
  for(K=0; K<15; K++)
  {
    Reg[K]=pe[K+30].peRed;
  }
  for(K=45; K<255; K++)
  {
    pe[K-15].peRed=pe[K].peRed;
  }
  for(K=0; K<15; K++)
  {
    pe[K+240].peRed=Reg[K];
  }
  for(K=0; K<15; K++)
  {
    Reg[K]=pe[K+30].peGreen;
  }
  for(K=45; K<255; K++)
  {
    pe[K-15].peGreen=pe[K].peGreen;
  }
  for(K=0; K<15; K++)
  {
    pe[K+240].peGreen=Reg[K];
  }
  for(K=0; K<15; K++)
  {
    Reg[K]=pe[K+30].peBlue;
  }
  for(K=45; K<255; K++)
  {
    pe[K-15].peBlue=pe[K].peBlue;
  }
  for(K=0; K<15; K++)
  {
    pe[K+240].peBlue=Reg[K];
  }
  for(K=2; K<17; K++) // WAS 15
  {
    Reg[K-2]=pe[15*K+14].peRed;
    pe[15*K+14].peRed=pe[15*K+13].peRed;
    pe[15*K+13].peRed=pe[15*K+12].peRed;
    pe[15*K+12].peRed=pe[15*K+11].peRed;
    pe[15*K+11].peRed=pe[15*K+10].peRed;
    pe[15*K+10].peRed=pe[15*K+9].peRed;
    pe[15*K+9].peRed=pe[15*K+8].peRed;
    pe[15*K+8].peRed=pe[15*K+7].peRed;
    pe[15*K+7].peRed=pe[15*K+6].peRed;
    pe[15*K+6].peRed=pe[15*K+5].peRed;
    pe[15*K+5].peRed=pe[15*K+4].peRed;
    pe[15*K+4].peRed=pe[15*K+3].peRed;
    pe[15*K+3].peRed=pe[15*K+2].peRed;
    pe[15*K+2].peRed=pe[15*K+1].peRed;
    pe[15*K+1].peRed=pe[15*K].peRed;
    pe[15*K].peRed=Reg[K-2];
    Reg[K-2]=pe[15*K+14].peGreen;
    pe[15*K+14].peGreen=pe[15*K+13].peGreen;
    pe[15*K+13].peGreen=pe[15*K+12].peGreen;
    pe[15*K+12].peGreen=pe[15*K+11].peGreen;
    pe[15*K+11].peGreen=pe[15*K+10].peGreen;
    pe[15*K+10].peGreen=pe[15*K+9].peGreen;
    pe[15*K+9].peGreen=pe[15*K+8].peGreen;
    pe[15*K+8].peGreen=pe[15*K+7].peGreen;
    pe[15*K+7].peGreen=pe[15*K+6].peGreen;
    pe[15*K+6].peGreen=pe[15*K+5].peGreen;
    pe[15*K+5].peGreen=pe[15*K+4].peGreen;
    pe[15*K+4].peGreen=pe[15*K+3].peGreen;
    pe[15*K+3].peGreen=pe[15*K+2].peGreen;
    pe[15*K+2].peGreen=pe[15*K+1].peGreen;
    pe[15*K+1].peGreen=pe[15*K].peGreen;
    pe[15*K].peGreen=Reg[K-2];
    Reg[K-2]=pe[15*K+14].peBlue;
    pe[15*K+14].peBlue=pe[15*K+13].peBlue;
    pe[15*K+13].peBlue=pe[15*K+12].peBlue;
    pe[15*K+12].peBlue=pe[15*K+11].peBlue;
    pe[15*K+11].peBlue=pe[15*K+10].peBlue;
    pe[15*K+10].peBlue=pe[15*K+9].peBlue;
    pe[15*K+9].peBlue=pe[15*K+8].peBlue;
    pe[15*K+8].peBlue=pe[15*K+7].peBlue;
    pe[15*K+7].peBlue=pe[15*K+6].peBlue;
    pe[15*K+6].peBlue=pe[15*K+5].peBlue;
    pe[15*K+5].peBlue=pe[15*K+4].peBlue;
    pe[15*K+4].peBlue=pe[15*K+3].peBlue;
    pe[15*K+3].peBlue=pe[15*K+2].peBlue;
    pe[15*K+2].peBlue=pe[15*K+1].peBlue;
    pe[15*K+1].peBlue=pe[15*K].peBlue;
    pe[15*K].peBlue=Reg[K-2];
  }
  IDirectDraw_WaitForVerticalBlank(lpDD,DDWAITVB_BLOCKBEGIN,NULL);
  if(IDirectDrawPalette_SetEntries(lpDDPal,0,0,256,pe)!=DD_OK)
  {
    return;
  }
}

int ReadBMPIntoSurfaces (void)
{
  HRESULT ddrval;
  HRSRC hBMP;
  RGBQUAD Palette[256];
  PALETTEENTRY pe[256];
  DDSURFACEDESC DDSDesc;
  LPSTR lpBits;
  LPSTR lpSrc;
  BYTE *lpBMP;
  int i;
  hBMP=FindResource(NULL,MAKEINTRESOURCE(10),RT_BITMAP);
  if(hBMP==NULL)
  {
    return 0;
  }
  lpBMP=(BYTE*)LockResource(LoadResource(NULL,hBMP));
  memcpy(Palette,&lpBMP[sizeof(BITMAPINFOHEADER)],sizeof(Palette));
  FreeResource(hBMP);
  for(i=0; i<=255; i+=1)
  {
    pe[i].peRed=Palette[i].rgbRed;
    pe[i].peGreen=Palette[i].rgbGreen;
    pe[i].peBlue=Palette[i].rgbBlue;
  }
  ddrval=IDirectDraw_CreatePalette(lpDD,DDPCAPS_8BIT,pe,&lpDDPal,NULL);
  if(ddrval!=DD_OK)
  {
    return 0;
  }
  IDirectDrawSurface_SetPalette(lpDDSPrimary,lpDDPal);
  DDSDesc.dwSize=sizeof(DDSDesc);
  ddrval=IDirectDrawSurface_Lock(lpDDSOne,NULL,&DDSDesc,0,NULL);
  if(ddrval!=DD_OK)
  {
    return 0;
  }
  lpBits=(LPSTR)DDSDesc.lpSurface;
  lpSrc=(LPSTR)(&lpBMP[sizeof(BITMAPINFOHEADER)+sizeof(Palette)+(640*480)]);
  for(i=0; i<=480; i+=1)
  {
    memcpy(lpBits,lpSrc,640);
    lpBits+=DDSDesc.lPitch;
    lpSrc-=640;
  }
  IDirectDrawSurface_Unlock(lpDDSOne,NULL);
  return TRUE;
}
