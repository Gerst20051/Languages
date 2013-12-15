/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef DEBUGGEROPTIONSDLG_H
#define DEBUGGEROPTIONSDLG_H

#include <wx/intl.h>
#include "configurationpanel.h"

class DebuggerGDB;

class DebuggerOptionsDlg : public cbConfigurationPanel
{
    public:
        DebuggerOptionsDlg(wxWindow* parent, DebuggerGDB* plugin);
        virtual ~DebuggerOptionsDlg();

        virtual wxString GetTitle() const { return _("Debugger settings"); }
        virtual wxString GetBitmapBaseName() const { return _T("debugger"); }
        virtual void OnApply();
        virtual void OnCancel(){}
    protected:
        DebuggerGDB* m_pPlugin;
    private:
};

#endif // DEBUGGEROPTIONSDLG_H
