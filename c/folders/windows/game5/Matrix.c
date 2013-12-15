// Matrix.c
// The transformation matrices module.
// Created 29/09/05 by Nick Oakley

#include <math.h>
#include "Matrix.h"
#include "Constants.h"
// Functions.

void Matrix_MakeIdentity( Matrix* pMat )
{
	pMat->_11 = 1.0f;	pMat->_12 = 0.0f;	pMat->_13 = 0.0f;	pMat->_14 = 0.0f;
	pMat->_21 = 0.0f;	pMat->_22 = 1.0f;	pMat->_23 = 0.0f;	pMat->_24 = 0.0f;
	pMat->_31 = 0.0f;	pMat->_32 = 0.0f;	pMat->_33 = 1.0f;	pMat->_34 = 0.0f;
}

// This assumes we have a normal transformation matrix and so we can do a quick invert.
// Basically just take the transpose of the 3x3 bit and negate the last column vector.
void Matrix_Invert( Matrix* pMatIn, Matrix* pMatOut )
{
	Matrix matLocal;
	Vector vecOffset;

	// First transpose the rotation part. Set the offset to zero so we can use this as a pure rotation matrix in the next step.
	matLocal._11 = pMatIn->_11;		matLocal._12 = pMatIn->_21;		matLocal._13 = pMatIn->_31;		matLocal._14 = 0.0f;
	matLocal._21 = pMatIn->_12;		matLocal._22 = pMatIn->_22;		matLocal._23 = pMatIn->_32;		matLocal._24 = 0.0f;
	matLocal._31 = pMatIn->_13;		matLocal._32 = pMatIn->_23;		matLocal._33 = pMatIn->_33;		matLocal._34 = 0.0f;
	// Now rotate the translation part by this new rotation as well as negating it. This is a TRUE inverse operation in that a subsequent
	// multiplication of this inverse matrix by the original will correctly subtract the two offset components. I figured this out myself
	// and seems a more thorough method than dealing with the offset seperately when doing transformations.
	Matrix_GetOffset( pMatIn, &vecOffset );
	Matrix_VectorMul( &matLocal, &vecOffset, &vecOffset );
	Vector_Scale( &vecOffset, -1.0f, &vecOffset );
	Matrix_SetOffset( &matLocal, &vecOffset );

	pMatOut->_11 = matLocal._11;	pMatOut->_12 = matLocal._12;	pMatOut->_13 = matLocal._13;	pMatOut->_14 = matLocal._14;
	pMatOut->_21 = matLocal._21;	pMatOut->_22 = matLocal._22;	pMatOut->_23 = matLocal._23;	pMatOut->_24 = matLocal._24;
	pMatOut->_31 = matLocal._31;	pMatOut->_32 = matLocal._32;	pMatOut->_33 = matLocal._33;	pMatOut->_34 = matLocal._34;
}

// Premultiply a column vector ( x, y, z, w ) by the matrix.
void Matrix_VectorMul( Matrix* pMat, Vector* pVecIn, Vector* pVecOut )
{
	Vector vecLocal;

	// Note I'm replacing pVecIn->w with 1.0f below in case it gets set to 0.0f elsewhere.
	vecLocal.x = pVecIn->x * pMat->_11 + pVecIn->y * pMat->_12 + pVecIn->z * pMat->_13 + 1.0f * pMat->_14;
	vecLocal.y = pVecIn->x * pMat->_21 + pVecIn->y * pMat->_22 + pVecIn->z * pMat->_23 + 1.0f * pMat->_24;
	vecLocal.z = pVecIn->x * pMat->_31 + pVecIn->y * pMat->_32 + pVecIn->z * pMat->_33 + 1.0f * pMat->_34;

	pVecOut->x = vecLocal.x;
	pVecOut->y = vecLocal.y;
	pVecOut->z = vecLocal.z;
	pVecOut->w = 1.0f;
}

