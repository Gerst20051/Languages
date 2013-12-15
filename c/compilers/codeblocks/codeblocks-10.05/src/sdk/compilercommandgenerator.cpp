/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision: 5895 $
 * $Id: compilercommandgenerator.cpp 5895 2009-11-03 12:11:29Z jenslody $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/sdk/compilercommandgenerator.cpp $
 */

#include "sdk_precomp.h"
#include "compilercommandgenerator.h"
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
#include "filefilters.h"

#include "scripting/bindings/sc_base_types.h"
#include "scripting/sqplus/sqplus.h"

// move this to globals if needed
wxString UnquoteStringIfNeeded(const wxString& str)
{
    wxString s = str;
    if (!str.IsEmpty() && str.GetChar(0) == _T('"') && str.Last() == _T('"'))
        s = str.Mid(1, str.Length() - 2);
    return s;
}

CompilerCommandGenerator::CompilerCommandGenerator()
{
    //ctor
}

CompilerCommandGenerator::~CompilerCommandGenerator()
{
    //dtor
}

void CompilerCommandGenerator::Init(cbProject* project)
{
    // clear old arrays
    m_Output.clear();
    m_StaticOutput.clear();
    m_DefOutput.clear();
    m_Inc.clear();
    m_Lib.clear();
    m_RC.clear();
    m_CFlags.clear();
    m_LDFlags.clear();
    m_RCFlags.clear();

    // don't clear the backticks cache - it wouldn't be a cache then :)
//    m_Backticks.clear();

    m_CompilerSearchDirs.clear();
    m_LinkerSearchDirs.clear();

    // access the default compiler
    Compiler* compiler = CompilerFactory::GetDefaultCompiler();
    if (!compiler)
        cbThrow(_T("Default compiler is invalid!"));

    if (!project)
    {
        m_DefOutput[0] = SetupOutputFilenames(compiler, 0);
        m_Inc[0] = SetupIncludeDirs(compiler, 0);
        m_Lib[0] = SetupLibrariesDirs(compiler, 0);
        m_RC[0] = SetupResourceIncludeDirs(compiler, 0);
        m_CFlags[0] = SetupCompilerOptions(compiler, 0);
        m_LDFlags[0] = SetupLinkerOptions(compiler, 0);
        m_LDAdd[0] = SetupLinkLibraries(compiler, 0);
        m_RCFlags[0] = SetupResourceCompilerOptions(compiler, 0);
        return;
    }
    else
    {
        m_PrjIncPath = project->GetCommonTopLevelPath();
        if (!m_PrjIncPath.IsEmpty())
        {
            if (m_PrjIncPath.Last() == _T('\\'))
                m_PrjIncPath.RemoveLast();
            QuoteStringIfNeeded(m_PrjIncPath);
            m_PrjIncPath.Prepend(compiler->GetSwitches().includeDirs);
        }
    }

    // reset failed-scripts arrays
    m_NotLoadedScripts.Clear();
    m_ScriptsWithErrors.Clear();

    // change to the project's base dir so scripts can be found
    // (they 're always stored relative to the base dir)
    wxSetWorkingDirectory(project->GetBasePath());

    // backup project settings
    bool projectWasModified = project->GetModified();
    CompileTargetBase backup = *(CompileTargetBase*)project;

    // Invoke plugins
    CodeBlocksEvent evt(cbEVT_COMPILER_SET_BUILD_OPTIONS, 0, project);
    Manager::Get()->ProcessEvent(evt);

    // project build scripts
    DoBuildScripts(project, project, _T("SetBuildOptions"));

    // for each target
    for (int i = 0; i < project->GetBuildTargetsCount(); ++i)
    {
        ProjectBuildTarget* target = project->GetBuildTarget(i);

        // access the compiler used for this target
        compiler = CompilerFactory::GetCompiler(target->GetCompilerID());

        // for commands-only targets (or if invalid compiler), nothing to setup
        // just add stub entries so that indices keep in sync
        if (!compiler || target->GetTargetType() == ttCommandsOnly || !target->SupportsCurrentPlatform())
        {
            m_Output[target] = wxEmptyString;
            m_StaticOutput[target] = wxEmptyString;
            m_DefOutput[target] = wxEmptyString;
            m_Inc[target] = wxEmptyString;
            m_Lib[target] = wxEmptyString;
            m_RC[target] = wxEmptyString;
            m_CFlags[target] = wxEmptyString;
            m_LDFlags[target] = wxEmptyString;
            m_LDAdd[target] = wxEmptyString;
            m_RCFlags[target] = wxEmptyString;
            // continue with next target
            continue;
        }

        // backup target settings
        CompileTargetBase backuptarget = *(CompileTargetBase*)target;

        // invoke plugins
        CodeBlocksEvent evt(cbEVT_COMPILER_SET_BUILD_OPTIONS, 0, project);
        evt.SetBuildTargetName(target->GetTitle());
        Manager::Get()->ProcessEvent(evt);

        // target build scripts
        DoBuildScripts(project, target, _T("SetBuildOptions"));

        m_DefOutput[target] = SetupOutputFilenames(compiler, target);
        m_Inc[target] = SetupIncludeDirs(compiler, target);
        m_Lib[target] = SetupLibrariesDirs(compiler, target);
        m_RC[target] = SetupResourceIncludeDirs(compiler, target);
        m_CFlags[target] = SetupCompilerOptions(compiler, target);
        m_LDFlags[target] = SetupLinkerOptions(compiler, target);
        m_LDAdd[target] = SetupLinkLibraries(compiler, target);
        m_RCFlags[target] = SetupResourceCompilerOptions(compiler, target);

        // restore target settings
        *(CompileTargetBase*)target = backuptarget;
    }

    // restore project settings
    *(CompileTargetBase*)project = backup;
    project->SetModified(projectWasModified);

    // let's display all script errors now in a batch
    if (!m_NotLoadedScripts.IsEmpty() || !m_ScriptsWithErrors.IsEmpty())
    {
        wxString msg;

        if (!m_NotLoadedScripts.IsEmpty())
        {
            msg << _("Scripts that failed to load either because they don't exist\n"
                    "or because they contain syntax errors:\n\n");
            for (size_t i = 0; i < m_NotLoadedScripts.GetCount(); ++i)
            {
                msg << m_NotLoadedScripts[i] << _T("\n");
            }
            msg << _T("\n");
        }
        if (!m_ScriptsWithErrors.IsEmpty())
        {
            msg << _("Scripts that failed to load because the mandatory function\n"
                    "SetBuildOptions() is missing:\n\n");
            for (size_t i = 0; i < m_ScriptsWithErrors.GetCount(); ++i)
            {
                msg << m_ScriptsWithErrors[i] << _T("\n");
            }
            msg << _T("\n");
        }

        if (Manager::IsBatchBuild()) // no dialog if batch building...
            Manager::Get()->GetLogManager()->LogToStdOut(msg);
        else
            cbMessageBox(msg, _("Error"), wxICON_ERROR);
    }
}

