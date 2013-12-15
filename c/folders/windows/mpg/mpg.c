/*************************************************************
 *  Mpg Puzzle Game - Ver : 0.02 - PK4BCX2 ( 2004 )          *
 *                                                           *
 *  Public Domain, no warranty given, use at your own risk.  *
 *  This code must retain this message.                      *
 *************************************************************/

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "mpgres.h"
#include "globals.h"
#include "bcxfn.h"

#define MoveTo(hdc, x, y) MoveToEx (hdc, x, y, NULL)

extern char szChildClass[];
static HWND hwndChild[4][4] ;
int    PosNew[16];
int    Divisions = 4;
int    rcDiv;
static HBITMAP hBitmap;
BOOL   BmpLoaded,JpgLoaded;
int    bm_x,bm_y;
HWND   hStatusbar;
int    NumMoves;

//---------------------------------------------------------------------------------------------

void FormLoad(void)
{
    hBitmap = LoadJPG("first.Jpg");
	CreateSBar(hwndMain,"Ready",1);
	SendMessage(hwndMain, WM_COMMAND, IDM_NORMAL, 0 );
	Center(hwndMain,0,0);
	ShowWindow(hwndMain, SW_SHOW);

}

//---------------------------------------------------------------------------------------------

void MainWndProc_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	char FileName[MAX_PATH];
	char Mask[256];

	HMENU hMenu;
	static  BITMAP  abitmap;

    hMenu = GetMenu(hwnd);

	switch(id)
    {

		case IDM_OPEN:
		  strcpy(Mask,
		      "bmp , jpg or gif files|*.bmp;*.jpg;*.gif");
		  strcpy(FileName,(char*)GetFileName("OPEN",Mask));
          strcpy(FileName,(char*)ucase(FileName));

          if(instr(FileName,".GIF",0,0)>0)
            {
              hBitmap=LoadGIF(FileName);
              GetObject(hBitmap,sizeof(BITMAP),&abitmap);
			  bm_x = abitmap.bmWidth;
			  bm_y = abitmap.bmHeight;
			  DestroyChilds();
        	  SetField(hwnd,bm_x,bm_y);
			  BmpLoaded = TRUE;
            }

          if(instr(FileName,".JPG",0,0)>0)
            {
              hBitmap=LoadJPG(FileName);
              GetObject(hBitmap,sizeof(BITMAP),&abitmap);
			  bm_x = abitmap.bmWidth;
			  bm_y = abitmap.bmHeight;
			  DestroyChilds();
        	  SetField(hwnd,bm_x,bm_y);
			  JpgLoaded = TRUE;
            }

		  if(instr(FileName,".BMP",0,0)>0)
            {
              hBitmap = BCX_LoadBMP(FileName);
			  GetObject(hBitmap,sizeof(BITMAP),&abitmap);
			  bm_x = abitmap.bmWidth;
			  bm_y = abitmap.bmHeight;
			  DestroyChilds();
        	  SetField(hwnd,bm_x,bm_y);
			  BmpLoaded = TRUE;
            }
            UpdateWindow(hwnd);
			NumMoves = 0;
        break;

		case IDM_NEW:
			NumMoves = 0;
			SetCursor(LoadCursor(NULL,IDC_WAIT ));
            DestroyChilds();

        	if (!BmpLoaded && !JpgLoaded)
        		SetField(hwnd,300,300);
			else
				SetField(hwnd,bm_x,bm_y);

			SetCursor(LoadCursor(NULL,IDC_ARROW ));
        break;

        case IDM_EASY:
			NumMoves = 0;
			SetCursor(LoadCursor(NULL,IDC_WAIT ));
			DestroyChilds();
			Divisions = 3;

			if (!BmpLoaded && !JpgLoaded)
        		SetField(hwnd,300,300);
			else
				SetField(hwnd,bm_x,bm_y);

			CheckMenuItem(hMenu,IDM_EASY,MF_CHECKED);
			CheckMenuItem(hMenu,IDM_NORMAL,MF_UNCHECKED);
			SetCursor(LoadCursor(NULL,IDC_ARROW ));
        break;

		case IDM_NORMAL:
			NumMoves = 0;
			SetCursor(LoadCursor(NULL,IDC_WAIT ));
            DestroyChilds();
        	Divisions = 4;

			if (!BmpLoaded && !JpgLoaded)
        		SetField(hwnd,300,300);
			else
				SetField(hwnd,bm_x,bm_y);

			CheckMenuItem(hMenu,IDM_EASY,MF_UNCHECKED);
			CheckMenuItem(hMenu,IDM_NORMAL,MF_CHECKED);
			SetCursor(LoadCursor(NULL,IDC_ARROW ));
        break;

		case IDM_ABOUT:
			DialogBox(hInst,MAKEINTRESOURCE(IDD_ABOUT),
				                           hwndMain,AboutDlg);
		break;

		case IDM_EXIT:
			PostMessage(hwnd,WM_CLOSE,0,0);
		break;
	}
}

//---------------------------------------------------------------------------------------------

