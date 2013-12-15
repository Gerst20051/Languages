/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision: 4909 $
 * $Id: compileoptionsbase.cpp 4909 2008-02-27 13:15:26Z mortenmacfly $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/sdk/compileoptionsbase.cpp $
 */

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include "compileoptionsbase.h"
    #include "sdk_events.h"
    #include "manager.h"
    #include "macrosmanager.h"
#endif

namespace { static const bool case_sensitive = platform::windows ? false : true; };

CompileOptionsBase::CompileOptionsBase()
	: m_Platform(spAll),
	m_Modified(false),
	m_AlwaysRunPostCmds(false)
{
	//ctor
}

CompileOptionsBase::~CompileOptionsBase()
{
	//dtor
}

void CompileOptionsBase::AddPlatform(int platform)
{
	if (m_Platform & platform)
		return;
	m_Platform |= platform;
	SetModified(true);
}

void CompileOptionsBase::RemovePlatform(int platform)
{
	if (!(m_Platform & platform))
		return;
	m_Platform &= ~platform;
	SetModified(true);
}

void CompileOptionsBase::SetPlatforms(int platforms)
{
	if (m_Platform == platforms)
		return;
	m_Platform = platforms;
	SetModified(true);
}

int CompileOptionsBase::GetPlatforms() const
{
	return m_Platform;
}

bool CompileOptionsBase::SupportsCurrentPlatform() const
{
	if(platform::windows)
        return m_Platform & spWindows;
	if(platform::unix)
        return m_Platform & spUnix;
	if(platform::macosx)
        return m_Platform & spMac;

    return false;
}

void CompileOptionsBase::SetLinkerOptions(const wxArrayString& linkerOpts)
{
	if (m_LinkerOptions == linkerOpts)
		return;
	m_LinkerOptions = linkerOpts;
	SetModified(true);
}

const wxArrayString& CompileOptionsBase::GetLinkerOptions() const
{
	return m_LinkerOptions;
}

void CompileOptionsBase::SetLinkLibs(const wxArrayString& linkLibs)
{
    if (m_LinkLibs == linkLibs)
        return;
    m_LinkLibs = linkLibs;
    SetModified(true);
}

const wxArrayString& CompileOptionsBase::GetLinkLibs() const
{
    return m_LinkLibs;
}

void CompileOptionsBase::SetCompilerOptions(const wxArrayString& compilerOpts)
{
	if (m_CompilerOptions == compilerOpts)
		return;
	m_CompilerOptions = compilerOpts;
	SetModified(true);
}

const wxArrayString& CompileOptionsBase::GetCompilerOptions() const
{
	return m_CompilerOptions;
}

void CompileOptionsBase::SetIncludeDirs(const wxArrayString& includeDirs)
{
	if (m_IncludeDirs == includeDirs)
		return;

    // make sure we don't have duplicate entries
    // that's why we don't assign the array but rather copy it entry by entry...

    m_IncludeDirs.Clear();
    for (size_t i = 0; i < includeDirs.GetCount(); ++i)
    {
        wxString entry = UnixFilename(includeDirs[i]);
        if (m_IncludeDirs.Index(entry, case_sensitive) == wxNOT_FOUND)
            m_IncludeDirs.Add(entry);
    }
	SetModified(true);
}

const wxArrayString& CompileOptionsBase::GetIncludeDirs() const
{
	return m_IncludeDirs;
}

void CompileOptionsBase::SetResourceIncludeDirs(const wxArrayString& resIncludeDirs)
{
	if (m_ResIncludeDirs == resIncludeDirs)
		return;

    // make sure we don't have duplicate entries
    // that's why we don't assign the array but rather copy it entry by entry...

    m_ResIncludeDirs.Clear();
    for (size_t i = 0; i < resIncludeDirs.GetCount(); ++i)
    {
        wxString entry = UnixFilename(resIncludeDirs[i]);
        if (m_ResIncludeDirs.Index(entry, case_sensitive) == wxNOT_FOUND)
            m_ResIncludeDirs.Add(entry);
    }
	SetModified(true);
}

const wxArrayString& CompileOptionsBase::GetResourceIncludeDirs() const
{
    return m_ResIncludeDirs;
}

