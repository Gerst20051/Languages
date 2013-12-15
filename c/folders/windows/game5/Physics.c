// Physics.c
// Module for physics, collision, resolution of arbitrary physical objects.
// Created 5/10/05 by Nick Oakley.
#include <windows.h>
#include <math.h>
#include "Constants.h"
#include "ErrMsg.h"
#include "Vector.h"
#include "Physics.h"
#include "Collision.h"

// Globals
PHYSICS		aPhysicsObjects[ MAX_NUM_PHYSICS_OBJECTS ];
PHYSICS*		apPhysicsObjects[ MAX_NUM_PHYSICS_OBJECTS ];
UINT			uiNumPhysicsObjects;

// Functions
void Physics_Init( HWND hWnd )
{
	UINT i = 0;
	for ( i = 0; i < MAX_NUM_PHYSICS_OBJECTS; i++ )
	{
		apPhysicsObjects[ i ] = NULL;
	}
	uiNumPhysicsObjects = 0;
}

// Go through and update all the velocities and ( desired ) positions of the physics objects ready 
// to do collision detection.
void Physics_UpdatePreCollision( HWND hWnd )
{
	Vector	vecVel, vecNewPos;
	UINT i = 0;

	for ( i = 0; i < MAX_NUM_PHYSICS_OBJECTS; i++ )
	{
		if ( apPhysicsObjects[ i ] != NULL )
		{
			PHYSICS* pPhysicsObj = apPhysicsObjects[ i ];

			// Store our previous position here in the physics so we can refer back to it later in this update.
			Collision_GetPos( pPhysicsObj->pCollision, &pPhysicsObj->vecOldPos );
			Vector_Set( &vecVel, 0.0f, -GAME_GRAVITY * GAME_FRAME_INTERVAL, 0.0f, 0.0f );
			Vector_Add( &pPhysicsObj->vecVel, &vecVel, &pPhysicsObj->vecVel );
			Vector_Add( &pPhysicsObj->vecOldPos, &pPhysicsObj->vecVel, &vecNewPos );
			// Quick dirty check for ground and walls. It will keep the bounce height correct, but
			// doesn't do a proper angle of incidence / reflection bounce.
			if ( vecNewPos.y < Collision_Radius( pPhysicsObj->pCollision ) )
			{
//				vecNewPos.y += 2.0f * ( Collision_Radius( pPhysicsObj->pCollision ) - vecNewPos.y );
//				pPhysicsObj->vecVel.y *= -1.0f;
				vecNewPos.y = Collision_Radius( pPhysicsObj->pCollision );
				pPhysicsObj->vecVel.y = 0.0f;
			}
			if ( vecNewPos.y > 50.0f - Collision_Radius( pPhysicsObj->pCollision ) )
			{
//				vecNewPos.y += 2.0f * ( Collision_Radius( pPhysicsObj->pCollision ) - vecNewPos.y );
				pPhysicsObj->vecVel.y = 0.0f;
//				vecNewPos.y = Collision_Radius( pPhysicsObj->pCollision );
//				pPhysicsObj->vecVel.y = 0.0f;
			}
			if ( vecNewPos.x < -WORLD_SIZE + Collision_Radius( pPhysicsObj->pCollision ) )
			{
				vecNewPos.x = -WORLD_SIZE + Collision_Radius( pPhysicsObj->pCollision );
				pPhysicsObj->vecVel.x *= -1.0f;
			}
			else if ( vecNewPos.x > WORLD_SIZE - Collision_Radius( pPhysicsObj->pCollision ) )
			{
				vecNewPos.x = WORLD_SIZE - Collision_Radius( pPhysicsObj->pCollision );
				pPhysicsObj->vecVel.x *= -1.0f;
			}
			if ( vecNewPos.z < -WORLD_SIZE + Collision_Radius( pPhysicsObj->pCollision ) )
			{
				vecNewPos.z = -WORLD_SIZE + Collision_Radius( pPhysicsObj->pCollision );
				pPhysicsObj->vecVel.z *= -1.0f;
			}
			else if ( vecNewPos.z > WORLD_SIZE - Collision_Radius( pPhysicsObj->pCollision ) )
			{
				vecNewPos.z = WORLD_SIZE - Collision_Radius( pPhysicsObj->pCollision );
				pPhysicsObj->vecVel.z *= -1.0f;
			}
			Collision_SetPos( pPhysicsObj->pCollision, &vecNewPos );
		}
	}
}

