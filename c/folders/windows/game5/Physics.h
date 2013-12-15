// Physics.h
// Module for manipulating lists of physics objects in the world that have been inserted when an object is created.
// Created 5/10/05 by Nick Oakley.
#ifndef PHYSICS_H
#define PHYSICS_H

#include <windows.h>
#include "PhysicsDefs.h"
#include "Vector.h"


// Prototypes
void		Physics_Init( HWND hWnd );
void		Physics_UpdatePreCollision( HWND hWnd );
void		Physics_UpdatePostCollision( HWND hWnd );
void		Physics_Shutdown( HWND hWnd );

PHYSICS*	Physics_New( HWND hWnd, Vector* pvecPos, Vector* pvecVel, float fMass );
void		Physics_Delete( HWND hWnd, PHYSICS* pPhysics );

void		Physics_SetImmovable( HWND hWnd, PHYSICS* pPhysics );
void		Physics_ClearImmovable( HWND hWnd, PHYSICS* pPhysics );

BOOL		Physics_IsImmovable( HWND hWnd, PHYSICS* pPhysics );

void		Physics_SetCollision( HWND hWnd, PHYSICS* pPhysics, ECOLTYPE eColType, float fRadius );
void		Physics_SetPos( HWND hWnd, PHYSICS* pPhysics, Vector* pvecPos );
void		Physics_SetVel( HWND hWnd, PHYSICS* pPhysics, Vector* pvecVel );

void		Physics_GetPos( HWND hWnd, PHYSICS* pPhysics, Vector* pvecPos );

BOOL		Physics_GetCollisionPos( HWND hWnd, PHYSICS* pPhysObj1, PHYSICS* pPhysObj2, 
									Vector* pvecVel1, Vector* pvecVel2, float* pfColRatio );
PHYSICS*	Physics_GetObjectFromCollision( HWND hWnd, COLLISION* pColObj );
BOOL		Physics_AreColliding( PHYSICS* pPhysObj1, PHYSICS* pPhysObj2 );


#endif // PHYSICS_H

