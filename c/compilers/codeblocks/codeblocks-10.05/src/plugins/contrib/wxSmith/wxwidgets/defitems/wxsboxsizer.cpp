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
* $Id: wxsboxsizer.cpp 4850 2008-01-29 21:45:49Z byo $
* $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/wxSmith/wxwidgets/defitems/wxsboxsizer.cpp $
*/

#include "wxsboxsizer.h"

namespace
{
    wxsRegisterItem<wxsBoxSizer> Reg(_T("BoxSizer"), wxsTSizer, _T("Layout"), 70);
}

wxsBoxSizer::wxsBoxSizer(wxsItemResData* Data):
    wxsSizer(Data,&Reg.Info),
    Orient(wxHORIZONTAL)
{
}

wxSizer* wxsBoxSizer::OnBuildSizerPreview(wxWindow* Parent)
{
    return new wxBoxSizer(Orient);
}

void wxsBoxSizer::OnBuildSizerCreatingCode()
{
    switch ( GetLanguage() )
    {
        case wxsCPP:
        {
            AddHeader(_T("<wx/sizer.h>"),GetInfo().ClassName,hfInPCH);
            Codef(_T("%C(%s);\n"),(Orient!=wxHORIZONTAL)?_T("wxVERTICAL"):_T("wxHORIZONTAL"));
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsBoxSizer::OnBuildSizerCreatingCode"),GetLanguage());
        }
    }
}

void wxsBoxSizer::OnEnumSizerProperties(long Flags)
{
    static const long    OrientValues[] = { wxHORIZONTAL, wxVERTICAL, 0 };
    static const wxChar* OrientNames[]  = { _T("wxHORIZONTAL"), _T("wxVERTICAL"), 0 };
    WXS_ENUM(wxsBoxSizer,Orient,_("Orientation"),_T("orient"),OrientValues,OrientNames,wxHORIZONTAL);
}
