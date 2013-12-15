#include "scroll.h"
#include <stdio.h>

void scroll(HWND hwnd,int dx,int dy);

int HandleScrollMessages(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg)
	{
	case WM_MOUSEMOVE:
		{
			static int xOld=0,yOld=0;
			int fwKeys = wParam;        // key flags
			int dx,dy;
			int xPos = LOWORD(lParam);  // horizontal position of cursor
			int yPos = HIWORD(lParam);  // vertical position of cursor
			if(fwKeys!=MK_LBUTTON)  {
				xOld=0;
				yOld=0;
			}
			else{
				POINT pt;
				GetScrolls(hwnd,NULL,&pt);
				int xpos=pt.x;
				int ypos=pt.y;	
				if (xOld==0)xOld=xPos;
				if (yOld==0)yOld=yPos;
				xpos+=dx=(xPos-xOld);
				ypos+=dy=(yPos-yOld);

				xOld=xPos;
				yOld=yPos;
 
				pt.x=xpos;
				pt.y=ypos;
			
				SetScrolls(hwnd,NULL,&pt);
		
			}
		}
	break;
	case WM_KEYDOWN:
		switch(wParam){
		case VK_HOME:
			SendMessage( hwnd,WM_VSCROLL,SB_TOP,0);
			break;
		case VK_END:
			SendMessage( hwnd,WM_VSCROLL,SB_BOTTOM,0);
			break;
		case VK_UP:
			SendMessage( hwnd,WM_VSCROLL,SB_LINEUP,0);
			break;
		case VK_DOWN:
			SendMessage( hwnd,WM_VSCROLL,SB_LINEDOWN,0);
			break;
		case VK_PRIOR:
			SendMessage( hwnd,WM_VSCROLL,SB_PAGEUP,0);
			break;
		case VK_NEXT:
			SendMessage( hwnd,WM_VSCROLL,SB_PAGEDOWN,0);
			break;
		case VK_LEFT:
			SendMessage( hwnd,WM_HSCROLL,SB_LINELEFT,0);
			break;
		case VK_RIGHT:
			SendMessage( hwnd,WM_HSCROLL,SB_LINERIGHT,0);
			break;
		}
	break;

	case WM_HSCROLL:
		{
			int nScrollCode = (int) LOWORD(wParam);  // scroll bar value
			int nPos = (int) HIWORD(wParam);   // scroll box position
			HWND hwndScrollBar = (HWND) lParam;

			static SCROLLINFO si;
			si.cbSize=sizeof(SCROLLINFO);
			si.fMask=SIF_ALL;
			GetScrollInfo( hwnd,SB_HORZ,&si);

			int xpos,xposold;
			xpos=xposold=si.nMin+si.nMax-si.nPos;

			RECT rc;
			GetClientRect( hwnd,&rc);
			int page=(int)((double)(rc.right-rc.left)*0.9);

			switch (nScrollCode){
			case SB_BOTTOM:		//Scrolls to the lower right.
				xpos=si.nMin;
				break;
			case SB_TOP:		//Scrolls to the lower left.
				xpos=si.nMax;
				break;
			case SB_ENDSCROLL:	//Ends scroll.
				break;
			case SB_LINELEFT:	//Scrolls left by one unit.
				xpos+=20;
				if(xpos>si.nMax)xpos=si.nMax;
				break;
			case SB_LINERIGHT:	//Scrolls right by one unit.
				xpos-=20;
				if(xpos<si.nMin)xpos=si.nMin;
				break;
			case SB_PAGELEFT:	//Scrolls left by the width of the window.
				xpos+=page;
				if(xpos>si.nMax)xpos=si.nMax;
				break;
			case SB_PAGERIGHT:	//Scrolls right by the width of the window.
				xpos-=page;
				if(xpos<si.nMin)xpos=si.nMin;
				break;
			case SB_THUMBPOSITION:	//Scrolls to the absolute position. The current position is specified by the nPos parameter.
				xpos=si.nMin+si.nMax-nPos;
				break;
			case SB_THUMBTRACK:	//Drags scroll box to the specified position. The current position is specified by the nPos parameter.
				xpos=si.nMin+si.nMax-nPos;
				break;
			}
			si.fMask=SIF_POS;
			si.nPos=si.nMax+si.nMin-xpos;
			SetScrollInfo( hwnd,SB_HORZ,&si,TRUE);

			scroll(hwnd,xpos-xposold,0);
		}
	break;
	case WM_VSCROLL:
		{
			int nScrollCode = (int) LOWORD(wParam);  // scroll bar value
			int nPos = (int) HIWORD(wParam);   // scroll box position
			HWND hwndScrollBar = (HWND) lParam;

			static SCROLLINFO si;
			si.cbSize=sizeof(SCROLLINFO);

			si.fMask=SIF_ALL;
			GetScrollInfo( hwnd,SIF_RANGE,&si);

			RECT rc;
			GetClientRect( hwnd,&rc);
			int page=(int)((double)(rc.bottom-rc.top)*0.9);

			int ypos,yposold;
			yposold=ypos=si.nMin+si.nMax-si.nPos;

			switch (nScrollCode){
			case SB_BOTTOM:		//Scrolls to the lower right.
				ypos=si.nMin;
				break;
			case SB_TOP:		//Scrolls to the upper left
				ypos=si.nMax;
				break;
			case SB_ENDSCROLL:	//Ends scroll.
				break;
			case SB_LINEDOWN:	//Scrolls one line down.
				ypos-=10;
				if(ypos<si.nMin)ypos=si.nMin;
				break;
			case SB_LINEUP:		//Scrolls one line up.
				ypos+=10;
				if(ypos>si.nMax)ypos=si.nMax;
				break;
			case SB_PAGEDOWN:	//Scrolls one page down.
				ypos-=page;
				if(ypos<si.nMin)ypos=si.nMin;
				break;
			case SB_PAGEUP:		//Scrolls one page up.
				ypos+=page;
				if(ypos>si.nMax)ypos=si.nMax;
				break;
			case SB_THUMBPOSITION:	//Scrolls to the absolute position. The current position is specified by the nPos parameter.
				si.nPos=si.nMin+si.nMax-nPos;
				break;
			case SB_THUMBTRACK:	//Drags scroll box to the specified position. The current position is specified by the nPos parameter.
				ypos=si.nMin+si.nMax-nPos;
				break;
			}
			si.fMask=SIF_POS;
			si.nPos=si.nMax+si.nMin-ypos;
			SetScrollInfo( hwnd,SB_VERT,&si,TRUE);

			scroll(hwnd,0,ypos-yposold);
		}
		break;
	default:
		return 1;
	}
