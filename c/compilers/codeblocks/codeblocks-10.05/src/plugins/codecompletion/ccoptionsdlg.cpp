/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision: 6082 $
 * $Id: ccoptionsdlg.cpp 6082 2010-01-13 13:52:20Z mortenmacfly $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/codecompletion/ccoptionsdlg.cpp $
 */

#include <sdk.h>
#include "ccoptionsdlg.h"
#include "codecompletion.h"
#include <wx/intl.h>
#include <wx/listbox.h>
#include <wx/xrc/xmlres.h>
#include <wx/spinctrl.h>
#include <wx/checkbox.h>
#include <wx/combobox.h>
#include <wx/treectrl.h>
#include <wx/slider.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/colordlg.h>
#include <wx/regex.h>
#include <configmanager.h>
#include <manager.h>
#include <editpairdlg.h>
#include <globals.h>

static const wxString g_SampleClasses =
    _T("class A_class"
    "{"
    "    public:"
    "        int someInt_A;"
    "    protected:"
    "        bool mSomeVar_A;"
    "    private:"
    "        char* mData_A;"
    "};"
    "class B_class"
    "{"
    "    public:"
    "        int someInt_B;"
    "    protected:"
    "        bool mSomeVar_B;"
    "    private:"
    "        char* mData_B;"
    "};"
    "class C_class : public A_class"
    "{"
    "    public:"
    "        int someInt_C;"
    "    protected:"
    "        bool mSomeVar_C;"
    "    private:"
    "        char* mData_C;"
    "};"
    "enum SomeEnum"
    "{"
    "    optOne,"
    "    optTwo,"
    "    optThree"
    "};"
    "int x;"
    "int y;"
    "#define SOME_DEFINITION\n"
    "#define SOME_DEFINITION_2\n\n");

BEGIN_EVENT_TABLE(CCOptionsDlg, wxPanel)
    EVT_UPDATE_UI(-1, CCOptionsDlg::OnUpdateUI)
    EVT_BUTTON(XRCID("btnAddRepl"), CCOptionsDlg::OnAddRepl)
    EVT_BUTTON(XRCID("btnEditRepl"), CCOptionsDlg::OnEditRepl)
    EVT_BUTTON(XRCID("btnDelRepl"), CCOptionsDlg::OnDelRepl)
    EVT_BUTTON(XRCID("btnColour"), CCOptionsDlg::OnChooseColour)
    EVT_COMMAND_SCROLL(XRCID("sliderDelay"), CCOptionsDlg::OnSliderScroll)
END_EVENT_TABLE()

