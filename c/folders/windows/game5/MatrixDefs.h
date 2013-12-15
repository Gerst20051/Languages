// MatrixDefs.h
// The 3x4 transformation matrix
// Created 29/09/05 Nick Oakley.
#ifndef MATRIXDEFS_H
#define MATRIXDEFS_H

// Defined here as 3 rows each of 4 columns.
typedef struct tag_Matrix {
	float	_11;
	float	_12;
	float	_13;
	float	_14;
	float	_21;
	float	_22;
	float	_23;
	float	_24;
	float	_31;
	float	_32;
	float	_33;
	float	_34;
} Matrix;


#endif // MATRIXDEFS_H

