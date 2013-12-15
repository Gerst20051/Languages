// Buttons.c
// Functions for creating and manipulating push buttons on the main window.

#include <windows.h>
#include "Buttons.h"
#include "ErrMsg.h"
#include "View.h"

// Globals
HWND		aButtons[ NUM_BUTTONS ];

// Create the button and place in the main window as a child.
void Buttons_Init( HWND hWnd )
{
	UINT	uiViewSize;
	UINT	uiButtonNumber;
	UINT	uiCurrentButtonY;

	uiViewSize = View_Width();
	uiCurrentButtonY = BUTTON_WINDOW_TOP_OFFSET;

	for ( uiButtonNumber = 0; uiButtonNumber < NUM_BUTTONS; uiButtonNumber++ )
	{
		aButtons[ uiButtonNumber ] = CreateWindowEx( 0, "BUTTON", "IGNORE ME!!",
														BS_DEFPUSHBUTTON | WS_CHILD | WS_VISIBLE, 
														uiViewSize + BUTTON_GRID_X_OFFSET,
														uiCurrentButtonY,
														BUTTON_WIDTH, 
														BUTTON_HEIGHT,
														hWnd,
														(HMENU)( IDC_BUTTON1 + uiButtonNumber ),
														(HINSTANCE)GetModuleHandle( NULL ),
														NULL );

		if ( !aButtons[ uiButtonNumber ] )
		{
			ErrMsg( hWnd, "Couldn't create Button" );
		}

		uiCurrentButtonY += BUTTON_HEIGHT + BUTTON_VERTICAL_GAP;
	}
}

void Buttons_Shutdown( HWND hWnd )
{
	// Don't need to destroy buttons as I think this is done automatically.
}

void Buttons_Update( HWND hWnd )
{
	HWND hButton;
	UINT	uiButtonNumber;

	for ( uiButtonNumber = 0; uiButtonNumber < NUM_BUTTONS; uiButtonNumber++ )
	{
		hButton = GetDlgItem( hWnd, IDC_BUTTON1 + uiButtonNumber );
		if ( hButton != NULL )
		{
			SetWindowPos( hButton, NULL,
							View_Width() + BUTTON_GRID_X_OFFSET, 
							BUTTON_WINDOW_TOP_OFFSET + uiButtonNumber * ( BUTTON_HEIGHT + BUTTON_VERTICAL_GAP ),
							BUTTON_WIDTH, BUTTON_HEIGHT,
							0 );
		}
	}
}
