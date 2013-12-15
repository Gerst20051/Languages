/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision: 6134 $
 * $Id: environmentsettingsdlg.cpp 6134 2010-02-03 17:43:56Z mortenmacfly $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/src/environmentsettingsdlg.cpp $
 */

#include <sdk.h>
#include <wx/aui/aui.h>
#include <wx/button.h>
#include <wx/xrc/xmlres.h>
#include <manager.h>
#include <configmanager.h>
#include <pluginmanager.h>
#include <wx/intl.h>
#include <wx/listbook.h>
#include <wx/listctrl.h>
#include <wx/combobox.h>
#include <wx/checkbox.h>
#include <wx/checklst.h>
#include <wx/radiobox.h>
#include <wx/spinctrl.h>
#include <wx/colordlg.h>
#include <wx/msgdlg.h>
#include <wx/imaglist.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include "appglobals.h"
#include "globals.h"
#include "associations.h"

#include "configurationpanel.h"
#include "environmentsettingsdlg.h"
#ifdef __WXMSW__
    #include "associations.h"
#endif

#ifndef CB_PRECOMP
    #include <wx/dir.h>
    #include "cbplugin.h" // cgCompiler, cgDebugger...
#endif

// images by order of pages
const wxString base_imgs[] =
{
    _T("general-prefs"),
    _T("view"),
    _T("notebook-appearance"),
    _T("colours"),
    _T("dialogs"),
    _T("net"),
};
const int IMAGES_COUNT = sizeof(base_imgs) / sizeof(wxString);

BEGIN_EVENT_TABLE(EnvironmentSettingsDlg, wxScrollingDialog)
    EVT_BUTTON(XRCID("btnSetAssocs"), EnvironmentSettingsDlg::OnSetAssocs)
    EVT_BUTTON(XRCID("btnManageAssocs"), EnvironmentSettingsDlg::OnManageAssocs)
    EVT_BUTTON(XRCID("btnAuiCaptionColour"), EnvironmentSettingsDlg::OnChooseColour)
    EVT_BUTTON(XRCID("btnAuiCaptionTextColour"), EnvironmentSettingsDlg::OnChooseColour)
    EVT_BUTTON(XRCID("btnAuiActiveCaptionColour"), EnvironmentSettingsDlg::OnChooseColour)
    EVT_BUTTON(XRCID("btnAuiActiveCaptionGradientColour"), EnvironmentSettingsDlg::OnChooseColour)
    EVT_BUTTON(XRCID("btnAuiActiveCaptionTextColour"), EnvironmentSettingsDlg::OnChooseColour)
    EVT_BUTTON(XRCID("btnAuiInactiveCaptionColour"), EnvironmentSettingsDlg::OnChooseColour)
    EVT_BUTTON(XRCID("btnAuiInactiveCaptionGradientColour"), EnvironmentSettingsDlg::OnChooseColour)
    EVT_BUTTON(XRCID("btnAuiInactiveCaptionTextColour"), EnvironmentSettingsDlg::OnChooseColour)
    EVT_BUTTON(XRCID("btnResetDefaultColours"), EnvironmentSettingsDlg::OnResetDefaultColours)
    EVT_CHECKBOX(XRCID("chkUseIPC"), EnvironmentSettingsDlg::OnUseIpcCheck)
    EVT_CHECKBOX(XRCID("chkDoPlace"), EnvironmentSettingsDlg::OnPlaceCheck)
    EVT_CHECKBOX(XRCID("chkPlaceHead"), EnvironmentSettingsDlg::OnHeadCheck)
    EVT_CHECKBOX(XRCID("chkAutoHideMessages"), EnvironmentSettingsDlg::OnAutoHide)
    EVT_CHECKBOX(XRCID("chkI18N"), EnvironmentSettingsDlg::OnI18NCheck)
    EVT_RADIOBOX(XRCID("rbSettingsIconsSize"), EnvironmentSettingsDlg::OnSettingsIconsSize)

    EVT_LISTBOOK_PAGE_CHANGING(XRCID("nbMain"), EnvironmentSettingsDlg::OnPageChanging)
    EVT_LISTBOOK_PAGE_CHANGED(XRCID("nbMain"), EnvironmentSettingsDlg::OnPageChanged)
