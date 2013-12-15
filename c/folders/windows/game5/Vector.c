// Vector.c
// Functions for manipulating ( x, y, z, w ) vectors
// Created 29/09/05 by Nick Oakley

#include <math.h>
#include "Vector.h"

// Functions

void Vector_Set( Vector* pVecOut, const float x, const float y, const float z, const float w )
{
	pVecOut->x = x;
	pVecOut->y = y;
	pVecOut->z = z;
	pVecOut->w = w;
}

void Vector_Scale( Vector* pVecIn, const float fScale, Vector* pVecOut )
{
	pVecOut->x = fScale * pVecIn->x;
	pVecOut->y = fScale * pVecIn->y;
	pVecOut->z = fScale * pVecIn->z;
}

void Vector_Add( Vector* pVecIn1, Vector* pVecIn2, Vector* pVecOut )
{
	pVecOut->x = pVecIn1->x + pVecIn2->x;
	pVecOut->y = pVecIn1->y + pVecIn2->y;
	pVecOut->z = pVecIn1->z + pVecIn2->z;
}

void Vector_Sub( Vector* pVecIn1, Vector* pVecIn2, Vector* pVecOut )
{
	pVecOut->x = pVecIn1->x - pVecIn2->x;
	pVecOut->y = pVecIn1->y - pVecIn2->y;
	pVecOut->z = pVecIn1->z - pVecIn2->z;
}

// Adds a multiple of one vector to another vector.
void Vector_AddMultiple( Vector* pVecIn, Vector* pVecAddMult, float fMultiple, Vector* pVecOut )
{
	pVecOut->x = pVecIn->x + fMultiple * pVecAddMult->x;
	pVecOut->y = pVecIn->y + fMultiple * pVecAddMult->y;
	pVecOut->z = pVecIn->z + fMultiple * pVecAddMult->z;
}

// Note that this function assumes the w component to be irrelevant at the moment.
float Vector_DotProd( Vector* pVecIn1, Vector* pVecIn2 )
{
	return ( pVecIn1->x * pVecIn2->x + pVecIn1->y * pVecIn2->y + pVecIn1->z * pVecIn2->z );
}

// Ignores the w component of the vectors.
void Vector_CrossProd( Vector* pVecIn1, Vector* pVecIn2, Vector* pVecOut )
{
	Vector vecLocal;

	vecLocal.x = pVecIn1->y * pVecIn2->z - pVecIn2->y * pVecIn1->z;
	vecLocal.y = -( pVecIn1->x * pVecIn2->z - pVecIn2->x * pVecIn1->z );
	vecLocal.z = pVecIn1->x * pVecIn2->y - pVecIn2->x * pVecIn1->y;

	pVecOut->x = vecLocal.x;
	pVecOut->y = vecLocal.y;
	pVecOut->z = vecLocal.z;
	pVecOut->w = 1.0f;
}

float Vector_MagnitudeSq( Vector* pVecIn )
{
	return ( Vector_DotProd( pVecIn, pVecIn ) );
}

float Vector_DistanceBetweenSq( Vector* pVecIn1, Vector* pVecIn2 )
{
	Vector	vecDiff;

	Vector_Sub( pVecIn1, pVecIn2, &vecDiff );
	return Vector_MagnitudeSq( &vecDiff );
}

void	Vector_Normalise( Vector* pVecIn, Vector* pVecOut )
{
	float	fMagnitude;

	fMagnitude = sqrtf( Vector_MagnitudeSq( pVecIn ) );
	Vector_Scale( pVecIn, 1.0f / fMagnitude, pVecOut );
}

