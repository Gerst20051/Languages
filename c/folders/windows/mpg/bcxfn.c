// ****************************************************************************
//  Created with BCX -- The BASIC To C Translator (ver 4.63)
//  BCX (c) 1999, 2000, 2001, 2002, 2003, 2004 by Kevin Diggins
// ****************************************************************************
//  Translated for compiling using the Pelles C Compiler
// ****************************************************************************
//  This code must retain this message !!!!.
// ****************************************************************************

#include <windows.h>    // Win32 Header File 
#include <windowsx.h>   // Win32 Header File 
#include <commctrl.h>   // Win32 Header File 
#include <mmsystem.h>   // Win32 Header File 
#include <shellapi.h>   // Win32 Header File 
#include <shlobj.h>     // Win32 Header File 
#include <richedit.h>   // Win32 Header File 
#include <wchar.h>      // Win32 Header File 
#include <objbase.h>    // Win32 Header File 
#include <ocidl.h>      // Win32 Header File 
#include <winuser.h>    // Win32 Header File 
#include <olectl.h>     // Win32 Header File 
#include <conio.h>
#include <direct.h>
#include <ctype.h>
#include <io.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <setjmp.h>
#include <time.h>
#include <stdarg.h>
#include <process.h>

#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"advapi32.lib")
#pragma comment(lib,"winspool.lib")
#pragma comment(lib,"shell32.lib")
#pragma comment(lib,"ole32.lib")
#pragma comment(lib,"oleaut32.lib")
#pragma comment(lib,"uuid.lib")
#pragma comment(lib,"odbc32.lib")
#pragma comment(lib,"odbccp32.lib")
#pragma comment(lib,"delayimp.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"comdlg32.lib")
#pragma comment(lib,"ole32.lib")
#pragma comment(lib,"uuid.lib")
#pragma comment(lib,"olepro32.lib")
#pragma comment(lib,"shell32.lib")


static HINSTANCE BCX_hInstance;
static int       BCX_ScaleX;
static int       BCX_ScaleY;
char   BCX_STR [1024*1024];

// *************************************************
//                System Variables
// *************************************************

HHOOK  CmDlgHook;

// ****************************************************************************
//  BCX - Run Time Functions ( Info -> BCX-Help File )
// ****************************************************************************


char *BCX_TmpStr (size_t Bites)
{
  static int   StrCnt;
  static char *StrFunc[2048];
  StrCnt=(StrCnt + 1) & 2047;
  if(StrFunc[StrCnt]) free (StrFunc[StrCnt]);
  return StrFunc[StrCnt]=(char*)calloc(Bites+128,sizeof(char));
}

//---------------------------------------------------------------------------------------------

void BCX_Set_Scale(HWND hwnd)
{

 RECT rc          =  {0,0,4,8};
 MapDialogRect       (NULL,&rc);
 BCX_ScaleX       =  rc.right/2;
 BCX_ScaleY       =  rc.bottom/4;
 BCX_hInstance    =  (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE);

}

//---------------------------------------------------------------------------------------------

void Center (HWND hwnd, HWND Xhwnd, HWND Yhwnd)
{
  RECT rect, rectP;
  int  x, y, width, height;
  int  screenwidth, screenheight;
  if(Xhwnd==0)
    {
      RECT  DesktopArea;
      RECT  rc;
      SystemParametersInfo(SPI_GETWORKAREA,0,&DesktopArea,0);
      GetWindowRect(hwnd,&rc);
      SetWindowPos(hwnd,HWND_TOP,
        ((DesktopArea.right-DesktopArea.left)-(rc.right-rc.left))/2+
          DesktopArea.left,((DesktopArea.bottom-DesktopArea.top)-
         (rc.bottom-rc.top))/2 + DesktopArea.top,0,0,SWP_NOSIZE);
      return;
    }
  GetWindowRect (hwnd,&rect);
  GetWindowRect (Xhwnd,&rectP);
  width = rect.right-rect.left;
  x = ((rectP.right-rectP.left)-width)/2 + rectP.left;
  if(Yhwnd==NULL)
    {
      height = rect.bottom-rect.top;
      y = ((rectP.bottom-rectP.top)-height)/2 + rectP.top;
    }
  else
    {
      GetWindowRect(Yhwnd,&rectP);
      height = rect.bottom-rect.top;
      y = ((rectP.bottom-rectP.top)-height)/2+rectP.top;
    }
  screenwidth = GetSystemMetrics(SM_CXSCREEN);
  screenheight = GetSystemMetrics(SM_CYSCREEN);
  if ((x<0)) x=0;
  if ((y<0)) y=0;
  if ((x+width>screenwidth))   x = screenwidth-width;
  if ((y+height>screenheight)) y = screenheight-height;
  MoveWindow (hwnd, x, y, width, height, FALSE);
}

