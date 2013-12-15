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
* $Id: wxsgauge.cpp 4850 2008-01-29 21:45:49Z byo $
* $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/wxSmith/wxwidgets/defitems/wxsgauge.cpp $
*/

#include <wx/gauge.h>
#include "wxsgauge.h"

namespace
{
    wxsRegisterItem<wxsGauge> Reg(_T("Gauge"),wxsTWidget,_T("Standard"),50);

    WXS_ST_BEGIN(wxsGaugeStyles,_T(""))
        WXS_ST(wxGA_HORIZONTAL)
        WXS_ST(wxGA_VERTICAL)
        WXS_ST(wxGA_SMOOTH)
        WXS_ST_DEFAULTS()
    WXS_ST_END()


    WXS_EV_BEGIN(wxsGaugeEvents)
    WXS_EV_END()
}

wxsGauge::wxsGauge(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxsGaugeEvents,
        wxsGaugeStyles),
    Range(100),
    Value(0),
    Shadow(0),
    Bezel(0)
{}



void wxsGauge::OnBuildCreatingCode()
{
    switch ( GetLanguage() )
    {
        case wxsCPP:
        {
            AddHeader(_T("<wx/gauge.h>"),GetInfo().ClassName,hfInPCH);
            Codef(_T("%C(%W, %I, %d, %P, %S, %T, %V, %N);\n"),Range);
            if ( Value )  Codef(_T("%ASetValue(%d);\n"),Value);
            if ( Shadow ) Codef(_T("%ASetShadowWidth(%d);\n"),Shadow);
            if ( Bezel )  Codef(_T("%ASetBezelFace(%d);\n"),Bezel);
            BuildSetupWindowCode();
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsGauge::OnBuildCreatingCode"),GetLanguage());
        }
    }
}


wxObject* wxsGauge::OnBuildPreview(wxWindow* Parent,long Flags)
{
    wxGauge* Preview = new wxGauge(Parent,GetId(),Range,Pos(Parent),Size(Parent),Style());
    if ( Value )  Preview->SetValue(Value);
    if ( Shadow ) Preview->SetShadowWidth(Shadow);
    if ( Bezel )  Preview->SetBezelFace(Bezel);
    return SetupWindow(Preview,Flags);
}


void wxsGauge::OnEnumWidgetProperties(long Flags)
{
    WXS_LONG(wxsGauge,Value,_("Value"),_T("value"),0)
    WXS_LONG(wxsGauge,Range,_("Range"),_T("range"),100)
    WXS_LONG(wxsGauge,Shadow,_("3D Shadow Width"),_T("shadow"),0)
    WXS_LONG(wxsGauge,Bezel,_("Bezel Face Width"),_T("bezel"),0)
}
