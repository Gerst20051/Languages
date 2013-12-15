/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision: 6104 $
 * $Id: editpathdlg.cpp 6104 2010-01-23 12:56:12Z mortenmacfly $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/sdk/editpathdlg.cpp $
 */

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include <wx/intl.h>
    #include <wx/xrc/xmlres.h>
    #include <wx/textctrl.h>
    #include <wx/button.h>
    #include <wx/filename.h>
    #include <wx/stattext.h>
    #include "globals.h"
    #include "manager.h"
    #include "macrosmanager.h"
#endif

#include "editpathdlg.h"
#include <wx/filedlg.h>
#include <wx/msgdlg.h>

// remember last path, when supplied path is empty
static wxString s_LastPath;

BEGIN_EVENT_TABLE(EditPathDlg, wxScrollingDialog)
    EVT_UPDATE_UI(-1, EditPathDlg::OnUpdateUI)
    EVT_BUTTON(XRCID("btnBrowse"), EditPathDlg::OnBrowse)
END_EVENT_TABLE()

EditPathDlg::EditPathDlg(wxWindow* parent,
        const wxString& path,
        const wxString& basepath,
        const wxString& title,
        const wxString& message,
        const bool wantDir,
        const bool allowMultiSel,
        const wxString& filter)
{
	//ctor
	wxXmlResource::Get()->LoadObject(this, parent, _T("dlgEditPath"),_T("wxScrollingDialog"));

	XRCCTRL(*this, "txtPath", wxTextCtrl)->SetValue(path);
	XRCCTRL(*this, "dlgEditPath", wxScrollingDialog)->SetTitle(title);

	if (!wantDir) {
        XRCCTRL(*this, "lblText", wxStaticText)->SetLabel(_("File:"));
	}

	m_Path = path;
	m_WantDir = wantDir;
	m_AllowMultiSel = allowMultiSel;
	m_Message = message;
	m_Basepath = basepath;
	m_Filter = filter;
	m_AskMakeRelative = true;
	m_ShowCreateDirButton = false;
}

EditPathDlg::~EditPathDlg()
{
	//dtor
}

void EditPathDlg::OnBrowse(wxCommandEvent& event)
{
    wxFileName path;
    wxArrayString multi;

    wxString val = XRCCTRL(*this, "txtPath", wxTextCtrl)->GetValue();
    int idx = val.Find(DEFAULT_ARRAY_SEP);
    if (idx != -1)
        val.Remove(idx);
    wxFileName fname(val);

    if (m_WantDir)
    {
        // try to "decode" custom var
        wxString bkp = val;
        Manager::Get()->GetMacrosManager()->ReplaceEnvVars(val);
        fname = val;
        fname.MakeAbsolute(m_Basepath);
        m_Path = fname.GetFullPath();

        path = ChooseDirectory(this, m_Message, (m_Path.IsEmpty() ? s_LastPath : m_Path),
                m_Basepath, false, m_ShowCreateDirButton);

        if (path.GetFullPath().IsEmpty())
            return;

        // if it was a custom var, see if we can re-insert it
        if (bkp != val)
        {
            wxString tmp = path.GetFullPath();
            if (tmp.Replace(val, bkp) != 0)
            {
                // done here
                XRCCTRL(*this, "txtPath", wxTextCtrl)->SetValue(tmp);
                return;
            }
        }
    }
    else
    {
        wxFileDialog dlg(this, m_Message, (fname.GetPath().IsEmpty() ? s_LastPath : fname.GetPath()),
                fname.GetFullName(), m_Filter, wxFD_CHANGE_DIR | (m_AllowMultiSel ? wxFD_MULTIPLE : 0) );

        PlaceWindow(&dlg);
        if (dlg.ShowModal() == wxID_OK)
        {
            if (m_AllowMultiSel)
                dlg.GetPaths(multi);
            else
                path = dlg.GetPath();
        }
        else
            return;
    }

    if (m_AllowMultiSel && multi.GetCount() != 0 && !multi[0].IsEmpty())
        s_LastPath = multi[0];
    else if (!path.GetFullPath().IsEmpty())
        s_LastPath = path.GetFullPath();

    wxString result;
    if (m_AskMakeRelative && !m_Basepath.IsEmpty())
    {
        // ask the user if he wants it to be kept as relative
        if (cbMessageBox(_("Keep this as a relative path?"),
                        _("Question"),
                        wxICON_QUESTION | wxYES_NO) == wxID_YES)
        {
            if (m_AllowMultiSel)
            {
                for (unsigned int i = 0; i < multi.GetCount(); ++i)
                {
                    path = multi[i];
                    path.MakeRelativeTo(m_Basepath);
                    multi[i] = path.GetFullPath();
                }
                result = GetStringFromArray(multi);
            }
            else
            {
                path.MakeRelativeTo(m_Basepath);
                result = path.GetFullPath();
            }
        }
        else
        {
            if (m_AllowMultiSel)
                result = GetStringFromArray(multi);
            else
                result = path.GetFullPath();
        }
    }
    else // always absolute path
    {
        if (m_AllowMultiSel)
            result = GetStringFromArray(multi);
        else
            result = path.GetFullPath();
    }

    // finally set the path
    XRCCTRL(*this, "txtPath", wxTextCtrl)->SetValue(result);
}

void EditPathDlg::OnUpdateUI(wxUpdateUIEvent& event)
{
    wxButton* btn = (wxButton*)FindWindow(wxID_OK);
    if (btn)
        btn->Enable(!XRCCTRL(*this, "txtPath", wxTextCtrl)->GetValue().IsEmpty());
}

void EditPathDlg::EndModal(int retCode)
{
    // update m_Lib
    m_Path = XRCCTRL(*this, "txtPath", wxTextCtrl)->GetValue();
    wxScrollingDialog::EndModal(retCode);
}
