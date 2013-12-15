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
* $Revision: 5796 $
* $Id: wxsstaticboxsizer.cpp 5796 2009-09-16 12:09:22Z mortenmacfly $
* $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/wxSmith/wxwidgets/defitems/wxsstaticboxsizer.cpp $
*/

#include "wxsstaticboxsizer.h"

namespace
{
    wxsRegisterItem<wxsStaticBoxSizer> Reg(_T("StaticBoxSizer"),wxsTSizer,_T("Layout"),40);
}


wxsStaticBoxSizer::wxsStaticBoxSizer(wxsItemResData* Data):
    wxsSizer(Data,&Reg.Info),
    Orient(wxHORIZONTAL),
    Label(_("Label"))
{
}


wxSizer* wxsStaticBoxSizer::OnBuildSizerPreview(wxWindow* Parent)
{
    return new wxStaticBoxSizer(Orient,Parent,Label);
}

void wxsStaticBoxSizer::OnBuildSizerCreatingCode()
{
    switch ( GetLanguage() )
    {
        case wxsCPP:
        {
            AddHeader(_T("<wx/sizer.h>"),GetInfo().ClassName,hfInPCH);
            Codef(_T("%C(%s, %W, %t);\n"),
                    (Orient!=wxHORIZONTAL)?_T("wxVERTICAL"):_T("wxHORIZONTAL"),
                    #if wxCHECK_VERSION(2, 9, 0)
                    Label.wx_str());
                    #else
                    Label.c_str());
                    #endif
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsStaticBoxSizer::OnBuildSizerCreatingCode"),GetLanguage());
        }
    }
}

void wxsStaticBoxSizer::OnEnumSizerProperties(long Flags)
{
    static const long    OrientValues[] = { wxHORIZONTAL, wxVERTICAL, 0 };
    static const wxChar* OrientNames[]  = { _T("wxHORIZONTAL"), _T("wxVERTICAL"), 0 };

    WXS_SHORT_STRING(wxsStaticBoxSizer,Label,_("Label"),_T("label"),_T(""),false);
    WXS_ENUM(wxsStaticBoxSizer,Orient,_("Orientation"),_T("orient"),OrientValues,OrientNames,wxHORIZONTAL);
}