END_EVENT_TABLE()

EnvironmentSettingsDlg::EnvironmentSettingsDlg(wxWindow* parent, wxAuiDockArt* art)
    : m_pArt(art)
{
    ConfigManager *cfg = Manager::Get()->GetConfigManager(_T("app"));
    ConfigManager *pcfg = Manager::Get()->GetConfigManager(_T("project_manager"));
    ConfigManager *mcfg = Manager::Get()->GetConfigManager(_T("message_manager"));
    ConfigManager *acfg = Manager::Get()->GetConfigManager(_T("an_dlg"));

    wxXmlResource::Get()->LoadObject(this, parent, _T("dlgEnvironmentSettings"),_T("wxScrollingDialog"));
    int sel = cfg->ReadInt(_T("/environment/settings_size"), 0);
    wxListbook* lb = XRCCTRL(*this, "nbMain", wxListbook);
    SetSettingsIconsStyle(lb->GetListView(), (SettingsIconsStyle)sel);
    LoadListbookImages();

    // this setting is not available under wxGTK
    #ifndef __WXMSW__
    XRCCTRL(*this, "rbSettingsIconsSize", wxRadioBox)->Enable(false);
    #endif

    // tab "General"
    XRCCTRL(*this, "chkShowSplash", wxCheckBox)->SetValue(cfg->ReadBool(_T("/environment/show_splash"), true));
    XRCCTRL(*this, "chkSingleInstance", wxCheckBox)->SetValue(cfg->ReadBool(_T("/environment/single_instance"), true));
#ifdef __WXMSW__
    static_cast<wxStaticBoxSizer*>(XRCCTRL(*this, "chkUseIPC", wxCheckBox)->GetContainingSizer())->GetStaticBox()->SetLabel(_("Dynamic Data Exchange (will take place after restart)"));
#endif
    bool useIpc = cfg->ReadBool(_T("/environment/use_ipc"), true);
    XRCCTRL(*this, "chkUseIPC",      wxCheckBox)->SetValue(useIpc);
    XRCCTRL(*this, "chkRaiseViaIPC", wxCheckBox)->SetValue(cfg->ReadBool(_T("/environment/raise_via_ipc"), true));
    XRCCTRL(*this, "chkRaiseViaIPC", wxCheckBox)->Enable(useIpc);

    XRCCTRL(*this, "chkAssociations",   wxCheckBox)->SetValue(cfg->ReadBool(_T("/environment/check_associations"), true));
    XRCCTRL(*this, "chkModifiedFiles",  wxCheckBox)->SetValue(cfg->ReadBool(_T("/environment/check_modified_files"), true));
    XRCCTRL(*this, "chkInvalidTargets", wxCheckBox)->SetValue(cfg->ReadBool(_T("/environment/ignore_invalid_targets"), true));
    XRCCTRL(*this, "rbAppStart", wxRadioBox)->SetSelection(cfg->ReadBool(_T("/environment/blank_workspace"), true) ? 1 : 0);
    wxTextCtrl* txt = XRCCTRL(*this, "txtConsoleTerm", wxTextCtrl);
    txt->SetValue(cfg->Read(_T("/console_terminal"), DEFAULT_CONSOLE_TERM));
#ifdef __WXMSW__
    // under win32, this option is not needed, so disable it
    txt->Enable(false);
#endif
    txt = XRCCTRL(*this, "txtConsoleShell", wxTextCtrl);
    txt->SetValue(cfg->Read(_T("/console_shell"), DEFAULT_CONSOLE_SHELL));
#ifdef __WXMSW__
    // under win32, this option is not needed, so disable it
    txt->Enable(false);
#endif

    // tab "View"
    bool do_place = cfg->ReadBool(_T("/dialog_placement/do_place"), false);
    XRCCTRL(*this, "chkDoPlace", wxCheckBox)->SetValue(do_place);
    XRCCTRL(*this, "chkPlaceHead", wxCheckBox)->SetValue(cfg->ReadInt(_T("/dialog_placement/dialog_position"), 0) == pdlHead ? 1 : 0);
    XRCCTRL(*this, "chkPlaceHead", wxCheckBox)->Enable(do_place);

    XRCCTRL(*this, "rbProjectOpen", wxRadioBox)->SetSelection(pcfg->ReadInt(_T("/open_files"), 1));
    XRCCTRL(*this, "rbToolbarSize", wxRadioBox)->SetSelection(cfg->ReadBool(_T("/environment/toolbar_size"), true) ? 1 : 0);
    XRCCTRL(*this, "rbSettingsIconsSize", wxRadioBox)->SetSelection(cfg->ReadInt(_T("/environment/settings_size"), 0));
    XRCCTRL(*this, "chkShowStartPage", wxCheckBox)->SetValue(cfg->ReadBool(_T("/environment/start_here_page"), true));
    XRCCTRL(*this, "spnLogFontSize", wxSpinCtrl)->SetValue(mcfg->ReadInt(_T("/log_font_size"), 8));

    bool en = mcfg->ReadBool(_T("/auto_hide"), false);
    XRCCTRL(*this, "chkAutoHideMessages", wxCheckBox)->SetValue(en);
    XRCCTRL(*this, "chkAutoShowMessagesOnSearch", wxCheckBox)->SetValue(mcfg->ReadBool(_T("/auto_show_search"), true));
    XRCCTRL(*this, "chkAutoShowMessagesOnWarn", wxCheckBox)->SetValue(mcfg->ReadBool(_T("/auto_show_build_warnings"), true));
    XRCCTRL(*this, "chkAutoShowMessagesOnErr", wxCheckBox)->SetValue(mcfg->ReadBool(_T("/auto_show_build_errors"), true));
    XRCCTRL(*this, "chkAutoShowMessagesOnSearch", wxCheckBox)->Enable(en);
    XRCCTRL(*this, "chkAutoShowMessagesOnWarn", wxCheckBox)->Enable(en);
    XRCCTRL(*this, "chkAutoShowMessagesOnErr", wxCheckBox)->Enable(en);

    bool i18n=cfg->ReadBool(_T("/locale/enable"), false);
        XRCCTRL(*this, "chkI18N", wxCheckBox)->SetValue(i18n);

    wxDir locDir(ConfigManager::GetDataFolder() + _T("/locale"));
    wxString locFName;

    if(locDir.IsOpened() && locDir.GetFirst(&locFName/*, wxEmptyString, wxDIR_DIRS*/))
    do
    {
        const wxLanguageInfo *info = wxLocale::FindLanguageInfo(locFName);
        if(info)
        {
            XRCCTRL(*this, "cbxLanguage", wxComboBox)->Append(info->Description);
        }
    }while(locDir.GetNext(&locFName));

    XRCCTRL(*this, "cbxLanguage", wxComboBox)->Enable(i18n);

    const wxLanguageInfo *info = wxLocale::FindLanguageInfo(cfg->Read(_T("/locale/language")));
    if(info)
        XRCCTRL(*this, "cbxLanguage", wxComboBox)->SetStringSelection(info->Description);


    // tab "Notebook"
    XRCCTRL(*this, "cmbEditorTabs",               wxComboBox)->SetSelection(cfg->ReadInt(_T("/environment/tabs_style"), 0));
    XRCCTRL(*this, "chkCloseOnAll",               wxCheckBox)->SetValue(cfg->ReadBool(_T("/environment/tabs_close_on_all"), 0));
    XRCCTRL(*this, "chkListTabs",                 wxCheckBox)->SetValue(cfg->ReadBool(_T("/environment/tabs_list"), 0));

    // tab "Docking"
    XRCCTRL(*this, "spnAuiBorder",                        wxSpinCtrl)->SetValue(cfg->ReadInt(_T("/environment/aui/border_size"), m_pArt->GetMetric(wxAUI_DOCKART_PANE_BORDER_SIZE)));
    XRCCTRL(*this, "spnAuiSash",                          wxSpinCtrl)->SetValue(cfg->ReadInt(_T("/environment/aui/sash_size"), m_pArt->GetMetric(wxAUI_DOCKART_SASH_SIZE)));
    XRCCTRL(*this, "spnAuiCaption",                       wxSpinCtrl)->SetValue(cfg->ReadInt(_T("/environment/aui/caption_size"), m_pArt->GetMetric(wxAUI_DOCKART_CAPTION_SIZE)));
    XRCCTRL(*this, "btnAuiActiveCaptionColour",           wxButton)->SetBackgroundColour(cfg->ReadColour(_T("/environment/aui/active_caption_colour"), m_pArt->GetColour(wxAUI_DOCKART_ACTIVE_CAPTION_COLOUR)));
    XRCCTRL(*this, "btnAuiActiveCaptionGradientColour",   wxButton)->SetBackgroundColour(cfg->ReadColour(_T("/environment/aui/active_caption_gradient_colour"), m_pArt->GetColour(wxAUI_DOCKART_ACTIVE_CAPTION_GRADIENT_COLOUR)));
    XRCCTRL(*this, "btnAuiActiveCaptionTextColour",       wxButton)->SetBackgroundColour(cfg->ReadColour(_T("/environment/aui/active_caption_text_colour"), m_pArt->GetColour(wxAUI_DOCKART_ACTIVE_CAPTION_TEXT_COLOUR)));
    XRCCTRL(*this, "btnAuiInactiveCaptionColour",         wxButton)->SetBackgroundColour(cfg->ReadColour(_T("/environment/aui/inactive_caption_colour"), m_pArt->GetColour(wxAUI_DOCKART_INACTIVE_CAPTION_COLOUR)));
    XRCCTRL(*this, "btnAuiInactiveCaptionGradientColour", wxButton)->SetBackgroundColour(cfg->ReadColour(_T("/environment/aui/inactive_caption_gradient_colour"), m_pArt->GetColour(wxAUI_DOCKART_INACTIVE_CAPTION_GRADIENT_COLOUR)));
    XRCCTRL(*this, "btnAuiInactiveCaptionTextColour",     wxButton)->SetBackgroundColour(cfg->ReadColour(_T("/environment/aui/inactive_caption_text_colour"), m_pArt->GetColour(wxAUI_DOCKART_INACTIVE_CAPTION_TEXT_COLOUR)));

    // tab "Dialogs"
    wxCheckListBox* clb = XRCCTRL(*this, "chkDialogs", wxCheckListBox);
    clb->Clear();

    ConfigManagerContainer::StringSet dialogs = acfg->ReadSSet(_T("/disabled"));
    for (ConfigManagerContainer::StringSet::iterator i = dialogs.begin(); i != dialogs.end(); ++i)
            clb->Append(*i);

    // tab "Network"
    XRCCTRL(*this, "txtProxy", wxTextCtrl)->SetValue(cfg->Read(_T("/network_proxy")));

    // disable some windows-only settings, in other platforms
#ifndef __WXMSW__
    XRCCTRL(*this, "chkAssociations", wxCheckBox)->Enable(false);
    XRCCTRL(*this, "btnSetAssocs", wxButton)->Enable(false);
    XRCCTRL(*this, "btnManageAssocs", wxButton)->Enable(false);
#endif

    // add all plugins configuration panels
    AddPluginPanels();

    // make sure everything is laid out properly
    GetSizer()->SetSizeHints(this);
    CentreOnParent();
}

