/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision: 5033 $
 * $Id: cbeditor.cpp 5033 2008-05-07 11:41:24Z mandrav $
 * $HeadURL: https://svn.berlios.de/svnroot/repos/codeblocks/trunk/src/sdk/cbeditor.cpp $
 */

// Stop following warning:
//warning: .../trunk/src/include/sdk_precomp.h.gch: not used because `EXPORT_LIB' not defined|
#undef CB_PRECOMP

#ifdef CB_PRECOMP
#include "sdk_precomp.h"
#endif

#ifndef CB_PRECOMP
    #include <wx/filename.h>
    #include <wx/notebook.h>
    #include <wx/wfstream.h>

    #include "scbeditor.h" // class's header file
    #include "globals.h"
    #include "sdk_events.h"
    #include "cbproject.h"
    #include "projectfile.h"
    #include "projectbuildtarget.h"
    #include "seditorcolourset.h"
    #include "manager.h"
    #include "configmanager.h"
    #include "projectmanager.h"
    #include "pluginmanager.h"
    #include "seditormanager.h"
    #include "logmanager.h"
    #include "macrosmanager.h" // ReplaceMacros
    #include "cbplugin.h"
#endif
#include "snippetsconfig.h"
#include "seditormanager.h"
#include "scbeditor.h"
#include "seditorcolourset.h"
#include "cbstyledtextctrl.h"

#include <wx/fontutil.h>
#include <wx/splitter.h>

#include "cbeditorprintout.h"
#include "editor_hooks.h"
#include "filefilters.h"
#include "encodingdetector.h"
#include "projectfileoptionsdlg.h"

const wxString g_EditorModified = _T("*");

#define ERROR_MARKER          1
#define ERROR_STYLE           wxSCI_MARK_SMALLRECT

#define BOOKMARK_MARKER       2
#define BOOKMARK_STYLE        wxSCI_MARK_ARROW

#define BREAKPOINT_MARKER     3
#define BREAKPOINT_STYLE      wxSCI_MARK_CIRCLE

#define DEBUG_MARKER          4
#define DEBUG_STYLE           wxSCI_MARK_ARROW



/* This struct holds private data for the ScbEditor class.
 * It's a paradigm to avoid rebuilding the entire project (as ScbEditor is a basic dependency)
 * for just adding a private var or method.
 * What happens is that we 've declared a cbEditorInternalData* private member in ScbEditor
 * and define it in the .cpp file (here). Effectively, this means that we can now add/remove
 * elements from cbEditorInternalData without needing to rebuild the project :)
 * The ScbEditor::m_pData is the variable to use in code. It's the very first thing
 * constructed and the very last destructed.
 *
 * So, if we want to add a new private member in ScbEditor, we add it here instead
 * and access it with m_pData->
 * e.g. m_pData->lastPosForCodeCompletion
 * and of course you can add member functions here ;)
 *
 * cbEditorInternalData also contains a pointer to the owner ScbEditor (named m_pOwner).
 * Using m_pOwner the struct's member functions can access ScbEditor functions
 * (even private ones - it's a friend).
 *
 * The same logic should be used all around the project's classes, gradually.
 */
struct ScbEditorInternalData
{
    ScbEditor* m_pOwner;

    ScbEditorInternalData(ScbEditor* owner, LoaderBase* fileLoader = 0)
        : m_pOwner(owner),
        m_strip_trailing_spaces(true),
        m_ensure_final_line_end(false),
        m_ensure_consistent_line_ends(true),
        m_LastMarginMenuLine(-1),
        m_LastDebugLine(-1),
        m_useByteOrderMark(false),
        m_byteOrderMarkLength(0),
        m_lineNumbersWidth(0),
        m_pFileLoader(fileLoader)
    {
        m_encoding = wxLocale::GetSystemEncoding();

        if (m_pFileLoader)
        {
            EncodingDetector enc(fileLoader);
            if (enc.IsOK())
            {
                m_byteOrderMarkLength = enc.GetBOMSizeInBytes();
                m_useByteOrderMark = enc.UsesBOM();
                m_encoding = enc.GetFontEncoding();
            }
        }
    }

    ~ScbEditorInternalData()
    {
        if (m_pFileLoader)
        {
            delete m_pFileLoader;
            m_pFileLoader = 0;
        }
    }

    // add member vars/funcs below
    wxString GetEOLString() const
    {
        wxString eolstring;
        cbStyledTextCtrl* control = m_pOwner->GetControl();
        switch (control->GetEOLMode())
        {
            case wxSCI_EOL_LF:
                eolstring = _T("\n");
                break;
            case wxSCI_EOL_CR:
                eolstring = _T("\r");
                break;
            default:
                eolstring = _T("\r\n");
        }
        return eolstring;
    }
    // funcs
    /** Get the last non-whitespace character before position */
    wxChar GetLastNonWhitespaceChar(int position = -1)
    {
        cbStyledTextCtrl* control = m_pOwner->GetControl();
        if (position == -1)
            position = control->GetCurrentPos();

        int count = 0; // Used to count the number of blank lines
        bool foundlf = false; // For the rare case of CR's without LF's
        while (position)
        {
            wxChar c = control->GetCharAt(--position);
            int style = control->GetStyleAt(position);
            bool inComment = style == wxSCI_C_COMMENT ||
                            style == wxSCI_C_COMMENTDOC ||
                            style == wxSCI_C_COMMENTDOCKEYWORD ||
                            style == wxSCI_C_COMMENTDOCKEYWORDERROR ||
                            style == wxSCI_C_COMMENTLINE ||
                            style == wxSCI_C_COMMENTLINEDOC;
            if (c == _T('\n'))
            {
                count++;
                foundlf = true;
            }
            else if (c == _T('\r') && !foundlf)
                count++;
            else
                foundlf = false;
            if (count > 1) return 0; // Don't over-indent
            if (!inComment && c != _T(' ') && c != _T('\t') && c != _T('\n') && c != _T('\r'))
                return c;
        }
        return 0;
    }

    int FindBlockStart(int position, wxChar blockStart, wxChar blockEnd, bool skipNested = true)
    {
        cbStyledTextCtrl* control = m_pOwner->GetControl();
        int lvl = 0;
        wxChar b = control->GetCharAt(position);
        while (b)
        {
            if (b == blockEnd)
                ++lvl;
            else if (b == blockStart)
            {
                if (lvl == 0)
                    return position;
                --lvl;
            }
            --position;
            b = control->GetCharAt(position);
        }
        return -1;
    }

    /** Strip Trailing Blanks before saving */
    void StripTrailingSpaces()
    {
        cbStyledTextCtrl* control = m_pOwner->GetControl();
        // The following code was adapted from the SciTE sourcecode

        int maxLines = control->GetLineCount();
        for (int line = 0; line < maxLines; line++)
        {
            int lineStart = control->PositionFromLine(line);
            int lineEnd = control->GetLineEndPosition(line);
            int i = lineEnd-1;
            wxChar ch = (wxChar)(control->GetCharAt(i));
            while ((i >= lineStart) && ((ch == _T(' ')) || (ch == _T('\t'))))
            {
                i--;
                ch = (wxChar)(control->GetCharAt(i));
            }
            if (i < (lineEnd-1))
            {
                control->SetTargetStart(i+1);
                control->SetTargetEnd(lineEnd);
                control->ReplaceTarget(_T(""));
            }
        }
    }

    /** Add extra blank line to the file */
    void EnsureFinalLineEnd()
    {
        cbStyledTextCtrl* control = m_pOwner->GetControl();
        // The following code was adapted from the SciTE sourcecode
        int maxLines = control->GetLineCount();
        int enddoc = control->PositionFromLine(maxLines);
        if(maxLines <= 1 || enddoc > control->PositionFromLine(maxLines-1))
            control->InsertText(enddoc,GetEOLString());
    }

    /** Make sure all the lines end with the same EOL mode */
    void EnsureConsistentLineEnds()
    {
        cbStyledTextCtrl* control = m_pOwner->GetControl();
        // The following code was adapted from the SciTE sourcecode
        control->ConvertEOLs(control->GetEOLMode());
    }

    /** Set line number column width */
    void SetLineNumberColWidth()
    {
        ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("editor"));

        int pixelWidth = m_pOwner->m_pControl->TextWidth(wxSCI_STYLE_LINENUMBER, _T("9"));

        if(cfg->ReadBool(_T("/margin/dynamic_width"), false))
        {
            int lineNumWidth = 1;
            int lineCount = m_pOwner->m_pControl->GetLineCount();

            while (lineCount >= 10)
            {
                lineCount /= 10;
                ++lineNumWidth;
            }

            if (lineNumWidth != m_lineNumbersWidth)
            {
                m_pOwner->m_pControl->SetMarginWidth(0, 6 + lineNumWidth * pixelWidth);
                if (m_pOwner->m_pControl2)
                    m_pOwner->m_pControl2->SetMarginWidth(0, 6 + lineNumWidth * pixelWidth);
                m_lineNumbersWidth = lineNumWidth;
            }
        }
        else
        {
            m_pOwner->m_pControl->SetMarginWidth(0, 6 + cfg->ReadInt(_T("/margin/width_chars"), 6) * pixelWidth);
            if (m_pOwner->m_pControl2)
                m_pOwner->m_pControl2->SetMarginWidth(0, 6 + cfg->ReadInt(_T("/margin/width_chars"), 6) * pixelWidth);
        }
    }

    //vars
    bool m_strip_trailing_spaces;
    bool m_ensure_final_line_end;
    bool m_ensure_consistent_line_ends;

    int m_LastMarginMenuLine;
    int m_LastDebugLine;

    bool mFoldingLimit;
    int mFoldingLimitLevel;

    wxFontEncoding m_encoding;
    bool m_useByteOrderMark;
    int m_byteOrderMarkLength;

    int m_lineNumbersWidth;

    LoaderBase* m_pFileLoader;

};
////////////////////////////////////////////////////////////////////////////////

const int idEmptyMenu = wxNewId();
const int idEdit = wxNewId();
const int idUndo = wxNewId();
const int idRedo = wxNewId();
const int idCut = wxNewId();
const int idCopy = wxNewId();
const int idPaste = wxNewId();
const int idDelete = wxNewId();
const int idUpperCase = wxNewId();
const int idLowerCase = wxNewId();
const int idSelectAll = wxNewId();
const int idSwapHeaderSource = wxNewId();
const int idBookmarks = wxNewId();
const int idBookmarksToggle = wxNewId();
const int idBookmarksPrevious = wxNewId();
const int idBookmarksNext = wxNewId();
const int idFolding = wxNewId();
const int idFoldingFoldAll = wxNewId();
const int idFoldingUnfoldAll = wxNewId();
const int idFoldingToggleAll = wxNewId();
const int idFoldingFoldCurrent = wxNewId();
const int idFoldingUnfoldCurrent = wxNewId();
const int idFoldingToggleCurrent = wxNewId();
const int idInsert = wxNewId();
const int idSplit = wxNewId();
const int idSplitHorz = wxNewId();
const int idSplitVert = wxNewId();
const int idUnsplit = wxNewId();
const int idConfigureEditor = wxNewId();
const int idProperties = wxNewId();

const int idBookmarkAdd = wxNewId();
const int idBookmarkRemove = wxNewId();

const int idBreakpointAdd = wxNewId();
const int idBreakpointEdit = wxNewId();
const int idBreakpointRemove = wxNewId();

BEGIN_EVENT_TABLE(ScbEditor, SEditorBase)
    EVT_CLOSE(ScbEditor::OnClose)
    // we got dynamic events; look in CreateEditor()

    EVT_MENU(idUndo, ScbEditor::OnContextMenuEntry)
    EVT_MENU(idRedo, ScbEditor::OnContextMenuEntry)
    EVT_MENU(idCut, ScbEditor::OnContextMenuEntry)
    EVT_MENU(idCopy, ScbEditor::OnContextMenuEntry)
    EVT_MENU(idPaste, ScbEditor::OnContextMenuEntry)
    EVT_MENU(idDelete, ScbEditor::OnContextMenuEntry)
    EVT_MENU(idUpperCase, ScbEditor::OnContextMenuEntry)
    EVT_MENU(idLowerCase, ScbEditor::OnContextMenuEntry)
    EVT_MENU(idSelectAll, ScbEditor::OnContextMenuEntry)
    EVT_MENU(idSwapHeaderSource, ScbEditor::OnContextMenuEntry)
    EVT_MENU(idBookmarksToggle, ScbEditor::OnContextMenuEntry)
    EVT_MENU(idBookmarksPrevious, ScbEditor::OnContextMenuEntry)
    EVT_MENU(idBookmarksNext, ScbEditor::OnContextMenuEntry)
    EVT_MENU(idFoldingFoldAll, ScbEditor::OnContextMenuEntry)
    EVT_MENU(idFoldingUnfoldAll, ScbEditor::OnContextMenuEntry)
    EVT_MENU(idFoldingToggleAll, ScbEditor::OnContextMenuEntry)
    EVT_MENU(idFoldingFoldCurrent, ScbEditor::OnContextMenuEntry)
    EVT_MENU(idFoldingUnfoldCurrent, ScbEditor::OnContextMenuEntry)
    EVT_MENU(idFoldingToggleCurrent, ScbEditor::OnContextMenuEntry)
    EVT_MENU(idConfigureEditor, ScbEditor::OnContextMenuEntry)
    EVT_MENU(idProperties, ScbEditor::OnContextMenuEntry)
    EVT_MENU(idBookmarkAdd, ScbEditor::OnContextMenuEntry)
    EVT_MENU(idBookmarkRemove, ScbEditor::OnContextMenuEntry)
    EVT_MENU(idBreakpointAdd, ScbEditor::OnContextMenuEntry)
    EVT_MENU(idBreakpointEdit, ScbEditor::OnContextMenuEntry)
    EVT_MENU(idBreakpointRemove, ScbEditor::OnContextMenuEntry)
    EVT_MENU(idSplitHorz, ScbEditor::OnContextMenuEntry)
    EVT_MENU(idSplitVert, ScbEditor::OnContextMenuEntry)
    EVT_MENU(idUnsplit, ScbEditor::OnContextMenuEntry)

    EVT_SCI_ZOOM(-1, ScbEditor::OnZoom)
    EVT_SCI_ZOOM(-1, ScbEditor::OnZoom)

