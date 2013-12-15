// WinSudoku
// Main.c
// Created 13/9/05

#include <windows.h>
#include <commctrl.h>

#include "Constants.h"
#include "ErrMsg.h"
#include "Grid.h"
#include "Buttons.h"
#include "Camera.h"
#include "State.h"
#include "Collision.h"
#include "Physics.h"
#include "Ball.h"
#include "View.h"

// Globals
const CHAR		gszMainWndClassName[] = "Game5Class";
const CHAR		gszMainTitle[] = "Game5";
BOOL			gbMouseLButtonDown;
INT				gOldMouseX, gOldMouseY;

// Prototypes
void InitApp( HWND hWnd );
void ShutdownApp( HWND hWnd );
void PaintWindow( HWND hWnd );
void ResizeWindow( HWND hWnd );
void UpdateWindowDisplay( HWND hWnd, HDC hDC );

void MouseClickCommand( HWND hWnd, WPARAM wParam, LPARAM lParam );


LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
HWND CreateMainWindow( HINSTANCE hInstance );
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd );

// Main function to redraw the view and update button positions.
void UpdateWindowDisplay( HWND hWnd, HDC hDC )
{
	View_ClearBackBuffer( hWnd );
	View_UpdateSize( hWnd );
	Grid_Draw( hWnd, View_hDCBuffer() );
	Buttons_Update( hWnd );
	View_SwapBuffers( hDC, View_hDCBuffer() );
}

void PaintWindow( HWND hWnd )
{
	HDC	hDC;
	PAINTSTRUCT	ps;

	hDC = BeginPaint( hWnd, &ps );
	if ( hDC == NULL )
	{
		ErrMsg( hWnd, "Couldn't get hDC in PaintWindow" );
		return;
	}

	UpdateWindowDisplay( hWnd, hDC );
	EndPaint( hWnd, &ps );
}

void ResizeWindow( HWND hWnd )
{
	HDC hDC;

	hDC = GetDC( hWnd );
	if ( hDC == NULL )
	{
		ErrMsg( hWnd, "Couldn't get DC in ResizeWindow" );
		return;
	}

	UpdateWindowDisplay( hWnd, hDC );
	ReleaseDC( hWnd, hDC );
}

void InitApp( HWND hWnd )
{
	UINT uResult;

	View_Init( hWnd );
	Buttons_Init( hWnd );
	Camera_Init( );
	State_Init();
	Collision_Init( hWnd );
	Physics_Init( hWnd );
	Balls_Init( hWnd );

	// Debug function creating a ball at each hand and one in space for collision testing.
	Balls_TestInit( hWnd ); 

	gbMouseLButtonDown = FALSE;

//	uResult = SetTimer( hWnd, IDT_TIMER1, TIMER_INTERVAL1, ( TIMERPROC ) NULL );
	uResult = 1;
	if ( uResult == 0 )
	{
		ErrMsg( hWnd, "Could not set timer." );
	}
}

void ShutdownApp( HWND hWnd )
{
	KillTimer( hWnd, IDT_TIMER1 );
	View_Shutdown( hWnd );
	Buttons_Shutdown( hWnd );
}

void MouseClickCommand( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
	State_Increment();
}

// The wParam contains flags stating which buttons are pressed when the mouse is moved.
// lParam contains the coords.
void MouseMoveCommand( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
	INT MouseX, MouseY;

	MouseX = ( INT )LOWORD( lParam );
	MouseY = ( INT )HIWORD( lParam );
	// Just deal with the left mouse button dragging.
	if ( wParam & MK_LBUTTON )
	{
		// Calcuate how much we've moved since the last message and update our camera accordingly.
		Camera_AddToParams( -(float)( MouseY - gOldMouseY ), -(float)( MouseX - gOldMouseX ), 0.0f, 0.0f );
	}
	if ( wParam & MK_RBUTTON )
	{
		Camera_AddToParams( 0.0f, 0.0f, 0.0f, (float)( MouseY - gOldMouseY ) );
	}
	// And update our stored positions.
	gOldMouseX = MouseX;
	gOldMouseY = MouseY;
}

