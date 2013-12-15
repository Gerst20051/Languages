/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * Copyright: 2008 Jens Lody
 *
 * $Revision: 6050 $
 * $Id: IncrementalSearchConfDlg.cpp 6050 2010-01-05 11:37:49Z jenslody $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/IncrementalSearch/IncrementalSearchConfDlg.cpp $
 */

#include "sdk.h"

#ifndef CB_PRECOMP
    #include <wx/xrc/xmlres.h>
    #include <wx/button.h>
    #include <wx/checkbox.h>
    #include <wx/choice.h>
    #include <wx/colordlg.h>
    #include <configmanager.h>
#endif

#include "IncrementalSearchConfDlg.h"

BEGIN_EVENT_TABLE(IncrementalSearchConfDlg,wxPanel)
    EVT_BUTTON(XRCID("btnIncSearchConfColourFound"),IncrementalSearchConfDlg::OnChooseColour)
    EVT_BUTTON(XRCID("btnIncSearchConfColourHighlight"),IncrementalSearchConfDlg::OnChooseColour)
    EVT_BUTTON(XRCID("btnIncSearchConfNotFoundBG"),IncrementalSearchConfDlg::OnChooseColour)
    EVT_BUTTON(XRCID("btnIncSearchConfWrappedBG"),IncrementalSearchConfDlg::OnChooseColour)
END_EVENT_TABLE()

IncrementalSearchConfDlg::IncrementalSearchConfDlg(wxWindow* parent)
{
    wxXmlResource::Get()->LoadObject(this,parent,_T("IncrementalSearchConfDlg"),_T("wxPanel"));

    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("editor"));

    // initialise colour-values
    XRCCTRL(*this, "btnIncSearchConfColourFound", wxButton)->SetBackgroundColour(cfg->ReadColour(_T("/incremental_search/text_found_colour"), wxColour(160, 32, 240)));
    XRCCTRL(*this, "btnIncSearchConfColourHighlight", wxButton)->SetBackgroundColour(cfg->ReadColour(_T("/incremental_search/highlight_colour"), wxColour(255, 165, 0)));
    XRCCTRL(*this, "btnIncSearchConfNotFoundBG", wxButton)->SetBackgroundColour(cfg->ReadColour(_T("/incremental_search/text_not_found_colour"), wxColour(255, 127, 127)));
    XRCCTRL(*this, "btnIncSearchConfWrappedBG", wxButton)->SetBackgroundColour(cfg->ReadColour(_T("/incremental_search/wrapped_colour"), wxColour(127, 127, 255)));

    // get value from conf-file or predefine them with default value
    XRCCTRL(*this, "chkIncSearchConfCenterText", wxCheckBox)->SetValue(cfg->ReadBool(_T("/incremental_search/center_found_text_on_screen"),true));
    XRCCTRL(*this, "idIncSearchSelectOnEscape", wxCheckBox)->SetValue(cfg->ReadBool(_T("/incremental_search/select_found_text_on_escape"),false));
    XRCCTRL(*this, "idIncSearchSelectOnFocus", wxCheckBox)->SetValue(cfg->ReadBool(_T("/incremental_search/select_text_on_focus"),false));
    XRCCTRL(*this, "idIncSearchHighlightDefault", wxChoice)->SetSelection(cfg->ReadInt(_T("/incremental_search/highlight_default_state"),0));
    XRCCTRL(*this, "idIncSearchSelectedDefault", wxChoice)->SetSelection(cfg->ReadInt(_T("/incremental_search/selected_default_state"),0));
    XRCCTRL(*this, "idIncSearchMatchCaseDefault", wxChoice)->SetSelection(cfg->ReadInt(_T("/incremental_search/match_case_default_state"),0));
    XRCCTRL(*this, "idIncSearchRegExDefault", wxChoice)->SetSelection(cfg->ReadInt(_T("/incremental_search/regex_default_state"),0));
}

IncrementalSearchConfDlg::~IncrementalSearchConfDlg()
{
}

void IncrementalSearchConfDlg::OnChooseColour(wxCommandEvent& event)
{
    wxColourData data;
    wxWindow* sender = FindWindowById(event.GetId());
    data.SetColour(sender->GetBackgroundColour());

    wxColourDialog dlg(this, &data);
    PlaceWindow(&dlg);
    if (dlg.ShowModal() == wxID_OK)
    {
        wxColour colour = dlg.GetColourData().GetColour();
        sender->SetBackgroundColour(colour);
    }
}

void IncrementalSearchConfDlg::SaveSettings()
{
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("editor"));
    // save checkbox/choice-settings
    cfg->Write(_T("/incremental_search/center_found_text_on_screen"),   XRCCTRL(*this, "chkIncSearchConfCenterText", wxCheckBox)->IsChecked());
    cfg->Write(_T("/incremental_search/select_found_text_on_escape"),   XRCCTRL(*this, "idIncSearchSelectOnEscape", wxCheckBox)->IsChecked());
    cfg->Write(_T("/incremental_search/select_text_on_focus"),          XRCCTRL(*this, "idIncSearchSelectOnFocus", wxCheckBox)->IsChecked());
    cfg->Write(_T("/incremental_search/highlight_default_state"),       XRCCTRL(*this, "idIncSearchHighlightDefault", wxChoice)->GetSelection());
    cfg->Write(_T("/incremental_search/selected_default_state"),        XRCCTRL(*this, "idIncSearchSelectedDefault", wxChoice)->GetSelection());
    cfg->Write(_T("/incremental_search/match_case_default_state"),      XRCCTRL(*this, "idIncSearchMatchCaseDefault", wxChoice)->GetSelection());
    cfg->Write(_T("/incremental_search/regex_default_state"),           XRCCTRL(*this, "idIncSearchRegExDefault", wxChoice)->GetSelection());

    // save colour-values
    cfg->Write(_T("/incremental_search/text_found_colour"),             XRCCTRL(*this, "btnIncSearchConfColourFound", wxButton)->GetBackgroundColour());
    cfg->Write(_T("/incremental_search/highlight_colour"),              XRCCTRL(*this, "btnIncSearchConfColourHighlight", wxButton)->GetBackgroundColour());
    cfg->Write(_T("/incremental_search/text_not_found_colour"),         XRCCTRL(*this, "btnIncSearchConfNotFoundBG", wxButton)->GetBackgroundColour());
    cfg->Write(_T("/incremental_search/wrapped_colour"),                XRCCTRL(*this, "btnIncSearchConfWrappedBG", wxButton)->GetBackgroundColour());
}

