// BallDefs.h
// The ball object and its associated data
// Created 5/10/05 by Nick Oakley
#ifndef BALLDEFS_H
#define BALLDEFS_H

#include "Vector.h"
#include "Physics.h"

#define MAX_NUM_BALLS		( 5 )

typedef enum {
	BS_UNDEFINED = -1,
	BS_NOTMOVING = 0,
	BS_MOVING = 1,
	BS_CAUGHT,

	BS_COUNT
} EBALLSTATE;

typedef struct tag_BALL {

	float			fRadius;
	PHYSICS*		pPhysics;
	EBALLSTATE	eState;
} BALL;

#endif // BALLDEFS_H

