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
* $Revision: 6104 $
* $Id: wxsarraystringcheckeditordlg.h 6104 2010-01-23 12:56:12Z mortenmacfly $
* $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/wxSmith/wxwidgets/properties/wxsarraystringcheckeditordlg.h $
*/

#ifndef WXSARRAYSTRINGCHECKEDITORDLG_H
#define WXSARRAYSTRINGCHECKEDITORDLG_H

#include "wxsarraystringcheckproperty.h"

//(*Headers(wxsArrayStringCheckEditorDlg)
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/checklst.h>
#include <wx/statline.h>
#include <wx/button.h>
#include "scrollingdialog.h"
//*)


class wxsArrayStringCheckEditorDlg: public wxScrollingDialog
{
	public:

		wxsArrayStringCheckEditorDlg(wxWindow* parent,wxArrayString& Strings,wxArrayBool& Bools,wxWindowID id = -1);
		virtual ~wxsArrayStringCheckEditorDlg();

		//(*Identifiers(wxsArrayStringCheckEditorDlg)
		static const long ID_TEXTCTRL1;
		static const long ID_BUTTON1;
		static const long ID_STATICLINE1;
		static const long ID_CHECKLISTBOX1;
		static const long ID_BUTTON2;
		static const long ID_BUTTON4;
		static const long ID_BUTTON3;
		static const long ID_BUTTON5;
		static const long ID_STATICLINE2;
		static const long ID_BUTTON6;
		static const long ID_BUTTON7;
		//*)

	protected:

		//(*Handlers(wxsArrayStringCheckEditorDlg)
		void OnButton1Click(wxCommandEvent& event);
		void OnButton2Click(wxCommandEvent& event);
		void OnButton4Click(wxCommandEvent& event);
		void OnButton3Click(wxCommandEvent& event);
		void OnButton5Click(wxCommandEvent& event);
		void OnButton6Click(wxCommandEvent& event);
		void OnButton7Click(wxCommandEvent& event);
		void OnStringListToggled(wxCommandEvent& event);
		//*)

		//(*Declarations(wxsArrayStringCheckEditorDlg)
		wxBoxSizer* BoxSizer4;
		wxButton* Button4;
		wxBoxSizer* BoxSizer5;
		wxButton* Button1;
		wxBoxSizer* BoxSizer2;
		wxButton* Button2;
		wxStaticLine* StaticLine2;
		wxButton* Button6;
		wxButton* Button5;
		wxButton* Button3;
		wxButton* Button7;
		wxStaticLine* StaticLine1;
		wxCheckListBox* StringList;
		wxBoxSizer* BoxSizer1;
		wxTextCtrl* EditArea;
		wxBoxSizer* BoxSizer3;
		//*)

	private:

        wxArrayString& Strings;
        wxArrayBool& Bools;

		DECLARE_EVENT_TABLE()
};

#endif
