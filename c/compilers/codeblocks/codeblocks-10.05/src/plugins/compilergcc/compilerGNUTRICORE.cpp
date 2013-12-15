/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision: 5929 $
 * $Id: compilerGNUTRICORE.cpp 5929 2009-11-21 14:34:53Z biplab $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/compilergcc/compilerGNUTRICORE.cpp $
 */

#include <sdk.h>
#include <prep.h>
#ifndef CB_PRECOMP
    #include <wx/intl.h>
    #include <wx/regex.h>
    #include <wx/utils.h> // wxGetOSDirectory, wxGetEnv
#endif
#include <wx/filefn.h> // wxFileExists
#include <wx/fileconf.h> // wxFileConfig
#ifdef __WXMSW__
#include <wx/msw/registry.h>
#endif // __WXMSW__
#include "compilerGNUTRICORE.h"

CompilerGNUTRICORE::CompilerGNUTRICORE()
        : Compiler(_("GNU GCC Compiler for TriCore"),_T("tricore-gcc"))
{
    Reset();
}

CompilerGNUTRICORE::~CompilerGNUTRICORE()
{
    //dtor
}

Compiler * CompilerGNUTRICORE::CreateCopy()
{
    Compiler* c = new CompilerGNUTRICORE(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
}

void CompilerGNUTRICORE::Reset()
{
    if (platform::windows)
    {
        m_Programs.C = _T("tricore-gcc.exe");
        m_Programs.CPP = _T("tricore-g++.exe");
        m_Programs.LD = _T("tricore-g++.exe");
        m_Programs.DBG = _T("tricore-insight.exe");
        m_Programs.LIB = _T("tricore-ar.exe");
        m_Programs.WINDRES = _T("");
        m_Programs.MAKE = _T("make.exe");
    }
    else
    {
        m_Programs.C = _T("tricore-gcc");
        m_Programs.CPP = _T("tricore-g++");
        m_Programs.LD = _T("tricore-g++");
        m_Programs.DBG = _T("tricore-insight");
        m_Programs.LIB = _T("tricore-ar");
        m_Programs.WINDRES = _T("");
        m_Programs.MAKE = _T("make");
    }
    m_Switches.includeDirs = _T("-I");
    m_Switches.libDirs = _T("-L");
    m_Switches.linkLibs = _T("-l");
    m_Switches.defines = _T("-D");
    m_Switches.genericSwitch = _T("-");
    m_Switches.objectExtension = _T("o");
    m_Switches.needDependencies = true;
    m_Switches.forceCompilerUseQuotes = false;
    m_Switches.forceLinkerUseQuotes = false;
    m_Switches.logging = clogFull;
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
                        _("You have optimizations enabled. This will make debugging difficult because variables may be optimized away etc."));
    wxString gprof_link = _T("-pg");
    if (platform::windows)
        gprof_link = _T("-pg -lgmon");
    m_Options.AddOption(_("Profile code when executed"), _T("-pg"), _("Profiling"), gprof_link);

    wxString category = _("Warnings");

    // warnings
    m_Options.AddOption(_("In C mode, support all ISO C90 programs. In C++ mode, remove GNU extensions that conflict with ISO C++"), _T("-ansi"), category);
    m_Options.AddOption(_("Enable all compiler warnings (overrides every other setting)"), _T("-Wall"), category);
    m_Options.AddOption(_("Enable standard compiler warnings"), _T("-W"), category);
    m_Options.AddOption(_("Stop compiling after first error"), _T("-Wfatal-errors"), category);
    m_Options.AddOption(_("Inhibit all warning messages"), _T("-w"), category);
    m_Options.AddOption(_("Enable warnings demanded by strict ISO C and ISO C++"), _T("-pedantic"), category);
    m_Options.AddOption(_("Treat as errors the warnings demanded by strict ISO C and ISO C++"), _T("-pedantic-errors"), category);
    m_Options.AddOption(_("Warn if main() is not conformant"), _T("-Wmain"), category);

    category = _("General Options");

    // general options
    m_Options.AddOption(_("Output an error if same variable is declared without extern in different modules"), _T("-fno-common"), category);
    m_Options.AddOption(_("Do not allocate to an enum type only as many bytes as it needs for the declared range of possible values"), _T("-fno-short-enums"), category);
    m_Options.AddOption(_("Save intermediate files in the build directory"), _T("-save-temps"), category);

    // Startup options
    category = _("Linker and startup code");
    m_Options.AddOption(_("do not link against the default crt0.o, so you can add your own startup code (TriCore specific)"), _T("-nocrt0"), category);
    m_Options.AddOption(_("do not link against standard system startup files"), _T("-nostartfiles"), category);
    m_Options.AddOption(_("only search library directories explicitly specified on the command line"), _T("-nostdlib"), category);

    // optimization
    category = _("Optimization");
    m_Options.AddOption(_("Strip all symbols from binary (minimizes size)"), _T(""), category, _T("-s"), true, _T("-g -ggdb"), _("Stripping the binary will strip debugging symbols as well!"));
    m_Options.AddOption(_("Optimize generated code (for speed)"), _T("-O"), category);
    m_Options.AddOption(_("Optimize more (for speed)"), _T("-O1"), category);
    m_Options.AddOption(_("Optimize even more (for speed)"), _T("-O2"), category);
    m_Options.AddOption(_("Optimize fully (for speed)"), _T("-O3"), category);
    m_Options.AddOption(_("Optimize generated code (for size)"), _T("-Os"), category);
    m_Options.AddOption(_("Expensive optimizations"), _T("-fexpensive-optimizations"), category);
    m_Options.AddOption(_("No instruction scheduling before reload"), _T("-fno-schedule-insns"), category);
    m_Options.AddOption(_("No instruction scheduling after reload"), _T("-fno-schedule-insns2"), category);

    // machine dependent options
    category = _("TriCore achitecture specific");

    m_Options.AddOption(_("jump tables in text section"), _T("-mjumptable-in-textsection"), category);
    m_Options.AddOption(_("include C source lines into assembler output"), _T("-masm-source-lines"), category);
    m_Options.AddOption(_("base relative addresses for small data objects"), _T("-msmall-pid"), category);
    m_Options.AddOption(_("allow _bit in structures and unions"), _T("-mbits-struct-unions"), category);
    m_Options.AddOption(_("allow callee and caller in different sections"), _T("-mnocallerrors"), category);
    m_Options.AddOption(_("allocate data in aligned section"), _T("-maligned-data-sections"), category);
    m_Options.AddOption(_("enable attribute alignedaccess"), _T("-maligned-access"), category);
    m_Options.AddOption(_("use optimized single float emulation"), _T("-moptfp"), category);
    m_Options.AddOption(_("layout bitfields EABI conform"), _T("-meabi-bitfields"), category);
    m_Options.AddOption(_("EABI conform"), _T("-meabi"), category);

    // machine dependent options
    category = _("TriCore MCU derivatives");

    m_Options.AddOption(_("TriCore TC1130"), _T("-mcpu=tc1130"), category);
    m_Options.AddOption(_("TriCore TC1130 BA step"), _T("-mcpu=tc1130ba"), category);
    m_Options.AddOption(_("TriCore TC1161"), _T("-mcpu=tc1161"), category);
    m_Options.AddOption(_("TriCore TC1162"), _T("-mcpu=tc1162"), category);
    m_Options.AddOption(_("TriCore TC1762"), _T("-mcpu=tc1762"), category);
    m_Options.AddOption(_("TriCore TC1765"), _T("-mcpu=tc1765"), category);
    m_Options.AddOption(_("TriCore TC1766"), _T("-mcpu=tc1766"), category);
    m_Options.AddOption(_("TriCore TC1767"), _T("-mcpu=tc1767"), category);
    m_Options.AddOption(_("TriCore TC1775"), _T("-mcpu=tc1775"), category);
    m_Options.AddOption(_("TriCore TC1792"), _T("-mcpu=tc1792"), category);
    m_Options.AddOption(_("TriCore TC1796"), _T("-mcpu=tc1796"), category);
    m_Options.AddOption(_("TriCore TC1797"), _T("-mcpu=tc1797"), category);
    m_Options.AddOption(_("TriCore TC1920"), _T("-mcpu=tc1920"), category);

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
    m_Commands[(int)ctLinkStaticCmd].push_back(CompilerTool(_T("$lib_linker -rs $static_output $link_objects")));
    m_Commands[(int)ctLinkNativeCmd] = m_Commands[(int)ctLinkConsoleExeCmd]; // unsupported currently

    LoadDefaultRegExArray();

    m_CompilerOptions.Clear();
    m_LinkerOptions.Clear();
    m_LinkLibs.Clear();
    m_CmdsBefore.Clear();
    m_CmdsAfter.Clear();
}

