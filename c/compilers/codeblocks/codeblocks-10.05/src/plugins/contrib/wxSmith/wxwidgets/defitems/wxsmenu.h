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
* $Id: wxsmenu.h 4850 2008-01-29 21:45:49Z byo $
* $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/wxSmith/wxwidgets/defitems/wxsmenu.h $
*/

#ifndef WXSMENU_H
#define WXSMENU_H

#include "../wxstool.h"

/** \brief handler for menus */
class wxsMenu: public wxsTool
{
    public:

        wxsMenu(wxsItemResData* Data);

    private:

        virtual void OnBuildCreatingCode();
        virtual void OnEnumToolProperties(long Flags);
        virtual bool OnIsPointer();
        virtual bool OnCanAddToResource(wxsItemResData* Data,bool ShowMessage) { return true; }
        virtual bool OnMouseDClick(wxWindow* Preview,int PosX,int PosY);
        virtual bool OnCanAddChild(wxsItem* Item,bool ShowMessage);
        virtual bool OnCanAddToParent(wxsParent* Item,bool ShowMessage);
        virtual bool OnXmlReadChild(TiXmlElement* Elem,bool IsXRC,bool IsExtra);
        virtual wxString OnGetTreeLabel(int& Image);

//        wxSize CalculateSize(wxArrayInt* Cols = 0); ///< \brief Getting size of this item requied by editor
//        void   Draw(wxDC* DC,int BegX,int BegY);    ///< \brief Drawing preview of this menu in some DC

        wxString m_Label;                           ///< \brief Main label

        friend class wxsMenuEditor;
};

#endif
