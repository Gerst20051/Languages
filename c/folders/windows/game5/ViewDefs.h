// Viewdefs.h
// For controlling the view calculations with the viewport and perspective, clipping and so on.
// Created 10/10/05 by Nick Oakley.
#ifndef VIEWDEFS_H
#define VIEWDEFS_H


#define	ANGLE_OF_VIEW					( 45.0f )
#define VIEW_START_POS_OFFSET		(10)		// How far right and down from top left corner of client area.
#define MIN_VIEW_SIZE					( 150 )   // Any smaller than this and we don't draw anything.

typedef struct tag_VIEW {

	UINT		uiHeight;			// Height of the viewport in pixels.
	UINT		uiWidth;				// Width of the viewport in pixels;

	HDC		hDCBuffer;			// Back buffer
	HBITMAP	hMemBmp;			// Bitmap that we created and attached to the back buffer.
	HBITMAP	hMemBmpOld;		// The old default bitmap that we restore before deleting the back buffer.
	float		fEyeDist;			// Distance that all depths are measured from when doing perspective.
	
} VIEW;

#endif // VIEWDEFS_H

