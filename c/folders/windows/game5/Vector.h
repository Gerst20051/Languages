// Vector.h
// Header for the vector module.
// Created 29/09/05 by Nick Oakley
#ifndef VECTOR_H
#define VECTOR_H

#include "VectorDefs.h"

// Functions
void	Vector_Set( Vector* pVecOut, const float x, const float y, const float z, const float w );
void	Vector_Scale( Vector* pVecIn, const float fScale, Vector* pVecOut );

void	Vector_Add( Vector* pVecIn1, Vector* pVecIn2, Vector* pVecOut );
void	Vector_Sub( Vector* pVecIn1, Vector* pVecIn2, Vector* pVecOut );
void	Vector_AddMultiple( Vector* pVecIn, Vector* pVecAddMult, float fMultiple, Vector* pVecOut );

float	Vector_DotProd( Vector* pVecIn1, Vector* pVecIn2 );
void	Vector_CrossProd( Vector* pVecIn1, Vector* pVecIn2, Vector* pVecOut );

float	Vector_MagnitudeSq( Vector* pVecIn );
float	Vector_DistanceBetweenSq( Vector* pVecIn1, Vector* pVecIn2 );

void	Vector_Normalise( Vector* pVecIn, Vector* pVecOut );
#endif // VECTOR_H