void MainUpdate( HWND hWnd )
{
	HDC hDC;

	hDC = GetDC( hWnd );
	View_ClearBackBuffer( hWnd );
	View_UpdateSize( hWnd );
	Camera_Update();
	Grid_Draw( hWnd, View_hDCBuffer() );
	Balls_UpdatePrePhysics( hWnd );
	Physics_UpdatePreCollision( hWnd );
	Collision_Update( hWnd );
	Physics_UpdatePostCollision( hWnd );
	Balls_UpdatePostPhysics( hWnd, View_hDCBuffer() );

	View_SwapBuffers( hDC, View_hDCBuffer() );
	ReleaseDC( hWnd, hDC );
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		case ( WM_TIMER ):

			if ( State_IsGamePaused() )
				return 0L;

			if ( wParam == IDT_TIMER1 )
			{
				MainUpdate( hWnd );
			}
		break;
		case ( WM_COMMAND ):
		{
			switch ( LOWORD( wParam ) )
			{
				case ( IDC_BUTTON1 ):
					MessageBox( hWnd, "Button 1", "Info", MB_OK | MB_ICONINFORMATION );
				break;
			}
		}
		break;
		case ( WM_LBUTTONDOWN ):
			// Record the mouse down so that we can decide if the user was clicking when we hit a mouse
			// up message, or if they are dragging when we hit a mouse move message.
			gbMouseLButtonDown = TRUE;
		break;
		case ( WM_LBUTTONUP ):
			// If there has been no dragging.
			if ( gbMouseLButtonDown )
			{
				MouseClickCommand( hWnd, wParam, lParam );
				gbMouseLButtonDown = FALSE;
			}
		break;
		case ( WM_MOUSEMOVE ):
			// Disallow a mouse click if there is movement.
			if ( gbMouseLButtonDown )
				gbMouseLButtonDown = FALSE;
			MouseMoveCommand( hWnd, wParam, lParam );
		break;
		case ( WM_CREATE ):
			InitApp( hWnd );
		break;
		case ( WM_PAINT ):
			PaintWindow( hWnd );
		break;
		case ( WM_SIZE ):
			ResizeWindow( hWnd );
		break;
		case ( WM_CLOSE ):
			ShutdownApp( hWnd );
			DestroyWindow( hWnd );
		break;
		case ( WM_DESTROY ):
			PostQuitMessage( 0 );
		break;
		default:
			return DefWindowProc( hWnd, message, wParam, lParam );
	}

	return 0L;
}

HWND CreateMainWindow( HINSTANCE hInstance )
{
	WNDCLASSEX wcx;
	HWND hWnd;

	wcx.cbSize = sizeof( WNDCLASSEX );
	wcx.hInstance = hInstance;
	wcx.lpfnWndProc = WndProc;
	wcx.style = 0; // CS_HREDRAW | CS_VREDRAW;
	wcx.hCursor = LoadCursor( NULL, IDC_ARROW );
	wcx.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wcx.hIconSm = LoadIcon( NULL, IDI_APPLICATION );
	wcx.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
	wcx.lpszClassName = gszMainWndClassName;
	wcx.lpszMenuName = NULL;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	
	if ( !RegisterClassEx( &wcx ) )
	{
		ErrMsg( NULL, "Window Registration Failed" );
		return NULL;
	}

	if ( ( hWnd = CreateWindowEx( WS_EX_CLIENTEDGE,
						gszMainWndClassName,
						gszMainTitle,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT, CW_USEDEFAULT,
						300, 300,
						NULL, NULL,
						hInstance,
						NULL ) ) == NULL )
	{
		ErrMsg( NULL, "Window Creation Failed" );
		return NULL;
	}

	return hWnd;
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	HWND hWnd;
	MSG msg;
	BOOL bRet;
	BOOL bFinished = FALSE;

	InitCommonControls( );

	if ( ( hWnd = CreateMainWindow( hInstance ) ) == NULL )
	{
		ErrMsg( NULL, "Failed to create Main Window" );
		return 0;
	}

	UpdateWindow( hWnd );
	ShowWindow( hWnd, nShowCmd );

//	while ( ( bRet = GetMessage( &msg, hWnd, 0, 0 ) ) != 0 )
	while ( !bFinished )
	{
		if ( bRet = PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) != 0 )
		{
			if ( bRet == -1 )
			{
				ErrMsg( hWnd, "Error Getting Windows Message!" );
				return 0;
			}

			// If it is the quit message ( which would cause GetMessage to return 0 )
			// then finish.
			if ( msg.message == WM_QUIT )
			{
				bFinished = TRUE;
			}
			else
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
		else
		{
			// Do stuff.
			MainUpdate( hWnd );
		}
	}

	return msg.wParam;
}
