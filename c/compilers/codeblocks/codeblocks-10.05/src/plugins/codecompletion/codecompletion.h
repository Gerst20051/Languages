/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef CODECOMPLETION_H
#define CODECOMPLETION_H

#include <settings.h> // SDK
#include <cbplugin.h>
#include <cbproject.h>
#include <sdk_events.h>
#include <wx/string.h>

#include "nativeparser.h"

#include <wx/timer.h>
#include <map>
#include <vector>
#include <set>

class cbEditor;
class wxScintillaEvent;
class wxChoice;

class CodeCompletion : public cbCodeCompletionPlugin
{
    public:
	struct FunctionScope
	{
		int StartLine;
		int EndLine;
		wxString Name;
		wxString Scope;
	};

	struct NameSpace
	{
		int StartLine;
		int EndLine;
		wxString Name;
	};

	typedef std::vector<FunctionScope> FunctionsScopeVec;
	typedef std::vector<NameSpace> NameSpaceVec;

	struct FunctionsScopePerFile
	{
		FunctionsScopeVec m_FunctionsScope;
		NameSpaceVec m_NameSpaces;
		bool parsed;
	};

	typedef map<wxString,FunctionsScopePerFile> FunctionsScopeMap;

        CodeCompletion();
        ~CodeCompletion();

        virtual void OnAttach();
        virtual void OnRelease(bool appShutDown);
        virtual int GetConfigurationGroup() const { return cgEditor; }
        virtual cbConfigurationPanel* GetConfigurationPanel(wxWindow* parent);
        virtual cbConfigurationPanel* GetProjectConfigurationPanel(wxWindow* parent, cbProject* project);
        virtual int Configure();
        virtual void BuildMenu(wxMenuBar* menuBar); // offer for menu space by host
        virtual void BuildModuleMenu(const ModuleType type, wxMenu* menu, const FileTreeData* data = 0);
        virtual bool BuildToolBar(wxToolBar* toolBar);
        virtual wxArrayString GetCallTips();
        virtual int CodeComplete();
        virtual void ShowCallTip();

        virtual void CodeCompleteIncludes();

        void EditorEventHook(cbEditor* editor, wxScintillaEvent& event);

		void RereadOptions(); // called by the configuration panel
    private:

        void LoadTokenReplacements();
        void SaveTokenReplacements();

        void OnUpdateUI(wxUpdateUIEvent& event);
        void OnViewClassBrowser(wxCommandEvent& event);
        void OnCodeComplete(wxCommandEvent& event);
        void OnShowCallTip(wxCommandEvent& event);
        void OnGotoFunction(wxCommandEvent& event);
        void OnGotoPrevFunction(wxCommandEvent& event);
        void OnGotoNextFunction(wxCommandEvent& event);
        void OnClassMethod(wxCommandEvent& event);
        void OnUnimplementedClassMethods(wxCommandEvent& event);
        void OnGotoDeclaration(wxCommandEvent& event);
        void OnOpenIncludeFile(wxCommandEvent& event);
        void OnAppDoneStartup(CodeBlocksEvent& event);
        void OnCodeCompleteTimer(wxTimerEvent& event);
        void OnWorkspaceChanged(CodeBlocksEvent& event);
        void OnProjectActivated(CodeBlocksEvent& event);
        void OnProjectClosed(CodeBlocksEvent& event);
        void OnProjectSaved(CodeBlocksEvent& event);
        void OnProjectFileAdded(CodeBlocksEvent& event);
        void OnProjectFileRemoved(CodeBlocksEvent& event);
        void OnUserListSelection(CodeBlocksEvent& event);
        void OnReparseActiveEditor(CodeBlocksEvent& event);
        void OnEditorOpen(CodeBlocksEvent& event);
        void OnEditorActivated(CodeBlocksEvent& event);
        void OnEditorClosed(CodeBlocksEvent& event);
        void OnParserEnd(wxCommandEvent& event);
        void OnValueTooltip(CodeBlocksEvent& event);
        void DoCodeComplete();
        void DoInsertCodeCompleteToken(wxString tokName);
        int DoClassMethodDeclImpl();
        int DoAllMethodsImpl();
        int FunctionPosition() const;
        void GotoFunctionPrevNext(bool next = false);
        int NameSpacePosition() const;
        void ParseActiveProjects();
        void OnStartParsingFunctions(wxTimerEvent& event);
        void OnFunction(wxCommandEvent& event);
        void ParseFunctionsAndFillToolbar(bool force = false);
        void FindFunctionAndUpdate(int currentLine);

        int                                m_PageIndex;
        bool                               m_InitDone;

        wxMenu*                            m_EditMenu;
        wxMenu*                            m_SearchMenu;
        wxMenu*                            m_ViewMenu;
        NativeParser                       m_NativeParser;
        set<cbProject*, less<cbProject*> > m_ParsedProjects;

        int                                m_EditorHookId;
        int                                m_LastPosForCodeCompletion;
        wxTimer                            m_TimerCodeCompletion;
        wxTimer                            m_TimerFunctionsParsing;
        cbEditor*                          m_pCodeCompletionLastEditor;
        int                                m_ActiveCalltipsNest;

        bool                               m_IsAutoPopup;

        wxChoice*                          m_Function;
        wxChoice*                          m_Scope;
        FunctionsScopeVec                  m_FunctionsScope;
        NameSpaceVec                       m_NameSpaces;
        FunctionsScopeMap                  m_AllFunctionsScopes;
        bool                               m_ToolbarChanged;

        int                                m_StartIdxNameSpaceInScope;
        int                                m_CurrentLine;
        map<wxString, int>                 m_SearchItem;
        wxString                           m_LastFile;
        bool                               m_LexerKeywordsToInclude[9];
        bool                               m_NeedReparse;

        DECLARE_EVENT_TABLE()
};

#endif // CODECOMPLETION_H