LRESULT CALLBACK MainWndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
    int x,y,z;
 
	switch (msg)
	{
    case WM_CREATE :
		 Divisions = 4;
         NumMoves = 0;
         z = 0;
         for (y = 0 ; y < Divisions ; y++)
		 	for (x = 0 ; x < Divisions ; x++)
                 {
					 z++;
					 hwndChild[x][y] = CreateWindow (szChildClass, NULL,
                                    WS_CHILDWINDOW | WS_VISIBLE,
                                    0, 0, 0, 0,
                                    hwnd, (HMENU) (z),
                                   (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE),
                                    NULL) ;
                  }
         SendMessage(hWndStatusbar,msg,wParam,lParam);
		 InitializeStatusBar(hWndStatusbar,1);

    return 0 ;

	case WM_LBUTTONDOWN :
          MessageBeep (0) ;
    break ;

	case WM_MENUSELECT:
		return MsgMenuSelect(hwnd,msg,wParam,lParam);

	case WM_COMMAND:
		HANDLE_WM_COMMAND(hwnd,wParam,lParam,MainWndProc_OnCommand);
    break;

	case WM_DESTROY:
		PostQuitMessage(0);
	break;

	default:
		return DefWindowProc(hwnd,msg,wParam,lParam);
	}
	return 0;
}

//---------------------------------------------------------------------------------------------

LRESULT CALLBACK ChildWndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
     HDC  hdcBitmap;
     HDC hdc;
     PAINTSTRUCT ps ;
     RECT        rect ;
     char        buf[30];
     WORD        WinwOld , WinwNew;
	 static int  x,EmptyPos;
	 HBITMAP     hbmOld;
     BITMAP      bm;

     switch (iMsg)
          {
          case WM_CREATE :

               SetWindowWord (hwnd, 0, 0) ;
          return 0 ;

          case WM_LBUTTONDOWN :
			   WinwOld = GetWindowWord (hwnd, 0);
			   InvalidateRect (hwnd, NULL, FALSE) ;

			   for (x = 1 ; x < ( Divisions * Divisions ) + 1; x++)
               {
					WinwNew = GetWindowWord (GetDlgItem(hwndMain,x), 0);
					if (WinwNew == 0)
						{
						EmptyPos = x;
						break;
						}
                }

			   if (!ValidMove(GetDlgCtrlID (hwnd),EmptyPos))
				              break;
               NumMoves++;
               sprintf(buf,"%s%d","Moves : ",NumMoves);
			   UpdateStatusBar(buf, 0, 0);
               DoSwitch(hwnd,WinwOld,EmptyPos);

			   if (LevelDone())
				   {
					   sprintf(buf,"%s%d%s","Done in : ",NumMoves, " Moves !!");
					   MessageBox(NULL,buf,"Finished !!!!",MB_OK);
					   NumMoves = 0;
					   SendMessage(hwnd,WM_COMMAND,IDM_NEW,0);
					}

               return 0 ;

          case WM_PAINT :
              
              hdc = BeginPaint (hwnd, &ps) ;
	          int xl = 0, yt = 0,xd = 0,yd = 0;

               GetClientRect (hwnd, &rect) ;
               Rectangle (hdc, 0, 0, rect.right, rect.bottom) ;

			   if (GetWindowWord (hwnd, 0))
                    {
                      x = GetWindowWord (hwnd,0);
					  //x = GetDlgCtrlID (hwnd);
					  if (x < (Divisions * Divisions))
						  {
						   yd = 1;
						   if ( x > Divisions ) yd = 2;
						   if ( x > 2 * Divisions ) yd = 3;
						   if ( x > 3 * Divisions ) yd = 4;
						   xd = 1;
						   if ( x < Divisions + 1 ) xd = x;
						   if ( (x > Divisions) && (x < (2 * Divisions)+1)) xd = x - Divisions;
						   if ( (x > (2 * Divisions)) && (x < (3 * Divisions) + 1))
							                                           xd = x - (2 * Divisions);
						   if ( (x > (3 * Divisions)) && (x < (4 * Divisions)))
							                                            xd = x - (3 * Divisions);
						   xl = ((xd * (rect.right - rect.left))-(rect.right - rect.left));
						   //xw = rect.right - rect.left;
						   yt = ((yd * (rect.bottom - rect.top))-(rect.bottom - rect.top));
						   //yw = rect.bottom - rect.top;
						  }

                      hdcBitmap=CreateCompatibleDC(hdc);
	                  hbmOld=SelectObject(hdcBitmap,hBitmap);

	                  GetObject(hBitmap,sizeof(bm),&bm);


                      BitBlt(hdc, 0, 0,rect.right - rect.left,rect.bottom - rect.top,
					                                               hdcBitmap, xl, yt, SRCCOPY);

		              SelectObject(hdcBitmap,hbmOld);
	                  DeleteDC(hdcBitmap);
					 sprintf(buf,"%d",GetWindowWord (hwnd,0));
					 TextOut(hdc,5,5,buf,strlen(buf));
                    }

               EndPaint (hwnd, &ps) ;
               return 0 ;
          }
	return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
}

//---------------------------------------------------------------------------------------------

