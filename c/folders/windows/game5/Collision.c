// Collision.c
// Collision module. Currently creates a list of colliding pairs of colliding spheres with their intersection depths.
// The most deeply colliding pair can then be resolved and the list regenerated ( in case the two colliding objects
// were referenced by another colliding pair somewhere in the list ). This is done until the list of colliding pairs
// is empty.
#include "Collision.h"
#include <windows.h>
#include <math.h>
#include "View.h"
#include "Constants.h"


COLLISION			aColObjects[ MAX_NUM_COL_OBJECTS ];
COLLISION*		apColObjects[ MAX_NUM_COL_OBJECTS ];
UINT				uiNumColObjects;

COLPAIR			aColPairs[ MAX_NUM_COL_PAIRS ];
COLPAIR*			apColPairs[ MAX_NUM_COL_PAIRS ];
UINT				uiNumColPairs;

// Internal prototypes;
void	Collision_ColPairs_Init( void );
void	Collision_ColPairs_New( COLLISION* pColObj1, COLLISION* pColObj2, float fDistSq, float fPenetrationSq );
BOOL	Collision_AreColliding( COLLISION* pColObj1, COLLISION* pColObj2, float* pfDistSq, float* pfPenetrationSq );


// Functions.
void Collision_Init( HWND hWnd )
{
	UINT i = 0;
	for ( i = 0; i < MAX_NUM_COL_OBJECTS; i++ )
	{
		apColObjects[ i ] = NULL;
	}
	uiNumColObjects = 0;

	Collision_ColPairs_Init();
}

void Collision_Shutdown( HWND hWnd )
{
}

// Creates a new collision object and returns a pointer to it.
COLLISION* Collision_New( ECOLTYPE eColType, Vector* pvecPos, float fRadius )
{
	UINT i = 0;

	if ( uiNumColObjects >= MAX_NUM_COL_OBJECTS )
		return NULL;

	for ( i = 0; i < MAX_NUM_COL_OBJECTS; i++ )
	{
		if ( apColObjects[ i ] == NULL )
		{
			COLLISION* pColObj;

			pColObj  = &aColObjects[ i ];
			apColObjects[ i ] = pColObj;
			pColObj->eColType = eColType;
			pColObj->vecPos = *pvecPos;
			pColObj->fRadius = fRadius;

			uiNumColObjects++;
			return pColObj;
		}
	}
	return NULL;
}

void Collision_Delete( COLLISION* pCollision )
{
	UINT i = 0;

	for ( i = 0; i < MAX_NUM_COL_OBJECTS; i++ )
	{
		if ( apColObjects[ i ] == pCollision )
		{
			apColObjects[ i ] = NULL;
			uiNumColObjects--;
		}
	}
}

void Collision_SetPos( COLLISION* pCollision, Vector* pvecPos )
{
	pCollision->vecPos = *pvecPos;
}

void Collision_SetRadius( COLLISION* pCollision, float fRadius )
{
	pCollision->fRadius = fRadius;
}

void Collision_SetColType( COLLISION* pCollision, ECOLTYPE eColType )
{
	pCollision->eColType = eColType;
}

void Collision_GetPos( COLLISION* pCollision, Vector* pvecPos )
{
	*pvecPos = pCollision->vecPos;
}

float Collision_Radius( COLLISION* pCollision )
{
	return pCollision->fRadius;
}

ECOLTYPE Collision_ColType( COLLISION* pCollision )
{
	return pCollision->eColType;
}

BOOL Collision_AreColliding( COLLISION* pColObj1, COLLISION* pColObj2, float* pfDistSq, float* pfPenetrationSq )
{
	float fRadiusSum = pColObj1->fRadius + pColObj2->fRadius;

	*pfDistSq = Vector_DistanceBetweenSq( &pColObj1->vecPos, &pColObj2->vecPos );
	*pfPenetrationSq = fRadiusSum * fRadiusSum - *pfDistSq;

	if ( *pfPenetrationSq > 0.0f )
		return TRUE;
	else
		return FALSE;
}

COLPAIR* Collision_GetClosest( COLLISION** ppColObj1, COLLISION** ppColObj2, COLPAIR* pLastColPair )
{
	COLPAIR* pClosestPair = Collision_ColPairs_GetClosest( pLastColPair );

	if ( pClosestPair )
	{
		*ppColObj1 = pClosestPair->pColObj1;
		*ppColObj2 = pClosestPair->pColObj2;
		return pClosestPair;
	}
	return NULL;
}