void CompilerCommandGenerator::GenerateCommandLine(wxString& macro,
                                                    ProjectBuildTarget* target,
                                                    ProjectFile* pf,
                                                    const wxString& file,
                                                    const wxString& object,
                                                    const wxString& FlatObject,
                                                    const wxString& deps)
{
    if (target && !target->SupportsCurrentPlatform())
    {
        macro.Clear();
        return;
    }

    Compiler* compiler = target
                            ? CompilerFactory::GetCompiler(target->GetCompilerID())
                            : CompilerFactory::GetDefaultCompiler();
    wxString compilerStr;
    if (pf)
    {
        if (pf->compilerVar.Matches(_T("CPP")))
            compilerStr = compiler->GetPrograms().CPP;
        else if (pf->compilerVar.Matches(_T("CC")))
            compilerStr = compiler->GetPrograms().C;
        else if (pf->compilerVar.Matches(_T("WINDRES")))
            compilerStr = compiler->GetPrograms().WINDRES;
    }
    else
    {
        // filename might be quoted, so unquote it if needed or extension can be 'c"'
        wxFileName fname(UnquoteStringIfNeeded(file));
        if (fname.GetExt().Lower().Matches(_T("c")))
            compilerStr = compiler->GetPrograms().C;
        else
            compilerStr = compiler->GetPrograms().CPP;
    }

    // check that we have valid compiler/linker program names (and are indeed needed by the macro)
    if ((compilerStr.IsEmpty() && macro.Contains(_T("$compiler"))) ||
        (compiler->GetPrograms().LD.IsEmpty() && macro.Contains(_T("$linker"))) ||
        (compiler->GetPrograms().LIB.IsEmpty() && macro.Contains(_T("$lib_linker"))) ||
        (compiler->GetPrograms().WINDRES.IsEmpty() && macro.Contains(_T("$rescomp"))))
    {
        #if wxCHECK_VERSION(2, 9, 0)
        Manager::Get()->GetLogManager()->DebugLog(F(_T("GenerateCommandLine: no executable found! (file=%s)"), file.wx_str()));
        #else
        Manager::Get()->GetLogManager()->DebugLog(F(_T("GenerateCommandLine: no executable found! (file=%s)"), file.c_str()));
        #endif
        macro.Clear();
        return;
    }

    FixPathSeparators(compiler, compilerStr);

    wxString fileInc;
    if (Manager::Get()->GetConfigManager(_T("compiler"))->ReadBool(_T("/include_file_cwd"), false))
    {
        // Because C::B doesn't compile each file by running in the same directory with it,
        // it can cause some problems when the file #includes other files relative,
        // e.g. #include "../a_lib/include/a.h"
        //
        // So here we add the currently compiling file's directory to the includes
        // search dir so it works.
        wxFileName fileCwd = UnquoteStringIfNeeded(file);
        fileInc = fileCwd.GetPath();
        if (!fileInc.IsEmpty()) // only if non-empty! (remember r1813 errors)
        {
            QuoteStringIfNeeded(fileInc);
            fileInc.Prepend(compiler->GetSwitches().includeDirs);
        }
    }
    if (Manager::Get()->GetConfigManager(_T("compiler"))->ReadBool(_T("/include_prj_cwd"), false))
    {
        // Because C::B doesn't compile each file by running in the same directory with it,
        // it can cause some problems when the file #includes other files relative,
        // e.g. #include "../a_lib/include/a.h"
        //
        // So here we add the project's top-level directory (common toplevel path) to the includes
        // search dir so it works.
        fileInc << _T(' ') << m_PrjIncPath;
    }
    FixPathSeparators(compiler, fileInc);

    wxString tmp;
    wxString tmpFile = file;
    wxString tmpDeps = deps;
    wxString tmpObject = object;
    wxString tmpFlatObject = FlatObject;
    wxFileName tmpFname = tmpFile;
    wxFileName tmpOutFname;

    FixPathSeparators(compiler, tmpFile);
    FixPathSeparators(compiler, tmpDeps);
    FixPathSeparators(compiler, tmpObject);
    FixPathSeparators(compiler, tmpFlatObject);

    macro.Replace(_T("$compiler"), compilerStr);
    macro.Replace(_T("$linker"), compiler->GetPrograms().LD);
    macro.Replace(_T("$lib_linker"), compiler->GetPrograms().LIB);
    macro.Replace(_T("$rescomp"), compiler->GetPrograms().WINDRES);
    macro.Replace(_T("$options"), m_CFlags[target]);
    macro.Replace(_T("$link_options"), m_LDFlags[target]);
    macro.Replace(_T("$includes"), m_Inc[target] + fileInc);
    macro.Replace(_T("$res_includes"), m_RC[target] + fileInc);
    macro.Replace(_T("$libdirs"), m_Lib[target]);
    macro.Replace(_T("$libs"), m_LDAdd[target]);
    macro.Replace(_T("$file_basename"), tmpFname.GetName()); // old way - remove later
    macro.Replace(_T("$file_name"), tmpFname.GetName());
    macro.Replace(_T("$file_dir"), tmpFname.GetPath());
    macro.Replace(_T("$file_ext"), tmpFname.GetExt());
    macro.Replace(_T("$file"), tmpFile);
    macro.Replace(_T("$dep_object"), tmpDeps);
    if (target)
    {  // this one has to come before $object, otherwise $object would go first
       // leaving nothing to replace for this $objects_output_dir,
       // and after $options because $objects_output_dir may be in compiler flags ($options).
        tmp = target->GetObjectOutput();
        FixPathSeparators(compiler, tmp);
        macro.Replace(_T("$objects_output_dir"), tmp);
    }
    macro.Replace(_T("$object"), tmpObject);
    macro.Replace(_T("$resource_output"), tmpObject);
    if (!target)
    {
        // single file compilation, probably
        wxString object_unquoted(object);
        if (!object_unquoted.IsEmpty() && object_unquoted.GetChar(0) == '"')
            object_unquoted.Replace(_T("\""), _T(""));
        wxFileName fname(object_unquoted);
        fname.SetExt(FileFilters::EXECUTABLE_EXT);
        wxString output = fname.GetFullPath();
        QuoteStringIfNeeded(output);
        FixPathSeparators(compiler, output);
        macro.Replace(_T("$exe_output"), output);
        tmpOutFname.Assign(output);
    }
    else
    {
        macro.Replace(_T("$exe_output"), m_Output[target]);
        tmpOutFname.Assign(m_Output[target]);
    }
    macro.Replace(_T("$exe_name"), tmpOutFname.GetName());
    macro.Replace(_T("$exe_dir"), tmpOutFname.GetPath());
    macro.Replace(_T("$exe_ext"), tmpOutFname.GetExt());

    macro.Replace(_T("$link_resobjects"), tmpDeps);
    macro.Replace(_T("$link_objects"), tmpObject);
    macro.Replace(_T("$link_flat_objects"), tmpFlatObject);
    // the following were added to support the QUICK HACK in compiler plugin:
    // DirectCommands::GetTargetLinkCommands()
    macro.Replace(_T("$+link_objects"), tmpObject);
    macro.Replace(_T("$-link_objects"), tmpObject);
    macro.Replace(_T("$-+link_objects"), tmpObject);
    macro.Replace(_T("$+-link_objects"), tmpObject);

    if (target && (target->GetTargetType() == ttStaticLib || target->GetTargetType() == ttDynamicLib))
    {
        if (target->GetTargetType() == ttStaticLib || target->GetCreateStaticLib())
            macro.Replace(_T("$static_output"), m_StaticOutput[target]);
        else
        {
            macro.Replace(_T("-Wl,--out-implib=$static_output"), _T("")); // special gcc case
            macro.Replace(_T("$static_output"), _T(""));
        }

        if (target->GetCreateDefFile())
            macro.Replace(_T("$def_output"), m_DefOutput[target]);
        else
        {
            macro.Replace(_T("-Wl,--output-def=$def_output"), _T("")); // special gcc case
            macro.Replace(_T("$def_output"), _T(""));
        }
    }

    // finally, replace all macros in one go
    Manager::Get()->GetMacrosManager()->ReplaceMacros(macro, target);
}

