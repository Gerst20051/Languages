// CollisionDefs.h
// Created 5/10/05 by Nick Oakley.
#ifndef COLLISIONDEFS_H
#define COLLISIONDEFS_H

#include "Vector.h"

#define MAX_NUM_COL_OBJECTS		( 50 )
#define MAX_NUM_COL_PAIRS			( 50 )

typedef enum {
	CT_UNDEFINED = -1,
	CT_SPHERE = 0,

	CT_COUNT

} ECOLTYPE;

// Stored as part of the physics definition of a world object.
typedef struct tag_COLLISION {

	ECOLTYPE			eColType;
	Vector				vecPos;
	float				fRadius;
} COLLISION;


// Created when storing a list of all colliding pairs for further processing.
typedef struct tag_COLPAIR {

	COLLISION*		pColObj1;
	COLLISION*		pColObj2;
	float				fDistSq;		// Dist between two centres of spheres.
	float				fColDepthSq;	// Depth of penetration sort of.
} COLPAIR;



#endif // COLLISIONDEFS_H

