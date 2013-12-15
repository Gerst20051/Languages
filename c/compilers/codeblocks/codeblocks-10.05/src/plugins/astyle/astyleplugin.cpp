/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision: 6089 $
 * $Id: astyleplugin.cpp 6089 2010-01-17 02:40:31Z biplab $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/astyle/astyleplugin.cpp $
 */

#include <sdk.h>

#include "astyleplugin.h"
#include <cbexception.h>
#include "astyleconfigdlg.h"
#include <sstream>
#include <string>
#include <vector>
#include "formattersettings.h"
#include <manager.h>
#include <editormanager.h>
#include <configmanager.h>
#include <cbeditor.h>
#include <wx/msgdlg.h>
#include <wx/xrc/xmlres.h>
#include <wx/fs_zip.h>
#include <wx/strconv.h>
#include <wx/progdlg.h>
#include "asstreamiterator.h"
#include "cbstyledtextctrl.h"
#include "cbproject.h"
#include "projectmanager.h"

using std::istringstream;
using std::string;

namespace
{
    const int idCodeFormatterFile = wxNewId();
    const int idCodeFormatterActiveFile = wxNewId();
    const int idCodeFormatterProject = wxNewId();
}

BEGIN_EVENT_TABLE( AStylePlugin, cbPlugin )
    EVT_MENU( idCodeFormatterActiveFile, AStylePlugin::OnFormatActiveFile )
    EVT_MENU( idCodeFormatterProject, AStylePlugin::OnFormatProject )
END_EVENT_TABLE()

// this auto-registers the plugin

namespace
{
    PluginRegistrant<AStylePlugin> reg(_T("AStylePlugin"));
}

AStylePlugin::AStylePlugin()
{
    //ctor

    if (!Manager::LoadResource(_T("astyle.zip")))
    {
        NotifyMissingFile(_T("astyle.zip"));
    }
}

AStylePlugin::~AStylePlugin()
{
    //dtor
}

void AStylePlugin::OnAttach()
{}

void AStylePlugin::OnRelease(bool appShutDown)
{
    // do de-initialization for your plugin
    // NOTE: after this function, the inherited member variable
    // m_IsAttached will be FALSE...
}

int AStylePlugin::Configure()
{
//  AstyleConfigDlg dlg(Manager::Get()->GetAppWindow());
//  dlg.ShowModal();

    return 0;
}

cbConfigurationPanel* AStylePlugin::GetConfigurationPanel(wxWindow* parent)
{
    AstyleConfigDlg* dlg = new AstyleConfigDlg(parent);
    // deleted by the caller

    return dlg;
}

void AStylePlugin::BuildModuleMenu( const ModuleType type, wxMenu* menu, const FileTreeData* data )
{
    if ( !menu || !IsAttached() )
        return;

    switch ( type )
    {
        case mtEditorManager:
            menu->AppendSeparator();
            menu->Append( idCodeFormatterActiveFile, _( "Format this file (AStyle)" ), _( "Format the source code in the current file" ) );
            break;

        case mtProjectManager:
            if ( data ) switch ( data->GetKind() )
                {
                    case FileTreeData::ftdkProject:
                        menu->AppendSeparator();
                        menu->Append( idCodeFormatterProject, _( "Format this project (AStyle)" ), _( "Format the source code in this project" ) );
                        break;

                    case FileTreeData::ftdkFile:
                        menu->AppendSeparator();
                        menu->Append( idCodeFormatterProject, _( "Format this file (AStyle)" ), _( "Format the source code in this file" ) );
                        break;

                    default:
                        // Do nothing.
                        break;
                }
            break;

        default:
            break;
    }
}

void AStylePlugin::OnFormatProject( wxCommandEvent& event )
{
    wxTreeCtrl *tree = Manager::Get()->GetProjectManager()->GetTree();

    if ( !tree )
        return;

    wxTreeItemId treeItem =  tree->GetSelection();

    if ( !treeItem.IsOk() )
        return;

    const FileTreeData *data = static_cast<FileTreeData*>( tree->GetItemData( treeItem ) );

    if ( !data )
        return;

    switch ( data->GetKind() )
    {
        case FileTreeData::ftdkProject:
            {
                cbProject* prj = data->GetProject();
                wxProgressDialog progressDlg(_("Please wait"), _("Formatting..."), prj->GetFilesCount(), 0, wxPD_CAN_ABORT|wxPD_AUTO_HIDE|wxPD_SMOOTH );
                progressDlg.Show();
                for ( int i = 0; i < prj->GetFilesCount(); ++i )
                {
                    ProjectFile* pf = prj->GetFile( i );
                    wxString filename = pf->file.GetFullPath();

                    FileType fileType = FileTypeOf( filename );
                    if ( fileType == ftSource || fileType == ftHeader )
                    {
                        FormatFile( filename );
                        if ( false == progressDlg.Update( i, wxString(_("Formatting ")) + pf->relativeFilename ) )
                            break;
                    }
                }
            }
            break;

        case FileTreeData::ftdkFile:
            {
                ProjectFile* f = data->GetProject()->GetFile( data->GetFileIndex() );
                if ( f )
                    FormatFile( f->file.GetFullPath() );
            }
            break;

        default:
            break;
    }
}

