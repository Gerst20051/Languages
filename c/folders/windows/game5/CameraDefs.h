// CameraDefs.h
// Data structure for the camera used to display 3d graphics on the screen.
// Created 29/09/05 by Nick Oakley
#ifndef CAMERADEFS_H
#define CAMERADEFS_H

#include "Vector.h"
#include "Matrix.h"

#define CAMERA_POV_DIST		( 200.0f )
#define MAX_ROTATION_DEG		( 360.0f )

typedef struct tag_Camera {

	Matrix		matCamera;
	Matrix		matInverseCamera;
	Vector		vecPOV;
	Vector		vecCamera;

	float		fPOVDist;  // Distance of camera from point of view.
	float		fElevation; // Angle rotated about the world x-axis before rotating by an
	float		fRotation;  // angle around the world y-axis.
	float		fTwist;		// Twist around the view direction vector ( own z-axis )
} Camera;


#endif // CAMERADEFS_H