EnvironmentSettingsDlg::~EnvironmentSettingsDlg()
{
    //dtor
}

void EnvironmentSettingsDlg::AddPluginPanels()
{
    const wxString base = _T("images/settings/");
    const wxString noimg = _T("images/settings/generic-plugin");

    wxListbook* lb = XRCCTRL(*this, "nbMain", wxListbook);
    // get all configuration panels which are *not* about compiler, debugger and editor.
    Manager::Get()->GetPluginManager()->GetConfigurationPanels(~(cgCompiler | cgDebugger | cgEditor), lb, m_PluginPanels);

    for (size_t i = 0; i < m_PluginPanels.GetCount(); ++i)
    {
        cbConfigurationPanel* panel = m_PluginPanels[i];
        lb->AddPage(panel, panel->GetTitle());

        wxString onFile = ConfigManager::LocateDataFile(base + panel->GetBitmapBaseName() + _T(".png"), sdDataGlobal | sdDataUser);
        if (onFile.IsEmpty())
            onFile = ConfigManager::LocateDataFile(noimg + _T(".png"), sdDataGlobal | sdDataUser);
        wxString offFile = ConfigManager::LocateDataFile(base + panel->GetBitmapBaseName() + _T("-off.png"), sdDataGlobal | sdDataUser);
        if (offFile.IsEmpty())
            offFile = ConfigManager::LocateDataFile(noimg + _T("-off.png"), sdDataGlobal | sdDataUser);

        lb->GetImageList()->Add(cbLoadBitmap(onFile));
        lb->GetImageList()->Add(cbLoadBitmap(offFile));
        lb->SetPageImage(lb->GetPageCount() - 1, lb->GetImageList()->GetImageCount() - 2);
    }

    UpdateListbookImages();
}

