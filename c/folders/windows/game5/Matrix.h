// Matrix.h
// Header file for the Matrix module
// Created 29/09/05 by Nick Oakley
#ifndef MATRIX_H
#define MATRIX_H

#include "Vector.h"
#include "MatrixDefs.h"

// Function prototypes.
void Matrix_MakeIdentity( Matrix* pMat );
void Matrix_Invert( Matrix* pMatIn, Matrix* pMatOut );
void Matrix_VectorMul( Matrix* pMat, Vector* pVecIn, Vector* pVecOut );
void Matrix_Mul( Matrix* pMatIn1, Matrix* pMatIn2, Matrix* pMatOut );

void Matrix_SetOffset( Matrix* pMatInOut, const Vector* pVecIn );
void Matrix_GetOffset( Matrix* pMatIn, Vector* pVecOut );

void Matrix_MakeXRotation( Matrix* pMatRot, const float fAngle );
void Matrix_MakeYRotation( Matrix* pMatRot, const float fAngle );
void Matrix_MakeZRotation( Matrix* pMatRot, const float fAngle );

void Matrix_VectorRotateAboutX( Vector* pVecIn, const float fAngle, Vector* pVecOut );
void Matrix_VectorRotateAboutY( Vector* pVecIn, const float fAngle, Vector* pVecOut );


#endif // MATRIX_H

