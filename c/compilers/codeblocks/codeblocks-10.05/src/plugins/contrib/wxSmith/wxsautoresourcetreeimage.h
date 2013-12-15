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
* $Id: wxsautoresourcetreeimage.h 4850 2008-01-29 21:45:49Z byo $
* $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/wxSmith/wxsautoresourcetreeimage.h $
*/

#ifndef WXSAUTORESOURCETREEIMAGE_H
#define WXSAUTORESOURCETREEIMAGE_H
/** \brief This class helps automatically loading images into resource browser */

class wxString;
class wxBitmap;

class wxsAutoResourceTreeImage
{
    public:

        /** \brief Load image from file in data directory
         *  \param FileName name of file relative to C::B's data directory
         *  \param DontFree if true, image's index won't be freed automatically
         *         when class is deleted. Set to true only in case of some global
         *         images which will stay loaded as long as wxSmith is loaded.
         */
        wxsAutoResourceTreeImage(const wxString& FileName,bool DontFree=false);

        /** \brief Create image from given bitmap
         *  \param Image bitmap to be added
         *  \param DontFree if true, image's index won't be freed automatically
         *         when class is deleted. Set to true only in case of some global
         *         images which will stay loaded as long as wxSmith is loaded.
         */
        wxsAutoResourceTreeImage(const wxBitmap& Image,bool DontFree=false);

        /** \brief Dctor - marks image's index as free if necessary */
        ~wxsAutoResourceTreeImage();

        /** \brief Get index of handled image */
        inline int GetIndex() const { return m_Index; }

    private:

        int  m_Index;
        bool m_DontFree;
};



#endif