CCOptionsDlg::CCOptionsDlg(wxWindow* parent, NativeParser* np, CodeCompletion* cc)
    : m_Parser(this),
    m_pNativeParsers(np),
    m_pCodeCompletion(cc)
{
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("code_completion"));

    wxXmlResource::Get()->LoadPanel(this, parent, _T("dlgCCSettings"));

    XRCCTRL(*this, "chkLocals", wxCheckBox)->SetValue(m_Parser.Options().followLocalIncludes);
    XRCCTRL(*this, "chkGlobals", wxCheckBox)->SetValue(m_Parser.Options().followGlobalIncludes);
    XRCCTRL(*this, "chkPreprocessor", wxCheckBox)->SetValue(m_Parser.Options().wantPreprocessor);
    XRCCTRL(*this, "chkNoCC", wxCheckBox)->SetValue(!cfg->ReadBool(_T("/use_code_completion"), true));
    XRCCTRL(*this, "chkSimpleMode", wxCheckBox)->SetValue(!m_Parser.Options().useSmartSense);
    XRCCTRL(*this, "chkTypeMode", wxCheckBox)->SetValue(m_Parser.Options().whileTyping);
    XRCCTRL(*this, "chkCaseSensitive", wxCheckBox)->SetValue(m_Parser.Options().caseSensitive);
    XRCCTRL(*this, "chkEvalTooltip", wxCheckBox)->SetValue(cfg->ReadBool(_T("/eval_tooltip"), true));
    XRCCTRL(*this, "chkAutoSelectOne", wxCheckBox)->SetValue(cfg->ReadBool(_T("/auto_select_one"), false));
    XRCCTRL(*this, "chkAutoLaunch", wxCheckBox)->SetValue(cfg->ReadBool(_T("/auto_launch"), true));
    XRCCTRL(*this, "spnAutoLaunchChars", wxSpinCtrl)->SetValue(cfg->ReadInt(_T("/auto_launch_chars"), 4));
    XRCCTRL(*this, "spnMaxMatches", wxSpinCtrl)->SetValue(cfg->ReadInt(_T("/max_matches"), 16384));
    XRCCTRL(*this, "chkInheritance", wxCheckBox)->SetValue(m_Parser.ClassBrowserOptions().showInheritance);
    XRCCTRL(*this, "chkExpandNS", wxCheckBox)->SetValue(m_Parser.ClassBrowserOptions().expandNS);
    XRCCTRL(*this, "chkTreeMembers", wxCheckBox)->SetValue(m_Parser.ClassBrowserOptions().treeMembers);
    XRCCTRL(*this, "spnThreadsNum", wxSpinCtrl)->SetValue(cfg->ReadInt(_T("/max_threads"), 1));
    XRCCTRL(*this, "spnThreadsNum", wxSpinCtrl)->Enable(false);
    XRCCTRL(*this, "chkFloatCB", wxCheckBox)->SetValue(cfg->ReadBool(_T("/as_floating_window"), false));
    XRCCTRL(*this, "chkNoSB", wxCheckBox)->SetValue(!cfg->ReadBool(_T("/use_symbols_browser"), true));
    XRCCTRL(*this, "txtFillupChars", wxTextCtrl)->SetValue(cfg->Read(_T("/fillup_chars"), wxEmptyString));

    XRCCTRL(*this, "chkKL_1", wxCheckBox)->SetValue(cfg->ReadBool(_T("/lexer_keywords_set1"), true));
    XRCCTRL(*this, "chkKL_2", wxCheckBox)->SetValue(cfg->ReadBool(_T("/lexer_keywords_set2"), true));
    XRCCTRL(*this, "chkKL_3", wxCheckBox)->SetValue(cfg->ReadBool(_T("/lexer_keywords_set3"), false));
    XRCCTRL(*this, "chkKL_4", wxCheckBox)->SetValue(cfg->ReadBool(_T("/lexer_keywords_set4"), false));
    XRCCTRL(*this, "chkKL_5", wxCheckBox)->SetValue(cfg->ReadBool(_T("/lexer_keywords_set5"), false));
    XRCCTRL(*this, "chkKL_6", wxCheckBox)->SetValue(cfg->ReadBool(_T("/lexer_keywords_set6"), false));
    XRCCTRL(*this, "chkKL_7", wxCheckBox)->SetValue(cfg->ReadBool(_T("/lexer_keywords_set7"), false));
    XRCCTRL(*this, "chkKL_8", wxCheckBox)->SetValue(cfg->ReadBool(_T("/lexer_keywords_set8"), false));
    XRCCTRL(*this, "chkKL_9", wxCheckBox)->SetValue(cfg->ReadBool(_T("/lexer_keywords_set9"), false));

    int timerDelay = cfg->ReadInt(_T("/cc_delay"), 500);
    XRCCTRL(*this, "sliderDelay", wxSlider)->SetValue(timerDelay / 100);
    UpdateSliderLabel();

    const ConfigManagerContainer::StringToStringMap& repl = Tokenizer::GetTokenReplacementsMap();
    ConfigManagerContainer::StringToStringMap::const_iterator it = repl.begin();
    while (it != repl.end())
    {
        XRCCTRL(*this, "lstRepl", wxListBox)->Append(it->first + _T(" -> ") + it->second);
        ++it;
    }

//    m_Parser.ParseBuffer(g_SampleClasses, true);
//    m_Parser.BuildTree(*XRCCTRL(*this, "treeClasses", wxTreeCtrl));
}

CCOptionsDlg::~CCOptionsDlg()
{
}

void CCOptionsDlg::UpdateSliderLabel()
{
    int position = XRCCTRL(*this, "sliderDelay", wxSlider)->GetValue();
    wxString lbl;
    if (position >= 10)
        lbl.Printf(_("%d.%d sec"), position / 10, position % 10);
    else
        lbl.Printf(_("%d ms"), position * 100);
    XRCCTRL(*this, "lblDelay", wxStaticText)->SetLabel(lbl);
}

bool CCOptionsDlg::ValidateReplacementToken(wxString& from, wxString& to)
{
    // cut off any leading / trailing spaces
    from.Trim(true).Trim(false);
    to.Trim(true).Trim(false);

    if (to.IsEmpty())
    {
        // Allow removing a token, but ask the user if that's OK.
        if (cbMessageBox( _("This setup will replace the token with an empty string.\n"
                            "This will *remove* the token and probably break CC for some cases.\n"
                            "Do you really want to *remove* that token?"),
                          _("Confirmation"),
                          wxICON_QUESTION | wxYES_NO ) == wxID_YES)
        {
            return true;
        }
    }
    else if (to.Contains(from))
    {
        cbMessageBox(_("Replacement token cannot contain search token.\n"
                       "This would cause an infinite loop otherwise."),
                     _("Error"), wxICON_ERROR);
        return false;
    }

    wxRegEx re(_T("[A-Za-z_]+[0-9]*[A-Za-z_]*"));
    if (!re.Matches(from))
    {
        cbMessageBox(_("Search token can only contain alphanumeric characters and underscores."),
                     _("Error"), wxICON_ERROR);
        return false;
    }
    if (!re.Matches(to))
    {
        // Allow replacing with special characters only if the user says it's ok.
        if (cbMessageBox( _("You are replacing a token with a string that contains\n"
                            "characters other than alphanumeric and underscores.\n"
                            "This could make parsing the file impossible.\n"
                            "Are you sure?"),
                          _("Confirmation"),
                          wxICON_QUESTION | wxYES_NO ) != wxID_YES)
        {
            return false;
        }
    }

    return true;
}