/// Apply pre-build scripts for @c base.
void CompilerCommandGenerator::DoBuildScripts(cbProject* project, CompileTargetBase* target, const wxString& funcName)
{
    ProjectBuildTarget* bt = dynamic_cast<ProjectBuildTarget*>(target);
    static const wxString clearout_buildscripts = _T("SetBuildOptions <- null;");
    const wxArrayString& scripts = target->GetBuildScripts();
    for (size_t i = 0; i < scripts.GetCount(); ++i)
    {
        wxString script_nomacro = scripts[i];
        Manager::Get()->GetMacrosManager()->ReplaceMacros(script_nomacro, bt);
        script_nomacro = wxFileName(script_nomacro).IsAbsolute() ? script_nomacro : project->GetBasePath() + wxFILE_SEP_PATH + script_nomacro;

        // if the script has failed before, skip it
        if (m_NotLoadedScripts.Index(script_nomacro) != wxNOT_FOUND ||
            m_ScriptsWithErrors.Index(script_nomacro) != wxNOT_FOUND)
        {
            continue;
        }

        // clear previous script's context
        Manager::Get()->GetScriptingManager()->LoadBuffer(clearout_buildscripts);

        // if the script doesn't exist, just return
        if (!Manager::Get()->GetScriptingManager()->LoadScript(script_nomacro))
        {
            m_NotLoadedScripts.Add(script_nomacro);
            continue;
        }

        try
        {
            SqPlus::SquirrelFunction<void> f(cbU2C(funcName));
            f(target);
        }
        catch (SquirrelError& e)
        {
            Manager::Get()->GetScriptingManager()->DisplayErrors(&e);
            m_ScriptsWithErrors.Add(script_nomacro);
        }
    }
}

