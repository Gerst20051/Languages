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
* $Id: wxsresourcetreeitemdata.h 4850 2008-01-29 21:45:49Z byo $
* $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/wxSmith/wxsresourcetreeitemdata.h $
*/

#ifndef WXSRESOURCETREEITEMDATA_H
#define WXSRESOURCETREEITEMDATA_H

#include <wx/treectrl.h>
#include <wx/menu.h>

/** \brief Data used by resource tree */
class wxsResourceTreeItemData: public wxTreeItemData
{
    public:

        /** \brief Ctor */
        wxsResourceTreeItemData();

        /** \brief Dctor */
        virtual ~wxsResourceTreeItemData();

    protected:

        /** \brief Called when corresponding tree item has been selected */
        virtual void OnSelect() {}

        /** \brief Called when right click was made on item */
        virtual void OnRightClick() {}

        /** \brief Called when popup menu generated event with given id
         *  \return true when event was processed, false otherwise
         */
        virtual bool OnPopup(long Id) { return false; }


        /** \brief Function to properly display popup menu */
        void PopupMenu(wxMenu* Menu);

    private:

        friend class wxsResourceTree;
};

#endif