END_EVENT_TABLE()

// class constructor
// ----------------------------------------------------------------------------
ScbEditor::ScbEditor(wxWindow* parent, const wxString& filename, SEditorColourSet* theme)
// ----------------------------------------------------------------------------
    : SEditorBase(parent, filename),
    m_pSplitter(0),
    m_pSizer(0),
    m_pControl(0),
    m_pControl2(0),
    m_foldBackup(0),
    m_SplitType(stNoSplit),
    m_Modified(false),
    m_Index(-1),
    m_pProjectFile(0L),
    m_pTheme(theme),
    m_lang(HL_AUTO)
{
    DoInitializations(filename);
}

// class constructor
// ----------------------------------------------------------------------------
ScbEditor::ScbEditor(wxWindow* parent, LoaderBase* fileLdr, const wxString& filename, SEditorColourSet* theme)
// ----------------------------------------------------------------------------
    : SEditorBase(parent, filename),
    m_pSplitter(0),
    m_pSizer(0),
    m_pControl(0),
    m_pControl2(0),
    m_foldBackup(0),
    m_SplitType(stNoSplit),
    m_Modified(false),
    m_Index(-1),
    m_pProjectFile(0L),
    m_pTheme(theme),
    m_lang(HL_AUTO)
{
    DoInitializations(filename, fileLdr);
}

// class destructor
// ----------------------------------------------------------------------------
ScbEditor::~ScbEditor()
// ----------------------------------------------------------------------------
{
    SetSizer(0);

    // moved in ~SEditorBase
//    NotifyPlugins(cbEVT_EDITOR_CLOSE, 0, m_Filename);

    UpdateProjectFile();
    if (m_pControl)
    {
        if (m_pProjectFile)
            m_pProjectFile->editorOpen = false;
        m_pControl->Destroy();
        m_pControl = 0;
    }
    DestroySplitView();

    delete m_pData;
}

// ----------------------------------------------------------------------------
void ScbEditor::DoInitializations(const wxString& filename, LoaderBase* fileLdr)
// ----------------------------------------------------------------------------
{
    // first thing to do!
    // if we add more constructors in the future, don't forget to set this!
    m_pData = new ScbEditorInternalData(this);//, fileLdr);

    m_pData->m_pFileLoader = fileLdr;
    m_IsBuiltinEditor = true;

    if (!filename.IsEmpty())
    {
        InitFilename(filename);
        wxFileName fname(m_Filename);
        NormalizePath(fname, wxEmptyString);
        m_Filename = fname.GetFullPath();
    }
    else
    {
        static int untitledCounter = 1;
        wxString f;
        cbProject* prj = Manager::Get()->GetProjectManager()->GetActiveProject();
        if (prj)
            f.Printf(_("%sUntitled%d"), prj->GetBasePath().c_str(), untitledCounter++);
        else
            f.Printf(_("Untitled%d"), untitledCounter++);

        InitFilename(f);
    }
//    Manager::Get()->GetLogManager()->DebugLog(_T("ctor: Filename=%s\nShort=%s"), m_Filename.c_str(), m_Shortname.c_str());

    // initialize left control (unsplit state)
    Freeze();
    m_pSizer = new wxBoxSizer(wxVERTICAL);
    m_pControl = CreateEditor();
    m_pSizer->Add(m_pControl, 1, wxEXPAND);
    SetSizer(m_pSizer);

    // the following two lines make the editors behave strangely in linux:
    // when resizing other docked windows, the editors do NOT resize too
    // and they stame the same size...
    // if commenting the following two lines causes problems in other platforms,
    // simply put an "#ifdef __WXGTK__" guard around and uncomment them.
//    m_pSizer->Fit(this);
//    m_pSizer->SetSizeHints(this);

    Thaw();
    m_pControl->SetZoom(GetEditorManager()->GetZoom());
    m_pSizer->SetItemMinSize(m_pControl, 32, 32);

    SetEditorStyleBeforeFileOpen();
    m_IsOK = Open();
    SetEditorStyleAfterFileOpen();

    // if !m_IsOK then it's a new file, so set the modified flag ON
    if (!m_IsOK || filename.IsEmpty())
    {
        SetModified(true);
        m_IsOK = false;
    }
}

// ----------------------------------------------------------------------------
void ScbEditor::NotifyPlugins(wxEventType type, int intArg, const wxString& strArg, int xArg, int yArg)
// ----------------------------------------------------------------------------
{
    //(pecan 2008/4/22) Dont notify plugins from non-sdk
    //-if (!Manager::Get()->GetPluginManager())
    //-    return; // no plugin manager! app shuting down?
    wxWindow* parent = GetEditorParent();
    if (not parent) return;

    CodeBlocksEvent event(type);
    event.SetEditor((EditorBase*)this);
    event.SetInt(intArg);
    event.SetString(strArg);
    event.SetX(xArg);
    event.SetY(yArg);
    //-wxPostEvent(Manager::Get()->GetAppWindow(), event);
    //-Manager::Get()->GetPluginManager()->NotifyPlugins(event);
    #if wxCHECK_VERSION(2, 9, 0)
    parent->GetEventHandler()->ProcessEvent(event);
    #else
    parent->ProcessEvent(event);
    #endif
}

void ScbEditor::DestroySplitView()
{
    if (m_pControl2)
    {
        m_pControl2->Destroy();
        m_pControl2 = 0;
    }
    if (m_pSplitter)
    {
        m_pSplitter->Destroy();
        m_pSplitter = 0;
    }
}

cbStyledTextCtrl* ScbEditor::GetControl() const
{
    // return the focused control (left or right)
    if (m_pControl2)
    {
        wxWindow* focused = wxWindow::FindFocus();
        if (focused == m_pControl2)
            return m_pControl2;
    }
    return m_pControl;
}

bool ScbEditor::GetModified() const
{
    return m_Modified || m_pControl->GetModify();
}

void ScbEditor::SetModified(bool modified)
{
    if (modified != m_Modified)
    {
        m_Modified = modified;
        if (!m_Modified)
        {
            m_pControl->SetSavePoint();
        }
        SetEditorTitle(m_Shortname);
        NotifyPlugins(cbEVT_EDITOR_MODIFIED);
        // visual state
        if (m_pProjectFile)
            m_pProjectFile->SetFileState(m_pControl->GetReadOnly() ? fvsReadOnly : (m_Modified ? fvsModified : fvsNormal));
    }
}

void ScbEditor::SetEditorTitle(const wxString& title)
{
    if(m_Modified)
    {
        SetTitle(g_EditorModified + title);
    }
    else
    {
        SetTitle(title);
    }
}

void ScbEditor::SetProjectFile(ProjectFile* project_file, bool preserve_modified)
{
    if (m_pProjectFile == project_file)
        return; // we 've been here before ;)

    bool wasmodified = false;
    if(preserve_modified)
        wasmodified = GetModified();

    m_pProjectFile = project_file;
    if (m_pProjectFile)
    {
        // update our filename
        m_Filename = UnixFilename(project_file->file.GetFullPath());

        m_pControl->GotoPos(m_pProjectFile->editorPos);
        m_pControl->ScrollToLine(m_pProjectFile->editorTopLine);
        m_pControl->ScrollToColumn(0);

        m_pProjectFile->editorOpen = true;

        if (Manager::Get()->GetConfigManager(_T("editor"))->ReadBool(_T("/tab_text_relative"), true))
            m_Shortname = m_pProjectFile->relativeToCommonTopLevelPath;
        else
            m_Shortname = m_pProjectFile->file.GetFullName();
        SetEditorTitle(m_Shortname);
    }

    if (!wxFileExists(m_Filename))
        m_pProjectFile->SetFileState(fvsMissing);
    else if (!wxFile::Access(m_Filename.c_str(), wxFile::write)) // readonly
        m_pProjectFile->SetFileState(fvsReadOnly);

#if 0
    wxString dbg;
    dbg << _T("[ed] Filename: ") << GetFilename() << _T('\n');
    dbg << _T("[ed] Short name: ") << GetShortName() << _T('\n');
    dbg << _T("[ed] Modified: ") << GetModified() << _T('\n');
    dbg << _T("[ed] Project: ") << ((m_pProjectFile && m_pProjectFile->project) ? m_pProjectFile->project->GetTitle() : _T("unknown")) << _T('\n');
    dbg << _T("[ed] Project file: ") << (m_pProjectFile ? m_pProjectFile->relativeFilename : _T("unknown")) << _T('\n');
    Manager::Get()->GetLogManager()->DebugLog(dbg);
#endif
    if(preserve_modified)
        SetModified(wasmodified);
}

void ScbEditor::UpdateProjectFile()
{
    if (m_pControl && m_pProjectFile)
    {
        m_pProjectFile->editorPos = m_pControl->GetCurrentPos();
        m_pProjectFile->editorTopLine = m_pControl->GetFirstVisibleLine();
        m_pProjectFile->editorOpen = true;
    }
}

void ScbEditor::SetMarkerStyle(int marker, int markerType, wxColor fore, wxColor back)
{
    m_pControl->MarkerDefine(marker, markerType);
    m_pControl->MarkerSetForeground(marker, fore);
    m_pControl->MarkerSetBackground(marker, back);

    if (m_pControl2)
    {
        m_pControl2->MarkerDefine(marker, markerType);
        m_pControl2->MarkerSetForeground(marker, fore);
        m_pControl2->MarkerSetBackground(marker, back);
    }
}

void ScbEditor::UnderlineFoldedLines(bool underline)
{
    m_pControl->SetFoldFlags(underline ? 16 : 0);
    if (m_pControl2)
        m_pControl2->SetFoldFlags(underline ? 16 : 0);
}

