// State.c
// Module for manipulating and querying the current state of the app.
// Create 1/10/05 by Nick Oakley
#include "State.h"

// Globals
MainState	gMainState;


// Functions
void			State_Init( void )
{
	gMainState.eState = EMS_WORLD;
	gMainState.bGamePaused = FALSE;
}

void			State_Set( EMAINSTATE eNewState )
{
	gMainState.eState = eNewState;
}

void State_Increment( void )
{
	if ( ++gMainState.eState == EMS_COUNT )
		gMainState.eState = 0;
}

EMAINSTATE	State_Get( void )
{
	return gMainState.eState;
}

void			State_SetGamePaused( void )
{
	gMainState.bGamePaused = TRUE;
}

void			State_ClearGamePaused( void )
{
	gMainState.bGamePaused = FALSE;
}

BOOL			State_IsGamePaused( void )
{
	return gMainState.bGamePaused;
}




