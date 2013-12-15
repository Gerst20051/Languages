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
* $Id: wxscustomeditorproperty.cpp 5796 2009-09-16 12:09:22Z mortenmacfly $
* $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/wxSmith/properties/wxscustomeditorproperty.cpp $
*/

#include "wxscustomeditorproperty.h"

#include <wx/dialog.h>
#include <wx/bitmap.h>
#include <wx/propgrid/propgrid.h>
#if !wxCHECK_VERSION(2, 9, 0)
#include <wx/propgrid/propdev.h>
#endif
#include <wx/propgrid/advprops.h>
#include <wx/propgrid/manager.h>

namespace
{
    class wxsCustomEditorPropertyPropClass: public wxCustomPropertyClass
    {
        public:
            /** \brief Standard property editor */
            wxsCustomEditorPropertyPropClass(
                const wxString& label,
                const wxString& name,
                wxsCustomEditorProperty* property,
                wxsPropertyContainer* object):
                    wxCustomPropertyClass(label,name),
                    Property(property),
                    Object(object)
            {
                SetEditor(wxPG_EDITOR(TextCtrlAndButton));
            }

            virtual bool OnEvent(
                wxPropertyGrid* propgrid,
                wxWindow* wnd_primary,
                wxEvent& event)
            {
                if ( event.GetEventType() == wxEVT_COMMAND_BUTTON_CLICKED )
                {
                    return Property->ShowEditor(Object);
                }
                return wxCustomPropertyClass::OnEvent(propgrid,wnd_primary,event);
            }


            /** \brief Pointer to wxsProperty which created this
             *
             * Pointer will be used to call ShowEditor
             */
            wxsCustomEditorProperty* Property;
            wxsPropertyContainer* Object;
    };
}

void wxsCustomEditorProperty::PGCreate(wxsPropertyContainer* Object,wxPropertyGridManager* Grid,wxPGId Parent)
{
    wxPGId PGId = Grid->AppendIn(Parent,new wxsCustomEditorPropertyPropClass(GetPGName(),wxPG_LABEL,this,Object));
    Grid->SetPropertyValue(PGId,GetStr(Object));
    if ( !CanParseStr() )
    {
        Grid->LimitPropertyEditing(PGId);
    }
    PGRegister(Object,Grid,PGId);
}

bool wxsCustomEditorProperty::PGRead(wxsPropertyContainer* Object,wxPropertyGridManager* Grid, wxPGId PGId,long Index)
{
    return CanParseStr() && ParseStr(Object,Grid->GetPropertyValue(PGId).GetString());
}

bool wxsCustomEditorProperty::PGWrite(wxsPropertyContainer* Object,wxPropertyGridManager* Grid, wxPGId PGId,long Index)
{
    Grid->SetPropertyValue(PGId,GetStr(Object));
    return true;
}
