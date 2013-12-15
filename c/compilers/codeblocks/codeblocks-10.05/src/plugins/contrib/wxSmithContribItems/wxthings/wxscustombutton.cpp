/*
* This file is part of wxSmithContribItems plugin for Code::Blocks Studio
* Copyright (C) 2007  Bartlomiej Swiecki
*
* wxSmith is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* wxSmith is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with wxSmith; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
*
* $Revision: 5796 $
* $Id: wxscustombutton.cpp 5796 2009-09-16 12:09:22Z mortenmacfly $
* $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/wxSmithContribItems/wxthings/wxscustombutton.cpp $
*/

#include "wxscustombutton.h"

#include <wx/things/toggle.h>

namespace
{
    #include "wxcustombutton16.xpm"
    #include "wxcustombutton32.xpm"

    wxsRegisterItem<wxsCustomButton> Reg(
        _T("wxCustomButton"),
        wxsTWidget,
        _T("wxWindows"),
        _T("Bruce Phillips, John Labenski"),
        _T("jlabenski@gmail.com"),
        _T("http://wxcode.sourceforge.net/showcomp.php?name=wxThings"),
        _T("Contrib"),
        50,
        _T("Button"),
        wxsCPP,
        1, 0,
        wxBitmap(wxcustombutton32_xpm),
        wxBitmap(wxcustombutton16_xpm),
        false);

    WXS_EV_BEGIN(wxsCustomButtonEvents)
        WXS_EVI(EVT_BUTTON,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEvent,Click)
        WXS_EVI(EVT_TOGGLEBUTTON,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,wxCommandEvent,Toggle)
        WXS_EV_DEFAULTS()
    WXS_EV_END()


    const long TypeValues[] = { wxCUSTBUT_NOTOGGLE, wxCUSTBUT_BUTTON, wxCUSTBUT_TOGGLE, wxCUSTBUT_BUT_DCLICK_TOG, wxCUSTBUT_TOG_DCLICK_BUT };
    const wxChar* TypeNames[] = { _T("wxCUSTBUT_NOTOGGLE"), _T("wxCUSTBUT_BUTTON"), _T("wxCUSTBUT_TOGGLE"), _T("wxCUSTBUT_BUT_DCLICK_TOG"), _T("wxCUSTBUT_TOG_DCLICK_BUT"), NULL };

    const long LabelPositionValues[] = { wxCUSTBUT_LEFT, wxCUSTBUT_RIGHT, wxCUSTBUT_TOP, wxCUSTBUT_BOTTOM };
    const wxChar* LabelPositionNames[] = { _T("wxCUSTBUT_LEFT"), _T("wxCUSTBUT_RIGHT"), _T("wxCUSTBUT_TOP"), _T("wxCUSTBUT_BOTTOM"), NULL };
}

wxsCustomButton::wxsCustomButton(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxsCustomButtonEvents,
        NULL)
{
    m_Type = wxCUSTBUT_BUTTON;
    m_Flat = false;
    m_Label = _("Label");
    m_LabelPosition = wxCUSTBUT_BOTTOM;
}

wxsCustomButton::~wxsCustomButton()
{
}

void wxsCustomButton::OnBuildCreatingCode()
{
    switch ( GetLanguage() )
    {
        case wxsCPP:
        {
            AddHeader(_T("<wx/things/toggle.h>"),GetInfo().ClassName);
            AddHeader(_T("<wx/tglbtn.h>"),_T(""),hfLocal);

            wxString Style;
            for ( int i=0; TypeNames[i]; i++ )
            {
                if ( TypeValues[i] == m_Type )
                {
                    Style = TypeNames[i];
                    break;
                }
            }

            for ( int i=0; LabelPositionNames[i]; i++ )
            {
                if ( m_LabelPosition == LabelPositionValues[i] )
                {
                    if ( !Style.IsEmpty() )
                    {
                        Style.Append(_T('|'));
                    }
                    Style.Append(LabelPositionNames[i]);
                    break;
                }
            }

            if ( m_Flat )
            {
                if ( !Style.IsEmpty() )
                {
                    Style.Append(_T('|'));
                }
                Style.Append(_T("wxCUSTBUT_FLAT"));
            }

            if ( Style.IsEmpty() )
            {
                Style = _T("0");
            }

            #if wxCHECK_VERSION(2, 9, 0)
            Codef(_T("%C(%W,%I,%t,%i,%P,%S,%s,%V,%N);\n"),m_Label.wx_str(),&m_Bitmap,wxART_OTHER,Style.wx_str());
            #else
            Codef(_T("%C(%W,%I,%t,%i,%P,%S,%s,%V,%N);\n"),m_Label.c_str(),&m_Bitmap,wxART_OTHER,Style.c_str());
            #endif

            if ( !m_BitmapSelected.IsEmpty() )
            {
                Codef(_T("%ASetBitmapSelected(%i);\n"),&m_BitmapSelected,wxART_OTHER);
            }

            if ( !m_BitmapFocused.IsEmpty() )
            {
                Codef(_T("%ASetBitmapFocus(%i);\n"),&m_BitmapFocused,wxART_OTHER);
            }

            if ( !m_BitmapDisabled.IsEmpty() )
            {
                Codef(_T("%ASetBitmapDisabled(%i);\n"),&m_BitmapDisabled,wxART_OTHER);
            }
            else if ( !m_Bitmap.IsEmpty() )
            {
                // Use internal function to generate bitmap dithered with background colour
                Codef(_T("%ASetBitmapDisabled(%ACreateBitmapDisabled(%AGetBitmapLabel()));\n"));
            }

            if ( !m_Margins.IsDefault )
            {
                Codef(_T("%ASetMargins(%z);\n"),&m_Margins);
            }

            if ( !m_LabelMargins.IsDefault )
            {
                Codef(_T("%ASetLabelMargin(%z);\n"),&m_LabelMargins);
            }

            if ( !m_BitmapMargins.IsDefault )
            {
                Codef(_T("%ASetBitmapMargin(%z);\n"),&m_BitmapMargins);
            }

            break;
        }

        default:
            wxsCodeMarks::Unknown(_T("wxsCustomButton::OnBuildCreatingCode"),GetLanguage());
    }
}

