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
* $Id: wxscolourproperty.cpp 4850 2008-01-29 21:45:49Z byo $
* $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/wxSmith/wxwidgets/properties/wxscolourproperty.cpp $
*/

#include "wxscolourproperty.h"

#include <logmanager.h>
#include <wx/settings.h>
#include <wx/colordlg.h>
#include <wx/dc.h>
#include "../wxsflags.h"

using namespace wxsFlags;


// Creating custom colour property.
// This is based on wxSystemColourProperty from propgrid

namespace
{
    static const wxChar* wxsColourLabels[] = {
        _("Default"),
        _("Custom"),
        _("Scrollbar"),
        _("Desktop"),
        _("Active window caption"),
        _("Inactive window caption"),
        _("Menu background"),
        _("Window background"),
        _("Window frame"),
        _("Menu text"),
        _("Text in window"),
        _("Text in window caption"),
        _("Active window border"),
        _("Inactive window border"),
        _("Background for MDI apps"),
        _("Selected item"),
        _("Text of selected item"),
        _("Face of button"),
        _("Edge of button"),
        _("Grayed (disabled) text"),
        _("Text on buttons"),
        _("Text of inactive caption"),
        _("Highlight colour for buttons"),
        _("Dark shadow for 3D items"),
        _("Light for 3D items"),
        _("Tooltip text"),
        _("Tooltip background"),
        _("Listbox"),
        _("Hot light"),
        _("Gradient of active caption"),
        _("Gradnent of inactive caption"),
        _("Selected menu item"),
        _("Menu bar"),
        0
    };

    const int wxsColourCount = (sizeof(wxsColourLabels) / sizeof(wxsColourLabels[0])) - 1;

    static long wxsColourValues[] = {
        wxsCOLOUR_DEFAULT,
        wxPG_COLOUR_CUSTOM,
        wxSYS_COLOUR_SCROLLBAR,
        wxSYS_COLOUR_DESKTOP,
        wxSYS_COLOUR_ACTIVECAPTION,
        wxSYS_COLOUR_INACTIVECAPTION,
        wxSYS_COLOUR_MENU,
        wxSYS_COLOUR_WINDOW,
        wxSYS_COLOUR_WINDOWFRAME,
        wxSYS_COLOUR_MENUTEXT,
        wxSYS_COLOUR_WINDOWTEXT,
        wxSYS_COLOUR_CAPTIONTEXT,
        wxSYS_COLOUR_ACTIVEBORDER,
        wxSYS_COLOUR_INACTIVEBORDER,
        wxSYS_COLOUR_APPWORKSPACE,
        wxSYS_COLOUR_HIGHLIGHT,
        wxSYS_COLOUR_HIGHLIGHTTEXT,
        wxSYS_COLOUR_BTNFACE,
        wxSYS_COLOUR_BTNSHADOW,
        wxSYS_COLOUR_GRAYTEXT,
        wxSYS_COLOUR_BTNTEXT,
        wxSYS_COLOUR_INACTIVECAPTIONTEXT,
        wxSYS_COLOUR_BTNHIGHLIGHT,
        wxSYS_COLOUR_3DDKSHADOW,
        wxSYS_COLOUR_3DLIGHT,
        wxSYS_COLOUR_INFOTEXT,
        wxSYS_COLOUR_INFOBK,
        wxSYS_COLOUR_LISTBOX,
        wxSYS_COLOUR_HOTLIGHT,
        wxSYS_COLOUR_GRADIENTACTIVECAPTION,
        wxSYS_COLOUR_GRADIENTINACTIVECAPTION,
        wxSYS_COLOUR_MENUHILIGHT,
        wxSYS_COLOUR_MENUBAR,
        0
    };

    // #@$%@#$% This was really annoying bit of code
    // wxPropertyGrid is really messy when trying to
    // create custom properties. And all this macro stuff
    // is just disaster for any developer, especially when
    // macros are buggy (produce compilation errors).
    class wxsMyColourPropertyClass : public wxEnumPropertyClass
    {
        public:

