// Ball.h
// Created 5/10/05 by Nick Oakley
#ifndef BALL_H
#define BALL_H

#include <windows.h>
#include "Constants.h"
#include "BallDefs.h"


// Prototypes.
void	Balls_Init( HWND hWnd );
void	Balls_UpdatePrePhysics( HWND hWnd );
void	Balls_UpdatePostPhysics( HWND hWnd, HDC hDC );

BALL*	Ball_New( HWND hWnd, Vector* pvecPos, Vector* pVecVel, const float fRadius, const float fMass );
void	Ball_Delete( HWND hWnd, BALL* pBall );

void	Ball_SetPos( HWND hWnd, BALL* pBall, Vector* pvecPos );
void	Ball_GetPos( HWND hWnd, BALL* pBall, Vector* pvecPos );
void	Ball_SetVel( HWND hWnd, BALL* pBall, Vector* pvecVel );

BOOL	Balls_AreColliding( BALL* pBall1, BALL* pBall2 );


void Balls_TestInit( HWND hWnd );

#endif // BALL_H

