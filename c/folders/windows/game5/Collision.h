// Collision.h
// Created 5/10/05 by Nick Oakley.
#ifndef COLLISION_H
#define COLLISION_H

#include <windows.h>
#include "CollisionDefs.h"
#include "Vector.h"

// Prototypes.
void			Collision_Init( HWND hWnd );
void 			Collision_Shutdown( HWND hWnd );
COLLISION*	Collision_New( ECOLTYPE eColType, Vector* pvecPos, float fRadius );
void			Collision_Delete( COLLISION* pCollision );
void			Collision_SetPos( COLLISION* pCollision, Vector* pvecPos );
void			Collision_SetRadius( COLLISION* pCollision, float fRadius );
void			Collision_SetColType( COLLISION* pCollision, ECOLTYPE eColType );

void			Collision_GetPos( COLLISION* pCollision, Vector* pvecPos );
float			Collision_Radius( COLLISION* pCollision );
ECOLTYPE		Collision_ColType( COLLISION* pCollision );

COLPAIR*		Collision_GetClosest( COLLISION** ppColObj1, COLLISION** ppColObj2, COLPAIR* pLastColPair );
BOOL			Collision_GetCollisionPos( HWND hWnd, COLLISION* pColObj1, COLLISION* pColObj2, 
										Vector* pvecVel1, Vector* pvecVel2, float* pfColRatio );

// Colliding pairs prototypes.
void			Collision_Update( HWND hWnd );
COLPAIR*		Collision_ColPairs_GetClosest( COLPAIR* pLastColPair );
BOOL			Collision_ColPairs_AreColliding( COLLISION* pColObj1, COLLISION* pColObj2 );


#endif // COLLISION_H

