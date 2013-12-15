// Ball.c
// Functions for creating balls and lists of balls and moving them around the world.
// First, all the balls are updated in the following order:
// 1. Calculate the new positions for all the balls but don't move them just yet.
// 2. Go through all the balls and store all colliding pairs in a new list.
// 3. Go through the list and find the two that are colliding the most ( with the
//     greatest intersection depth ). Resolve these two to their new positions.
// 3. Update the list of colliding pairs in case these two resolved balls were referenced elsewhere.
// 4. Keep going through the list until all colliding pairs are resolved.
// Created 5/10/05 by Nick Oakley.
#include <windows.h>
#include <assert.h>
#include <math.h>
#include "Constants.h"
#include "Ball.h"
#include "Vector.h"
#include "ErrMsg.h"
#include "Collision.h"
#include "Physics.h"
#include "View.h"
#include "Camera.h"

// Globals
BALL		aBalls[ MAX_NUM_BALLS ];
BALL*		apBalls[ MAX_NUM_BALLS ];
UINT		uiNumBalls;

// Ball Functions
void Balls_Init( HWND hWnd )
{
	UINT	i = 0;
	for ( i = 0; i < MAX_NUM_BALLS; i++ )
	{
		apBalls[ i ] = NULL;
	}
	uiNumBalls = 0;
}

BALL* Ball_New( HWND hWnd, Vector* pvecPos, Vector* pvecVel, const float fRadius, const float fMass )
{
	UINT i = 0;
	for ( i = 0; i < MAX_NUM_BALLS; i++ )
	{
		if ( apBalls[ i ] == NULL )
		{
			BALL* pBall = &aBalls[ i ];

			apBalls[ i ] = pBall;
			// In this case, the ball's radius and collision radius are the same.
			pBall->fRadius = fRadius;
			pBall->eState = BS_NOTMOVING;
			pBall->pPhysics = Physics_New( hWnd, pvecPos, pvecVel, fMass );
			// Uses spherical collision ( duh! )
			Physics_SetCollision( hWnd, pBall->pPhysics, CT_SPHERE, fRadius );

			uiNumBalls++;
			return pBall;
		}
	}
	return NULL;
}

void Ball_Delete( HWND hWnd, BALL* pBall )
{
	UINT i = 0;
	for ( i = 0; i < MAX_NUM_BALLS; i++ )
	{
		if ( apBalls[ i ] == pBall )
		{
			Physics_Delete( hWnd, pBall->pPhysics );
			apBalls[ i ] == NULL;
			uiNumBalls--;
			return;
		}
	}
}

void Ball_SetPos( HWND hWnd, BALL* pBall, Vector* pvecPos )
{
	assert( pBall );

	Physics_SetPos( hWnd, pBall->pPhysics, pvecPos );
}

void Ball_SetVel( HWND hWnd, BALL* pBall, Vector* pvecVel )
{
	assert( pBall );

	Physics_SetVel( hWnd, pBall->pPhysics, pvecVel );
}

void Ball_GetPos( HWND hWnd, BALL* pBall, Vector* pvecPos )
{
	assert( pBall );

	Physics_GetPos( hWnd, pBall->pPhysics, pvecPos );
}

// Looks in the colliding pairs list to see if these two balls are colliding there.
BOOL Balls_AreColliding( BALL* pBall1, BALL* pBall2 )
{
	assert( pBall1 );
	assert( pBall2 );

	return Physics_AreColliding( pBall1->pPhysics, pBall2->pPhysics );
}

// Currently a test function for creating balls for debugging collision.
void Balls_TestInit( HWND hWnd )
{
	Vector	vecBallPos;
	Vector	vecBallVel;
	UINT	uiBall;

	for ( uiBall = 0; uiBall < MAX_NUM_BALLS; uiBall++ )
	{
		Vector_Set( &vecBallPos, cosf( (float)uiBall / (float)MAX_NUM_BALLS * 360.0f * DEGTORAD ) * 50.0f,
					5.0f,
					sinf( (float)uiBall / (float)MAX_NUM_BALLS * 360.0f * DEGTORAD ) * 50.0f,
					1.0f );
		Vector_Set( &vecBallVel, 1.0f + (float)uiBall / 10.0f, 0.0f, 0.0f + (float)uiBall / 10.0f, 1.0f );
		Ball_New( hWnd, &vecBallPos, &vecBallVel, 5.0f, 1.0f );
	}
}

void Balls_UpdatePrePhysics( HWND hWnd )
{
	Vector	vecVel;
	Vector	vecOffset;

	Vector_Set( &vecOffset, 0.0f, 0.0f, 0.0f , 1.0f );
	Vector_Set( &vecVel, 0.0f, 0.0f, 0.0f, 1.0f );

	// Does nothing much at the moment.
}

void Balls_UpdatePostPhysics( HWND hWnd, HDC hDC )
{
	Vector	vecPos;
	UINT	uiBall;

	if ( uiNumBalls == 0 )
		return;

	for ( uiBall = 0; uiBall < MAX_NUM_BALLS; uiBall++ )
	{
		if ( apBalls[ uiBall ] != NULL )
		{
			BALL*		pBall;
			DWORD	dwColours;

			pBall = apBalls[ uiBall ];

			Ball_GetPos( hWnd, pBall, &vecPos );
			// Construct some pretty colours.
			dwColours = 0x0080FFFF * (DWORD)uiBall / (DWORD)MAX_NUM_BALLS;
			View_DrawSphereRGB( hWnd, hDC, &vecPos, pBall->fRadius, (COLORREF)dwColours );
		}
	}
}