cbStyledTextCtrl* ScbEditor::CreateEditor()
{
    m_ID = wxNewId();

    // avoid gtk-critical because of sizes less than -1 (can happen with wxAuiNotebook)
    wxSize size = m_pControl ? wxDefaultSize : GetSize();
    size.x = std::max(size.x, -1);
    size.y = std::max(size.y, -1);

    cbStyledTextCtrl* control = new cbStyledTextCtrl(this, m_ID, wxDefaultPosition, size);
    control->UsePopUp(false);

    wxString enc_name = Manager::Get()->GetConfigManager(_T("editor"))->Read(_T("/default_encoding"), wxEmptyString);
    m_pData->m_encoding = wxFontMapper::GetEncodingFromName(enc_name);

    // dynamic events
    Connect( m_ID,  -1, wxEVT_SCI_MARGINCLICK,
                  (wxObjectEventFunction) (wxEventFunction) (wxScintillaEventFunction)
                  &ScbEditor::OnMarginClick );
    Connect( m_ID,  -1, wxEVT_SCI_UPDATEUI,
                  (wxObjectEventFunction) (wxEventFunction) (wxScintillaEventFunction)
                  &ScbEditor::OnEditorUpdateUI );
    Connect( m_ID,  -1, wxEVT_SCI_CHANGE,
                  (wxObjectEventFunction) (wxEventFunction) (wxScintillaEventFunction)
                  &ScbEditor::OnEditorChange );
    Connect( m_ID,  -1, wxEVT_SCI_CHARADDED,
                  (wxObjectEventFunction) (wxEventFunction) (wxScintillaEventFunction)
                  &ScbEditor::OnEditorCharAdded );
    Connect( m_ID,  -1, wxEVT_SCI_DWELLSTART,
                  (wxObjectEventFunction) (wxEventFunction) (wxScintillaEventFunction)
                  &ScbEditor::OnEditorDwellStart );
    Connect( m_ID,  -1, wxEVT_SCI_DWELLEND,
                  (wxObjectEventFunction) (wxEventFunction) (wxScintillaEventFunction)
                  &ScbEditor::OnEditorDwellEnd );
    Connect( m_ID,  -1, wxEVT_SCI_USERLISTSELECTION,
                  (wxObjectEventFunction) (wxEventFunction) (wxScintillaEventFunction)
                  &ScbEditor::OnUserListSelection );
    Connect( m_ID,  -1, wxEVT_SCI_MODIFIED,
                  (wxObjectEventFunction) (wxEventFunction) (wxScintillaEventFunction)
                  &ScbEditor::OnEditorModified );

    // Now bind all *other* scintilla events to a common function so that editor hooks
    // can be informed for them too.
    // If you implement one of these events using a different function, do the following:
    //  * comment it out here,
    //  * "connect" it in the above block
    //  * and make sure you call OnScintillaEvent() from your new handler function
    // This will make sure that all editor hooks will be called when needed.
    int scintilla_events[] =
    {
//        wxEVT_SCI_CHANGE,
        wxEVT_SCI_STYLENEEDED,
//        wxEVT_SCI_CHARADDED,
        wxEVT_SCI_SAVEPOINTREACHED,
        wxEVT_SCI_SAVEPOINTLEFT,
        wxEVT_SCI_ROMODIFYATTEMPT,
        wxEVT_SCI_KEY,
        wxEVT_SCI_DOUBLECLICK,
//        wxEVT_SCI_UPDATEUI,
//        wxEVT_SCI_MODIFIED,
        wxEVT_SCI_MACRORECORD,
//        wxEVT_SCI_MARGINCLICK,
        wxEVT_SCI_NEEDSHOWN,
        wxEVT_SCI_PAINTED,
//        wxEVT_SCI_USERLISTSELECTION,
        wxEVT_SCI_URIDROPPED,
//        wxEVT_SCI_DWELLSTART,
//        wxEVT_SCI_DWELLEND,
        wxEVT_SCI_START_DRAG,
        wxEVT_SCI_DRAG_OVER,
        wxEVT_SCI_DO_DROP,
        wxEVT_SCI_ZOOM,
        wxEVT_SCI_HOTSPOT_CLICK,
        wxEVT_SCI_HOTSPOT_DCLICK,
        wxEVT_SCI_CALLTIP_CLICK,
//        wxEVT_SCI_AUTOCOMP_SELECTION,
//        wxEVT_SCI_INDICATOR_CLICK,
//        wxEVT_SCI_INDICATOR_RELEASE,

        -1 // to help enumeration of this array
    };
    int i = 0;
    while (scintilla_events[i] != -1)
    {
        Connect( m_ID,  -1, scintilla_events[i],
                      (wxObjectEventFunction) (wxEventFunction) (wxScintillaEventFunction)
                      &ScbEditor::OnScintillaEvent );
        ++i;
    }

    return control;
}

void ScbEditor::Split(ScbEditor::SplitType split)
{
    Freeze();

    // unsplit first, if needed
    if (m_pSplitter)
    {
        Unsplit();
        Manager::Yield();
    }
    m_SplitType = split;
    if (m_SplitType == stNoSplit)
    {
        Thaw();
        return;
    }

    // remove the left control from the sizer
    m_pSizer->Detach(m_pControl);

    // create the splitter window
    m_pSplitter = new wxSplitterWindow(this, wxNewId(), wxDefaultPosition, wxDefaultSize, wxSP_NOBORDER | wxSP_LIVE_UPDATE);
    m_pSplitter->SetMinimumPaneSize(32);

    // create the right control
    m_pControl2 = CreateEditor();

    // update controls' look'n'feel
    // do it here (before) document is attached, speeds up syntaxhighlighting
    // we do not call "SetEditorStyleAfterFileOpen" here becaus it calls SetLanguage for the already loaded text inside
    // the left control and slows down loaduing of large files a lot.
    ConfigManager* mgr = Manager::Get()->GetConfigManager(_T("editor"));
    SetFoldingIndicator(mgr->ReadInt(_T("/folding/indicator"), 2));
    UnderlineFoldedLines(mgr->ReadBool(_T("/folding/underline_folded_line"), true));
    InternalSetEditorStyleBeforeFileOpen(m_pControl2);

    if (m_pTheme)
        m_pTheme->Apply(m_lang, m_pControl2);

    // and make it a live copy of left control
    m_pControl2->SetDocPointer(m_pControl->GetDocPointer());

    // parent both controls under the splitter
    m_pControl->Reparent(m_pSplitter);
    m_pControl2->Reparent(m_pSplitter);

    // add the splitter in the sizer
    m_pSizer->SetDimension(0, 0, GetSize().x, GetSize().y);
    m_pSizer->Add(m_pSplitter, 1, wxEXPAND);
    m_pSizer->Layout();

    // split as needed
    switch (m_SplitType)
    {
        case stHorizontal:
            m_pSplitter->SplitHorizontally(m_pControl, m_pControl2, 0);
            break;

        case stVertical:
            m_pSplitter->SplitVertically(m_pControl, m_pControl2, 0);
            break;

        default:
            break;
    }

    SetEditorStyleAfterFileOpen();


    // make sure the line numbers margin is correct for the new control
    m_pControl2->SetMarginWidth(0, m_pControl->GetMarginWidth(0));

    Thaw();
}

void ScbEditor::Unsplit()
{
    m_SplitType = stNoSplit;
    if (!m_pSplitter)
        return;

    Freeze();

    // if "unsplit" requested on right control, swap left-right first
    if (GetControl() == m_pControl2)
    {
        cbStyledTextCtrl* tmp = m_pControl;
        m_pControl = m_pControl2;
        m_pControl2 = tmp;
    }

    // remove the splitter from the sizer
    m_pSizer->Detach(m_pSplitter);
    // parent the left control under this
    m_pControl->Reparent(this);
    // add it in the sizer
    m_pSizer->Add(m_pControl, 1, wxEXPAND);
    // destroy the splitter and right control
    DestroySplitView();
    // and layout
    m_pSizer->Layout();

    Thaw();
}

// static
wxColour ScbEditor::GetOptionColour(const wxString& option, const wxColour _default)
{
    return Manager::Get()->GetConfigManager(_T("editor"))->ReadColour(option, _default);
}

void ScbEditor::SetEditorStyle()
{
    SetEditorStyleBeforeFileOpen();
    SetEditorStyleAfterFileOpen();
}

void ScbEditor::SetEditorStyleBeforeFileOpen()
{
    ConfigManager* mgr = Manager::Get()->GetConfigManager(_T("editor"));

    // update the tab text based on preferences
    if (m_pProjectFile)
    {
        if (mgr->ReadBool(_T("/tab_text_relative"), true))
            m_Shortname = m_pProjectFile->relativeToCommonTopLevelPath;
        else
            m_Shortname = m_pProjectFile->file.GetFullName();
        SetEditorTitle(m_Shortname);
    }

    // Folding properties.
    m_pData->mFoldingLimit = mgr->ReadBool(_T("/folding/limit"), false);
    m_pData->mFoldingLimitLevel = mgr->ReadInt(_T("/folding/limit_level"), 1);

    // EOL properties
    m_pData->m_strip_trailing_spaces = mgr->ReadBool(_T("/eol/strip_trailing_spaces"), true);
    m_pData->m_ensure_final_line_end = mgr->ReadBool(_T("/eol/ensure_final_line_end"), true);
    m_pData->m_ensure_consistent_line_ends = mgr->ReadBool(_T("/eol/ensure_consistent_line_ends"), false);

    InternalSetEditorStyleBeforeFileOpen(m_pControl);

    SetFoldingIndicator(mgr->ReadInt(_T("/folding/indicator"), 2));
    UnderlineFoldedLines(mgr->ReadBool(_T("/folding/underline_folded_line"), true));

    if (m_pControl2)
        InternalSetEditorStyleBeforeFileOpen(m_pControl2);

    SetLanguage( HL_AUTO );
}

void ScbEditor::SetEditorStyleAfterFileOpen()
{
    InternalSetEditorStyleAfterFileOpen(m_pControl);
    if (m_pControl2)
        InternalSetEditorStyleAfterFileOpen(m_pControl2);

    ConfigManager* mgr = Manager::Get()->GetConfigManager(_T("editor"));

    // line numbers
    if (mgr->ReadBool(_T("/show_line_numbers"), true))
        m_pData->SetLineNumberColWidth();
    else
    {
        m_pControl->SetMarginWidth(0, 0);
        if (m_pControl2)
            m_pControl2->SetMarginWidth(0, 0);
    }
}

// static
// public version of InternalSetEditorStyleBeforeFileOpen
void ScbEditor::ApplyStyles(cbStyledTextCtrl* control)
{
    if (!control)
        return;
    InternalSetEditorStyleBeforeFileOpen(control);
    InternalSetEditorStyleAfterFileOpen(control);

    ConfigManager* mgr = Manager::Get()->GetConfigManager(_T("editor"));

    int pixelWidth = control->TextWidth(wxSCI_STYLE_LINENUMBER, _T("9"));
    if (mgr->ReadBool(_T("/show_line_numbers"), true))
        control->SetMarginWidth(0, 5 * pixelWidth); // hardcoded width up to 99999 lines
}

