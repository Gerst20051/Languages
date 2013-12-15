// State.h
// Header for the main state module
// Created 1/10/05 by Nick Oakley
#ifndef STATE_H
#define STATE_H

#include "StateDefs.h"

// Function Prototypes
void			State_Init( void );
void			State_Set( EMAINSTATE eNewState );
void			State_SetGamePaused( void );
void			State_ClearGamePaused( void );
BOOL			State_IsGamePaused( void );
void			State_Increment( void );

EMAINSTATE	State_Get( void );


#endif // STATE_H