void CompilerCommandGenerator::FixPathSeparators(Compiler* compiler, wxString& inAndOut)
{
    // replace path separators with forward slashes if needed by this compiler
    if (!compiler || !compiler->GetSwitches().forceFwdSlashes)
        return;

    size_t i = 0;
    while (i < inAndOut.Length())
    {
        if (inAndOut.GetChar(i) == _T('\\') &&
            (i == inAndOut.Length() - 1 || inAndOut.GetChar(i + 1) != _T(' ')))
        {
            inAndOut.SetChar(i, _T('/'));
        }
        ++i;
    }
}

/// Setup output filename for build target.
wxString CompilerCommandGenerator::SetupOutputFilenames(Compiler* compiler, ProjectBuildTarget* target)
{
    if (!target)
        return wxEmptyString;

    // exe file
    wxString result = target->GetOutputFilename();
    QuoteStringIfNeeded(result);
    FixPathSeparators(compiler, result);
    m_Output[target] = result;

    // Replace Variables FIRST to address the $(VARIABLE)libfoo.a problem
    // if $(VARIABLE) expands to /bar/ then wxFileName will still consider $(VARIABLE)libfoo.a a filename,
    // not a fully qualified path, so we will prepend lib to /bar/libfoo.a incorrectly
    // NOTE (thomas#1#): A better solution might be to use a regex, but finding an universal regex might not be easy...
    wxString fnameString(target->GetOutputFilename());
    Manager::Get()->GetMacrosManager()->ReplaceMacros(fnameString, target);
    wxFileName fname(fnameString);

    TargetFilenameGenerationPolicy PrefixPolicy;
    TargetFilenameGenerationPolicy ExtensionPolicy;
    target->GetTargetFilenameGenerationPolicy(PrefixPolicy, ExtensionPolicy);
    if ((PrefixPolicy == tgfpPlatformDefault) || (target->GetTargetType() == ttDynamicLib))
    {
        if (!fname.GetName().StartsWith(compiler->GetSwitches().libPrefix))
        {
            fname.SetName(compiler->GetSwitches().libPrefix + fname.GetName());
        }
    }
    if ((ExtensionPolicy == tgfpPlatformDefault) || (target->GetTargetType() == ttDynamicLib))
    {
        wxString current_ext   = fname.GetExt();
        wxString requested_ext = compiler->GetSwitches().libExtension;
        if      (   (platform::windows && !current_ext.IsSameAs(requested_ext, false))
                 || (!current_ext.IsSameAs(requested_ext)) )
        {
            // Note: Do not use SetExt here to handle libs like e.g. System.Core correctly.
            // Otherwise SetExt would result in System.dll instead of System.Core.dll
            fname.SetFullName(fname.GetFullName()+wxFILE_SEP_EXT+requested_ext);
        }
    }
    result = UnixFilename(fname.GetFullPath());
    QuoteStringIfNeeded(result);
    FixPathSeparators(compiler, result);
    m_StaticOutput[target] = result;

    // def
    fname.SetExt(_T("def"));
    result = UnixFilename(fname.GetFullPath());
    QuoteStringIfNeeded(result); // NOTE (thomas#1#): Do we really need to call QuoteStringIfNeeded that often? ReplaceMacros already does it, and we do it twice again without ever possibly adding whitespace
    FixPathSeparators(compiler, result);

    return result;
}

