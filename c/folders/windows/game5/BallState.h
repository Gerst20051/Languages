// BallDefs.h
// The ball object and its associated data
// Created 5/10/05 by Nick Oakley
#ifndef BALLDEFS_H
#define BALLDEFS_H

#include "Vector.h"

typedef enum {
	BS_UNDEFINED = -1,
	BS_NOTMOVING = 0,
	BS_MOVING = 1,
	BS_CAUGHT,

	BS_COUNT
} EBALLSTATE;

typedef struct tag_Ball {

	Vector			vecPos;
	Vector			vecVel;
	float			fRadius;

	EBALLSTATE	eState;
} Ball;

#endif // BALLDEFS_H

