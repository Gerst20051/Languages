// View.h
// Functions for manipulating world coordinates and view coordinates and drawing objects to the screen.
// Created 29/09/05 by Nick Oakley
#ifndef VIEW_H
#define VIEW_H

#include "ViewDefs.h"
#include "Vector.h"
#include "Matrix.h"
#include "Camera.h"


// Prototypes
void	View_Init( HWND hWnd );
void	View_Shutdown( HWND hWnd );
HDC	View_hDCBuffer( void );
UINT	View_Height( void );
UINT	View_Width( void );

void	View_ClearBackBuffer( HWND hWnd );
void	View_UpdateSize( HWND hWnd );
void	View_SwapBuffers( HDC hFrontBuffer, HDC hBackBuffer );

void	View_CalculateNewSize( HWND hWnd, RECT* prc, UINT* puiHeight, UINT* puiWidth );
void	View_ConvertWorldToScreen( Vector* pVecWorld, Vector* pVecScreen );

BOOL	View_ClipLineToView( Vector* pVecStart, Vector* pVecEnd,
						const float fMinX, const float fMinY,
						const float fMaxX, const float fMaxY );
void	View_DrawLine( HWND hWnd, HDC hDC, Vector* pVecStart, Vector* pVecEnd );
void	View_DrawBox( HWND hWnd, HDC hDC, Vector* pVecPos, const float fX, const float fY, const float fZ );

void	View_DrawLineRGB( HWND hWnd, HDC hDC, Vector* pVecStart, Vector* pVecEnd, COLORREF rgbValue );
void	View_DrawBoxRGB( HWND hWnd, HDC hDC, Vector* pVecPos, 
							const float fX, const float fY, const float fZ, COLORREF rgbValue );
void	View_DrawSphere( HWND hWnd, HDC hDC, Vector* pVecPos, float fRadius );
void	View_DrawSphereRGB( HWND hWnd, HDC hDC, Vector* pVecPos, float fRadius, COLORREF rgbValue );

#endif // VIEW_H