return 0;
}
void scroll(HWND hwnd,int dx,int dy){
	RECT rc,rcClip,rcScroll;
	GetClientRect(hwnd,&rc);
	rcScroll=rcClip=rc;

	ScrollWindowEx(
		hwnd,       // handle of window to scroll
		dx,     // amount of horizontal scrolling
		dy,     // amount of vertical scrolling
		NULL,//&rcScroll,      // address of structure with scroll rectangle
		NULL,//&rcClip,        // address of structure with clip rectangle
		NULL,   // handle of update region
		NULL,   // address of structure for update rectangle
		SW_INVALIDATE | SW_ERASE  // scrolling flags
		);
		UpdateWindow(hwnd);
}
void SetScrolls(HWND hwnd, RECT* lpRect, POINT* lpPoint)
{
	int xOld,yOld;
	SCROLLINFO siX, siY;

	memset(&siX,0,sizeof(SCROLLINFO));
	siX.cbSize=sizeof(SCROLLINFO);
	siX.fMask=SIF_ALL;

	memset(&siY,0,sizeof(SCROLLINFO));
	siY.cbSize=sizeof(SCROLLINFO);
	siY.fMask=SIF_ALL;
	
	GetScrollInfo( hwnd,SB_HORZ,&siX);
	GetScrollInfo( hwnd,SB_VERT,&siY);
	xOld=siX.nPos;
	yOld=siY.nPos;	

	if(lpRect!=NULL){
		siX.nMin=lpRect->left;
		siX.nMax=lpRect->right;
	}
	if(lpPoint!=NULL){
		siX.nPos=siX.nMin+siX.nMax-lpPoint->x;
	}
	SetScrollInfo(hwnd,SB_HORZ,&siX,TRUE);
	GetScrollInfo(hwnd,SB_HORZ,&siX);

	if(lpRect!=NULL){
		siY.nMin=lpRect->top;
		siY.nMax=lpRect->bottom;
	}
	if(lpPoint!=NULL){
		siY.nPos=siY.nMin+siY.nMax-lpPoint->y;
	}
	SetScrollInfo(hwnd,SB_VERT,&siY,TRUE);
	GetScrollInfo(hwnd,SB_VERT,&siY);
	
	if(lpPoint!=NULL){
		scroll(hwnd,-(siX.nPos-xOld),-(siY.nPos-yOld));
	}
}
void GetScrolls(HWND hwnd, RECT* lpRect, POINT* lpPoint)
{
	SCROLLINFO si;
	si.cbSize=sizeof(SCROLLINFO);
	si.fMask=SIF_POS|SIF_RANGE;
	GetScrollInfo( hwnd,SB_HORZ,&si);

	if(lpRect!=NULL){
		lpRect->left=si.nMin;
		lpRect->right=si.nMax;
	}
	if(lpPoint!=NULL){
		lpPoint->x=si.nMin+si.nMax-si.nPos;
	}

	GetScrollInfo( hwnd,SB_VERT,&si);
	if(lpRect!=NULL){
		lpRect->top=si.nMin;
		lpRect->bottom=si.nMax;
	}
	if(lpPoint!=NULL){
		lpPoint->y=si.nMin+si.nMax-si.nPos;
	}
}
