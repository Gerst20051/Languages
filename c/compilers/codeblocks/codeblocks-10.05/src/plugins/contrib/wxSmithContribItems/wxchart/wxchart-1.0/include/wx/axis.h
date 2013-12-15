/////////////////////////////////////////////////////////////////////////////
// Name:        axis.h
// Purpose:     wxChart
// Author:      Paolo Gava
// Modified by:
// Created:
// Copyright:   (C) 2006, Paolo Gava
// RCS-ID:      $Id: axis.h 5987 2009-12-29 08:24:01Z killerbot $
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if !defined( __AXIS_H__ )
#define __AXIS_H__

//----------------------------------------------------------------------------
// Headers
//----------------------------------------------------------------------------
#include "wx/charttypes.h"

// External Classes
//-----------------
class wxChartSizes;

//+++-S-cd-------------------------------------------------------------------
//	NAME:		wxAxis
//	DESC:		
//	INTERFACE:
//
//----------------------------------------------------------------------E-+++
class WXDLLIMPEXP_CHART wxAxis
{
public:
	wxAxis(ChartValue max = 0, ChartValue min = 0);
	virtual ~wxAxis() {};

	// Draw xaxis area
	//-----------------
	virtual void Draw(CHART_HPAINT hp, CHART_HRECT hr) = 0;

	// Get/Set virtual size
	//---------------------
	virtual void SetVirtualMax(ChartValue v);
	virtual void SetVirtualMin(ChartValue v);
	virtual ChartValue GetVirtualMax() const;
	virtual ChartValue GetVirtualMin() const;

	// Get/Set zoom
	//-------------
	virtual void SetZoom(double z);
	virtual double GetZoom();

	// Set sizes for drawing
	//----------------------
	virtual void SetSizes(wxChartSizes *sizes);
	virtual wxChartSizes* GetSizes() const;

private:
	ChartValue m_vMax;
	ChartValue m_vMin;
	double m_Zoom;
	wxChartSizes *m_Sizes;
};

#endif // __XAXIS_H__
