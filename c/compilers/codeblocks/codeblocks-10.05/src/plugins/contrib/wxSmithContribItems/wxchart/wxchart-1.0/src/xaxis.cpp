/////////////////////////////////////////////////////////////////////////////
// Name:        xaxis.cpp
// Purpose:     wxChart
// Author:      Paolo Gava
// Modified by:
// Created:
// Copyright:   (C) 2006, Paolo Gava
// RCS-ID:      $Id: xaxis.cpp 5961 2009-12-08 06:24:32Z mortenmacfly $
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// Headers
//----------------------------------------------------------------------------

// c++
#include <cmath>

// wx
#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "xaxis.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "wx/xaxis.h"
#include "wx/chartsizes.h"

//+++-S-cf-------------------------------------------------------------------
//	NAME:		Draw()
//	DESC:		Draw xaxis
//	PARAMETERS:	CHART_HPAINT hp, 
//				CHART_HRECT hr
//	RETURN:		None
//----------------------------------------------------------------------E-+++
void wxXAxis::Draw(
	CHART_HPAINT hp, 
	CHART_HRECT hr
)
{
    
	if ( GetVirtualMax() > 0 )
	{
		wxFont font(8, wxROMAN, wxNORMAL, wxNORMAL);
        hp->SetFont(font);
        hp->SetPen( *wxBLACK_PEN );
		
        int iNodes = static_cast<int>(ceil( GetVirtualMax() ));
		wxChartSizes *sizes = GetSizes();
		
		double x;
		
		//-------------------------------------------------------------------
		// If hr->x != 0 means we are drawing the axis on file. So the 
		// following condition prevent from drawing only part of the axis ie
		// ignor any scroll position
		// TODO: any better idea?!
		//-------------------------------------------------------------------
		if ( hr->x == 0 )
		{
            hr->xscroll *= sizes->GetScroll();
			x = 0 - hr->xscroll;
		}
		else
			x = 0;
		
		for ( int iNode = 0; iNode <= iNodes; ++ iNode )
		{
			if ( x >= 0 )
			{
				wxString label;
				
                hp->DrawLine( static_cast<int>(ceil(x)) + hr->x, 5 + hr->y, 
                              static_cast<int>(ceil(x)) + hr->x, 15 + hr->y );
				
                label.Printf( wxT("%d"), iNode );
                hp->DrawText( label, static_cast<int>(ceil(x)) +
                              hr->x, 20 + hr->y );
			}
            x +=  GetZoom() * ( sizes->GetWidthBar() * sizes->GetNumBar() + 
                    sizes->GetWidthBar3d() * sizes->GetNumBar3d() +
                    sizes->GetGap() );
		}
		
        hp->DrawLine( hr->x, hr->y + 1, hr->x + static_cast<int>(ceil(x)), 
                      hr->y + 1 );
		
	}
}
