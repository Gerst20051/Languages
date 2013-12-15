/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision: 6104 $
 * $Id: associations.cpp 6104 2010-01-23 12:56:12Z mortenmacfly $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/src/associations.cpp $
 */

#include <sdk.h>
#ifndef CB_PRECOMP
    #include <wx/radiobox.h>
    #include <wx/xrc/xmlres.h>
#endif
#include "associations.h"
#include "appglobals.h"
#include <manager.h>
#include <configmanager.h>
#include <filefilters.h>
#include <wx/checklst.h>

const Associations::Assoc knownTypes[] =
{
/*
    { Extension (wxString), Description (wxString), IconIndex (int) }
      Note: "index" is the index of the icon resource in "resources.rc"
            Keep all indices in sync with icon indices in "resources.rc"!
*/
    { FileFilters::CODEBLOCKS_EXT,      _T("project file"),                  1 },
    { FileFilters::WORKSPACE_EXT,       _T("workspace file"),               11 },

    { FileFilters::C_EXT,               _T("C source file"),                 3 },

    { FileFilters::CC_EXT,              _T("C++ source file"),               4 },
    { FileFilters::CPP_EXT,             _T("C++ source file"),               4 },
    { FileFilters::CXX_EXT,             _T("C++ source file"),               4 },
    { FileFilters::INL_EXT,             _T("C++ source file"),               4 },

    { FileFilters::H_EXT,               _T("Header file"),                   5 },
    { FileFilters::HH_EXT,              _T("Header file"),                   5 },
    { FileFilters::HPP_EXT,             _T("Header file"),                   5 },
    { FileFilters::HXX_EXT,             _T("Header file"),                   5 },

    { FileFilters::JAVA_EXT,            _T("Java source file"),              6 },
    { _T("cg"),                         _T("cg source file"),                7 },
    { FileFilters::D_EXT,               _T("D source file"),                 8 },
    { FileFilters::RESOURCE_EXT,        _T("resource file"),                10 },
    { FileFilters::XRCRESOURCE_EXT,     _T("XRC resource file"),            10 },

    { FileFilters::ASM_EXT,             _T("ASM source file"),               2 },
    { FileFilters::S_EXT,               _T("ASM source file"),               2 },
    { FileFilters::SS_EXT,              _T("ASM source file"),               2 },
    { FileFilters::S62_EXT,             _T("ASM source file"),               2 },

    { FileFilters::F_EXT,               _T("Fortran source file"),           9 },
    { FileFilters::F77_EXT,             _T("Fortran source file"),           9 },
    { FileFilters::F90_EXT,             _T("Fortran source file"),           9 },
    { FileFilters::F95_EXT,             _T("Fortran source file"),           9 },

    { FileFilters::DEVCPP_EXT,          _T("Dev-CPP project file"),         21 },
    { FileFilters::MSVC6_EXT,           _T("MS Visual C++ project file"),   22 },
    { FileFilters::MSVC6_WORKSPACE_EXT, _T("MS Visual C++ workspace file"), 23 }
    //{ _T("proj"),                       _T("XCODE Project file"),           24 }
};

inline void DoSetAssociation(const wxString& executable, int index)
{
    Associations::DoSetAssociation(knownTypes[index].ext, knownTypes[index].descr, executable, knownTypes[index].index);
};

inline bool DoCheckAssociation(const wxString& executable, int index)
{
    return Associations::DoCheckAssociation(knownTypes[index].ext, knownTypes[index].descr, executable, knownTypes[index].index);
};

unsigned int Associations::CountAssocs()
{
    return sizeof(knownTypes)/sizeof(Associations::Assoc);
}

void Associations::SetBatchBuildOnly()
{
    wxChar name[MAX_PATH] = {0};
    GetModuleFileName(0L, name, MAX_PATH);

    ::DoSetAssociation(name, 0);
    ::DoSetAssociation(name, 1);

    UpdateChanges();
}

void Associations::UpdateChanges()
{
    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0L, 0L);
}

void Associations::SetCore()
{
    wxChar name[MAX_PATH] = {0};
    GetModuleFileName(0L, name, MAX_PATH);

    for(int i = 0; i <= 12; ++i)        // beware, the number 12 is hardcoded ;)
        ::DoSetAssociation(name, i);

    UpdateChanges();
}

void Associations::SetAll()
{
    wxChar name[MAX_PATH] = {0};
    GetModuleFileName(0L, name, MAX_PATH);

    for(unsigned int i = 0; i < CountAssocs(); ++i)
        ::DoSetAssociation(name, i);

    UpdateChanges();
}

void Associations::ClearAll()
{
    wxChar name[MAX_PATH] = {0};
    GetModuleFileName(0L, name, MAX_PATH);

    for(unsigned int i = 0; i < CountAssocs(); ++i)
    {
        DoClearAssociation(knownTypes[i].ext);
    };

    UpdateChanges();
}