            virtual wxPG_VALUETYPE_MSGVAL GetValueType() const
            {
                return wxPG_VALUETYPE(wxColourPropertyValue);
            }

            wxsMyColourPropertyClass(
                const wxString& label = wxEmptyString,
                const wxString& name = wxPG_LABEL,
                const wxColourPropertyValue& value = wxColourPropertyValue(wxsCOLOUR_DEFAULT,*wxWHITE) );

            virtual ~wxsMyColourPropertyClass();

            virtual void DoSetValue(wxPGVariant value);
            virtual wxPGVariant DoGetValue() const;
            virtual bool SetValueFromString(const wxString& text, int);
            virtual wxString GetValueAsString(int) const;
            virtual bool OnEvent(wxPropertyGrid* propGrid, wxWindow* primary, wxEvent& event);
            virtual wxSize GetImageSize() const;
            virtual void OnCustomPaint(wxDC& dc,const wxRect& rect,wxPGPaintData& paintdata);

        protected:

            wxColourPropertyValue   m_value;

    };

    wxsMyColourPropertyClass::wxsMyColourPropertyClass( const wxString& label, const wxString& name,
        const wxColourPropertyValue& value )
        : wxEnumPropertyClass( label, name, wxsColourLabels, wxsColourValues, wxsColourCount )
    {
        wxPG_INIT_REQUIRED_TYPE(wxColourPropertyValue)
        m_value.m_type = value.m_type;
        m_value.m_colour = value.m_colour.Ok() ? value.m_colour : *wxWHITE;
        m_flags |= wxPG_PROP_STATIC_CHOICES;
        DoSetValue(&m_value);
    }

    wxsMyColourPropertyClass::~wxsMyColourPropertyClass () { }

    void wxsMyColourPropertyClass::DoSetValue ( wxPGVariant value )
    {
        wxColourPropertyValue* pval = wxPGVariantToWxObjectPtr(value,wxColourPropertyValue);
        m_flags &= ~(wxPG_PROP_UNSPECIFIED);

        if ( pval == (wxColourPropertyValue*) 0 )
        {
            m_value.Init(wxsCOLOUR_DEFAULT,*wxWHITE);
        }
        else if ( pval != &m_value )
        {
            m_value = *pval;
        }

        if ( m_value.m_type < wxPG_COLOUR_WEB_BASE )
        {
            m_value.m_colour = wxSystemSettings::GetColour((wxSystemColour)m_value.m_type);
            wxEnumPropertyClass::DoSetValue((long)m_value.m_type);
        }
        else if ( m_value.m_type == wxsCOLOUR_DEFAULT )
        {
            wxEnumPropertyClass::DoSetValue((long)m_value.m_type);
        }
    }

    wxPGVariant wxsMyColourPropertyClass::DoGetValue () const
    {
        return wxPGVariantCreator(&m_value);
    }

    wxString wxsMyColourPropertyClass::GetValueAsString ( int ) const
    {
        if ( m_value.m_type == wxPG_COLOUR_CUSTOM )
        {
            wxString temp;
            temp.Printf ( wxT("(%i,%i,%i)"),
                (int)m_value.m_colour.Red(),(int)m_value.m_colour.Green(),(int)m_value.m_colour.Blue());
            return temp;
        }
        return m_choices.GetLabel(m_index);
    }

    wxSize wxsMyColourPropertyClass::GetImageSize() const
    {
        return wxSize(-1,-1);
    }

