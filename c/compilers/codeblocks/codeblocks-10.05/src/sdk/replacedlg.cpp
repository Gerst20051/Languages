/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision: 6104 $
 * $Id: replacedlg.cpp 6104 2010-01-23 12:56:12Z mortenmacfly $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/sdk/replacedlg.cpp $
 */

#include "sdk_precomp.h"
#ifndef CB_PRECOMP
    #include "globals.h"
    #include "configmanager.h"

    #include <wx/xrc/xmlres.h>
    #include <wx/intl.h>
    #include <wx/combobox.h>
    #include <wx/checkbox.h>
    #include <wx/button.h>
    #include <wx/notebook.h>
    #include <wx/radiobox.h>
#endif

#include "replacedlg.h"


#define CONF_GROUP _T("/replace_options")

BEGIN_EVENT_TABLE(ReplaceDlg, wxScrollingDialog)
    EVT_NOTEBOOK_PAGE_CHANGED(XRCID("nbReplace"), ReplaceDlg::OnFindChange)
	EVT_CHECKBOX(XRCID("chkRegEx1"), 	ReplaceDlg::OnRegEx)
	EVT_ACTIVATE(                       ReplaceDlg::OnActivate)
END_EVENT_TABLE()

ReplaceDlg::ReplaceDlg(wxWindow* parent, const wxString& initial, bool hasSelection,
    bool findInFilesOnly, bool replaceInFilesActive)
    : FindReplaceBase(parent, initial, hasSelection),
    m_Complete(!replaceInFilesActive)
{
	wxXmlResource::Get()->LoadObject(this, parent, _T("dlgReplace"),_T("wxScrollingDialog"));
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("editor"));

	// load last searches
	FillComboWithLastValues(XRCCTRL(*this, "cmbFind1", wxComboBox), CONF_GROUP _T("/last"));
	FillComboWithLastValues(XRCCTRL(*this, "cmbReplace1", wxComboBox), CONF_GROUP _T("/lastReplace"));
	FillComboWithLastValues(XRCCTRL(*this, "cmbFind2", wxComboBox), CONF_GROUP _T("/last"));
	FillComboWithLastValues(XRCCTRL(*this, "cmbReplace2", wxComboBox), CONF_GROUP _T("/lastReplace"));

	// replace options
	XRCCTRL(*this, "cmbFind1", wxComboBox)->SetValue(initial);
	XRCCTRL(*this, "chkWholeWord1", wxCheckBox)->SetValue(cfg->ReadBool(CONF_GROUP _T("/match_word"), false));
	XRCCTRL(*this, "chkStartWord1", wxCheckBox)->SetValue(cfg->ReadBool(CONF_GROUP _T("/start_word"), false));
	XRCCTRL(*this, "chkMatchCase1", wxCheckBox)->SetValue(cfg->ReadBool(CONF_GROUP _T("/match_case"), false));
	XRCCTRL(*this, "chkRegEx1", wxCheckBox)->SetValue(cfg->ReadBool(CONF_GROUP _T("/regex"), false));
	XRCCTRL(*this, "rbDirection", wxRadioBox)->SetSelection(cfg->ReadInt(CONF_GROUP _T("/direction"), 1));
	XRCCTRL(*this, "rbDirection", wxRadioBox)->Enable(!XRCCTRL(*this, "chkRegEx1", wxCheckBox)->GetValue()); // if regex, only forward searches
	XRCCTRL(*this, "rbOrigin", wxRadioBox)->SetSelection(cfg->ReadInt(CONF_GROUP _T("/origin"), 0));
	XRCCTRL(*this, "rbScope1", wxRadioBox)->SetSelection(hasSelection);
	//XRCCTRL(*this, "rbScope1", wxRadioBox)->Enable(hasSelection);
	// special key, uses same config for both find & replace options
	XRCCTRL(*this, "chkAutoWrapSearch", wxCheckBox)->SetValue(cfg->ReadBool(_T("/find_options/auto_wrap_search"), true));

	// replace in files options
	XRCCTRL(*this, "cmbFind2", wxComboBox)->SetValue(initial);
	XRCCTRL(*this, "chkWholeWord2", wxCheckBox)->SetValue(cfg->ReadBool(CONF_GROUP _T("/match_word2"), false));
	XRCCTRL(*this, "chkStartWord2", wxCheckBox)->SetValue(cfg->ReadBool(CONF_GROUP _T("/start_word2"), false));
	XRCCTRL(*this, "chkMatchCase2", wxCheckBox)->SetValue(cfg->ReadBool(CONF_GROUP _T("/match_case2"), false));
	XRCCTRL(*this, "chkRegEx2", wxCheckBox)->SetValue(cfg->ReadBool(CONF_GROUP _T("/regex2"), false));
	XRCCTRL(*this, "rbScope2", wxRadioBox)->SetSelection(cfg->ReadInt(CONF_GROUP _T("/scope2"), 0));

    if (findInFilesOnly)
    {
        XRCCTRL(*this, "nbReplace", wxNotebook)->DeletePage(0); // no active editor, so only replace-in-files
        XRCCTRL(*this, "cmbFind2", wxComboBox)->SetFocus();
    }
	else if (replaceInFilesActive)
	{
		XRCCTRL(*this, "nbReplace", wxNotebook)->SetSelection(1); // Search->Replace in Files was selected
		XRCCTRL(*this, "cmbFind2", wxComboBox)->SetFocus();
	}
}