bool Associations::Check()
{
    wxChar name[MAX_PATH] = {0};
    GetModuleFileName(0L, name, MAX_PATH);

    bool result = true;

    for(int i = 0; i <= 12; ++i)        // beware, the number 12 is hardcoded ;)
        result &= ::DoCheckAssociation(name, i);

    return  result;
}

void Associations::DoSetAssociation(const wxString& ext, const wxString& descr, const wxString& exe, int icoNum)
{
    wxString BaseKeyName(_T("HKEY_CURRENT_USER\\Software\\Classes\\"));
    if(platform::WindowsVersion() == platform::winver_Windows9598ME)
        BaseKeyName = _T("HKEY_CLASSES_ROOT\\");

    wxString node(_T("CodeBlocks.") + ext);

    wxRegKey key; // defaults to HKCR
    key.SetName(BaseKeyName + _T(".") + ext);
    key.Create();
    key = _T("CodeBlocks.") + ext;

    key.SetName(BaseKeyName + node);
    key.Create();
    key = descr;

    key.SetName(BaseKeyName + node + _T("\\DefaultIcon"));
    key.Create();
    key = exe + wxString::Format(_T(",%d"), icoNum);

    key.SetName(BaseKeyName + node + _T("\\shell\\open\\command"));
    key.Create();
    key = _T("\"") + exe + _T("\" \"%1\"");

    key.SetName(BaseKeyName + node + _T("\\shell\\open\\ddeexec"));
    key.Create();
    key = _T("[Open(\"%1\")]");

    key.SetName(BaseKeyName + node + _T("\\shell\\open\\ddeexec\\application"));
    key.Create();
    key = DDE_SERVICE;

    key.SetName(BaseKeyName + node + _T("\\shell\\open\\ddeexec\\ifexec"));
    key.Create();
    key = _T("[IfExec_Open(\"%1\")]");;

    key.SetName(BaseKeyName + node + _T("\\shell\\open\\ddeexec\\topic"));
    key.Create();
    key = DDE_TOPIC;

    if(ext.IsSameAs(FileFilters::CODEBLOCKS_EXT) || ext.IsSameAs(FileFilters::WORKSPACE_EXT))
    {
        wxString batchbuildargs = Manager::Get()->GetConfigManager(_T("app"))->Read(_T("/batch_build_args"), appglobals::DefaultBatchBuildArgs);
        key.SetName(BaseKeyName + node + _T("\\shell\\Build\\command"));
        key.Create();
        key = _T("\"") + exe + _T("\" ") + batchbuildargs + _T(" --build \"%1\"");

        key.SetName(BaseKeyName + node + _T("\\shell\\Rebuild (clean)\\command"));
        key.Create();
        key = _T("\"") + exe + _T("\" ") + batchbuildargs + _T(" --rebuild \"%1\"");
    }
}

void Associations::DoClearAssociation(const wxString& ext)
{
    wxString BaseKeyName(_T("HKEY_CURRENT_USER\\Software\\Classes\\"));
    if(platform::WindowsVersion() == platform::winver_Windows9598ME)
        BaseKeyName = _T("HKEY_CLASSES_ROOT\\");

    wxRegKey key; // defaults to HKCR
    key.SetName(BaseKeyName + _T(".") + ext);
    if(key.Exists())
    {
        wxString s;
        #if wxCHECK_VERSION(2, 9, 0)
        if(key.QueryValue(wxEmptyString, s) && s.StartsWith(_T("CodeBlocks")))
        #else
        if(key.QueryValue(NULL, s) && s.StartsWith(_T("CodeBlocks")))
        #endif
            key.DeleteSelf();
    }

    key.SetName(BaseKeyName + _T("CodeBlocks.") + ext);
    if(key.Exists())
        key.DeleteSelf();
}