    bool wxsMyColourPropertyClass::OnEvent ( wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event )
    {
        if ( event.GetEventType() == wxEVT_COMMAND_COMBOBOX_SELECTED )
        {
            int type = wxEnumPropertyClass::DoGetValue().GetLong();

            if ( type == wxPG_COLOUR_CUSTOM )
            {
                m_value.m_type = wxPG_COLOUR_CUSTOM;

                wxColourData data;
                data.SetChooseFull(true);
                int i;
                for ( i = 0; i < 16; i++)
                {
                    wxColour colour(i*16, i*16, i*16);
                    data.SetCustomColour(i, colour);
                }

                wxColourDialog dialog(propgrid, &data);
                bool Ret = false;
                if ( dialog.ShowModal() == wxID_OK )
                {
                    wxColourData retData = dialog.GetColourData();
                    m_value.m_colour = retData.GetColour();
                    wxsMyColourPropertyClass::DoSetValue(m_value);
                    Ret = true;
                }

                // Update text in combo box (so it is "(R,G,B)" not "Custom").
                if ( primary )
                {
                    GetEditorClass()->SetControlStringValue(primary,GetValueAsString(0));
                }

                return Ret;
            }
            else if ( type == wxsCOLOUR_DEFAULT )
            {
                m_value.m_type = wxsCOLOUR_DEFAULT;
                m_value.m_colour = wxColour(0,0,0);

                if ( primary )
                {
                    GetEditorClass()->SetControlStringValue(primary,GetValueAsString(0));
                }

                return true;
            }
            else
            {
                m_value.m_type = type;
                m_value.m_colour = wxSystemSettings::GetColour((wxSystemColour)m_value.m_type);
                return true;
            }
        }
        return false;
    }

    void wxsMyColourPropertyClass::OnCustomPaint ( wxDC& dc,
        const wxRect& rect, wxPGPaintData& paintdata )
    {
        int value;

        if ( paintdata.m_choiceItem >= 0 && paintdata.m_choiceItem < (int)(GetItemCount()) )
        {
            int index = paintdata.m_choiceItem;
            value = wxsColourValues[index];
        }
        else if ( !(m_flags & wxPG_PROP_UNSPECIFIED) )
        {
            value = m_value.m_type;
        }
        else
        {
            dc.SetBrush ( *wxWHITE );
            dc.DrawRectangle ( rect );
            return;
        }

        if ( value == wxsCOLOUR_DEFAULT )
        {
            dc.SetBrush(*wxWHITE);
            dc.DrawRectangle(rect);
            dc.SetBrush(wxBrush(*wxBLACK,wxBDIAGONAL_HATCH));
            dc.DrawRectangle(rect);
            return;
        }

        if ( value == wxPG_COLOUR_CUSTOM )
        {
            dc.SetBrush( m_value.m_colour );
        }
        else
        {
            dc.SetBrush(wxSystemSettings::GetColour((wxSystemColour)value));
        }
        dc.DrawRectangle(rect);
    }

    bool wxsMyColourPropertyClass::SetValueFromString ( const wxString& text, int flags )
    {
        wxColourPropertyValue val;
        if ( text[0] == wxT('(') )
        {
            // Custom colour.
            val.m_type = wxPG_COLOUR_CUSTOM;

            int r, g, b;
            wxSscanf(text.c_str(),wxT("(%i,%i,%i)"),&r,&g,&b);
            val.m_colour.Set(r,g,b);

            wxsMyColourPropertyClass::DoSetValue ( &val );

            return true;
        }
        else
        {
            // Predefined colour.
            bool res = wxEnumPropertyClass::SetValueFromString(text,flags);
            if ( res )
            {
                val.m_type = wxsColourValues[m_index];

                // Get proper colour for type.
                if ( val.m_type == wxsCOLOUR_DEFAULT )
                {
                    val.m_colour = wxColour(0,0,0);
                }
                else if ( val.m_type != wxPG_COLOUR_CUSTOM )
                {
                    val.m_colour = wxSystemSettings::GetColour((wxSystemColour)val.m_type);
                }
                wxsMyColourPropertyClass::DoSetValue(&val);
                return true;
            }
        }
        return true;
    };
}


