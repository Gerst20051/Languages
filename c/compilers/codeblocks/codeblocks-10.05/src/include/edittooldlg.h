/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef EDITTOOLDLG_H
#define EDITTOOLDLG_H

#include "scrollingdialog.h"

class cbTool;
class wxCommandEvent;
class wxUpdateUIEvent;

class EditToolDlg : public wxScrollingDialog
{
	public:
		EditToolDlg(wxWindow* parent, cbTool* tool);
		~EditToolDlg();
		void EndModal(int retCode);
	private:
		void OnUpdateUI(wxUpdateUIEvent& event);
		void OnBrowseCommand(wxCommandEvent& event);
		void OnBrowseDir(wxCommandEvent& event);

		cbTool* m_Tool;   //!< the tool we are editing (setting up)

		DECLARE_EVENT_TABLE()
};

#endif // EDITTOOLDLG_H

