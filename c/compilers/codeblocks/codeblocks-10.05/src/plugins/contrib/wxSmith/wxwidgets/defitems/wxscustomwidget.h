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
* $Id: wxscustomwidget.h 4850 2008-01-29 21:45:49Z byo $
* $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/wxSmith/wxwidgets/defitems/wxscustomwidget.h $
*/

#ifndef WXSCUSTOMWIDGET_H
#define WXSCUSTOMWIDGET_H

#include "../wxswidget.h"

/** \brief Class for custom widgets
 *
 * Using custom widget class, User can add it's own widgets here.
 * Because widgets in XRC must be threated differently from the ones
 * created using source code, XRC will use additional xml configuration,
 * source code will must have creating code defined.
 */
class wxsCustomWidget: public wxsWidget
{
    public:
        wxsCustomWidget(wxsItemResData* Data);

    private:

        virtual void OnBuildCreatingCode();
        virtual wxObject* OnBuildPreview(wxWindow* Parent,long Flags);
        virtual void OnEnumWidgetProperties(long Flags);
        virtual bool OnIsPointer() { return true; }
        virtual bool OnXmlRead(TiXmlElement* Element,bool IsXRC,bool IsExtra);
        virtual bool OnXmlWrite(TiXmlElement* Element,bool IsXRC,bool IsExtra);

        void RebuildXmlData();
        bool RebuildXmlDataDoc();

        wxString m_CreatingCode;
        wxString m_Style;
        wxString m_XmlData;
        TiXmlDocument m_XmlDataDoc;
        wxString m_IncludeFile;
        bool m_IncludeIsLocal;

};

#endif