wxObject* wxsCustomButton::OnBuildPreview(wxWindow* Parent,long Flags)
{
    wxCustomButton* Button = new wxCustomButton(
        Parent,
        GetId(),
        m_Label,
        m_Bitmap.GetPreview(wxDefaultSize),
        Pos(Parent),
        Size(Parent),
        m_Type | m_LabelPosition | (m_Flat?wxCUSTBUT_FLAT:0) );

    if ( !m_BitmapSelected.IsEmpty() )
    {
        Button->SetBitmapSelected(m_BitmapSelected.GetPreview(wxDefaultSize));
    }

    if ( !m_BitmapFocused.IsEmpty() )
    {
        Button->SetBitmapFocus(m_BitmapFocused.GetPreview(wxDefaultSize));
    }

    if ( !m_BitmapDisabled.IsEmpty() )
    {
        Button->SetBitmapDisabled(m_BitmapDisabled.GetPreview(wxDefaultSize));
    }
    else if ( !m_Bitmap.IsEmpty() )
    {
        Button->SetBitmapDisabled(Button->CreateBitmapDisabled(Button->GetBitmapLabel()));
    }

    if ( !m_Margins.IsDefault )
    {
        Button->SetMargins(m_Margins.GetSize(Parent));
    }

    if ( !m_LabelMargins.IsDefault )
    {
        Button->SetLabelMargin(m_LabelMargins.GetSize(Parent));
    }

    if ( !m_BitmapMargins.IsDefault )
    {
        Button->SetBitmapMargin(m_BitmapMargins.GetSize(Parent));
    }

    return Button;
}

void wxsCustomButton::OnEnumWidgetProperties(long Flags)
{
    WXS_ENUM(wxsCustomButton,m_Type,_("Type"),_T("type"),TypeValues,TypeNames,wxCUSTBUT_BUTTON);
    WXS_BOOL(wxsCustomButton,m_Flat,_("Flat"),_T("flat"),false);
    WXS_STRING(wxsCustomButton,m_Label,_("Label"),_T("label"),_T(""),false);
    WXS_ENUM(wxsCustomButton,m_LabelPosition,_("Label position"),_T("label_position"),LabelPositionValues,LabelPositionNames,wxCUSTBUT_BOTTOM);
    WXS_BITMAP(wxsCustomButton,m_Bitmap,_("Bitmap"),_T("bitmap"),_T("wxART_OTHER"));
    WXS_BITMAP(wxsCustomButton,m_BitmapSelected,_("Selected bmp"),_T("selected"),_T("wxART_OTHER"));
    WXS_BITMAP(wxsCustomButton,m_BitmapFocused,_("Focused bmp"),_T("focused"),_T("wxART_OTHER"));
    WXS_BITMAP(wxsCustomButton,m_BitmapDisabled,_("Disbled bmp"),_T("disabled"),_T("wxART_OTHER"));
    WXS_SIZE(wxsCustomButton,m_Margins,_("Default margin"),_("Margin width"),_("Margin height"),_("Margin in Dialog Units"),_T("margin"));
    WXS_SIZE(wxsCustomButton,m_LabelMargins,_("Default label margin"),_("Label marg. width"),_("Label marg. height"),_("Label marg. in Dialog Units"),_T("label_margin"));
    WXS_SIZE(wxsCustomButton,m_BitmapMargins,_("Default bitmap margin"),_("Bitmap marg. width"),_("Bitmap marg. height"),_("Bitmap marg. in Dialog Units"),_T("bitmap_margin"));
}
