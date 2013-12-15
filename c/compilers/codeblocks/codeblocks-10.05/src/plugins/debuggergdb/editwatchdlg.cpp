/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision: 6104 $
 * $Id: editwatchdlg.cpp 6104 2010-01-23 12:56:12Z mortenmacfly $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/debuggergdb/editwatchdlg.cpp $
 */

#include <sdk.h>
#include "editwatchdlg.h"
#include <wx/intl.h>
#include <wx/xrc/xmlres.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/radiobox.h>
#include <wx/spinctrl.h>

EditWatchDlg::EditWatchDlg(Watch* w, wxWindow* parent)
    : m_Watch(_T(""))
{
    //ctor
    wxXmlResource::Get()->LoadObject(this, parent, _T("dlgEditWatch"),_T("wxScrollingDialog"));

    if (w)
        m_Watch = *w;
    XRCCTRL(*this, "txtKeyword", wxTextCtrl)->SetValue(m_Watch.keyword);
    XRCCTRL(*this, "rbFormat", wxRadioBox)->SetSelection((int)m_Watch.format);
    XRCCTRL(*this, "chkArray", wxCheckBox)->SetValue(m_Watch.is_array);
    XRCCTRL(*this, "spnArrStart", wxSpinCtrl)->SetValue(m_Watch.array_start);
    XRCCTRL(*this, "spnArrCount", wxSpinCtrl)->SetValue(m_Watch.array_count);

    XRCCTRL(*this, "txtKeyword", wxTextCtrl)->SetFocus();
}

EditWatchDlg::~EditWatchDlg()
{
    //dtor
}

void EditWatchDlg::EndModal(int retCode)
{
    if (retCode == wxID_OK)
    {
        m_Watch.keyword = XRCCTRL(*this, "txtKeyword", wxTextCtrl)->GetValue();
        m_Watch.format = (WatchFormat)XRCCTRL(*this, "rbFormat", wxRadioBox)->GetSelection();
        m_Watch.is_array = XRCCTRL(*this, "chkArray", wxCheckBox)->GetValue();
        m_Watch.array_start = XRCCTRL(*this, "spnArrStart", wxSpinCtrl)->GetValue();
        m_Watch.array_count = XRCCTRL(*this, "spnArrCount", wxSpinCtrl)->GetValue();
    }
    wxScrollingDialog::EndModal(retCode);
}