// static
void ScbEditor::InternalSetEditorStyleBeforeFileOpen(cbStyledTextCtrl* control)
{
    if (!control)
        return;

    ConfigManager* mgr = Manager::Get()->GetConfigManager(_T("editor"));

    // 8 point is not readable on Mac OS X, increase font size
    wxFont font(platform::macosx ? 10 : 8, wxMODERN, wxNORMAL, wxNORMAL);

    wxString fontstring = mgr->Read(_T("/font"), wxEmptyString);

    if (!fontstring.IsEmpty())
    {
        wxNativeFontInfo nfi;
        nfi.FromString(fontstring);
        font.SetNativeFontInfo(nfi);
    }

    control->SetMouseDwellTime(1000);

    control->SetCaretWidth(mgr->ReadInt(_T("/caret/width"), 1));
    control->SetCaretForeground(GetOptionColour(_T("/caret/colour"), *wxBLACK));
    control->SetCaretPeriod(mgr->ReadInt(_T("/caret/period"), 500));
    control->SetCaretLineVisible(mgr->ReadBool(_T("/highlight_caret_line"), false));
    control->SetCaretLineBackground(GetOptionColour(_T("/highlight_caret_line_colour"), wxColour(0xFF, 0xFF, 0x00)));

    // if user wants "Home" key to set cursor to the very beginning of line
    if (mgr->ReadBool(_T("/simplified_home"), false))
    {
        control->CmdKeyAssign(wxSCI_KEY_HOME, wxSCI_SCMOD_NORM, wxSCI_CMD_HOME);
        control->CmdKeyAssign(wxSCI_KEY_HOME, wxSCI_SCMOD_SHIFT, wxSCI_CMD_HOMEEXTEND);
        control->CmdKeyAssign(wxSCI_KEY_HOME, wxSCI_SCMOD_ALT | wxSCI_SCMOD_SHIFT, wxSCI_CMD_HOMERECTEXTEND);
    }
    else // else set default "Home" key behavior
    {
        control->CmdKeyAssign(wxSCI_KEY_HOME, wxSCI_SCMOD_NORM, wxSCI_CMD_VCHOME);
        control->CmdKeyAssign(wxSCI_KEY_HOME, wxSCI_SCMOD_SHIFT, wxSCI_CMD_VCHOMEEXTEND);
        control->CmdKeyAssign(wxSCI_KEY_HOME, wxSCI_SCMOD_ALT | wxSCI_SCMOD_SHIFT, wxSCI_CMD_VCHOMERECTEXTEND);
    }

    control->SetUseTabs(mgr->ReadBool(_T("/use_tab"), false));
    control->SetIndentationGuides(mgr->ReadBool(_T("/show_indent_guides"), false)?wxSCI_IV_LOOKBOTH:wxSCI_IV_NONE);
    control->SetTabIndents(mgr->ReadBool(_T("/tab_indents"), true));
    control->SetBackSpaceUnIndents(mgr->ReadBool(_T("/backspace_unindents"), true));
    control->SetWrapMode(mgr->ReadBool(_T("/word_wrap"), false));
    control->SetViewEOL(mgr->ReadBool(_T("/show_eol"), false));
    control->SetViewWhiteSpace(mgr->ReadInt(_T("/view_whitespace"), 0));
    //gutter
    control->SetEdgeMode(mgr->ReadInt(_T("/gutter/mode"), 0));
    control->SetEdgeColour(GetOptionColour(_T("/gutter/colour"), *wxLIGHT_GREY));
    control->SetEdgeColumn(mgr->ReadInt(_T("/gutter/column"), 80));

    control->StyleSetFont(wxSCI_STYLE_DEFAULT, font);
    control->StyleClearAll();

    control->SetTabWidth(mgr->ReadInt(_T("/tab_size"), 4));

    // margin for bookmarks, breakpoints etc.
    // FIXME: how to display a mark with an offset???
    control->SetMarginWidth(1, 16);
    control->SetMarginType(1, wxSCI_MARGIN_SYMBOL);
    control->SetMarginSensitive(1, mgr->ReadBool(_T("/margin_1_sensitive"), true));
    control->SetMarginMask(1, (1 << BOOKMARK_MARKER) |
                                 (1 << BREAKPOINT_MARKER) |
                                 (1 << DEBUG_MARKER) |
                                 (1 << ERROR_MARKER));
    control->MarkerDefine(BOOKMARK_MARKER, BOOKMARK_STYLE);
    control->MarkerSetBackground(BOOKMARK_MARKER, wxColour(0xA0, 0xA0, 0xFF));
    control->MarkerDefine(BREAKPOINT_MARKER, BREAKPOINT_STYLE);
    control->MarkerSetBackground(BREAKPOINT_MARKER, wxColour(0xFF, 0x00, 0x00));
    control->MarkerDefine(DEBUG_MARKER, DEBUG_STYLE);
    control->MarkerSetBackground(DEBUG_MARKER, wxColour(0xFF, 0xFF, 0x00));
    control->MarkerDefine(ERROR_MARKER, ERROR_STYLE);
    control->MarkerSetBackground(ERROR_MARKER, wxColour(0xFF, 0x00, 0x00));

    // NOTE: duplicate line in editorconfigurationdlg.cpp (ctor)
    static const int default_eol = platform::windows ? wxSCI_EOL_CRLF : wxSCI_EOL_LF; // Windows takes CR+LF, other platforms LF only

    control->SetEOLMode(mgr->ReadInt(_T("/eol/eolmode"), default_eol));

    // folding margin
    control->SetProperty(_T("fold"), mgr->ReadBool(_T("/folding/show_folds"), true) ? _T("1") : _T("0"));
    control->SetProperty(_T("fold.html"), mgr->ReadBool(_T("/folding/fold_xml"), true) ? _T("1") : _T("0"));
    control->SetProperty(_T("fold.comment"), mgr->ReadBool(_T("/folding/fold_comments"), false) ? _T("1") : _T("0"));
    control->SetProperty(_T("fold.compact"), _T("0"));
    control->SetProperty(_T("fold.preprocessor"), mgr->ReadBool(_T("/folding/fold_preprocessor"), false) ? _T("1") : _T("0"));
    if (mgr->ReadBool(_T("/folding/show_folds"), true))
    {
        control->SetFoldFlags(16);
        control->SetMarginType(2, wxSCI_MARGIN_SYMBOL);
        control->SetMarginWidth(2, 16);
        control->SetMarginMask(2, wxSCI_MASK_FOLDERS);
        control->SetMarginSensitive(2, 1);

        /*Default behaviour
        control->MarkerDefine(wxSCI_MARKNUM_FOLDEROPEN, wxSCI_MARK_BOXMINUS);
        control->MarkerSetForeground(wxSCI_MARKNUM_FOLDEROPEN, wxColour(0xff, 0xff, 0xff));
        control->MarkerSetBackground(wxSCI_MARKNUM_FOLDEROPEN, wxColour(0x80, 0x80, 0x80));
        control->MarkerDefine(wxSCI_MARKNUM_FOLDER, wxSCI_MARK_BOXPLUS);
        control->MarkerSetForeground(wxSCI_MARKNUM_FOLDER, wxColour(0xff, 0xff, 0xff));
        control->MarkerSetBackground(wxSCI_MARKNUM_FOLDER, wxColour(0x80, 0x80, 0x80));
        control->MarkerDefine(wxSCI_MARKNUM_FOLDERSUB, wxSCI_MARK_VLINE);
        control->MarkerSetForeground(wxSCI_MARKNUM_FOLDERSUB, wxColour(0xff, 0xff, 0xff));
        control->MarkerSetBackground(wxSCI_MARKNUM_FOLDERSUB, wxColour(0x80, 0x80, 0x80));
        control->MarkerDefine(wxSCI_MARKNUM_FOLDERTAIL, wxSCI_MARK_LCORNER);
        control->MarkerSetForeground(wxSCI_MARKNUM_FOLDERTAIL, wxColour(0xff, 0xff, 0xff));
        control->MarkerSetBackground(wxSCI_MARKNUM_FOLDERTAIL, wxColour(0x80, 0x80, 0x80));
        control->MarkerDefine(wxSCI_MARKNUM_FOLDEREND, wxSCI_MARK_BOXPLUSCONNECTED);
        control->MarkerSetForeground(wxSCI_MARKNUM_FOLDEREND, wxColour(0xff, 0xff, 0xff));
        control->MarkerSetBackground(wxSCI_MARKNUM_FOLDEREND, wxColour(0x80, 0x80, 0x80));
        control->MarkerDefine(wxSCI_MARKNUM_FOLDEROPENMID, wxSCI_MARK_BOXMINUSCONNECTED);
        control->MarkerSetForeground(wxSCI_MARKNUM_FOLDEROPENMID, wxColour(0xff, 0xff, 0xff));
        control->MarkerSetBackground(wxSCI_MARKNUM_FOLDEROPENMID, wxColour(0x80, 0x80, 0x80));
        control->MarkerDefine(wxSCI_MARKNUM_FOLDERMIDTAIL, wxSCI_MARK_TCORNER);
        control->MarkerSetForeground(wxSCI_MARKNUM_FOLDERMIDTAIL, wxColour(0xff, 0xff, 0xff));
        control->MarkerSetBackground(wxSCI_MARKNUM_FOLDERMIDTAIL, wxColour(0x80, 0x80, 0x80));
        */
    }
    else
        control->SetMarginWidth(2, 0);
}

// static
void ScbEditor::InternalSetEditorStyleAfterFileOpen(cbStyledTextCtrl* control)
{
    if (!control)
        return;

    // line numbering
    control->SetMarginType(0, wxSCI_MARGIN_NUMBER);
}

void ScbEditor::SetColourSet(SEditorColourSet* theme)
{
    m_pTheme = theme;
    SetLanguage( m_lang );
}

wxFontEncoding ScbEditor::GetEncoding( ) const
{
    if (!m_pData)
        return wxFONTENCODING_SYSTEM;
    return m_pData->m_encoding;
}

wxString ScbEditor::GetEncodingName( ) const
{
    return wxFontMapper::GetEncodingName(GetEncoding());
}

void ScbEditor::SetEncoding( wxFontEncoding encoding )
{
    if (!m_pData)
        return;

    if ( encoding == wxFONTENCODING_SYSTEM )
        encoding = wxLocale::GetSystemEncoding();

    if ( encoding == GetEncoding() )
        return;

    m_pData->m_encoding = encoding;
    SetModified(true);

    /* NOTE (Biplab#1#): Following method is wrong. The file is still in old encoding
    *  So if you try to load it with new encoding, you'll get garbage*/
    /*wxString msg;
    msg.Printf(_("Do you want to reload the file with the new encoding (you will lose any unsaved work)?"));
    if (cbMessageBox(msg, _("Reload file?"), wxYES_NO) == wxID_YES)
        Reload(false);
    else
        SetModified(true);*/
}

bool ScbEditor::GetUseBom() const
{
    if (!m_pData)
        return false;
    return m_pData->m_useByteOrderMark;
}

void ScbEditor::SetUseBom( bool bom )
{
    if (!m_pData)
        return;

    if ( bom == GetUseBom() )
        return;

    m_pData->m_useByteOrderMark = bom;
    SetModified(true);
}

bool ScbEditor::Reload(bool detectEncoding)
{
    // keep current pos
    const int pos = m_pControl ? m_pControl->GetCurrentPos() : 0;
    const int pos2 = m_pControl2 ? m_pControl2->GetCurrentPos() : 0;

    // call open
    if (!Open(detectEncoding))
    {
        return false;
    }
    // Re-establish margin styles, width,  etc
    SetEditorStyleAfterFileOpen();

    // return (if possible) to old pos
    if (m_pControl)
    {
        m_pControl->GotoPos(pos);
    }
    if (m_pControl2)
    {
        m_pControl2->GotoPos(pos2);
    }
    return true;
}

void ScbEditor::Touch()
{
    m_LastModified = wxDateTime::Now();
}

void ScbEditor::DetectEncoding( )
{
    if (!m_pData)
        return;

    EncodingDetector detector(m_Filename);
    if (!detector.IsOK())
        return;

    m_pData->m_useByteOrderMark = detector.UsesBOM();
    m_pData->m_byteOrderMarkLength = detector.GetBOMSizeInBytes();
    m_pData->m_encoding = detector.GetFontEncoding();

    // FIXME: Should this default to local encoding or latin-1? (IOW, implement proper encoding detection)
    if (m_pData->m_encoding == wxFONTENCODING_ISO8859_1)
    {
        // if the encoding detector returned the default value,
        // use the user's preference then
        wxString enc_name = Manager::Get()->GetConfigManager(_T("editor"))->Read(_T("/default_encoding"), wxLocale::GetSystemEncodingName());
        m_pData->m_encoding = wxFontMapper::GetEncodingFromName(enc_name);
    }
}

void ScbEditor::SetLanguage( HighlightLanguage lang )
{
    if (m_pTheme)
    {
        m_lang = m_pTheme->Apply(this, lang);
    }
    else
    {
        m_lang = HL_AUTO;
    }
}

bool ScbEditor::Open(bool detectEncoding)
{
    if (m_pProjectFile)
    {
        if (!wxFileExists(m_Filename))
            m_pProjectFile->SetFileState(fvsMissing);
        else if (!wxFile::Access(m_Filename.c_str(), wxFile::write)) // readonly
            m_pProjectFile->SetFileState(fvsReadOnly);
    }

    if (!wxFileExists(m_Filename))
        return false;

    // open file
    m_pControl->SetReadOnly(false);
    wxString st;

    m_pControl->ClearAll();
    m_pControl->SetModEventMask(0);

    if (!m_pData)
        return false;

    if (!m_pData->m_pFileLoader)
    {
        m_pData->m_pFileLoader = Manager::Get()->GetFileManager()->Load(m_Filename, false);
    }

    EncodingDetector enc((wxByte*)m_pData->m_pFileLoader->GetData(), m_pData->m_pFileLoader->GetLength());
    st = enc.GetWxStr();
    if (detectEncoding)
    {
        m_pData->m_useByteOrderMark = enc.UsesBOM();
        m_pData->m_byteOrderMarkLength = enc.GetBOMSizeInBytes();
        m_pData->m_encoding = enc.GetFontEncoding();

        SetEncoding(enc.GetFontEncoding());
        m_pData->m_byteOrderMarkLength = enc.GetBOMSizeInBytes();
        SetUseBom(m_pData->m_byteOrderMarkLength > 0);
    }

    m_pControl->InsertText(0, st);
    m_pControl->EmptyUndoBuffer();
    m_pControl->SetModEventMask(wxSCI_MODEVENTMASKALL);

    // mark the file read-only, if applicable
    bool read_only = !wxFile::Access(m_Filename.c_str(), wxFile::write);
    m_pControl->SetReadOnly(read_only);
//    SetLanguage(HL_AUTO); // bottleneck !!! no need to set it here, it's already done in SetEditorStyleBeforeFileOpen

    if (Manager::Get()->GetConfigManager(_T("editor"))->ReadBool(_T("/folding/fold_all_on_open"), false))
        FoldAll();

    wxFileName fname(m_Filename);
    m_LastModified = fname.GetModificationTime();

    SetModified(false);
    NotifyPlugins(cbEVT_EDITOR_OPEN);

    m_pControl->SetZoom(GetEditorManager()->GetZoom());
    if (m_pControl2)
        m_pControl2->SetZoom(GetEditorManager()->GetZoom());

    if (m_pData->m_pFileLoader)
    {
        delete m_pData->m_pFileLoader;
        m_pData->m_pFileLoader = 0;
    }

    return true;
}
// ----------------------------------------------------------------------------
bool ScbEditor::Save()
// ----------------------------------------------------------------------------
{
    //(pecan 2008/5/02)
    // Allow parent to see save request first.
    NotifyPlugins(cbEVT_EDITOR_SAVE);

    if (!GetModified())
        return true;

    // one undo action for all modifications in this context
    // (angled braces added for clarity)
    m_pControl->BeginUndoAction();
    {
        if(m_pData->m_strip_trailing_spaces)
        {
            m_pData->StripTrailingSpaces();
        }
        if(m_pData->m_ensure_consistent_line_ends)
        {
            m_pData->EnsureConsistentLineEnds();
        }
        if(m_pData->m_ensure_final_line_end)
        {
            m_pData->EnsureFinalLineEnd();
        }
    }
    m_pControl->EndUndoAction();

    if (!m_IsOK)
    {
        return SaveAs();
    }

    if(!cbSaveToFile(m_Filename, m_pControl->GetText(),GetEncoding(),GetUseBom()))
    {
        wxString msg;
        msg.Printf(_("File %s could not be saved..."), GetFilename().c_str());
        cbMessageBox(msg, _("Error saving file"), wxICON_ERROR);
        return false; // failed; file is read-only?
    }

    wxFileName fname(m_Filename);
    m_LastModified = fname.GetModificationTime();

    m_IsOK = true;

    m_pControl->SetSavePoint();
    SetModified(false);

    return true;
} // end of Save

