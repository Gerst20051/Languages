// Camera.h
// Header file for Camera module.
// Created 29/09/05 by Nick Oakley
#ifndef CAMERA_H
#define CAMERA_H

#include "CameraDefs.h"
#include "Vector.h"
#include "Matrix.h"

// Prototypes
void Camera_Init( void );
void Camera_Update( void );
void Camera_Shutdown( void );
void Camera_ConvertWorldToScreen( Vector* pVecWorld, Vector* pVecScreen );

void Camera_GetInverse( Matrix* pMatInv );

void Camera_AddToParams( const float fElevOffs, const float fRotOffs, const float fTwistOffs, const float fPOVDistOffs );

void Camera_SetPOV( Vector* pvecPOV );

#endif // CAMERA_H

