// Grid.c
// Draw the grid representing the ground in the world and also the xyz axes at the origin.
// Created 18/10/05 by Nick Oakley
#include <windows.h>
#include "Grid.h"
#include "Vector.h"
#include "View.h"


void Grid_DrawOrigin( HWND hWnd, HDC hDC )
{
	Vector	vec1, vec2;

	// The x,y,z coordinate axes.
	Vector_Set( &vec1, 0.0f, 0.0f, 0.0f, 0.0f );
	Vector_Set( &vec2, GRID_AXES_SIZE, 0.0f, 0.0f, 0.0f );
	View_DrawLineRGB( hWnd, hDC, &vec1, &vec2, RGB( 255, 0, 0 ) );

	Vector_Set( &vec1, 0.0f, 0.0f, 0.0f, 0.0f );
	Vector_Set( &vec2, 0.0f, GRID_AXES_SIZE, 0.0f, 0.0f );
	View_DrawLineRGB( hWnd, hDC, &vec1, &vec2, RGB( 0, 255, 0 ) );

	Vector_Set( &vec1, 0.0f, 0.0f, 0.0f, 0.0f );
	Vector_Set( &vec2, 0.0f, 0.0f, GRID_AXES_SIZE, 0.0f );
	View_DrawLineRGB( hWnd, hDC, &vec1, &vec2, RGB( 0, 0, 255 ) );
}

void Grid_Draw( HWND hWnd, HDC hDC )
{
	float fX = 0.0f, fZ = 0.0f;
	Vector vecStart, vecEnd;

	for ( fX = -GRID_SIZE; fX < GRID_SIZE + GRID_SQUARE_SIZE; fX += GRID_SQUARE_SIZE )
	{
		for ( fZ = -GRID_SIZE; fZ < GRID_SIZE + GRID_SQUARE_SIZE; fZ += GRID_SQUARE_SIZE )
		{
			Vector_Set( &vecStart, fX, 0.0f, fZ, 0.0f );
			Vector_Set( &vecEnd, fX, 0.0f, fZ + GRID_SQUARE_SIZE, 0.0f );
			View_DrawLineRGB( hWnd, hDC, &vecStart, &vecEnd, RGB( 50, 50, 50 ) );
			Vector_Set( &vecStart, fX, 0.0f, fZ, 0.0f );
			Vector_Set( &vecEnd, fX + GRID_SQUARE_SIZE, 0.0f, fZ, 0.0f );
			View_DrawLineRGB( hWnd, hDC, &vecStart, &vecEnd, RGB( 50, 50, 50 ) );
		}
	}

	Grid_DrawOrigin( hWnd, hDC );
}