wxColour wxsColourData::GetColour()
{
    if ( m_type == wxsCOLOUR_DEFAULT )
    {
        return wxColour();
    }

    if ( m_type == wxPG_COLOUR_CUSTOM )
    {
        return m_colour;
    }

    return wxSystemSettings::GetColour((wxSystemColour)m_type);
}

wxString wxsColourData::BuildCode(wxsCoderContext* Context)
{
    if ( m_type == wxsCOLOUR_DEFAULT )
    {
        return wxEmptyString;
    }

    switch ( Context->m_Language )
    {
        case wxsCPP:
        {
            if ( m_type == wxPG_COLOUR_CUSTOM )
            {
                return wxString::Format(_T("wxColour(%d,%d,%d)"),
                    (unsigned int)m_colour.Red(),
                    (unsigned int)m_colour.Green(),
                    (unsigned int)m_colour.Blue());
            }

            wxString SysColName;

            #define SYSCLR(N) if ( m_type == N ) SysColName = _T(#N); else
            SYSCLR(wxSYS_COLOUR_SCROLLBAR)
            SYSCLR(wxSYS_COLOUR_BACKGROUND)
            SYSCLR(wxSYS_COLOUR_DESKTOP)
            SYSCLR(wxSYS_COLOUR_ACTIVECAPTION)
            SYSCLR(wxSYS_COLOUR_INACTIVECAPTION)
            SYSCLR(wxSYS_COLOUR_MENU)
            SYSCLR(wxSYS_COLOUR_WINDOW)
            SYSCLR(wxSYS_COLOUR_WINDOWFRAME)
            SYSCLR(wxSYS_COLOUR_MENUTEXT)
            SYSCLR(wxSYS_COLOUR_WINDOWTEXT)
            SYSCLR(wxSYS_COLOUR_CAPTIONTEXT)
            SYSCLR(wxSYS_COLOUR_ACTIVEBORDER)
            SYSCLR(wxSYS_COLOUR_INACTIVEBORDER)
            SYSCLR(wxSYS_COLOUR_APPWORKSPACE)
            SYSCLR(wxSYS_COLOUR_HIGHLIGHT)
            SYSCLR(wxSYS_COLOUR_HIGHLIGHTTEXT)
            SYSCLR(wxSYS_COLOUR_BTNFACE)
            SYSCLR(wxSYS_COLOUR_3DFACE)
            SYSCLR(wxSYS_COLOUR_BTNSHADOW)
            SYSCLR(wxSYS_COLOUR_3DSHADOW)
            SYSCLR(wxSYS_COLOUR_GRAYTEXT)
            SYSCLR(wxSYS_COLOUR_BTNTEXT)
            SYSCLR(wxSYS_COLOUR_INACTIVECAPTIONTEXT)
            SYSCLR(wxSYS_COLOUR_BTNHIGHLIGHT)
            SYSCLR(wxSYS_COLOUR_BTNHILIGHT)
            SYSCLR(wxSYS_COLOUR_3DHIGHLIGHT)
            SYSCLR(wxSYS_COLOUR_3DHILIGHT)
            SYSCLR(wxSYS_COLOUR_3DDKSHADOW)
            SYSCLR(wxSYS_COLOUR_3DLIGHT)
            SYSCLR(wxSYS_COLOUR_INFOTEXT)
            SYSCLR(wxSYS_COLOUR_INFOBK)
            SYSCLR(wxSYS_COLOUR_LISTBOX)
            SYSCLR(wxSYS_COLOUR_HOTLIGHT)
            SYSCLR(wxSYS_COLOUR_GRADIENTACTIVECAPTION)
            SYSCLR(wxSYS_COLOUR_GRADIENTINACTIVECAPTION)
            SYSCLR(wxSYS_COLOUR_MENUHILIGHT)
            SYSCLR(wxSYS_COLOUR_MENUBAR)
            {}
            #undef SYSCLR

            if ( SysColName.empty() )
            {
                return wxEmptyString;
            }

            Context->AddHeader(_T("<wx/settings.h>"),_T(""),hfLocal);
            return _T("wxSystemSettings::GetColour(") + SysColName + _T(")");
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsColourData::BuildCode"),Context->m_Language);
        }
    }

    return wxEmptyString;
}



