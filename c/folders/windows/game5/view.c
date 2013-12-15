// View.c
// Functions for drawing to the 2d viewport and for converting between world matrices and screen coords.
// Created 29/09/05 by Nick Oakley

#include <windows.h>
#include <math.h>
#include <assert.h>
#include "Constants.h"
#include "Vector.h"
#include "View.h"
#include "ErrMsg.h"

// Prototypes
void View_DrawReflectedPoints( HWND hWnd, HDC hDC, Vector* pVecPos, float fX, float fY, float fZ, float fX1, float fY1, float fZ1 );

// Globals
VIEW			gView;

// Functions.
void View_Init( HWND hWnd )
{
	HDC hDC;
	RECT rect;

	hDC = GetDC( hWnd );
	if ( hDC == NULL )
	{
		ErrMsg( hWnd, "Couldn't get DC in View_Init" );
	}
	// Our back buffer for drawing to before blitting to the client area.
	gView.hDCBuffer = CreateCompatibleDC( hDC );
	if ( gView.hDCBuffer == NULL )
	{
		ErrMsg( hWnd, "Couldn't create hDCBuffer" );
	}
	// Our back buffer needs a proper bitmap other than the default 1by1 thing put there on creation.
	// Just using a max size for now.
	gView.hMemBmp = CreateCompatibleBitmap( hDC, 800, 600 );
	gView.hMemBmpOld = SelectObject( gView.hDCBuffer, gView.hMemBmp );
	if ( !gView.hMemBmpOld )
	{
		ErrMsg( hWnd, "Problem selecting new bitmap into MemDC" );
	}
	ReleaseDC( hWnd, hDC );

	GetClientRect( hWnd, &rect );
	View_CalculateNewSize( hWnd, &rect, &gView.uiHeight, &gView.uiWidth );

	// Calculate the distance from our window on the world to the eye.
	gView.fEyeDist = (float)gView.uiWidth / 2.0f / tanf( ANGLE_OF_VIEW / 2.0f * DEGTORAD );
}

void View_CalculateNewSize( HWND hWnd, RECT* prc, UINT* puiHeight, UINT* puiWidth )
{
	UINT uiWidth, uiHeight;
	assert( prc != NULL );

	// Do the width as a 4:3 ratio to match the monitor.
	uiHeight = prc->bottom - prc->top + 1 - 2 * VIEW_START_POS_OFFSET;
	uiWidth = ( UINT )( 0.5f + 4.0f * ( float )uiHeight / 3.0f );

	*puiHeight = uiHeight;
	*puiWidth = uiWidth;
}


void View_UpdateSize( HWND hWnd )
{
	RECT rect;

	GetClientRect( hWnd, &rect );
	View_CalculateNewSize( hWnd, &rect, &gView.uiHeight, &gView.uiWidth );
}


void View_SwapBuffers( HDC hFrontBuffer, HDC hBackBuffer )
{
	if ( !BitBlt( hFrontBuffer, VIEW_START_POS_OFFSET, VIEW_START_POS_OFFSET, 
			gView.uiWidth, gView.uiHeight, hBackBuffer, 
			VIEW_START_POS_OFFSET, VIEW_START_POS_OFFSET, 
			SRCCOPY ) )
	{
		ErrMsg( NULL, "BitBlt failed." );
	}
}


void View_ClearBackBuffer( HWND hWnd )
{
	RECT	oldrc;

	// Get rid of the previous grid.
	oldrc.left = VIEW_START_POS_OFFSET;
	oldrc.top = VIEW_START_POS_OFFSET;
	// Adding 1 for that extra line that gets drawn. The other 1 I'm not sure ( maybe the rectangle doesn't
	// draw all the way to the end like the lines? )
	oldrc.right = gView.uiWidth + VIEW_START_POS_OFFSET+ 2;
	oldrc.bottom = gView.uiHeight + VIEW_START_POS_OFFSET+ 2;
	FillRect( gView.hDCBuffer, &oldrc, ( HBRUSH )( COLOR_WINDOW + 1 ) );
}


void View_Update( HWND hWnd, HDC hDC )
{
	// Does nothing yet.
}

void View_Shutdown( HWND hWnd )
{
	SelectObject( gView.hDCBuffer, gView.hMemBmpOld );
	DeleteObject( gView.hMemBmp );
	ReleaseDC( hWnd, gView.hDCBuffer );
}

