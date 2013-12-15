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
* $Id: wxsitemfactory.cpp 4850 2008-01-29 21:45:49Z byo $
* $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/wxSmith/wxwidgets/wxsitemfactory.cpp $
*/

#include "wxsitemfactory.h"
#include "wxsitem.h"
#include "../wxsresourcetree.h"

wxsItem* wxsItemFactory::Build(const wxString& Name,wxsItemResData* Data)
{
    ItemMapT::iterator it = ItemMap().find(Name);
    if ( it == ItemMap().end() ) return 0;
    wxsItem* Item = it->second->OnBuild(Data);

    // Checking few things in item's info
    switch ( Item->GetInfo().Type )
    {
        case wxsTTool:
            if ( !Item->ConvertToTool() )
            {
                // Fake item
                delete Item;
                return 0;
            }
            break;

        case wxsTContainer:
            if ( !Item->ConvertToParent() )
            {
                // Fake item
                delete Item;
                return 0;
            }
            break;

        case wxsTSizer:
        case wxsTSpacer:
        case wxsTWidget:
            break;

        default:
            delete Item;
            return 0;
    }

    return Item;
}

const wxsItemInfo* wxsItemFactory::GetInfo(const wxString& Name)
{
    ItemMapT::iterator it = ItemMap().find(Name);
    if ( it == ItemMap().end() ) return 0;
    return it->second->m_Info;
}

const wxsItemInfo* wxsItemFactory::GetFirstInfo()
{
    m_Iter = ItemMap().begin();
    return (m_Iter==ItemMap().end()) ? 0 : m_Iter->second->m_Info;
}

const wxsItemInfo* wxsItemFactory::GetNextInfo()
{
    if ( m_Iter==ItemMap().end() ) return 0;
    ++m_Iter;
    return (m_Iter==ItemMap().end()) ? 0 : m_Iter->second->m_Info;
}

wxImageList& wxsItemFactory::GetImageList()
{
    return wxsResourceTree::GetGlobalImageList();
}

int wxsItemFactory::LoadImage(const wxString& FileName)
{
    return wxsResourceTree::LoadImage(FileName);
}

wxsItemFactory::wxsItemFactory(const wxsItemInfo* Info):
    m_Info(Info)
{
    m_Name = Info->ClassName;
    if ( Info==0 ) return;
    ItemMap()[m_Name] = this;
}

wxsItemFactory::wxsItemFactory(const wxsItemInfo* Info,wxString ClassName):
    m_Info(Info)
{
    m_Name = ClassName;
    if ( Info==0 ) return;
    ItemMap()[m_Name] = this;
}

wxsItemFactory::~wxsItemFactory()
{
    if ( !m_Info ) return;
    ItemMapT::iterator it = ItemMap().find(m_Name);
    if ( it == ItemMap().end() ) return;
    if ( it->second!=this ) return;
    ItemMap().erase(it);
}

wxsItemFactory::ItemMapT& wxsItemFactory::ItemMap()
{
    static ItemMapT Map;
    return Map;
}

wxsItemFactory::ItemMapT::iterator wxsItemFactory::m_Iter(wxsItemFactory::ItemMap().end());
