/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision: 5887 $
 * $Id: workspaceloader.cpp 5887 2009-10-26 12:19:37Z mortenmacfly $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/sdk/workspaceloader.cpp $
 */

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include <wx/confbase.h>
    #include <wx/fileconf.h>
    #include <wx/intl.h>
    #include <wx/string.h>

    #include "manager.h"
    #include "projectmanager.h"
    #include "logmanager.h"
    #include "cbproject.h"
    #include "globals.h"
    #include "workspaceloader.h"
#endif



#include "tinyxml/tinyxml.h"
#include "tinyxml/tinywxuni.h"

WorkspaceLoader::WorkspaceLoader()
{
	//ctor
}

WorkspaceLoader::~WorkspaceLoader()
{
	//dtor
}

inline ProjectManager* GetpMan() { return Manager::Get()->GetProjectManager(); }
inline LogManager* GetpMsg() { return Manager::Get()->GetLogManager(); }

bool WorkspaceLoader::Open(const wxString& filename, wxString& Title)
{
    TiXmlDocument doc;
    if (!TinyXML::LoadDocument(filename, &doc))
        return false;

//    ProjectManager* pMan = Manager::Get()->GetProjectManager();
//    LogManager* pMsg = Manager::Get()->GetLogManager();

    if (!GetpMan() || !GetpMsg())
        return false;

    // BUG: Race condition. to be fixed by Rick.
    // If I click close AFTER pMan and pMsg are calculated,
    // I get a segfault.
    // I modified classes projectmanager and logmanager,
    // so that when self==NULL, they do nothing
    // (constructors, destructors and static functions excempted from this)
    // This way, we'll use the *manager::Get() functions to check for nulls.

    TiXmlElement* root = doc.FirstChildElement("CodeBlocks_workspace_file");
    if (!root)
    {
        // old tag
        root = doc.FirstChildElement("Code::Blocks_workspace_file");
        if (!root)
        {
            GetpMsg()->DebugLog(_T("Not a valid Code::Blocks workspace file..."));
            return false;
        }
    }
    TiXmlElement* wksp = root->FirstChildElement("Workspace");
    if (!wksp)
    {
        GetpMsg()->DebugLog(_T("No 'Workspace' element in file..."));
        return false;
    }

    Title = cbC2U(wksp->Attribute("title")); // Conversion to unicode is automatic (see wxString::operator= )

    TiXmlElement* proj = wksp->FirstChildElement("Project");
    if (!proj)
    {
        GetpMsg()->DebugLog(_T("Workspace file contains no projects..."));
        return false;
    }

    // first loop to load projects
    while (proj)
    {
        if(Manager::isappShuttingDown() || !GetpMan() || !GetpMsg())
            return false;
        wxString projectFilename = UnixFilename(cbC2U(proj->Attribute("filename")));
        if (projectFilename.IsEmpty())
        {
            GetpMsg()->DebugLog(_T("'Project' node exists, but no filename?!?"));
        }
        else
        {
            wxFileName fname(projectFilename);
            wxFileName wfname(filename);
            fname.MakeAbsolute(wfname.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR));
            int active = 0;
            int ret = proj->QueryIntAttribute("active", &active);
            switch (ret)
            {
                case TIXML_SUCCESS:
                    if (active == 1)
					{
						cbProject* pProject = GetpMan()->LoadProject(fname.GetFullPath(), true); // activate it
						if(!pProject)
						{
							cbMessageBox(_("Unable to open ") + projectFilename,
							 _("Opening WorkSpace") + filename, wxICON_WARNING);
						}
					}
                    break;
                case TIXML_WRONG_TYPE:
                    GetpMsg()->DebugLog(F(_T("Error %s: %s"), doc.Value(), doc.ErrorDesc()));
                    GetpMsg()->DebugLog(_T("Wrong attribute type (expected 'int')"));
                    break;
                default:
					cbProject* pProject = GetpMan()->LoadProject(fname.GetFullPath(), false); // don't activate it
					if(!pProject)
					{
						cbMessageBox(_("Unable to open ") + projectFilename,
						 _("Opening WorkSpace") + filename, wxICON_WARNING);
					}
                    break;
            }
        }
        proj = proj->NextSiblingElement("Project");
    }

    // second loop to setup dependencies
    proj = wksp->FirstChildElement("Project");
    while (proj)
    {
        cbProject* thisprj = 0;
        wxString projectFilename = UnixFilename(cbC2U(proj->Attribute("filename")));
        if (projectFilename.IsEmpty())
        {
            GetpMsg()->DebugLog(_T("'Project' node exists, but no filename?!?"));
            thisprj = 0;
        }
        else
        {
            wxFileName fname(projectFilename);
            wxFileName wfname(filename);
            fname.MakeAbsolute(wfname.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR));
            thisprj = Manager::Get()->GetProjectManager()->IsOpen(fname.GetFullPath());
        }

        if (thisprj)
        {
            TiXmlElement* dep = proj->FirstChildElement("Depends");
            while (dep)
            {
                wxFileName fname(UnixFilename(cbC2U(dep->Attribute("filename"))));
                wxFileName wfname(filename);
                fname.MakeAbsolute(wfname.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR));
                cbProject* depprj = Manager::Get()->GetProjectManager()->IsOpen(fname.GetFullPath());
                if (depprj)
                    Manager::Get()->GetProjectManager()->AddProjectDependency(thisprj, depprj);
                dep = dep->NextSiblingElement("Depends");
            }
        }
        proj = proj->NextSiblingElement("Project");
    }

    return true;
}

