/*************************************************************
 *  Mpg Puzzle Game - Ver : 0.02 - PK4BCX2 ( 2004 )          *
 *                                                           *
 *  Public Domain, no warranty given, use at your own risk.  *
 *  This code must retain this message.                      *
 *************************************************************/

#include <windows.h>
#include "mpgres.h"
#include "bcxfn.h"

HICON       Icon1;
PAINTSTRUCT ps;
HDC         hdc;
HWND        Label1,Label2;

BOOL _stdcall AboutDlg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_INITDIALOG:
			 BCX_Set_Scale(hwnd);
		     Icon1=LoadIcon((HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE),
			                                         MAKEINTRESOURCE(IDI_MPGICON));
			 Label1 = BCX_Label("MPG Puzzle Game",hwnd,-1,14,9,80,20);
             SendMessage(Label1,(UINT)WM_SETFONT,
			                           (WPARAM)BCX_Set_Font("Verdana",12,FW_BOLD ),1);
             BCX_Label("Version : 0.02",hwnd,-1,45,34,60,20);
			 Label2 = BCX_Label("PK4BCX2 ( 2004 )",hwnd,-1,20,53,70,20);
			 SendMessage(Label2,(UINT)WM_SETFONT,
			                           (WPARAM)BCX_Set_Font("Verdana",10,FW_BOLD ),1);
             BCX_Group("",hwnd,-1,14,22,80,50);
			 BCX_Label("Built with : Pelles C and BCX",hwnd,-1,15,80,80,12);
             SetFocus(GetDlgItem(hwnd,IDOK));
        return 1;

        case WM_PAINT:

        	hdc = BeginPaint(hwnd, &ps);
        	DrawIcon(hdc, 40, 57,Icon1);
        	EndPaint;
		return 1;

		case WM_CLOSE:
			EndDialog(hwnd,0);
		return 1;

		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDOK:
					EndDialog(hwnd,1);
					return 1;
			}
		break;
	}
	return 0;
}