// Premultiply MatIn2 by MatIn1 so they are written in the order pMatIn1.pMatIn2
void Matrix_Mul( Matrix* pMatIn1, Matrix* pMatIn2, Matrix* pMatOut )
{
	Matrix matLocal;

	// First row of resultant.
	matLocal._11 = pMatIn1->_11 * pMatIn2->_11 + pMatIn1->_12 * pMatIn2->_21 + pMatIn1->_13 * pMatIn2->_31;
	matLocal._12 = pMatIn1->_11 * pMatIn2->_12 + pMatIn1->_12 * pMatIn2->_22 + pMatIn1->_13 * pMatIn2->_32;
	matLocal._13 = pMatIn1->_11 * pMatIn2->_13 + pMatIn1->_12 * pMatIn2->_23 + pMatIn1->_13 * pMatIn2->_33;
	matLocal._14 = pMatIn1->_11 * pMatIn2->_14 + pMatIn1->_12 * pMatIn2->_24 + pMatIn1->_13 * pMatIn2->_34 + pMatIn1->_14;
	// Second row of resultant.
	matLocal._21 = pMatIn1->_21 * pMatIn2->_11 + pMatIn1->_22 * pMatIn2->_21 + pMatIn1->_23 * pMatIn2->_31;
	matLocal._22 = pMatIn1->_21 * pMatIn2->_12 + pMatIn1->_22 * pMatIn2->_22 + pMatIn1->_23 * pMatIn2->_32;
	matLocal._23 = pMatIn1->_21 * pMatIn2->_13 + pMatIn1->_22 * pMatIn2->_23 + pMatIn1->_23 * pMatIn2->_33;
	matLocal._24 = pMatIn1->_21 * pMatIn2->_14 + pMatIn1->_22 * pMatIn2->_24 + pMatIn1->_23 * pMatIn2->_34 + pMatIn1->_24;
	// Third row of resultant.
	matLocal._31 = pMatIn1->_31 * pMatIn2->_11 + pMatIn1->_32 * pMatIn2->_21 + pMatIn1->_33 * pMatIn2->_31;
	matLocal._32 = pMatIn1->_31 * pMatIn2->_12 + pMatIn1->_32 * pMatIn2->_22 + pMatIn1->_33 * pMatIn2->_32;
	matLocal._33 = pMatIn1->_31 * pMatIn2->_13 + pMatIn1->_32 * pMatIn2->_23 + pMatIn1->_33 * pMatIn2->_33;
	matLocal._34 = pMatIn1->_31 * pMatIn2->_14 + pMatIn1->_32 * pMatIn2->_24 + pMatIn1->_33 * pMatIn2->_34 + pMatIn1->_34;
	// And copy to the output matrix.
	pMatOut->_11 = matLocal._11;	pMatOut->_12 = matLocal._12;	pMatOut->_13 = matLocal._13;	pMatOut->_14 = matLocal._14;
	pMatOut->_21 = matLocal._21;	pMatOut->_22 = matLocal._22;	pMatOut->_23 = matLocal._23;	pMatOut->_24 = matLocal._24;
	pMatOut->_31 = matLocal._31;	pMatOut->_32 = matLocal._32;	pMatOut->_33 = matLocal._33;	pMatOut->_34 = matLocal._34;

}

void Matrix_SetOffset( Matrix* pMatInOut, const Vector* pVecIn )
{
	pMatInOut->_14 = pVecIn->x;
	pMatInOut->_24 = pVecIn->y;
	pMatInOut->_34 = pVecIn->z;
}

void Matrix_GetOffset( Matrix* pMatIn, Vector* pVecOut )
{
	pVecOut->x = pMatIn->_14;
	pVecOut->y = pMatIn->_24;
	pVecOut->z = pMatIn->_34;
	pVecOut->w = 1.0f;
}

// Creates a rotation matrix about the X axis by the supplied angle in degrees.
void Matrix_MakeXRotation( Matrix* pMatRot, const float fAngle )
{
	float fAngRads;

	fAngRads = fAngle * DEGTORAD;
	Matrix_MakeIdentity( pMatRot );
	// Y-axis
	pMatRot->_22 = cosf( fAngRads );
	pMatRot->_32 = sinf( fAngRads );
	// Z-axis
	pMatRot->_23 = -sinf( fAngRads );
	pMatRot->_33 = cosf( fAngRads );
}

void Matrix_MakeYRotation( Matrix* pMatRot, const float fAngle )
{
	float fAngRads;

	fAngRads = fAngle * DEGTORAD;
	Matrix_MakeIdentity( pMatRot );
	// X-axis
	pMatRot->_11 = cosf( fAngRads );
	pMatRot->_31 = -sinf( fAngRads );
	// Z-axis
	pMatRot->_13 = sinf( fAngRads );
	pMatRot->_33 = cosf( fAngRads );
}

void Matrix_MakeZRotation( Matrix* pMatRot, const float fAngle )
{
	float fAngRads;

	fAngRads = fAngle * DEGTORAD;
	Matrix_MakeIdentity( pMatRot );
	// X-axis
	pMatRot->_11 = cosf( fAngRads );
	pMatRot->_21 = sinf( fAngRads );
	// Y-axis
	pMatRot->_12 = -sinf( fAngRads );
	pMatRot->_22 = cosf( fAngRads );
}

// Higher level function for rotating a position / vector about an axis by theta degrees.
void Matrix_VectorRotateAboutX( Vector* pVecIn, const float fAngle, Vector* pVecOut )
{
	Matrix	matXRot;

	Matrix_MakeXRotation( &matXRot, fAngle );
	Matrix_VectorMul( &matXRot, pVecOut, pVecOut );
}

void Matrix_VectorRotateAboutY( Vector* pVecIn, const float fAngle, Vector* pVecOut )
{
	Matrix matYRot;

	Matrix_MakeYRotation( &matYRot, fAngle );
	Matrix_VectorMul( &matYRot, pVecOut, pVecOut );
}