ReplaceDlg::~ReplaceDlg()
{
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("editor"));

	// save last searches (up to 10)
	if (IsFindInFiles())
	{
	    SaveComboValues(XRCCTRL(*this, "cmbFind2", wxComboBox), CONF_GROUP _T("/last"));
        SaveComboValues(XRCCTRL(*this, "cmbReplace2", wxComboBox), CONF_GROUP _T("/lastReplace"));
	}
	else
	{
	    SaveComboValues(XRCCTRL(*this, "cmbFind1", wxComboBox), CONF_GROUP _T("/last"));
        SaveComboValues(XRCCTRL(*this, "cmbReplace1", wxComboBox), CONF_GROUP _T("/lastReplace"));
	}

    if (m_Complete)
	{
        // find(replace) options
        cfg->Write(CONF_GROUP _T("/match_word"), XRCCTRL(*this, "chkWholeWord1", wxCheckBox)->GetValue());
        cfg->Write(CONF_GROUP _T("/start_word"), XRCCTRL(*this, "chkStartWord1", wxCheckBox)->GetValue());
        cfg->Write(CONF_GROUP _T("/match_case"), XRCCTRL(*this, "chkMatchCase1", wxCheckBox)->GetValue());
        cfg->Write(CONF_GROUP _T("/regex"), XRCCTRL(*this, "chkRegEx1", wxCheckBox)->GetValue());
        cfg->Write(CONF_GROUP _T("/direction"), XRCCTRL(*this, "rbDirection", wxRadioBox)->GetSelection());
        cfg->Write(CONF_GROUP _T("/origin"), XRCCTRL(*this, "rbOrigin", wxRadioBox)->GetSelection());
        // special key, uses same config for both find & replace options
        cfg->Write(_T("/find_options/auto_wrap_search"), XRCCTRL(*this, "chkAutoWrapSearch", wxCheckBox)->GetValue());
	}

	// find(replace) in files options
	cfg->Write(CONF_GROUP _T("/match_word2"), XRCCTRL(*this, "chkWholeWord2", wxCheckBox)->GetValue());
	cfg->Write(CONF_GROUP _T("/start_word2"), XRCCTRL(*this, "chkStartWord2", wxCheckBox)->GetValue());
	cfg->Write(CONF_GROUP _T("/match_case2"), XRCCTRL(*this, "chkMatchCase2", wxCheckBox)->GetValue());
	cfg->Write(CONF_GROUP _T("/regex2"), XRCCTRL(*this, "chkRegEx2", wxCheckBox)->GetValue());
	cfg->Write(CONF_GROUP _T("/scope2"), XRCCTRL(*this, "rbScope2", wxRadioBox)->GetSelection());
}

void ReplaceDlg::FillComboWithLastValues(wxComboBox* combo, const wxString& configKey)
{
	wxArrayString values;
	Manager::Get()->GetConfigManager(_T("editor"))->Read(configKey, &values);
	for (unsigned int i = 0; i < values.GetCount(); ++i)
	{
		if (!values[i].IsEmpty())
			combo->Append(values[i]);
	}
}

void ReplaceDlg::SaveComboValues(wxComboBox* combo, const wxString& configKey)
{
	wxArrayString values;
	for (int i = 0; (i < (int)combo->GetCount()) && (i < 10); ++i)
	{
		if (!combo->GetString(i).IsEmpty() && (values.Index(combo->GetString(i)) == wxNOT_FOUND))
			values.Add(combo->GetString(i));
	}
	wxString find = combo->GetValue();
    int prev_pos = values.Index(find);
    if (prev_pos != wxNOT_FOUND)
        values.RemoveAt(prev_pos);
    values.Insert(find, 0);
	Manager::Get()->GetConfigManager(_T("editor"))->Write(configKey, values);
}

wxString ReplaceDlg::GetFindString() const
{
    if (IsFindInFiles())
        return XRCCTRL(*this, "cmbFind2", wxComboBox)->GetValue();
    else
        return XRCCTRL(*this, "cmbFind1", wxComboBox)->GetValue();
}

