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
* $Id: wxsdeleteitemres.h 6104 2010-01-23 12:56:12Z mortenmacfly $
* $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/wxSmith/wxwidgets/wxsdeleteitemres.h $
*/

#ifndef WXSDELETEITEMRES_H
#define WXSDELETEITEMRES_H

//(*Headers(wxsDeleteItemRes)
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include "scrollingdialog.h"
#include <wx/stattext.h>
//*)

class wxsDeleteItemRes: public wxScrollingDialog
{
	public:

		wxsDeleteItemRes();
		virtual ~wxsDeleteItemRes();

		//(*Identifiers(wxsDeleteItemRes)
		static const long ID_CHECKBOX1;
		static const long ID_CHECKBOX2;
		static const long ID_CHECKBOX3;
		static const long ID_STATICTEXT1;
		//*)

		//(*Handlers(wxsDeleteItemRes)
		void Onm_DeleteSourcesClick(wxCommandEvent& event);
		//*)

		//(*Declarations(wxsDeleteItemRes)
		wxCheckBox* m_PhisDeleteWXS;
		wxStaticText* StaticText1;
		wxBoxSizer* BoxSizer1;
		wxCheckBox* m_PhisDeleteSources;
		wxStdDialogButtonSizer* StdDialogButtonSizer1;
		wxStaticBoxSizer* StaticBoxSizer1;
		wxCheckBox* m_DeleteSources;
		//*)

	private:

		DECLARE_EVENT_TABLE()
};

#endif
