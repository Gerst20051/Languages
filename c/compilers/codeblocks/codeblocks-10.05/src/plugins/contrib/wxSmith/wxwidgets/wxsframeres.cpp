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
* $Id: wxsframeres.cpp 4850 2008-01-29 21:45:49Z byo $
* $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/wxSmith/wxwidgets/wxsframeres.cpp $
*/

#include "wxsframeres.h"
#include "wxsitemresdata.h"
#include "wxsflags.h"
#include <wx/button.h>

using namespace wxsFlags;

namespace
{
    class wxsFrameResPreview: public wxFrame
    {
        public:

            wxsFrameResPreview(wxWindow* Parent,wxsItemResData* Data): m_Data(Data)
            {
                m_Data->GetRootItem()->BuildPreview(this,pfExact);
                wxAcceleratorEntry Acc[1];
                Acc[0].Set(wxACCEL_NORMAL,WXK_ESCAPE,wxID_EXIT);
                wxAcceleratorTable Table(1,Acc);
                SetAcceleratorTable(Table);
            }

            ~wxsFrameResPreview()
            {
                m_Data->NotifyPreviewClosed();
            }

            void OnEscape(wxCommandEvent& event)
            {
                Close();
            }

            void OnClose(wxCloseEvent& event)
            {
                Destroy();
            }

            void OnButton(wxCommandEvent& event)
            {
                wxWindowID Id = event.GetId();
                if ( Id == wxID_OK  ||
                     Id == wxID_APPLY ||
                     Id == wxID_CANCEL )
                {
                    Close();
                }
            }

            wxsItemResData* m_Data;

            DECLARE_EVENT_TABLE()
    };

    BEGIN_EVENT_TABLE(wxsFrameResPreview,wxFrame)
        EVT_MENU(wxID_EXIT,wxsFrameResPreview::OnEscape)
        EVT_CLOSE(wxsFrameResPreview::OnClose)
        EVT_BUTTON(wxID_ANY,wxsFrameResPreview::OnButton)
    END_EVENT_TABLE()
}

const wxString wxsFrameRes::ResType = _T("wxFrame");

wxString wxsFrameRes::OnGetAppBuildingCode()
{
    return wxString::Format(
        _T("\t%s* Frame = new %s(0);\n")
        _T("\tFrame->Show();\n")
        _T("\tSetTopWindow(Frame);\n"),
            GetResourceName().c_str(),
            GetResourceName().c_str());
}

wxWindow* wxsFrameRes::OnBuildExactPreview(wxWindow* Parent,wxsItemResData* Data)
{
    wxFrame* Frm = new wxsFrameResPreview(Parent,Data);
    Frm->Show();
    return Frm;
}
