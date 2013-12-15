/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision: 5929 $
 * $Id: compilerMINGWgenerator.cpp 5929 2009-11-21 14:34:53Z biplab $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/compilergcc/compilerMINGWgenerator.cpp $
 */

#include <sdk.h>
#include "compilerMINGWgenerator.h"
#include <wx/intl.h>
#include "cbexception.h"
#include "cbproject.h"
#include "compilerfactory.h"
#include "compiler.h"
#include "manager.h"
#include "configmanager.h"
#include "logmanager.h"
#include "macrosmanager.h"
#include "scriptingmanager.h"

CompilerMINGWGenerator::CompilerMINGWGenerator()
{
    //ctor
    m_VerStr = wxEmptyString;
}

CompilerMINGWGenerator::~CompilerMINGWGenerator()
{
    //dtor
}

wxString CompilerMINGWGenerator::SetupIncludeDirs(Compiler* compiler, ProjectBuildTarget* target)
{
    wxString result = CompilerCommandGenerator::SetupIncludeDirs(compiler, target);
    m_VerStr = compiler->GetVersionString();
    wxString pch_prepend = wxEmptyString;
    bool IsGcc4 = m_VerStr.Left(1).IsSameAs(_T("4"));
    bool HasPCH = false; // We don't know yet if there are any header files to be compiled...

    // for PCH to work, the very first include dir *must* be the object output dir
    // *only* if PCH is generated in the object output dir
    if (target &&
        target->GetParentProject()->GetModeForPCH() == pchObjectDir)
    {
        wxArrayString includedDirs; // avoid adding duplicate dirs...
        wxString sep = wxFILE_SEP_PATH;
        // find all PCH in project
        int count = target->GetParentProject()->GetFilesCount();
        for (int i = 0; i < count; ++i)
        {
            ProjectFile* f = target->GetParentProject()->GetFile(i);
            if (FileTypeOf(f->relativeFilename) == ftHeader &&
                f->compile)
            {
                // it is a PCH; add it's object dir to includes
                wxString dir = wxFileName(target->GetObjectOutput() + sep + f->GetObjName()).GetPath();
                if (includedDirs.Index(dir) == wxNOT_FOUND)
                {
                    includedDirs.Add(dir);
                    QuoteStringIfNeeded(dir);
                    if (!IsGcc4)
                        pch_prepend << compiler->GetSwitches().includeDirs << dir << _T(' ');
                    else
                        pch_prepend << _T("-iquote") << dir << _T(' ');
                }
                HasPCH = true; // there is at least one header file to be compiled
            }
        }
        // for gcc-4.0+, use the following:
        // pch_prepend << _T("-iquote") << dir << _T(' ');
        // for earlier versions, -I- must be used
        if (!IsGcc4)
            pch_prepend << _T("-I- ");
        count = (int)includedDirs.GetCount();
        for (int i = 0; i < count; ++i)
        {
            QuoteStringIfNeeded(includedDirs[i]);
            pch_prepend << compiler->GetSwitches().includeDirs << includedDirs[i] << _T(' ');
        }
        pch_prepend << _T("-I. ");
    }

    // add in array
    if (HasPCH)
        result.Prepend(pch_prepend);

    return result;
}