/// Setup compiler include dirs for build target.
wxString CompilerCommandGenerator::SetupIncludeDirs(Compiler* compiler, ProjectBuildTarget* target)
{
    wxString result;

    if (target)
    {
        // currently, we ignore compiler search dirs (despite the var's name)
        // we only care about project/target search dirs
        wxArrayString prjSearchDirs = target->GetParentProject()->GetIncludeDirs();
        wxArrayString tgtSearchDirs = target->GetIncludeDirs();
        wxArrayString searchDirs;
        searchDirs = GetOrderedOptions(target, ortIncludeDirs, prjSearchDirs, tgtSearchDirs);
        // replace vars
        for (unsigned int x = 0; x < searchDirs.GetCount(); ++x)
        {
            Manager::Get()->GetMacrosManager()->ReplaceMacros(searchDirs[x], target);
        }
        // respect include dirs set by specific options (helps dependency tracking)
        bool incPrjDir = Manager::Get()->GetConfigManager(_T("compiler"))->ReadBool(_T("/include_prj_cwd"), false);
        if (incPrjDir)
            searchDirs.Add(target->GetParentProject()->GetBasePath());
        bool incFileDir = Manager::Get()->GetConfigManager(_T("compiler"))->ReadBool(_T("/include_file_cwd"), false);
        if (incFileDir)
            searchDirs.Add(_T("."));
        m_CompilerSearchDirs.insert(m_CompilerSearchDirs.end(), std::make_pair(target, searchDirs));

        // target dirs
        wxString tstr;
        const wxArrayString& arr = target->GetIncludeDirs();
        for (unsigned int x = 0; x < arr.GetCount(); ++x)
        {
            wxString tmp = arr[x];
            Manager::Get()->GetMacrosManager()->ReplaceMacros(tmp, target);
            QuoteStringIfNeeded(tmp);
            FixPathSeparators(compiler, tmp);
            tstr << compiler->GetSwitches().includeDirs << tmp << _T(' ');
        }

        // project dirs
        wxString pstr;
        const wxArrayString& parr = target->GetParentProject()->GetIncludeDirs();
        for (unsigned int x = 0; x < parr.GetCount(); ++x)
        {
            wxString tmp = parr[x];
            Manager::Get()->GetMacrosManager()->ReplaceMacros(tmp, target);
            QuoteStringIfNeeded(tmp);
            FixPathSeparators(compiler, tmp);
            pstr << compiler->GetSwitches().includeDirs << tmp << _T(' ');
        }

        // decide order
        result << GetOrderedOptions(target, ortIncludeDirs, pstr, tstr);
    }

    // compiler dirs
    const wxArrayString& carr = compiler->GetIncludeDirs();
    for (unsigned int x = 0; x < carr.GetCount(); ++x)
    {
        wxString tmp = carr[x];
        Manager::Get()->GetMacrosManager()->ReplaceMacros(tmp, target);
        QuoteStringIfNeeded(tmp);
        FixPathSeparators(compiler, tmp);
        result << compiler->GetSwitches().includeDirs << tmp << _T(' ');
    }

    // add in array
    return result;
}

