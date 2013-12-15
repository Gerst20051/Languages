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
* $Id: wxsradiobox.cpp 5796 2009-09-16 12:09:22Z mortenmacfly $
* $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/wxSmith/wxwidgets/defitems/wxsradiobox.cpp $
*/

#include <wx/defs.h> // to have wxUSE_RADIOBOX defined in the radiobox.h header
#include <wx/radiobox.h>
#include <wx/window.h> // GetParent
#include "wxsradiobox.h"

namespace
{
    wxsRegisterItem<wxsRadioBox> Reg(_T("RadioBox"),wxsTWidget,_T("Standard"),60);

    WXS_ST_BEGIN(wxsRadioBoxStyles,_T(""))
        WXS_ST_CATEGORY("wxRadioBox")
        WXS_ST(wxRA_SPECIFY_COLS)
        WXS_ST(wxRA_HORIZONTAL)
        WXS_ST(wxRA_SPECIFY_ROWS)
        WXS_ST(wxRA_VERTICAL)
        WXS_ST_DEFAULTS()
    WXS_ST_END()


    WXS_EV_BEGIN(wxsRadioBoxEvents)
        WXS_EVI(EVT_RADIOBOX,wxEVT_COMMAND_RADIOBOX_SELECTED,wxCommandEvent,Select)
    WXS_EV_END()
}

wxsRadioBox::wxsRadioBox(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxsRadioBoxEvents,
        wxsRadioBoxStyles),
    Label(_("Label")),
    DefaultSelection(-1),
    Dimension(1)
{}


void wxsRadioBox::OnBuildCreatingCode()
{
    switch ( GetLanguage() )
    {
        case wxsCPP:
        {
            AddHeader(_T("<wx/radiobox.h>"),GetInfo().ClassName,hfInPCH);

            // wxRadioBox does not have Append Function , therefore , have to build a wxString[]
            // to pass in to the ctor
            wxString ChoicesName;
            if ( ArrayChoices.GetCount() > 0 )
            {
                ChoicesName = GetCoderContext()->GetUniqueName(_T("__wxRadioBoxChoices"));
                #if wxCHECK_VERSION(2, 9, 0)
                Codef(_T("wxString %s[%d] = \n{\n"),ChoicesName.wx_str(),(int)ArrayChoices.GetCount());
                #else
                Codef(_T("wxString %s[%d] = \n{\n"),ChoicesName.c_str(),(int)ArrayChoices.GetCount());
                #endif
                for ( size_t i = 0; i < ArrayChoices.GetCount(); ++i )
                {
                    #if wxCHECK_VERSION(2, 9, 0)
                    Codef(_T("\t%t%s\n"),ArrayChoices[i].wx_str(),((i!=ArrayChoices.GetCount()-1)?_T(","):_T("")));
                    #else
                    Codef(_T("\t%t%s\n"),ArrayChoices[i].c_str(),((i!=ArrayChoices.GetCount()-1)?_T(","):_T("")));
                    #endif
                }
                Codef(_T("};\n"));
            }

            if ( Dimension < 1 ) Dimension = 1;

            Codef(_T("%C(%W, %I, %t, %P, %S, %d, %s, %d, %T, %V, %N);\n"),
                        #if wxCHECK_VERSION(2, 9, 0)
                        Label.wx_str(),ArrayChoices.GetCount(),
                        (ArrayChoices.IsEmpty()?_T("0"):ChoicesName.wx_str()),
                        #else
                        Label.c_str(),ArrayChoices.GetCount(),
                        (ArrayChoices.IsEmpty()?_T("0"):ChoicesName.c_str()),
                        #endif
                        Dimension);

            if ( DefaultSelection >= 0 && DefaultSelection < (int)ArrayChoices.GetCount() )
            {
                Codef( _T("%ASetSelection(%d);\n"), DefaultSelection);
            }
            BuildSetupWindowCode();
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsRadioBox::OnBuildCreatingCode"),GetLanguage());
        }
    }
}


wxObject* wxsRadioBox::OnBuildPreview(wxWindow* Parent,long Flags)
{
    wxRadioBox* Preview = new wxRadioBox(Parent,GetId(),Label,Pos(Parent),Size(Parent),ArrayChoices, Dimension, Style());
    if ( DefaultSelection >= 0 && DefaultSelection < (int)ArrayChoices.GetCount() )
    {
        Preview->SetSelection(DefaultSelection);
    }
    return SetupWindow(Preview,Flags);
}

void wxsRadioBox::OnEnumWidgetProperties(long Flags)
{
    WXS_SHORT_STRING(wxsRadioBox,Label,_("Label"),_T("label"),_T(""),true)
    WXS_ARRAYSTRING(wxsRadioBox,ArrayChoices,_("Choices"),_T("content"),_T("item"))
    WXS_LONG(wxsRadioBox,DefaultSelection,_("Default"),_T("default"),0)
    WXS_LONG(wxsRadioBox,Dimension,_("Dimension"),_T("dimension"),1)

}