void CompileOptionsBase::SetLibDirs(const wxArrayString& libDirs)
{
	if (m_LibDirs == libDirs)
		return;

    // make sure we don't have duplicate entries
    // that's why we don't assign the array but rather copy it entry by entry...

    m_LibDirs.Clear();
    for (size_t i = 0; i < libDirs.GetCount(); ++i)
    {
        wxString entry = UnixFilename(libDirs[i]);
        if (m_LibDirs.Index(entry, case_sensitive) == wxNOT_FOUND)
            m_LibDirs.Add(entry);
    }
	SetModified(true);
}

const wxArrayString& CompileOptionsBase::GetLibDirs() const
{
	return m_LibDirs;
}

void CompileOptionsBase::SetBuildScripts(const wxArrayString& scripts)
{
	if (m_Scripts == scripts)
		return;

    // make sure we don't have duplicate entries
    // that's why we don't assign the array but rather copy it entry by entry...

    m_Scripts.Clear();
    for (size_t i = 0; i < scripts.GetCount(); ++i)
    {
        wxString entry = UnixFilename(scripts[i]);
        if (m_Scripts.Index(entry, case_sensitive) == wxNOT_FOUND)
            m_Scripts.Add(entry);
    }
	SetModified(true);
}

const wxArrayString& CompileOptionsBase::GetBuildScripts() const
{
    return m_Scripts;
}

void CompileOptionsBase::SetCommandsBeforeBuild(const wxArrayString& commands)
{
	if (m_CmdsBefore == commands)
		return;
	m_CmdsBefore = commands;
	SetModified(true);
}

const wxArrayString& CompileOptionsBase::GetCommandsBeforeBuild() const
{
	return m_CmdsBefore;
}

void CompileOptionsBase::SetCommandsAfterBuild(const wxArrayString& commands)
{
	if (m_CmdsAfter == commands)
		return;
	m_CmdsAfter = commands;
	SetModified(true);
}

const wxArrayString& CompileOptionsBase::GetCommandsAfterBuild() const
{
	return m_CmdsAfter;
}

bool CompileOptionsBase::GetAlwaysRunPostBuildSteps() const
{
    return m_AlwaysRunPostCmds;
}

void CompileOptionsBase::SetAlwaysRunPostBuildSteps(bool always)
{
    if (m_AlwaysRunPostCmds == always)
        return;
    m_AlwaysRunPostCmds = always;
    SetModified(true);
}

bool CompileOptionsBase::GetModified() const
{
	return m_Modified;
}

void CompileOptionsBase::SetModified(bool modified)
{
	m_Modified = modified;
}

void CompileOptionsBase::AddLinkerOption(const wxString& option)
{
    wxString envopt = option;
    if (m_LinkerOptions.Index(envopt, case_sensitive) == wxNOT_FOUND)
    {
        m_LinkerOptions.Add(envopt);
        SetModified(true);
    }
}

void CompileOptionsBase::AddLinkLib(const wxString& lib)
{
    wxString envopt = lib;
    if (m_LinkLibs.Index(envopt, case_sensitive) == wxNOT_FOUND)
    {
        m_LinkLibs.Add(envopt);
        SetModified(true);
    }
}

void CompileOptionsBase::AddCompilerOption(const wxString& option)
{
    wxString envopt = option;
    if (m_CompilerOptions.Index(envopt, case_sensitive) == wxNOT_FOUND)
    {
        m_CompilerOptions.Add(envopt);
        SetModified(true);
    }
}

void CompileOptionsBase::AddIncludeDir(const wxString& option)
{
    wxString entry = UnixFilename(option);
    if (m_IncludeDirs.Index(entry, case_sensitive) == wxNOT_FOUND)
    {
        m_IncludeDirs.Add(entry);
        SetModified(true);
    }
}

void CompileOptionsBase::AddResourceIncludeDir(const wxString& option)
{
    wxString entry = UnixFilename(option);
    if (m_ResIncludeDirs.Index(entry, case_sensitive) == wxNOT_FOUND)
    {
        m_ResIncludeDirs.Add(entry);
        SetModified(true);
    }
}

void CompileOptionsBase::AddLibDir(const wxString& option)
{
    wxString entry = UnixFilename(option);
    if (m_LibDirs.Index(entry, case_sensitive) == wxNOT_FOUND)
    {
        m_LibDirs.Add(entry);
        SetModified(true);
    }
}

void CompileOptionsBase::AddCommandsBeforeBuild(const wxString& command)
{
    wxString envopt = command;
	m_CmdsBefore.Add(envopt);
	SetModified(true);
}

