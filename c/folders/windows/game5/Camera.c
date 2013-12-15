// Camera.c
// Camera module for placing a camera in the world and using this to display a view of the 3d world
// on the 2d screen.
// Created 29/09/05 by Nick Oakley.

#include "Camera.h"
#include "Vector.h"
#include "Matrix.h"

// Globals
Camera gCamera;

// Functions
void Camera_Init( void )
{
	Matrix_MakeIdentity( &gCamera.matCamera );
	Matrix_Invert( &gCamera.matCamera, &gCamera.matInverseCamera );

	Vector_Set( &gCamera.vecPOV, 0.0f, 0.0f, 0.0f, 0.0f );
	Vector_Set( &gCamera.vecCamera, 0.0f, 0.0f, 0.0f, 0.0f );

	gCamera.fPOVDist = CAMERA_POV_DIST;
	gCamera.fElevation = 30.0f;
	gCamera.fRotation = 0.0f;
	gCamera.fTwist = 0.0f;

	Camera_Update( );
}


void Camera_Update( void )
{
	Vector vecZ;
	Matrix	matCam, matRot;

	Vector_Set( &vecZ, 0.0f, 0.0f, 1.0f, 0.0f );
	Vector_Set( &gCamera.vecCamera, 0.0f, 0.0f, 0.0f, 0.0f );
	// Move back along the z-axis by an amount.
	Vector_AddMultiple( &gCamera.vecCamera, &vecZ, -gCamera.fPOVDist, &gCamera.vecCamera );

	// Stick this vector in the offset ( last column ) part of our matrix.
	Matrix_MakeIdentity( &matCam );
	Matrix_SetOffset( &matCam, &gCamera.vecCamera );
	// Now rotate it around the x and then the y axes to get the final camera position and orientation.
	Matrix_MakeZRotation( &matRot, gCamera.fTwist );
	Matrix_Mul( &matRot, &matCam, &matCam );
	Matrix_MakeXRotation( &matRot, gCamera.fElevation );
	Matrix_Mul( &matRot, &matCam, &matCam );
	Matrix_MakeYRotation( &matRot, gCamera.fRotation );
	Matrix_Mul( &matRot, &matCam, &matCam );
	// And finally the POV.
	Matrix_GetOffset( &matCam, &gCamera.vecCamera );
	Vector_Add( &gCamera.vecCamera, &gCamera.vecPOV, &gCamera.vecCamera );
	Matrix_SetOffset( &matCam, &gCamera.vecCamera );

	gCamera.matCamera = matCam;
	// And store the inverse.
	Matrix_Invert( &gCamera.matCamera, &gCamera.matInverseCamera );
}

void Camera_AddToParams( const float fElevOffs, const float fRotOffs, const float fTwistOffs, const float fPOVDistOffs )
{
	gCamera.fElevation += fElevOffs;
	if ( gCamera.fElevation > MAX_ROTATION_DEG )
		gCamera.fElevation -= MAX_ROTATION_DEG;
	gCamera.fRotation += fRotOffs;
	if ( gCamera.fRotation > MAX_ROTATION_DEG )
		gCamera.fRotation -= MAX_ROTATION_DEG;
	gCamera.fTwist += fTwistOffs;
	if ( gCamera.fTwist > MAX_ROTATION_DEG )
		gCamera.fTwist -= MAX_ROTATION_DEG;
	gCamera.fPOVDist += fPOVDistOffs;
}

void Camera_SetPOV( Vector* pvecPOV )
{
	gCamera.vecPOV = *pvecPOV;
}

void Camera_Shutdown( void )
{
	// Do nothing for now.
}

void Camera_GetInverse( Matrix* pMatInv )
{
	*pMatInv = gCamera.matInverseCamera;
}