// Helper macros for fetching variables
#define VALUE   wxsVARIABLE(Object,ValueOffset,wxsColourData)

wxsColourProperty::wxsColourProperty(
    const wxString& PGName,
    const wxString& DataName,
    long _ValueOffset,
    int Priority):
        wxsProperty(PGName,DataName,Priority),
        ValueOffset(_ValueOffset)
{}


void wxsColourProperty::PGCreate(wxsPropertyContainer* Object,wxPropertyGridManager* Grid,wxPGId Parent)
{
    PGRegister(Object,Grid,Grid->AppendIn(Parent,new wxsMyColourPropertyClass(GetPGName(),wxPG_LABEL,VALUE)));
//    PGRegister(Object,Grid,Grid->AppendIn(Parent,wxSystemColourProperty(GetPGName(),wxPG_LABEL,VALUE)));
}

bool wxsColourProperty::PGRead(wxsPropertyContainer* Object,wxPropertyGridManager* Grid,wxPGId Id,long Index)
{
    wxVariant value = Grid->GetPropertyValue(Id);
    wxColourPropertyValue* Val = wxGetVariantCast(value,wxColourPropertyValue);
    if ( !Val ) return false;

    VALUE = *Val;

    return true;
}

bool wxsColourProperty::PGWrite(wxsPropertyContainer* Object,wxPropertyGridManager* Grid,wxPGId Id,long Index)
{
    Grid->SetPropertyValue(Id,VALUE);
    return true;
}

bool wxsColourProperty::XmlRead(wxsPropertyContainer* Object,TiXmlElement* Element)
{
    if ( !Element )
    {
        VALUE.m_type = wxsCOLOUR_DEFAULT;
        VALUE.m_colour = wxColour(0,0,0);
        return false;
    }

    wxString Str;
    if ( !XmlGetString(Element,Str) || Str.empty() )
    {
        VALUE.m_type = wxsCOLOUR_DEFAULT;
        VALUE.m_colour = wxColour(0,0,0);
        return false;
    }

    unsigned long tmp = 0;
    if ( Str.Length() == 7 && Str[0] == _T('#') && wxSscanf(Str.c_str(),_T("#%lX"),&tmp) == 1 )
    {
        VALUE.m_type = wxPG_COLOUR_CUSTOM;
        VALUE.m_colour = wxColour((unsigned char) ((tmp & 0xFF0000) >> 16) ,
                                  (unsigned char) ((tmp & 0x00FF00) >> 8),
                                  (unsigned char) ((tmp & 0x0000FF)));
    }
    else
    {
        #define SYSCLR(N) if ( Str == _T(#N) ) { VALUE.m_type = N; VALUE.m_colour = wxSystemSettings::GetColour(N); } else
        SYSCLR(wxSYS_COLOUR_SCROLLBAR)
        SYSCLR(wxSYS_COLOUR_BACKGROUND)
        SYSCLR(wxSYS_COLOUR_DESKTOP)
        SYSCLR(wxSYS_COLOUR_ACTIVECAPTION)
        SYSCLR(wxSYS_COLOUR_INACTIVECAPTION)
        SYSCLR(wxSYS_COLOUR_MENU)
        SYSCLR(wxSYS_COLOUR_WINDOW)
        SYSCLR(wxSYS_COLOUR_WINDOWFRAME)
        SYSCLR(wxSYS_COLOUR_MENUTEXT)
        SYSCLR(wxSYS_COLOUR_WINDOWTEXT)
        SYSCLR(wxSYS_COLOUR_CAPTIONTEXT)
        SYSCLR(wxSYS_COLOUR_ACTIVEBORDER)
        SYSCLR(wxSYS_COLOUR_INACTIVEBORDER)
        SYSCLR(wxSYS_COLOUR_APPWORKSPACE)
        SYSCLR(wxSYS_COLOUR_HIGHLIGHT)
        SYSCLR(wxSYS_COLOUR_HIGHLIGHTTEXT)
        SYSCLR(wxSYS_COLOUR_BTNFACE)
        SYSCLR(wxSYS_COLOUR_3DFACE)
        SYSCLR(wxSYS_COLOUR_BTNSHADOW)
        SYSCLR(wxSYS_COLOUR_3DSHADOW)
        SYSCLR(wxSYS_COLOUR_GRAYTEXT)
        SYSCLR(wxSYS_COLOUR_BTNTEXT)
        SYSCLR(wxSYS_COLOUR_INACTIVECAPTIONTEXT)
        SYSCLR(wxSYS_COLOUR_BTNHIGHLIGHT)
        SYSCLR(wxSYS_COLOUR_BTNHILIGHT)
        SYSCLR(wxSYS_COLOUR_3DHIGHLIGHT)
        SYSCLR(wxSYS_COLOUR_3DHILIGHT)
        SYSCLR(wxSYS_COLOUR_3DDKSHADOW)
        SYSCLR(wxSYS_COLOUR_3DLIGHT)
        SYSCLR(wxSYS_COLOUR_INFOTEXT)
        SYSCLR(wxSYS_COLOUR_INFOBK)
        SYSCLR(wxSYS_COLOUR_LISTBOX)
        SYSCLR(wxSYS_COLOUR_HOTLIGHT)
        SYSCLR(wxSYS_COLOUR_GRADIENTACTIVECAPTION)
        SYSCLR(wxSYS_COLOUR_GRADIENTINACTIVECAPTION)
        SYSCLR(wxSYS_COLOUR_MENUHILIGHT)
        SYSCLR(wxSYS_COLOUR_MENUBAR)
        {
            VALUE.m_type = wxsCOLOUR_DEFAULT;
            VALUE.m_colour = wxColour(0,0,0);
        }
        #undef SYSCLR
    }

    return true;
}

