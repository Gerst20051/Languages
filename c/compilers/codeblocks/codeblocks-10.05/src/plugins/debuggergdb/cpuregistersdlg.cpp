/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision: 5931 $
 * $Id: cpuregistersdlg.cpp 5931 2009-11-21 15:58:42Z biplab $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/debuggergdb/cpuregistersdlg.cpp $
 */

#include <sdk.h>
#include "cpuregistersdlg.h"
#include "debuggergdb.h"
#include <wx/intl.h>
#include <wx/sizer.h>
#include <wx/listctrl.h>

BEGIN_EVENT_TABLE(CPURegistersDlg, wxPanel)
//    EVT_BUTTON(XRCID("btnRefresh"), CPURegistersDlg::OnRefresh)
END_EVENT_TABLE()

CPURegistersDlg::CPURegistersDlg(wxWindow* parent, DebuggerGDB* debugger)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize),
    m_pDbg(debugger)
{
    //ctor
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    m_pList = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
    sizer->Add(m_pList, 1, wxGROW);
    SetSizer(sizer);
    Layout();

    wxFont font(8, wxMODERN, wxNORMAL, wxNORMAL);
    m_pList->SetFont(font);

    Clear();
}

CPURegistersDlg::~CPURegistersDlg()
{
    //dtor
}

void CPURegistersDlg::Clear()
{
    m_pList->ClearAll();
    m_pList->Freeze();
    m_pList->DeleteAllItems();
    m_pList->InsertColumn(0, _("Register"), wxLIST_FORMAT_LEFT);
    m_pList->InsertColumn(1, _("Hex"), wxLIST_FORMAT_RIGHT);
    m_pList->InsertColumn(2, _("Integer"), wxLIST_FORMAT_RIGHT);
    m_pList->Thaw();
}

int CPURegistersDlg::RegisterIndex(const wxString& reg_name)
{
    for (int i = 0; i < m_pList->GetItemCount(); ++i)
    {
        if (m_pList->GetItemText(i).CmpNoCase(reg_name) == 0)
            return i;
    }
    return -1;
}

void CPURegistersDlg::SetRegisterValue(const wxString& reg_name, unsigned long int value)
{
    // find existing register
    int idx = RegisterIndex(reg_name);
    if (idx == -1)
    {
        // if it doesn't exist, add it
        idx = m_pList->GetItemCount();
        m_pList->InsertItem(idx, reg_name);
    }

    wxString fmt;
    fmt.Printf(_T("0x%x"), (size_t)value);
    m_pList->SetItem(idx, 1, fmt);
    fmt.Printf(_T("%lu"), value);
    m_pList->SetItem(idx, 2, fmt);

    for (int i = 0; i < 3; ++i)
    {
        m_pList->SetColumnWidth(i, wxLIST_AUTOSIZE);
    }
}