void CCOptionsDlg::OnAddRepl(wxCommandEvent& event)
{
    wxString key;
    wxString value;
    EditPairDlg dlg(this, key, value, _("Add new replacement token"), EditPairDlg::bmDisable);
    PlaceWindow(&dlg);
    if (dlg.ShowModal() == wxID_OK)
    {
        if (ValidateReplacementToken(key, value))
        {
            Tokenizer::SetReplacementString(key, value);
            XRCCTRL(*this, "lstRepl", wxListBox)->Append(key + _T(" -> ") + value);
        }
    }
}

void CCOptionsDlg::OnEditRepl(wxCommandEvent& event)
{
    wxString key;
    wxString value;

    int sel = XRCCTRL(*this, "lstRepl", wxListBox)->GetSelection();
    if (sel == -1)
        return;

    key = XRCCTRL(*this, "lstRepl", wxListBox)->GetStringSelection();
    value = key;

    key = key.BeforeFirst(_T(' '));
    value = value.AfterLast(_T(' '));

    EditPairDlg dlg(this, key, value, _("Edit replacement token"), EditPairDlg::bmDisable);
    PlaceWindow(&dlg);
    if (dlg.ShowModal() == wxID_OK)
    {
        if (ValidateReplacementToken(key, value))
        {
            Tokenizer::SetReplacementString(key, value);
            XRCCTRL(*this, "lstRepl", wxListBox)->SetString(sel, key + _T(" -> ") + value);
        }
    }
}

void CCOptionsDlg::OnDelRepl(wxCommandEvent& event)
{
    int sel = XRCCTRL(*this, "lstRepl", wxListBox)->GetSelection();
    if (sel == -1)
        return;

    if (cbMessageBox(_("Are you sure you want to delete this replacement token?"),
                     _("Confirmation"), wxICON_QUESTION | wxYES_NO) == wxID_YES)
    {
        wxString key = XRCCTRL(*this, "lstRepl", wxListBox)->GetStringSelection();
        key = key.BeforeFirst(_T(' '));
        Tokenizer::RemoveReplacementString(key);
        XRCCTRL(*this, "lstRepl", wxListBox)->Delete(sel);
    }
}

void CCOptionsDlg::OnChooseColour(wxCommandEvent& event)
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

void CCOptionsDlg::OnSliderScroll(wxScrollEvent& event)
{
    UpdateSliderLabel();
}

void CCOptionsDlg::OnUpdateUI(wxUpdateUIEvent& event)
{
    bool en = !XRCCTRL(*this, "chkNoCC", wxCheckBox)->GetValue();
    bool auto_launch = XRCCTRL(*this, "chkAutoLaunch", wxCheckBox)->GetValue();

    XRCCTRL(*this, "chkCaseSensitive", wxCheckBox)->Enable(en);
    XRCCTRL(*this, "chkEvalTooltip", wxCheckBox)->Enable(en);
    XRCCTRL(*this, "chkAutoSelectOne", wxCheckBox)->Enable(en);
    XRCCTRL(*this, "chkAutoLaunch", wxCheckBox)->Enable(en);
    XRCCTRL(*this, "spnAutoLaunchChars", wxSpinCtrl)->Enable(en && auto_launch);
    XRCCTRL(*this, "spnMaxMatches", wxSpinCtrl)->Enable(en);
    XRCCTRL(*this, "sliderDelay", wxSlider)->Enable(en);
    XRCCTRL(*this, "chkSimpleMode", wxCheckBox)->Enable(en);
    XRCCTRL(*this, "chkTypeMode", wxCheckBox)->Enable(en);
    XRCCTRL(*this, "lblFillupChars", wxStaticText)->Enable(en);
    XRCCTRL(*this, "txtFillupChars", wxTextCtrl)->Enable(en);

    en = !XRCCTRL(*this, "chkNoSB", wxCheckBox)->GetValue();
    XRCCTRL(*this, "chkInheritance", wxCheckBox)->Enable(en);
    XRCCTRL(*this, "chkFloatCB", wxCheckBox)->Enable(en);

    int sel = XRCCTRL(*this, "lstRepl", wxListBox)->GetSelection();
    XRCCTRL(*this, "btnEditRepl", wxButton)->Enable(sel != -1);
    XRCCTRL(*this, "btnDelRepl", wxButton)->Enable(sel != -1);
}

