/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * Copyright: 2008 Jens Lody
 *
 * $Revision: 6050 $
 * $Id: IncrementalSearch.h 6050 2010-01-05 11:37:49Z jenslody $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/IncrementalSearch/IncrementalSearch.h $
 */

#ifndef INCREMENTAL_SEARCH_H_INCLUDED
#define INCREMENTAL_SEARCH_H_INCLUDED

#include <cbplugin.h>

class cbStyledTextCtrl;

class IncrementalSearch : public cbPlugin
{
public:
    IncrementalSearch();
    ~IncrementalSearch();

    int GetConfigurationPriority() const
    {
        return 50;
    }
    int GetConfigurationGroup() const
    {
        return cgEditor;
    }
    cbConfigurationPanel* GetConfigurationPanel(wxWindow* parent);
    cbConfigurationPanel* GetProjectConfigurationPanel(wxWindow* parent, cbProject* project)
    {
        return 0;
    }
    void BuildMenu(wxMenuBar* menuBar);
    void BuildModuleMenu(const ModuleType type, wxMenu* menu, const FileTreeData* data = 0) {}
    bool BuildToolBar(wxToolBar* toolBar);

private:
    void OnAttach();
    void OnRelease(bool appShutDown);
    void OnKeyDown(wxKeyEvent& event);
    void OnFocusToolbar(wxCommandEvent& event);
    void OnTextChanged(wxCommandEvent& event);
    void OnKillFocus(wxCommandEvent& event);
    void OnClearText(wxCommandEvent& event);
    void OnSearchPrev(wxCommandEvent& event);
    void OnSearchNext(wxCommandEvent& event);
    void OnToggleToolbar(wxCommandEvent& event);
    void OnEditorEvent(CodeBlocksEvent& event);
    void OnToggleHighlight(wxCommandEvent& event);
    void OnToggleSelectedOnly(wxCommandEvent& event);
    void OnToggleMatchCase(wxCommandEvent& event);
    void OnToggleUseRegex(wxCommandEvent& event);
    void DoClearText();
    void DoSearchPrev();
    void DoSearchNext();
    void DoToggleHighlight(bool checked);
    void DoToggleSelectedOnly(bool checked);
    void DoToggleMatchCase(bool checked);
    void DoToggleUseRegex(bool checked);
    void DoFocusToolbar();
    void HighlightText();
    void SearchText();
    void DoSearch(int fromPos, int startPos=wxSCI_INVALID_POSITION, int endPos=wxSCI_INVALID_POSITION);
    void VerifyPosition();
    void SetRange();
    wxString m_SearchText;
    wxColour m_textCtrlBG_Default;
    wxToolBar* m_pToolbar;
    wxTextCtrl* m_pTextCtrl;
    cbEditor* m_pEditor;
    int m_NewPos;
    int m_OldPos;
    int m_SelStart;
    int m_SelEnd;
    int m_MinPos;
    int m_MaxPos;
    int m_flags;
    bool m_Highlight;
    bool m_SelectedOnly;
    const int m_IndicFound;
    const int m_IndicHighlight;
    int m_LengthFound;
    long m_LastInsertionPoint;

    DECLARE_EVENT_TABLE()
};

#endif // INCREMENTAL_SEARCH_H_INCLUDED
