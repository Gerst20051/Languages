// ErrMsg.c
// Generic error messaging dialog
// Created 13/09/05

#include "ErrMsg.h"
#include "Constants.h"
#include "State.h"



void ErrMsg( HWND hWnd, LPSTR szMsg )
{
	// So that timers are not acted upon in the background, possibly before the
	// game is even ready for them.
	State_SetGamePaused();
	MessageBox( hWnd, szMsg, "Error!", MB_ICONEXCLAMATION | MB_OK );
	State_ClearGamePaused();
}