HDC View_hDCBuffer( void )
{
	return gView.hDCBuffer;
}

UINT View_Height( void )
{
	return gView.uiHeight;
}

UINT View_Width( void )
{
	return gView.uiWidth;
}

// Multiplies the supplied world coordinates by the inverse camera matrix to get them from the point of
// view of the camera. Then scales the coordinates based on the distance from the plane representing the
// 'window' on the world that we are representing by the monitor screen.
// Note: The world coordinate frame assumes that we have x to the left, y up and z into the screen. Therefore
// the view direction of the camera is down the z-axis with x to the left and y up. We therefore need to scale all
// coordinates based on the z value,  and then plot from the centre of the screen with x pointing to the left and
// y pointing up. Doesn't do any error checking for behind the camera or out of bounds of the viewport.
void View_ConvertWorldToScreen( Vector* pVecWorld, Vector* pVecScreen )
{
	Matrix	matInvCam;
	Vector	vecScreen;
	float	fRatio;

	Camera_GetInverse( &matInvCam );
	Matrix_VectorMul( &matInvCam, pVecWorld, &vecScreen );
	// Now scale our screen coords
	// Assume for now that we have a 45 degree half fov angle so that the width of the screen is the same
	// as the distance from the 'eye' to the view window.
	fRatio = vecScreen.z != 0.0f ? gView.fEyeDist / vecScreen.z : 0.0f;
	vecScreen.x *= fRatio;
	vecScreen.y *= fRatio;
	// Now convert to screen coords where 0,0 is top left and x is to the right with y down.
	pVecScreen->x = ( float )gView.uiWidth / 2.0f - vecScreen.x;
	pVecScreen->y = ( float )gView.uiHeight / 2.0f - vecScreen.y;
	pVecScreen->z = vecScreen.z;
	pVecScreen->w = 0.0f;
}

BOOL View_ClipLineToView( Vector* pVecStart, Vector* pVecEnd,
						const float fMinX, const float fMinY,
						const float fMaxX, const float fMaxY )
{
	// The x-coords
	if ( pVecStart->x < fMinX )
	{
		pVecStart->y += ( fMinX - pVecStart->x ) / ( pVecEnd->x - pVecStart->x ) * ( pVecEnd->y - pVecStart->y );
		pVecStart->x = fMinX;
	}
	if ( pVecStart->x > fMaxX )
	{
		pVecStart->y += ( fMaxX - pVecStart->x ) / ( pVecEnd->x - pVecStart->x ) * ( pVecEnd->y - pVecStart->y );
		pVecStart->x = fMaxX;
	}
	if ( pVecEnd->x < fMinX )
	{
		pVecEnd->y += ( fMinX - pVecEnd->x ) / ( pVecEnd->x - pVecStart->x ) * ( pVecEnd->y - pVecStart->y );
		pVecEnd->x = fMinX;
	}
	if ( pVecEnd->x > fMaxX )
	{
		pVecEnd->y += ( fMaxX - pVecEnd->x ) / ( pVecEnd->x - pVecStart->x ) * ( pVecEnd->y - pVecStart->y );
		pVecEnd->x = fMaxX;
	}
	// The y-coords
	if ( pVecStart->y < fMinY )
	{
		pVecStart->x += ( fMinY - pVecStart->y ) / ( pVecEnd->y - pVecStart->y ) * ( pVecEnd->x - pVecStart->x );
		pVecStart->y = fMinY;
	}
	if ( pVecStart->y > fMaxY )
	{
		pVecStart->x += ( fMaxY - pVecStart->y ) / ( pVecEnd->y - pVecStart->y ) * ( pVecEnd->x - pVecStart->x );
		pVecStart->y = fMaxY;
	}
	if ( pVecEnd->y < fMinY )
	{
		pVecEnd->x += ( fMinY - pVecEnd->y ) / ( pVecEnd->y - pVecStart->y ) * ( pVecEnd->x - pVecStart->x );
		pVecEnd->y = fMinY;
	}
	if ( pVecEnd->y > fMaxY )
	{
		pVecEnd->x += ( fMaxY - pVecEnd->y ) / ( pVecEnd->y - pVecStart->y ) * ( pVecEnd->x - pVecStart->x );
		pVecEnd->y = fMaxY;
	}
	return TRUE;
}

