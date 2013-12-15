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
* $Id: wxsspinctrl.cpp 5796 2009-09-16 12:09:22Z mortenmacfly $
* $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/wxSmith/wxwidgets/defitems/wxsspinctrl.cpp $
*/

#include "wxsspinctrl.h"

#include <wx/spinctrl.h>

namespace
{
    wxsRegisterItem<wxsSpinCtrl> Reg(_T("SpinCtrl"),wxsTWidget,_T("Standard"),50);

    WXS_ST_BEGIN(wxsSpinCtrlStyles,_T(""))
        WXS_ST_CATEGORY("wxSpinCtrl")
        WXS_ST(wxSP_HORIZONTAL)
        WXS_ST(wxSP_VERTICAL)
        WXS_ST(wxSP_ARROW_KEYS)
        WXS_ST(wxSP_WRAP)
        WXS_ST_DEFAULTS()
    WXS_ST_END()


    WXS_EV_BEGIN(wxsSpinCtrlEvents)
        WXS_EVI(EVT_SPINCTRL,wxEVT_COMMAND_SPINCTRL_UPDATED,wxSpinEvent,Change)
    WXS_EV_END()
}

wxsSpinCtrl::wxsSpinCtrl(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxsSpinCtrlEvents,
        wxsSpinCtrlStyles),
    Value(_T("0")),
    Min(0),
    Max(100)
{}

void wxsSpinCtrl::OnBuildCreatingCode()
{
    switch ( GetLanguage() )
    {
        case wxsCPP:
        {
            AddHeader(_T("<wx/spinctrl.h>"),GetInfo().ClassName,0);
            AddHeader(_T("<wx/spinctrl.h>"),_T("wxSpinEvent"),0);
            long ValueLong = 0;
            Value.ToLong(&ValueLong);
            #if wxCHECK_VERSION(2, 9, 0)
            Codef(_T("%C(%W, %I, %n, %P, %S, %T, %d, %d, %d, %N);\n"),Value.wx_str(),Min,Max,ValueLong);
            if ( !Value.empty() ) Codef(_T("%ASetValue(%n);\n"),Value.wx_str());
            #else
            Codef(_T("%C(%W, %I, %n, %P, %S, %T, %d, %d, %d, %N);\n"),Value.c_str(),Min,Max,ValueLong);
            if ( !Value.empty() ) Codef(_T("%ASetValue(%n);\n"),Value.c_str());
            #endif
            BuildSetupWindowCode();
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsSpinCtrl::OnBuildCreatingCode"),GetLanguage());
        }
    }
}

wxObject* wxsSpinCtrl::OnBuildPreview(wxWindow* Parent,long Flags)
{
    wxSpinCtrl* Preview = new wxSpinCtrl(Parent,GetId(),Value,Pos(Parent),Size(Parent),Style(),Min,Max);
    if ( !Value.empty() ) Preview->SetValue(Value);
    return SetupWindow(Preview,Flags);
}

void wxsSpinCtrl::OnEnumWidgetProperties(long Flags)
{
    WXS_SHORT_STRING(wxsSpinCtrl,Value,_("Value"),_T("value"),_T(""),true)
    WXS_LONG(wxsSpinCtrl,Min,_("Min"),_T("min"),0)
    WXS_LONG(wxsSpinCtrl,Max,_("Max"),_T("max"),100)
}
