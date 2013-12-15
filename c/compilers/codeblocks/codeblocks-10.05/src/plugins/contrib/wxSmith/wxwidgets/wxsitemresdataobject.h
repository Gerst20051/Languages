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
* $Id: wxsitemresdataobject.h 4850 2008-01-29 21:45:49Z byo $
* $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/wxSmith/wxwidgets/wxsitemresdataobject.h $
*/

#ifndef WXSITEMRESDATAOBJECT_H
#define WXSITEMRESDATAOBJECT_H

#include <wx/dataobj.h>
#include <tinyxml/tinyxml.h>

#define wxsDF_WIDGET   _T("wxSmith XML")

class wxsItem;
class wxsItemResData;

/** \brief Class representing one or more items with resource structure using wxDataObject class */
class wxsItemResDataObject : public wxDataObject
{
	public:

        /** \brief Ctor */
		wxsItemResDataObject();

		/** \brief Dctor */
		virtual ~wxsItemResDataObject();

        //=====================================
        // Opertating on data
        //=====================================

        /** \brief Clering all data */
        void Clear();

		/** \brief Adding widget into this data object */
		bool AddItem(wxsItem* Item);

		/** \brief Getting number of handled widgets inside this object */
		int GetItemCount() const;

		/** \brief Building wxsItem class from this data object
		 *  \param Resource - resource owning item
		 *  \param Index - id of item (in range 0..GetWidgetCount()-1)
		 *  \return created item or 0 on error
		 */
		wxsItem* BuildItem(wxsItemResData* Data,int Index = 0) const;

		/** \brief Setting Xml string describing widget */
		bool SetXmlData(const wxString& Data);

		/** \brief Getting Xml strting desecribing widget */
		wxString GetXmlData() const;

        //=====================================
        // Members of wxDataObject class
        //=====================================

		/** \brief Enumerating all data formats.
		 *
		 * Formats available for reading and writing:
         * - wxDF_TEXT
         * - internal type ("wxSmith XML")
         */
		virtual void GetAllFormats(wxDataFormat *formats,Direction dir) const;

		/** \brief Copying data to raw buffer */
		virtual bool GetDataHere(const wxDataFormat& format,void *buf) const;

		/** \brief Returns number of data bytes */
		virtual size_t GetDataSize(const wxDataFormat& format) const;

		/** \brief Returns number of suported formats (in both cases - 2) */
		virtual size_t GetFormatCount(Direction dir) const;

		/** \brief Returning best format - "wxSmith XML" */
		virtual wxDataFormat GetPreferredFormat(Direction dir) const;

		/** \brief Setting data - will load Xml data */
		virtual bool SetData(const wxDataFormat& format,size_t len,const void *buf);

    private:

        TiXmlDocument m_XmlDoc;
        TiXmlElement* m_XmlElem;
        int m_ItemCount;
};

#endif
