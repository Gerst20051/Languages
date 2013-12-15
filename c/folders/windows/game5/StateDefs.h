// StateDefs.h
// Data for containing the current state of the program.
// Created 29/09/05 by Nick Oakley
#ifndef STATEDEFS_H
#define STATEDEFS_H

#include <windows.h>

typedef enum {

	EMS_UNDEFINED = -1,
	EMS_WAVYBOXES = 0,
	EMS_SINECURVES = 1,
	EMS_WORLD,
	EMS_COUNT

} EMAINSTATE;

typedef struct tag_MainState {

	EMAINSTATE	eState;
	BOOL			bGamePaused;
} MainState;




#endif // STATEDEFS_H

