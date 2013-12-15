/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef TODOSETTINGSDLG_H
#define TODOSETTINGSDLG_H

#include <wx/intl.h>
#include "configurationpanel.h"
#include <settings.h>

class ToDoSettingsDlg : public cbConfigurationPanel
{
	public:
		ToDoSettingsDlg(wxWindow* parent);
		~ToDoSettingsDlg();

        virtual wxString GetTitle() const { return _("Todo list"); }
        virtual wxString GetBitmapBaseName() const { return _T("todo"); }
        virtual void OnApply();
        virtual void OnCancel(){}
};

#endif // TODOSETTINGSDLG_H