void EnvironmentSettingsDlg::LoadListbookImages()
{
    const wxString base = ConfigManager::GetDataFolder() + _T("/images/settings/");

    wxImageList* images = new wxImageList(80, 80);
    wxBitmap bmp;
    for (int i = 0; i < IMAGES_COUNT; ++i)
    {
        bmp = cbLoadBitmap(base + base_imgs[i] + _T(".png"));
        images->Add(bmp);
        bmp = cbLoadBitmap(base + base_imgs[i] + _T("-off.png"));
        images->Add(bmp);
    }
    wxListbook* lb = XRCCTRL(*this, "nbMain", wxListbook);
    lb->AssignImageList(images);

    UpdateListbookImages();
}

void EnvironmentSettingsDlg::UpdateListbookImages()
{
    wxListbook* lb = XRCCTRL(*this, "nbMain", wxListbook);
    int sel = lb->GetSelection();
    // set page images according to their on/off status
    for (size_t i = 0; i < IMAGES_COUNT + m_PluginPanels.GetCount(); ++i)
    {
        lb->SetPageImage(i, (i * 2) + (sel == (int)i ? 0 : 1));
    }

    // the selection colour is ruining the on/off effect,
    // so make sure no item is selected ;)
    // (only if we have icons showing)
    if (GetSettingsIconsStyle(lb->GetListView()) != sisNoIcons)
        lb->GetListView()->Select(sel, false);

    // update the page title
    wxString label = lb->GetPageText(sel);
    // replace any stray & with && because label makes it an underscore
    while (label.Replace(_T(" & "), _T(" && ")))
        ;
    XRCCTRL(*this, "lblBigTitle", wxStaticText)->SetLabel(label);
}

