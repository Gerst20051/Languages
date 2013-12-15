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
* $Id: wxsitemfactory.h 5243 2008-10-06 19:31:44Z killerbot $
* $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/wxSmith/wxwidgets/wxsitemfactory.h $
*/

#ifndef WXSITEMFACTORY_H
#define WXSITEMFACTORY_H

#include "wxsiteminfo.h"
#include "../wxsautoresourcetreeimage.h"
#include "../wxscodinglang.h"

#include <wx/string.h>
#include <wx/hashmap.h>
#include <wx/imaglist.h>

#include <configmanager.h>

#if defined(__WXMSW__) && defined(LoadImage)
    // Fix Windows winuser.h Header define of LoadImage.
    #undef LoadImage
#endif


class wxsItem;
class wxsItemResData;

/** \brief Class managing creation, destruction and enumeration of supported
 *         items
 *
 * All public functinos are static which means that this class is something
 * like singleton but using static members for global operations.
 * Each instance of this class does represent one wxsItem - provides it's info
 * and is able to build this item.
 * Addind new supported item to factory is done by deriving other class from
 * this one and writing it's
 */
class wxsItemFactory
{
	public:

        /** \brief Creating item with given name */
        static wxsItem* Build(const wxString& ClassName,wxsItemResData* Data);

        /** \brief Returning info for item with given name
         *  \param ClassName name of item's class
         *  \return Pointer to info or 0 if there's no such item
         */
        static const wxsItemInfo* GetInfo(const wxString& ClassName);

        /** \brief Getting onfo of first item */
        static const wxsItemInfo* GetFirstInfo();

        /** \brief Continuing getting item infos */
        static const wxsItemInfo* GetNextInfo();

        /** \brief Getting global image list with enteries for items
         *
         * In fact this function takes image list from resource browser
         */
        static wxImageList& GetImageList();

        /** \brief Loading icon into image list */
        static int LoadImage(const wxString& Name);

        /** \brief Ctor */
		wxsItemFactory(const wxsItemInfo* Info);

		/** \brief Extra ctor for templates - uses class name from param.
		 *         It may be used when Info is going to be created
		 *         inside constructor.
		 */
        wxsItemFactory(const wxsItemInfo* Info,wxString ClassName);

        /** \brief Dctor */
		virtual ~wxsItemFactory();

    protected:

        /** \brief Building item */
        virtual wxsItem* OnBuild(wxsItemResData* Data) = 0;

	private:

        WX_DECLARE_STRING_HASH_MAP(wxsItemFactory*,ItemMapT);

        /** \brief Function for getting global item's map */
        static ItemMapT& ItemMap();

        const wxsItemInfo* m_Info;          ///< \brief Info of item handled by this instance
        wxString m_Name;                    ///< \brief Item's name
        static ItemMapT::iterator m_Iter;   ///< \brief Iterator used for GetFirstInfo / GetNextInfo
};

/** \brief Template for easy registering of new items */
template<class T> class wxsRegisterItem: public wxsItemFactory
{
    public:

        /** \brief Publically available info - may be accessed from item */
        wxsItemInfo Info;

        /** \brief Ctor - bitmaps are generated from wxBitmap classes */
        wxsRegisterItem(
            wxString ClassName,
            wxsItemType Type,
            wxString License,
            wxString Author,
            wxString Email,
            wxString Site,
            wxString Category,
            long Priority,
            wxString DefaultVarName,
            long Languages,
            unsigned short VerHi,
            unsigned short VerLo,
            const wxBitmap& Bmp32,
            const wxBitmap& Bmp16,
            bool AllowInXRC = true
            ): wxsItemFactory(&Info,ClassName),
               m_TreeImage(Bmp16)
        {
            Info.ClassName = ClassName;
            Info.Type = Type;
            Info.License = License;
            Info.Author = Author;
            Info.Email = Email;
            Info.Site = Site;
            Info.Category = Category;
            Info.Priority = Priority;
            Info.DefaultVarName = DefaultVarName;
            Info.Languages = Languages;
            Info.VerHi = VerHi;
            Info.VerLo = VerLo;
            Info.Icon32 = Bmp32.GetSubBitmap(wxRect(0,0,Bmp32.GetWidth(),Bmp32.GetHeight()));
            Info.Icon16 = Bmp16.GetSubBitmap(wxRect(0,0,Bmp16.GetWidth(),Bmp16.GetHeight()));
            Info.AllowInXRC = AllowInXRC;
            Info.TreeIconId = m_TreeImage.GetIndex();
        }

        /** \brief Ctor - bitmaps are loaded from files */
        wxsRegisterItem(
            wxString ClassName,
            wxsItemType Type,
            wxString License,
            wxString Author,
            wxString Email,
            wxString Site,
            wxString Category,
            long Priority,
            wxString DefaultVarName,
            long Languages,
            unsigned short VerHi,
            unsigned short VerLo,
            wxString Bmp32FileName,
            wxString Bmp16FileName,
            bool AllowInXRC = true
            ): wxsItemFactory(&Info,ClassName),
               m_TreeImage(Bmp16FileName)
        {
            Info.ClassName = ClassName;
            Info.Type = Type;
            Info.License = License;
            Info.Author = Author;
            Info.Email = Email;
            Info.Site = Site;
            Info.Category = Category;
            Info.Priority = Priority;
            Info.DefaultVarName = DefaultVarName;
            Info.Languages = Languages;
            Info.VerHi = VerHi;
            Info.VerLo = VerLo;
            Info.AllowInXRC = AllowInXRC;

            wxString DataPath = ConfigManager::GetDataFolder()+_T("/");
            Info.Icon32.LoadFile(DataPath+Bmp32FileName,wxBITMAP_TYPE_ANY);
            Info.Icon16.LoadFile(DataPath+Bmp16FileName,wxBITMAP_TYPE_ANY);
            Info.TreeIconId = m_TreeImage.GetIndex();
        }

        /** \brief Ctor for built-in items from wxWidgets - sets most of data fields to default */
        wxsRegisterItem(
            wxString ClassNameWithoutWx,
            wxsItemType Type,
            wxString Category,
            long Priority,
            bool AllowInXRC=true):
                wxsItemFactory(&Info,_T("wx") + ClassNameWithoutWx),
                m_TreeImage(_T("images/wxsmith/wx")+ClassNameWithoutWx+_T("16.png"),true)
        {
            Info.ClassName = _T("wx") + ClassNameWithoutWx;
            Info.Type = Type;
            Info.License = _("wxWidgets license");
            Info.Author = _("wxWidgets team");
            Info.Email = _T("");
            Info.Site = _T("www.wxwidgets.org");
            Info.Category = Category;
            Info.Priority = Priority;
            Info.DefaultVarName = ClassNameWithoutWx;
            Info.Languages = wxsCPP;
            Info.VerHi = 2;
            Info.VerLo = 6;
            Info.AllowInXRC = AllowInXRC;

            wxString DataPath = ConfigManager::GetDataFolder() + _T("/images/wxsmith/");
            Info.Icon32.LoadFile(DataPath+Info.ClassName+_T("32.png"),wxBITMAP_TYPE_PNG);
            Info.Icon16.LoadFile(DataPath+Info.ClassName+_T("16.png"),wxBITMAP_TYPE_PNG);

            Info.TreeIconId = m_TreeImage.GetIndex();
        }

    protected:

        virtual wxsItem* OnBuild(wxsItemResData* Data)
        {
            return new T(Data);
        }

        wxsAutoResourceTreeImage m_TreeImage;
};

#endif