/// Setup linker include dirs for build target.
wxString CompilerCommandGenerator::SetupLibrariesDirs(Compiler* compiler, ProjectBuildTarget* target)
{
    wxString result;

    if (target)
    {
        // currently, we ignore compiler search dirs (despite the var's name)
        // we only care about project/target search dirs
        wxArrayString prjSearchDirs = target->GetParentProject()->GetLibDirs();
        wxArrayString tgtSearchDirs = target->GetLibDirs();
        wxArrayString searchDirs;
        searchDirs = GetOrderedOptions(target, ortLibDirs, prjSearchDirs, tgtSearchDirs);
        // replace vars
        for (unsigned int x = 0; x < searchDirs.GetCount(); ++x)
        {
            Manager::Get()->GetMacrosManager()->ReplaceMacros(searchDirs[x], target);

            // also, normalize path (make absolute)
            wxFileName fn(searchDirs[x]);
            if (fn.IsRelative())
            {
                fn.MakeAbsolute(target->GetParentProject()->GetBasePath());
                searchDirs[x] = fn.GetFullPath();
            }
        }
        m_LinkerSearchDirs.insert(m_LinkerSearchDirs.end(), std::make_pair(target, searchDirs));

        // target dirs
        wxString tstr;
        const wxArrayString& arr = target->GetLibDirs();
        for (unsigned int x = 0; x < arr.GetCount(); ++x)
        {
            wxString tmp = arr[x];
            Manager::Get()->GetMacrosManager()->ReplaceMacros(tmp, target);
            QuoteStringIfNeeded(tmp);
            FixPathSeparators(compiler, tmp);
            tstr << compiler->GetSwitches().libDirs << tmp << _T(' ');
        }

        // project dirs
        wxString pstr;
        const wxArrayString& parr = target->GetParentProject()->GetLibDirs();
        for (unsigned int x = 0; x < parr.GetCount(); ++x)
        {
            wxString tmp = parr[x];
            Manager::Get()->GetMacrosManager()->ReplaceMacros(tmp, target);
            QuoteStringIfNeeded(tmp);
            FixPathSeparators(compiler, tmp);
            pstr << compiler->GetSwitches().libDirs << tmp << _T(' ');
        }

        // decide order
        result = GetOrderedOptions(target, ortLibDirs, pstr, tstr);
    }

    // compiler dirs
    const wxArrayString& carr = compiler->GetLibDirs();
    for (unsigned int x = 0; x < carr.GetCount(); ++x)
    {
        wxString cstr = carr[x];
        Manager::Get()->GetMacrosManager()->ReplaceMacros(cstr, target);
        QuoteStringIfNeeded(cstr);
        FixPathSeparators(compiler, cstr);
        result << compiler->GetSwitches().libDirs << cstr << _T(' ');
    }

    // add in array
    return result;
}

/// Setup resource compiler include dirs for build target.
wxString CompilerCommandGenerator::SetupResourceIncludeDirs(Compiler* compiler, ProjectBuildTarget* target)
{
    wxString result;

    if (target)
    {
        // target dirs
        wxString tstr;
        const wxArrayString& arr = target->GetResourceIncludeDirs();
        for (unsigned int x = 0; x < arr.GetCount(); ++x)
        {
            wxString tmp = arr[x];
            Manager::Get()->GetMacrosManager()->ReplaceMacros(tmp, target);
            QuoteStringIfNeeded(tmp);
            FixPathSeparators(compiler, tmp);
            tstr << compiler->GetSwitches().includeDirs << tmp << _T(' ');
        }

        // project dirs
        wxString pstr;
        const wxArrayString& parr = target->GetParentProject()->GetResourceIncludeDirs();
        for (unsigned int x = 0; x < parr.GetCount(); ++x)
        {
            wxString tmp = parr[x];
            Manager::Get()->GetMacrosManager()->ReplaceMacros(tmp, target);
            QuoteStringIfNeeded(tmp);
            FixPathSeparators(compiler, tmp);
            pstr << compiler->GetSwitches().includeDirs << tmp << _T(' ');
        }

        // decide order
        result = GetOrderedOptions(target, ortResDirs, pstr, tstr);
    }

    // compiler dirs
    const wxArrayString& carr = compiler->GetResourceIncludeDirs();
    for (unsigned int x = 0; x < carr.GetCount(); ++x)
    {
        wxString cstr = carr[x];
        Manager::Get()->GetMacrosManager()->ReplaceMacros(cstr, target);
        QuoteStringIfNeeded(cstr);
        FixPathSeparators(compiler, cstr);
        result << compiler->GetSwitches().includeDirs << cstr << _T(' ');
    }

    // add in array
    return result;
}

/// Setup compiler flags for build target.
wxString CompilerCommandGenerator::SetupCompilerOptions(Compiler* compiler, ProjectBuildTarget* target)
{
    wxString result;

    if (target)
    {
        // target options
        wxString tstr = GetStringFromArray(target->GetCompilerOptions(), _T(' ')) << _T(" ");

        // project options
        wxString pstr = GetStringFromArray(target->GetParentProject()->GetCompilerOptions(), _T(' ')) << _T(" ");

        // decide order
        result = GetOrderedOptions(target, ortCompilerOptions, pstr, tstr);
    }

    // compiler options
    result << GetStringFromArray(compiler->GetCompilerOptions(), _T(' ')) << _T(" ");

    wxString bt = ExpandBackticks(result);
    SearchDirsFromBackticks(compiler, target, bt);

    // add in array
    return result;
}

/// Setup linker flags for build target.
wxString CompilerCommandGenerator::SetupLinkerOptions(Compiler* compiler, ProjectBuildTarget* target)
{
    wxString result;

    if (target)
    {
        // target options
        wxString tstr = GetStringFromArray(target->GetLinkerOptions(), _T(' '));

        // project options
        wxString pstr = GetStringFromArray(target->GetParentProject()->GetLinkerOptions(), _T(' '));

        // decide order
        result = GetOrderedOptions(target, ortLinkerOptions, pstr, tstr);
    }

    // linker options
    result << GetStringFromArray(compiler->GetLinkerOptions(), _T(' '));

    wxString bt = ExpandBackticks(result);
    SearchDirsFromBackticks(compiler, target, bt);

    // add in array
    return result;
}