wxString ReplaceDlg::GetReplaceString() const
{
    if (IsFindInFiles())
        return XRCCTRL(*this, "cmbReplace2", wxComboBox)->GetValue();
    else
        return XRCCTRL(*this, "cmbReplace1", wxComboBox)->GetValue();
}

bool ReplaceDlg::IsFindInFiles() const
{
	return !m_Complete || XRCCTRL(*this, "nbReplace", wxNotebook)->GetSelection() == 1;
}

bool ReplaceDlg::GetDeleteOldSearches() const
{
    return false; // checkbox doesn't exist in both dialogs
}

bool ReplaceDlg::GetMatchWord() const
{
    if (IsFindInFiles())
		return XRCCTRL(*this, "chkWholeWord2", wxCheckBox)->GetValue();
	else
        return XRCCTRL(*this, "chkWholeWord1", wxCheckBox)->GetValue();
}

bool ReplaceDlg::GetStartWord() const
{
    if (IsFindInFiles())
		return XRCCTRL(*this, "chkStartWord2", wxCheckBox)->GetValue();
	else
        return XRCCTRL(*this, "chkStartWord1", wxCheckBox)->GetValue();
}

bool ReplaceDlg::GetMatchCase() const
{
    if (IsFindInFiles())
		return XRCCTRL(*this, "chkMatchCase2", wxCheckBox)->GetValue();
	else
        return XRCCTRL(*this, "chkMatchCase1", wxCheckBox)->GetValue();
}

bool ReplaceDlg::GetRegEx() const
{
    if (IsFindInFiles())
		return XRCCTRL(*this, "chkRegEx2", wxCheckBox)->GetValue();
	else
		return XRCCTRL(*this, "chkRegEx1", wxCheckBox)->GetValue();
}

bool ReplaceDlg::GetAutoWrapSearch() const
{
    if (IsFindInFiles())
		return false; // not for replace in files
	else
		return XRCCTRL(*this, "chkAutoWrapSearch", wxCheckBox)->GetValue();
}

bool ReplaceDlg::GetFindUsesSelectedText() const
{
    return false; // not for replace
}

int ReplaceDlg::GetDirection() const
{
    if (IsFindInFiles())
        return 1;
    else
        return XRCCTRL(*this, "rbDirection", wxRadioBox)->GetSelection();
}

int ReplaceDlg::GetOrigin() const
{
    if (IsFindInFiles())
        return 1;
    else
        return XRCCTRL(*this, "rbOrigin", wxRadioBox)->GetSelection();
}

int ReplaceDlg::GetScope() const
{
    if (IsFindInFiles())
        return XRCCTRL(*this, "rbScope2", wxRadioBox)->GetSelection();
    else
        return XRCCTRL(*this, "rbScope1", wxRadioBox)->GetSelection();
}

// events

void ReplaceDlg::OnFindChange(wxNotebookEvent& event)
{
    if (    (event.GetOldSelection() == 0 && event.GetSelection() == 1)
         || (event.GetOldSelection() == 1 && event.GetSelection() == 0))
    {
        wxComboBox* cmbFind1 = XRCCTRL(*this, "cmbFind1", wxComboBox);
        wxComboBox* cmbFind2 = XRCCTRL(*this, "cmbFind2", wxComboBox);
        wxComboBox* cmbReplace1 =XRCCTRL(*this, "cmbReplace1", wxComboBox);
        wxComboBox* cmbReplace2 =XRCCTRL(*this, "cmbReplace2", wxComboBox);

        if (event.GetSelection() == 0)
        {
            cmbFind1->SetValue(cmbFind2->GetValue());
            cmbReplace1->SetValue(cmbReplace2->GetValue());
            cmbFind1->SetFocus();            
        }
        else if (event.GetSelection() == 1)
        {
            cmbFind2->SetValue(cmbFind1->GetValue());
            cmbReplace2->SetValue(cmbReplace1->GetValue());
            cmbFind2->SetFocus();            
        }
    }    
    event.Skip();
}

void ReplaceDlg::OnRegEx(wxCommandEvent& event)
{
    if (m_Complete)
        XRCCTRL(*this, "rbDirection", wxRadioBox)->Enable(!XRCCTRL(*this, "chkRegEx1", wxCheckBox)->GetValue());
}

void ReplaceDlg::OnActivate(wxActivateEvent& event)
{
    wxComboBox* cbp = 0;
    if (IsFindInFiles())
        cbp = XRCCTRL(*this, "cmbFind2", wxComboBox);
    else
        cbp = XRCCTRL(*this, "cmbFind1", wxComboBox);

    if (cbp != 0) cbp->SetFocus();
    event.Skip();
}