void EnvironmentSettingsDlg::OnPageChanging(wxListbookEvent& event)
{
}

void EnvironmentSettingsDlg::OnPageChanged(wxListbookEvent& event)
{
    // update only on real change, not on dialog creation
    if (event.GetOldSelection() != -1 && event.GetSelection() != -1)
    {
        UpdateListbookImages();
    }
}

void EnvironmentSettingsDlg::OnSetAssocs(wxCommandEvent& event)
{
#ifdef __WXMSW__
    Associations::SetCore();
    //cbMessageBox(_("Code::Blocks associated with C/C++ files."), _("Information"), wxICON_INFORMATION);
#endif
}

void EnvironmentSettingsDlg::OnManageAssocs(wxCommandEvent& event)
{
#ifdef __WXMSW__
    ManageAssocsDialog dlg(this);
    PlaceWindow(&dlg);
    dlg.ShowModal();
#endif
}

void EnvironmentSettingsDlg::OnChooseColour(wxCommandEvent& event)
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

void EnvironmentSettingsDlg::OnResetDefaultColours(wxCommandEvent& event)
{
    wxAuiDockArt* art = new wxAuiDefaultDockArt;

    XRCCTRL(*this, "spnAuiBorder", wxSpinCtrl)->SetValue(art->GetMetric(wxAUI_DOCKART_PANE_BORDER_SIZE));
    XRCCTRL(*this, "spnAuiSash", wxSpinCtrl)->SetValue(art->GetMetric(wxAUI_DOCKART_SASH_SIZE));
    XRCCTRL(*this, "spnAuiCaption", wxSpinCtrl)->SetValue(art->GetMetric(wxAUI_DOCKART_CAPTION_SIZE));
    XRCCTRL(*this, "btnAuiActiveCaptionColour", wxButton)->SetBackgroundColour(art->GetColour(wxAUI_DOCKART_ACTIVE_CAPTION_COLOUR));
    XRCCTRL(*this, "btnAuiActiveCaptionGradientColour", wxButton)->SetBackgroundColour(art->GetColour(wxAUI_DOCKART_ACTIVE_CAPTION_GRADIENT_COLOUR));
    XRCCTRL(*this, "btnAuiActiveCaptionTextColour", wxButton)->SetBackgroundColour(art->GetColour(wxAUI_DOCKART_ACTIVE_CAPTION_TEXT_COLOUR));
    XRCCTRL(*this, "btnAuiInactiveCaptionColour", wxButton)->SetBackgroundColour(art->GetColour(wxAUI_DOCKART_INACTIVE_CAPTION_COLOUR));
    XRCCTRL(*this, "btnAuiInactiveCaptionGradientColour", wxButton)->SetBackgroundColour(art->GetColour(wxAUI_DOCKART_INACTIVE_CAPTION_GRADIENT_COLOUR));
    XRCCTRL(*this, "btnAuiInactiveCaptionTextColour", wxButton)->SetBackgroundColour(art->GetColour(wxAUI_DOCKART_INACTIVE_CAPTION_TEXT_COLOUR));

    delete art;
}

