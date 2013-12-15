/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef EDITARRAYSTRINGDLG_H
#define EDITARRAYSTRINGDLG_H

#include "scrollingdialog.h"
#include <wx/arrstr.h>
#include "settings.h"

class DLLIMPORT EditArrayStringDlg : public wxScrollingDialog
{
	public:
		EditArrayStringDlg(wxWindow* parent, wxArrayString& array);
		virtual ~EditArrayStringDlg();
		EditArrayStringDlg& operator=(const EditArrayStringDlg&){ return *this; } // just to satisfy script bindings (never used)
		virtual void EndModal(int retCode);
	protected:
		void OnAdd(wxCommandEvent& event);
		void OnEdit(wxCommandEvent& event);
		void OnDelete(wxCommandEvent& event);
		void OnUpdateUI(wxUpdateUIEvent& event);
		wxArrayString& m_Array;
	private:
		DECLARE_EVENT_TABLE()
};

#endif // EDITARRAYSTRINGDLG_H

