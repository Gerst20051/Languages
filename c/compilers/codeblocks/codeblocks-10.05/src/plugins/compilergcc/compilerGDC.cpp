/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision: 5929 $
 * $Id: compilerGDC.cpp 5929 2009-11-21 14:34:53Z biplab $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/compilergcc/compilerGDC.cpp $
 */

#include <sdk.h>
#include <prep.h>
#include "compilerGDC.h"
#include <wx/intl.h>
#include <wx/regex.h>
#include <wx/config.h>
#include <wx/fileconf.h>
#include <wx/msgdlg.h>
#include "manager.h"
#include "logmanager.h"

#include <configmanager.h>

#ifdef __WXMSW__
    #include <wx/msw/registry.h>
#endif

CompilerGDC::CompilerGDC()
    : Compiler(_("GDC D Compiler"), _T("gdc"))
{
    Reset();
}

CompilerGDC::~CompilerGDC()
{
    //dtor
}

Compiler * CompilerGDC::CreateCopy()
{
    Compiler* c = new CompilerGDC(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
}

void CompilerGDC::Reset()
{
    if (platform::windows)
    {
        m_Programs.C = _T("mingw32-gdc.exe");
        m_Programs.CPP = _T("mingw32-gdc.exe");
        m_Programs.LD = _T("mingw32-gdc.exe");
        m_Programs.DBG = _T("gdb.exe");
        m_Programs.LIB = _T("ar.exe");
        m_Programs.WINDRES = _T("windres.exe");
        m_Programs.MAKE = _T("mingw32-make.exe");
    }
    else
    {
        m_Programs.C = _T("gdc");
        m_Programs.CPP = _T("gdc");
        m_Programs.LD = _T("gdc");
        m_Programs.DBG = _T("gdb");
        m_Programs.LIB = _T("ar");
        m_Programs.WINDRES = _T("");
        m_Programs.MAKE = _T("make");
    }
    m_Switches.includeDirs = _T("-I");
    m_Switches.libDirs = _T("-L");
    m_Switches.linkLibs = _T("-l");
    m_Switches.defines = _T("-fversion=");
    m_Switches.genericSwitch = _T("-");
    m_Switches.objectExtension = _T("o");
    m_Switches.needDependencies = true;
    m_Switches.forceCompilerUseQuotes = false;
    m_Switches.forceLinkerUseQuotes = false;
    m_Switches.logging = clogSimple;
    m_Switches.libPrefix = _T("lib");
    m_Switches.libExtension = _T("a");
    m_Switches.linkerNeedsLibPrefix = false;
    m_Switches.linkerNeedsLibExtension = false;

    // Summary of GCC options: http://gcc.gnu.org/onlinedocs/gcc/Option-Summary.html

    m_Options.ClearOptions();
    m_Options.AddOption(_("Produce debugging symbols"),
                _T("-g"),
                _("Debugging"),
                _T(""),
                true,
                _T("-O -O1 -O2 -O3 -Os"),
                _("You have optimizations enabled. This is Not A Good Thing(tm) when producing debugging symbols..."));
    wxString gprof_link = _T("-pg");
    if (platform::windows)
        gprof_link = _T("-pg -lgmon");
    m_Options.AddOption(_("Profile code when executed"), _T("-pg"), _("Profiling"), gprof_link);

    wxString category = _("Warnings");

    // warnings
    m_Options.AddOption(_("Enable all compiler warnings (overrides every other setting)"), _T("-Wall"), category);
    m_Options.AddOption(_("Enable standard compiler warnings"), _T("-W"), category);
    m_Options.AddOption(_("Stop compiling after first error"), _T("-Wfatal-errors"), category);
    m_Options.AddOption(_("Inhibit all warning messages"), _T("-w"), category);
    m_Options.AddOption(_("Enable warnings demanded by strict ISO C and ISO C++"), _T("-pedantic"), category);
    m_Options.AddOption(_("Treat as errors the warnings demanded by strict ISO C and ISO C++"), _T("-pedantic-errors"), category);
    m_Options.AddOption(_("Warn if main() is not conformant"), _T("-Wmain"), category);
    // D features
    category = _("D");
    m_Options.AddOption(_("generate documentation"), _T("-fdoc"), category);
    m_Options.AddOption(_("allow deprecated features"), _T("-fdeprecated"), category);
    m_Options.AddOption(_("compile in debug code"), _T("-debug"), category);
    m_Options.AddOption(_("inline expand functions"), _T("-finline-functions"), category);
    m_Options.AddOption(_("compile release version, which means not generating code for contracts and asserts"), _T("-frelease"), category);
    m_Options.AddOption(_("compile in unittest code, also turns on asserts"), _T("-funittest"), category);

    // optimization
    category = _("Optimization");
    m_Options.AddOption(_("Strip all symbols from binary (minimizes size)"), _T(""), category, _T("-s"), true, _T("-g -ggdb"), _("Stripping the binary will strip debugging symbols as well!"));
    m_Options.AddOption(_("Optimize generated code (for speed)"), _T("-O"), category);
    m_Options.AddOption(_("Optimize more (for speed)"), _T("-O1"), category);
    m_Options.AddOption(_("Optimize even more (for speed)"), _T("-O2"), category);
    m_Options.AddOption(_("Optimize fully (for speed)"), _T("-O3"), category);
    m_Options.AddOption(_("Optimize generated code (for size)"), _T("-Os"), category);
    m_Options.AddOption(_("Expensive optimizations"), _T("-fexpensive-optimizations"), category);
    // machine dependent options - cpu arch
    category = _("CPU architecture tuning (choose none, or only one of these)");
    m_Options.AddOption(_("i386"), _T("-march=i386"), category);
    m_Options.AddOption(_("i486"), _T("-march=i486"), category);
    m_Options.AddOption(_("Intel Pentium"), _T("-march=i586"), category);
    m_Options.AddOption(_("Intel Pentium (MMX)"), _T("-march=pentium-mmx"), category);
    m_Options.AddOption(_("Intel Pentium PRO"), _T("-march=i686"), category);
    m_Options.AddOption(_("Intel Pentium 2 (MMX)"), _T("-march=pentium2"), category);
    m_Options.AddOption(_("Intel Pentium 3 (MMX, SSE)"), _T("-march=pentium3"), category);
    m_Options.AddOption(_("Intel Pentium 4 (MMX, SSE, SSE2)"), _T("-march=pentium4"), category);
    m_Options.AddOption(_("Intel Pentium 4 Prescott (MMX, SSE, SSE2, SSE3)"), _T("-march=prescott"), category);
    m_Options.AddOption(_("Intel Pentium 4 Nocona (MMX, SSE, SSE2, SSE3, 64bit extensions)"), _T("-march=nocona"), category);
    m_Options.AddOption(_("Intel Pentium M (MMX, SSE, SSE2)"), _T("-march=pentium-m"), category);
    m_Options.AddOption(_("Intel Core2 (MMX, SSE, SSE2, SSE3, SSSE3, 64bit extensions)"), _T("-march=core2"), category);
    m_Options.AddOption(_("AMD K6 (MMX)"), _T("-march=k6"), category);
    m_Options.AddOption(_("AMD K6-2 (MMX, 3DNow!)"), _T("-march=k6-2"), category);
    m_Options.AddOption(_("AMD K6-3 (MMX, 3DNow!)"), _T("-march=k6-3"), category);
    m_Options.AddOption(_("AMD Athlon (MMX, 3DNow!, enhanced 3DNow!, SSE prefetch)"), _T("-march=athlon"), category);
    m_Options.AddOption(_("AMD Athlon Thunderbird (MMX, 3DNow!, enhanced 3DNow!, SSE prefetch)"), _T("-march=athlon-tbird"), category);
    m_Options.AddOption(_("AMD Athlon 4 (MMX, 3DNow!, enhanced 3DNow!, full SSE)"), _T("-march=athlon-4"), category);
    m_Options.AddOption(_("AMD Athlon XP (MMX, 3DNow!, enhanced 3DNow!, full SSE)"), _T("-march=athlon-xp"), category);
    m_Options.AddOption(_("AMD Athlon MP (MMX, 3DNow!, enhanced 3DNow!, full SSE)"), _T("-march=athlon-mp"), category);
    m_Options.AddOption(_("AMD K8 core (x86-64 instruction set)"), _T("-march=k8"), category);
    m_Options.AddOption(_("AMD Opteron (x86-64 instruction set)"), _T("-march=opteron"), category);
    m_Options.AddOption(_("AMD Athlon64 (x86-64 instruction set)"), _T("-march=athlon64"), category);
    m_Options.AddOption(_("AMD Athlon-FX (x86-64 instruction set)"), _T("-march=athlon-fx"), category);

    m_Commands[(int)ctCompileObjectCmd].push_back(CompilerTool(_T("$compiler $options $includes -c $file -o $object")));
    m_Commands[(int)ctGenDependenciesCmd].push_back(CompilerTool(_T("$compiler -MM $options -MF $dep_object -MT $object $includes $file")));
    m_Commands[(int)ctCompileResourceCmd].push_back(CompilerTool(_T("$rescomp -i $file -J rc -o $resource_output -O coff $res_includes")));
    m_Commands[(int)ctLinkConsoleExeCmd].push_back(CompilerTool(_T("$linker $libdirs -o $exe_output $link_objects $link_resobjects $link_options $libs")));
    if (platform::windows)
    {
        m_Commands[(int)ctLinkExeCmd].push_back(CompilerTool(_T("$linker $libdirs -o $exe_output $link_objects $link_resobjects $link_options $libs -mwindows")));
        m_Commands[(int)ctLinkDynamicCmd].push_back(CompilerTool(_T("$linker -shared -Wl,--output-def=$def_output -Wl,--out-implib=$static_output -Wl,--dll $libdirs $link_objects $link_resobjects -o $exe_output $link_options $libs")));
    }
    else
    {
        m_Commands[(int)ctLinkExeCmd] = m_Commands[(int)ctLinkConsoleExeCmd]; // no -mwindows
        m_Commands[(int)ctLinkDynamicCmd].push_back(CompilerTool(_T("$linker -shared $libdirs $link_objects $link_resobjects -o $exe_output $link_options $libs")));
    }
    m_Commands[(int)ctLinkStaticCmd].push_back(CompilerTool(_T("$lib_linker -r $static_output $link_objects\nranlib $static_output")));
    m_Commands[(int)ctLinkNativeCmd] = m_Commands[(int)ctLinkConsoleExeCmd]; // unsupported currently

    LoadDefaultRegExArray();

    m_CompilerOptions.Clear();
    m_LinkerOptions.Clear();
    m_LinkLibs.Clear();
    m_CmdsBefore.Clear();
    m_CmdsAfter.Clear();
}

void CompilerGDC::LoadDefaultRegExArray()
{
    m_RegExes.Clear();
    m_RegExes.Add(RegExStruct(_("Fatal error"), cltError, _T("FATAL:[ \t]*(.*)"), 1));
    m_RegExes.Add(RegExStruct(_("'Instantiated from here' info"), cltNormal, _T("(") + FilePathWithSpaces + _T("):([0-9]+):[ \t]+([iI]nstantiated from here.*)"), 3, 1, 2));    m_RegExes.Add(RegExStruct(_("Resource compiler error"), cltError, _T("windres.exe:[ \t](") + FilePathWithSpaces + _T("):([0-9]+):[ \t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Resource compiler error"), cltError, _T("windres.exe:[ \t](") + FilePathWithSpaces + _T("):([0-9]+):[ \t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Resource compiler error (2)"), cltError, _T("windres.exe:[ \t](.*)"), 1));
    m_RegExes.Add(RegExStruct(_("Preprocessor warning"), cltWarning, _T("(") + FilePathWithSpaces + _T("):([0-9]+):([0-9]+):[ \t]([Ww]arning:[ \t].*)"), 4, 1, 2));
    m_RegExes.Add(RegExStruct(_("Preprocessor error"), cltError, _T("(") + FilePathWithSpaces + _T("):([0-9]+):[0-9]+:[ \t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Compiler warning"), cltWarning, _T("(") + FilePathWithSpaces + _T("):([0-9]+):[ \t]([Ww]arning:[ \t].*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Compiler error"), cltError, _T("(") + FilePathWithSpaces + _T("):([0-9]+):[ \t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Linker error"), cltError, _T("(") + FilePathWithSpaces + _T("):([0-9]+):[0-9]+:[ \t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Linker error (2)"), cltError, FilePathWithSpaces + _T("\\(.text\\+[0-9A-Za-z]+\\):([ \tA-Za-z0-9_:+/\\.-]+):[ \t](.*)"), 2, 1));
    m_RegExes.Add(RegExStruct(_("Linker error (lib not found)"), cltError, _T(".*(ld.*):[ \t](cannot find.*)"), 2, 1));
    m_RegExes.Add(RegExStruct(_("Undefined reference"), cltError, _T("(") + FilePathWithSpaces + _T("):[ \t](undefined reference.*)"), 2, 1));
    m_RegExes.Add(RegExStruct(_("General warning"), cltWarning, _T("([Ww]arning:[ \t].*)"), 1));
}

AutoDetectResult CompilerGDC::AutoDetectInstallationDir()
{
    wxString sep = wxFileName::GetPathSeparator();
    if (platform::windows)
    {
        // look first if MinGW was installed with Code::Blocks (new in beta6)
        m_MasterPath = ConfigManager::GetExecutableFolder();
        if (!wxFileExists(m_MasterPath + sep + _T("bin") + sep + m_Programs.C))
        {
            // no... search for MinGW installation dir
            wxString windir = wxGetOSDirectory();
            wxFileConfig ini(_T(""), _T(""), windir + _T("/MinGW.ini"), _T(""), wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_NO_ESCAPE_CHARACTERS);
            m_MasterPath = ini.Read(_T("/InstallSettings/InstallPath"), _T("C:\\MinGW"));
            if (!wxFileExists(m_MasterPath + sep + _T("bin") + sep + m_Programs.C))
            {
#ifdef __WXMSW__ // for wxRegKey
                // not found...
                // look for dev-cpp installation
                wxRegKey key; // defaults to HKCR
                key.SetName(_T("HKEY_LOCAL_MACHINE\\Software\\Dev-C++"));
                if (key.Exists() && key.Open(wxRegKey::Read)) {
                    // found; read it
                    key.QueryValue(_T("Install_Dir"), m_MasterPath);
                }
                else {
                    // installed by inno-setup
                    // HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Minimalist GNU for Windows 4.1_is1
                    wxString name;
                    long index;
                    key.SetName(_T("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"));
                    //key.SetName("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion");
                    bool ok = key.GetFirstKey(name, index);
                    while (ok && !name.StartsWith(_T("Minimalist GNU for Windows"))) {
                        ok = key.GetNextKey(name, index);
                    }
                    if (ok) {
                        name = key.GetName() + _T("\\") + name;
                        key.SetName(name);
                        #if wxCHECK_VERSION(2, 9, 0)
                        Manager::Get()->GetLogManager()->DebugLog(F(_T("name: %s"), name.wx_str()));
                        #else
                        Manager::Get()->GetLogManager()->DebugLog(F(_T("name: %s"), name.c_str()));
                        #endif
                        if (key.Exists()) key.QueryValue(_T("InstallLocation"), m_MasterPath);
                    }
                }
#endif
            }
        }
        else
            m_Programs.MAKE = _T("make.exe"); // we distribute "make" not "mingw32-make"
    }
    else
        m_MasterPath = _T("/usr");

    AutoDetectResult ret = wxFileExists(m_MasterPath + sep + _T("bin") + sep + m_Programs.C) ? adrDetected : adrGuessed;
    if (ret == adrDetected)
    {
        AddIncludeDir(m_MasterPath + sep + _T("include") + sep + _T("d"));
        AddLibDir(m_MasterPath + sep + _T("lib"));
    }
    return ret;
}
