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
* $Id: wxsguifactory.cpp 4850 2008-01-29 21:45:49Z byo $
* $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/wxSmith/wxsguifactory.cpp $
*/

#include "wxsguifactory.h"
#include "wxsgui.h"

#include <logmanager.h>
#include <wx/choicdlg.h>

wxsGUIFactory::wxsGUIFactory(const wxString& Name): m_Name(Name)
{
    // Registering this gui in new hash
    GetHash()[m_Name] = this;
}

wxsGUIFactory::~wxsGUIFactory()
{
    // Assuming that factories are available
    // during all wxSmith work time, it's not
    // necessarry to remove any bindings
}

wxsGUI* wxsGUIFactory::Build(const wxString& Name,wxsProject* Project)
{
    if ( GetHash().find(Name) == GetHash().end() ) return 0;
    wxsGUIFactory* Factory = GetHash()[Name];
    wxsGUI* NewGUI = Factory->OnCreate(Project);
    if ( NewGUI->GetName() != Name )
    {
        // Some hack? Bug in factory?
        Manager::Get()->GetLogManager()->DebugLog(_T("wxSmith: Error while creating wxsGUI object (name mismatch)."));
        Manager::Get()->GetLogManager()->DebugLog(_T("wxSmith:   Looks like bug in one wf wxsGUIFactory-derived classes or"));
        Manager::Get()->GetLogManager()->DebugLog(_T("wxSmith:   some hack attempt."));
        delete NewGUI;
        return 0;
    }
    return NewGUI;
}

wxsGUI* wxsGUIFactory::SelectNew(const wxString& Message,wxsProject* Project)
{
    if ( GetHash().empty() )
    {
        return 0;
    }
    if ( GetHash().size() == 1 )
    {
        return Build(GetHash().begin()->first,Project);
    }

    wxArrayString GUIList;
    for ( GUIItemHashT::iterator i = GetHash().begin(); i!=GetHash().end(); ++i )
    {
        GUIList.Add(i->first);
    }

    wxString SelectedGUI = ::wxGetSingleChoice(Message,_("Select GUI"), GUIList);
    if ( SelectedGUI.empty() )
    {
        return 0;
    }

    return Build(SelectedGUI,Project);
}

inline wxsGUIFactory::GUIItemHashT& wxsGUIFactory::GetHash()
{
    static GUIItemHashT Hash;
    return Hash;
}
