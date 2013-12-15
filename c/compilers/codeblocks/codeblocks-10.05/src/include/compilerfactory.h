/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef COMPILERFACTORY_H
#define COMPILERFACTORY_H

#ifndef CB_PRECOMP
    #include <wx/arrstr.h> // WX_DEFINE_ARRAY
    #include "compiler.h" // Compiler
#endif

#include "settings.h"

class Compiler;

// Well, not really a factory ;)

WX_DEFINE_ARRAY(Compiler*, CompilersArray);

class DLLIMPORT CompilerFactory
{
    public:
        /// @return the number of registered compilers.
        static size_t GetCompilersCount();
        /// @return the compiler by an index.
        static Compiler* GetCompiler(size_t index);
        /// @return the compiler by a name (ID). *Not* the compiler's title...
        static Compiler* GetCompiler(const wxString& id);
        /// @return the compiler by title.
        static Compiler* GetCompilerByName(const wxString& title);

        /// @return the compiler's index from its id. Returns -1 if it doesn't exist.
        static int GetCompilerIndex(const wxString& id);
        /// @return the compiler's index. Returns -1 if it doesn't exist.
        static int GetCompilerIndex(Compiler* compiler);

        /// @return true if the specified compiler ID is valid, false if not.
        static bool IsValidCompilerID(const wxString& id){ return GetCompilerIndex(id) != -1; }

        /// @return true if compiler ID @c id inherits, directly or indirectly, from compiler ID @c from_id.
        static bool CompilerInheritsFrom(const wxString& id, const wxString& from_id);
        /// @return true if @c compiler inherits, directly or indirectly, from compiler ID @c from_id.
        static bool CompilerInheritsFrom(Compiler* compiler, const wxString& from_id);

        /// Register a supported (builtin) compiler.
        static void RegisterCompiler(Compiler* compiler);
        /// Register all user-defined compiler copies.
        static void RegisterUserCompilers();
        /// Create a copy of a compiler.
        static Compiler* CreateCompilerCopy(Compiler* compiler, const wxString& newName);
        /// Remove a compiler.
        static void RemoveCompiler(Compiler* compiler);
        /// Unregister all compilers.
        static void UnregisterCompilers();
        /// get the version number as string for the compiler with the specified index
        static wxString GetCompilerVersionString(const wxString& Id);

        static void SaveSettings();
        static void LoadSettings();

        static const wxString& GetDefaultCompilerID();
        static Compiler* GetDefaultCompiler();
        static void SetDefaultCompiler(size_t index);
        static void SetDefaultCompiler(const wxString& id);
        static void SetDefaultCompiler(Compiler* compiler);

        static Compiler* SelectCompilerUI(const wxString& message = _("Select compiler"), const wxString& preselectedID = wxEmptyString);
    private:
        static CompilersArray Compilers;
        static Compiler* s_DefaultCompiler;
};

#endif // COMPILERFACTORY_H
