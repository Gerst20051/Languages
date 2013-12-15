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
* $Id: wxsboolproperty.cpp 4850 2008-01-29 21:45:49Z byo $
* $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/wxSmith/properties/wxsboolproperty.cpp $
*/

#include "wxsboolproperty.h"

// Helper macro for fetching variable
#define VALUE   wxsVARIABLE(Object,Offset,bool)

wxsBoolProperty::wxsBoolProperty(const wxString& PGName,const wxString& DataName,long _Offset,bool _Default,int Priority):
    wxsProperty(PGName,DataName,Priority),
    Offset(_Offset),
    Default(_Default)
{}


void wxsBoolProperty::PGCreate(wxsPropertyContainer* Object,wxPropertyGridManager* Grid,wxPGId Parent)
{
    wxPGId ID = Grid->AppendIn(Parent,wxBoolProperty(GetPGName(),wxPG_LABEL,VALUE));
    Grid->SetPropertyAttribute(ID,wxPG_BOOL_USE_CHECKBOX,1L,wxPG_RECURSE);
    PGRegister(Object,Grid,ID);
}

bool wxsBoolProperty::PGRead(wxsPropertyContainer* Object,wxPropertyGridManager* Grid,wxPGId Id,long Index)
{
    VALUE = Grid->GetPropertyValue(Id).GetBool();
    return true;
}

bool wxsBoolProperty::PGWrite(wxsPropertyContainer* Object,wxPropertyGridManager* Grid,wxPGId Id,long Index)
{
    Grid->SetPropertyValue(Id,VALUE);
    return true;
}

bool wxsBoolProperty::XmlRead(wxsPropertyContainer* Object,TiXmlElement* Element)
{
    if ( !Element )
    {
        VALUE = Default;
        return false;
    }
    const char* Text = Element->GetText();
    if ( !Text )
    {
        VALUE = Default;
        return false;
    }
    VALUE = atol(Text) != 0;
    return true;
}

bool wxsBoolProperty::XmlWrite(wxsPropertyContainer* Object,TiXmlElement* Element)
{
    if ( VALUE != Default )
    {
        Element->InsertEndChild(TiXmlText(VALUE?"1":"0"));
        return true;
    }
    return false;
}

bool wxsBoolProperty::PropStreamRead(wxsPropertyContainer* Object,wxsPropertyStream* Stream)
{
    return Stream->GetBool(GetDataName(),VALUE,Default);
}

bool wxsBoolProperty::PropStreamWrite(wxsPropertyContainer* Object,wxsPropertyStream* Stream)
{
    return Stream->PutBool(GetDataName(),VALUE,Default);
}
