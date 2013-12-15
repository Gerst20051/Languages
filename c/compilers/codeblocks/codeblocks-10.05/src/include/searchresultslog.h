/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef SEARCHRESULTSLOG_H
#define SEARCHRESULTSLOG_H

#include <wx/string.h>
#include "loggers.h"

class wxArrayString;
class wxCommandEvent;

class SearchResultsLog : public ListCtrlLogger, public wxEvtHandler
{
	public:
		SearchResultsLog(const wxArrayString& titles, wxArrayInt& widths);
		virtual ~SearchResultsLog();
		void FocusEntry(size_t index);
		void SetBasePath(const wxString base){ m_Base = base; }

		virtual wxWindow* CreateControl(wxWindow* parent);
	protected:
        void OnDoubleClick(wxCommandEvent& event);
        void SyncEditor(int selIndex);

        wxString m_Base;
	private:
        DECLARE_EVENT_TABLE()
};

#endif // SEARCHRESULTSLOG_H
