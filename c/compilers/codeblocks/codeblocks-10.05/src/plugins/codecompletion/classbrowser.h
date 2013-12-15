/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef CLASSBROWSER_H
#define CLASSBROWSER_H

#include <settings.h> // SDK
#include <cbplugin.h>
#include <manager.h>
#include <wx/panel.h>
#include "parser/parser.h"
#include "parser/token.h"
#include "classbrowserbuilderthread.h"

class NativeParser;
class wxComboBox;
class wxTreeCtrl;
class wxTextCtrl;
class cbProject;

/*
 * No description
 */
class ClassBrowser : public wxPanel
{
    public:
        // class constructor
        ClassBrowser(wxWindow* parent, NativeParser* np);
        // class destructor
        ~ClassBrowser();
        const wxTreeCtrl* GetTree() { return m_Tree; }
        void SetParser(Parser* parser);
        const Parser& GetParser(){ return *m_pParser; }
        const Parser* GetParserPtr() { return m_pParser; }
        void UnlinkParser();
        void UpdateView(bool checkHeaderSwap = false);

        void OnSize(wxSizeEvent& event);
    private:
        friend class myTextCtrl;
        void OnTreeItemDoubleClick(wxTreeEvent& event);
        void OnTreeItemRightClick(wxTreeEvent& event);
        void OnJumpTo(wxCommandEvent& event);
        void OnRefreshTree(wxCommandEvent& event);
        void OnForceReparse(wxCommandEvent& event);
        void OnCBViewMode(wxCommandEvent& event);
        void OnCBExpandNS(wxCommandEvent& event);
        void OnViewScope(wxCommandEvent& event);
        void OnDebugSmartSense(wxCommandEvent& event);
        void OnSetSortType(wxCommandEvent& event);

        void OnSearch(wxCommandEvent& event);
        bool FoundMatch(const wxString& search, wxTreeCtrl* tree, const wxTreeItemId& item);
        wxTreeItemId FindNext(const wxString& search, wxTreeCtrl* tree, const wxTreeItemId& start);
        wxTreeItemId FindChild(const wxString& search, wxTreeCtrl* tree, const wxTreeItemId& start, bool recurse = false, bool partialMatch = false);
        bool RecursiveSearch(const wxString& search, wxTreeCtrl* tree, const wxTreeItemId& parent, wxTreeItemId& result);

        void ShowMenu(wxTreeCtrl* tree, wxTreeItemId id, const wxPoint& pt);

        void BuildTree();

        void OnTreeItemSelected(wxTreeEvent& event);
        void OnTreeItemExpanding(wxTreeEvent& event);
        void OnTreeItemCollapsing(wxTreeEvent& event);

        NativeParser*   m_NativeParser;
        CBTreeCtrl*     m_Tree;
        CBTreeCtrl*     m_TreeBottom;
        wxComboBox*     m_Search;
        wxTreeCtrl*     m_TreeForPopupMenu;
        Parser*         m_pParser;
        wxTreeItemId    m_RootNode;

        // filtering
        wxString m_ActiveFilename;
        cbProject* m_pActiveProject;

        wxSemaphore m_Semaphore;
        ClassBrowserBuilderThread* m_pBuilderThread;

        DECLARE_EVENT_TABLE()
};

#endif // CLASSBROWSER_H