/// Fix library name based on advanced compiler settings
wxString CompilerCommandGenerator::FixupLinkLibraries(Compiler* compiler, const wxString& lib)
{
    if (lib.IsEmpty())
        return wxEmptyString;

    wxString result = lib;

    // construct linker option for each result, based on compiler's settings
    wxString libPrefix = compiler->GetSwitches().libPrefix;
    wxString libExt = compiler->GetSwitches().libExtension;
    QuoteStringIfNeeded(result);
    FixPathSeparators(compiler, result);
    // run replacements on libs only if no slashes in name (which means it's a relative or absolute path)
    if (result.Find('/') == -1 && result.Find('\\') == -1)
    {
        // 'result' prefix
        bool hadLibPrefix = false;
        if (!compiler->GetSwitches().linkerNeedsLibPrefix &&
            !libPrefix.IsEmpty() &&
            result.StartsWith(libPrefix))
        {
            result.Remove(0, libPrefix.Length());
            hadLibPrefix = true;
        }
        // extension
        if (!compiler->GetSwitches().linkerNeedsLibExtension &&
            result.Length() > libExt.Length() &&
            result.Right(libExt.Length() + 1) == _T(".") + libExt)
        {
            // remove the extension only if we had a result prefix
            if (hadLibPrefix)
                result.RemoveLast(libExt.Length() + 1);
        }
        else if (compiler->GetSwitches().linkerNeedsLibExtension &&
                !libExt.IsEmpty())
        {
            if (result.Length() <= libExt.Length() ||
                result.Right(libExt.Length() + 1) != _T(".") + libExt)
            {
                result << _T(".") << libExt;
            }
        }
        result = compiler->GetSwitches().linkLibs + result;
    }
    return result;
}

/// Setup link libraries for build target.
wxString CompilerCommandGenerator::SetupLinkLibraries(Compiler* compiler, ProjectBuildTarget* target)
{
    wxString result;

    if (target)
    {
        // target options
        wxString tstr;
        const wxArrayString& arr = target->GetLinkLibs();
        for (unsigned int x = 0; x < arr.GetCount(); ++x)
            tstr << FixupLinkLibraries(compiler, arr[x]) << _T(' ');

        // project options
        wxString pstr;
        const wxArrayString& parr = target->GetParentProject()->GetLinkLibs();
        for (unsigned int x = 0; x < parr.GetCount(); ++x)
            pstr << FixupLinkLibraries(compiler, parr[x]) << _T(' ');

        // decide order
        result = GetOrderedOptions(target, ortLinkerOptions, pstr, tstr);
    }

    // compiler link libraries
    wxString cstr;
    const wxArrayString& carr = compiler->GetLinkLibs();
    for (unsigned int x = 0; x < carr.GetCount(); ++x)
    {
        cstr << FixupLinkLibraries(compiler, carr[x]) << _T(' ');
    }
    result << cstr;

    // add in array
    return result;
} // end of SetupLinkLibraries

/// Setup resource compiler flags for build target.
wxString CompilerCommandGenerator::SetupResourceCompilerOptions(Compiler* compiler, ProjectBuildTarget* target)
{
    // resource compiler options are not implemented in C::B yet
    return wxEmptyString;
}

const wxArrayString& CompilerCommandGenerator::GetCompilerSearchDirs(ProjectBuildTarget* target)
{
    static wxArrayString retIfError;
    retIfError.Clear();

    SearchDirsMap::iterator it = m_CompilerSearchDirs.find(target);
    if (it == m_CompilerSearchDirs.end())
        return retIfError;

    return it->second;
}

const wxArrayString& CompilerCommandGenerator::GetLinkerSearchDirs(ProjectBuildTarget* target)
{
    static wxArrayString retIfError;
    retIfError.Clear();

    SearchDirsMap::iterator it = m_LinkerSearchDirs.find(target);
    if (it == m_LinkerSearchDirs.end())
        return retIfError;

    return it->second;
}

/** Arrange order of options.
  * Depending on the order defined for the build target, it concatenates
  * @c project_options with @c target_options and returns the result.
  */
wxString CompilerCommandGenerator::GetOrderedOptions(const ProjectBuildTarget* target, OptionsRelationType rel, const wxString& project_options, const wxString& target_options)
{
    wxString result;
    OptionsRelation relation = target->GetOptionRelation(rel);
    switch (relation)
    {
        case orUseParentOptionsOnly:
            result << project_options;
            break;
        case orUseTargetOptionsOnly:
            result << target_options;
            break;
        case orPrependToParentOptions:
            result << target_options << project_options;
            break;
        case orAppendToParentOptions:
            result << project_options << target_options;
            break;
    }
    return result;
}

/** Arrange order of options.
  * Depending on the order defined for the build target, it concatenates
  * @c project_options with @c target_options and returns the result.
  */