BOOL ValidMove(int PosClicked,int EmptyPos)
{
	if (( PosClicked + Divisions == EmptyPos) || ( PosClicked - Divisions == EmptyPos))
		return TRUE;
	else if (( PosClicked + 1 == EmptyPos) || ( PosClicked - 1 == EmptyPos))
	    return TRUE;
	else
		return FALSE;

}

//---------------------------------------------------------------------------------------------

void DoSwitch(HWND hwndClicked,int WinwOld, int EmptyPos)
{
	SetWindowWord (GetDlgItem(hwndMain,EmptyPos), 0, WinwOld);
    InvalidateRect (GetDlgItem(hwndMain,EmptyPos), NULL, FALSE) ;
	SetWindowWord (hwndClicked, 0, 0);
    InvalidateRect (GetDlgItem(hwndMain,EmptyPos), NULL, FALSE) ;
}

//---------------------------------------------------------------------------------------------

BOOL LevelDone(void)
{
    int x;
	int y = 0;
	WORD WinwNew;

	for (x = 1 ; x < ( Divisions * Divisions ) + 1; x++)
		{
			WinwNew = GetWindowWord (GetDlgItem(hwndMain,x), 0);

			if (WinwNew == x)
				{
					y++;
					if ( y ==  (Divisions * Divisions) - 1)
						{
							return TRUE;
							break;
						}
				}
         }
	return FALSE;
}

//---------------------------------------------------------------------------------------------

void SetField(HWND hwnd,int w,int h)
{

	int    cxBlock, cyBlock, x, y ,z;
	static RECT   rc, StatusRect, WinMainRect;

    hStatusbar = GetDlgItem(hwnd,IDM_STATUSBAR);

	x = 0;y = 0,z=0;

	for (y = 0 ; y < Divisions ; y++)
		for (x = 0 ; x < Divisions ; x++)
        	{
				z++;
				hwndChild[x][y] = CreateWindow (szChildClass, NULL,
                	WS_CHILDWINDOW | WS_VISIBLE,
                	0, 0, 0, 0,
                     hwnd, (HMENU) (z),
                     (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE),
                      NULL) ;
            }


		     GetClientRect(hwnd, &rc) ;
			 GetWindowRect(hStatusbar, &StatusRect);
             GetWindowRect(hwnd, &WinMainRect);
			 rcDiv = ( WinMainRect.bottom - WinMainRect.top)- (rc.bottom -rc.top);

			 MoveWindow(hwnd, WinMainRect.left, WinMainRect.top,
			             w + 5, h + (StatusRect.bottom - StatusRect.top)+ rcDiv ,TRUE);

             MoveWindow(hStatusbar, 0,
			              WinMainRect.bottom - (StatusRect.bottom - StatusRect.top),
			              w,h + (StatusRect.bottom - StatusRect.top) , TRUE);
             GetClientRect(hwnd, &rc) ;

			 cxBlock = rc.right  - rc.left;
			 cyBlock = rc.bottom - rc.top - (StatusRect.bottom - StatusRect.top) ;

			 cxBlock = cxBlock / Divisions ;
             cyBlock = cyBlock / Divisions ;

               for (x = 0 ; x < Divisions ; x++)
                    for (y = 0 ; y < Divisions ; y++)
                         MoveWindow (hwndChild[x][y],
                              x * cxBlock, y * cyBlock,
                              cxBlock, cyBlock, TRUE) ;

			srand(time(NULL));
            x = 0;

			while(1)
			 {
				y =rand() % ( Divisions * Divisions );
                for ( z = 1 ; z < (Divisions * Divisions); z++ )
					if (y == PosNew[z] )
						{
					     y = -1;
						 break;
					    }
				   if (y > -1 )
					    {
						 x++;
						 PosNew[x] = y;
						 SetWindowWord (GetDlgItem(hwnd,x), 0, y) ;
						 //PosNew[x] = x;
						 //SetWindowWord (GetDlgItem(hwnd,x), 0, x) ;
					    }
				if (x == ( Divisions * Divisions ))
					break;
			}
}

//---------------------------------------------------------------------------------------------

void DestroyChilds(void)
{
	int x = 0 , y = 0;

	if (Divisions == 3)
		{
			for (y = 0 ; y < Divisions + 1 ; y++)
		 		for (x = 0 ; x < Divisions + 1; x++)
                	{
					 DestroyWindow(hwndChild[x][y]);
                  	}
		}

	if (Divisions == 4)
		{
			for (y = 0 ; y < Divisions ; y++)
		 		for (x = 0 ; x < Divisions ; x++)
                	{
					 DestroyWindow(hwndChild[x][y]);
                  	}
		}

}

//---------------------------------------------------------------------------------------------

HBITMAP LoadJPG( char* FileName )
{
  HBITMAP   hBitmap ;
  hBitmap = BCX_LoadImage(FileName);
  return hBitmap;
}

//---------------------------------------------------------------------------------------------

HBITMAP LoadGIF (char *FileName)
{
  HBITMAP   hBitmap ;
  hBitmap = BCX_LoadImage(FileName);
  return hBitmap;
}