// We now possibly have a list of colliding pairs of objects and need to go through and resolve them one by one.
void Physics_UpdatePostCollision( HWND hWnd )
{
	COLPAIR* pColPair = NULL;
	COLLISION* pColObj1, *pColObj2;
	PHYSICS*	pPhysicsObj1, *pPhysicsObj2;
	Vector		vecBetweenCentres, vecColPos1, vecColPos2, vecRelVel;
	float		fColRatio;		// How far along the time interval the collision occurred.
	float		fDotProd;
	UINT		fNumLoops = 0.0f;


	// Find the first collision and resolve the bounce of the objects. This is done by calculating the point of contact
	// and then bouncing one ( if one is immovable ) or both of the objects apart and setting their positions to be the 
	// new position after the bounce. The last param ( pColPair ) keeps track of the last collision pair found in the list
	// until we've done them all. The search for the next closest one basically starts from pColPair in the list.
	while ( pColPair = Collision_GetClosest( &pColObj1, &pColObj2, pColPair ) )
	{
		fNumLoops += 1.0f;

		pPhysicsObj1 = Physics_GetObjectFromCollision( hWnd, pColObj1 );
		pPhysicsObj2 = Physics_GetObjectFromCollision( hWnd, pColObj2 );
		// Set the positions back to where they were at the beginning of this update and find where they
		// first collided.
		Collision_SetPos( pColObj1, &pPhysicsObj1->vecOldPos );
		Collision_SetPos( pColObj2, &pPhysicsObj2->vecOldPos );
		if ( !Collision_GetCollisionPos( hWnd, pColObj1, pColObj2, 
									&pPhysicsObj1->vecVel, &pPhysicsObj2->vecVel, &fColRatio ) )
		{
			ErrMsg( hWnd, "Couldn't find collision pos when should be able to!" );
			return;
		}
		// Now find the positions of the spheres at impact.
		Vector_AddMultiple( &pPhysicsObj1->vecOldPos, &pPhysicsObj1->vecVel, fColRatio, &vecColPos1 );
		Vector_AddMultiple( &pPhysicsObj2->vecOldPos, &pPhysicsObj2->vecVel, fColRatio, &vecColPos2 );
		// Vector from ball 1 to ball 2 between the centres.
		Vector_Sub( &vecColPos2, &vecColPos1, &vecBetweenCentres );
		Vector_Normalise( &vecBetweenCentres, &vecBetweenCentres );

		if ( pPhysicsObj1 != NULL && pPhysicsObj2 != NULL )
		{
			if ( Physics_IsImmovable( hWnd, pPhysicsObj1 ) &&
				Physics_IsImmovable( hWnd, pPhysicsObj2 ) )
			{
				// Just ignore this case.
				//ErrMsg( hWnd, "Two immovable objects collided!" );
			}
			else if ( Physics_IsImmovable( hWnd, pPhysicsObj1 ) )
			{
				Vector_Scale( &vecBetweenCentres, -1.0f, &vecBetweenCentres );
				Vector_Sub( &pPhysicsObj2->vecVel, &pPhysicsObj1->vecVel, &vecRelVel );
				// Bounce Obj2 away. Get component of velocity in direction of contact.
				fDotProd = Vector_DotProd( &vecRelVel, &vecBetweenCentres );
				// Bounce the velocity.
				Vector_AddMultiple( &pPhysicsObj2->vecVel, &vecBetweenCentres, -2.0f * fDotProd, &pPhysicsObj2->vecVel );
				// Add on that little bit of interval that was cut short by the collision.
				Vector_AddMultiple( &vecColPos2, &pPhysicsObj2->vecVel, ( 1.0f - fColRatio ), &vecColPos2 );
				Collision_SetPos( pPhysicsObj2->pCollision, &vecColPos2 );
				// And put the immovable object where it should be. Can reuse vecOldPos as it's finished with.
				Vector_Add( &pPhysicsObj1->vecOldPos, &pPhysicsObj1->vecVel, &pPhysicsObj1->vecOldPos );
				Collision_SetPos( pPhysicsObj1->pCollision, &pPhysicsObj1->vecOldPos );
			}
			else if ( Physics_IsImmovable( hWnd, pPhysicsObj2 ) )
			{
				Vector_Sub( &pPhysicsObj1->vecVel, &pPhysicsObj2->vecVel, &vecRelVel );
				// Bounce Obj1 away. Get component of velocity in direction of contact.
				fDotProd = Vector_DotProd( &vecRelVel, &vecBetweenCentres );
				// Bounce the velocity.
				Vector_AddMultiple( &pPhysicsObj1->vecVel, &vecBetweenCentres, -2.0f * fDotProd, &pPhysicsObj1->vecVel );
				// Add on that little bit of interval that was cut short by the collision.
				Vector_AddMultiple( &vecColPos1, &pPhysicsObj1->vecVel, ( 1.0f - fColRatio ), &vecColPos1 );
				Collision_SetPos( pPhysicsObj1->pCollision, &vecColPos1 );
				// And put the immovable object where it should be. Can reuse vecOldPos as it's finished with.
				Vector_Add( &pPhysicsObj2->vecOldPos, &pPhysicsObj2->vecVel, &pPhysicsObj2->vecOldPos );
				Collision_SetPos( pPhysicsObj2->pCollision, &pPhysicsObj2->vecOldPos );
			}
			else
			{
				// They both bounce.
				// The velocity of ball 1 relative to ball 2.
				Vector_Sub( &pPhysicsObj1->vecVel, &pPhysicsObj2->vecVel, &vecRelVel );
				// Bounce Obj1 away. Get component of velocity in direction of contact.
				fDotProd = Vector_DotProd( &vecRelVel, &vecBetweenCentres );
				// Bounce the velocity.
				Vector_AddMultiple( &pPhysicsObj1->vecVel, &vecBetweenCentres, -1.0f * fDotProd, &pPhysicsObj1->vecVel );
				// Add on that little bit of interval that was cut short by the collision.
				Vector_AddMultiple( &vecColPos1, &pPhysicsObj1->vecVel, ( 1.0f - fColRatio ), &vecColPos1 );
				Collision_SetPos( pPhysicsObj1->pCollision, &vecColPos1 );
				// Now bounce Obj2 two in the same way.
				Vector_AddMultiple( &pPhysicsObj2->vecVel, &vecBetweenCentres, 1.0f * fDotProd, &pPhysicsObj2->vecVel );
				Vector_AddMultiple( &vecColPos2, &pPhysicsObj2->vecVel, ( 1.0f - fColRatio ), &vecColPos2 );
				Collision_SetPos( pPhysicsObj2->pCollision, &vecColPos2 );

				// Debugging bouncing
//				Collision_SetPos( pColObj1, &pPhysicsObj1->vecOldPos );
//				Collision_SetPos( pColObj2, &pPhysicsObj2->vecOldPos );
			}
		}
	}

	if ( fNumLoops > 2.0f )
	{
		fNumLoops = fNumLoops + 0.0f;
	}
}