static wxString ExportFilename(const wxFileName &filename)
{
    wxString fn = filename.GetFullPath();
    fn.Replace(_T("\\"), _T("/"), true);
    return fn;
}

bool WorkspaceLoader::Save(const wxString& title, const wxString& filename)
{
    const char* ROOT_TAG = "CodeBlocks_workspace_file";

    TiXmlDocument doc;
    doc.SetCondenseWhiteSpace(false);
    doc.InsertEndChild(TiXmlDeclaration("1.0", "UTF-8", "yes"));
    TiXmlElement* rootnode = static_cast<TiXmlElement*>(doc.InsertEndChild(TiXmlElement(ROOT_TAG)));
    if (!rootnode)
        return false;

    TiXmlElement* wksp = static_cast<TiXmlElement*>(rootnode->InsertEndChild(TiXmlElement("Workspace")));
    wksp->SetAttribute("title", cbU2C(title));

    ProjectsArray* arr = Manager::Get()->GetProjectManager()->GetProjects();
    for (unsigned int i = 0; i < arr->GetCount(); ++i)
    {
        cbProject* prj = arr->Item(i);

        wxFileName wfname(filename);
        wxFileName fname(prj->GetFilename());
        fname.MakeRelativeTo(wfname.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR));

        TiXmlElement* node = static_cast<TiXmlElement*>(wksp->InsertEndChild(TiXmlElement("Project")));
        node->SetAttribute("filename", cbU2C( ExportFilename(fname) ) );
        if (prj == Manager::Get()->GetProjectManager()->GetActiveProject())
            node->SetAttribute("active", 1);

        const ProjectsArray* deps = Manager::Get()->GetProjectManager()->GetDependenciesForProject(prj);
        if (deps && deps->GetCount())
        {
            for (size_t i = 0; i < deps->GetCount(); ++i)
            {
                prj = deps->Item(i);
                fname.Assign(prj->GetFilename());
                fname.MakeRelativeTo(wfname.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR));
                TiXmlElement* dnode = static_cast<TiXmlElement*>(node->InsertEndChild(TiXmlElement("Depends")));
                dnode->SetAttribute("filename", cbU2C( ExportFilename(fname) ) );
            }
        }
    }
    return cbSaveTinyXMLDocument(&doc, filename);
}
