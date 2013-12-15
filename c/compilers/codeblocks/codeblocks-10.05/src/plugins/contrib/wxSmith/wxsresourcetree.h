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
* $Revision: 5243 $
* $Id: wxsresourcetree.h 5243 2008-10-06 19:31:44Z killerbot $
* $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/wxSmith/wxsresourcetree.h $
*/

#ifndef WXSRESOURCETREE_H
#define WXSRESOURCETREE_H

#include <wx/treectrl.h>
#include "wxsresourcetreeitemdata.h"

#if defined(__WXMSW__) && defined(LoadImage)
    // Fix Windows winuser.h Header define of LoadImage.
    #undef LoadImage
#endif


class wxsResource;
class wxsProject;

/** \brief Definition of resource tree identifier. Separate name of type could be usefull in future */
typedef wxTreeItemId wxsResourceItemId;

// TODO: Rewrite this class with custom widget

/** \brief Resource tree class */
class wxsResourceTree: public wxTreeCtrl
{
    public:

        /** \brief Ctor */
        wxsResourceTree(wxWindow* Parent);

        /** \brief Dctor */
        virtual ~wxsResourceTree();

        /** \brief Function building new tree item for project
         *
         * This item takes name of associated cbProject and initializes
         * item data to point to project.
         */
        wxsResourceItemId NewProjectItem(const wxString& ProjectTitle,wxsProject* Project);

        /** \brief Function returning item id for external resources */
        wxsResourceItemId ExternalResourcesId();

        /** \brief Function deleting identifier for external resources */
        void DeleteExternalResourcesId();

        /** \brief Getting singleton instance */
        static inline wxsResourceTree* Get() { return m_Singleton; }

        /** \brief Getting global image list
         *
         * This list is declared as static, so it can be used even
         * before wxsResourceTree is created
         */
        static wxImageList& GetGlobalImageList();

        /** \brief Loading image to list
         *  \param FileName name relative to codeblock's data path
         *  \return Index in list
         */
        static int LoadImage(const wxString& FileName);

        /** \brief Adding bitmap into global list
         *  \param Bitmap bitmap to be added
         *  \return Index in list
         */
        static int InsertImage(const wxBitmap& Bitmap);

        /** \brief Freeing image at given index
         *
         * This function marks image at given index as freed, so
         * it may be replaced by LoadImage in future without
         * unnecessarily increasing size of list
         */
        static void FreeImage(int Index);

        /** \brief Blocking processing select events
         * \note This function must be paired with UnblockSelect
         */
        void BlockSelect();

        /** \brief Unblocking select events
         * \note This function must be paired with BlockSelect
         */
        void UnblockSelect();

    private:

        wxsResourceItemId m_ExtId;              ///< \brief id of tree item containing external resources
        bool m_IsExt;                           ///< \brief True if there is item for external resources
        int m_BlockCount;                       ///< \brief if > 0 must block all select events
        wxsResourceTreeItemData* m_Data;        ///< \brief Pointer to item data object used in last action
        static wxsResourceTree* m_Singleton;    ///< \brief singleton object

        static int m_RootImageId;
        static int m_ProjectImageId;
        static int m_ExternalImageId;

        static wxArrayInt& GetFreedList();

        void PopupMenu(wxMenu* Menu,wxsResourceTreeItemData* ItemData);
        void InvalidateItemData(wxsResourceTreeItemData* ItemData);

        void OnSelect(wxTreeEvent& event);
        void OnRightClick(wxTreeEvent& event);
        void OnPopupMenu(wxCommandEvent& event);

        friend void wxsResourceTreeItemData::PopupMenu(wxMenu* Menu);
        friend wxsResourceTreeItemData::~wxsResourceTreeItemData();

        DECLARE_EVENT_TABLE()
};

/** \brief Helper function for getting resource tree singleton object */
inline wxsResourceTree* wxsTree() { return wxsResourceTree::Get(); }

#endif