void View_DrawLine( HWND hWnd, HDC hDC, Vector* pVecStart, Vector* pVecEnd )
{
	Vector vecStart;
	Vector vecEnd;

	View_ConvertWorldToScreen( pVecStart, &vecStart );
	View_ConvertWorldToScreen( pVecEnd, &vecEnd );

	if ( vecStart.z < 0.0f || vecEnd.z < 0.0f )
	{
		// Don't draw stuff behind the camera
		return;
	}

	// This function returns FALSE if the line can't be drawn because it is outside the
	// viewport.
	if ( View_ClipLineToView( &vecStart, &vecEnd, 10.0f, 10.0f, (float)gView.uiWidth - 10.0f , (float)gView.uiHeight - 10.0f ) )
	{
		MoveToEx( hDC, ( UINT )( vecStart.x + 0.5f ), ( UINT )( vecStart.y + 0.5f ), NULL );
		LineTo( hDC, ( UINT )( vecEnd.x + 0.5f ), ( UINT )( vecEnd.y + 0.5f ) );
	}
}

void	View_DrawLineRGB( HWND hWnd, HDC hDC, Vector* pVecStart, Vector* pVecEnd, COLORREF rgbValue )
{
	HPEN hPenOld;
	HPEN hPen;
	
	hPen = CreatePen( PS_SOLID, 0, rgbValue );
	hPenOld = SelectObject( hDC, hPen );
	View_DrawLine( hWnd, hDC, pVecStart, pVecEnd );
	SelectObject( hDC, hPenOld );
	DeleteObject( hPen );
}

