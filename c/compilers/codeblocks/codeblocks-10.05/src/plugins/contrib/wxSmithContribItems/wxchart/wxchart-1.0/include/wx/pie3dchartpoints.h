/////////////////////////////////////////////////////////////////////////////
// Name:        pie3dcharpoints.h
// Purpose:     wxChart
// Author:      Paolo Gava
// Modified by:
// Created:
// Copyright:   (C) 2006, Paolo Gava
// RCS-ID:      $Id: pie3dchartpoints.h 5987 2009-12-29 08:24:01Z killerbot $
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if !defined( __PIE3DCHARTPOINTS_H__ )
#define __PIE3DCHARTPOINTS_H__

//----------------------------------------------------------------------------
// Headers
//----------------------------------------------------------------------------
#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "pie3dchartpoints.h"
#endif

#include "wx/points.h"
#include "wx/chartcolors.h"
#include "wx/chartpoints.h"

// External Classes
//-----------------
class wxChartSizes;

//+++-S-cd-------------------------------------------------------------------
//	NAME:		wxPie3DChartPoints
//	DESC:		Definition of pie chart
//	INTERFACE:
//
//----------------------------------------------------------------------E-+++
class WXDLLIMPEXP_CHART wxPie3DChartPoints : public wxChartPoints
{
public:

    // Create object. This class has to be created on the heap
    // because CChart needs its base pointer to create a list
    // example:
    // {
    //   CPieChartPoints bcp;
    //   CChart c;
    //   c.Add( &bcp );
    // }
    // If this is allow after the bcp is out of scope the list has
    // a pointer which has been deallocated!
    //------------------------------------------------------------
    static wxPie3DChartPoints* CreateWxPie3DChartPoints(wxString name, 
            ChartColor c = wxCHART_NOCOLOR, bool showlabel = false); 

	virtual ~wxPie3DChartPoints() {};

	// Draw the series of points
	//--------------------------
	void Draw(CHART_HPAINT hp, CHART_HRECT hr);

	// Get n-th point information
	//---------------------------
	ChartValue GetXVal(int n) const;
	ChartValue GetYVal(int n) const;
    wxString GetName(int n) const;
	ChartColor GetColor(int n) const;

	// Get stat values
	//----------------
	int GetCount() const;
	ChartValue GetMaxX() const;
	ChartValue GetMaxY() const;
	ChartValue GetMinX() const;
	ChartValue GetMinY() const;

	// Get/Set zoom
	//-------------
	void SetZoom(double z);
	double GetZoom();

	// Set sizes for drawing
	//----------------------
	void SetSizes(wxChartSizes *sizes);
	wxChartSizes* GetSizes() const;

	// Get/Set Color
	//--------------
	ChartColor GetColor() const;
	void SetColor(ChartColor c);

	// Get/Set Name
	//--------------
	wxString GetName() const;
	void SetName(wxString name);

	// Add point
	//----------
	void Add(wxString name, ChartValue x, ChartValue y);
	void Add(wxString name, ChartValue x, ChartValue y, 
			 ChartColor c);

	// Set/Get Display option
	//-----------------------
	void SetDisplayTag(wxDISPLAY_LABEL d);
	wxDISPLAY_LABEL GetDisplayTag() const;

private:
	wxPoints m_Points;
	wxString m_Name;
	ChartColor m_Color;
	double m_Zoom;
	wxDISPLAY_LABEL m_PieTag;
    bool m_ShowLabel;   
	wxChartSizes *m_Sizes;
    
    // ctor
    // has to be created on the heap!
    //-------------------------------
    wxPie3DChartPoints(wxString name, ChartColor c, bool showlabel);

	// copy ctor & op= NOT allow
	//--------------------------
	wxPie3DChartPoints(const wxPie3DChartPoints&);
	wxPie3DChartPoints& operator=(const wxPie3DChartPoints&);
};


#endif // __PIE3DCHARTPOINTS_H__