//---------------------------------------------------------------------------------------------

HBITMAP BCX_LoadBMP (char *F, int i)
{
 if(i)
   return (HBITMAP)LoadImage(GetModuleHandle(0),                   MAKEINTRESOURCE(i),IMAGE_BITMAP,0,0,0);
 else
   return (HBITMAP)LoadImage(GetModuleHandle(0),F,                   IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
}


//---------------------------------------------------------------------------------------------

HOOKPROC CALLBACK SBProc (UINT Msg, WPARAM wParam, LPARAM lParam)
{
  if(Msg==HCBT_ACTIVATE)
    {
     static  RECT  rc1;
     static  RECT  rc2;
     GetWindowRect(GetDesktopWindow(),&rc1);
     GetWindowRect((HWND)wParam,&rc2);
     SetWindowPos((HWND)wParam,HWND_TOP,(rc1.left+rc1.right-rc2.right+rc2.left)/2,
     (rc1.top+rc1.bottom-rc2.bottom+rc2.top)/2,0,0,SWP_NOSIZE|SWP_NOACTIVATE);
     UnhookWindowsHookEx(CmDlgHook);
    }
  return 0;
}

//---------------------------------------------------------------------------------------------

char *GetFileName(char*Title,char*Filter,int Flag,HWND hWnd,DWORD Flags,char*InitialDir)
 {
   OPENFILENAME OpenFileStruct;
   char* filename  = BCX_TmpStr(500000);
   char* RET       = BCX_TmpStr(500000);
   char* Extension = BCX_TmpStr(256);
   char* filetitle = BCX_TmpStr(256);
   char ch=0;
   int Counter=0;
   char *strtmp=0;

   memset(&OpenFileStruct,0,sizeof(OpenFileStruct));
   strtmp = BCX_TmpStr(500000);
   strcat(Extension,Filter);

   for(Counter=1;Counter<=strlen(Filter)-1;Counter++)
     {
       ch=Filter[Counter];
       if(ch=='|')
         Extension[Counter]=0;
       else
         Extension[Counter]=ch;
   }
   CmDlgHook=SetWindowsHookEx(WH_CBT,(HOOKPROC)SBProc,(HINSTANCE)NULL,GetCurrentThreadId());
   OpenFileStruct.lStructSize=sizeof(OpenFileStruct);
   OpenFileStruct.hwndOwner=hWnd;
   OpenFileStruct.hInstance=0;
   OpenFileStruct.lpstrFilter=Extension;
   OpenFileStruct.lpstrTitle=Title;
   OpenFileStruct.Flags=Flags;
   OpenFileStruct.nMaxFile=500000;
   OpenFileStruct.nMaxFileTitle=255;
   OpenFileStruct.lpstrFile=filename;
   OpenFileStruct.lpstrFileTitle=filetitle;
   OpenFileStruct.lpstrCustomFilter=0;
   OpenFileStruct.nMaxCustFilter=0;
   OpenFileStruct.nFilterIndex=0;
   OpenFileStruct.lpstrInitialDir=InitialDir;
   OpenFileStruct.nFileOffset=0;
   OpenFileStruct.nFileExtension=0;
   OpenFileStruct.lpstrDefExt=0;
   OpenFileStruct.lCustData=0;
   OpenFileStruct.lpfnHook=0;
   OpenFileStruct.lpTemplateName=0;
   OpenFileStruct.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT | OFN_EXPLORER;
   if(!Flag)
     {
       if(GetOpenFileName(&OpenFileStruct))
     {
   int len=strlen(OpenFileStruct.lpstrFile);
   if(OpenFileStruct.lpstrFile[len+1]==0)
     return strcpy(strtmp,OpenFileStruct.lpstrFile);
   else
   {
     strcpy(RET,OpenFileStruct.lpstrFile);
     OpenFileStruct.lpstrFile+=len+1;
     while(OpenFileStruct.lpstrFile[0])
     {
       len=strlen(OpenFileStruct.lpstrFile);
       sprintf(RET,"%s%s%s",RET,",",OpenFileStruct.lpstrFile);
       OpenFileStruct.lpstrFile+=len+1;
     }
     return strcpy(strtmp,RET);
   }
 }
}
else
{
 if(GetSaveFileName(&OpenFileStruct))
   return strcpy(strtmp,OpenFileStruct.lpstrFile);
 }
   return strcpy(strtmp,"");
}

//---------------------------------------------------------------------------------------------
char *ucase (char *S)
{
  register char *strtmp = BCX_TmpStr(strlen(S));
  return _strupr(strcpy(strtmp,S));
}

//---------------------------------------------------------------------------------------------

char *curdir (void)
{
  register char *strtmp = BCX_TmpStr(2048);
  GetCurrentDirectory (1024,strtmp);
  return strtmp;
}

//---------------------------------------------------------------------------------------------

char *_stristr_(char *String, char *Pattern)
{
  register char *pptr, *sptr, *start;
  register UINT  slen, plen;
  for (start = (char *)String,
    pptr  = (char *)Pattern,
    slen  = strlen(String),
    plen  = strlen(Pattern);
    slen >= plen;
    start++, slen--)
    {
     while (toupper(*start) != toupper(*Pattern))
       {
        start++;
        slen--;
        if (slen < plen)
        return(0);
       }
      sptr = start;
      pptr = (char *)Pattern;
      while (toupper(*sptr) == toupper(*pptr))
        {
         sptr++;
         pptr++;
         if (!*pptr) return (start);
        }
    }
 return(0);
}

//---------------------------------------------------------------------------------------------

int instr(char* mane,char* match,int offset,int sensflag)
{
  register char *s;
  if (!mane || !match || ! *match || offset>(int)strlen(mane)) return 0;
  if (sensflag)
    s = _stristr_(offset>0 ? mane+offset-1 : mane,match);
  else
    s = strstr (offset>0 ? mane+offset-1 : mane,match);
  return s ? (int)(s-mane)+1 : 0;
}

//---------------------------------------------------------------------------------------------

BOOL FindFirstInstance (char *Application)
{
  HWND  hWnd;
  char EventName[2048];
  hWnd=FindWindow(Application,NULL);
  if(hWnd)
    {
      if(IsIconic(hWnd))
        {
          sprintf(BCX_STR,"%s%s",Application,"_EnFI");
          strcpy(EventName,BCX_STR);
          CreateMutex(NULL,TRUE,EventName);
          if(GetLastError()==ERROR_ALREADY_EXISTS)
            {
              return TRUE;
            }
          DWORD  dwVersion;
          int    IsNT;
          dwVersion=GetVersion();
          if(dwVersion<0x80000000&&(DWORD)(LOBYTE(LOWORD(dwVersion)))>=5)
            {
              IsNT=1;
            }
          while(GetForegroundWindow()!=hWnd)
            {
              FlashWindow(hWnd,TRUE);
              Sleep(450);
              if(IsNT==1)
                {
                  FlashWindow(hWnd,FALSE);
                }
            }
        }
      SetForegroundWindow(hWnd);
      return TRUE;
    }
  return FALSE;
}
//---------------------------------------------------------------------------------------------

HWND BCX_Label
(char* Text,HWND hWnd,int id,int X,int Y,int W,int H,int Style,int Exstyle)
{
  HWND  A;
  if(!Style)
    {
      Style=WS_CHILD | SS_NOTIFY | SS_LEFT | WS_VISIBLE;
    }
  A = CreateWindowEx(Exstyle,"static",Text,Style,
                     X*BCX_ScaleX, Y*BCX_ScaleY, W*BCX_ScaleX, H*BCX_ScaleY,
                     hWnd,(HMENU)id,BCX_hInstance,NULL);
  SendMessage(A,(UINT)WM_SETFONT,(WPARAM)GetStockObject
             (DEFAULT_GUI_FONT),(LPARAM)MAKELPARAM(FALSE,0));
  if (W==0)
    {
      HDC  hdc=GetDC(A);
      SIZE  size;
      GetTextExtentPoint32(hdc,Text,strlen(Text),&size);
      ReleaseDC(A,hdc);
      MoveWindow(A,X*BCX_ScaleX,Y*BCX_ScaleY,size.cx,size.cy,TRUE);
   }
  return A;
}

//---------------------------------------------------------------------------------------------

HWND BCX_Group
(char* Text,HWND hWnd,int id,int X,int Y,int W,int H,int Style,int Exstyle)
{
  HWND  A;
  if(!Style)
    {
      Style=BS_GROUPBOX | WS_CHILD | WS_VISIBLE;
    }
  A = CreateWindowEx(Exstyle,"button",Text,Style,
                     X*BCX_ScaleX, Y*BCX_ScaleY, W*BCX_ScaleX, H*BCX_ScaleY,
                     hWnd,(HMENU)id,BCX_hInstance,NULL);
  SendMessage(A,(UINT)WM_SETFONT,(WPARAM)GetStockObject
             (DEFAULT_GUI_FONT),(LPARAM)MAKELPARAM(FALSE,0));
  return A;
}

//---------------------------------------------------------------------------------------------

HFONT BCX_Set_Font (char *Font,int Size,int Bold,int Italic,int Underline,int StrikeThru)
{
  HDC hDC=GetDC(HWND_DESKTOP);
  int CyPixels=GetDeviceCaps(hDC,LOGPIXELSY);
  ReleaseDC(HWND_DESKTOP,hDC);
  return CreateFont(0-(Size*CyPixels)/72,0,0,0,Bold,Italic,Underline,StrikeThru,
  0,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_DONTCARE,Font);
}

//---------------------------------------------------------------------------------------------

HBITMAP BCX_LoadImage (char *szFile, int r)
{
  HANDLE   hFile;
  DWORD    dwFileSize;
  LPVOID   pvData;
  HGLOBAL  hGlobal;
  DWORD    dwBytesRead;
  LPSTREAM pstm;
  HRESULT  hr;
  HBITMAP  hTempBitmap;
  LPVOID   hrlp;
  LPVOID   lpPicData;
  static   LPPICTURE gpPicture;

  if(r)
    {
      hrlp=FindResource(BCX_hInstance,MAKEINTRESOURCE(r),RT_RCDATA);
      if(hrlp!=0)
        {
          dwFileSize=SizeofResource(BCX_hInstance,hrlp);
          hGlobal=LoadResource(BCX_hInstance,hrlp);
          if(hGlobal!=0)
            {
              lpPicData=LockResource(hGlobal);
              pvData=NULL;
              hGlobal=GlobalAlloc(GMEM_MOVEABLE|GMEM_NODISCARD,dwFileSize);
              pvData=GlobalLock(hGlobal);
              CopyMemory(pvData,lpPicData,dwFileSize);
              GlobalUnlock(hGlobal);
            }
        }
    }
  else
    {
      if(GetFileAttributes(szFile)==0xffffffff) return 0;
      hFile=CreateFile(szFile,GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
      dwFileSize=GetFileSize(hFile,NULL);
      pvData=NULL;
      hGlobal=GlobalAlloc(GMEM_MOVEABLE|GMEM_NODISCARD,dwFileSize);
      pvData=GlobalLock(hGlobal);
      dwBytesRead=0;
      ReadFile(hFile,pvData,dwFileSize,&dwBytesRead,NULL);
      GlobalUnlock(hGlobal);
      CloseHandle(hFile);
    }
  pstm=NULL;
  hr=CreateStreamOnHGlobal(hGlobal,TRUE,&pstm);
  if(gpPicture) gpPicture->lpVtbl->Release(gpPicture);
  hr=OleLoadPicture(pstm,dwFileSize,FALSE,&IID_IPicture,(LPVOID*)&gpPicture);
  pstm->lpVtbl->Release(pstm);
  gpPicture->lpVtbl->get_Handle(gpPicture,(OLE_HANDLE*)&hTempBitmap);
  hTempBitmap=CopyImage(hTempBitmap,IMAGE_BITMAP,0,0,LR_COPYRETURNORG);
  return hTempBitmap;
}