void AStylePlugin::OnFormatActiveFile( wxCommandEvent& event )
{
        Execute();
}

int AStylePlugin::Execute()
{
    if (!IsAttached())
    {
        return -1;
    }

    cbEditor *ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();

    if (!ed)
    {
        return 0;
    }

    FormatEditor( ed );

    return 0;
}

void AStylePlugin::FormatFile( const wxString &filename )
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->IsBuiltinOpen( filename );

    if ( ed )
    {
        // File is already open
        FormatEditor( ed );
    }
    else
    {
        // File is not open.  We must open it.
        ed = Manager::Get()->GetEditorManager()->Open( filename );

        if ( ed )
        {
            bool changed = FormatEditor( ed );

            if ( !changed )
            {
                // We opened a file and it didn't change.  Close it.
                Manager::Get()->GetEditorManager()->Close( filename );
            }
        }
    }
}

// Special code to compare strings which doesn't care
// about spaces leading up to the EOL.
static bool BuffersDiffer( const wxString &a, const wxString &b )
{
    const wxChar *aCurrent = a.c_str();
    const wxChar *bCurrent = b.c_str();
    const wxChar * const aEnd = aCurrent + a.Len();
    const wxChar * const bEnd = bCurrent + b.Len();

    while ( aCurrent != aEnd && bCurrent != bEnd )
    {
        if ( *aCurrent != *bCurrent )
        {
            // Check for varying space at EOL
            while ( *aCurrent == ' ' || *aCurrent == '\t' )
            {
                if ( ++aCurrent == aEnd )
                    break;
            }
            while ( *bCurrent == ' ' || *bCurrent == '\t' )
            {
                if ( ++bCurrent == bEnd )
                    break;
            }

            // Make sure it was at EOL
            if ( ( *aCurrent != '\r' && *aCurrent != '\n' ) || ( *bCurrent != '\r' && *bCurrent != '\n' ) )
                return true;
        }

        ++aCurrent;
        ++bCurrent;
    }

    while ( aCurrent != aEnd )
    {
        if ( *aCurrent != ' ' && *aCurrent != '\t' )
        {
            return true;
        }
        ++aCurrent;
    }

    while ( bCurrent != bEnd )
    {
        if ( *bCurrent != ' ' && *bCurrent != '\t' )
        {
            return true;
        }
        ++bCurrent;
    }

    return false;
}

bool AStylePlugin::FormatEditor( cbEditor *ed )
{
    if (ed->GetControl()->GetReadOnly())
    {
        cbMessageBox(_("The file is read-only!"), _("Error"), wxICON_ERROR);
        return false;
    }

    wxString edText(ed->GetControl()->GetText());

    wxString formattedText;

    astyle::ASFormatter formatter;

    // load settings
    FormatterSettings settings;
    settings.ApplyTo(formatter);

    wxString eolChars;

    switch (ed->GetControl()->GetEOLMode())
    {

        case wxSCI_EOL_CRLF:
            eolChars = _T("\r\n");
            break;

        case wxSCI_EOL_CR:
            eolChars = _T("\r");
            break;

        case wxSCI_EOL_LF:
            eolChars = _T("\n");
            break;
    }

    if (edText.size() && edText.Last() != _T('\r') && edText.Last() != _T('\n'))
    {
        edText += eolChars;
    }

    ASStreamIterator *asi = new ASStreamIterator(ed, edText);

    formatter.init(asi);

    int lineCounter = 0;
    std::vector<int> new_bookmark;
    std::vector<int> ed_breakpoints;

    // hack: we need to evaluate the special case of having a bookmark in the first line here

    if (ed->HasBookmark(0))
    {
        new_bookmark.push_back(0);
    }
    if (ed->HasBreakpoint(0))
    {
        ed_breakpoints.push_back(0);
    }

    wxSetCursor(*wxHOURGLASS_CURSOR);

    while (formatter.hasMoreLines())
    {
        formattedText << cbC2U(formatter.nextLine().c_str());

        if (formatter.hasMoreLines())
        {
            formattedText << eolChars;
        }

        ++lineCounter;

        if (asi->FoundBookmark())
        {
            new_bookmark.push_back(lineCounter);
            asi->ClearFoundBookmark();
        }
        if (asi->FoundBreakpoint())
        {
            ed_breakpoints.push_back(lineCounter);
            asi->ClearFoundBreakpoint();
        }
    }

    int pos = ed->GetControl()->GetCurrentPos();

    bool changed = BuffersDiffer( formattedText, edText );

    if ( changed )
    {
        ed->GetControl()->BeginUndoAction();
        ed->GetControl()->SetText(formattedText);

        for (std::vector<int>::const_iterator i = new_bookmark.begin(); i != new_bookmark.end(); ++i)
        {
            ed->ToggleBookmark(*i);
        }

        for (std::vector<int>::const_iterator i = ed_breakpoints.begin(); i != ed_breakpoints.end(); ++i)
        {
            ed->ToggleBreakpoint(*i);
        }

        ed->GetControl()->EndUndoAction();
        ed->GetControl()->GotoPos(pos);
        ed->SetModified(true);
    }

    wxSetCursor(wxNullCursor);

    return changed;
}
