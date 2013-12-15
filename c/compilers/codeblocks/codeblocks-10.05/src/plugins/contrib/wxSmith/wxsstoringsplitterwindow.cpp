/*
* This file is part of wxSmith plugin for Code::Blocks Studio
* Copyright (C) 2006-2007  Bartlomiej Swiecki
*
* wxSmith is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* wxSmith is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with wxSmith. If not, see <http://www.gnu.org/licenses/>.
*
* $Revision: 4850 $
* $Id: wxsstoringsplitterwindow.cpp 4850 2008-01-29 21:45:49Z byo $
* $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/wxSmith/wxsstoringsplitterwindow.cpp $
*/

#include "wxsstoringsplitterwindow.h"

#include <wx/sizer.h>

/** \brief Minimal size of each sub panel in pixels */
#define MIN_MARGIN  20

#include <manager.h>
#include <logmanager.h>
#include <configmanager.h>

static const wxWindowID SplitterId = wxNewId();

wxsStoringSplitterWindow::wxsStoringSplitterWindow(wxWindow* Parent): wxPanel(Parent)
{
    wxBoxSizer* Sizer = new wxBoxSizer(wxVERTICAL);
    Sizer->Add( Splitter = new wxSplitterWindow(this,SplitterId), 1, wxGROW|wxALL );
    SetSizer(Sizer);
    SplitPosition = Manager::Get()->GetConfigManager(_T("wxsmith"))->ReadInt(_T("/res_panel_split"),-1);
}

void wxsStoringSplitterWindow::Split(wxWindow* Top,wxWindow* Bottom,int SashPosition)
{
    if ( Top ) Top->SetParent(Splitter);
    if ( Bottom ) Bottom->SetParent(Splitter);
    if ( SashPosition ) SplitPosition = SashPosition;
    Splitter->SplitHorizontally(Top,Bottom,SplitterFixup(SplitPosition));
}

void wxsStoringSplitterWindow::OnSize(wxSizeEvent& event)
{
	Splitter->SetSashPosition(SplitterFixup(SplitPosition));
	event.Skip();
}

void wxsStoringSplitterWindow::OnSplitterChanging(wxSplitterEvent& event)
{
	SplitPosition = event.GetSashPosition();
	event.SetSashPosition(SplitterFixup(SplitPosition));
}

void wxsStoringSplitterWindow::OnSplitterChanged(wxSplitterEvent& event)
{
    // We use value which was previously set through OnSplitterChanging()
    // This will avoid hiding top panel when docking managment window
    event.SetSashPosition(SplitterFixup(SplitPosition));
    Manager::Get()->GetConfigManager(_T("wxsmith"))->Write(_T("/res_panel_split"),(int)SplitPosition);
}

int wxsStoringSplitterWindow::SplitterFixup(int Position)
{
    int Height = GetSize().GetHeight();
	int MinMargin = Height / 2;

    if ( Position<0 || Position>Height ) return Height/2;

	if ( MinMargin > MIN_MARGIN ) MinMargin = MIN_MARGIN;
	if ( Position < MinMargin ) Position = MinMargin;
	if ( Position > Height - MinMargin ) Position = Height - MinMargin;

	return Position;
}

BEGIN_EVENT_TABLE(wxsStoringSplitterWindow,wxPanel)
    EVT_SIZE(wxsStoringSplitterWindow::OnSize)
    EVT_SPLITTER_SASH_POS_CHANGING(SplitterId,wxsStoringSplitterWindow::OnSplitterChanging)
    EVT_SPLITTER_SASH_POS_CHANGED(SplitterId,wxsStoringSplitterWindow::OnSplitterChanged)
END_EVENT_TABLE()