void		Physics_Shutdown( HWND hWnd )
{
}

PHYSICS*	Physics_New( HWND hWnd, Vector* pvecPos, Vector* pvecVel, float fMass )
{
	UINT i = 0;

	if ( uiNumPhysicsObjects >= MAX_NUM_PHYSICS_OBJECTS )
		return NULL;

	for ( i = 0; i < MAX_NUM_PHYSICS_OBJECTS; i++ )
	{
		if ( apPhysicsObjects[ i ] == NULL )
		{
			PHYSICS* pPhysics = &aPhysicsObjects[ i ];

			apPhysicsObjects[ i ] = pPhysics;
			Vector_Set( &pPhysics->vecOldPos, 0.0f, 0.0f, 0.0f, 0.0f );
			pPhysics->vecVel = *pvecVel;
			pPhysics->fMass = fMass;
			pPhysics->dwFlags = 0x00000000;

			pPhysics->pCollision = Collision_New( CT_UNDEFINED, pvecPos, 0.0f );

			uiNumPhysicsObjects++;
			return pPhysics;
		}
	}
	return NULL;
}

void Physics_Delete( HWND hWnd, PHYSICS* pPhysics )
{
	UINT i = 0;

	Collision_Delete( pPhysics->pCollision );

	for ( i = 0; i < MAX_NUM_PHYSICS_OBJECTS; i++ )
	{
		if ( apPhysicsObjects[ i ] == pPhysics )
		{
			apPhysicsObjects[ i ] = NULL;
			uiNumPhysicsObjects--;
		}
	}
}