// Takes two collision spheres at their start positions and calculates their positions at the point of impact.
// Concentrate now, here we go :)
// Two start points P1 and P2. Relative velocity of sphere1 calculated and normalised. Cross prod  p1p2 with rel vel to get
// a normal vector. Don't normalise this vector and cross prod again with the velocity direction to get the vector from p2 to
// the line from p1 in direction of velocity. This line also has the correct magnitude of the side of
// the resulting right angled triangle. Use pythagoras with the distance between the two centres at contact to find the distance 
// back along the velocity direction we need to go. We have two distances from p2 to the line and then back along the
// velocity direction to get to the centre of the sphere at the point of its contact.
BOOL Collision_GetCollisionPos( HWND hWnd, COLLISION* pColObj1, COLLISION* pColObj2, 
							Vector* pvecVel1, Vector* pvecVel2, float* pfColRatio )
{
	Vector	vecRelVel;
	Vector	vecBetweenStartCentres;
	Vector	vecNormal1;
	Vector	vecMinusRelVelNormalised;
	Vector	vecCollisionPos;
	float	fOneOverRelVelSq;
	float	fNormal1Sq;
	float	fSquareRootArgument;
	float	fDistAlongMinusVel;
	float	fCollisionRatioX;

	// Relative velocity of Obj1 relative to Obj2 ( as if Obj2 were stationary. )
	Vector_Sub( pvecVel1, pvecVel2, &vecRelVel );
	// For dividing the results of cross products later.
	fOneOverRelVelSq = 1.0f / Vector_MagnitudeSq( &vecRelVel );
	// Vector between the two start positions from Obj1 to Obj2.
	Vector_Sub( &pColObj2->vecPos, &pColObj1->vecPos, &vecBetweenStartCentres );
	// Cross product to get an intermediate vector normal to velocity and vec between start centres.
	Vector_CrossProd( &vecBetweenStartCentres, &vecRelVel, &vecNormal1 );
	// Another cross prod to get the vector from the centre of pos2 to the line defined by pos1 and the rel vel.
	Vector_CrossProd( &vecNormal1, &vecRelVel, &vecNormal1 );
	// Scale it down to the right size. ( The distance from pos2 to the line of the velocity of obj1 )
	Vector_Scale( &vecNormal1, fOneOverRelVelSq, &vecNormal1 );
	// Now going to move back along the rel vel vector by an amount to be calculated shortly.
	Vector_Scale( &vecRelVel, -sqrtf( fOneOverRelVelSq ), &vecMinusRelVelNormalised );
	fNormal1Sq = Vector_MagnitudeSq( &vecNormal1 );
	// This is one place that can fail - if the square root argument is negative then we don't have a collision.
	fSquareRootArgument = ( pColObj1->fRadius + pColObj2->fRadius ) * ( pColObj1->fRadius + pColObj2->fRadius ) - fNormal1Sq;
	if ( fSquareRootArgument < 0.0f )
		return FALSE;
	fDistAlongMinusVel = sqrtf( fSquareRootArgument );
	// And now get our final collision position!
	Vector_Add( &pColObj2->vecPos, &vecNormal1, &vecCollisionPos );
	Vector_AddMultiple( &vecCollisionPos, &vecMinusRelVelNormalised, fDistAlongMinusVel, &vecCollisionPos );
	// And figure this out as a ratio of the velocity: the three values should be the same if it is in the direction of the 
	// velocity from point p1
	Vector_Sub( &vecCollisionPos, &pColObj1->vecPos, &vecCollisionPos );
	// Assume that the ratios of y and z are the same and choose one that isn't zero.
	if ( vecRelVel.x > FLOAT_EPSILON || vecRelVel.x < -FLOAT_EPSILON )
	{
		fCollisionRatioX = vecCollisionPos.x / vecRelVel.x;
	}
	else if ( vecRelVel.y > FLOAT_EPSILON || vecRelVel.y < -FLOAT_EPSILON )
	{
		fCollisionRatioX = vecCollisionPos.y / vecRelVel.y;
	}
	else if ( vecRelVel.z > FLOAT_EPSILON || vecRelVel.z < -FLOAT_EPSILON )
	{
		fCollisionRatioX = vecCollisionPos.z / vecRelVel.z;
	}
	else
	{
		return FALSE;
	}
	*pfColRatio = fCollisionRatioX;

	// Another failure is if the collision occurs beyond the length of our velocity vector, ie: outside this interval.
	if ( fCollisionRatioX > 1.0f )
	{
		return FALSE;
	}

	return TRUE;
}