void CompilerGNUTRICORE::LoadDefaultRegExArray()
{
    m_RegExes.Clear();
    m_RegExes.Add(RegExStruct(_("Fatal error"), cltError, _T("FATAL:[ \t]*(.*)"), 1));
    m_RegExes.Add(RegExStruct(_("'In function...' info"), cltInfo, _T("(") + FilePathWithSpaces + _T("):[ \t]+") + _T("([iI]n ([cC]lass|[cC]onstructor|[dD]estructor|[fF]unction|[mM]ember [fF]unction).*)"), 2, 1));
    m_RegExes.Add(RegExStruct(_("'Instantiated from' info"), cltInfo, _T("(") + FilePathWithSpaces + _T("):([0-9]+):[ \t]+([iI]nstantiated from .*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Resource compiler error"), cltError, _T("windres.exe:[ \t](") + FilePathWithSpaces + _T("):([0-9]+):[ \t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Resource compiler error (2)"), cltError, _T("windres.exe:[ \t](.*)"), 1));
    m_RegExes.Add(RegExStruct(_("Preprocessor warning"), cltWarning, _T("(") + FilePathWithSpaces + _T("):([0-9]+):([0-9]+):[ \t]([Ww]arning:[ \t].*)"), 4, 1, 2));
    m_RegExes.Add(RegExStruct(_("Preprocessor error"), cltError, _T("(") + FilePathWithSpaces + _T("):([0-9]+):[0-9]+:[ \t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Compiler note"), cltInfo, _T("(") + FilePathWithSpaces + _T("):([0-9]+):[ \t]([Nn]ote:[ \t].*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("General note"), cltInfo, _T("([Nn]ote:[ \t].*)"), 1));
    m_RegExes.Add(RegExStruct(_("Compiler warning"), cltWarning, _T("(") + FilePathWithSpaces + _T("):([0-9]+):[ \t]([Ww]arning:[ \t].*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Compiler error"), cltError, _T("(") + FilePathWithSpaces + _T("):([0-9]+):[ \t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Linker error"), cltError, _T("(") + FilePathWithSpaces + _T("):([0-9]+):[0-9]+:[ \t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Linker error (2)"), cltError, FilePathWithSpaces + _T("\\(.text\\+[0-9A-Za-z]+\\):([ \tA-Za-z0-9_:+/\\.-]+):[ \t](.*)"), 2, 1));
    m_RegExes.Add(RegExStruct(_("Linker error (lib not found)"), cltError, _T(".*(ld.*):[ \t](cannot find.*)"), 2, 1));
    m_RegExes.Add(RegExStruct(_("Undefined reference"), cltError, _T("(") + FilePathWithSpaces + _T("):[ \t](undefined reference.*)"), 2, 1));
    m_RegExes.Add(RegExStruct(_("General warning"), cltWarning, _T("([Ww]arning:[ \t].*)"), 1));
    m_RegExes.Add(RegExStruct(_("Auto-import info"), cltInfo, _T("([Ii]nfo:[ \t].*)\\(auto-import\\)"), 1));
}

AutoDetectResult CompilerGNUTRICORE::AutoDetectInstallationDir()
{
    wxString sep = wxFileName::GetPathSeparator();
#ifdef __WXMSW__
    m_MasterPath = _T("C:\\HighTec\\TriCore"); // just a guess

    //    wxLogNull ln;
    wxRegKey key; // defaults to HKCR
    key.SetName(_T("HKEY_LOCAL_MACHINE\\Software\\HighTec EDV-Systeme\\TriCore\\"));
    if (key.Exists() && key.Open(wxRegKey::Read))
    {
     // found; read it
    	if (key.HasValue(_T("InstallPath")))
	{
		key.QueryValue(_T("InstallPath"), m_MasterPath);
        }
     }
#else
        m_MasterPath = _T("/usr/local/tricore");
#endif // __WXMSW__
    AutoDetectResult ret = wxFileExists(m_MasterPath + sep + _T("bin") + sep + m_Programs.C) ? adrDetected : adrGuessed;
    if (ret == adrDetected)
    {
          AddIncludeDir(m_MasterPath + sep + _T("tricore") + sep + _T("include"));
    }
    return ret;
}