void Physics_SetImmovable( HWND hWnd, PHYSICS* pPhysics )
{
	pPhysics->dwFlags |= PHYSICSFLAG_IMMOVABLE;
}

void Physics_ClearImmovable( HWND hWnd, PHYSICS* pPhysics )
{
	pPhysics->dwFlags &= ~PHYSICSFLAG_IMMOVABLE;
}

BOOL Physics_IsImmovable( HWND hWnd, PHYSICS* pPhysics )
{
	return ( pPhysics->dwFlags & PHYSICSFLAG_IMMOVABLE );
}

void Physics_SetCollision( HWND hWnd, PHYSICS* pPhysics, ECOLTYPE eColType, float fRadius )
{
	Collision_SetColType( pPhysics->pCollision, eColType );
	Collision_SetRadius( pPhysics->pCollision, fRadius );
}

void Physics_SetPos( HWND hWnd, PHYSICS* pPhysics, Vector* pvecPos )
{
	Collision_SetPos( pPhysics->pCollision, pvecPos );
}

void Physics_GetPos( HWND hWnd, PHYSICS* pPhysics, Vector* pvecPos )
{
	Collision_GetPos( pPhysics->pCollision, pvecPos );
}

void Physics_SetVel( HWND hWnd, PHYSICS* pPhysics, Vector* pvecVel )
{
	pPhysics->vecVel = *pvecVel;
}

BOOL Physics_GetCollisionPos( HWND hWnd, PHYSICS* pPhysObj1, PHYSICS* pPhysObj2, 
									Vector* pvecVel1, Vector* pvecVel2, float* pfColRatio )
{
	return Collision_GetCollisionPos( hWnd, pPhysObj1->pCollision, pPhysObj2->pCollision, pvecVel1, pvecVel2, pfColRatio );
}

PHYSICS* Physics_GetObjectFromCollision( HWND hWnd, COLLISION* pColObj )
{
	UINT	i = 0;
	for ( i = 0; i < MAX_NUM_PHYSICS_OBJECTS; i++ )
	{
		if ( apPhysicsObjects[ i ] != NULL )
		{
			if ( apPhysicsObjects[ i ]->pCollision == pColObj )
				return apPhysicsObjects[ i ];
		}
	}
	return NULL;
}

BOOL Physics_AreColliding( PHYSICS* pPhysObj1, PHYSICS* pPhysObj2 )
{
	return Collision_ColPairs_AreColliding( pPhysObj1->pCollision, pPhysObj2->pCollision );
}