bool ScbEditor::SaveAs()
{
    wxFileName fname;
    fname.Assign(m_Filename);
    ConfigManager* mgr = Manager::Get()->GetConfigManager(_T("app"));
    int StoredIndex = 0;
    wxString Filters = FileFilters::GetFilterString();
    wxString Path = fname.GetPath();
    wxString Extension = fname.GetExt();
    wxString Filter;
    if (!Extension.IsEmpty())
    {    // use the current extension as the filter
        // Select filter belonging to this file type:
        Extension.Prepend(_T("."));
        Filter = FileFilters::GetFilterString(Extension);
    }
    else if(mgr)
    {
        // File type is unknown. Select the last used filter:
        Filter = mgr->Read(_T("/file_dialogs/save_file_as/filter"), _T("C/C++ files"));
    }
    if(!Filter.IsEmpty())
    {
        // We found a filter, look up its index:
        int sep = Filter.find(_T("|"));
        if (sep != wxNOT_FOUND)
        {
            Filter.Truncate(sep);
        }
        if (!Filter.IsEmpty())
        {
            FileFilters::GetFilterIndexFromName(Filters, Filter, StoredIndex);
        }
    }
    if(mgr && Path.IsEmpty())
    {
        Path = mgr->Read(_T("/file_dialogs/save_file_as/directory"), Path);
    }
    wxFileDialog dlg(Manager::Get()->GetAppWindow(),
                                         _("Save file"),
                                         Path,
                                         fname.GetFullName(),
                                         Filters,
                                         wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    dlg.SetFilterIndex(StoredIndex);
    PlaceWindow(&dlg);
    if (dlg.ShowModal() != wxID_OK)
    {  // cancelled out
        return false;
    }
    m_Filename = dlg.GetPath();
    Manager::Get()->GetLogManager()->Log(m_Filename);
    fname.Assign(m_Filename);
    m_Shortname = fname.GetFullName();
    SetEditorTitle(m_Shortname);
    //Manager::Get()->GetLogManager()->Log(mltDevDebug, "Filename=%s\nShort=%s", m_Filename.c_str(), m_Shortname.c_str());
    m_IsOK = true;
    SetModified(true);
    SetLanguage( HL_AUTO );
    // store the last used filter and directory
    if(mgr)
    {
        int Index = dlg.GetFilterIndex();
        wxString Filter;
        if(FileFilters::GetFilterNameFromIndex(Filters, Index, Filter))
        {
            mgr->Write(_T("/file_dialogs/save_file_as/filter"), Filter);
        }
        wxString Test = dlg.GetDirectory();
        mgr->Write(_T("/file_dialogs/save_file_as/directory"), dlg.GetDirectory());
    }
    return Save();
} // end of SaveAs

bool ScbEditor::SaveFoldState()
{
    bool bRet = false;
    if((m_foldBackup = CreateEditor()))
    {
        ApplyStyles(m_foldBackup);
        m_foldBackup->SetText(m_pControl->GetText());
        int count = m_pControl->GetLineCount();
        for (int i = 0; i < count; ++i)
        {
            m_foldBackup->SetFoldLevel(i,m_pControl->GetFoldLevel(i));
        }
        bRet = true;
    }
    return bRet;
} // end of SaveFoldState

bool ScbEditor::FixFoldState()
{
    bool bRet = false;
    if(m_foldBackup)
    {
        int backupLength = m_foldBackup->GetLineCount();
        int realLength = m_pControl->GetLineCount();
        if(backupLength == realLength) //It is supposed to be the same, but you never know :)
        {
            ConfigManager* mgr = Manager::Get()->GetConfigManager(_T("editor"));
            if (mgr->ReadBool(_T("/folding/show_folds"), true)) //Only fix the folds if the folds are enabled
            {
                m_pControl->Colourise(0, -1); // the *most* important part!
                m_foldBackup->Colourise(0, -1); // " Not so sure here.. but what the hell :)
                int count = m_pControl->GetLineCount();
                for (int i = 0; i < count; ++i)
                {
                    int oldFoldLevel = m_foldBackup->GetFoldLevel(i);
                    int newFoldLevel = m_pControl->GetFoldLevel(i);
                    if(oldFoldLevel != newFoldLevel)
                    {
                        if(m_pControl->GetLineVisible(i) == true)
                        {
                            m_pControl->SetFoldExpanded(i, true);
                        }
                        else
                        {
                            int parent = m_foldBackup->GetFoldParent(i);
                            while(parent != -1)
                            {
                                m_pControl->ToggleFold(parent);
                                parent = m_foldBackup->GetFoldParent(parent);
                            }
                            m_pControl->ShowLines(i, i);
                            parent = m_foldBackup->GetFoldParent(i);
                            while(parent != -1)
                            {
                                m_pControl->ToggleFold(parent);
                                parent = m_foldBackup->GetFoldParent(parent);
                            }
                        }
                    }
                }
            }
            bRet = true;
        }
        m_foldBackup->Destroy();
        m_foldBackup = 0;
    }
    return bRet;
} // end of FixFoldState

void ScbEditor::AutoComplete()
{
    LogManager* msgMan = Manager::Get()->GetLogManager();
    SAutoCompleteMap& map = GetEditorManager()->GetAutoCompleteMap();
    cbStyledTextCtrl* control = GetControl();
    int curPos = control->GetCurrentPos();
    int wordStartPos = control->WordStartPosition(curPos, true);
    wxString keyword = control->GetTextRange(wordStartPos, curPos);
    wxString lineIndent = GetLineIndentString(control->GetCurrentLine());
    msgMan->DebugLog(_T("Auto-complete keyword: ") + keyword);

    SAutoCompleteMap::iterator it;
    for (it = map.begin(); it != map.end(); ++it)
    {
        if (keyword == it->first)
        {
            // found; auto-complete it
            msgMan->DebugLog(_T("Match found"));

            // indent code accordingly
            wxString code = it->second;
            code.Replace(_T("\n"), _T('\n') + lineIndent);

            // look for and replace macros
            bool canceled = false;
            int macroPos = code.Find(_T("$("));
            while (macroPos != -1)
            {
                // locate ending parenthesis
                int macroPosEnd = macroPos + 2;
                int len = (int)code.Length();
                while (macroPosEnd < len && code.GetChar(macroPosEnd) != _T(')'))
                    ++macroPosEnd;
                if (macroPosEnd == len)
                    break; // no ending parenthesis

                wxString macroName = code.SubString(macroPos + 2, macroPosEnd - 1);
                msgMan->DebugLog(_T("Found macro: ") + macroName);
                wxString macro = wxGetTextFromUser(_("Please enter the text for \"") + macroName + _T("\":"), _("Macro substitution"));
                if (macro.IsEmpty())
                {
                    canceled = true;
                    break;
                }
                code.Replace(_T("$(") + macroName + _T(")"), macro);
                macroPos = code.Find(_T("$("));
            }

            if (canceled)
                break;

            control->BeginUndoAction();

            // delete keyword
            control->SetSelectionVoid(wordStartPos, curPos);
            control->ReplaceSelection(_T(""));
            curPos = wordStartPos;

            // replace any other macros in the generated code
            Manager::Get()->GetMacrosManager()->ReplaceMacros(code);
            // add the text
            control->InsertText(curPos, code);

            // put cursor where "|" appears in code (if it appears)
            int caretPos = code.Find(_T('|'));
            if (caretPos != -1)
            {
                control->SetCurrentPos(curPos + caretPos);
                control->SetSelectionVoid(curPos + caretPos, curPos + caretPos + 1);
                control->ReplaceSelection(_T(""));
            }

            control->EndUndoAction();
            break;
        }
    }
}

void ScbEditor::DoFoldAll(int fold)
{
    cbAssert(m_pControl);
    if (m_SplitType != stNoSplit)
        cbAssert(m_pControl2);
    cbStyledTextCtrl* ctrl = GetControl();
    ctrl->Colourise(0, -1); // the *most* important part!
    int count = ctrl->GetLineCount();
    for (int i = 0; i <= count; ++i)
        DoFoldLine(i, fold);
}

void ScbEditor::DoFoldBlockFromLine(int line, int fold)
{
    cbAssert(m_pControl);
    if (m_SplitType != stNoSplit)
        cbAssert(m_pControl2);
    cbStyledTextCtrl* ctrl = GetControl();
    ctrl->Colourise(0, -1); // the *most* important part!
    int i, parent, maxLine, level, UnfoldUpto = line;
    bool FoldedInside = false;

    parent = ctrl->GetFoldParent(line);
    level = ctrl->GetFoldLevel(parent);
    /* The following code will check if the child is hidden
    *  under parent before unfolding it
    */
    if (fold == 0)
    {
        do
        {
            if (!ctrl->GetFoldExpanded(parent))
            {
                FoldedInside = true;
                UnfoldUpto = parent;
            }
            if (wxSCI_FOLDLEVELBASE == (level & wxSCI_FOLDLEVELNUMBERMASK))
                break;
            parent = ctrl->GetFoldParent(parent);
            level = ctrl->GetFoldLevel(parent);
        }
        while (parent != -1);
    }

    maxLine = ctrl->GetLastChild(line, -1);

    for (i = UnfoldUpto; i <= maxLine; ++i)
        DoFoldLine(i, fold);
}

bool ScbEditor::DoFoldLine(int line, int fold)
{
    cbAssert(m_pControl);
    if (m_SplitType != stNoSplit)
        cbAssert(m_pControl2);
    cbStyledTextCtrl* ctrl = GetControl();
    int level = ctrl->GetFoldLevel(line);

    // The fold parameter is the type of folding action requested
    // 0 = Unfold; 1 = Fold; 2 = Toggle folding.

    // Check if the line is a header (fold point).
    if (level & wxSCI_FOLDLEVELHEADERFLAG)
    {
        bool IsExpanded = ctrl->GetFoldExpanded(line);

        // If a fold/unfold request is issued when the block is already
        // folded/unfolded, ignore the request.
        if (fold == 0 && IsExpanded) return true;
        if (fold == 1 && !IsExpanded) return true;

        // Apply the folding level limit only if the current block will be
        // folded (that means it's currently expanded), folding level limiter
        // must be enabled of course. Unfolding will not be affected.
        if (m_pData->mFoldingLimit && IsExpanded)
        {
            if ((level & wxSCI_FOLDLEVELNUMBERMASK) > (wxSCI_FOLDLEVELBASE + m_pData->mFoldingLimitLevel-1))
                return false;
        }

        ctrl->ToggleFold(line);
        return true;
    }
    return false;
}

void ScbEditor::FoldAll()
{
    DoFoldAll(1);
}

void ScbEditor::UnfoldAll()
{
    DoFoldAll(0);
}

void ScbEditor::ToggleAllFolds()
{
    DoFoldAll(2);
}

void ScbEditor::SetFoldingIndicator(int id)
{
    //Arrow
    if(id == 0)
    {
        SetMarkerStyle(wxSCI_MARKNUM_FOLDEROPEN, wxSCI_MARK_ARROWDOWN, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
        SetMarkerStyle(wxSCI_MARKNUM_FOLDER, wxSCI_MARK_ARROW, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
        SetMarkerStyle(wxSCI_MARKNUM_FOLDERSUB, wxSCI_MARK_BACKGROUND, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
        SetMarkerStyle(wxSCI_MARKNUM_FOLDERTAIL, wxSCI_MARK_BACKGROUND, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
        SetMarkerStyle(wxSCI_MARKNUM_FOLDEREND, wxSCI_MARK_ARROW, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
        SetMarkerStyle(wxSCI_MARKNUM_FOLDEROPENMID, wxSCI_MARK_ARROWDOWN, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
        SetMarkerStyle(wxSCI_MARKNUM_FOLDERMIDTAIL, wxSCI_MARK_BACKGROUND, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
    }

    //Circle
    else if(id == 1)
    {
        SetMarkerStyle(wxSCI_MARKNUM_FOLDEROPEN, wxSCI_MARK_CIRCLEMINUS, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
        SetMarkerStyle(wxSCI_MARKNUM_FOLDER, wxSCI_MARK_CIRCLEPLUS, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
        SetMarkerStyle(wxSCI_MARKNUM_FOLDERSUB, wxSCI_MARK_VLINE, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
        SetMarkerStyle(wxSCI_MARKNUM_FOLDERTAIL, wxSCI_MARK_LCORNERCURVE, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
        SetMarkerStyle(wxSCI_MARKNUM_FOLDEREND, wxSCI_MARK_CIRCLEPLUSCONNECTED, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
        SetMarkerStyle(wxSCI_MARKNUM_FOLDEROPENMID, wxSCI_MARK_CIRCLEMINUSCONNECTED, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
        SetMarkerStyle(wxSCI_MARKNUM_FOLDERMIDTAIL, wxSCI_MARK_TCORNER, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
    }

    //Square
    else if(id == 2)
    {
        SetMarkerStyle(wxSCI_MARKNUM_FOLDEROPEN, wxSCI_MARK_BOXMINUS, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
        SetMarkerStyle(wxSCI_MARKNUM_FOLDER, wxSCI_MARK_BOXPLUS, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
        SetMarkerStyle(wxSCI_MARKNUM_FOLDERSUB, wxSCI_MARK_VLINE, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
        SetMarkerStyle(wxSCI_MARKNUM_FOLDERTAIL, wxSCI_MARK_LCORNER, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
        SetMarkerStyle(wxSCI_MARKNUM_FOLDEREND, wxSCI_MARK_BOXPLUSCONNECTED, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
        SetMarkerStyle(wxSCI_MARKNUM_FOLDEROPENMID, wxSCI_MARK_BOXMINUSCONNECTED, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
        SetMarkerStyle(wxSCI_MARKNUM_FOLDERMIDTAIL, wxSCI_MARK_TCORNER, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
    }

    //Simple
    else if(id == 3)
    {
        SetMarkerStyle(wxSCI_MARKNUM_FOLDEROPEN, wxSCI_MARK_MINUS, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
		SetMarkerStyle(wxSCI_MARKNUM_FOLDER, wxSCI_MARK_PLUS, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
		SetMarkerStyle(wxSCI_MARKNUM_FOLDERSUB, wxSCI_MARK_BACKGROUND, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
		SetMarkerStyle(wxSCI_MARKNUM_FOLDERTAIL, wxSCI_MARK_BACKGROUND, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
		SetMarkerStyle(wxSCI_MARKNUM_FOLDEREND, wxSCI_MARK_PLUS, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
		SetMarkerStyle(wxSCI_MARKNUM_FOLDEROPENMID, wxSCI_MARK_MINUS, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
		SetMarkerStyle(wxSCI_MARKNUM_FOLDERMIDTAIL, wxSCI_MARK_BACKGROUND, wxColor(0xff, 0xff, 0xff), wxColor(0x80, 0x80, 0x80));
    }
}

void ScbEditor::FoldBlockFromLine(int line)
{
    if (line == -1)
        line = GetControl()->GetCurrentLine();
    DoFoldBlockFromLine(line, 1);
}

void ScbEditor::UnfoldBlockFromLine(int line)
{
    if (line == -1)
        line = GetControl()->GetCurrentLine();
    DoFoldBlockFromLine(line, 0);
}

void ScbEditor::ToggleFoldBlockFromLine(int line)
{
    if (line == -1)
        line = GetControl()->GetCurrentLine();
    DoFoldBlockFromLine(line, 2);
}

void ScbEditor::GotoLine(int line, bool centerOnScreen)
{
    cbStyledTextCtrl* control = GetControl();

    // Make sure the line is not folded. This is done before moving to that
    // line because folding may change the lines layout.
    control->EnsureVisible(line);

    // If the line or the following is a fold point it will be unfolded, in this way
    // when the line is a function declaration (or only contains the opening brace of it [yes, that happens sometimes] )
    // the body is shown.
    DoFoldLine(line,0);
    DoFoldLine(line+1,0);

    if (centerOnScreen)
    {
        int onScreen = control->LinesOnScreen() >> 1;
        control->GotoLine(line - onScreen);
        control->GotoLine(line + onScreen);
    }
    control->GotoLine(line);
}

bool ScbEditor::AddBreakpoint(int line, bool notifyDebugger)
{
    if (HasBreakpoint(line))
        return false;
    if (line == -1)
        line = GetControl()->GetCurrentLine();

    if (!notifyDebugger)
    {
        MarkerToggle(BREAKPOINT_MARKER, line);
        return false;
    }

    // Notify all debugger plugins
    PluginsArray arr = Manager::Get()->GetPluginManager()->GetOffersFor(ptDebugger);
    if (!arr.GetCount())
        return false;
    bool accepted=false;
    for(size_t i=0;i<arr.GetCount();i++)
    {
        cbDebuggerPlugin* debugger = (cbDebuggerPlugin*)arr[i];
        if (!debugger)
            continue; //kinda scary if this isn't a debugger? perhaps this should be a logged error??
        if (debugger->AddBreakpoint(m_Filename, line))
        {
            accepted=true;
        }
    }
    // If at least one breakpoint changed, return true
    // (could still cause problems if one debugger previously responded to add but another
    // now responds to remove of that bp - hopefully the debuggers are coded sufficiently well
    // that this doesn't happen)
    if(accepted)
    {
        MarkerToggle(BREAKPOINT_MARKER, line);
        return true;
    }
    return false;
}

bool ScbEditor::RemoveBreakpoint(int line, bool notifyDebugger)
{
    if (!HasBreakpoint(line))
        return false;
    if (line == -1)
        line = GetControl()->GetCurrentLine();

    if (!notifyDebugger)
    {
        MarkerToggle(BREAKPOINT_MARKER, line);
        return false;
    }

    PluginsArray arr = Manager::Get()->GetPluginManager()->GetOffersFor(ptDebugger);
    if (!arr.GetCount())
        return false;
    bool accepted=false;
    for(size_t i=0;i<arr.GetCount();i++)
    {
        cbDebuggerPlugin* debugger = (cbDebuggerPlugin*)arr[i];
        if (!debugger)
            continue; //kinda scary if this isn't a debugger? perhaps this should be a logged error??
        if (debugger->RemoveBreakpoint(m_Filename, line))
        {
            accepted=true;
        }
    }
    if(accepted)
    {
        MarkerToggle(BREAKPOINT_MARKER, line);
        return true;
    }
    return false;
}

void ScbEditor::ToggleBreakpoint(int line, bool notifyDebugger)
{
    if (line == -1)
        line = GetControl()->GetCurrentLine();
    if (!notifyDebugger)
    {
        MarkerToggle(BREAKPOINT_MARKER, line);
        return;
    }

    PluginsArray arr = Manager::Get()->GetPluginManager()->GetOffersFor(ptDebugger);
    if (!arr.GetCount())
        return;
    bool toggle=false;
    for(size_t i=0;i<arr.GetCount();i++)
    {
        cbDebuggerPlugin* debugger = (cbDebuggerPlugin*)arr[i];
        if (HasBreakpoint(line))
        {
            if (debugger->RemoveBreakpoint(m_Filename, line))
                toggle=true;
        }
        else
        {
            if (debugger->AddBreakpoint(m_Filename, line))
                toggle=true;
        }
    }
    if(toggle)
        MarkerToggle(BREAKPOINT_MARKER, line);
}

bool ScbEditor::HasBreakpoint(int line) const
{
    if (line == -1)
        line = GetControl()->GetCurrentLine();
    return LineHasMarker(BREAKPOINT_MARKER, line);
}

void ScbEditor::GotoNextBreakpoint()
{
    MarkerNext(BREAKPOINT_MARKER);
}

void ScbEditor::GotoPreviousBreakpoint()
{
    MarkerPrevious(BREAKPOINT_MARKER);
}

void ScbEditor::ToggleBookmark(int line)
{
    MarkerToggle(BOOKMARK_MARKER, line);
}

bool ScbEditor::HasBookmark(int line) const
{
    return LineHasMarker(BOOKMARK_MARKER, line);
}

void ScbEditor::GotoNextBookmark()
{
    MarkerNext(BOOKMARK_MARKER);
}

void ScbEditor::GotoPreviousBookmark()
{
    MarkerPrevious(BOOKMARK_MARKER);
}

void ScbEditor::SetDebugLine(int line)
{
    MarkLine(DEBUG_MARKER, line);
    m_pData->m_LastDebugLine = line;
}

void ScbEditor::SetErrorLine(int line)
{
    MarkLine(ERROR_MARKER, line);
}

void ScbEditor::Undo()
{
    cbAssert(GetControl());
    GetControl()->Undo();
}

void ScbEditor::Redo()
{
    cbAssert(GetControl());
    GetControl()->Redo();
}

void ScbEditor::Cut()
{
    cbAssert(GetControl());
    GetControl()->Cut();
}

void ScbEditor::Copy()
{
    cbAssert(GetControl());
    GetControl()->Copy();
}

void ScbEditor::Paste()
{
    cbAssert(GetControl());
    GetControl()->Paste();
}

bool ScbEditor::CanUndo() const
{
    cbAssert(GetControl());
    return !IsReadOnly() && GetControl()->CanUndo();
}

bool ScbEditor::CanRedo() const
{
    cbAssert(GetControl());
    return !IsReadOnly() && GetControl()->CanRedo();
}

bool ScbEditor::HasSelection() const
{
    cbAssert(GetControl());
    cbStyledTextCtrl* control = GetControl();
    return control->GetSelectionStart() != control->GetSelectionEnd();
}

bool ScbEditor::CanPaste() const
{
    cbAssert(GetControl());
    if(platform::gtk)
        return !IsReadOnly();

    return GetControl()->CanPaste() && !IsReadOnly();
}

bool ScbEditor::IsReadOnly() const
{
    cbAssert(GetControl());
    return GetControl()->GetReadOnly();
}

bool ScbEditor::LineHasMarker(int marker, int line) const
{
    if (line == -1)
        line = GetControl()->GetCurrentLine();
    return m_pControl->MarkerGet(line) & (1 << marker);
}

void ScbEditor::MarkerToggle(int marker, int line)
{
    if (line == -1)
        line = GetControl()->GetCurrentLine();
    if (LineHasMarker(marker, line))
        GetControl()->MarkerDelete(line, marker);
    else
        GetControl()->MarkerAdd(line, marker);
}

void ScbEditor::MarkerNext(int marker)
{
    int line = GetControl()->GetCurrentLine() + 1;
    int newLine = GetControl()->MarkerNext(line, 1 << marker);
    if (newLine != -1)
        GotoLine(newLine);
}

void ScbEditor::MarkerPrevious(int marker)
{
    int line = GetControl()->GetCurrentLine() - 1;
    int newLine = GetControl()->MarkerPrevious(line, 1 << marker);
    if (newLine != -1)
        GotoLine(newLine);
}

void ScbEditor::MarkLine(int marker, int line)
{
    if (line == -1)
        GetControl()->MarkerDeleteAll(marker);
    else
        GetControl()->MarkerAdd(line, marker);
}

void ScbEditor::GotoMatchingBrace()
{
    cbStyledTextCtrl* control = GetControl();

    // this works only when the caret is *before* the brace
    int matchingBrace = control->BraceMatch(control->GetCurrentPos());

    // if we haven't found it, we 'll search at pos-1 too
    if(matchingBrace == wxSCI_INVALID_POSITION)
        matchingBrace = control->BraceMatch(control->GetCurrentPos() - 1);

    // now, we either found it or not
    if(matchingBrace != wxSCI_INVALID_POSITION)
        control->GotoPos(matchingBrace);
}

void ScbEditor::HighlightBraces()
{
    cbStyledTextCtrl* control = GetControl();

    ////// BRACES HIGHLIGHTING ///////
    int currPos = control->GetCurrentPos();
    int newPos = control->BraceMatch(currPos);
    if (newPos == wxSCI_INVALID_POSITION)
    {
        if(currPos > 0)
        {
            currPos--;
        }
        newPos = control->BraceMatch(currPos);
    }
    wxChar ch = control->GetCharAt(currPos);
    if (ch == _T('{') || ch == _T('[') || ch == _T('(') ||
        ch == _T('}') || ch == _T(']') || ch == _T(')'))
    {
        if (newPos != wxSCI_INVALID_POSITION)
        {
            control->BraceHighlight(currPos, newPos);
            const int currColum = control->GetColumn(currPos);
            const int newColum = control->GetColumn(newPos);
            control->SetHighlightGuide((currColum < newColum) ? currColum :newColum);
        }
        else
        {
            control->BraceBadLight(currPos);
        }
    }
    else
    {
        control->BraceHighlight(-1, -1);
    }
}

int ScbEditor::GetLineIndentInSpaces(int line) const
{
    cbStyledTextCtrl* control = GetControl();
    int currLine = (line == -1)
                    ? control->LineFromPosition(control->GetCurrentPos())
                    : line;
    wxString text = control->GetLine(currLine);
    unsigned int len = text.Length();
    int spaceCount = 0;
    for (unsigned int i = 0; i < len; ++i)
    {
        if (text[i] == _T(' '))
        {
            ++spaceCount;
        }
        else if (text[i] == _T('\t'))
        {
            spaceCount += control->GetTabWidth();
        }
        else
        {
            break;
        }
    }
    return spaceCount;
}

wxString ScbEditor::GetLineIndentString(int line) const
{
    cbStyledTextCtrl* control = GetControl();
    int currLine = (line == -1)
                    ? control->LineFromPosition(control->GetCurrentPos())
                    : line;
    wxString text = control->GetLine(currLine);
    unsigned int len = text.Length();
    wxString indent;
    for (unsigned int i = 0; i < len; ++i)
    {
        if (text[i] == _T(' ') || text[i] == _T('\t'))
        {
            indent << text[i];
        }
        else
        {
            break;
        }
    }
    return indent;
}

// Creates a submenu for a Context Menu based on the submenu's specific Id
wxMenu* ScbEditor::CreateContextSubMenu(long id)
{
    cbStyledTextCtrl* control = GetControl();
    wxMenu* menu = 0;
    if(id == idInsert)
    {
        menu = new wxMenu;
        menu->Append(idEmptyMenu, _("Empty"));
        menu->Enable(idEmptyMenu, false);
    }
    else if(id == idEdit)
    {
        menu = new wxMenu;
        menu->Append(idUndo, _("Undo"));
        menu->Append(idRedo, _("Redo"));
        menu->AppendSeparator();
        menu->Append(idCut, _("Cut"));
        menu->Append(idCopy, _("Copy"));
        menu->Append(idPaste, _("Paste"));
        menu->Append(idDelete, _("Delete"));
        menu->AppendSeparator();
        menu->Append(idUpperCase, _("UPPERCASE"));
        menu->Append(idLowerCase, _("lowercase"));
        menu->AppendSeparator();
        menu->Append(idSelectAll, _("Select All"));

        bool hasSel = control->GetSelectionEnd() - control->GetSelectionStart() != 0;

        menu->Enable(idUndo, control->CanUndo());
        menu->Enable(idRedo, control->CanRedo());
        menu->Enable(idCut, !control->GetReadOnly() && hasSel);
        menu->Enable(idCopy, hasSel);

        if(platform::gtk) // a wxGTK bug causes the triggering of unexpected events
            menu->Enable(idPaste, !control->GetReadOnly());
        else
            menu->Enable(idPaste, !control->GetReadOnly() && control->CanPaste());

        menu->Enable(idDelete, !control->GetReadOnly() && hasSel);
        menu->Enable(idUpperCase, !control->GetReadOnly() && hasSel);
        menu->Enable(idLowerCase, !control->GetReadOnly() && hasSel);
    }
    else if(id == idBookmarks)
    {
        menu = new wxMenu;
        menu->Append(idBookmarksToggle, _("Toggle bookmark"));
        menu->Append(idBookmarksPrevious, _("Previous bookmark"));
        menu->Append(idBookmarksNext, _("Next bookmark"));
    }
    else if(id == idFolding)
    {
        menu = new wxMenu;
        menu->Append(idFoldingFoldAll, _("Fold all"));
        menu->Append(idFoldingUnfoldAll, _("Unfold all"));
        menu->Append(idFoldingToggleAll, _("Toggle all folds"));
        menu->AppendSeparator();
        menu->Append(idFoldingFoldCurrent, _("Fold current block"));
        menu->Append(idFoldingUnfoldCurrent, _("Unfold current block"));
        menu->Append(idFoldingToggleCurrent, _("Toggle current block"));
    }
    else
        menu = SEditorBase::CreateContextSubMenu(id);

    return menu;
}

// Adds menu items to context menu (both before and after loading plugins' items)
void ScbEditor::AddToContextMenu(wxMenu* popup,ModuleType type,bool pluginsdone)
{
    bool noeditor = (type != mtEditorManager);
    if(!pluginsdone)
    {
        wxMenu *bookmarks = 0, *folding = 0, *editsubmenu = 0, *insert = 0;
        if(!noeditor)
        {
            insert = CreateContextSubMenu(idInsert);
            editsubmenu = CreateContextSubMenu(idEdit);
            bookmarks = CreateContextSubMenu(idBookmarks);
            folding = CreateContextSubMenu(idFolding);
        }
        if(insert)
        {
            popup->Append(idInsert, _("Insert"), insert);
            popup->AppendSeparator();
        }
        popup->Append(idSwapHeaderSource, _("Swap header/source"));
        if(!noeditor)
            popup->AppendSeparator();

        if(editsubmenu)
            popup->Append(idEdit, _("Edit"), editsubmenu);
        if(bookmarks)
            popup->Append(idBookmarks, _("Bookmarks"), bookmarks);
        if(folding)
            popup->Append(idFolding, _("Folding"), folding);
    }
    else
    {
        wxMenu* splitMenu = new wxMenu;
        splitMenu->Append(idSplitHorz, _("Horizontally"));
        splitMenu->Append(idSplitVert, _("Vertically"));
        splitMenu->AppendSeparator();
        splitMenu->Append(idUnsplit, _("Unsplit"));
        // enable/disable entries accordingly
        bool isSplitHorz = m_pSplitter && m_pSplitter->GetSplitMode() == wxSPLIT_HORIZONTAL;
        bool isSplitVert = m_pSplitter && m_pSplitter->GetSplitMode() == wxSPLIT_VERTICAL;
        splitMenu->Enable(idSplitHorz, !isSplitHorz);
        splitMenu->Enable(idSplitVert, !isSplitVert);
        splitMenu->Enable(idUnsplit, isSplitHorz || isSplitVert);
        popup->Append(idSplit, _("Split view"), splitMenu);

        if(!noeditor)
            popup->Append(idConfigureEditor, _("Configure editor..."));
        popup->Append(idProperties, _("Properties..."));

        // remove "Insert/Empty" if more than one entry
        wxMenu* insert = 0;
        wxMenuItem* insertitem = popup->FindItem(idInsert);
        if(insertitem)
            insert = insertitem->GetSubMenu();
        if(insert)
        {
            if (insert->GetMenuItemCount() > 1)
                insert->Delete(idEmptyMenu);
        }
    }
}

bool ScbEditor::OnBeforeBuildContextMenu(const wxPoint& position, ModuleType type)
{
    bool noeditor = (type != mtEditorManager);
    if (!noeditor && position!=wxDefaultPosition)
    {
        // right mouse click inside the editor

        // because here the focus has not switched yet (i.e. the left control has the focus,
        // but the user right-clicked inside the right control), we find out the active control differently...
        wxPoint clientpos(ScreenToClient(position));
        const int margin = m_pControl->GetMarginWidth(0) + // numbers, if present
                           m_pControl->GetMarginWidth(1) + // breakpoints, bookmarks... if present
                           m_pControl->GetMarginWidth(2);  // folding, if present
        wxRect r = m_pControl->GetRect();

        bool inside1 = r.Contains(clientpos);

        cbStyledTextCtrl* control = !m_pControl2 || inside1 ? m_pControl : m_pControl2;
//        control->SetFocus();

        clientpos = control->ScreenToClient(position);
        if (clientpos.x < margin)
        {
            // keep the line
            int pos = control->PositionFromPoint(clientpos);
            m_pData->m_LastMarginMenuLine = control->LineFromPosition(pos);

            // create special menu
            wxMenu* popup = new wxMenu;

            if (LineHasMarker(BREAKPOINT_MARKER, m_pData->m_LastMarginMenuLine))
            {
                popup->Append(idBreakpointEdit, _("Edit breakpoint"));
                popup->Append(idBreakpointRemove, _("Remove breakpoint"));
            }
            else
            {
                popup->Append(idBreakpointAdd, _("Add breakpoint"));
            }

            popup->AppendSeparator();

            if (LineHasMarker(BOOKMARK_MARKER, m_pData->m_LastMarginMenuLine))
            {
                popup->Append(idBookmarkRemove, _("Remove bookmark"));
            }
            else
            {
                popup->Append(idBookmarkAdd, _("Add bookmark"));
            }

            // display menu... wxWindows help says not to force the position
            PopupMenu(popup);

            delete popup;
            return false;
        }

        // before the context menu creation, move the caret to where mouse is

        // get caret position and line from mouse cursor
        const int pos = control->PositionFromPoint(control->ScreenToClient(wxGetMousePosition()));

        // this re-enables 1-click "Find declaration of..."
        // but avoids losing selection for cut/copy
        if(control->GetSelectionStart() > pos ||
           control->GetSelectionEnd() < pos)
        {
            control->GotoPos(pos);
        }
    }

    // follow default strategy
    return SEditorBase::OnBeforeBuildContextMenu(position, type);
}

void ScbEditor::OnAfterBuildContextMenu(ModuleType type)
{
    // we don't care
}

void ScbEditor::Print(bool selectionOnly, PrintColourMode pcm, bool line_numbers)
{
    // print line numbers?
    m_pControl->SetMarginType(0, wxSCI_MARGIN_NUMBER);
    if (!line_numbers)
    {
        m_pControl->SetPrintMagnification(-1);
        m_pControl->SetMarginWidth(0, 0);
    }
    else
    {
        m_pControl->SetPrintMagnification(-2);
        m_pControl->SetMarginWidth(0, 1);
    }
    // never print the gutter line
    m_pControl->SetEdgeMode(wxSCI_EDGE_NONE);

    switch (pcm)
    {
        case pcmAsIs:
            m_pControl->SetPrintColourMode(wxSCI_PRINT_NORMAL);
            break;
        case pcmBlackAndWhite:
            m_pControl->SetPrintColourMode(wxSCI_PRINT_BLACKONWHITE);
            break;
        case pcmColourOnWhite:
            m_pControl->SetPrintColourMode(wxSCI_PRINT_COLOURONWHITE);
            break;
        case pcmInvertColours:
            m_pControl->SetPrintColourMode(wxSCI_PRINT_INVERTLIGHT);
            break;
    }
    InitPrinting();
    wxPrintout* printout = new cbEditorPrintout(m_Filename, m_pControl, selectionOnly);
    if (!g_printer->Print(this, printout, true))
    {
        if (wxPrinter::GetLastError() == wxPRINTER_ERROR)
        {
            cbMessageBox(_("There was a problem printing.\n"
                            "Perhaps your current printer is not set correctly?"), _("Printing"), wxICON_ERROR);
            DeInitPrinting();
        }
    }
    else
    {
        wxPrintData* ppd = &(g_printer->GetPrintDialogData().GetPrintData());
        Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/printerdialog/paperid"), (int)ppd->GetPaperId());
        Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/printerdialog/paperorientation"), (int)ppd->GetOrientation());
    }
    delete printout;

    // revert line numbers and gutter settings
    ConfigManager* mgr = Manager::Get()->GetConfigManager(_T("editor"));
    if (mgr->ReadBool(_T("/show_line_numbers"), true))
        m_pControl->SetMarginWidth(0, 48);
    else
        m_pControl->SetMarginWidth(0, 0);
    m_pControl->SetEdgeMode(mgr->ReadInt(_T("/gutter/mode"), 0));
}

// events

void ScbEditor::OnContextMenuEntry(wxCommandEvent& event)
{
    cbStyledTextCtrl* control = GetControl();

    // we have a single event handler for all popup menu entries,
    // so that we can add/remove options without the need to recompile
    // the whole project (almost) but more importantly, to
    // *not* break ScbEditor's interface for such a trivial task...
    const int id = event.GetId();

    if (id == idUndo)
        control->Undo();
    else if (id == idRedo)
        control->Redo();
    else if (id == idCut)
        control->Cut();
    else if (id == idCopy)
        control->Copy();
    else if (id == idPaste)
        control->Paste();
    else if (id == idDelete)
        control->ReplaceSelection(wxEmptyString);
    else if (id == idUpperCase)
        control->UpperCase();
    else if (id == idLowerCase)
        control->LowerCase();
    else if (id == idSelectAll)
        control->SelectAll();
    else if (id == idSwapHeaderSource)
        GetEditorManager()->SwapActiveHeaderSource();
    else if (id == idBookmarkAdd)
        control->MarkerAdd(m_pData->m_LastMarginMenuLine, BOOKMARK_MARKER);
    else if (id == idBookmarkRemove)
        control->MarkerDelete(m_pData->m_LastMarginMenuLine, BOOKMARK_MARKER);
    else if (id == idBookmarksToggle)
        MarkerToggle(BOOKMARK_MARKER);
    else if (id == idBookmarksNext)
        MarkerNext(BOOKMARK_MARKER);
    else if (id == idBookmarksPrevious)
        MarkerPrevious(BOOKMARK_MARKER);
    else if (id == idFoldingFoldAll)
        FoldAll();
    else if (id == idFoldingUnfoldAll)
        UnfoldAll();
    else if (id == idFoldingToggleAll)
        ToggleAllFolds();
    else if (id == idFoldingFoldCurrent)
        FoldBlockFromLine();
    else if (id == idFoldingUnfoldCurrent)
        UnfoldBlockFromLine();
    else if (id == idFoldingToggleCurrent)
        ToggleFoldBlockFromLine();
    else if (id == idSplitHorz)
        Split(stHorizontal);
    else if (id == idSplitVert)
        Split(stVertical);
    else if (id == idUnsplit)
        Unsplit();
    else if (id == idConfigureEditor)
        ;//GetEditorManager()->Configure(); //(pecan 2008/5/06)
    else if (id == idProperties)
    {
        if (m_pProjectFile)
            m_pProjectFile->ShowOptions(this);
        else
        {
            // active editor not-in-project
            ProjectFileOptionsDlg dlg(this, GetFilename());
            PlaceWindow(&dlg);
            dlg.ShowModal();
        }
    }
    else if (id == idBreakpointAdd)
        AddBreakpoint(m_pData->m_LastMarginMenuLine);
    else if (id == idBreakpointEdit)
        NotifyPlugins(cbEVT_EDITOR_BREAKPOINT_EDIT, m_pData->m_LastMarginMenuLine, m_Filename);
    else if (id == idBreakpointRemove)
        RemoveBreakpoint(m_pData->m_LastMarginMenuLine);
    else
        event.Skip();
    //Manager::Get()->GetLogManager()->DebugLog(_T("Leaving OnContextMenuEntry"));
}

void ScbEditor::OnMarginClick(wxScintillaEvent& event)
{
    switch (event.GetMargin())
    {
        case 1: // bookmarks and breakpoints margin
        {
            int lineYpix = event.GetPosition();
            int line = GetControl()->LineFromPosition(lineYpix);

            ToggleBreakpoint(line);
            break;
        }
        case 2: // folding margin
        {
            int lineYpix = event.GetPosition();
            int line = GetControl()->LineFromPosition(lineYpix);

            GetControl()->ToggleFold(line);
            break;
        }
    }
    OnScintillaEvent(event);
}

void ScbEditor::OnEditorUpdateUI(wxScintillaEvent& event)
{
    if (GetEditorManager()->GetActiveEditor() == this)
    {
        NotifyPlugins(cbEVT_EDITOR_UPDATE_UI);
        HighlightBraces(); // brace highlighting
    }
    OnScintillaEvent(event);
}

void ScbEditor::OnEditorChange(wxScintillaEvent& event)
{
    SetModified(m_pControl->GetModify());
    OnScintillaEvent(event);
}

void ScbEditor::OnEditorCharAdded(wxScintillaEvent& event)
{
    // if message manager is auto-hiding, this will close it if not needed open
//    Manager::Get()->GetLogManager()->Close();

    cbStyledTextCtrl* control = GetControl();
    int pos = control->GetCurrentPos();
    wxChar ch = event.GetKey();

    // indent
    if (ch == _T('\n'))
    {
        control->BeginUndoAction();
        // new-line: adjust indentation
        bool autoIndent = Manager::Get()->GetConfigManager(_T("editor"))->ReadBool(_T("/auto_indent"), true);
        bool smartIndent = Manager::Get()->GetConfigManager(_T("editor"))->ReadBool(_T("/smart_indent"), true);
        int currLine = control->LineFromPosition(pos);
        if (autoIndent && currLine > 0)
        {
            wxString indent = GetLineIndentString(currLine - 1);
            if (smartIndent)
            {
                cbStyledTextCtrl* stc = GetControl();
                 // if the last entered char before newline was an opening curly brace,
                 // increase indentation level (the closing brace is handled in another block)

                // SMART INDENTING - THIS IS LANGUAGE SPECIFIC, BUT CURRENTLY ONLY IMPLEMENTED FOR C/C++ AND PYTHON
                wxChar b = m_pData->GetLastNonWhitespaceChar();
                switch(stc->GetLexer())
                {
                    case wxSCI_LEX_CPP:
                        if (b == _T('{'))
                        {
                            if(control->GetUseTabs())
                                indent << _T('\t'); // 1 tab
                            else
                                indent << wxString(_T(' '), control->GetTabWidth()); // n spaces
                        }
                        break;
                    case wxSCI_LEX_PYTHON:
                        if (b == _T(':'))
                        {
                            if(control->GetUseTabs())
                                indent << _T('\t'); // 1 tab
                            else
                                indent << wxString(_T(' '), control->GetTabWidth()); // n spaces
                        }
                        break;
                 }
            }
            control->InsertText(pos, indent);
            control->GotoPos(pos + indent.Length());
            control->ChooseCaretX();
        }
        control->EndUndoAction();
    }

    // unindent
    else if (ch == _T('}'))
    {
        bool smartIndent = Manager::Get()->GetConfigManager(_T("editor"))->ReadBool(_T("/smart_indent"), true);
        if ( smartIndent && ( (control->GetLexer() == wxSCI_LEX_CPP) || (control->GetLexer() == wxSCI_LEX_D) ) )
        {
            control->BeginUndoAction();
            // undo block indentation, if needed
            wxString str = control->GetLine(control->GetCurrentLine());
            str.Trim(false);
            str.Trim(true);
            if (str.Matches(_T("}")))
            {
                // just the brace here; unindent
                // find opening brace (skipping nested blocks)
                int pos = control->GetCurrentPos() - 2;
                pos = m_pData->FindBlockStart(pos, _T('{'), _T('}'));
                if (pos != -1)
                {
                    wxString indent = GetLineIndentString(control->LineFromPosition(pos));
                    indent << _T('}');
                    control->DelLineLeft();
                    control->DelLineRight();
                    pos = control->GetCurrentPos();
                    control->InsertText(pos, indent);
                    control->GotoPos(pos + indent.Length());
                    control->ChooseCaretX();
                }
            }
            control->EndUndoAction();
        }
    }

    OnScintillaEvent(event);
}

void ScbEditor::OnEditorDwellStart(wxScintillaEvent& event)
{
    cbStyledTextCtrl* control = GetControl();
    int pos = control->PositionFromPoint(wxPoint(event.GetX(), event.GetY()));
    int style = control->GetStyleAt(pos);
    NotifyPlugins(cbEVT_EDITOR_TOOLTIP, style, wxEmptyString, event.GetX(), event.GetY());
    OnScintillaEvent(event);
}

void ScbEditor::OnEditorDwellEnd(wxScintillaEvent& event)
{
    NotifyPlugins(cbEVT_EDITOR_TOOLTIP_CANCEL);
    OnScintillaEvent(event);
}

void ScbEditor::OnEditorModified(wxScintillaEvent& event)
{
//    wxString txt = _T("OnEditorModified(): ");
//    int flags = event.GetModificationType();
//    if (flags & wxSCI_MOD_CHANGEMARKER) txt << _T("wxSCI_MOD_CHANGEMARKER, ");
//    if (flags & wxSCI_MOD_INSERTTEXT) txt << _T("wxSCI_MOD_INSERTTEXT, ");
//    if (flags & wxSCI_MOD_DELETETEXT) txt << _T("wxSCI_MOD_DELETETEXT, ");
//    if (flags & wxSCI_MOD_CHANGEFOLD) txt << _T("wxSCI_MOD_CHANGEFOLD, ");
//    if (flags & wxSCI_PERFORMED_USER) txt << _T("wxSCI_PERFORMED_USER, ");
//    if (flags & wxSCI_MOD_BEFOREINSERT) txt << _T("wxSCI_MOD_BEFOREINSERT, ");
//    if (flags & wxSCI_MOD_BEFOREDELETE) txt << _T("wxSCI_MOD_BEFOREDELETE, ");
//    txt << _T("pos=")
//        << wxString::Format(_T("%d"), event.GetPosition())
//        << _T(", line=")
//        << wxString::Format(_T("%d"), event.GetLine())
//        << _T(", linesAdded=")
//        << wxString::Format(_T("%d"), event.GetLinesAdded());
//    Manager::Get()->GetLogManager()->DebugLog(txt);

    ////// whenever event.GetLinesAdded() != 0, we must re-set breakpoints for lines greater
    ////// than LineFromPosition(event.GetPosition())
    ////int linesAdded = event.GetLinesAdded();
    ////bool isAdd = event.GetModificationType() & wxSCI_MOD_INSERTTEXT;
    ////bool isDel = event.GetModificationType() & wxSCI_MOD_DELETETEXT;
    ////if ((isAdd || isDel) && linesAdded != 0)
    ////{
    ////    // in case of no line numbers to be shown no need to set
    ////    // NOTE : on every modification of the Editor we consult ConfigManager
    ////    //        hopefully not to time consuming, otherwise we make a member out of it
    ////    ConfigManager* mgr = Manager::Get()->GetConfigManager(_T("editor"));
    ////    if (mgr->ReadBool(_T("/show_line_numbers"), true))
    ////    {
    ////        m_pData->SetLineNumberColWidth();
    ////    }
    ////
    ////    // NB: I don't think polling for each debugger every time will slow things down enough
    ////    // to worry about unless there are automated tasks that call this routine regularly
    ////    //
    ////    // well, scintilla events happen regularly
    ////    // although we only reach this part of the code only if a line has been added/removed
    ////    // so, yes, it might not be that bad after all
    ////    PluginsArray arr = Manager::Get()->GetPluginManager()->GetOffersFor(ptDebugger);
    ////    int startline = m_pControl->LineFromPosition(event.GetPosition());
    ////    for(size_t i=0;i<arr.GetCount();i++)
    ////    {
    ////        cbDebuggerPlugin* debugger = (cbDebuggerPlugin*)arr[i];
    ////        debugger->EditorLinesAddedOrRemoved((cbEditor*)this, startline, linesAdded);
    ////    }
    ////
    ////}

    OnScintillaEvent(event);
} // end of OnEditorModified

void ScbEditor::OnUserListSelection(wxScintillaEvent& event)
{
    OnScintillaEvent(event);
}

void ScbEditor::OnClose(wxCloseEvent& event)
{
    GetEditorManager()->Close(this);
}

void ScbEditor::DoIndent()
{
    cbStyledTextCtrl* control = GetControl();
    if (control)
        control->SendMsg(wxSCI_CMD_TAB);
}

void ScbEditor::DoUnIndent()
{
    cbStyledTextCtrl* control = GetControl();
    if (control)
        control->SendMsg(wxSCI_CMD_BACKTAB);
}

void ScbEditor::OnZoom(wxScintillaEvent& event)
{
    GetEditorManager()->SetZoom(GetControl()->GetZoom());
    OnScintillaEvent(event);
}

// generic scintilla event handler
void ScbEditor::OnScintillaEvent(wxScintillaEvent& event)
{
//	wxString txt;
//    wxEventType type = event.GetEventType();
//	if (type == wxEVT_SCI_CHANGE) txt << _T("wxEVT_SCI_CHANGE");
//	else if (type == wxEVT_SCI_STYLENEEDED) txt << _T("wxEVT_SCI_STYLENEEDED");
//	else if (type == wxEVT_SCI_CHARADDED) txt << _T("wxEVT_SCI_CHARADDED");
//	else if (type == wxEVT_SCI_SAVEPOINTREACHED) txt << _T("wxEVT_SCI_SAVEPOINTREACHED");
//	else if (type == wxEVT_SCI_SAVEPOINTLEFT) txt << _T("wxEVT_SCI_SAVEPOINTLEFT");
//	else if (type == wxEVT_SCI_ROMODIFYATTEMPT) txt << _T("wxEVT_SCI_ROMODIFYATTEMPT");
//	else if (type == wxEVT_SCI_KEY) txt << _T("wxEVT_SCI_KEY");
//	else if (type == wxEVT_SCI_DOUBLECLICK) txt << _T("wxEVT_SCI_DOUBLECLICK");
//	else if (type == wxEVT_SCI_UPDATEUI) txt << _T("wxEVT_SCI_UPDATEUI");
//	else if (type == wxEVT_SCI_MODIFIED) txt << _T("wxEVT_SCI_MODIFIED");
//	else if (type == wxEVT_SCI_MACRORECORD) txt << _T("wxEVT_SCI_MACRORECORD");
//	else if (type == wxEVT_SCI_MARGINCLICK) txt << _T("wxEVT_SCI_MARGINCLICK");
//	else if (type == wxEVT_SCI_NEEDSHOWN) txt << _T("wxEVT_SCI_NEEDSHOWN");
//	else if (type == wxEVT_SCI_PAINTED) txt << _T("wxEVT_SCI_PAINTED");
//	else if (type == wxEVT_SCI_USERLISTSELECTION) txt << _T("wxEVT_SCI_USERLISTSELECTION");
//	else if (type == wxEVT_SCI_URIDROPPED) txt << _T("wxEVT_SCI_URIDROPPED");
//	else if (type == wxEVT_SCI_DWELLSTART) txt << _T("wxEVT_SCI_DWELLSTART");
//	else if (type == wxEVT_SCI_DWELLEND) txt << _T("wxEVT_SCI_DWELLEND");
//	else if (type == wxEVT_SCI_START_DRAG) txt << _T("wxEVT_SCI_START_DRAG");
//	else if (type == wxEVT_SCI_DRAG_OVER) txt << _T("wxEVT_SCI_DRAG_OVER");
//	else if (type == wxEVT_SCI_DO_DROP) txt << _T("wxEVT_SCI_DO_DROP");
//	else if (type == wxEVT_SCI_ZOOM) txt << _T("wxEVT_SCI_ZOOM");
//	else if (type == wxEVT_SCI_HOTSPOT_CLICK) txt << _T("wxEVT_SCI_HOTSPOT_CLICK");
//	else if (type == wxEVT_SCI_HOTSPOT_DCLICK) txt << _T("wxEVT_SCI_HOTSPOT_DCLICK");
//	else if (type == wxEVT_SCI_CALLTIP_CLICK) txt << _T("wxEVT_SCI_CALLTIP_CLICK");
//	else if (type == wxEVT_SCI_AUTOCOMP_SELECTION) txt << _T("wxEVT_SCI_AUTOCOMP_SELECTION");
//	else if (type == wxEVT_SCI_INDICATOR_CLICK) txt << _T("wxEVT_SCI_INDICATOR_CLICK");
//	else if (type == wxEVT_SCI_INDICATOR_RELEASE) txt << _T("wxEVT_SCI_INDICATOR_RELEASE");
//    Manager::Get()->GetLogManager()->DebugLog(txt);

    // call any hooked functors
    if (!ProjectManager::IsBusy() && EditorHooks::HasRegisteredHooks())
    {
        // Crash in CodeCompletion if next call enabled //(pecan 2008/7/20)
        //-EditorHooks::CallHooks((cbEditor*)this, event);
    }
}