bool wxsColourProperty::XmlWrite(wxsPropertyContainer* Object,TiXmlElement* Element)
{
    if ( VALUE.m_type == wxsCOLOUR_DEFAULT )
    {
        return false;
    }

    if ( VALUE.m_type == wxPG_COLOUR_CUSTOM )
    {
        XmlSetString(Element,
            wxString::Format(_T("#%02X%02X%02X"),
                (unsigned int)VALUE.m_colour.Red(),
                (unsigned int)VALUE.m_colour.Green(),
                (unsigned int)VALUE.m_colour.Blue()));
        return true;
    }

    #define SYSCLR(N) if ( VALUE.m_type == N ) XmlSetString(Element,_T(#N)); else
    SYSCLR(wxSYS_COLOUR_SCROLLBAR)
    SYSCLR(wxSYS_COLOUR_BACKGROUND)
    SYSCLR(wxSYS_COLOUR_DESKTOP)
    SYSCLR(wxSYS_COLOUR_ACTIVECAPTION)
    SYSCLR(wxSYS_COLOUR_INACTIVECAPTION)
    SYSCLR(wxSYS_COLOUR_MENU)
    SYSCLR(wxSYS_COLOUR_WINDOW)
    SYSCLR(wxSYS_COLOUR_WINDOWFRAME)
    SYSCLR(wxSYS_COLOUR_MENUTEXT)
    SYSCLR(wxSYS_COLOUR_WINDOWTEXT)
    SYSCLR(wxSYS_COLOUR_CAPTIONTEXT)
    SYSCLR(wxSYS_COLOUR_ACTIVEBORDER)
    SYSCLR(wxSYS_COLOUR_INACTIVEBORDER)
    SYSCLR(wxSYS_COLOUR_APPWORKSPACE)
    SYSCLR(wxSYS_COLOUR_HIGHLIGHT)
    SYSCLR(wxSYS_COLOUR_HIGHLIGHTTEXT)
    SYSCLR(wxSYS_COLOUR_BTNFACE)
    SYSCLR(wxSYS_COLOUR_3DFACE)
    SYSCLR(wxSYS_COLOUR_BTNSHADOW)
    SYSCLR(wxSYS_COLOUR_3DSHADOW)
    SYSCLR(wxSYS_COLOUR_GRAYTEXT)
    SYSCLR(wxSYS_COLOUR_BTNTEXT)
    SYSCLR(wxSYS_COLOUR_INACTIVECAPTIONTEXT)
    SYSCLR(wxSYS_COLOUR_BTNHIGHLIGHT)
    SYSCLR(wxSYS_COLOUR_BTNHILIGHT)
    SYSCLR(wxSYS_COLOUR_3DHIGHLIGHT)
    SYSCLR(wxSYS_COLOUR_3DHILIGHT)
    SYSCLR(wxSYS_COLOUR_3DDKSHADOW)
    SYSCLR(wxSYS_COLOUR_3DLIGHT)
    SYSCLR(wxSYS_COLOUR_INFOTEXT)
    SYSCLR(wxSYS_COLOUR_INFOBK)
    SYSCLR(wxSYS_COLOUR_LISTBOX)
    SYSCLR(wxSYS_COLOUR_HOTLIGHT)
    SYSCLR(wxSYS_COLOUR_GRADIENTACTIVECAPTION)
    SYSCLR(wxSYS_COLOUR_GRADIENTINACTIVECAPTION)
    SYSCLR(wxSYS_COLOUR_MENUHILIGHT)
    SYSCLR(wxSYS_COLOUR_MENUBAR)
    {
        return false;
    }
    #undef SYSCLR

    return true;
}