void	View_DrawBox( HWND hWnd, HDC hDC, Vector* pVecPos, const float fX, const float fY, const float fZ )
{
	Vector vecStart, vecEnd;
	// 12 lines with centre of volume at the pVecPos point.
	// Left face ( constant x )
	Vector_Set( &vecStart, pVecPos->x - 0.5f * fX, pVecPos->y - 0.5f * fY, pVecPos->z - 0.5f * fZ, 0.0f );
	Vector_Set( &vecEnd, pVecPos->x - 0.5f * fX, pVecPos->y - 0.5f * fY, pVecPos->z + 0.5f * fZ, 0.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	Vector_Set( &vecStart, pVecPos->x - 0.5f * fX, pVecPos->y - 0.5f * fY, pVecPos->z + 0.5f *fZ, 0.0f );
	Vector_Set( &vecEnd, pVecPos->x - 0.5f * fX, pVecPos->y + 0.5f * fY, pVecPos->z + 0.5f * fZ, 0.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	Vector_Set( &vecStart, pVecPos->x - 0.5f * fX, pVecPos->y + 0.5f * fY, pVecPos->z + 0.5f * fZ, 0.0f );
	Vector_Set( &vecEnd, pVecPos->x - 0.5f * fX, pVecPos->y + 0.5f * fY, pVecPos->z - 0.5f * fZ, 0.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	Vector_Set( &vecStart, pVecPos->x - 0.5f * fX, pVecPos->y + 0.5f * fY, pVecPos->z - 0.5f * fZ, 0.0f );
	Vector_Set( &vecEnd, pVecPos->x - 0.5f * fX, pVecPos->y - 0.5f * fY, pVecPos->z - 0.5f * fZ, 0.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	// Right face ( constant x other side. )
	Vector_Set( &vecStart, pVecPos->x + 0.5f * fX, pVecPos->y - 0.5f * fY, pVecPos->z - 0.5f * fZ, 0.0f );
	Vector_Set( &vecEnd, pVecPos->x + 0.5f * fX, pVecPos->y - 0.5f * fY, pVecPos->z + 0.5f * fZ, 0.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	Vector_Set( &vecStart, pVecPos->x + 0.5f * fX, pVecPos->y - 0.5f * fY, pVecPos->z + 0.5f *fZ, 0.0f );
	Vector_Set( &vecEnd, pVecPos->x + 0.5f * fX, pVecPos->y + 0.5f * fY, pVecPos->z + 0.5f * fZ, 0.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	Vector_Set( &vecStart, pVecPos->x + 0.5f * fX, pVecPos->y + 0.5f * fY, pVecPos->z + 0.5f * fZ, 0.0f );
	Vector_Set( &vecEnd, pVecPos->x + 0.5f * fX, pVecPos->y + 0.5f * fY, pVecPos->z - 0.5f * fZ, 0.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	Vector_Set( &vecStart, pVecPos->x + 0.5f * fX, pVecPos->y + 0.5f * fY, pVecPos->z - 0.5f * fZ, 0.0f );
	Vector_Set( &vecEnd, pVecPos->x + 0.5f * fX, pVecPos->y - 0.5f * fY, pVecPos->z - 0.5f * fZ, 0.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	// Joining 4 lines from each corner of each face.
	Vector_Set( &vecStart, pVecPos->x - 0.5f * fX, pVecPos->y - 0.5f * fY, pVecPos->z - 0.5f * fZ, 0.0f );
	Vector_Set( &vecEnd, pVecPos->x + 0.5f * fX, pVecPos->y - 0.5f * fY, pVecPos->z - 0.5f * fZ, 0.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	Vector_Set( &vecStart, pVecPos->x - 0.5f * fX, pVecPos->y - 0.5f * fY, pVecPos->z + 0.5f *fZ, 0.0f );
	Vector_Set( &vecEnd, pVecPos->x + 0.5f * fX, pVecPos->y - 0.5f * fY, pVecPos->z + 0.5f * fZ, 0.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	Vector_Set( &vecStart, pVecPos->x - 0.5f * fX, pVecPos->y + 0.5f * fY, pVecPos->z + 0.5f * fZ, 0.0f );
	Vector_Set( &vecEnd, pVecPos->x + 0.5f * fX, pVecPos->y + 0.5f * fY, pVecPos->z + 0.5f * fZ, 0.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	Vector_Set( &vecStart, pVecPos->x - 0.5f * fX, pVecPos->y + 0.5f * fY, pVecPos->z - 0.5f * fZ, 0.0f );
	Vector_Set( &vecEnd, pVecPos->x + 0.5f * fX, pVecPos->y + 0.5f * fY, pVecPos->z - 0.5f * fZ, 0.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
}

void View_DrawBoxRGB( HWND hWnd, HDC hDC, Vector* pVecPos, 
						const float fX, const float fY, const float fZ, COLORREF rgbValue )
{
	HPEN hPenOld;
	HPEN hPen;
	
	hPen = CreatePen( PS_SOLID, 0, rgbValue );
	hPenOld = SelectObject( hDC, hPen );
	View_DrawBox( hWnd, hDC, pVecPos, fX, fY, fZ );
	SelectObject( hDC, hPenOld );
	DeleteObject( hPen );
}

void View_DrawReflectedPoints( HWND hWnd, HDC hDC, Vector* pVecPos, 
								float fX, float fY, float fZ, float fX1, float fY1, float fZ1 )
{
	Vector vecStart, vecEnd;

	// +/- X on one side of the x-axis
	Vector_Set( &vecStart, pVecPos->x + fX, pVecPos->y + fY, pVecPos->z + fZ, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x + fX1, pVecPos->y + fY1, pVecPos->z + fZ1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	Vector_Set( &vecStart, pVecPos->x - fX, pVecPos->y + fY, pVecPos->z + fZ, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x - fX1, pVecPos->y + fY1, pVecPos->z + fZ1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	// +/- X on the negative side of the x-axis
	Vector_Set( &vecStart, pVecPos->x + fX, pVecPos->y + fY, pVecPos->z - fZ, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x + fX1, pVecPos->y + fY1, pVecPos->z - fZ1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	Vector_Set( &vecStart, pVecPos->x - fX, pVecPos->y + fY, pVecPos->z - fZ, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x - fX1, pVecPos->y + fY1, pVecPos->z - fZ1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	// +/- Z on one side of the z-axis
	Vector_Set( &vecStart, pVecPos->x + fZ, pVecPos->y + fY, pVecPos->z + fX, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x + fZ1, pVecPos->y + fY1, pVecPos->z + fX1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	Vector_Set( &vecStart, pVecPos->x - fZ, pVecPos->y + fY, pVecPos->z + fX, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x - fZ1, pVecPos->y + fY1, pVecPos->z + fX1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	// -/- Z on the other side of the z-axis
	Vector_Set( &vecStart, pVecPos->x + fZ, pVecPos->y + fY, pVecPos->z - fX, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x + fZ1, pVecPos->y + fY1, pVecPos->z - fX1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	Vector_Set( &vecStart, pVecPos->x - fZ, pVecPos->y + fY, pVecPos->z - fX, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x - fZ1, pVecPos->y + fY1, pVecPos->z - fX1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	// The above but below the y-axis
	Vector_Set( &vecStart, pVecPos->x + fX, pVecPos->y - fY, pVecPos->z + fZ, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x + fX1, pVecPos->y - fY1, pVecPos->z + fZ1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	Vector_Set( &vecStart, pVecPos->x - fX, pVecPos->y - fY, pVecPos->z + fZ, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x - fX1, pVecPos->y - fY1, pVecPos->z + fZ1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	// The above but below the y-axis
	Vector_Set( &vecStart, pVecPos->x + fX, pVecPos->y - fY, pVecPos->z - fZ, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x + fX1, pVecPos->y - fY1, pVecPos->z - fZ1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	Vector_Set( &vecStart, pVecPos->x - fX, pVecPos->y - fY, pVecPos->z - fZ, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x - fX1, pVecPos->y - fY1, pVecPos->z - fZ1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	// The above but below the y-axis
	Vector_Set( &vecStart, pVecPos->x + fZ, pVecPos->y - fY, pVecPos->z + fX, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x + fZ1, pVecPos->y - fY1, pVecPos->z + fX1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	Vector_Set( &vecStart, pVecPos->x - fZ, pVecPos->y - fY, pVecPos->z + fX, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x - fZ1, pVecPos->y - fY1, pVecPos->z + fX1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	// The above but below the y-axis
	Vector_Set( &vecStart, pVecPos->x + fZ, pVecPos->y - fY, pVecPos->z - fX, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x + fZ1, pVecPos->y - fY1, pVecPos->z - fX1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	Vector_Set( &vecStart, pVecPos->x - fZ, pVecPos->y - fY, pVecPos->z - fX, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x - fZ1, pVecPos->y - fY1, pVecPos->z - fX1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	// Now the horizontal circles.
	Vector_Set( &vecStart, pVecPos->x + fX, pVecPos->y + fZ, pVecPos->z + fY, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x + fX1, pVecPos->y + fZ1, pVecPos->z + fY1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	Vector_Set( &vecStart, pVecPos->x - fX, pVecPos->y + fZ, pVecPos->z + fY, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x - fX1, pVecPos->y + fZ1, pVecPos->z + fY1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );

	Vector_Set( &vecStart, pVecPos->x + fX, pVecPos->y + fZ, pVecPos->z - fY, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x + fX1, pVecPos->y + fZ1, pVecPos->z - fY1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	Vector_Set( &vecStart, pVecPos->x - fX, pVecPos->y + fZ, pVecPos->z - fY, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x - fX1, pVecPos->y + fZ1, pVecPos->z - fY1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );

	Vector_Set( &vecStart, pVecPos->x + fY, pVecPos->y + fZ, pVecPos->z + fX, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x + fY1, pVecPos->y + fZ1, pVecPos->z + fX1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	Vector_Set( &vecStart, pVecPos->x - fY, pVecPos->y + fZ, pVecPos->z + fX, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x - fY1, pVecPos->y + fZ1, pVecPos->z + fX1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );

	Vector_Set( &vecStart, pVecPos->x + fY, pVecPos->y + fZ, pVecPos->z - fX, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x + fY1, pVecPos->y + fZ1, pVecPos->z - fX1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	Vector_Set( &vecStart, pVecPos->x - fY, pVecPos->y + fZ, pVecPos->z - fX, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x - fY1, pVecPos->y + fZ1, pVecPos->z - fX1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );

	Vector_Set( &vecStart, pVecPos->x + fX, pVecPos->y - fZ, pVecPos->z + fY, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x + fX1, pVecPos->y - fZ1, pVecPos->z + fY1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	Vector_Set( &vecStart, pVecPos->x - fX, pVecPos->y - fZ, pVecPos->z + fY, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x - fX1, pVecPos->y - fZ1, pVecPos->z + fY1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );

	Vector_Set( &vecStart, pVecPos->x + fX, pVecPos->y - fZ, pVecPos->z - fY, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x + fX1, pVecPos->y - fZ1, pVecPos->z - fY1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	Vector_Set( &vecStart, pVecPos->x - fX, pVecPos->y - fZ, pVecPos->z - fY, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x - fX1, pVecPos->y - fZ1, pVecPos->z - fY1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );

	Vector_Set( &vecStart, pVecPos->x + fY, pVecPos->y - fZ, pVecPos->z + fX, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x + fY1, pVecPos->y - fZ1, pVecPos->z + fX1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	Vector_Set( &vecStart, pVecPos->x - fY, pVecPos->y - fZ, pVecPos->z + fX, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x - fY1, pVecPos->y - fZ1, pVecPos->z + fX1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );

	Vector_Set( &vecStart, pVecPos->x + fY, pVecPos->y - fZ, pVecPos->z - fX, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x + fY1, pVecPos->y - fZ1, pVecPos->z - fX1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
	Vector_Set( &vecStart, pVecPos->x - fY, pVecPos->y - fZ, pVecPos->z - fX, 1.0f );
	Vector_Set( &vecEnd, pVecPos->x - fY1, pVecPos->y - fZ1, pVecPos->z - fX1, 1.0f );
	View_DrawLine( hWnd, hDC, &vecStart, &vecEnd );
}

void View_DrawSphere( HWND hWnd, HDC hDC, Vector* pVecPos, float fRadius )
{
	float fX, fY, fZ;
	float fX1, fY1, fZ1;

	// A sphere consisting of points at 0, 30, 60, 90 etc degrees around the clock. Instead of sin and cos,
	// am using the known values at these angles.
	// Central two circles.
	fX = fRadius;									fY = 0.0f;								fZ = 0.0f;
	fX1 = fRadius * ROOT3OVER2;					fY1 = fRadius * 0.5f;						fZ1 = 0.0f;
	View_DrawReflectedPoints( hWnd, hDC, pVecPos, fX, fY, fZ, fX1, fY1, fZ1 );
	fX = fRadius * ROOT3OVER2;					fY = fRadius * 0.5f;						fZ = 0.0f;
	fX1 = fRadius * 0.5f;								fY1 = fRadius * ROOT3OVER2;			fZ1 = 0.0f;
	View_DrawReflectedPoints( hWnd, hDC, pVecPos, fX, fY, fZ, fX1, fY1, fZ1 );
	fX = fRadius * 0.5f;								fY = fRadius * ROOT3OVER2;			fZ = 0.0f;
	fX1 = 0.0f;										fY1 = fRadius;							fZ1 = 0.0f;
	View_DrawReflectedPoints( hWnd, hDC, pVecPos, fX, fY, fZ, fX1, fY1, fZ1 );
	fX = fRadius * ROOT3OVER2;					fY = 0.0f;										fZ = fRadius * 0.5f;
	fX1 = fRadius * ROOT3OVER2 * ROOT3OVER2;	fY1 = fRadius * ROOT3OVER2 * 0.5f;				fZ1 = fRadius * 0.5f;
	View_DrawReflectedPoints( hWnd, hDC, pVecPos, fX, fY, fZ, fX1, fY1, fZ1 );
	fX = fRadius * ROOT3OVER2 * ROOT3OVER2;	fY = fRadius * ROOT3OVER2 * 0.5f;				fZ = fRadius * 0.5f;
	fX1 = fRadius * ROOT3OVER2 * 0.5f;				fY1 = fRadius * ROOT3OVER2 * ROOT3OVER2;	fZ1 = fRadius * 0.5f;
	View_DrawReflectedPoints( hWnd, hDC, pVecPos, fX, fY, fZ, fX1, fY1, fZ1 );
	fX = fRadius * ROOT3OVER2 * 0.5f;				fY = fRadius * ROOT3OVER2 * ROOT3OVER2;	fZ = fRadius * 0.5f;
	fX1 = 0.0f;										fY1 = fRadius * ROOT3OVER2;					fZ1 = fRadius * 0.5f;
	View_DrawReflectedPoints( hWnd, hDC, pVecPos, fX, fY, fZ, fX1, fY1, fZ1 );
}

// A step towards creating lists of vertices and faces. Calculate the sphere in terms of triangular faces.
void View_DrawSphereTriangles( HWND hWnd, HDC hDC, Vector pVecPos, float fRadius )
{
}

void View_DrawSphereRGB( HWND hWnd, HDC hDC, Vector* pVecPos, float fRadius, COLORREF rgbValue )
{
	HPEN hPenOld;
	HPEN hPen;
	
	hPen = CreatePen( PS_SOLID, 0, rgbValue );
	hPenOld = SelectObject( hDC, hPen );
	View_DrawSphere( hWnd, hDC, pVecPos, fRadius );
	SelectObject( hDC, hPenOld );
	DeleteObject( hPen );
}