wxArrayString CompilerCommandGenerator::GetOrderedOptions(const ProjectBuildTarget* target, OptionsRelationType rel, const wxArrayString& project_options, const wxArrayString& target_options)
{
    wxArrayString result;
    OptionsRelation relation = target->GetOptionRelation(rel);
    switch (relation)
    {
        case orUseParentOptionsOnly:
            for (size_t i = 0; i < project_options.GetCount(); ++i)
                result.Add(project_options[i]);
            break;
        case orUseTargetOptionsOnly:
            for (size_t i = 0; i < target_options.GetCount(); ++i)
                result.Add(target_options[i]);
            break;
        case orPrependToParentOptions:
            for (size_t i = 0; i < target_options.GetCount(); ++i)
                result.Add(target_options[i]);
            for (size_t i = 0; i < project_options.GetCount(); ++i)
                result.Add(project_options[i]);
            break;
        case orAppendToParentOptions:
            for (size_t i = 0; i < project_options.GetCount(); ++i)
                result.Add(project_options[i]);
            for (size_t i = 0; i < target_options.GetCount(); ++i)
                result.Add(target_options[i]);
            break;
    }
    return result;
}

/** Adds support for backtick'd expressions under windows. */
wxString CompilerCommandGenerator::ExpandBackticks(wxString& str)
{
    wxString ret;

    // this function is not windows-only anymore because we parse the backticked command's output
    // for compiler/linker search dirs

    size_t start = str.find(_T('`'));
    if (start == wxString::npos)
        return ret; // no backticks here
    size_t end = str.find(_T('`'), start + 1);
    if (end == wxString::npos)
        return ret; // no ending backtick; error?

    while (start != wxString::npos && end != wxString::npos)
    {
        wxString cmd = str.substr(start + 1, end - start - 1);
        cmd.Trim(true);
        cmd.Trim(false);
        if (cmd.IsEmpty())
            break;

        wxString bt;
        BackticksMap::iterator it = m_Backticks.find(cmd);
        if (it != m_Backticks.end())
        {
            // in cache :)
            bt = it->second;
        }
        else
        {
            #if wxCHECK_VERSION(2, 9, 0)
            Manager::Get()->GetLogManager()->DebugLog(F(_T("Caching result of `%s`"), cmd.wx_str()));
            #else
            Manager::Get()->GetLogManager()->DebugLog(F(_T("Caching result of `%s`"), cmd.c_str()));
            #endif
            wxArrayString output;
            if (platform::WindowsVersion() >= platform::winver_WindowsNT2000)
                wxExecute(_T("cmd /c ") + cmd, output, wxEXEC_NODISABLE);
            else
                wxExecute(cmd, output, wxEXEC_NODISABLE);
            bt = GetStringFromArray(output, _T(" "));
            // add it in the cache
            m_Backticks[cmd] = bt;
            Manager::Get()->GetLogManager()->DebugLog(_T("Cached"));
        }
        ret << bt << _T(' ');
        str = str.substr(0, start) + bt + str.substr(end + 1, wxString::npos);

        // find next occurrence
        start = str.find(_T('`'));
        end = str.find(_T('`'), start + 1);
    }

    return ret;
}

// parse the result of a backticked expression for compiler/linker search dirs
void CompilerCommandGenerator::SearchDirsFromBackticks(Compiler* compiler, ProjectBuildTarget* target, const wxString& btOutput)
{
    if (btOutput.IsEmpty())
        return;

    // NOTE: this ignores spaces in search dirs
    // but usually backticks are only used under non-windows platforms by
    // large libs and they never use spaces in paths.
    // so, nobody should notice this :)

    // compiler search dirs
    size_t pos = 0;
    while (true)
    {
        pos = btOutput.find(compiler->GetSwitches().includeDirs, pos);
        if (pos == wxString::npos)
            break;

        pos += compiler->GetSwitches().includeDirs.Length();
        size_t pos2 = btOutput.find(_T(' '), pos);
        if (pos2 != pos)
        {
            if (pos2 == wxString::npos) // whole remaining string
                m_CompilerSearchDirs[target].Add(btOutput.Mid(pos, btOutput.Length() - pos));
            else
                m_CompilerSearchDirs[target].Add(btOutput.Mid(pos, pos2 - pos));
        }
        ++pos;
    }

    // linker search dirs
    pos = 0;
    while (true)
    {
        pos = btOutput.find(compiler->GetSwitches().libDirs, pos);
        if (pos == wxString::npos)
            break;

        pos += compiler->GetSwitches().libDirs.Length();
        size_t pos2 = btOutput.find(_T(' '), pos);
        if (pos2 != pos)
        {
            // note that backtick'd expressions always return full paths so no need to
            // re-normalize it here
            if (pos2 == wxString::npos) // whole remaining string
                m_LinkerSearchDirs[target].Add(btOutput.Mid(pos, btOutput.Length() - pos));
            else
                m_LinkerSearchDirs[target].Add(btOutput.Mid(pos, pos2 - pos));
        }
        ++pos;
    }
}
