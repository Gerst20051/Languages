/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef IBASELOADER_H
#define IBASELOADER_H

#include <wx/string.h>
#include "settings.h"
#include "tinyxml/tinyxml.h"

/** Base interface for project loaders. */
class DLLIMPORT IBaseLoader
{
	public:
        /// Constructor
		IBaseLoader(){}
		/// Destructor
		virtual ~IBaseLoader(){}

        /** Open a file.
          * @param filename The file to open.
          * @return True on success, false on failure. */
        virtual bool Open(const wxString& filename) = 0;

        /** Save a file.
          * @param filename The file to save.
          * @return True on success, false on failure. */
        virtual bool Save(const wxString& filename) = 0;
    protected:
};

#endif // IBASELOADER_H