bool Associations::DoCheckAssociation(const wxString& ext, const wxString& descr, const wxString& exe, int icoNum)
{
    wxString BaseKeyName(_T("HKEY_CURRENT_USER\\Software\\Classes\\"));

    if(platform::WindowsVersion() == platform::winver_Windows9598ME)
        BaseKeyName = _T("HKEY_CLASSES_ROOT\\");

    wxRegKey key; // defaults to HKCR
    key.SetName(BaseKeyName + _T(".") + ext);
    if (!key.Exists())
        return false;

    key.SetName(BaseKeyName + _T("CodeBlocks.") + ext);
    if (!key.Exists())
        return false;

    key.SetName(BaseKeyName + _T("CodeBlocks.") + ext + _T("\\DefaultIcon"));
    if (!key.Exists())
        return false;
    wxString strVal;
    if (!key.QueryValue(wxEmptyString, strVal))
        return false;
    if (strVal != wxString::Format(_T("%s,%d"), exe.c_str(), icoNum))
        return false;

    key.SetName(BaseKeyName + _T("CodeBlocks.") + ext + _T("\\shell\\open\\command"));
    if (!key.Open())
        return false;
    if (!key.QueryValue(wxEmptyString, strVal))
        return false;
    if (strVal != wxString::Format(_T("\"%s\" \"%%1\""), exe.c_str()))
        return false;

    key.SetName(BaseKeyName + _T("CodeBlocks.") + ext + _T("\\shell\\open\\ddeexec"));
    if (!key.Open())
        return false;
    if (!key.QueryValue(wxEmptyString, strVal))
        return false;
    if (strVal != _T("[Open(\"%1\")]"))
        return false;

    key.SetName(BaseKeyName + _T("CodeBlocks.") + ext + _T("\\shell\\open\\ddeexec\\application"));
    if (!key.Open())
        return false;
    if (!key.QueryValue(wxEmptyString, strVal))
        return false;
    if (strVal != DDE_SERVICE)
        return false;

    key.SetName(BaseKeyName + _T("CodeBlocks.") + ext + _T("\\shell\\open\\ddeexec\\ifexec"));
    if (!key.Open())
        return false;
    if (!key.QueryValue(wxEmptyString, strVal))
        return false;
    if (strVal != _T("[IfExec_Open(\"%1\")]"))
        return false;

    key.SetName(BaseKeyName + _T("CodeBlocks.") + ext + _T("\\shell\\open\\ddeexec\\topic"));
    if (!key.Open())
        return false;
    if (!key.QueryValue(wxEmptyString, strVal))
        return false;
    if (strVal != DDE_TOPIC)
        return false;

    if(ext.IsSameAs(FileFilters::CODEBLOCKS_EXT) || ext.IsSameAs(FileFilters::WORKSPACE_EXT))
    {
        wxString batchbuildargs = Manager::Get()->GetConfigManager(_T("app"))->Read(_T("/batch_build_args"), appglobals::DefaultBatchBuildArgs);
        key.SetName(BaseKeyName + _T("CodeBlocks.") + ext + _T("\\shell\\Build\\command"));
        if (!key.Open())
            return false;
        if (!key.QueryValue(wxEmptyString, strVal))
            return false;
        if (strVal != _T("\"") + exe + _T("\" ") + batchbuildargs + _T(" --build \"%1\""))
            return false;

        key.SetName(BaseKeyName + _T("CodeBlocks.") + ext + _T("\\shell\\Rebuild (clean)\\command"));
        if (!key.Open())
            return false;
        if (!key.QueryValue(wxEmptyString, strVal))
            return false;
        if (strVal != _T("\"") + exe + _T("\" ") + batchbuildargs + _T(" --rebuild \"%1\""))
            return false;
    }

    return true;
}



BEGIN_EVENT_TABLE(ManageAssocsDialog, wxScrollingDialog)
    EVT_BUTTON(XRCID("wxID_OK"), ManageAssocsDialog::OnApply)
    EVT_BUTTON(XRCID("wxID_CANCEL"), ManageAssocsDialog::OnCancel)
    EVT_BUTTON(XRCID("clearAll"), ManageAssocsDialog::OnClearAll)
END_EVENT_TABLE()



ManageAssocsDialog::ManageAssocsDialog(wxWindow* parent)
{
    wxXmlResource::Get()->LoadObject(this, parent, _T("dlgManageAssocs"),_T("wxScrollingDialog"));

    list = XRCCTRL(*this, "checkList", wxCheckListBox);
    assert(list);

    wxString d(_T("."));

    wxChar exe[MAX_PATH] = {0};
    GetModuleFileName(0L, exe, MAX_PATH);

    for(unsigned int i = 0; i < Associations::CountAssocs(); ++i)
    {
        list->Append(d + knownTypes[i].ext + _T("  (") + knownTypes[i].descr + _T(")"));
        list->Check(i, Associations::DoCheckAssociation(knownTypes[i].ext, knownTypes[i].descr, exe, knownTypes[i].index));
    }

    CentreOnParent();
}

void ManageAssocsDialog::OnApply(wxCommandEvent& event)
{
    wxChar name[MAX_PATH] = {0};
    GetModuleFileName(0L, name, MAX_PATH);

    for(int i = 0; i < (int)list->GetCount(); ++i)
    {
        if(list->IsChecked(i))
            ::DoSetAssociation(name, i);
        else
            Associations::DoClearAssociation(knownTypes[i].ext);
    }

    Associations::UpdateChanges();
    EndModal(0);
}

void ManageAssocsDialog::OnCancel(wxCommandEvent& event)
{
    EndModal(0);
}

void ManageAssocsDialog::OnClearAll(wxCommandEvent& event)
{
    Associations::ClearAll();
    Associations::UpdateChanges();
    EndModal(0);
}



BEGIN_EVENT_TABLE(AskAssocDialog, wxScrollingDialog)
    EVT_BUTTON(XRCID("wxID_OK"), AskAssocDialog::OnOK)
END_EVENT_TABLE()



AskAssocDialog::AskAssocDialog(wxWindow* parent)
{
    wxXmlResource::Get()->LoadObject(this, parent, _T("askAssoc"),_T("wxScrollingDialog"));
}

void AskAssocDialog::OnOK(wxCommandEvent& event)
{
    EndModal(XRCCTRL(*this, "choice", wxRadioBox)->GetSelection());
}
