/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef MSVCLOADER_H
#define MSVCLOADER_H

#include <wx/dynarray.h>
#include <wx/filename.h>
#include <wx/hashmap.h>
#include "ibaseloader.h"
#include "compiletargetbase.h" // for target type

// forward decls
class cbProject;
class ProjectBuildTarget;

class MSVCLoader : public IBaseLoader
{
	public:
		MSVCLoader(cbProject* project);
		virtual ~MSVCLoader();

		bool Open(const wxString& filename);
		bool Save(const wxString& filename);
	protected:
        bool ReadConfigurations();
        bool ParseConfiguration(int index);
        bool ParseSourceFiles();
        bool ParseResponseFile(const wxString filename, wxArrayString& output);
        void ProcessCompilerOptions(ProjectBuildTarget* target, const wxString& opts);
        void ProcessLinkerOptions(ProjectBuildTarget* target, const wxString& opts);
        void ProcessResourceCompilerOptions(ProjectBuildTarget* target, const wxString& opts);
        void ProcessPostBuildCommand(ProjectBuildTarget* target, const wxString& cmd);
        wxArrayString OptStringTokeniser(const wxString& opts);
        wxString RemoveQuotes(const wxString& src);

        cbProject* m_pProject;
        bool m_ConvertSwitches;
        wxArrayString m_Configurations;
        wxArrayInt m_ConfigurationsLineIndex;
        wxFileName m_Filename;
        TargetType m_Type;
        WX_DECLARE_STRING_HASH_MAP(TargetType, HashTargetType);
        HashTargetType m_TargType;
        HashTargetType m_TargetBasedOn;
        int m_BeginTargetLine;
	private:
};

#endif // MSVCLOADER_H

