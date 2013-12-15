// PhysicsDefs.h
// Structure for containing things like mass, velocity, collision volume for doing physical simulation.
// Created 5/10/05 by Nick Oakley
#ifndef PHYSICSDEFS_H
#define PHYSICSDEFS_H

#include <windows.h>
#include "Vector.h"
#include "Collision.h"

#define MAX_NUM_PHYSICS_OBJECTS		( 50 )
#define	PHYSICSFLAG_IMMOVABLE		0x00000001

typedef struct tag_PHYSICS {

	Vector			vecOldPos; // Temporary store for use by the physics system before and after collision.
	Vector			vecVel;
	float			fMass;

	DWORD		dwFlags;

	COLLISION*	pCollision;
} PHYSICS;



#endif // PHYSICSDEFS_H