void CCOptionsDlg::OnApply()
{
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("code_completion"));

    // force parser to read its options that we write in the config
    cfg->Write(_T("/use_code_completion"), (bool)!XRCCTRL(*this, "chkNoCC", wxCheckBox)->GetValue());
    cfg->Write(_T("/max_threads"), (int)XRCCTRL(*this, "spnThreadsNum", wxSpinCtrl)->GetValue());

    int timerDelay = XRCCTRL(*this, "sliderDelay", wxSlider)->GetValue() * 100;
    cfg->Write(_T("/cc_delay"), (int)timerDelay);
    m_Parser.ReadOptions();

    // set all other member options
    m_Parser.Options().followLocalIncludes = XRCCTRL(*this, "chkLocals", wxCheckBox)->GetValue();
    m_Parser.Options().followGlobalIncludes = XRCCTRL(*this, "chkGlobals", wxCheckBox)->GetValue();
    m_Parser.Options().wantPreprocessor = XRCCTRL(*this, "chkPreprocessor", wxCheckBox)->GetValue();
    cfg->Write(_T("/auto_select_one"), (bool)XRCCTRL(*this, "chkAutoSelectOne", wxCheckBox)->GetValue());
    cfg->Write(_T("/auto_launch"), (bool)XRCCTRL(*this, "chkAutoLaunch", wxCheckBox)->GetValue());
    cfg->Write(_T("/auto_launch_chars"), (int)XRCCTRL(*this, "spnAutoLaunchChars", wxSpinCtrl)->GetValue());
    cfg->Write(_T("/max_matches"), (int)XRCCTRL(*this, "spnMaxMatches", wxSpinCtrl)->GetValue());
    m_Parser.Options().caseSensitive = XRCCTRL(*this, "chkCaseSensitive", wxCheckBox)->GetValue();
    cfg->Write(_T("/eval_tooltip"), (bool)XRCCTRL(*this, "chkEvalTooltip", wxCheckBox)->GetValue());
    m_Parser.Options().useSmartSense = !XRCCTRL(*this, "chkSimpleMode", wxCheckBox)->GetValue();
    m_Parser.Options().whileTyping = XRCCTRL(*this, "chkTypeMode", wxCheckBox)->GetValue();

    cfg->Write(_T("/use_symbols_browser"), (bool)!XRCCTRL(*this, "chkNoSB", wxCheckBox)->GetValue());
    cfg->Write(_T("/fillup_chars"), XRCCTRL(*this, "txtFillupChars", wxTextCtrl)->GetValue());
    m_Parser.ClassBrowserOptions().showInheritance = XRCCTRL(*this, "chkInheritance", wxCheckBox)->GetValue();
    m_Parser.ClassBrowserOptions().expandNS = XRCCTRL(*this, "chkExpandNS", wxCheckBox)->GetValue();
    m_Parser.ClassBrowserOptions().treeMembers = XRCCTRL(*this, "chkTreeMembers", wxCheckBox)->GetValue();
    cfg->Write(_T("/as_floating_window"), (bool)XRCCTRL(*this, "chkFloatCB", wxCheckBox)->GetValue());

    cfg->Write(_T("/lexer_keywords_set1"), (bool)XRCCTRL(*this, "chkKL_1", wxCheckBox)->GetValue());
    cfg->Write(_T("/lexer_keywords_set2"), (bool)XRCCTRL(*this, "chkKL_2", wxCheckBox)->GetValue());
    cfg->Write(_T("/lexer_keywords_set3"), (bool)XRCCTRL(*this, "chkKL_3", wxCheckBox)->GetValue());
    cfg->Write(_T("/lexer_keywords_set4"), (bool)XRCCTRL(*this, "chkKL_4", wxCheckBox)->GetValue());
    cfg->Write(_T("/lexer_keywords_set5"), (bool)XRCCTRL(*this, "chkKL_5", wxCheckBox)->GetValue());
    cfg->Write(_T("/lexer_keywords_set6"), (bool)XRCCTRL(*this, "chkKL_6", wxCheckBox)->GetValue());
    cfg->Write(_T("/lexer_keywords_set7"), (bool)XRCCTRL(*this, "chkKL_7", wxCheckBox)->GetValue());
    cfg->Write(_T("/lexer_keywords_set8"), (bool)XRCCTRL(*this, "chkKL_8", wxCheckBox)->GetValue());
    cfg->Write(_T("/lexer_keywords_set9"), (bool)XRCCTRL(*this, "chkKL_9", wxCheckBox)->GetValue());

    m_Parser.WriteOptions();
    m_pNativeParsers->RereadParserOptions();
    m_pCodeCompletion->RereadOptions();
}