void CompileOptionsBase::AddCommandsAfterBuild(const wxString& command)
{
    wxString envopt = command;
	m_CmdsAfter.Add(envopt);
	SetModified(true);
}

void CompileOptionsBase::AddBuildScript(const wxString& script)
{
    wxString envopt = UnixFilename(script);
    if (m_Scripts.Index(envopt, case_sensitive) == wxNOT_FOUND)
    {
        m_Scripts.Add(envopt);
        SetModified(true);
    }
}

void CompileOptionsBase::RemoveLinkerOption(const wxString& option)
{
    wxString envopt = option;
    int idx = m_LinkerOptions.Index(envopt, case_sensitive);
    if (idx != wxNOT_FOUND)
    {
        m_LinkerOptions.RemoveAt(idx);
        SetModified(true);
    }
}

void CompileOptionsBase::RemoveLinkLib(const wxString& lib)
{
    wxString envopt = lib;
    int idx = m_LinkLibs.Index(envopt, case_sensitive);
    if (idx != wxNOT_FOUND)
    {
        m_LinkLibs.RemoveAt(idx);
        SetModified(true);
    }
}

void CompileOptionsBase::RemoveCompilerOption(const wxString& option)
{
    wxString envopt = option;
    int idx = m_CompilerOptions.Index(envopt, case_sensitive);
    if (idx != wxNOT_FOUND)
    {
        m_CompilerOptions.RemoveAt(idx);
        SetModified(true);
    }
}

void CompileOptionsBase::RemoveIncludeDir(const wxString& option)
{
    wxString entry = UnixFilename(option);
    int idx = m_IncludeDirs.Index(entry, case_sensitive);
    if (idx != wxNOT_FOUND)
    {
        m_IncludeDirs.RemoveAt(idx);
        SetModified(true);
    }
}

void CompileOptionsBase::RemoveResourceIncludeDir(const wxString& option)
{
    wxString entry = UnixFilename(option);
    int idx = m_ResIncludeDirs.Index(entry, case_sensitive);
    if (idx != wxNOT_FOUND)
    {
        m_ResIncludeDirs.RemoveAt(idx);
        SetModified(true);
    }
}

void CompileOptionsBase::RemoveLibDir(const wxString& option)
{
    wxString entry = UnixFilename(option);
    int idx = m_LibDirs.Index(entry, case_sensitive);
    if (idx != wxNOT_FOUND)
    {
        m_LibDirs.RemoveAt(idx);
        SetModified(true);
    }
}

void CompileOptionsBase::RemoveCommandsBeforeBuild(const wxString& command)
{
    wxString envopt = command;
	m_CmdsBefore.Remove(envopt);
	SetModified(true);
}

void CompileOptionsBase::RemoveCommandsAfterBuild(const wxString& command)
{
    wxString envopt = command;
	m_CmdsAfter.Remove(envopt);
	SetModified(true);
}

void CompileOptionsBase::RemoveBuildScript(const wxString& script)
{
    wxString envopt = UnixFilename(script);
    int idx = m_Scripts.Index(envopt, case_sensitive);
    if (idx != wxNOT_FOUND)
    {
        m_Scripts.RemoveAt(idx);
        SetModified(true);
    }
}

bool CompileOptionsBase::SetVar(const wxString& key, const wxString& value, bool onlyIfExists)
{
    if (onlyIfExists)
    {
        StringHash::iterator it = m_Vars.find(key);
        if (it == m_Vars.end())
            return false;
        it->second = value;
        return true;
    }

    m_Vars[key] = value;
    SetModified(true);
    return true;
}

bool CompileOptionsBase::UnsetVar(const wxString& key)
{
    StringHash::iterator it = m_Vars.find(key);
    if (it != m_Vars.end())
    {
        m_Vars.erase(it);
        SetModified(true);
        return true;
    }
    return false;
}

void CompileOptionsBase::UnsetAllVars()
{
    m_Vars.clear();
}

const wxString& CompileOptionsBase::GetVar(const wxString& key) const
{
    StringHash::const_iterator it = m_Vars.find(key);
    if (it != m_Vars.end())
        return it->second;

    static wxString emptystring = wxEmptyString;
    return emptystring;
}

const StringHash& CompileOptionsBase::GetAllVars() const
{
    return m_Vars;
}
