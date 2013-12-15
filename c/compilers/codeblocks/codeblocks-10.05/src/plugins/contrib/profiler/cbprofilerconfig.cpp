/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision: 5854 $
 * $Id: cbprofilerconfig.cpp 5854 2009-10-09 05:26:51Z mortenmacfly $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/profiler/cbprofilerconfig.cpp $
 */

#include "sdk.h"
#ifndef CB_PRECOMP
    #include <wx/checkbox.h>
    #include <wx/event.h>
    #include <wx/spinctrl.h>
    #include <wx/textctrl.h>
    #include <wx/xrc/xmlres.h>
    #include "manager.h"
    #include "configmanager.h"
#endif
#include "cbprofilerconfig.h"

BEGIN_EVENT_TABLE(CBProfilerConfigDlg, wxPanel)
    EVT_CHECKBOX(XRCID("chkAnnSource"), CBProfilerConfigDlg::CheckBoxEvent)
    EVT_CHECKBOX(XRCID("chkMinCount"),  CBProfilerConfigDlg::CheckBoxEvent)
END_EVENT_TABLE()

CBProfilerConfigDlg::CBProfilerConfigDlg(wxWindow* parent)
{
    //ctor
    wxXmlResource::Get()->LoadPanel(this, parent, _T("dlgCBProfilerConfig"));
    LoadSettings();
}

CBProfilerConfigDlg::~CBProfilerConfigDlg()
{
    //dtor
}

void CBProfilerConfigDlg::CheckBoxEvent(wxCommandEvent& event)
{
    if(event.GetId() == XRCID("chkAnnSource"))
        XRCCTRL(*this, "txtAnnSource", wxTextCtrl)->Enable(XRCCTRL(*this, "chkAnnSource", wxCheckBox)->GetValue());
    if(event.GetId() == XRCID("chkMinCount"))
        XRCCTRL(*this, "spnMinCount", wxSpinCtrl)->Enable(XRCCTRL(*this, "chkMinCount", wxCheckBox)->GetValue());
}

void CBProfilerConfigDlg::LoadSettings()
{
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("cbprofiler"));

    // Values to be used many times along
    bool annSource = cfg->ReadBool(_T("/ann_source_chk"), false);
    bool minCount  = cfg->ReadBool(_T("/min_count_chk"),  false);

    // Output Options
    XRCCTRL(*this, "chkAnnSource",       wxCheckBox)->SetValue(annSource);
    XRCCTRL(*this, "txtAnnSource",       wxTextCtrl)->SetValue(cfg->Read(_T("/ann_source_txt"),        wxEmptyString));
    XRCCTRL(*this, "chkBrief",           wxCheckBox)->SetValue(cfg->ReadBool(_T("/brief"),             false));
    XRCCTRL(*this, "chkFileInfo",        wxCheckBox)->SetValue(cfg->ReadBool(_T("/file_info"),         false));
    XRCCTRL(*this, "chkUnusedFunctions", wxCheckBox)->SetValue(cfg->ReadBool(_T("/unused_functions"),  false));
    XRCCTRL(*this, "chkStaticCallGraph", wxCheckBox)->SetValue(cfg->ReadBool(_T("/static_call_graph"), false));

    // Analysis Options
    XRCCTRL(*this, "chkNoStatic",        wxCheckBox)->SetValue(cfg->ReadBool(_T("/no_static"),         false));
    XRCCTRL(*this, "chkMinCount",        wxCheckBox)->SetValue(minCount);
    XRCCTRL(*this, "spnMinCount",        wxSpinCtrl)->SetValue(cfg->ReadInt(_T("/min_count_spn"),      0));

    // Miscellaneous Options
    XRCCTRL(*this, "chkSum",             wxCheckBox)->SetValue(cfg->ReadBool(_T("/sum"),               false));

    // Extra Options
    XRCCTRL(*this, "txtExtra",           wxTextCtrl)->SetValue(cfg->Read(_T("/extra_txt"),             wxEmptyString));

    // Enable/Disable the TextCtrl(s) as well as SpinCtrl(s)
    XRCCTRL(*this, "txtAnnSource",       wxTextCtrl)->Enable(annSource);
    XRCCTRL(*this, "spnMinCount",        wxSpinCtrl)->Enable(minCount);
}

void CBProfilerConfigDlg::SaveSettings()
{
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("cbprofiler"));

    // Output Options
    cfg->Write(_T("/ann_source_chk"),    XRCCTRL(*this, "chkAnnSource",       wxCheckBox)->GetValue());
    cfg->Write(_T("/ann_source_txt"),    XRCCTRL(*this, "txtAnnSource",       wxTextCtrl)->GetValue());
    cfg->Write(_T("/brief"),             XRCCTRL(*this, "chkBrief",           wxCheckBox)->GetValue());
    cfg->Write(_T("/file_info"),         XRCCTRL(*this, "chkFileInfo",        wxCheckBox)->GetValue());
    cfg->Write(_T("/unused_functions"),  XRCCTRL(*this, "chkUnusedFunctions", wxCheckBox)->GetValue());
    cfg->Write(_T("/static_call_graph"), XRCCTRL(*this, "chkStaticCallGraph", wxCheckBox)->GetValue());

    // Analysis Options
    cfg->Write(_T("/no_static"),         XRCCTRL(*this, "chkNoStatic",        wxCheckBox)->GetValue());
    cfg->Write(_T("/min_count_chk"),     XRCCTRL(*this, "chkMinCount",        wxCheckBox)->GetValue());
    cfg->Write(_T("/min_count_spn"),     XRCCTRL(*this, "spnMinCount",        wxSpinCtrl)->GetValue());

    // Miscellaneous Options
    cfg->Write(_T("/sum"),               XRCCTRL(*this, "chkSum",             wxCheckBox)->GetValue());

    // Extra Options
    cfg->Write(_T("/extra_txt"),         XRCCTRL(*this, "txtExtra",           wxTextCtrl)->GetValue());
}

void CBProfilerConfigDlg::OnApply()
{
    // user pressed OK; save settings
    SaveSettings();
}