// Colliding pairs functions.
void Collision_ColPairs_Init( void )
{
	UINT i = 0;

	for ( i = 0; i < MAX_NUM_COL_PAIRS; i++ )
	{
		apColPairs[ i ] = NULL;
	}
	uiNumColPairs = 0;
}

void Collision_ColPairs_New( COLLISION* pColObj1, COLLISION* pColObj2, float fDistSq, float fPenetrationSq )
{
	UINT	i = 0;

	if ( uiNumColPairs >= MAX_NUM_COL_PAIRS )
		return;

	for ( i = 0; i < MAX_NUM_COL_PAIRS; i++ )
	{
		if ( apColPairs[ i ] == NULL )
		{
			COLPAIR* pColPair = &aColPairs[ i ];

			apColPairs[ i ] = pColPair;
			pColPair->pColObj1 = pColObj1;
			pColPair->pColObj2 = pColObj2;
			pColPair->fDistSq = fDistSq;
			pColPair->fColDepthSq = fPenetrationSq;

			uiNumColPairs++;
			return;
		}
	}
}

void Collision_ColPairs_Generate( HWND hWnd )
{
	UINT	i = 0, j = 0;

	Collision_ColPairs_Init();
	
	for ( i = 0; i < MAX_NUM_COL_OBJECTS -1; i++ )
	{
		for ( j = i + 1; j < MAX_NUM_COL_OBJECTS; j++ )
		{
			COLLISION* pColObj1 = apColObjects[ i ];
			COLLISION* pColObj2 = apColObjects[ j ];
			float	fDistSq, fPenetrationDistSq;

			if ( pColObj1 == NULL || pColObj2 == NULL )
				continue;

			if ( Collision_AreColliding( pColObj1, pColObj2, &fDistSq, &fPenetrationDistSq ) )
			{
				Collision_ColPairs_New( pColObj1, pColObj2, fDistSq, fPenetrationDistSq );
			}
		}
	}
}

void Collision_Update( HWND hWnd )
{
	Collision_ColPairs_Generate( hWnd );
}

COLPAIR*	Collision_ColPairs_GetClosest( COLPAIR* pLastColPair )
{
	UINT i = 0;
	COLPAIR*	pClosestPair = NULL;
	float	fMaxDepthSoFarSq = 0.0f;

	// If there was a last closest pair passed in then, start from there.
	if ( pLastColPair )
	{
		fMaxDepthSoFarSq = pLastColPair->fColDepthSq;
	}

	if ( uiNumColPairs == 0 )
	{
		return NULL;
	}

	for ( i = 0; i < MAX_NUM_COL_PAIRS; i++ )
	{
		// If a valid pointer and not the last one we looked at.
		if ( apColPairs[ i ] != NULL && pLastColPair != apColPairs[ i ] )
		{
			pClosestPair = apColPairs[ i ]->fColDepthSq >= fMaxDepthSoFarSq ? apColPairs[ i ] : pClosestPair;
			if ( pClosestPair )
				fMaxDepthSoFarSq = pClosestPair->fColDepthSq;
		}
	}
	return pClosestPair;
}

// Search through list of collision pairs to see if these two are in there somewhere.
BOOL	Collision_ColPairs_AreColliding( COLLISION* pColObj1, COLLISION* pColObj2 )
{
	UINT i = 0;
	COLPAIR* pColPair = NULL;

	for ( i = 0; i < MAX_NUM_COL_PAIRS; i++ )
	{
		if ( apColPairs[ i ] != NULL )
		{
			pColPair = apColPairs[ i ];
			if ( ( pColPair->pColObj1 == pColObj1 && pColPair->pColObj2 == pColObj2 ) ||
				( pColPair->pColObj1 == pColObj2 && pColPair->pColObj2 == pColObj1 ) )
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}