void EnvironmentSettingsDlg::OnAutoHide(wxCommandEvent& event)
{
    bool en = XRCCTRL(*this, "chkAutoHideMessages", wxCheckBox)->GetValue();
    XRCCTRL(*this, "chkAutoShowMessagesOnSearch",   wxCheckBox)->Enable(en);
    XRCCTRL(*this, "chkAutoShowMessagesOnWarn",     wxCheckBox)->Enable(en);
    XRCCTRL(*this, "chkAutoShowMessagesOnErr",      wxCheckBox)->Enable(en);
}

void EnvironmentSettingsDlg::OnUseIpcCheck(wxCommandEvent& event)
{
    XRCCTRL(*this, "chkRaiseViaIPC", wxCheckBox)->Enable(event.IsChecked());
}

void EnvironmentSettingsDlg::OnPlaceCheck(wxCommandEvent& event)
{
    XRCCTRL(*this, "chkPlaceHead", wxCheckBox)->Enable(event.IsChecked());
}

void EnvironmentSettingsDlg::OnHeadCheck(wxCommandEvent& event)
{
    PlaceWindow(this, event.IsChecked() ? pdlHead : pdlCentre, true);
}

void EnvironmentSettingsDlg::OnI18NCheck(wxCommandEvent& event)
{
    XRCCTRL(*this, "cbxLanguage", wxComboBox)->Enable(event.IsChecked());
}

void EnvironmentSettingsDlg::OnSettingsIconsSize(wxCommandEvent& event)
{
    wxListbook* lb = XRCCTRL(*this, "nbMain", wxListbook);
    SetSettingsIconsStyle(lb->GetListView(), (SettingsIconsStyle)event.GetSelection());
}