bool wxsColourProperty::PropStreamRead(wxsPropertyContainer* Object,wxsPropertyStream* Stream)
{
    bool Ret = true;
    long Colour;
    unsigned long Type;
    Stream->SubCategory(GetDataName());
    if ( !Stream->GetULong(_T("type"),Type,wxsCOLOUR_DEFAULT) ) Ret = false;
    VALUE.m_type = Type;
    if ( !Stream->GetLong(_T("value"),Colour,0) ) Ret = false;
    if ( Type == wxsCOLOUR_DEFAULT )
    {
        VALUE.m_colour = wxColour(0,0,0);
    }
    else if ( Type == wxPG_COLOUR_CUSTOM )
    {
        VALUE.m_colour = Colour;
    }
    else
    {
        VALUE.m_colour = wxSystemSettings::GetColour((wxSystemColour)VALUE.m_type);
    }
    Stream->PopCategory();
    return Ret;
}

bool wxsColourProperty::PropStreamWrite(wxsPropertyContainer* Object,wxsPropertyStream* Stream)
{
    bool Ret = true;
    long Colour =
        ( (unsigned int)VALUE.m_colour.Red()   << 16 ) |
        ( (unsigned int)VALUE.m_colour.Green() <<  8 ) |
        ( (unsigned int)VALUE.m_colour.Blue() );
    unsigned long Type = VALUE.m_type;

    Stream->SubCategory(GetDataName());

    if ( !Stream->PutULong(_T("type"),Type,wxsCOLOUR_DEFAULT) ) Ret = false;
    VALUE.m_type = Type;

    if ( !Stream->PutLong(_T("value"),Colour,0) ) Ret = false;

    if ( VALUE.m_type == wxsCOLOUR_DEFAULT )
    {
        VALUE.m_colour = wxColour(0,0,0);
    }
    else if ( VALUE.m_type == wxPG_COLOUR_CUSTOM )
    {
        VALUE.m_colour = Colour;
    }
    else
    {
        VALUE.m_colour = wxSystemSettings::GetColour((wxSystemColour)VALUE.m_type);
    }

    Stream->PopCategory();
    return Ret;
}


