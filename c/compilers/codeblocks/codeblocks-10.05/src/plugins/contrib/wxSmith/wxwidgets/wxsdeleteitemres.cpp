/*
* This file is part of wxSmith plugin for Code::Blocks Studio
* Copyright (C) 2007  Bartlomiej Swiecki
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
* $Revision: 6104 $
* $Id: wxsdeleteitemres.cpp 6104 2010-01-23 12:56:12Z mortenmacfly $
* $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/wxSmith/wxwidgets/wxsdeleteitemres.cpp $
*/

#include "wxsdeleteitemres.h"

//(*InternalHeaders(wxsDeleteItemRes)
#include <wx/button.h>
#include <wx/string.h>
#include <wx/intl.h>
//*)

//(*IdInit(wxsDeleteItemRes)
const long wxsDeleteItemRes::ID_CHECKBOX1 = wxNewId();
const long wxsDeleteItemRes::ID_CHECKBOX2 = wxNewId();
const long wxsDeleteItemRes::ID_CHECKBOX3 = wxNewId();
const long wxsDeleteItemRes::ID_STATICTEXT1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(wxsDeleteItemRes,wxScrollingDialog)
	//(*EventTable(wxsDeleteItemRes)
	//*)
END_EVENT_TABLE()

wxsDeleteItemRes::wxsDeleteItemRes()
{
	//(*Initialize(wxsDeleteItemRes)
	Create(0, wxID_ANY, _("Deleting resource"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, this, _("Delete options"));
	m_PhisDeleteWXS = new wxCheckBox(this, ID_CHECKBOX1, _("Physically delete WXS file"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
	m_PhisDeleteWXS->SetValue(true);
	StaticBoxSizer1->Add(m_PhisDeleteWXS, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_DeleteSources = new wxCheckBox(this, ID_CHECKBOX2, _("Delete source files from project"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
	m_DeleteSources->SetValue(false);
	StaticBoxSizer1->Add(m_DeleteSources, 0, wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_PhisDeleteSources = new wxCheckBox(this, ID_CHECKBOX3, _("Phisically delete source files"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX3"));
	m_PhisDeleteSources->SetValue(false);
	m_PhisDeleteSources->Disable();
	StaticBoxSizer1->Add(m_PhisDeleteSources, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Note that there\'s no undo\nafter deleting resource"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_STATICTEXT1"));
	StaticBoxSizer1->Add(StaticText1, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(StaticBoxSizer1, 1, wxTOP|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StdDialogButtonSizer1 = new wxStdDialogButtonSizer();
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_CANCEL, wxEmptyString));
	StdDialogButtonSizer1->Realize();
	BoxSizer1->Add(StdDialogButtonSizer1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	
	Connect(ID_CHECKBOX2,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&wxsDeleteItemRes::Onm_DeleteSourcesClick);
	//*)
}

wxsDeleteItemRes::~wxsDeleteItemRes()
{
	//(*Destroy(wxsDeleteItemRes)
	//*)
}


void wxsDeleteItemRes::Onm_DeleteSourcesClick(wxCommandEvent& event)
{
    m_PhisDeleteSources->Enable(m_DeleteSources->GetValue());
}