void EnvironmentSettingsDlg::EndModal(int retCode)
{
    if (retCode == wxID_OK)
    {
        ConfigManager *cfg = Manager::Get()->GetConfigManager(_T("app"));
        ConfigManager *pcfg = Manager::Get()->GetConfigManager(_T("project_manager"));
        ConfigManager *mcfg = Manager::Get()->GetConfigManager(_T("message_manager"));
        ConfigManager *acfg = Manager::Get()->GetConfigManager(_T("an_dlg"));

        // tab "General"
        cfg->Write(_T("/environment/show_splash"),           (bool) XRCCTRL(*this, "chkShowSplash", wxCheckBox)->GetValue());
        cfg->Write(_T("/environment/single_instance"),       (bool) XRCCTRL(*this, "chkSingleInstance", wxCheckBox)->GetValue());
        cfg->Write(_T("/environment/use_ipc"),               (bool) XRCCTRL(*this, "chkUseIPC", wxCheckBox)->GetValue());
        cfg->Write(_T("/environment/raise_via_ipc"),         (bool) XRCCTRL(*this, "chkRaiseViaIPC", wxCheckBox)->GetValue());
        cfg->Write(_T("/environment/check_associations"),    (bool) XRCCTRL(*this, "chkAssociations", wxCheckBox)->GetValue());
        cfg->Write(_T("/environment/check_modified_files"),  (bool) XRCCTRL(*this, "chkModifiedFiles", wxCheckBox)->GetValue());
        cfg->Write(_T("/environment/ignore_invalid_targets"),(bool) XRCCTRL(*this, "chkInvalidTargets", wxCheckBox)->GetValue());
        cfg->Write(_T("/console_shell"),                            XRCCTRL(*this, "txtConsoleShell", wxTextCtrl)->GetValue());
        cfg->Write(_T("/console_terminal"),                         XRCCTRL(*this, "txtConsoleTerm", wxTextCtrl)->GetValue());

        // tab "View"
        cfg->Write(_T("/environment/blank_workspace"),       (bool) XRCCTRL(*this, "rbAppStart", wxRadioBox)->GetSelection() ? true : false);
        pcfg->Write(_T("/open_files"),                       (int)  XRCCTRL(*this, "rbProjectOpen", wxRadioBox)->GetSelection());
        cfg->Write(_T("/environment/toolbar_size"),          (bool) XRCCTRL(*this, "rbToolbarSize", wxRadioBox)->GetSelection() == 1);
        cfg->Write(_T("/environment/settings_size"),         (int)  XRCCTRL(*this, "rbSettingsIconsSize", wxRadioBox)->GetSelection());
        mcfg->Write(_T("/auto_hide"),                        (bool) XRCCTRL(*this, "chkAutoHideMessages", wxCheckBox)->GetValue());
        mcfg->Write(_T("/auto_show_search"),                 (bool) XRCCTRL(*this, "chkAutoShowMessagesOnSearch", wxCheckBox)->GetValue());
        mcfg->Write(_T("/auto_show_build_warnings"),         (bool) XRCCTRL(*this, "chkAutoShowMessagesOnWarn", wxCheckBox)->GetValue());
        mcfg->Write(_T("/auto_show_build_errors"),           (bool) XRCCTRL(*this, "chkAutoShowMessagesOnErr", wxCheckBox)->GetValue());
        cfg->Write(_T("/environment/start_here_page"),       (bool) XRCCTRL(*this, "chkShowStartPage", wxCheckBox)->GetValue());

        cfg->Write(_T("/locale/enable"),                     (bool) XRCCTRL(*this, "chkI18N", wxCheckBox)->GetValue());
        const wxLanguageInfo *info = wxLocale::FindLanguageInfo(XRCCTRL(*this, "cbxLanguage", wxComboBox)->GetStringSelection());
        if(info)
            cfg->Write(_T("/locale/language"), info->CanonicalName);
        else
            cfg->Write(_T("/locale/language"), wxEmptyString);

        mcfg->Write(_T("/log_font_size"),                    (int)  XRCCTRL(*this, "spnLogFontSize", wxSpinCtrl)->GetValue());
        cfg->Write(_T("/dialog_placement/do_place"),         (bool) XRCCTRL(*this, "chkDoPlace",     wxCheckBox)->GetValue());
        cfg->Write(_T("/dialog_placement/dialog_position"),  (int)  XRCCTRL(*this, "chkPlaceHead",   wxCheckBox)->GetValue() ? pdlHead : pdlCentre);

        // tab "Appearence"
        cfg->Write(_T("/environment/tabs_style"),            (int)  XRCCTRL(*this, "cmbEditorTabs",               wxComboBox)->GetSelection());
        cfg->Write(_T("/environment/tabs_close_on_all"),     (bool) XRCCTRL(*this, "chkCloseOnAll",               wxCheckBox)->GetValue());
        cfg->Write(_T("/environment/tabs_list"),             (bool) XRCCTRL(*this, "chkListTabs",                 wxCheckBox)->GetValue());
        cfg->Write(_T("/environment/aui/border_size"),                  (int)XRCCTRL(*this, "spnAuiBorder", wxSpinCtrl)->GetValue());
        cfg->Write(_T("/environment/aui/sash_size"),                    (int)XRCCTRL(*this, "spnAuiSash", wxSpinCtrl)->GetValue());
        cfg->Write(_T("/environment/aui/caption_size"),                 (int)XRCCTRL(*this, "spnAuiCaption", wxSpinCtrl)->GetValue());
        cfg->Write(_T("/environment/aui/active_caption_colour"),             XRCCTRL(*this, "btnAuiActiveCaptionColour", wxButton)->GetBackgroundColour());
        cfg->Write(_T("/environment/aui/active_caption_gradient_colour"),    XRCCTRL(*this, "btnAuiActiveCaptionGradientColour", wxButton)->GetBackgroundColour());
        cfg->Write(_T("/environment/aui/active_caption_text_colour"),        XRCCTRL(*this, "btnAuiActiveCaptionTextColour", wxButton)->GetBackgroundColour());
        cfg->Write(_T("/environment/aui/inactive_caption_colour"),           XRCCTRL(*this, "btnAuiInactiveCaptionColour", wxButton)->GetBackgroundColour());
        cfg->Write(_T("/environment/aui/inactive_caption_gradient_colour"),  XRCCTRL(*this, "btnAuiInactiveCaptionGradientColour", wxButton)->GetBackgroundColour());
        cfg->Write(_T("/environment/aui/inactive_caption_text_colour"),      XRCCTRL(*this, "btnAuiInactiveCaptionTextColour", wxButton)->GetBackgroundColour());

        m_pArt->SetMetric(wxAUI_DOCKART_PANE_BORDER_SIZE,                XRCCTRL(*this, "spnAuiBorder", wxSpinCtrl)->GetValue());
        m_pArt->SetMetric(wxAUI_DOCKART_SASH_SIZE,                       XRCCTRL(*this, "spnAuiSash", wxSpinCtrl)->GetValue());
        m_pArt->SetMetric(wxAUI_DOCKART_CAPTION_SIZE,                    XRCCTRL(*this, "spnAuiCaption", wxSpinCtrl)->GetValue());
        m_pArt->SetColour(wxAUI_DOCKART_ACTIVE_CAPTION_COLOUR,           XRCCTRL(*this, "btnAuiActiveCaptionColour", wxButton)->GetBackgroundColour());
        m_pArt->SetColour(wxAUI_DOCKART_ACTIVE_CAPTION_GRADIENT_COLOUR,  XRCCTRL(*this, "btnAuiActiveCaptionGradientColour", wxButton)->GetBackgroundColour());
        m_pArt->SetColour(wxAUI_DOCKART_ACTIVE_CAPTION_TEXT_COLOUR,      XRCCTRL(*this, "btnAuiActiveCaptionTextColour", wxButton)->GetBackgroundColour());
        m_pArt->SetColour(wxAUI_DOCKART_INACTIVE_CAPTION_COLOUR,         XRCCTRL(*this, "btnAuiInactiveCaptionColour", wxButton)->GetBackgroundColour());
        m_pArt->SetColour(wxAUI_DOCKART_INACTIVE_CAPTION_GRADIENT_COLOUR,XRCCTRL(*this, "btnAuiInactiveCaptionGradientColour", wxButton)->GetBackgroundColour());
        m_pArt->SetColour(wxAUI_DOCKART_INACTIVE_CAPTION_TEXT_COLOUR,    XRCCTRL(*this, "btnAuiInactiveCaptionTextColour", wxButton)->GetBackgroundColour());

        // tab "Dialogs"
        wxCheckListBox* lb = XRCCTRL(*this, "chkDialogs", wxCheckListBox);

        ConfigManagerContainer::StringSet dialogs = acfg->ReadSSet(_T("/disabled"));

        for (int i = 0; i < (int)lb->GetCount(); ++i)
        {
            if (lb->IsChecked(i))
                dialogs.erase(lb->GetString(i));
        }

        acfg->Write(_T("/disabled"), dialogs);

        // tab "Network"
        cfg->Write(_T("/network_proxy"),    XRCCTRL(*this, "txtProxy", wxTextCtrl)->GetValue());

        // finally, apply settings in all plugins' panels
        for (size_t i = 0; i < m_PluginPanels.GetCount(); ++i)
        {
            cbConfigurationPanel* panel = m_PluginPanels[i];
            panel->OnApply();
        }
    }
    else
    {
        // finally, cancel settings in all plugins' panels
        for (size_t i = 0; i < m_PluginPanels.GetCount(); ++i)
        {
            cbConfigurationPanel* panel = m_PluginPanels[i];
            panel->OnCancel();
        }
    }

    wxScrollingDialog::EndModal(retCode);
}
