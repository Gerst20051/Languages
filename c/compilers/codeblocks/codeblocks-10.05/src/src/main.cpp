/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision: 6206 $
 * $Id: main.cpp 6206 2010-05-01 08:37:51Z jenslody $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/src/main.cpp $
 */

#include <sdk.h>
#include "app.h"
#include "main.h"
#include "appglobals.h"
#include "environmentsettingsdlg.h"
#include "compilersettingsdlg.h"
#include <cbworkspace.h>
#include <globals.h>
#include <filefilters.h>
#include <wx/tokenzr.h>

#if defined(_MSC_VER) && defined( _DEBUG )
    #define _CRTDBG_MAP_ALLOC
    #include <stdlib.h>
    #include <crtdbg.h>
#endif

#include <wx/tipdlg.h>
#include <wx/dnd.h>
#include <wx/sstream.h>
#include <wx/fileconf.h>
#include <wx/xrc/xmlres.h>
#include <wx/gauge.h>
#include <configmanager.h>
#include <cbproject.h>
#include <cbplugin.h>
#include <sdk_events.h>
#include <projectmanager.h>
#include <editormanager.h>
#include <logmanager.h>
#include <pluginmanager.h>
#include <templatemanager.h>
#include <toolsmanager.h>
#include <scriptingmanager.h>
#include <cbexception.h>
#include <annoyingdialog.h>
#include <editorcolourset.h>
#include <logmanager.h>
#include <personalitymanager.h>
#include "cbstyledtextctrl.h"

#include "infopane.h"
#include "dlgaboutplugin.h"
#include "dlgabout.h"
#include "startherepage.h"
#include "scriptconsole.h"
#include "scriptingsettingsdlg.h"
#include "printdlg.h"
#include "batchbuild.h"
#include <wx/printdlg.h>
#include <wx/filename.h>
#include "cbauibook.h"

#include "uservarmanager.h"
#include "infowindow.h"
#include "notebookstyles.h"
#include "switcherdlg.h"

class wxMyFileDropTarget : public wxFileDropTarget
{
public:
    wxMyFileDropTarget(MainFrame *frame):m_frame(frame){}
    virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
    {
        if (!m_frame) return false;
        return m_frame->OnDropFiles(x,y,filenames);
    }
private:
    MainFrame* m_frame;
};

const static wxString gDefaultLayout = _T("Code::Blocks default");
static wxString gDefaultLayoutData; // this will keep the "hardcoded" default layout

int wxID_FILE10 = wxNewId();
int wxID_FILE11 = wxNewId();
int wxID_FILE12 = wxNewId();
int wxID_FILE13 = wxNewId();
int wxID_FILE14 = wxNewId();
int wxID_FILE15 = wxNewId();
int wxID_FILE16 = wxNewId();
int wxID_FILE17 = wxNewId();
int wxID_FILE18 = wxNewId();
int wxID_FILE19 = wxNewId();

int idToolNew = XRCID("idToolNew");
int idFileNew = XRCID("idFileNew");
int idFileNewEmpty = XRCID("idFileNewEmpty");
int idFileNewProject = XRCID("idFileNewProject");
int idFileNewTarget = XRCID("idFileNewTarget");
int idFileNewFile = XRCID("idFileNewFile");
int idFileNewCustom = XRCID("idFileNewCustom");
int idFileNewUser = XRCID("idFileNewUser");
int idFileOpen = XRCID("idFileOpen");
int idFileReopen = XRCID("idFileReopen");
int idFileOpenRecentFileClearHistory = XRCID("idFileOpenRecentFileClearHistory");
int idFileOpenRecentProjectClearHistory = XRCID("idFileOpenRecentProjectClearHistory");
int idFileImportProjectDevCpp = XRCID("idFileImportProjectDevCpp");
int idFileImportProjectMSVC = XRCID("idFileImportProjectMSVC");
int idFileImportProjectMSVCWksp = XRCID("idFileImportProjectMSVCWksp");
int idFileImportProjectMSVS = XRCID("idFileImportProjectMSVS");
int idFileImportProjectMSVSWksp = XRCID("idFileImportProjectMSVSWksp");
int idFileSave = XRCID("idFileSave");
int idFileSaveAs = XRCID("idFileSaveAs");
int idFileSaveAllFiles = XRCID("idFileSaveAllFiles");
int idFileSaveProject = XRCID("idFileSaveProject");
int idFileSaveProjectAs = XRCID("idFileSaveProjectAs");
int idFileSaveProjectAllProjects = XRCID("idFileSaveProjectAllProjects");
int idFileSaveProjectTemplate = XRCID("idFileSaveProjectTemplate");
int idFileOpenDefWorkspace = XRCID("idFileOpenDefWorkspace");
int idFileSaveWorkspace = XRCID("idFileSaveWorkspace");
int idFileSaveWorkspaceAs = XRCID("idFileSaveWorkspaceAs");
int idFileSaveAll = XRCID("idFileSaveAll");
int idFileCloseWorkspace = XRCID("idFileCloseWorkspace");
int idFileClose = XRCID("idFileClose");
int idFileCloseAll = XRCID("idFileCloseAll");
int idFileCloseProject = XRCID("idFileCloseProject");
int idFileCloseAllProjects = XRCID("idFileCloseAllProjects");
int idFilePrintSetup = XRCID("idFilePrintSetup");
int idFilePrint = XRCID("idFilePrint");
int idFileExit = XRCID("idFileExit");
int idFileNext = wxNewId();
int idFilePrev = wxNewId();

int idEditUndo = XRCID("idEditUndo");
int idEditRedo = XRCID("idEditRedo");
int idEditClearHistory = XRCID("idEditClearHistory");
int idEditCopy = XRCID("idEditCopy");
int idEditCut = XRCID("idEditCut");
int idEditPaste = XRCID("idEditPaste");
int idEditSwapHeaderSource = XRCID("idEditSwapHeaderSource");
int idEditGotoMatchingBrace = XRCID("idEditGotoMatchingBrace");
int idEditHighlightMode = XRCID("idEditHighlightMode");
int idEditHighlightModeText = XRCID("idEditHighlightModeText");
int idEditBookmarks = XRCID("idEditBookmarks");
int idEditBookmarksToggle = XRCID("idEditBookmarksToggle");
int idEditBookmarksPrevious = XRCID("idEditBookmarksPrevious");
int idEditBookmarksNext = XRCID("idEditBookmarksNext");
int idEditFolding = XRCID("idEditFolding");
int idEditFoldAll = XRCID("idEditFoldAll");
int idEditUnfoldAll = XRCID("idEditUnfoldAll");
int idEditToggleAllFolds = XRCID("idEditToggleAllFolds");
int idEditFoldBlock = XRCID("idEditFoldBlock");
int idEditUnfoldBlock = XRCID("idEditUnfoldBlock");
int idEditToggleFoldBlock = XRCID("idEditToggleFoldBlock");
int idEditEOLMode = XRCID("idEditEOLMode");
int idEditEOLCRLF = XRCID("idEditEOLCRLF");
int idEditEOLCR = XRCID("idEditEOLCR");
int idEditEOLLF = XRCID("idEditEOLLF");
int idEditEncoding = XRCID("idEditEncoding");
int idEditEncodingDefault = XRCID("idEditEncodingDefault");
int idEditEncodingUseBom = XRCID("idEditEncodingUseBom");
int idEditEncodingAscii = XRCID("idEditEncodingAscii");
int idEditEncodingUtf7 = XRCID("idEditEncodingUtf7");
int idEditEncodingUtf8 = XRCID("idEditEncodingUtf8");
int idEditEncodingUnicode = XRCID("idEditEncodingUnicode");
int idEditEncodingUtf16 = XRCID("idEditEncodingUtf16");
int idEditEncodingUtf32 = XRCID("idEditEncodingUtf32");
int idEditEncodingUnicode16BE = XRCID("idEditEncodingUnicode16BE");
int idEditEncodingUnicode16LE = XRCID("idEditEncodingUnicode16LE");
int idEditEncodingUnicode32BE = XRCID("idEditEncodingUnicode32BE");
int idEditEncodingUnicode32LE = XRCID("idEditEncodingUnicode32LE");
int idEditSpecialCommands = XRCID("idEditSpecialCommands");
int idEditSpecialCommandsMovement = XRCID("idEditSpecialCommandsMovement");
int idEditParaUp = XRCID("idEditParaUp");
int idEditParaUpExtend = XRCID("idEditParaUpExtend");
int idEditParaDown = XRCID("idEditParaDown");
int idEditParaDownExtend = XRCID("idEditParaDownExtend");
int idEditWordPartLeft = XRCID("idEditWordPartLeft");
int idEditWordPartLeftExtend = XRCID("idEditWordPartLeftExtend");
int idEditWordPartRight = XRCID("idEditWordPartRight");
int idEditWordPartRightExtend = XRCID("idEditWordPartRightExtend");
int idEditSpecialCommandsZoom = XRCID("idEditSpecialCommandsZoom");
int idEditZoomIn = XRCID("idEditZoomIn");
int idEditZoomOut = XRCID("idEditZoomOut");
int idEditZoomReset = XRCID("idEditZoomReset");
int idEditSpecialCommandsLine = XRCID("idEditSpecialCommandsLine");
int idEditLineCut = XRCID("idEditLineCut");
int idEditLineDelete = XRCID("idEditLineDelete");
int idEditLineDuplicate = XRCID("idEditLineDuplicate");
int idEditLineTranspose = XRCID("idEditLineTranspose");
int idEditLineCopy = XRCID("idEditLineCopy");
int idEditLinePaste = XRCID("idEditLinePaste");
int idEditSpecialCommandsCase = XRCID("idEditSpecialCommandsCase");
int idEditUpperCase = XRCID("idEditUpperCase");
int idEditLowerCase = XRCID("idEditLowerCase");
int idEditSelectAll = XRCID("idEditSelectAll");
int idEditCommentSelected = XRCID("idEditCommentSelected");
int idEditUncommentSelected = XRCID("idEditUncommentSelected");
int idEditToggleCommentSelected = XRCID("idEditToggleCommentSelected");
int idEditStreamCommentSelected = XRCID("idEditStreamCommentSelected");
int idEditBoxCommentSelected = XRCID("idEditBoxCommentSelected");
int idEditAutoComplete = XRCID("idEditAutoComplete");

int idViewLayoutDelete = XRCID("idViewLayoutDelete");
int idViewLayoutSave = XRCID("idViewLayoutSave");
int idViewToolbars = XRCID("idViewToolbars");
int idViewToolMain = XRCID("idViewToolMain");
int idViewManager = XRCID("idViewManager");
int idViewLogManager = XRCID("idViewLogManager");
int idViewStatusbar = XRCID("idViewStatusbar");
int idViewScriptConsole = XRCID("idViewScriptConsole");
int idViewFocusEditor = XRCID("idViewFocusEditor");
int idViewSwitchTabs = XRCID("idViewSwitchTabs");
int idViewFullScreen = XRCID("idViewFullScreen");

int idSearchFind = XRCID("idSearchFind");
int idSearchFindInFiles = XRCID("idSearchFindInFiles");
int idSearchFindNext = XRCID("idSearchFindNext");
int idSearchFindPrevious = XRCID("idSearchFindPrevious");
int idSearchReplace = XRCID("idSearchReplace");
int idSearchReplaceInFiles = XRCID("idSearchReplaceInFiles");
int idSearchGotoLine = XRCID("idSearchGotoLine");
int idSearchGotoNextChanged = XRCID("idSearchGotoNextChanged");
int idSearchGotoPreviousChanged = XRCID("idSearchGotoPreviousChanged");

int idSettingsEnvironment = XRCID("idSettingsEnvironment");
int idSettingsGlobalUserVars = XRCID("idSettingsGlobalUserVars");
int idSettingsEditor = XRCID("idSettingsEditor");
int idSettingsCompilerDebugger = XRCID("idSettingsCompilerDebugger");
int idPluginsManagePlugins = XRCID("idPluginsManagePlugins");
int idSettingsScripting = XRCID("idSettingsScripting");

int idHelpTips = XRCID("idHelpTips");
int idHelpPlugins = XRCID("idHelpPlugins");

int idLeftSash = XRCID("idLeftSash");
int idBottomSash = XRCID("idBottomSash");
int idCloseFullScreen = XRCID("idCloseFullScreen");
int idShiftTab = wxNewId();
int idStartHerePageLink = wxNewId();
int idStartHerePageVarSubst = wxNewId();

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_ERASE_BACKGROUND(MainFrame::OnEraseBackground)
    EVT_SIZE(MainFrame::OnSize)
    EVT_CLOSE(MainFrame::OnApplicationClose)

    EVT_UPDATE_UI(idFileOpenRecentFileClearHistory, MainFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileOpenRecentProjectClearHistory, MainFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileSave, MainFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileSaveAs, MainFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileSaveAllFiles, MainFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileOpenDefWorkspace, MainFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileSaveWorkspace, MainFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileSaveWorkspaceAs, MainFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileCloseWorkspace, MainFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileClose, MainFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileCloseAll, MainFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFilePrintSetup, MainFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFilePrint, MainFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileSaveProject, MainFrame::OnProjectMenuUpdateUI)
    EVT_UPDATE_UI(idFileSaveProjectAs, MainFrame::OnProjectMenuUpdateUI)
    EVT_UPDATE_UI(idFileSaveProjectAllProjects, MainFrame::OnProjectMenuUpdateUI)
    EVT_UPDATE_UI(idFileSaveProjectTemplate, MainFrame::OnProjectMenuUpdateUI)
    EVT_UPDATE_UI(idFileSaveAll, MainFrame::OnProjectMenuUpdateUI)
    EVT_UPDATE_UI(idFileCloseProject, MainFrame::OnProjectMenuUpdateUI)
    EVT_UPDATE_UI(idFileCloseAllProjects, MainFrame::OnProjectMenuUpdateUI)

    EVT_UPDATE_UI(idEditUndo, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditRedo, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditClearHistory, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditCopy, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditCut, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditPaste, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditSwapHeaderSource, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditGotoMatchingBrace, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditFoldAll, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditUnfoldAll, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditToggleAllFolds, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditFoldBlock, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditUnfoldBlock, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditToggleFoldBlock, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditEOLCRLF, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditEOLCR, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditEOLLF, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditEncoding, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditSelectAll, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditBookmarksToggle, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditBookmarksNext, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditBookmarksPrevious, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditCommentSelected, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditAutoComplete, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditUncommentSelected, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditToggleCommentSelected, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditStreamCommentSelected, MainFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditBoxCommentSelected, MainFrame::OnEditMenuUpdateUI)

    EVT_UPDATE_UI(idSearchFind, MainFrame::OnSearchMenuUpdateUI)
    EVT_UPDATE_UI(idSearchFindInFiles, MainFrame::OnSearchMenuUpdateUI)
    EVT_UPDATE_UI(idSearchFindNext, MainFrame::OnSearchMenuUpdateUI)
    EVT_UPDATE_UI(idSearchFindPrevious, MainFrame::OnSearchMenuUpdateUI)
    EVT_UPDATE_UI(idSearchReplace, MainFrame::OnSearchMenuUpdateUI)
    EVT_UPDATE_UI(idSearchReplaceInFiles, MainFrame::OnSearchMenuUpdateUI)
    EVT_UPDATE_UI(idSearchGotoLine, MainFrame::OnSearchMenuUpdateUI)
    EVT_UPDATE_UI(idSearchGotoNextChanged, MainFrame::OnSearchMenuUpdateUI)
    EVT_UPDATE_UI(idSearchGotoPreviousChanged, MainFrame::OnSearchMenuUpdateUI)

    EVT_UPDATE_UI(idViewToolMain, MainFrame::OnViewMenuUpdateUI)
    EVT_UPDATE_UI(idViewLogManager, MainFrame::OnViewMenuUpdateUI)
    EVT_UPDATE_UI(idViewManager, MainFrame::OnViewMenuUpdateUI)
    EVT_UPDATE_UI(idViewStatusbar, MainFrame::OnViewMenuUpdateUI)
    EVT_UPDATE_UI(idViewScriptConsole, MainFrame::OnViewMenuUpdateUI)
    EVT_UPDATE_UI(idViewFocusEditor, MainFrame::OnViewMenuUpdateUI)
    EVT_UPDATE_UI(idViewFullScreen, MainFrame::OnViewMenuUpdateUI)

    EVT_MENU(idFileNewEmpty, MainFrame::OnFileNewWhat)
    EVT_MENU(idFileNewProject, MainFrame::OnFileNewWhat)
    EVT_MENU(idFileNewTarget, MainFrame::OnFileNewWhat)
    EVT_MENU(idFileNewFile, MainFrame::OnFileNewWhat)
    EVT_MENU(idFileNewCustom, MainFrame::OnFileNewWhat)
    EVT_MENU(idFileNewUser, MainFrame::OnFileNewWhat)

    EVT_MENU(idToolNew, MainFrame::OnFileNew)
    EVT_MENU(idFileOpen,  MainFrame::OnFileOpen)
    EVT_MENU(idFileOpenRecentProjectClearHistory, MainFrame::OnFileOpenRecentProjectClearHistory)
    EVT_MENU(idFileOpenRecentFileClearHistory, MainFrame::OnFileOpenRecentClearHistory)
    EVT_MENU_RANGE(wxID_FILE1, wxID_FILE9, MainFrame::OnFileReopen)
    EVT_MENU_RANGE(wxID_FILE10, wxID_FILE19, MainFrame::OnFileReopenProject)
    EVT_MENU(idFileImportProjectDevCpp,  MainFrame::OnFileImportProjectDevCpp)
    EVT_MENU(idFileImportProjectMSVC,  MainFrame::OnFileImportProjectMSVC)
    EVT_MENU(idFileImportProjectMSVCWksp,  MainFrame::OnFileImportProjectMSVCWksp)
    EVT_MENU(idFileImportProjectMSVS,  MainFrame::OnFileImportProjectMSVS)
    EVT_MENU(idFileImportProjectMSVSWksp,  MainFrame::OnFileImportProjectMSVSWksp)
    EVT_MENU(idFileSave,  MainFrame::OnFileSave)
    EVT_MENU(idFileSaveAs,  MainFrame::OnFileSaveAs)
    EVT_MENU(idFileSaveAllFiles,  MainFrame::OnFileSaveAllFiles)
    EVT_MENU(idFileSaveProject,  MainFrame::OnFileSaveProject)
    EVT_MENU(idFileSaveProjectAs,  MainFrame::OnFileSaveProjectAs)
    EVT_MENU(idFileSaveProjectTemplate,  MainFrame::OnFileSaveProjectTemplate)
    EVT_MENU(idFileSaveProjectAllProjects,  MainFrame::OnFileSaveProjectAllProjects)
    EVT_MENU(idFileOpenDefWorkspace,  MainFrame::OnFileOpenDefWorkspace)
    EVT_MENU(idFileSaveWorkspace,  MainFrame::OnFileSaveWorkspace)
    EVT_MENU(idFileSaveWorkspaceAs,  MainFrame::OnFileSaveWorkspaceAs)
    EVT_MENU(idFileSaveAll,  MainFrame::OnFileSaveAll)
    EVT_MENU(idFileCloseWorkspace,  MainFrame::OnFileCloseWorkspace)
    EVT_MENU(idFileClose,  MainFrame::OnFileClose)
    EVT_MENU(idFileCloseAll,  MainFrame::OnFileCloseAll)
    EVT_MENU(idFileCloseProject,  MainFrame::OnFileCloseProject)
    EVT_MENU(idFileCloseAllProjects,  MainFrame::OnFileCloseAllProjects)
    EVT_MENU(idFilePrint,  MainFrame::OnFilePrint)
    EVT_MENU(idFileExit,  MainFrame::OnFileQuit)
    EVT_MENU(idFileNext,  MainFrame::OnFileNext)
    EVT_MENU(idFilePrev,  MainFrame::OnFilePrev)

    EVT_MENU(idEditUndo,  MainFrame::OnEditUndo)
    EVT_MENU(idEditRedo,  MainFrame::OnEditRedo)
    EVT_MENU(idEditClearHistory,  MainFrame::OnEditClearHistory)
    EVT_MENU(idEditCopy,  MainFrame::OnEditCopy)
    EVT_MENU(idEditCut,  MainFrame::OnEditCut)
    EVT_MENU(idEditPaste,  MainFrame::OnEditPaste)
    EVT_MENU(idEditSwapHeaderSource,  MainFrame::OnEditSwapHeaderSource)
    EVT_MENU(idEditGotoMatchingBrace,  MainFrame::OnEditGotoMatchingBrace)
    EVT_MENU(idEditHighlightModeText,  MainFrame::OnEditHighlightMode)
    EVT_MENU(idEditFoldAll,  MainFrame::OnEditFoldAll)
    EVT_MENU(idEditUnfoldAll,  MainFrame::OnEditUnfoldAll)
    EVT_MENU(idEditToggleAllFolds,  MainFrame::OnEditToggleAllFolds)
    EVT_MENU(idEditFoldBlock,  MainFrame::OnEditFoldBlock)
    EVT_MENU(idEditUnfoldBlock,  MainFrame::OnEditUnfoldBlock)
    EVT_MENU(idEditToggleFoldBlock,  MainFrame::OnEditToggleFoldBlock)
    EVT_MENU(idEditEOLCRLF,  MainFrame::OnEditEOLMode)
    EVT_MENU(idEditEOLCR,  MainFrame::OnEditEOLMode)
    EVT_MENU(idEditEOLLF,  MainFrame::OnEditEOLMode)
    EVT_MENU(idEditEncodingDefault,  MainFrame::OnEditEncoding)
    EVT_MENU(idEditEncodingUseBom,  MainFrame::OnEditEncoding)
    EVT_MENU(idEditEncodingAscii,  MainFrame::OnEditEncoding)
    EVT_MENU(idEditEncodingUtf7,  MainFrame::OnEditEncoding)
    EVT_MENU(idEditEncodingUtf8,  MainFrame::OnEditEncoding)
    EVT_MENU(idEditEncodingUnicode,  MainFrame::OnEditEncoding)
    EVT_MENU(idEditEncodingUtf16,  MainFrame::OnEditEncoding)
    EVT_MENU(idEditEncodingUtf32,  MainFrame::OnEditEncoding)
    EVT_MENU(idEditEncodingUnicode16BE,  MainFrame::OnEditEncoding)
    EVT_MENU(idEditEncodingUnicode16LE,  MainFrame::OnEditEncoding)
    EVT_MENU(idEditEncodingUnicode32BE,  MainFrame::OnEditEncoding)
    EVT_MENU(idEditEncodingUnicode32LE,  MainFrame::OnEditEncoding)
    EVT_MENU(idEditParaUp,  MainFrame::OnEditParaUp)
    EVT_MENU(idEditParaUpExtend,  MainFrame::OnEditParaUpExtend)
    EVT_MENU(idEditParaDown,  MainFrame::OnEditParaDown)
    EVT_MENU(idEditParaDownExtend,  MainFrame::OnEditParaDownExtend)
    EVT_MENU(idEditWordPartLeft,  MainFrame::OnEditWordPartLeft)
    EVT_MENU(idEditWordPartLeftExtend,  MainFrame::OnEditWordPartLeftExtend)
    EVT_MENU(idEditWordPartRight,  MainFrame::OnEditWordPartRight)
    EVT_MENU(idEditWordPartRightExtend,  MainFrame::OnEditWordPartRightExtend)
    EVT_MENU(idEditZoomIn,  MainFrame::OnEditZoomIn)
    EVT_MENU(idEditZoomOut,  MainFrame::OnEditZoomOut)
    EVT_MENU(idEditZoomReset,  MainFrame::OnEditZoomReset)
    EVT_MENU(idEditLineCut,  MainFrame::OnEditLineCut)
    EVT_MENU(idEditLineDelete,  MainFrame::OnEditLineDelete)
    EVT_MENU(idEditLineDuplicate,  MainFrame::OnEditLineDuplicate)
    EVT_MENU(idEditLineTranspose,  MainFrame::OnEditLineTranspose)
    EVT_MENU(idEditLineCopy,  MainFrame::OnEditLineCopy)
    EVT_MENU(idEditLinePaste,  MainFrame::OnEditLinePaste)
    EVT_MENU(idEditUpperCase,  MainFrame::OnEditUpperCase)
    EVT_MENU(idEditLowerCase,  MainFrame::OnEditLowerCase)
    EVT_MENU(idEditSelectAll,  MainFrame::OnEditSelectAll)
    EVT_MENU(idEditBookmarksToggle,  MainFrame::OnEditBookmarksToggle)
    EVT_MENU(idEditBookmarksNext,  MainFrame::OnEditBookmarksNext)
    EVT_MENU(idEditBookmarksPrevious,  MainFrame::OnEditBookmarksPrevious)
    EVT_MENU(idEditCommentSelected, MainFrame::OnEditCommentSelected)
    EVT_MENU(idEditAutoComplete, MainFrame::OnEditAutoComplete)
    EVT_MENU(idEditUncommentSelected, MainFrame::OnEditUncommentSelected)
    EVT_MENU(idEditToggleCommentSelected, MainFrame::OnEditToggleCommentSelected)
    EVT_MENU(idEditStreamCommentSelected, MainFrame::OnEditStreamCommentSelected)
    EVT_MENU(idEditBoxCommentSelected, MainFrame::OnEditBoxCommentSelected)

    EVT_MENU(idSearchFind, MainFrame::OnSearchFind)
    EVT_MENU(idSearchFindInFiles, MainFrame::OnSearchFind)
    EVT_MENU(idSearchFindNext, MainFrame::OnSearchFindNext)
    EVT_MENU(idSearchFindPrevious, MainFrame::OnSearchFindNext)
    EVT_MENU(idSearchReplace, MainFrame::OnSearchReplace)
    EVT_MENU(idSearchReplaceInFiles, MainFrame::OnSearchReplace)
    EVT_MENU(idSearchGotoLine, MainFrame::OnSearchGotoLine)
    EVT_MENU(idSearchGotoNextChanged, MainFrame::OnSearchGotoNextChanged)
    EVT_MENU(idSearchGotoPreviousChanged, MainFrame::OnSearchGotoPrevChanged)

    EVT_MENU(idViewLayoutSave, MainFrame::OnViewLayoutSave)
    EVT_MENU(idViewLayoutDelete, MainFrame::OnViewLayoutDelete)
    EVT_MENU(idViewToolMain, MainFrame::OnToggleBar)
    EVT_MENU(idViewLogManager, MainFrame::OnToggleBar)
    EVT_MENU(idViewManager, MainFrame::OnToggleBar)
    EVT_MENU(idViewStatusbar, MainFrame::OnToggleStatusBar)
    EVT_MENU(idViewScriptConsole, MainFrame::OnViewScriptConsole)
    EVT_MENU(idViewFocusEditor, MainFrame::OnFocusEditor)
    EVT_MENU(idViewSwitchTabs, MainFrame::OnSwitchTabs)
    EVT_MENU(idViewFullScreen, MainFrame::OnToggleFullScreen)

    EVT_MENU(idSettingsEnvironment, MainFrame::OnSettingsEnvironment)
    EVT_MENU(idSettingsGlobalUserVars, MainFrame::OnGlobalUserVars)
    EVT_MENU(idSettingsEditor, MainFrame::OnSettingsEditor)
    EVT_MENU(idSettingsCompilerDebugger, MainFrame::OnSettingsCompilerDebugger)
    EVT_MENU(idPluginsManagePlugins, MainFrame::OnSettingsPlugins)
    EVT_MENU(idSettingsScripting, MainFrame::OnSettingsScripting)

    EVT_MENU(wxID_ABOUT, MainFrame::OnHelpAbout)
    EVT_MENU(idHelpTips, MainFrame::OnHelpTips)

    EVT_MENU(idStartHerePageLink, MainFrame::OnStartHereLink)
    EVT_MENU(idStartHerePageVarSubst, MainFrame::OnStartHereVarSubst)

    EVT_NOTEBOOK_PAGE_CHANGED(ID_NBEditorManager, MainFrame::OnPageChanged)

    /// CloseFullScreen event handling
    EVT_BUTTON( idCloseFullScreen, MainFrame::OnToggleFullScreen )

    /// Shift-Tab bug workaround
    EVT_MENU(idShiftTab,MainFrame::OnShiftTab)

END_EVENT_TABLE()

MainFrame::MainFrame(wxWindow* parent)
       : wxFrame(parent, -1, _T("MainWin"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE),
       m_LayoutManager(this),
       m_pAccel(0L),
       m_pFilesHistory(0),
       m_pProjectsHistory(0),
       m_pCloseFullScreenBtn(0L),
       m_pEdMan(0L),
       m_pPrjMan(0L),
       m_pMsgMan(0L),
       m_pInfoPane(0L),
       m_pToolbar(0L),
       m_ToolsMenu(0L),
       m_HelpPluginsMenu(0L),
       m_StartupDone(false), // one-time flag
       m_InitiatedShutdown(false),
       m_AutoHideLockCounter(0),
       m_LastLayoutIsTemp(false),
       m_pScriptConsole(0),
       m_pBatchBuildDialog(0),
       m_pProgressBar(0)
{
#if defined( _MSC_VER ) && defined( _DEBUG )
    int tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );
    //tmpFlag |= _CRTDBG_CHECK_ALWAYS_DF;
    _CrtSetDbgFlag( tmpFlag );
#endif

    // register event sinks
    RegisterEvents();

    // New: Allow drag and drop of files into the editor
    SetDropTarget(new wxMyFileDropTarget(this));

    // Accelerator table
    wxAcceleratorEntry entries[7];

    entries[0].Set(wxACCEL_CTRL | wxACCEL_SHIFT,  (int) 'W', idFileCloseAll);
    entries[1].Set(wxACCEL_CTRL | wxACCEL_SHIFT,  WXK_F4, idFileCloseAll);
    entries[2].Set(wxACCEL_CTRL,  (int) 'W', idFileClose);
    entries[3].Set(wxACCEL_CTRL,  WXK_F4, idFileClose);
    entries[4].Set(wxACCEL_CTRL,  WXK_F6, idFileNext);
    entries[5].Set(wxACCEL_CTRL | wxACCEL_SHIFT,  WXK_F6, idFilePrev);
    entries[6].Set(wxACCEL_SHIFT,  WXK_TAB, idShiftTab);
    m_pAccel = new wxAcceleratorTable(7, entries);

    this->SetAcceleratorTable(*m_pAccel);

    // add file filters for supported projects/workspaces
    FileFilters::AddDefaultFileFilters();

    m_SmallToolBar = Manager::Get()->GetConfigManager(_T("app"))->ReadBool(_T("/environment/toolbar_size"), true);
    CreateIDE();

#ifdef __WXMSW__
    SetIcon(wxICON(A_MAIN_ICON));
#else
    SetIcon(wxIcon(app));
#endif // __WXMSW__

    DoCreateStatusBar();
#if wxUSE_STATUSBAR
    SetStatusText(_("Welcome to ")+ appglobals::AppName + _T("!"));
#endif // wxUSE_STATUSBAR

    SetTitle(appglobals::AppName + _T(" v") + appglobals::AppVersion);

    ScanForPlugins();

    // save default view
    wxString deflayout = Manager::Get()->GetConfigManager(_T("app"))->Read(_T("/main_frame/layout/default"));
    if (deflayout.IsEmpty())
        Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/main_frame/layout/default"), gDefaultLayout);
    DoFixToolbarsLayout();
    gDefaultLayoutData = m_LayoutManager.SavePerspective(); // keep the "hardcoded" layout handy
    SaveViewLayout(gDefaultLayout, gDefaultLayoutData);
    LoadWindowState();

    ShowHideStartPage();

    RegisterScriptFunctions();
    RunStartupScripts();

//    m_StartupDone = true;
//    DoUpdateLayout();

//    if (Manager::Get()->GetLogManager()->HasErrors())
//    {
//        InfoWindow::Display(_("Errors logged!"), _("Some errors have been logged during\n"
//                                                    "the Code::Blocks startup process.\n\n"
//                                                    "Please review them in the logs...\n\n"), 8000, 1000);
//    }
//    else if (Manager::Get()->GetLogManager()->HasWarnings())
//    {
//        InfoWindow::Display(_("Warnings logged!"), _("Some warnings have been logged during\n"
//                                                    "the Code::Blocks startup process.\n\n"
//                                                    "Please review them in the logs...\n\n"), 8000, 1000);
//    }

    Manager::Get()->GetLogManager()->DebugLog(_T("Initializing plugins..."));
}

MainFrame::~MainFrame()
{
//    if (m_pBatchBuildDialog)
//        m_pBatchBuildDialog->Destroy();
//    m_pBatchBuildDialog = 0;

    this->SetAcceleratorTable(wxNullAcceleratorTable);
    delete m_pAccel;

    DeInitPrinting();
    //Manager::Get()->Free();
}

void MainFrame::RegisterEvents()
{
    Manager* pm = Manager::Get();

    pm->RegisterEventSink(cbEVT_EDITOR_UPDATE_UI, new cbEventFunctor<MainFrame, CodeBlocksEvent>(this, &MainFrame::OnEditorUpdateUI));

    pm->RegisterEventSink(cbEVT_PROJECT_ACTIVATE, new cbEventFunctor<MainFrame, CodeBlocksEvent>(this, &MainFrame::OnProjectActivated));
    pm->RegisterEventSink(cbEVT_PROJECT_OPEN, new cbEventFunctor<MainFrame, CodeBlocksEvent>(this, &MainFrame::OnProjectOpened));
    pm->RegisterEventSink(cbEVT_PROJECT_CLOSE, new cbEventFunctor<MainFrame, CodeBlocksEvent>(this, &MainFrame::OnProjectClosed));
    pm->RegisterEventSink(cbEVT_EDITOR_CLOSE, new cbEventFunctor<MainFrame, CodeBlocksEvent>(this, &MainFrame::OnEditorClosed));
    pm->RegisterEventSink(cbEVT_EDITOR_OPEN, new cbEventFunctor<MainFrame, CodeBlocksEvent>(this, &MainFrame::OnEditorOpened));
    pm->RegisterEventSink(cbEVT_EDITOR_ACTIVATED, new cbEventFunctor<MainFrame, CodeBlocksEvent>(this, &MainFrame::OnEditorActivated));
    pm->RegisterEventSink(cbEVT_EDITOR_SAVE, new cbEventFunctor<MainFrame, CodeBlocksEvent>(this, &MainFrame::OnEditorSaved));
    pm->RegisterEventSink(cbEVT_EDITOR_MODIFIED, new cbEventFunctor<MainFrame, CodeBlocksEvent>(this, &MainFrame::OnEditorModified));

    pm->RegisterEventSink(cbEVT_ADD_DOCK_WINDOW, new cbEventFunctor<MainFrame, CodeBlocksDockEvent>(this, &MainFrame::OnRequestDockWindow));
    pm->RegisterEventSink(cbEVT_REMOVE_DOCK_WINDOW, new cbEventFunctor<MainFrame, CodeBlocksDockEvent>(this, &MainFrame::OnRequestUndockWindow));
    pm->RegisterEventSink(cbEVT_SHOW_DOCK_WINDOW, new cbEventFunctor<MainFrame, CodeBlocksDockEvent>(this, &MainFrame::OnRequestShowDockWindow));
    pm->RegisterEventSink(cbEVT_HIDE_DOCK_WINDOW, new cbEventFunctor<MainFrame, CodeBlocksDockEvent>(this, &MainFrame::OnRequestHideDockWindow));
    pm->RegisterEventSink(cbEVT_DOCK_WINDOW_VISIBILITY, new cbEventFunctor<MainFrame, CodeBlocksDockEvent>(this, &MainFrame::OnDockWindowVisibility));

    pm->RegisterEventSink(cbEVT_PLUGIN_ATTACHED, new cbEventFunctor<MainFrame, CodeBlocksEvent>(this, &MainFrame::OnPluginLoaded));
    pm->RegisterEventSink(cbEVT_PLUGIN_RELEASED, new cbEventFunctor<MainFrame, CodeBlocksEvent>(this, &MainFrame::OnPluginUnloaded));
    pm->RegisterEventSink(cbEVT_PLUGIN_INSTALLED, new cbEventFunctor<MainFrame, CodeBlocksEvent>(this, &MainFrame::OnPluginInstalled));
    pm->RegisterEventSink(cbEVT_PLUGIN_UNINSTALLED, new cbEventFunctor<MainFrame, CodeBlocksEvent>(this, &MainFrame::OnPluginUninstalled));

    pm->RegisterEventSink(cbEVT_QUERY_VIEW_LAYOUT, new cbEventFunctor<MainFrame, CodeBlocksLayoutEvent>(this, &MainFrame::OnLayoutQuery));
    pm->RegisterEventSink(cbEVT_SWITCH_VIEW_LAYOUT, new cbEventFunctor<MainFrame, CodeBlocksLayoutEvent>(this, &MainFrame::OnLayoutSwitch));

    pm->RegisterEventSink(cbEVT_ADD_LOG_WINDOW, new cbEventFunctor<MainFrame, CodeBlocksLogEvent>(this, &MainFrame::OnAddLogWindow));
    pm->RegisterEventSink(cbEVT_REMOVE_LOG_WINDOW, new cbEventFunctor<MainFrame, CodeBlocksLogEvent>(this, &MainFrame::OnRemoveLogWindow));
    pm->RegisterEventSink(cbEVT_SWITCH_TO_LOG_WINDOW, new cbEventFunctor<MainFrame, CodeBlocksLogEvent>(this, &MainFrame::OnSwitchToLogWindow));
    pm->RegisterEventSink(cbEVT_SHOW_LOG_MANAGER, new cbEventFunctor<MainFrame, CodeBlocksLogEvent>(this, &MainFrame::OnShowLogManager));
    pm->RegisterEventSink(cbEVT_HIDE_LOG_MANAGER, new cbEventFunctor<MainFrame, CodeBlocksLogEvent>(this, &MainFrame::OnHideLogManager));
    pm->RegisterEventSink(cbEVT_LOCK_LOG_MANAGER, new cbEventFunctor<MainFrame, CodeBlocksLogEvent>(this, &MainFrame::OnLockLogManager));
    pm->RegisterEventSink(cbEVT_UNLOCK_LOG_MANAGER, new cbEventFunctor<MainFrame, CodeBlocksLogEvent>(this, &MainFrame::OnUnlockLogManager));
}

void MainFrame::ShowTips(bool forceShow)
{
    bool showAtStartup = Manager::Get()->GetConfigManager(_T("app"))->ReadBool(_T("/show_tips"), true);
    if (forceShow || showAtStartup)
    {
        wxString tipsFile = ConfigManager::GetDataFolder() + _T("/tips.txt");
        long tipsIndex = Manager::Get()->GetConfigManager(_T("app"))->ReadInt(_T("/next_tip"), 0);
        wxTipProvider* tipProvider = wxCreateFileTipProvider(tipsFile, tipsIndex);
        showAtStartup = wxShowTip(this, tipProvider, showAtStartup);
        Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/show_tips"), showAtStartup);
        Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/next_tip"), (int)tipProvider->GetCurrentTip());
        delete tipProvider;
    }
}

void MainFrame::CreateIDE()
{
    int leftW = Manager::Get()->GetConfigManager(_T("app"))->ReadInt(_T("/main_frame/layout/left_block_width"), 200);
//    int bottomH = Manager::Get()->GetConfigManager(_T("app"))->ReadInt(_T("/main_frame/layout/bottom_block_height"), 150);
    SetSize(800,600);
    wxSize clientsize = GetClientSize();

    // Create CloseFullScreen Button, and hide it initially
    m_pCloseFullScreenBtn = new wxButton(this, idCloseFullScreen, _( "Close full screen" ), wxDefaultPosition );
    m_pCloseFullScreenBtn->Show( false );

    // project manager
    Manager::Get(this);
    m_LayoutManager.AddPane(Manager::Get()->GetProjectManager()->GetNotebook(), wxAuiPaneInfo().
                              Name(wxT("ManagementPane")).Caption(_("Management")).
                              BestSize(wxSize(leftW, clientsize.GetHeight())).MinSize(wxSize(100,100)).
                              Left().Layer(1));

    // logs manager
    SetupGUILogging();

    CreateMenubar();

    m_pEdMan = Manager::Get()->GetEditorManager();
    m_pPrjMan = Manager::Get()->GetProjectManager();
    m_pMsgMan = Manager::Get()->GetLogManager();

    CreateToolbars();
    SetToolBar(0);

    // editor manager
    m_LayoutManager.AddPane(m_pEdMan->GetNotebook(), wxAuiPaneInfo().Name(wxT("MainPane")).
                            CentrePane());

    m_pScriptConsole = new ScriptConsole(this, -1);
    m_LayoutManager.AddPane(m_pScriptConsole, wxAuiPaneInfo().Name(wxT("ScriptConsole")).
                            Caption(_("Scripting console")).Float().MinSize(100,100));

    DoUpdateLayout();
    DoUpdateLayoutColours();
    DoUpdateEditorStyle();

    m_pEdMan->GetNotebook()->SetDropTarget(new wxMyFileDropTarget(this));
    m_pPrjMan->GetNotebook()->SetDropTarget(new wxMyFileDropTarget(this));
}


void MainFrame::SetupGUILogging()
{
    // allow new docked windows to use be 3/4 of the available space, the default (0.3) is sometimes too small, especially for "Logs & others"
    m_LayoutManager.SetDockSizeConstraint(0.75,0.75);

    int bottomH = Manager::Get()->GetConfigManager(_T("app"))->ReadInt(_T("/main_frame/layout/bottom_block_height"), 150);
    wxSize clientsize = GetClientSize();

    LogManager* mgr = Manager::Get()->GetLogManager();

    if (!Manager::IsBatchBuild())
    {
        m_pInfoPane = new InfoPane(this);
        m_LayoutManager.AddPane(m_pInfoPane, wxAuiPaneInfo().
                                  Name(wxT("MessagesPane")).Caption(_("Logs & others")).
                                  BestSize(wxSize(clientsize.GetWidth(), bottomH)).//MinSize(wxSize(50,50)).
                                  Bottom());

        wxWindow* log;

        for(size_t i = LogManager::app_log; i < ::max_logs; ++i)
        {
            if ((log = mgr->Slot(i).GetLogger()->CreateControl(m_pInfoPane)))
                m_pInfoPane->AddLogger(mgr->Slot(i).GetLogger(), log, mgr->Slot(i).title, mgr->Slot(i).icon);
        }
    }
    else
    {
        m_pBatchBuildDialog = new BatchLogWindow(this, _("Batch build"));
        wxSizer* s = new wxBoxSizer(wxVERTICAL);
        m_pInfoPane = new InfoPane(m_pBatchBuildDialog);
        s->Add(m_pInfoPane, 1, wxEXPAND);
        m_pBatchBuildDialog->SetSizer(s);

        // setting &g_null_log causes the app to crash on exit for some reason...
        mgr->SetLog(new NullLogger, LogManager::app_log);
        mgr->SetLog(new NullLogger, LogManager::debug_log);
    }

    mgr->NotifyUpdate();
    m_pInfoPane->SetDropTarget(new wxMyFileDropTarget(this));
}


DECLARE_INSTANCE_TYPE(MainFrame);

void MainFrame::RegisterScriptFunctions()
{
    SqPlus::SQClassDef<MainFrame>("MainFrame").
                    func(&MainFrame::Open, "Open");

    SqPlus::BindVariable(this, "App", SqPlus::VAR_ACCESS_READ_ONLY);
}

void MainFrame::RunStartupScripts()
{
    ConfigManager* mgr = 0;
    mgr = Manager::Get()->GetConfigManager(_T("scripting"));
    wxArrayString keys = mgr->EnumerateKeys(_T("/startup_scripts"));

    for (size_t i = 0; i < keys.GetCount(); ++i)
    {
        ScriptEntry se;
        wxString ser;
        if (mgr->Read(_T("/startup_scripts/") + keys[i], &ser))
        {
            se.SerializeIn(ser);
            if (!se.enabled)
                continue;

            try
            {
                wxString startup = se.script;
                if (wxFileName(se.script).IsRelative())
                    startup = ConfigManager::LocateDataFile(se.script, sdScriptsUser | sdScriptsGlobal);
                if (!startup.IsEmpty())
                {
                    if (!se.registered)
                        Manager::Get()->GetScriptingManager()->LoadScript(startup);
                    else if (!se.menu.IsEmpty())
                        Manager::Get()->GetScriptingManager()->RegisterScriptMenu(se.menu, startup, false);
                    else
                    #if wxCHECK_VERSION(2, 9, 0)
                        Manager::Get()->GetLogManager()->LogWarning(F(_("Startup script/function '%s' not loaded: invalid configuration"), se.script.wx_str()));
                    #else
                        Manager::Get()->GetLogManager()->LogWarning(F(_("Startup script/function '%s' not loaded: invalid configuration"), se.script.c_str()));
                    #endif
                }
                else
                #if wxCHECK_VERSION(2, 9, 0)
                    Manager::Get()->GetLogManager()->LogWarning(F(_("Startup script '%s' not found"), se.script.wx_str()));
                #else
                    Manager::Get()->GetLogManager()->LogWarning(F(_("Startup script '%s' not found"), se.script.c_str()));
                #endif
            }
            catch (SquirrelError& exception)
            {
                Manager::Get()->GetScriptingManager()->DisplayErrors(&exception);
            }
        }
    }
}

void MainFrame::PluginsUpdated(cbPlugin* plugin, int status)
{
    Freeze();

    // menu
    RecreateMenuBar();

    // update view->toolbars because we re-created the menubar
    PluginElementsArray plugins = Manager::Get()->GetPluginManager()->GetPlugins();
    for (unsigned int i = 0; i < plugins.GetCount(); ++i)
    {
        cbPlugin* plug = plugins[i]->plugin;
        const PluginInfo* info = Manager::Get()->GetPluginManager()->GetPluginInfo(plug);
        if (!info)
            continue;

        if (m_PluginsTools[plug]) // if plugin has a toolbar
        {
            // toolbar exists; add the menu item
            wxMenu* viewToolbars = 0;
            GetMenuBar()->FindItem(idViewToolMain, &viewToolbars);
            if (viewToolbars)
            {
                if (viewToolbars->FindItem(info->title) != wxNOT_FOUND)
                    continue;
                wxMenuItem* item = AddPluginInMenus(viewToolbars, plug,
                                                    (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)&MainFrame::OnToggleBar,
                                                    -1, true);
                if (item)
                {
                    item->Check(IsWindowReallyShown(m_PluginsTools[plug]));
                }
            }
        }
    }

    Thaw();
}

void MainFrame::RecreateMenuBar()
{
    Freeze();
    wxMenuBar* m = GetMenuBar();
    SetMenuBar(0); // unhook old menubar
    CreateMenubar(); // create new menubar
    delete m; // delete old menubar

    // update layouts menu
    for (LayoutViewsMap::iterator it = m_LayoutViews.begin(); it != m_LayoutViews.end(); ++it)
    {
        if (it->first.IsEmpty())
            continue;
        SaveViewLayout(it->first, it->second, it->first == m_LastLayoutName);
    }

    Thaw();
}

void MainFrame::CreateMenubar()
{
    CodeBlocksEvent event(cbEVT_MENUBAR_CREATE_BEGIN);
    Manager::Get()->ProcessEvent(event);

    int tmpidx;
    wxMenuBar* mbar=0L;
    wxMenu *hl=0L, *tools=0L, *plugs=0L, *pluginsM=0L;
    wxMenuItem *tmpitem=0L;

    wxString resPath = ConfigManager::GetDataFolder();
    wxXmlResource *myres = wxXmlResource::Get();
    myres->Load(resPath + _T("/resources.zip#zip:main_menu.xrc"));
    mbar = myres->LoadMenuBar(_T("main_menu_bar"));
    if (!mbar)
        mbar = new wxMenuBar(); // Some error happened.
    if (mbar)
        SetMenuBar(mbar);

    // Find Menus that we'll change later

    tmpidx=mbar->FindMenu(_("&Edit"));
    if (tmpidx!=wxNOT_FOUND)
    {
        mbar->FindItem(idEditHighlightModeText, &hl);
        if (hl)
        {
            EditorColourSet* theme = Manager::Get()->GetEditorManager()->GetColourSet();
            if (theme)
            {
                wxArrayString langs = theme->GetAllHighlightLanguages();
                for (size_t i = 0; i < langs.GetCount(); ++i)
                {
                    if (i > 0 && !(i % 20))
                        hl->Break(); // break into columns every 20 items
                    int id = wxNewId();
                    hl->AppendRadioItem(id, langs[i],
                                wxString::Format(_("Switch highlighting mode for current document to \"%s\""), langs[i].c_str()));
                    Connect(id, -1, wxEVT_COMMAND_MENU_SELECTED,
                            (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction)
                            &MainFrame::OnEditHighlightMode);
                }
            }
        }
    }

    tmpidx=mbar->FindMenu(_("&Tools"));
    if (tmpidx!=wxNOT_FOUND)
        tools = mbar->GetMenu(tmpidx);

    tmpidx=mbar->FindMenu(_("P&lugins"));
    if (tmpidx!=wxNOT_FOUND)
        plugs = mbar->GetMenu(tmpidx);

    if ((tmpitem = mbar->FindItem(idHelpPlugins,NULL)))
        pluginsM = tmpitem->GetSubMenu();

    m_ToolsMenu = tools ? tools : new wxMenu();
    m_PluginsMenu = plugs ? plugs : new wxMenu();
    m_HelpPluginsMenu = pluginsM ? pluginsM : new wxMenu();

    // core modules: create menus
    Manager::Get()->GetProjectManager()->CreateMenu(mbar);
    Manager::Get()->GetEditorManager()->CreateMenu(mbar);
//    Manager::Get()->GetLogManager()->CreateMenu(mbar);

    // ask all plugins to rebuild their menus
    PluginElementsArray plugins = Manager::Get()->GetPluginManager()->GetPlugins();
    for (unsigned int i = 0; i < plugins.GetCount(); ++i)
    {
        cbPlugin* plug = plugins[i]->plugin;
        if (plug && plug->IsAttached())
        {
            if (plug->GetType() == ptTool)
            {
                DoAddPlugin(plug);
            }
            else
            {
                AddPluginInSettingsMenu(plug);
                AddPluginInHelpPluginsMenu(plug);
                try
                {
                    plug->BuildMenu(mbar);
                }
                catch (cbException& e)
                {
                    e.ShowErrorMessage();
                }
            }
        }
    }

    Manager::Get()->GetToolsManager()->BuildToolsMenu(m_ToolsMenu);

    // Ctrl+Tab workaround for non windows platforms:
    if ((platform::carbon) || (platform::gtk))
    {
        // Find the menu item for tab switching:
        tmpidx = mbar->FindMenu(_("&View"));
        if (tmpidx != wxNOT_FOUND)
        {
            wxMenu* view = mbar->GetMenu(tmpidx);
            wxMenuItem* switch_item = view->FindItem(idViewSwitchTabs);
            if (switch_item)
            {
                // Change the accelerator for this menu item:
                wxString accel;
                if (platform::carbon)
                    accel = wxT("Alt+Tab");
                else if (platform::gtk)
                    accel = wxT("Ctrl+,");
                switch_item->SetItemLabel(wxString(_("S&witch Tabs")) + wxT("\t") + accel);
            }
        }
    }

    SetMenuBar(mbar);
    InitializeRecentFilesHistory();

    CodeBlocksEvent event2(cbEVT_MENUBAR_CREATE_END);
    Manager::Get()->ProcessEvent(event2);
}

void MainFrame::CreateToolbars()
{
    wxXmlResource *myres = wxXmlResource::Get();
    if (m_pToolbar)
    {
        SetToolBar(0L);
//        delete m_pToolbar;
        m_pToolbar = 0L;
    }

    wxString resPath = ConfigManager::GetDataFolder();
    wxString xrcToolbarName = _T("main_toolbar");
    if (m_SmallToolBar) // Insert logic here
        xrcToolbarName += _T("_16x16");
    myres->Load(resPath + _T("/resources.zip#zip:*.xrc"));
    Manager::Get()->GetLogManager()->DebugLog(_T("Loading toolbar..."));

    wxSize size = m_SmallToolBar ? wxSize(16, 16) : (platform::macosx ? wxSize(32, 32) : wxSize(22, 22));
    m_pToolbar = new wxToolBar(this, -1, wxDefaultPosition, size, wxTB_FLAT | wxTB_NODIVIDER);
    m_pToolbar->SetToolBitmapSize(size);
    Manager::Get()->AddonToolBar(m_pToolbar,xrcToolbarName);

    m_pToolbar->Realize();

    m_pToolbar->SetInitialSize();

    // add toolbars in docking system
    m_LayoutManager.AddPane(m_pToolbar, wxAuiPaneInfo().
                          Name(wxT("MainToolbar")).Caption(_("Main Toolbar")).
                          ToolbarPane().Top());
    DoUpdateLayout();

    // ask all plugins to rebuild their toolbars
    PluginElementsArray plugins = Manager::Get()->GetPluginManager()->GetPlugins();
    for (unsigned int i = 0; i < plugins.GetCount(); ++i)
    {
        cbPlugin* plug = plugins[i]->plugin;
        if (plug && plug->IsAttached())
        {
            DoAddPluginToolbar(plug);
        }
    }

    Manager::ProcessPendingEvents();
}

void MainFrame::AddToolbarItem(int id, const wxString& title, const wxString& shortHelp, const wxString& longHelp, const wxString& image)
{
    m_pToolbar->AddTool(id, title, cbLoadBitmap(image, wxBITMAP_TYPE_PNG));
    m_pToolbar->SetToolShortHelp(id, shortHelp);
    m_pToolbar->SetToolLongHelp(id, longHelp);
}

void MainFrame::ScanForPlugins()
{
    m_PluginIDsMap.clear();

    PluginManager* m_PluginManager = Manager::Get()->GetPluginManager();

    // user paths first
    wxString path = ConfigManager::GetPluginsFolder(false);
    Manager::Get()->GetLogManager()->Log(_("Scanning for plugins in ") + path);
    int count = m_PluginManager->ScanForPlugins(path);

    // global paths
    path = ConfigManager::GetPluginsFolder(true);
    Manager::Get()->GetLogManager()->Log(_("Scanning for plugins in ") + path);
    count += m_PluginManager->ScanForPlugins(path);

    // actually load plugins
    if (count > 0)
    {
        Manager::Get()->GetLogManager()->Log(_("Loading:"));
        m_PluginManager->LoadAllPlugins();
    }
}

wxMenuItem* MainFrame::AddPluginInMenus(wxMenu* menu, cbPlugin* plugin, wxObjectEventFunction callback, int pos, bool checkable)
{
    wxMenuItem* item = 0;
    if (!plugin || !menu)
        return item;

    const PluginInfo* info = Manager::Get()->GetPluginManager()->GetPluginInfo(plugin);
    if (!info)
        return 0;

    PluginIDsMap::iterator it;
    for (it = m_PluginIDsMap.begin(); it != m_PluginIDsMap.end(); ++it)
    {
        if (it->second == info->name)
        {
            item = menu->FindItem(it->first);
            if (item)
                return item;
        }
    }

    int id = wxNewId();
    wxString title = info->title + (menu == m_HelpPluginsMenu ? _T("...") : wxEmptyString);
    m_PluginIDsMap[id] = info->name;
    if (pos == -1)
        pos = menu->GetMenuItemCount();

    while(!item)
    {
        #if wxCHECK_VERSION(2, 9, 0)
        if (!pos || title.CmpNoCase(menu->FindItemByPosition(pos - 1)->GetItemLabelText()) > 0)
        #else
        if (!pos || title.CmpNoCase(menu->FindItemByPosition(pos - 1)->GetLabel()) > 0)
        #endif
            item = menu->Insert(pos, id, title, wxEmptyString, checkable ? wxITEM_CHECK : wxITEM_NORMAL);

        --pos;
    }

    Connect( id,  wxEVT_COMMAND_MENU_SELECTED, callback );
    return item;
}

void MainFrame::AddPluginInPluginsMenu(cbPlugin* plugin)
{
    // "Plugins" menu is special case because it contains "Manage plugins",
    // which must stay at the end of the menu
    // So we insert entries, not append...

    // this will insert a separator when the first plugin is added in the "Plugins" menu
    if (m_PluginsMenu->GetMenuItemCount() == 1)
         m_PluginsMenu->Insert(0, wxID_SEPARATOR, _T(""));

    AddPluginInMenus(m_PluginsMenu, plugin,
                    (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)&MainFrame::OnPluginsExecuteMenu,
                    m_PluginsMenu->GetMenuItemCount() - 2);
}

void MainFrame::AddPluginInSettingsMenu(cbPlugin* plugin)
{
//    if (!plugin)
//        return;
//    if (!plugin->GetInfo()->hasConfigure)
//        return;
//    AddPluginInMenus(m_SettingsMenu, plugin,
//                    (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)&MainFrame::OnPluginSettingsMenu);
}

void MainFrame::AddPluginInHelpPluginsMenu(cbPlugin* plugin)
{
    AddPluginInMenus(m_HelpPluginsMenu, plugin,
                    (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)&MainFrame::OnHelpPluginMenu);
}

void MainFrame::RemovePluginFromMenus(const wxString& pluginName)
{
    //Manager::Get()->GetLogManager()->DebugLog("Unloading %s plugin", pluginName.c_str());
    if (pluginName.IsEmpty())
        return;

    // look for plugin's id
    wxArrayInt id;
    PluginIDsMap::iterator it = m_PluginIDsMap.begin();
    while (it != m_PluginIDsMap.end())
    {
        if (pluginName.Matches(it->second))
        {
            id.Add(it->first);
            PluginIDsMap::iterator it2 = it;
            ++it;
            m_PluginIDsMap.erase(it2);
        }
        else
            ++it;
    }
    //Manager::Get()->GetLogManager()->DebugLog("id=%d", id);
    if (id.GetCount() == 0)
        return; // not found

    for (unsigned int i = 0; i < id.GetCount(); ++i)
    {
        Disconnect( id[i],  wxEVT_COMMAND_MENU_SELECTED,
            (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction)
            &MainFrame::OnPluginsExecuteMenu );
        m_PluginIDsMap.erase(id[i]);
        m_PluginsMenu->Delete(id[i]);
        m_HelpPluginsMenu->Delete(id[i]);
    }
}

void MainFrame::LoadWindowState()
{
    wxArrayString subs = Manager::Get()->GetConfigManager(_T("app"))->EnumerateSubPaths(_T("/main_frame/layout"));
    for (size_t i = 0; i < subs.GetCount(); ++i)
    {
        wxString name = Manager::Get()->GetConfigManager(_T("app"))->Read(_T("/main_frame/layout/") + subs[i] + _T("/name"));
        wxString layout = Manager::Get()->GetConfigManager(_T("app"))->Read(_T("/main_frame/layout/") + subs[i] + _T("/data"));
        SaveViewLayout(name, layout);
    }
    wxString deflayout = Manager::Get()->GetConfigManager(_T("app"))->Read(_T("/main_frame/layout/default"));
    LoadViewLayout(deflayout);

    // load manager and messages selected page
    Manager::Get()->GetProjectManager()->GetNotebook()->SetSelection(Manager::Get()->GetConfigManager(_T("app"))->ReadInt(_T("/main_frame/layout/left_block_selection"), 0));
    m_pInfoPane->SetSelection(Manager::Get()->GetConfigManager(_T("app"))->ReadInt(_T("/main_frame/layout/bottom_block_selection"), 0));

#ifndef __WXMAC__
    int x = 0;
    int y = 0;
#else
    int x = 0;
    int y = wxSystemSettings::GetMetric(wxSYS_MENU_Y, this); // make sure it doesn't hide under the menu bar
#endif
    int w = 800;
    int h = 600;

    // load window size and position
    wxRect rect(Manager::Get()->GetConfigManager(_T("app"))->ReadInt(_T("/main_frame/layout/left"), x),
                Manager::Get()->GetConfigManager(_T("app"))->ReadInt(_T("/main_frame/layout/top"), y),
                Manager::Get()->GetConfigManager(_T("app"))->ReadInt(_T("/main_frame/layout/width"), w),
                Manager::Get()->GetConfigManager(_T("app"))->ReadInt(_T("/main_frame/layout/height"), h));
    // maximize if needed
    Maximize(Manager::Get()->GetConfigManager(_T("app"))->ReadBool(_T("/main_frame/layout/maximized"), true));
    // set size and position
    SetSize(rect);

    // close message manager (if auto-hiding)
//    Manager::Get()->GetLogManager()->Close();
}

void MainFrame::SaveWindowState()
{
    DoCheckCurrentLayoutForChanges(false);

    // first delete all previos layouts, otherwise they might remain
    // if the new amount of layouts is less than the previous, because only the first layouts will be overwritten
    wxArrayString subs = Manager::Get()->GetConfigManager(_T("app"))->EnumerateSubPaths(_T("/main_frame/layout"));
    for (size_t i = 0; i < subs.GetCount(); ++i)
    {
        Manager::Get()->GetConfigManager(_T("app"))->DeleteSubPath(_T("/main_frame/layout/") + subs[i]);
    }

    int count = 0;
    for (LayoutViewsMap::iterator it = m_LayoutViews.begin(); it != m_LayoutViews.end(); ++it)
    {
        if (it->first.IsEmpty())
            continue;
        ++count;
        wxString key = wxString::Format(_T("/main_frame/layout/view%d/"), count);
        Manager::Get()->GetConfigManager(_T("app"))->Write(key + _T("name"), it->first);
        Manager::Get()->GetConfigManager(_T("app"))->Write(key + _T("data"), it->second);
    }

    // save manager and messages selected page
    Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/main_frame/layout/left_block_selection"), Manager::Get()->GetProjectManager()->GetNotebook()->GetSelection());
    Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/main_frame/layout/bottom_block_selection"), m_pInfoPane->GetSelection());

    // save window size and position
    Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/main_frame/layout/maximized"), IsMaximized());
    if (!IsMaximized() && !IsIconized())
    {
        Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/main_frame/layout/left"), GetPosition().x);
        Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/main_frame/layout/top"), GetPosition().y);
        Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/main_frame/layout/width"), GetSize().x);
        Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/main_frame/layout/height"), GetSize().y);
    }
}

void MainFrame::LoadViewLayout(const wxString& name, bool isTemp)
{
    if (m_LastLayoutName != name && !DoCheckCurrentLayoutForChanges(true))
        return;

    m_LastLayoutIsTemp = isTemp;

    wxString layout = m_LayoutViews[name];
    if (layout.IsEmpty())
    {
        layout = m_LayoutViews[gDefaultLayout];
        DoSelectLayout(gDefaultLayout);
    }
    else
        DoSelectLayout(name);

    m_LayoutManager.LoadPerspective(layout, false);
    DoFixToolbarsLayout();
    DoUpdateLayout();

    m_LastLayoutName = name;
    m_LastLayoutData = layout;

    CodeBlocksLayoutEvent evt(cbEVT_SWITCHED_VIEW_LAYOUT);
    evt.layout = name;
    Manager::Get()->ProcessEvent(evt);
}

void MainFrame::SaveViewLayout(const wxString& name, const wxString& layout, bool select)
{
    if (name.IsEmpty())
        return;
    m_LayoutViews[name] = layout;
    wxMenu* viewLayouts = 0;
    GetMenuBar()->FindItem(idViewLayoutSave, &viewLayouts);
    if (viewLayouts && viewLayouts->FindItem(name) == wxNOT_FOUND)
    {
        int id = wxNewId();
        viewLayouts->InsertCheckItem(viewLayouts->GetMenuItemCount() - 3, id, name, wxString::Format(_("Switch to %s perspective"), name.c_str()));
        Connect( id,  wxEVT_COMMAND_MENU_SELECTED,
            (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)&MainFrame::OnViewLayout);
        m_PluginIDsMap[id] = name;
    }
    if (select)
    {
        DoSelectLayout(name);
        m_LastLayoutName = name;
    }
} // end of SaveViewLayout

bool MainFrame::LayoutDifferent(const wxString& layout1,const wxString& layout2,const wxString& delimiter)
{
    wxStringTokenizer strTok;
    unsigned long j;

    strTok.SetString(layout1, delimiter);
    wxArrayString arLayout1;
    while(strTok.HasMoreTokens())
    {
        wxStringTokenizer strTokColon(strTok.GetNextToken(), _T(";"));
        while(strTokColon.HasMoreTokens())
        {
            wxString theToken = strTokColon.GetNextToken();
            if (theToken.StartsWith(_T("state=")))
            {
                theToken=theToken.Right(theToken.Len() - wxString(_T("state=")).Len());
                theToken.ToULong(&j);
                // we filter out the hidden/show state
                theToken=wxString::Format(_("state=%d"),j & wxAuiPaneInfo::optionHidden);
            }
               arLayout1.Add(theToken);
        }
    }

    strTok.SetString(layout2, delimiter);
    wxArrayString arLayout2;
    while(strTok.HasMoreTokens())
    {
        wxStringTokenizer strTokColon(strTok.GetNextToken(), _T(";"));
        while(strTokColon.HasMoreTokens())
        {
            wxString theToken = strTokColon.GetNextToken();
            if (theToken.StartsWith(_T("state=")))
            {
                theToken=theToken.Right(theToken.Len() - wxString(_T("state=")).Len());
                theToken.ToULong(&j);
                // we filter out the hidden/show state
                theToken=wxString::Format(_("state=%d"),j & wxAuiPaneInfo::optionHidden);
            }
               arLayout2.Add(theToken);
        }
    }

    arLayout1.Sort();
    arLayout2.Sort();

    return arLayout1 != arLayout2;
}

bool MainFrame::DoCheckCurrentLayoutForChanges(bool canCancel)
{
    DoFixToolbarsLayout();
    wxString lastlayout = m_LayoutManager.SavePerspective();
    if (!m_LastLayoutName.IsEmpty() && LayoutDifferent(lastlayout, m_LastLayoutData))
    {
        AnnoyingDialog dlg(_("Layout changed"),
                            wxString::Format(_("The perspective '%s' has changed. Do you want to save it?"), m_LastLayoutName.c_str()),
                            wxART_QUESTION,
                            canCancel ? AnnoyingDialog::YES_NO_CANCEL : AnnoyingDialog::YES_NO,
                            wxID_YES);
        switch (dlg.ShowModal())
        {
            case wxID_YES:
                SaveViewLayout(m_LastLayoutName, lastlayout, false);
                break;
            case wxID_CANCEL:
                DoSelectLayout(m_LastLayoutName);
                return false;
            default:
                break;
        }
    }
    return true;
}

void MainFrame::DoFixToolbarsLayout()
{
    // because the user might change the toolbar icons size, we must cater for it...
    wxAuiPaneInfoArray& panes = m_LayoutManager.GetAllPanes();
    for (size_t i = 0; i < panes.GetCount(); ++i)
    {
        wxAuiPaneInfo& info = panes[i];
        if (info.state & wxAuiPaneInfo::optionToolbar)
        {
            info.best_size = info.window->GetSize();
        }
    }
}

void MainFrame::DoSelectLayout(const wxString& name)
{
    wxMenu* viewLayouts = 0;
    GetMenuBar()->FindItem(idViewLayoutSave, &viewLayouts);
    if (viewLayouts)
    {
        wxMenuItemList& items = viewLayouts->GetMenuItems();
        for (size_t i = 0; i < items.GetCount(); ++i)
        {
            if (!items[i]->IsCheckable())
                continue;
            #if wxCHECK_VERSION(2, 9, 0)
            items[i]->Check(items[i]->GetItemLabel().IsSameAs(name));
            #else
            items[i]->Check(items[i]->GetText().IsSameAs(name));
            #endif
        }

        if (!m_LastLayoutIsTemp)
            Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/main_frame/layout/default"), name);
    }
}

void MainFrame::DoAddPluginToolbar(cbPlugin* plugin)
{
    wxSize size = m_SmallToolBar ? wxSize(16, 16) : (platform::macosx ? wxSize(32, 32) : wxSize(22, 22));
    wxToolBar* tb = new wxToolBar(this, -1, wxDefaultPosition, size, wxTB_FLAT | wxTB_NODIVIDER);
    tb->SetToolBitmapSize(size);
    if (plugin->BuildToolBar(tb))
    {
        SetToolBar(0);

#if defined __WXMSW__ && !wxCHECK_VERSION(2, 8, 9)
        // HACK: for all windows versions (including XP *without* using a manifest file),
        //       the best size for a toolbar is not correctly calculated by wxWidgets/wxAUI/whatever.
        //       so we try to help the situation a little. It's not perfect, but it works.
        // not needed for versions >= 2.8.9: fixed in upstream, toolbars with standard-controls
        // are much too large with it (at least on w2k).
        if (!UsesCommonControls6()) // all windows versions, including XP without a manifest file
        {
            // calculate the total width of all wxWindow* in the toolbar (if any)
            int w = 0;
            int ccount = 0;
            for (wxWindowList::compatibility_iterator node = tb->GetChildren().GetFirst(); node; node = node->GetNext())
            {
                wxWindow *win = (wxWindow *)node->GetData();
                if (win)
                {
                    w += win->GetSize().GetWidth();
                    ++ccount;
                }
            }
            #if wxCHECK_VERSION(2, 8, 0)
            wxSize s(w + tb->GetEffectiveMinSize().GetWidth() - (ccount * (tb->GetToolSize().GetWidth() / 3)), 0);
            tb->SetInitialSize(s);
            #else
            wxSize s(w + tb->GetBestFittingSize().GetWidth() - (ccount * (tb->GetToolSize().GetWidth() / 3)), 0);
            tb->SetBestFittingSize(s);
            #endif
        }
        else
            tb->SetInitialSize();
        // end of HACK
#else
        tb->SetInitialSize();
#endif

        // add View->Toolbars menu item for toolbar
        wxMenu* viewToolbars = 0;
        GetMenuBar()->FindItem(idViewToolMain, &viewToolbars);
        if (viewToolbars)
        {
            wxMenuItem* item = AddPluginInMenus(viewToolbars, plugin,
                                                (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)&MainFrame::OnToggleBar,
                                                -1, true);
            if (item)
            {
                item->Check(true);
                m_PluginsTools[plugin] = tb;
            }
        }

        const PluginInfo* info = Manager::Get()->GetPluginManager()->GetPluginInfo(plugin);
        if (!info)
            cbThrow(_T("No plugin info?!?"));

        static int row = 1;
        m_LayoutManager.AddPane(tb, wxAuiPaneInfo().
                              Name(info->name + _T("Toolbar")).Caption(info->title + _(" Toolbar")).
                              ToolbarPane().Top().Row(row++));
        DoUpdateLayout();
    }
    else
        delete tb;
}

void MainFrame::DoAddPlugin(cbPlugin* plugin)
{
    //Manager::Get()->GetLogManager()->DebugLog(_T("Adding plugin: %s"), plugin->GetInfo()->name.c_str());
    AddPluginInSettingsMenu(plugin);
    AddPluginInHelpPluginsMenu(plugin);
    if (plugin->GetType() == ptTool)
    {
        AddPluginInPluginsMenu(plugin);
    }
    // offer menu and toolbar space for other plugins
    else
    {
        // menu
        try
        {
            wxMenuBar* mbar = GetMenuBar();
            plugin->BuildMenu(mbar);
        }
        catch (cbException& e)
        {
            e.ShowErrorMessage();
        }
        // toolbar
        DoAddPluginToolbar(plugin);
    }
}

bool MainFrame::Open(const wxString& filename, bool addToHistory)
{
    wxFileName fn(filename);
    fn.Normalize(); // really important so that two same files with different names are not loaded twice
    wxString name = fn.GetFullPath();
    //Manager::Get()->GetLogManager()->DebugLog(_T("Opening file '%s'"), sname.c_str());
    Manager::Get()->GetLogManager()->DebugLog(_T("Opening file ") + name);
    bool ret = OpenGeneric(name, addToHistory);
    return ret;
}

wxString MainFrame::ShowOpenFileDialog(const wxString& caption, const wxString& filter)
{
    wxFileDialog dlg(this,
                     caption,
                     wxEmptyString,
                     wxEmptyString,
                     filter,
                     wxFD_OPEN | compatibility::wxHideReadonly);
    wxString sel;
    PlaceWindow(&dlg);
    if (dlg.ShowModal() == wxID_OK)
        sel = dlg.GetPath();
    return sel;
}

bool MainFrame::OpenGeneric(const wxString& filename, bool addToHistory)
{
    if (filename.IsEmpty())
        return false;
    wxFileName fname(filename);
    fname.ClearExt();
    fname.SetExt(_T("cbp"));
    switch(FileTypeOf(filename))
    {
        //
        // Workspace
        //
        case ftCodeBlocksWorkspace:
            // verify that it's not the same as the one already open
            if (filename != Manager::Get()->GetProjectManager()->GetWorkspace()->GetFilename() &&
                DoCloseCurrentWorkspace())
            {
                wxBusyCursor wait; // loading a worspace can take some time -> showhourglass
                ShowHideStartPage(true); // hide startherepage, so we can use full tab-range
                bool ret = Manager::Get()->GetProjectManager()->LoadWorkspace(filename);
                if (!ret)
                {
                    ShowHideStartPage(); // show/hide startherepage, dependant of settings, if loading failed
                }
                else if (addToHistory)
                {
                    AddToRecentProjectsHistory(Manager::Get()->GetProjectManager()->GetWorkspace()->GetFilename());
                }
                return ret;
            }
            else
            {
                return false;
            }
            break;

        //
        // Project
        //
        case ftCodeBlocksProject:
        {
            // Make a check whether the project exists in current workspace
            cbProject* prj = Manager::Get()->GetProjectManager()->IsOpen(fname.GetFullPath());
            if (!prj)
            {
                wxBusyCursor wait; // loading a worspace can take some time -> showhourglass
                return DoOpenProject(filename, addToHistory);
            }
            else
            {
                // NOTE (Morten#1#): A message here will prevent batch-builds from working and is shown sometimes even if correct
                Manager::Get()->GetProjectManager()->SetProject(prj, false);
                return true;
            }
        }
        //
        // Source files
        //
        case ftHeader:
            // fallthrough
        case ftSource:
            // fallthrough
        case ftResource:
            return DoOpenFile(filename, addToHistory);
        //
        // For all other files, ask MIME plugin for a suitable handler
        //
        default:
        {
            cbMimePlugin* plugin = Manager::Get()->GetPluginManager()->GetMIMEHandlerForFile(filename);
            // warn user that "Files extension handler" is disabled
            if (!plugin)
            {
                cbMessageBox(_("Could not open file ") + filename + _(",\nbecause no extension handler could be found."), _("Error"), wxICON_ERROR);
                return false;
            }
            if (plugin->OpenFile(filename) == 0)
            {
                AddToRecentFilesHistory(filename);
                return true;
            }
            return false;
        }
    }
    return true;
}

bool MainFrame::DoOpenProject(const wxString& filename, bool addToHistory)
{
//    Manager::Get()->GetLogManager()->DebugLog(_T("Opening project '%s'"), filename.c_str());
    if (!wxFileExists(filename))
    {
        cbMessageBox(_("The project file does not exist..."), _("Error"), wxICON_ERROR);
        return false;
    }

    ShowHideStartPage(true); // hide startherepage, so we can use full tab-range
    cbProject* prj = Manager::Get()->GetProjectManager()->LoadProject(filename, true);
    if (prj)
    {
        if (addToHistory)
        {
            AddToRecentProjectsHistory(prj->GetFilename());
        }
        return true;
    }
    ShowHideStartPage(); // show/hide startherepage, dependant of settings, if loading failed
    return false;
}

bool MainFrame::DoOpenFile(const wxString& filename, bool addToHistory)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->Open(filename);
    if (ed)
    {
        if (addToHistory)
            AddToRecentFilesHistory(ed->GetFilename());
        return true;
    }
    return false;
}

bool MainFrame::DoCloseCurrentWorkspace()
{
    return Manager::Get()->GetProjectManager()->CloseWorkspace();
}

void MainFrame::DoCreateStatusBar()
{
#if wxUSE_STATUSBAR
    wxCoord width[16]; // 16 max

    wxClientDC dc(this);
    wxFont font = dc.GetFont();
    int h;
    int num = 0;

    width[num++] = -1; // main field
//    width[num++] = 128; // progress bar
    dc.GetTextExtent(_(" WINDOWS-1252 "), &width[num++], &h);
    dc.GetTextExtent(_(" Line 12345, Column 123 "), &width[num++], &h);
    dc.GetTextExtent(_(" Overwrite "),  &width[num++], &h);
    dc.GetTextExtent(_(" Modified "),   &width[num++], &h);
    dc.GetTextExtent(_(" Read/Write....."), &width[num++], &h);
    dc.GetTextExtent(_(" name_of_profile "), &width[num++], &h);

    CreateStatusBar(num);
    SetStatusWidths(num, width);

    // here for later usage
//    m_pProgressBar = new wxGauge(GetStatusBar(), -1, 100);
#endif // wxUSE_STATUSBAR
}

void MainFrame::DoUpdateStatusBar()
{
#if wxUSE_STATUSBAR
    if (!GetStatusBar())
        return;
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    wxString personality(Manager::Get()->GetPersonalityManager()->GetPersonality());
    if (ed)
    {
        int panel = 0;
        int pos = ed->GetControl()->GetCurrentPos();
        wxString msg;
        msg.Printf(_("Line %d, Column %d"), ed->GetControl()->GetCurrentLine() + 1, ed->GetControl()->GetColumn(pos) + 1);
        SetStatusText(ed->GetFilename(), panel++);                    //tiwag 050917
        SetStatusText(ed->GetEncodingName(), panel++);
        SetStatusText(msg, panel++);
        SetStatusText(ed->GetControl()->GetOvertype() ? _("Overwrite") : _("Insert"), panel++);
        #if wxCHECK_VERSION(2, 9, 0)
        SetStatusText(ed->GetModified() ? _("Modified") : _T(""), panel++);
        #else
        SetStatusText(ed->GetModified() ? _("Modified") : wxEmptyString, panel++);
        #endif
        SetStatusText(ed->GetControl()->GetReadOnly() ? _("Read only") : _("Read/Write"), panel++);
        SetStatusText(personality, panel++);
    }
    else
    {
        int panel = 0;
        SetStatusText(_("Welcome to ") + appglobals::AppName + _T("!"), panel++);
        SetStatusText(wxEmptyString, panel++);
        SetStatusText(wxEmptyString, panel++);
        SetStatusText(wxEmptyString, panel++);
        SetStatusText(wxEmptyString, panel++);
        SetStatusText(wxEmptyString, panel++);
        SetStatusText(personality, panel++);
    }
#endif // wxUSE_STATUSBAR
}

void MainFrame::DoUpdateEditorStyle(cbAuiNotebook* target, const wxString& prefix, long defaultStyle)
{
    if (!target)
        return;

    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("app"));
    target->SetTabCtrlHeight(-1);

    long nbstyle = cfg->ReadInt(_T("/environment/tabs_style"), 0);
    switch (nbstyle)
    {
        case 1: // simple style
            target->SetArtProvider(new wxAuiSimpleTabArt());
            break;

        case 2: // VC 7.1 style
            target->SetArtProvider(new NbStyleVC71());
            break;

        case 3: // Firefox 2 style
            target->SetArtProvider(new NbStyleFF2());
            break;

        default: // default style
            target->SetArtProvider(new wxAuiDefaultTabArt());
            break;
    }

    nbstyle = defaultStyle;
    if (cfg->ReadBool(_T("/environment/") + prefix + _T("_tabs_bottom")))
        nbstyle |= wxAUI_NB_BOTTOM;

    if (cfg->ReadBool(_T("/environment/tabs_list")))
        nbstyle |= wxAUI_NB_WINDOWLIST_BUTTON;

    target->SetWindowStyleFlag(nbstyle);
}

void MainFrame::DoUpdateEditorStyle()
{
    long closestyle = Manager::Get()->GetConfigManager(_T("app"))->ReadBool(_T("/environment/tabs_close_on_all"))?wxAUI_NB_CLOSE_ON_ALL_TABS:0;
    cbAuiNotebook* an = Manager::Get()->GetEditorManager()->GetNotebook();

    DoUpdateEditorStyle(an, _T("editor"), closestyle | wxAUI_NB_DEFAULT_STYLE | wxNO_FULL_REPAINT_ON_RESIZE | wxCLIP_CHILDREN);

    an = m_pInfoPane;
    DoUpdateEditorStyle(an, _T("message"), closestyle | wxAUI_NB_DEFAULT_STYLE);

    an = Manager::Get()->GetProjectManager()->GetNotebook();
    DoUpdateEditorStyle(an, _T("project"), wxAUI_NB_SCROLL_BUTTONS | wxAUI_NB_TAB_MOVE);
}

void MainFrame::DoUpdateLayoutColours()
{
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("app"));
    wxAuiDockArt* art = m_LayoutManager.GetArtProvider();

#ifndef __WXGTK__
    m_LayoutManager.SetFlags(wxAUI_MGR_DEFAULT | wxAUI_MGR_ALLOW_ACTIVE_PANE | wxAUI_MGR_TRANSPARENT_DRAG);
#else // #ifndef __WXGTK__
    // workaround for a wxWidgets-bug that makes C::B crash when a floating window gets docked and composite-effects are enabled
    m_LayoutManager.SetFlags((wxAUI_MGR_DEFAULT | wxAUI_MGR_ALLOW_ACTIVE_PANE | wxAUI_MGR_TRANSPARENT_DRAG | wxAUI_MGR_VENETIAN_BLINDS_HINT)& ~wxAUI_MGR_TRANSPARENT_HINT);
#endif // #ifndef __WXGTK__

    art->SetMetric(wxAUI_DOCKART_PANE_BORDER_SIZE,   cfg->ReadInt(_T("/environment/aui/border_size"), art->GetMetric(wxAUI_DOCKART_PANE_BORDER_SIZE)));
    art->SetMetric(wxAUI_DOCKART_SASH_SIZE,          cfg->ReadInt(_T("/environment/aui/sash_size"), art->GetMetric(wxAUI_DOCKART_SASH_SIZE)));
    art->SetMetric(wxAUI_DOCKART_CAPTION_SIZE,       cfg->ReadInt(_T("/environment/aui/caption_size"), art->GetMetric(wxAUI_DOCKART_CAPTION_SIZE)));
    art->SetColour(wxAUI_DOCKART_ACTIVE_CAPTION_COLOUR,              cfg->ReadColour(_T("/environment/aui/active_caption_colour"), art->GetColour(wxAUI_DOCKART_ACTIVE_CAPTION_COLOUR)));
    art->SetColour(wxAUI_DOCKART_ACTIVE_CAPTION_GRADIENT_COLOUR,     cfg->ReadColour(_T("/environment/aui/active_caption_gradient_colour"), art->GetColour(wxAUI_DOCKART_ACTIVE_CAPTION_GRADIENT_COLOUR)));
    art->SetColour(wxAUI_DOCKART_ACTIVE_CAPTION_TEXT_COLOUR,         cfg->ReadColour(_T("/environment/aui/active_caption_text_colour"), art->GetColour(wxAUI_DOCKART_ACTIVE_CAPTION_TEXT_COLOUR)));
    art->SetColour(wxAUI_DOCKART_INACTIVE_CAPTION_COLOUR,            cfg->ReadColour(_T("/environment/aui/inactive_caption_colour"), art->GetColour(wxAUI_DOCKART_INACTIVE_CAPTION_COLOUR)));
    art->SetColour(wxAUI_DOCKART_INACTIVE_CAPTION_GRADIENT_COLOUR,   cfg->ReadColour(_T("/environment/aui/inactive_caption_gradient_colour"), art->GetColour(wxAUI_DOCKART_INACTIVE_CAPTION_GRADIENT_COLOUR)));
    art->SetColour(wxAUI_DOCKART_INACTIVE_CAPTION_TEXT_COLOUR,       cfg->ReadColour(_T("/environment/aui/inactive_caption_text_colour"), art->GetColour(wxAUI_DOCKART_INACTIVE_CAPTION_TEXT_COLOUR)));

    DoUpdateLayout();
}

void MainFrame::DoUpdateLayout()
{
    if (!m_StartupDone)
        return;
    m_LayoutManager.Update();
}

void MainFrame::DoUpdateAppTitle()
{
    EditorBase* ed = Manager::Get()->GetEditorManager() ? Manager::Get()->GetEditorManager()->GetActiveEditor() : 0L;
    cbProject* prj = 0;
    if (ed && ed->IsBuiltinEditor())
    {
        ProjectFile* prjf = ((cbEditor*)ed)->GetProjectFile();
        if (prjf)
            prj = prjf->GetParentProject();
    }
    else
        prj = Manager::Get()->GetProjectManager() ? Manager::Get()->GetProjectManager()->GetActiveProject() : 0L;
    wxString projname;
    wxString edname;
    wxString fulltitle;
    if (ed || prj)
    {
        if (prj)
        {
            if (Manager::Get()->GetProjectManager()->GetActiveProject() == prj)
                projname = wxString(_T(" [")) + prj->GetTitle() + _T("]");
            else
                projname = wxString(_T(" (")) + prj->GetTitle() + _T(")");
        }
        if (ed)
            edname = ed->GetTitle();
        fulltitle = edname + projname;
        if (!fulltitle.IsEmpty())
            fulltitle.Append(_T(" - "));
    }
    fulltitle.Append(appglobals::AppName);
    fulltitle.Append(_T(" "));
    fulltitle.Append(appglobals::AppVersion);
    SetTitle(fulltitle);
}

void MainFrame::ShowHideStartPage(bool forceHasProject)
{
    if (Manager::IsBatchBuild())
        return;

    // we use the 'forceHasProject' param because when a project is opened
    // the EVT_PROJECT_OPEN event is fired *before* ProjectManager::GetProjects()
    // and ProjectManager::GetActiveProject() are updated...

    if (m_InitiatedShutdown)
    {
        EditorBase* sh = Manager::Get()->GetEditorManager()->GetEditor(g_StartHereTitle);
        if (sh)
            sh->Destroy();
        return;
    }

    bool show = !forceHasProject &&
                Manager::Get()->GetProjectManager()->GetProjects()->GetCount() == 0 &&
                Manager::Get()->GetConfigManager(_T("app"))->ReadBool(_T("/environment/start_here_page"), true);

    EditorBase* sh = Manager::Get()->GetEditorManager()->GetEditor(g_StartHereTitle);
    if (show && !sh)
        sh = new StartHerePage(this, Manager::Get()->GetEditorManager()->GetNotebook());
    else if (!show && sh)
        sh->Destroy();

    DoUpdateAppTitle();
}

void MainFrame::ShowHideScriptConsole()
{
    if (Manager::IsBatchBuild())
        return;
    bool isVis = IsWindowReallyShown(m_pScriptConsole);
    CodeBlocksDockEvent evt(isVis ? cbEVT_HIDE_DOCK_WINDOW : cbEVT_SHOW_DOCK_WINDOW);
    evt.pWindow = m_pScriptConsole;
    Manager::Get()->ProcessEvent(evt);
}

void MainFrame::OnStartHereLink(wxCommandEvent& event)
{
    wxCommandEvent evt;
    evt.SetId(idFileNewProject);
    wxString link = event.GetString();
    if (link.IsSameAs(_T("CB_CMD_NEW_PROJECT")))
        OnFileNewWhat(evt);
    else if (link.IsSameAs(_T("CB_CMD_OPEN_PROJECT")))
        DoOnFileOpen(true);
//    else if (link.IsSameAs(_T("CB_CMD_CONF_ENVIRONMENT")))
//        OnSettingsEnvironment(evt);
//    else if (link.IsSameAs(_T("CB_CMD_CONF_EDITOR")))
//        Manager::Get()->GetEditorManager()->Configure();
//    else if (link.IsSameAs(_T("CB_CMD_CONF_COMPILER")))
//        OnSettingsCompilerDebugger(evt);
    else if (link.StartsWith(_T("CB_CMD_OPEN_HISTORY_")))
    {
        wxFileHistory* hist = link.StartsWith(_T("CB_CMD_OPEN_HISTORY_PROJECT_")) ? m_pProjectsHistory : m_pFilesHistory;
        unsigned long count;
        link.AfterLast(_T('_')).ToULong(&count);
        --count;
        if (count < hist->GetCount())
        {
            if (!OpenGeneric(hist->GetHistoryFile(count), true))
            {
                AskToRemoveFileFromHistory(hist, count);
            }
        }
    }
    else if (link.StartsWith(_T("CB_CMD_DELETE_HISTORY_")))
    {
        wxFileHistory* hist = link.StartsWith(_T("CB_CMD_DELETE_HISTORY_PROJECT_")) ? m_pProjectsHistory : m_pFilesHistory;
        unsigned long count;
        link.AfterLast(_T('_')).ToULong(&count);
        --count;
        if (count < hist->GetCount())
        {
            AskToRemoveFileFromHistory(hist, count, false);
        }
    }
}

void MainFrame::AskToRemoveFileFromHistory(wxFileHistory* hist, int id, bool cannot_open)
{
    wxString question(_("Do you want to remove it from the recent files list?"));
    wxString query(wxEmptyString);
    if (cannot_open)
    {
        query << _("The file cannot be opened (probably it's not available anymore).")
              << _T("\n") << question;
    }
    else
    {
        query << question;
    }


    if (cbMessageBox(query, _("Question"), wxYES_NO | wxICON_QUESTION) == wxID_YES)
    {
        hist->RemoveFileFromHistory(id);
        // update start here page
        EditorBase* sh = Manager::Get()->GetEditorManager()->GetEditor(g_StartHereTitle);
        if (sh)
            ((StartHerePage*)sh)->Reload();
    }
}

void MainFrame::OnStartHereVarSubst(wxCommandEvent& event)
{
    EditorBase* sh = Manager::Get()->GetEditorManager()->GetEditor(g_StartHereTitle);
    if (!sh)
        return;

    // replace history vars

    wxString buf = event.GetString();
    wxString links;

    links << _T("<table>\n<tr><td colspan=\"2\"><b>");
    links << _("Recent projects");
    links << _T("</b></td></tr>\n");
    if (m_pProjectsHistory->GetCount())
    {
        for (size_t i = 0; i < m_pProjectsHistory->GetCount(); ++i)
        {
            links << _T("<tr><td width=\"50\"><img alt=\"\" width=\"20\" src=\"blank.png\" />");
            links << wxString::Format(_T("<a href=\"CB_CMD_DELETE_HISTORY_PROJECT_%d\"><img alt=\"\" src=\"trash_16x16.png\" /></a>"),
                                      i + 1);
            links << _T("<img alt=\"\"  width=\"10\" src=\"blank.png\" /></td><td width=\"10\">");
            links << wxString::Format(_T("<a href=\"CB_CMD_OPEN_HISTORY_PROJECT_%d\">%s</a>"),
                                      i + 1, m_pProjectsHistory->GetHistoryFile(i).c_str());
            links << _T("</td></tr>\n");
        }
    }
    else
    {
        links << _T("<tr><td style=\"width:2em;\"></td><td>&nbsp;&nbsp;&nbsp;&nbsp;");
        links << _("No recent projects");
        links << _T("</td></tr>\n");
    }

    links << _T("</table>\n<table>\n<tr><td colspan=\"2\"><b>");
    links << _("Recent files");
    links <<_T("</b></td></tr>\n");
    if (m_pFilesHistory->GetCount())
    {
        for (size_t i = 0; i < m_pFilesHistory->GetCount(); ++i)
        {
            links << _T("<tr><td width=\"50\"><img alt=\"\" width=\"20\" src=\"blank.png\" />");
            links << wxString::Format(_T("<a href=\"CB_CMD_DELETE_HISTORY_FILE_%d\"><img alt=\"\" src=\"trash_16x16.png\" /></a>"),
                                      i + 1);
            links << _T("<img alt=\"\"  width=\"10\" src=\"blank.png\" /></td><td width=\"10\">");
            links << wxString::Format(_T("<a href=\"CB_CMD_OPEN_HISTORY_FILE_%d\">%s</a>"),
                                      i + 1, m_pFilesHistory->GetHistoryFile(i).c_str());
            links << _T("</td></tr>\n");
        }
    }
    else
    {
        links << _T("<tr><td style=\"width:2em;\"></td><td>&nbsp;&nbsp;&nbsp;&nbsp;");
        links << _("No recent files");
        links << _T("</td></tr>\n");
    }

    links << _T("</table>\n");


    // update page
    buf.Replace(_T("CB_VAR_RECENT_FILES_AND_PROJECTS"), links);
    buf.Replace(_T("CB_TXT_NEW_PROJECT"), _("Create a new project"));
    buf.Replace(_T("CB_TXT_OPEN_PROJECT"), _("Open an existing project"));
    buf.Replace(_T("CB_TXT_VISIT_FORUMS"), _("Visit the Code::Blocks forums"));
    buf.Replace(_T("CB_TXT_REPORT_BUG"), _("Report a bug"));
    buf.Replace(_T("CB_TXT_REQ_NEW_FEATURE"), _("Request a new feature"));
    ((StartHerePage*)sh)->SetPageContent(buf);
}

void MainFrame::InitializeRecentFilesHistory()
{
    TerminateRecentFilesHistory();

    wxMenuBar* mbar = GetMenuBar();
    if (!mbar)
        return;
    int pos = mbar->FindMenu(_("&File"));
    if (pos != wxNOT_FOUND)
    {
        m_pFilesHistory = new wxFileHistory(9, wxID_FILE1);

        wxMenu* menu = mbar->GetMenu(pos);
        if (!menu)
            return;
        wxMenu* recentFiles = 0;
        menu->FindItem(idFileOpenRecentFileClearHistory, &recentFiles);
        if (recentFiles)
        {
            wxArrayString files = Manager::Get()->GetConfigManager(_T("app"))->ReadArrayString(_T("/recent_files"));
            for (int i = (int)files.GetCount() - 1; i >= 0; --i)
            {
                if (wxFileExists(files[i]))
                {
                    m_pFilesHistory->AddFileToHistory(files[i]);
                }
            }
            if (m_pFilesHistory->GetCount() > 0)
            {
                recentFiles->InsertSeparator(0);
                for (size_t i = 0; i < m_pFilesHistory->GetCount(); ++i)
                {
                    recentFiles->Insert(recentFiles->GetMenuItemCount() - 2, wxID_FILE1 + i,
                        wxString::Format(_T("&%d "), i + 1) + m_pFilesHistory->GetHistoryFile(i));
                }
            }
        }
        wxMenu* recentProjects = 0;
        menu->FindItem(idFileOpenRecentProjectClearHistory, &recentProjects);
        if (recentProjects)
        {
            m_pProjectsHistory = new wxFileHistory(9, wxID_FILE10);

            wxArrayString files = Manager::Get()->GetConfigManager(_T("app"))->ReadArrayString(_T("/recent_projects"));
            for (int i = (int)files.GetCount() - 1; i >= 0; --i)
            {
                if (wxFileExists(files[i]))
                    m_pProjectsHistory->AddFileToHistory(files[i]);
            }
            if (m_pProjectsHistory->GetCount() > 0)
            {
                recentProjects->InsertSeparator(0);
                for (size_t i = 0; i < m_pProjectsHistory->GetCount(); ++i)
                {
                    recentProjects->Insert(recentProjects->GetMenuItemCount() - 2, wxID_FILE10 + i,
                        wxString::Format(_T("&%d "), i + 1) + m_pProjectsHistory->GetHistoryFile(i));
                }
            }
        }
    }
}

void MainFrame::AddToRecentFilesHistory(const wxString& FileName)
{
    wxString filename = FileName;
#ifdef __WXMSW__
    // for windows, look for case-insensitive matches
    // if found, don't add it
    wxString low = filename.Lower();
    for (size_t i = 0; i < m_pFilesHistory->GetCount(); ++i)
    {
        if (low == m_pFilesHistory->GetHistoryFile(i).Lower())
        {    // it exists, set filename to the existing name, so it can become
            // the most recent one
            filename = m_pFilesHistory->GetHistoryFile(i);
            break;
        }
    }
#endif

    m_pFilesHistory->AddFileToHistory(filename);

    // because we append "clear history" menu to the end of the list,
    // each time we must add a history item we have to:
    // a) remove "Clear history" (Biplab#1: Don't remove or you'll loose icon)
    // b) clear the menu (Biplab#1: except the last item)
    // c) fill it with the history items (Biplab#1: by inserting them)
    // and d) append "Clear history"... (Biplab#1: Not needed, item has not been removed)
    wxMenuBar* mbar = GetMenuBar();
    if (!mbar)
        return;
    int pos = mbar->FindMenu(_("&File"));
    if (pos == wxNOT_FOUND)
        return;
    wxMenu* menu = mbar->GetMenu(pos);
    if (!menu)
        return;
    wxMenu* recentFiles = 0;
    menu->FindItem(idFileOpenRecentFileClearHistory, &recentFiles);
    if (recentFiles)
    {
        while (recentFiles->GetMenuItemCount() > 1)
        {
            recentFiles->Delete(recentFiles->GetMenuItems()[0]);
        }
        if (m_pFilesHistory->GetCount() > 0)
        {
            recentFiles->InsertSeparator(0);
            for (size_t i = 0; i < m_pFilesHistory->GetCount(); ++i)
            {
                recentFiles->Insert(recentFiles->GetMenuItemCount() - 2, wxID_FILE1 + i,
                    wxString::Format(_T("&%d "), i + 1) + m_pFilesHistory->GetHistoryFile(i));
            }
        }
    }

    // update start here page
    EditorBase* sh = Manager::Get()->GetEditorManager()->GetEditor(g_StartHereTitle);
    if (sh)
        ((StartHerePage*)sh)->Reload();
}

void MainFrame::AddToRecentProjectsHistory(const wxString& FileName)
{
    wxString filename = FileName;
#ifdef __WXMSW__
    // for windows, look for case-insensitive matches
    // if found, don't add it
    wxString low = filename.Lower();
    for (size_t i = 0; i < m_pProjectsHistory->GetCount(); ++i)
    {
        if (low == m_pProjectsHistory->GetHistoryFile(i).Lower())
        {    // it exists, set filename to the existing name, so it can become
            // the most recent one
            filename = m_pProjectsHistory->GetHistoryFile(i);
            break;
        }
    }
#endif

    m_pProjectsHistory->AddFileToHistory(filename);

    // because we append "clear history" menu to the end of the list,
    // each time we must add a history item we have to:
    // a) remove "Clear history"
    // b) clear the menu
    // c) fill it with the history items
    // and d) append "Clear history"...
    wxMenuBar* mbar = GetMenuBar();
    if (!mbar)
        return;
    int pos = mbar->FindMenu(_("&File"));
    if (pos == wxNOT_FOUND)
        return;
    wxMenu* menu = mbar->GetMenu(pos);
    if (!menu)
        return;
    wxMenu* recentProjects = 0;
    menu->FindItem(idFileOpenRecentProjectClearHistory, &recentProjects);
    if (recentProjects)
    {
        while (recentProjects->GetMenuItemCount() > 1)
        {
            recentProjects->Delete(recentProjects->GetMenuItems()[0]);
        }
        if (m_pProjectsHistory->GetCount() > 0)
        {
            recentProjects->InsertSeparator(0);
            for (size_t i = 0; i < m_pProjectsHistory->GetCount(); ++i)
            {
                recentProjects->Insert(recentProjects->GetMenuItemCount() - 2, wxID_FILE10 + i,
                    wxString::Format(_T("&%d "), i + 1) + m_pProjectsHistory->GetHistoryFile(i));
            }
        }
    }

    // update start here page
    EditorBase* sh = Manager::Get()->GetEditorManager()->GetEditor(g_StartHereTitle);
    if (sh)
        ((StartHerePage*)sh)->Reload();
}

void MainFrame::TerminateRecentFilesHistory()
{
    if (m_pFilesHistory)
    {
        wxArrayString files;
        for (unsigned int i = 0; i < m_pFilesHistory->GetCount(); ++i)
            files.Add(m_pFilesHistory->GetHistoryFile(i));
        Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/recent_files"), files);

        wxMenuBar* mbar = GetMenuBar();
        if (mbar)
        {
            int pos = mbar->FindMenu(_("&File"));
            if (pos != wxNOT_FOUND)
            {
                wxMenu* menu = mbar->GetMenu(pos);
                if (menu)
                {
                    wxMenu* recentFiles = 0;
                    menu->FindItem(idFileOpenRecentFileClearHistory, &recentFiles);
                    if (recentFiles)
                    {
                        if (!Manager::IsAppShuttingDown())
                        {
                            while (recentFiles->GetMenuItemCount() > 1)
                            {
                                recentFiles->Delete(recentFiles->GetMenuItems()[0]);
                            }
                        }
                        else
                        {
                            m_pFilesHistory->RemoveMenu(recentFiles);
                        }
                    }
                }
            }
        }
        delete m_pFilesHistory;
        m_pFilesHistory = 0;
    }

    if (m_pProjectsHistory)
    {
        wxArrayString files;
        for (unsigned int i = 0; i < m_pProjectsHistory->GetCount(); ++i)
        {
            files.Add(m_pProjectsHistory->GetHistoryFile(i));
        }
        Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/recent_projects"), files);

        wxMenuBar* mbar = GetMenuBar();
        if (mbar)
        {
            int pos = mbar->FindMenu(_("&File"));
            if (pos != wxNOT_FOUND)
            {
                wxMenu* menu = mbar->GetMenu(pos);
                if (menu)
                {
                    wxMenu* recentProjects = 0;
                    menu->FindItem(idFileOpenRecentProjectClearHistory, &recentProjects);
                    if (recentProjects)
                    {
                        if (!Manager::IsAppShuttingDown())
                        {
                            while (recentProjects->GetMenuItemCount() > 1)
                            {
                                recentProjects->Delete(recentProjects->GetMenuItems()[0]);
                            }
                        }
                        else
                        {
                            m_pProjectsHistory->RemoveMenu(recentProjects);
                        }
                    }
                }
            }
        }
        delete m_pProjectsHistory;
        m_pProjectsHistory = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
// event handlers
////////////////////////////////////////////////////////////////////////////////

void MainFrame::OnPluginsExecuteMenu(wxCommandEvent& event)
{
    wxString pluginName = m_PluginIDsMap[event.GetId()];
    if (!pluginName.IsEmpty())
        Manager::Get()->GetPluginManager()->ExecutePlugin(pluginName);
    else
        Manager::Get()->GetLogManager()->DebugLog(F(_T("No plugin found for ID %d"), event.GetId()));
}

void MainFrame::OnPluginSettingsMenu(wxCommandEvent& event)
{
    wxString pluginName = m_PluginIDsMap[event.GetId()];
    if (!pluginName.IsEmpty())
        Manager::Get()->GetPluginManager()->ConfigurePlugin(pluginName);
    else
        Manager::Get()->GetLogManager()->DebugLog(F(_T("No plugin found for ID %d"), event.GetId()));
}

void MainFrame::OnHelpPluginMenu(wxCommandEvent& event)
{
    wxString pluginName = m_PluginIDsMap[event.GetId()];
    if (!pluginName.IsEmpty())
    {
        const PluginInfo* pi = Manager::Get()->GetPluginManager()->GetPluginInfo(pluginName);
        if (!pi)
        {
            Manager::Get()->GetLogManager()->DebugLog(_T("No plugin info for ") + pluginName);
            return;
        }
        dlgAboutPlugin dlg(this, pi);
        PlaceWindow(&dlg);
        dlg.ShowModal();
    }
    else
        Manager::Get()->GetLogManager()->DebugLog(F(_T("No plugin found for ID %d"), event.GetId()));
}

void MainFrame::OnFileNewWhat(wxCommandEvent& event)
{
    int id = event.GetId();
    if (id != idFileNewEmpty)
    {
        // wizard-based

        TemplateOutputType tot = totProject;
        if (id == idFileNewProject) tot = totProject;
        else if (id == idFileNewTarget) tot = totTarget;
        else if (id == idFileNewFile) tot = totFiles;
        else if (id == idFileNewCustom) tot = totCustom;
        else if (id == idFileNewUser) tot = totUser;
        else
            return;

        wxString filename;
        cbProject* prj = TemplateManager::Get()->New(tot, &filename);
        // verify that the open files are still in sync
        // the new file might have overwritten an existing one)
        Manager::Get()->GetEditorManager()->CheckForExternallyModifiedFiles();

        // If both are empty it means that the wizard has failed
        if (!prj && filename.IsEmpty())
            return;

        if (prj)
        {
            prj->Save();
            prj->SaveAllFiles();
        }

        if (!filename.IsEmpty())
        {
            if (prj)
                AddToRecentProjectsHistory(filename);
            else
                AddToRecentFilesHistory(filename);
        }
        if (prj && tot == totProject) // Created project should be parsed
        {
            CodeBlocksEvent evt(cbEVT_PROJECT_OPEN, 0, prj);
            Manager::Get()->GetPluginManager()->NotifyPlugins(evt);
        }
        return;
    }

    // new empty file quick shortcut code below

    cbProject* project = Manager::Get()->GetProjectManager()->GetActiveProject();
    if (project)
        wxSetWorkingDirectory(project->GetBasePath());
    cbEditor* ed = Manager::Get()->GetEditorManager()->New();
    if (ed)
    {
        // initially start change-collection if configured on empty files
        ed->GetControl()->SetChangeCollection(Manager::Get()->GetConfigManager(_T("editor"))->ReadBool(_T("/margin/use_changebar"), true));
    }

    if (ed && ed->IsOK())
    {
        AddToRecentFilesHistory(ed->GetFilename());
    }

    if (!ed || !project)
        return;

    wxString oldname = ed->GetFilename();
    if (cbMessageBox(_("Do you want to add this new file in the active project (has to be saved first)?"),
                    _("Add file to project"),
                    wxYES_NO | wxICON_QUESTION) == wxID_YES &&
        ed->SaveAs() && ed->IsOK())
    {
        wxArrayInt targets;
        if (Manager::Get()->GetProjectManager()->AddFileToProject(ed->GetFilename(), project, targets) != 0)
        {
            ProjectFile* pf = project->GetFileByFilename(ed->GetFilename(), false);
            ed->SetProjectFile(pf);
            Manager::Get()->GetProjectManager()->RebuildTree();
        }
    }
    // verify that the open files are still in sync
    // the new file might have overwritten an existing one)
    Manager::Get()->GetEditorManager()->CheckForExternallyModifiedFiles();
}

bool MainFrame::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& files)
{
    bool success = true; // Safe case initialisation

    // first check to see if a workspace is passed. If so, only this will be loaded
    wxString foundWorkspace;
    for (unsigned int i = 0; i < files.GetCount(); ++i)
    {
        FileType ft = FileTypeOf(files[i]);
        if (ft == ftCodeBlocksWorkspace || ft == ftMSVC6Workspace || ft == ftMSVC7Workspace)
        {
            foundWorkspace = files[i];
            break;
        }
    }

    if (!foundWorkspace.IsEmpty())
      success &= OpenGeneric(foundWorkspace);
    else
    {
        wxBusyCursor useless;
        wxPaintEvent e;
        ProcessEvent(e);

        Freeze();
        for (unsigned int i = 0; i < files.GetCount(); ++i)
          success &= OpenGeneric(files[i]);
        Thaw();
    }
    return success;
}

void MainFrame::OnFileNew(wxCommandEvent& event)
{
    wxMenu* popup = 0;
    wxMenuBar* bar = GetMenuBar();
    if (!bar)
        return;

    bar->FindItem(idFileNewProject, &popup);
    if (popup)
        PopupMenu(popup); // this will lead us in OnFileNewWhat() - the meat is there ;)
}

// in case we are opening a project (bProject == true) we do not want to interfere
// with 'last type of files' (probably the call was open (existing) project on the
// start here page --> so we know it's a project --> set the filter accordingly
// but as said don't force the 'last used type of files' to change, that should
// only change when an open file is carried out (so (source) file <---> project (file) )
// TODO : when regular file open and user manually sets filter to project files --> will change
//      the last type : is that expected behaviour ???
void MainFrame::DoOnFileOpen(bool bProject)
{
    wxString Filters = FileFilters::GetFilterString();
    // the value returned by GetIndexForFilterAll() is updated by GetFilterString()
    int StoredIndex = FileFilters::GetIndexForFilterAll();
    wxString Path;
    ConfigManager* mgr = Manager::Get()->GetConfigManager(_T("app"));
    if (mgr)
    {
        if (!bProject)
        {
            wxString Filter = mgr->Read(_T("/file_dialogs/file_new_open/filter"));
            if (!Filter.IsEmpty())
            {
                FileFilters::GetFilterIndexFromName(Filters, Filter, StoredIndex);
            }
            Path = mgr->Read(_T("/file_dialogs/file_new_open/directory"), Path);
        }
        else
        {
            FileFilters::GetFilterIndexFromName(Filters, _("Code::Blocks project files"), StoredIndex);
        }
    }
    wxFileDialog dlg(this,
                            _("Open file"),
                            Path,
                            wxEmptyString,
                            Filters,
                            wxFD_OPEN | wxFD_MULTIPLE | compatibility::wxHideReadonly);
    dlg.SetFilterIndex(StoredIndex);

    PlaceWindow(&dlg);
    if (dlg.ShowModal() == wxID_OK)
    {
        // store the last used filter and directory
        // as said : don't do this in case of an 'open project'
        if (mgr && !bProject)
        {
            int Index = dlg.GetFilterIndex();
            wxString Filter;
            if (FileFilters::GetFilterNameFromIndex(Filters, Index, Filter))
            {
                mgr->Write(_T("/file_dialogs/file_new_open/filter"), Filter);
            }
            wxString Test = dlg.GetDirectory();
            mgr->Write(_T("/file_dialogs/file_new_open/directory"), dlg.GetDirectory());
        }
        wxArrayString files;
        dlg.GetPaths(files);
        OnDropFiles(0,0,files);
    }
} // end of DoOnFileOpen

void MainFrame::OnFileOpen(wxCommandEvent& event)
{
    DoOnFileOpen(false); // through file menu (not sure if we are opening a project)
} // end of OnFileOpen

void MainFrame::OnFileReopenProject(wxCommandEvent& event)
{
    size_t id = event.GetId() - wxID_FILE10;
    wxString fname = m_pProjectsHistory->GetHistoryFile(id);
    if (!OpenGeneric(fname, true))
    {
        AskToRemoveFileFromHistory(m_pProjectsHistory, id);
    }
}

void MainFrame::OnFileOpenRecentProjectClearHistory(wxCommandEvent& event)
{
    while (m_pProjectsHistory->GetCount())
    {
        m_pProjectsHistory->RemoveFileFromHistory(0);
    }
    Manager::Get()->GetConfigManager(_T("app"))->DeleteSubPath(_T("/recent_projects"));

    // update start here page
    InitializeRecentFilesHistory();
    EditorBase* sh = Manager::Get()->GetEditorManager()->GetEditor(g_StartHereTitle);
    if (sh)
        ((StartHerePage*)sh)->Reload();
}

void MainFrame::OnFileReopen(wxCommandEvent& event)
{
    size_t id = event.GetId() - wxID_FILE1;
    wxString fname = m_pFilesHistory->GetHistoryFile(id);
    if (!OpenGeneric(fname, true))
    {
        AskToRemoveFileFromHistory(m_pFilesHistory, id);
    }
}

void MainFrame::OnFileOpenRecentClearHistory(wxCommandEvent& event)
{
    while (m_pFilesHistory->GetCount())
    {
        m_pFilesHistory->RemoveFileFromHistory(0);
    }
    Manager::Get()->GetConfigManager(_T("app"))->DeleteSubPath(_T("/recent_files"));

    // update start here page
    InitializeRecentFilesHistory();
    EditorBase* sh = Manager::Get()->GetEditorManager()->GetEditor(g_StartHereTitle);
    if (sh)
        ((StartHerePage*)sh)->Reload();
}

void MainFrame::OnFileSave(wxCommandEvent& event)
{
    if (!Manager::Get()->GetEditorManager()->SaveActive())
    {
        wxString msg;
        msg.Printf(_("File %s could not be saved..."), Manager::Get()->GetEditorManager()->GetActiveEditor()->GetFilename().c_str());
        cbMessageBox(msg, _("Error saving file"), wxICON_ERROR);
    }
    DoUpdateStatusBar();
}

void MainFrame::OnFileSaveAs(wxCommandEvent& event)
{
    Manager::Get()->GetEditorManager()->SaveActiveAs();
    DoUpdateStatusBar();
} // end of OnFileSaveAs

void MainFrame::OnFileSaveAllFiles(wxCommandEvent& event)
{
    Manager::Get()->GetEditorManager()->SaveAll();
    DoUpdateStatusBar();
}

void MainFrame::OnFileSaveProject(wxCommandEvent& event)
{
    // no need to call SaveActiveProjectAs(), because this is handled in cbProject::Save()
    if (Manager::Get()->GetProjectManager()->SaveActiveProject())
        AddToRecentProjectsHistory(Manager::Get()->GetProjectManager()->GetActiveProject()->GetFilename());
    DoUpdateStatusBar();
    DoUpdateAppTitle();
}

void MainFrame::OnFileSaveProjectAs(wxCommandEvent& event)
{
    if (Manager::Get()->GetProjectManager()->SaveActiveProjectAs())
        AddToRecentProjectsHistory(Manager::Get()->GetProjectManager()->GetActiveProject()->GetFilename());
    DoUpdateStatusBar();
    DoUpdateAppTitle();
}

void MainFrame::OnFileSaveProjectAllProjects(wxCommandEvent& event)
{
    Manager::Get()->GetProjectManager()->SaveAllProjects();
    DoUpdateStatusBar();
    DoUpdateAppTitle();
}

void MainFrame::OnFileSaveAll(wxCommandEvent& event)
{
    Manager::Get()->GetEditorManager()->SaveAll();
    Manager::Get()->GetProjectManager()->SaveAllProjects();

    if (Manager::Get()->GetProjectManager()->GetWorkspace()->GetModified()
        && !Manager::Get()->GetProjectManager()->GetWorkspace()->IsDefault()
        && Manager::Get()->GetProjectManager()->SaveWorkspace())
    {
        AddToRecentProjectsHistory(Manager::Get()->GetProjectManager()->GetWorkspace()->GetFilename());
    }
    DoUpdateStatusBar();
    DoUpdateAppTitle();
}

void MainFrame::OnFileSaveProjectTemplate(wxCommandEvent& event)
{
    TemplateManager::Get()->SaveUserTemplate(Manager::Get()->GetProjectManager()->GetActiveProject());
}

void MainFrame::OnFileCloseProject(wxCommandEvent& event)
{
    // we 're not actually shutting down here, but we want to check if the
    // active project is still opening files (still busy)
    if (!ProjectManager::CanShutdown() || !EditorManager::CanShutdown())
    {
        wxBell();
        return;
    }
    Manager::Get()->GetProjectManager()->CloseActiveProject();
    DoUpdateStatusBar();
}

void MainFrame::OnFileCloseAllProjects(wxCommandEvent& event)
{
    if (!ProjectManager::CanShutdown() || !EditorManager::CanShutdown())
    {
        wxBell();
        return;
    }
    DoCloseCurrentWorkspace();
    DoUpdateStatusBar();
}

void MainFrame::OnFileImportProjectDevCpp(wxCommandEvent& event)
{
    OpenGeneric(ShowOpenFileDialog(_("Import Dev-C++ project"), FileFilters::GetFilterString(_T('.') + FileFilters::DEVCPP_EXT)), false);
}

void MainFrame::OnFileImportProjectMSVC(wxCommandEvent& event)
{
    OpenGeneric(ShowOpenFileDialog(_("Import MS Visual C++ 6.0 project"), FileFilters::GetFilterString(_T('.') + FileFilters::MSVC6_EXT)), false);
}

void MainFrame::OnFileImportProjectMSVCWksp(wxCommandEvent& event)
{
    OpenGeneric(ShowOpenFileDialog(_("Import MS Visual C++ 6.0 workspace"), FileFilters::GetFilterString(_T('.') + FileFilters::MSVC6_WORKSPACE_EXT)), false);
}

void MainFrame::OnFileImportProjectMSVS(wxCommandEvent& event)
{
    OpenGeneric(ShowOpenFileDialog(_("Import MS Visual Studio 7.0+ project"), FileFilters::GetFilterString(_T('.') + FileFilters::MSVC7_EXT)), false);
}

void MainFrame::OnFileImportProjectMSVSWksp(wxCommandEvent& event)
{
    OpenGeneric(ShowOpenFileDialog(_("Import MS Visual Studio 7.0+ solution"), FileFilters::GetFilterString(_T('.') + FileFilters::MSVC7_WORKSPACE_EXT)), false);
}

void MainFrame::OnFileOpenDefWorkspace(wxCommandEvent& event)
{
    ProjectManager *pman = Manager::Get()->GetProjectManager();
    if (!pman->GetWorkspace()->IsDefault() && !pman->LoadWorkspace())
    {
        // do not add the default workspace in recent projects list
        // it's always one menu click away
        cbMessageBox(_("Can't open default workspace (file exists?)"), _("Warning"), wxICON_WARNING);
    }
}

void MainFrame::OnFileSaveWorkspace(wxCommandEvent& event)
{
    if (Manager::Get()->GetProjectManager()->SaveWorkspace())
        AddToRecentProjectsHistory(Manager::Get()->GetProjectManager()->GetWorkspace()->GetFilename());
}

void MainFrame::OnFileSaveWorkspaceAs(wxCommandEvent& event)
{
    if (Manager::Get()->GetProjectManager()->SaveWorkspaceAs(_T("")))
        AddToRecentProjectsHistory(Manager::Get()->GetProjectManager()->GetWorkspace()->GetFilename());
}

void MainFrame::OnFileCloseWorkspace(wxCommandEvent& event)
{
    DoCloseCurrentWorkspace();
}

void MainFrame::OnFileClose(wxCommandEvent& WXUNUSED(event))
{
    Manager::Get()->GetEditorManager()->CloseActive();
    DoUpdateStatusBar();
    Refresh();
}

void MainFrame::OnFileCloseAll(wxCommandEvent& WXUNUSED(event))
{
    Manager::Get()->GetEditorManager()->CloseAll();
    DoUpdateStatusBar();
}

void MainFrame::OnFileNext(wxCommandEvent& event)
{
    Manager::Get()->GetEditorManager()->ActivateNext();
    DoUpdateStatusBar();
}

void MainFrame::OnFilePrev(wxCommandEvent& event)
{
    Manager::Get()->GetEditorManager()->ActivatePrevious();
    DoUpdateStatusBar();
}

void MainFrame::OnFilePrint(wxCommandEvent& event)
{
    PrintDialog dlg(this);
    PlaceWindow(&dlg);
    if (dlg.ShowModal() == wxID_OK)
        Manager::Get()->GetEditorManager()->Print(dlg.GetPrintScope(), dlg.GetPrintColourMode(), dlg.GetPrintLineNumbers());
}

void MainFrame::OnFileQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void MainFrame::OnEraseBackground(wxEraseEvent& event)
{
    // for flicker-free display
    event.Skip();
}

void MainFrame::OnSize(wxSizeEvent& event)
{
    if (m_pProgressBar)
    {
        wxRect r;
        GetStatusBar()->GetFieldRect(1, r);
        m_pProgressBar->SetPosition(r.GetPosition());
        m_pProgressBar->SetSize(r.GetSize());
    }

    // for flicker-free display
    event.Skip();
}

void MainFrame::OnApplicationClose(wxCloseEvent& event)
{
    if (m_InitiatedShutdown)
        return;

    CodeBlocksEvent evt(cbEVT_APP_START_SHUTDOWN);
    Manager::Get()->ProcessEvent(evt);
    Manager::Yield();


    m_InitiatedShutdown = true;
    Manager::BlockYields(true);

    ProjectManager* prjman = Manager::Get()->GetProjectManager();
    if (prjman)
    {
        cbProject* prj = prjman->GetActiveProject();
        if (prj && prj->GetCurrentlyCompilingTarget())
        {
            event.Veto();
            wxBell();
            m_InitiatedShutdown = false;
            Manager::BlockYields(false);
            return;
        }
    }
    if (!ProjectManager::CanShutdown() || !EditorManager::CanShutdown())
    {
        event.Veto();
        wxBell();
        m_InitiatedShutdown = false;
        Manager::BlockYields(false);
        return;
    }

    if (!DoCloseCurrentWorkspace())
    {
        event.Veto();
        m_InitiatedShutdown = false;
        Manager::BlockYields(false);
        return;
    }

    Manager::Get()->GetLogManager()->DebugLog(_T("Deinitializing plugins..."));
    CodeBlocksEvent evtShutdown(cbEVT_APP_START_SHUTDOWN);
    Manager::Get()->ProcessEvent(evtShutdown);

    if (Manager::IsBatchBuild() == false)
        SaveWindowState();

    m_LayoutManager.DetachPane(Manager::Get()->GetProjectManager()->GetNotebook());
    m_LayoutManager.DetachPane(m_pInfoPane);
    m_LayoutManager.DetachPane(Manager::Get()->GetEditorManager()->GetNotebook());

    m_LayoutManager.UnInit();
    TerminateRecentFilesHistory();

    // remove all other event handlers from this window
    // this stops it from crashing, when no plugins are loaded
    while (GetEventHandler() != this)
        PopEventHandler(false);

    // Hide the window
    Hide();

    if (!Manager::IsBatchBuild())
    {
        m_pInfoPane->Destroy();
        m_pInfoPane = 0L;
    }

    Manager::Shutdown(); // Shutdown() is not Free(), Manager is automatically destroyed at exit
    Destroy();
}

void MainFrame::OnEditSwapHeaderSource(wxCommandEvent& event)
{
    Manager::Get()->GetEditorManager()->SwapActiveHeaderSource();
    DoUpdateStatusBar();
}

void MainFrame::OnEditGotoMatchingBrace(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
        ed->GotoMatchingBrace();
}

void MainFrame::OnEditBookmarksToggle(wxCommandEvent& event)
{
    EditorBase* ed = Manager::Get()->GetEditorManager()->GetActiveEditor();
    if (ed)
        ed->ToggleBookmark();
}

void MainFrame::OnEditBookmarksNext(wxCommandEvent& event)
{
    EditorBase* ed = Manager::Get()->GetEditorManager()->GetActiveEditor();
    if (ed)
        ed->GotoNextBookmark();
}

void MainFrame::OnEditBookmarksPrevious(wxCommandEvent& event)
{
    EditorBase* ed = Manager::Get()->GetEditorManager()->GetActiveEditor();
    if (ed)
        ed->GotoPreviousBookmark();
}

void MainFrame::OnEditUndo(wxCommandEvent& event)
{
    EditorBase* ed = Manager::Get()->GetEditorManager()->GetActiveEditor();
    if (ed)
        ed->Undo();
}

void MainFrame::OnEditRedo(wxCommandEvent& event)
{
    EditorBase* ed = Manager::Get()->GetEditorManager()->GetActiveEditor();
    if (ed)
        ed->Redo();
}

void MainFrame::OnEditClearHistory(wxCommandEvent& event)
{
    EditorBase* ed = Manager::Get()->GetEditorManager()->GetActiveEditor();
    if (ed)
        ed->ClearHistory();
}

void MainFrame::OnEditCopy(wxCommandEvent& event)
{
    EditorBase* ed = Manager::Get()->GetEditorManager()->GetActiveEditor();
    if (ed)
        ed->Copy();
}

void MainFrame::OnEditCut(wxCommandEvent& event)
{
    EditorBase* ed = Manager::Get()->GetEditorManager()->GetActiveEditor();
    if (ed)
        ed->Cut();
}

void MainFrame::OnEditPaste(wxCommandEvent& event)
{
    EditorBase* ed = Manager::Get()->GetEditorManager()->GetActiveEditor();
    if (ed)
        ed->Paste();
}

void MainFrame::OnEditParaUp(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
    {
        ed->GetControl()->ParaUp();
    }
}

void MainFrame::OnEditParaUpExtend(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
    {
        ed->GetControl()->ParaUpExtend();
    }
}

void MainFrame::OnEditParaDown(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
    {
        ed->GetControl()->ParaDown();
    }
}

void MainFrame::OnEditParaDownExtend(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
    {
        ed->GetControl()->ParaDownExtend();
    }
}

void MainFrame::OnEditWordPartLeft(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
    {
        ed->GetControl()->WordPartLeft();
    }
}

void MainFrame::OnEditWordPartLeftExtend(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
    {
        ed->GetControl()->WordPartLeftExtend();
    }
}

void MainFrame::OnEditWordPartRight(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
    {
        ed->GetControl()->WordPartRight();
    }
}

void MainFrame::OnEditWordPartRightExtend(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
    {
        ed->GetControl()->WordPartRightExtend();
    }
}

void MainFrame::OnEditZoomIn(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
        ed->GetControl()->ZoomIn();
}

void MainFrame::OnEditZoomOut(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
        ed->GetControl()->ZoomOut();
}

void MainFrame::OnEditZoomReset(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
        ed->GetControl()->SetZoom(0);
}

void MainFrame::OnEditLineCut(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
        ed->GetControl()->LineCut();
}

void MainFrame::OnEditLineDelete(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
        ed->GetControl()->LineDelete();
}

void MainFrame::OnEditLineDuplicate(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
        ed->GetControl()->LineDuplicate();
}

void MainFrame::OnEditLineTranspose(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
        ed->GetControl()->LineTranspose();
}

void MainFrame::OnEditLineCopy(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
        ed->GetControl()->LineCopy();
}

void MainFrame::OnEditLinePaste(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
    {
        //We want to undo all in one step
        ed->GetControl()->BeginUndoAction();

        int pos = ed->GetControl()->GetCurrentPos();
        int line = ed->GetControl()->LineFromPosition(pos);
        ed->GetControl()->GotoLine(line);
        int column = pos - ed->GetControl()->GetCurrentPos();
        ed->GetControl()->Paste();
        pos = ed->GetControl()->GetCurrentPos();
        ed->GetControl()->GotoPos(pos+column);

        ed->GetControl()->EndUndoAction();
    }
}

void MainFrame::OnEditUpperCase(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
        ed->GetControl()->UpperCase();
}

void MainFrame::OnEditLowerCase(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
        ed->GetControl()->LowerCase();
}

void MainFrame::OnEditSelectAll(wxCommandEvent& event)
{
    EditorBase* eb = Manager::Get()->GetEditorManager()->GetActiveEditor();
    if (eb)
        eb->SelectAll();
}

/* This is a shameless rip-off of the original OnEditCommentSelected function,
 * now more suitingly named OnEditToggleCommentSelected (because that's what
 * it does :)
 */
void MainFrame::OnEditCommentSelected(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
    {
        cbStyledTextCtrl* stc = ed->GetControl();
        CommentToken comment =
            Manager::Get()->GetEditorManager()->GetColourSet()->GetCommentToken( ed->GetLanguage() );
        if (comment.lineComment==wxEmptyString && comment.streamCommentStart==wxEmptyString)
            return;

        stc->BeginUndoAction();
        if ( wxSCI_INVALID_POSITION != stc->GetSelectionStart() )
        {
            int startLine = stc->LineFromPosition( stc->GetSelectionStart() );
            int endLine   = stc->LineFromPosition( stc->GetSelectionEnd() );
            int curLine=startLine;
            /**
                Fix a glitch: when selecting multiple lines and the caret
                is at the start of the line after the last line selected,
                the code would, wrongly, (un)comment that line too.
                This fixes it.
            */
            if (startLine != endLine && // selection is more than one line
                stc->GetColumn( stc->GetSelectionEnd() ) == 0) // and the caret is at the start of the line
            {
                // don't take into account the line the caret is on,
                // because it contains no selection (caret_column == 0)...
                --endLine;
            }

            while( curLine <= endLine )
            {
                // For each line: comment.
                if (comment.lineComment!=wxEmptyString)
                    stc->InsertText( stc->PositionFromLine( curLine ), comment.lineComment );
                else // if the language doesn't support line comments use stream comments
                {
                    stc->InsertText( stc->PositionFromLine( curLine ), comment.streamCommentStart );
                    stc->InsertText( stc->GetLineEndPosition( curLine ), comment.streamCommentEnd );
                }
                ++curLine;
            } // end while
            stc->SetSelectionVoid(stc->PositionFromLine(startLine),stc->PositionFromLine(endLine)+stc->LineLength(endLine));
        }
        stc->EndUndoAction();
    }
} // end of OnEditCommentSelected

/* See above (OnEditCommentSelected) for details. */
void MainFrame::OnEditUncommentSelected(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
    {
        cbStyledTextCtrl* stc = ed->GetControl();
        CommentToken comment =
            Manager::Get()->GetEditorManager()->GetColourSet()->GetCommentToken( ed->GetLanguage() );

        if (comment.lineComment==wxEmptyString && comment.streamCommentStart==wxEmptyString)
            return;

        stc->BeginUndoAction();
        if ( wxSCI_INVALID_POSITION != stc->GetSelectionStart() )
        {
            int startLine = stc->LineFromPosition( stc->GetSelectionStart() );
            int endLine   = stc->LineFromPosition( stc->GetSelectionEnd() );
            int curLine   = startLine;
            /**
                Fix a glitch: when selecting multiple lines and the caret
                is at the start of the line after the last line selected,
                the code would, wrongly, (un)comment that line too.
                This fixes it.
            */
            if (startLine != endLine && // selection is more than one line
            stc->GetColumn( stc->GetSelectionEnd() ) == 0) // and the caret is at the start of the line
            {
                // don't take into account the line the caret is on,
                // because it contains no selection (caret_column == 0)...
                --endLine;
            }

            while( curLine <= endLine )
            {
                // For each line: if it is commented, uncomment.
                wxString strLine = stc->GetLine( curLine );

                bool startsWithComment;
                bool endsWithComment;

                // check for line comment
                startsWithComment = strLine.Strip( wxString::leading ).StartsWith( comment.lineComment );
                if ( startsWithComment )
                {      // we know the comment is there (maybe preceded by white space)
                    int Pos = strLine.Find(comment.lineComment);
                    int start = stc->PositionFromLine( curLine ) + Pos;
                    int end = start + comment.lineComment.Length();
                    stc->SetTargetStart( start );
                    stc->SetTargetEnd( end );
                    stc->ReplaceTarget( wxEmptyString );
                }

                // check for stream comment
                startsWithComment = strLine.Strip( wxString::leading  ).StartsWith( comment.streamCommentStart ); // check for stream comment start
                endsWithComment = strLine.Strip( wxString::trailing ).EndsWith( comment.streamCommentEnd); // check for stream comment end
                if ( startsWithComment && endsWithComment )
                {
                    int Pos;
                    int start;
                    int end;

                    // we know the start comment is there (maybe preceded by white space)
                    Pos = strLine.Find(comment.streamCommentStart);
                    start = stc->PositionFromLine( curLine ) + Pos;
                    end = start + comment.streamCommentStart.Length();
                    stc->SetTargetStart( start );
                    stc->SetTargetEnd( end );
                    stc->ReplaceTarget( wxEmptyString );

                    // we know the end comment is there too (maybe followed by white space)
                    // attention!! we have to subtract the length of the comment we already removed
                    Pos = strLine.rfind(comment.streamCommentEnd,strLine.npos) - comment.streamCommentStart.Length();
                    start = stc->PositionFromLine( curLine ) + Pos;
                    end = start + comment.streamCommentEnd.Length();
                    stc->SetTargetStart( start );
                    stc->SetTargetEnd( end );
                    stc->ReplaceTarget( wxEmptyString );
                }
                ++curLine;
            } // end while
            stc->SetSelectionVoid(stc->PositionFromLine(startLine),stc->PositionFromLine(endLine)+stc->LineLength(endLine));
        }
        stc->EndUndoAction();
    }
} // end of OnEditUncommentSelected

void MainFrame::OnEditToggleCommentSelected(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
    {
        cbStyledTextCtrl* stc = ed->GetControl();
        wxString comment =
            Manager::Get()->GetEditorManager()->GetColourSet()->GetCommentToken( ed->GetLanguage() ).lineComment;
        if (comment==wxEmptyString)
            return;

        stc->BeginUndoAction();
        if ( wxSCI_INVALID_POSITION != stc->GetSelectionStart() )
        {
            int startLine = stc->LineFromPosition( stc->GetSelectionStart() );
            int endLine   = stc->LineFromPosition( stc->GetSelectionEnd() );
            int curLine   = startLine;
            /**
                Fix a glitch: when selecting multiple lines and the caret
                is at the start of the line after the last line selected,
                the code would, wrongly, (un)comment that line too.
                This fixes it.
            */
            if (startLine != endLine && // selection is more than one line
            stc->GetColumn( stc->GetSelectionEnd() ) == 0) // and the caret is at the start of the line
            {
                // don't take into account the line the caret is on,
                // because it contains no selection (caret_column == 0)...
                --endLine;
            }

            while( curLine <= endLine )
            {
                // For each line: If it's commented, uncomment. Otherwise, comment.
                wxString strLine = stc->GetLine( curLine );
                int commentPos = strLine.Strip( wxString::leading ).Find( comment );

                if ( -1 == commentPos || commentPos > 0 )
                {
                    stc->InsertText( stc->PositionFromLine( curLine ), comment );
                }
                else
                {      // we know the comment is there (maybe preceded by white space)
                    int Pos = strLine.Find(comment);
                    int start = stc->PositionFromLine( curLine ) + Pos;
                    int end = start + comment.Length();
                    stc->SetTargetStart( start );
                    stc->SetTargetEnd( end );
                    stc->ReplaceTarget( wxEmptyString );
                }
                ++curLine;
            }
            stc->SetSelectionVoid(stc->PositionFromLine(startLine),stc->PositionFromLine(endLine)+stc->LineLength(endLine));
        }
        stc->EndUndoAction();
    }
} // end of OnEditToggleCommentSelected

void MainFrame::OnEditStreamCommentSelected(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
    {
        cbStyledTextCtrl* stc = ed->GetControl();
        CommentToken comment =
            Manager::Get()->GetEditorManager()->GetColourSet()->GetCommentToken( ed->GetLanguage() );
        if (comment.streamCommentStart==wxEmptyString)
            return;

        stc->BeginUndoAction();
        if ( wxSCI_INVALID_POSITION != stc->GetSelectionStart() )
        {
            int startPos = stc->GetSelectionStart();
            int endPos   = stc->GetSelectionEnd();
            if ( startPos == endPos ) { // if nothing selected stream comment current line
                startPos = stc->PositionFromLine  (stc->LineFromPosition(startPos));
                endPos   = stc->GetLineEndPosition(stc->LineFromPosition(startPos));
            }
            else {
                /**
                    Fix a glitch: when selecting multiple lines and the caret
                    is at the start of the line after the last line selected,
                    the code would, wrongly, (un)comment that line too.
                    This fixes it.
                */
                if (stc->GetColumn( stc->GetSelectionEnd() ) == 0) // and the caret is at the start of the line
                {
                    // don't take into account the line the caret is on,
                    // because it contains no selection (caret_column == 0)...
                    --endPos;
                }
            }
            // stream comment block
            stc->InsertText( startPos, comment.streamCommentStart );
             // we already inserted some characters so out endPos changed
            endPos += comment.streamCommentStart.Length();
            stc->InsertText( endPos, comment.streamCommentEnd );
            stc->SetSelectionVoid(startPos,endPos);
        }
        stc->EndUndoAction();
    }
} // end of OnStreamToggleCommentSelected

void MainFrame::OnEditBoxCommentSelected(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
    {

        cbStyledTextCtrl* stc = ed->GetControl();
        CommentToken comment =
            Manager::Get()->GetEditorManager()->GetColourSet()->GetCommentToken( ed->GetLanguage() );
        if (comment.boxCommentStart==wxEmptyString)
            return;

        wxString nlc;
        switch (stc->GetEOLMode())
        {
            case wxSCI_EOL_CRLF: nlc=_T("\r\n"); break;
            case wxSCI_EOL_CR:   nlc=_T("\r");   break;
            case wxSCI_EOL_LF:   nlc=_T("\n");   break;

        }

        stc->BeginUndoAction();
        if ( wxSCI_INVALID_POSITION != stc->GetSelectionStart() )
        {
            int startLine = stc->LineFromPosition( stc->GetSelectionStart() );
            int endLine   = stc->LineFromPosition( stc->GetSelectionEnd() );
            int curLine   = startLine;
            /**
                Fix a glitch: when selecting multiple lines and the caret
                is at the start of the line after the last line selected,
                the code would, wrongly, (un)comment that line too.
                This fixes it.
            */
            if (startLine != endLine && // selection is more than one line
            stc->GetColumn( stc->GetSelectionEnd() ) == 0) // and the caret is at the start of the line
            {
                // don't take into account the line the caret is on,
                // because it contains no selection (caret_column == 0)...
                --endLine;
            }



            if (startLine == endLine) // if selection is only one line ...
            {
                // ... then insert streamcomment tokens at the beginning and the end of the line
                stc->InsertText( stc->PositionFromLine  ( curLine ), comment.streamCommentStart );
                stc->InsertText( stc->GetLineEndPosition( curLine ), comment.streamCommentEnd   );
            }
            else // selection is more than one line
            {
                // insert boxcomment start token
                stc->InsertText( stc->PositionFromLine( curLine ), comment.boxCommentStart );
                ++curLine; // we already commented the first line about 9 lines above
                while( curLine <= endLine )
                {
                    // For each line: comment.
                    stc->InsertText( stc->PositionFromLine( curLine ), comment.boxCommentMid );
                    ++curLine;
                } // end while

                // insert boxcomment end token and add a new line character
                stc->InsertText( stc->PositionFromLine( curLine ), comment.boxCommentEnd + nlc);
            } // end if
            stc->SetSelectionVoid(stc->PositionFromLine(startLine),stc->PositionFromLine(endLine)+stc->LineLength(endLine));
        }
        stc->EndUndoAction();
    }
} // end of OnEditBoxCommentSelected

void MainFrame::OnEditAutoComplete(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
        ed->AutoComplete();
}

void MainFrame::OnEditHighlightMode(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
    {
        EditorColourSet* theme = Manager::Get()->GetEditorManager()->GetColourSet();
        if (theme)
        {
            HighlightLanguage lang = theme->GetHighlightLanguage(_T(""));
            if (event.GetId() != idEditHighlightModeText)
            {
                wxMenu* hl = 0;
                GetMenuBar()->FindItem(idEditHighlightModeText, &hl);
                if (hl)
                {
                    wxMenuItem* item = hl->FindItem(event.GetId());
                    if (item)
                    #if wxCHECK_VERSION(2, 9, 0)
                        lang = theme->GetHighlightLanguage(item->GetItemLabelText());
                    #else
                        lang = theme->GetHighlightLanguage(item->GetLabel());
                    #endif
                }
            }
            ed->SetLanguage(lang);
        }
    }
}

void MainFrame::OnEditFoldAll(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
        ed->FoldAll();
}

void MainFrame::OnEditUnfoldAll(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
        ed->UnfoldAll();
}

void MainFrame::OnEditToggleAllFolds(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
        ed->ToggleAllFolds();
}

void MainFrame::OnEditFoldBlock(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
        ed->FoldBlockFromLine();
}

void MainFrame::OnEditUnfoldBlock(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
        ed->UnfoldBlockFromLine();
}

void MainFrame::OnEditToggleFoldBlock(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
        ed->ToggleFoldBlockFromLine();
}

void MainFrame::OnEditEOLMode(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
    {
        int mode = -1;

        if (event.GetId() == idEditEOLCRLF)
            mode = wxSCI_EOL_CRLF;
        else if (event.GetId() == idEditEOLCR)
            mode = wxSCI_EOL_CR;
        else if (event.GetId() == idEditEOLLF)
            mode = wxSCI_EOL_LF;

        if (mode != -1 && mode != ed->GetControl()->GetEOLMode())
        {
            ed->GetControl()->BeginUndoAction();
            ed->GetControl()->ConvertEOLs(mode);
            ed->GetControl()->SetEOLMode(mode);
            ed->GetControl()->EndUndoAction();
        }
    }
}

void MainFrame::OnEditEncoding(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (!ed)
        return;

    if ( event.GetId() == idEditEncodingUseBom )
    {
        ed->SetUseBom( !ed->GetUseBom() );
        return;
    }

    wxFontEncoding encoding = wxFONTENCODING_SYSTEM;

    if ( event.GetId() == idEditEncodingDefault )
        encoding = wxFONTENCODING_SYSTEM;
    else if ( event.GetId() == idEditEncodingAscii )
        encoding = wxFONTENCODING_ISO8859_1;
    else if ( event.GetId() == idEditEncodingUtf7 )
        encoding = wxFONTENCODING_UTF7;
    else if ( event.GetId() == idEditEncodingUtf8 )
        encoding = wxFONTENCODING_UTF8;
    else if ( event.GetId() == idEditEncodingUtf16 )
        encoding = wxFONTENCODING_UTF16;
    else if ( event.GetId() == idEditEncodingUtf32 )
        encoding = wxFONTENCODING_UTF32;
    else if ( event.GetId() == idEditEncodingUnicode )
        encoding = wxFONTENCODING_UNICODE;
    else if ( event.GetId() == idEditEncodingUnicode16BE )
        encoding = wxFONTENCODING_UTF16BE;
    else if ( event.GetId() == idEditEncodingUnicode16LE )
        encoding = wxFONTENCODING_UTF16LE;
    else if ( event.GetId() == idEditEncodingUnicode32BE )
        encoding = wxFONTENCODING_UTF32BE;
    else if ( event.GetId() == idEditEncodingUnicode32LE )
        encoding = wxFONTENCODING_UTF32LE;

    ed->SetEncoding(encoding);
}

void MainFrame::OnViewLayout(wxCommandEvent& event)
{
    LoadViewLayout(m_PluginIDsMap[event.GetId()]);
}

void MainFrame::OnViewLayoutSave(wxCommandEvent& event)
{
    wxString def = Manager::Get()->GetConfigManager(_T("app"))->Read(_T("/main_frame/layout/default"));
    wxString name = wxGetTextFromUser(_("Enter the name for this perspective"), _("Save current perspective"), def);
    if (!name.IsEmpty())
    {
        DoFixToolbarsLayout();
        SaveViewLayout(name, m_LayoutManager.SavePerspective(), true);
    }
}

void MainFrame::OnViewLayoutDelete(wxCommandEvent& event)
{
    if (m_LastLayoutName == gDefaultLayout)
    {
        if (cbMessageBox(_("The default perspective cannot be deleted. It can always be reverted to "
                        "a predefined state though.\nDo you want to revert it now?"),
                        _("Confirmation"),
                        wxICON_QUESTION | wxYES_NO | wxNO_DEFAULT) == wxID_YES)
        {
            m_LayoutViews[gDefaultLayout] = gDefaultLayoutData;
            LoadViewLayout(gDefaultLayout);
        }
        return;
    }

    if (cbMessageBox(wxString::Format(_("Are you really sure you want to delete the perspective '%s'?"), m_LastLayoutName.c_str()),
                    _("Confirmation"),
                    wxICON_QUESTION | wxYES_NO | wxNO_DEFAULT) == wxID_YES)
    {
        // first delete it from the hashmap
        LayoutViewsMap::iterator it = m_LayoutViews.find(m_LastLayoutName);
        if (it != m_LayoutViews.end())
            m_LayoutViews.erase(it);

        // now delete the menu item too
        wxMenu* viewLayouts = 0;
        GetMenuBar()->FindItem(idViewLayoutSave, &viewLayouts);
        if (viewLayouts)
        {
            int id = viewLayouts->FindItem(m_LastLayoutName);
            if (id != wxNOT_FOUND)
                viewLayouts->Delete(id);
            // delete the id from the map too
            PluginIDsMap::iterator it2 = m_PluginIDsMap.find(id);
            if (it2 != m_PluginIDsMap.end())
                m_PluginIDsMap.erase(it2);
        }

        cbMessageBox(wxString::Format(_("Perspective '%s' deleted.\nWill now revert to perspective '%s'..."), m_LastLayoutName.c_str(), gDefaultLayout.c_str()),
                        _("Information"), wxICON_INFORMATION);

        // finally, revert to the default layout
        m_LastLayoutName = gDefaultLayout; // do not ask to save old layout ;)
        LoadViewLayout(gDefaultLayout);
    }
}

void MainFrame::OnViewScriptConsole(wxCommandEvent& event)
{
    ShowHideScriptConsole();
}

void MainFrame::OnSearchFind(wxCommandEvent& event)
{
    bool bDoMultipleFiles = (event.GetId() == idSearchFindInFiles);
    if (!bDoMultipleFiles)
    {
        bDoMultipleFiles = !Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    }
    Manager::Get()->GetEditorManager()->ShowFindDialog(false, bDoMultipleFiles);
}// end of OnSearchFind

void MainFrame::OnSearchFindNext(wxCommandEvent& event)
{
    bool bNext = !(event.GetId() == idSearchFindPrevious);
    Manager::Get()->GetEditorManager()->FindNext(bNext);
} // end of OnSearchFindNext

void MainFrame::OnSearchReplace(wxCommandEvent& event)
{
    bool bDoMultipleFiles = (event.GetId() == idSearchReplaceInFiles);
    if (!bDoMultipleFiles)
    {
        bDoMultipleFiles = !Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    }
    Manager::Get()->GetEditorManager()->ShowFindDialog(true, bDoMultipleFiles);
} // end of OnSearchReplace

void MainFrame::OnSearchGotoLine(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (!ed)
        return;

    int max = ed->GetControl()->LineFromPosition(ed->GetControl()->GetLength()) + 1;

    /**
    @remarks We use wxGetText instead of wxGetNumber because wxGetNumber *must*
    provide an initial line number...which doesn't make sense, and just keeps the
    user deleting the initial line number everytime he instantiates the dialog.
    However, this is just a temporary hack, because the default dialog used isn't
    that suitable either.
    */
    wxString strLine = wxGetTextFromUser( wxString::Format(_("Line (1 - %d): "), max),
                                        _("Goto line"),
                                        _T( "" ),
                                        this );
    long int line = 0;
    strLine.ToLong(&line);
    if ( line >= 1 && line <= max )
    {
        ed->UnfoldBlockFromLine(line - 1);
        ed->GotoLine(line - 1);
    }
}

void MainFrame::OnSearchGotoNextChanged(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
        ed->GotoNextChanged();
}

void MainFrame::OnSearchGotoPrevChanged(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
        ed->GotoPreviousChanged();
}

void MainFrame::OnHelpAbout(wxCommandEvent& WXUNUSED(event))
{
    dlgAbout dlg(this);
    PlaceWindow(&dlg, pdlHead);
    dlg.ShowModal();
}

void MainFrame::OnHelpTips(wxCommandEvent& event)
{
    ShowTips(true);
}

void MainFrame::OnFileMenuUpdateUI(wxUpdateUIEvent& event)
{
    if (Manager::isappShuttingDown())
    {
        event.Skip();
        return;
    }
    EditorBase* ed = Manager::Get()->GetEditorManager() ? Manager::Get()->GetEditorManager()->GetActiveEditor() : 0;
    cbProject* prj = Manager::Get()->GetProjectManager() ? Manager::Get()->GetProjectManager()->GetActiveProject() : 0L;
    EditorBase* sh = Manager::Get()->GetEditorManager()->GetEditor(g_StartHereTitle);
    cbWorkspace* wksp = Manager::Get()->GetProjectManager()->GetWorkspace();
    wxMenuBar* mbar = GetMenuBar();

    bool canCloseProject = (ProjectManager::CanShutdown() && EditorManager::CanShutdown())
                            && prj && !prj->GetCurrentlyCompilingTarget();
    bool canClose = ed && !(sh && Manager::Get()->GetEditorManager()->GetEditorsCount() == 1);
    bool canSaveFiles = ed && !(sh && Manager::Get()->GetEditorManager()->GetEditorsCount() == 1);

    bool canSaveAll = (prj && prj->GetModified()) || canSaveFiles || (wksp && !wksp->IsDefault() && wksp->GetModified());

    mbar->Enable(idFileCloseProject,canCloseProject);
    mbar->Enable(idFileOpenRecentFileClearHistory, m_pFilesHistory->GetCount());
    mbar->Enable(idFileOpenRecentProjectClearHistory, m_pProjectsHistory->GetCount());
    mbar->Enable(idFileClose, canClose);
    mbar->Enable(idFileCloseAll, canClose);
    mbar->Enable(idFileSave, ed && ed->GetModified());
    mbar->Enable(idFileSaveAs, canSaveFiles);
    mbar->Enable(idFileSaveAllFiles, canSaveFiles);
    mbar->Enable(idFileSaveProject, prj && prj->GetModified() && canCloseProject);
    mbar->Enable(idFileSaveProjectAs, prj && canCloseProject);
    mbar->Enable(idFileOpenDefWorkspace, canCloseProject);
    mbar->Enable(idFileSaveWorkspace, Manager::Get()->GetProjectManager() && canCloseProject);
    mbar->Enable(idFileSaveWorkspaceAs, Manager::Get()->GetProjectManager() && canCloseProject);
    mbar->Enable(idFileCloseWorkspace, Manager::Get()->GetProjectManager() && canCloseProject);
    mbar->Enable(idFileSaveAll, canSaveAll);
    mbar->Enable(idFilePrint, Manager::Get()->GetEditorManager() && Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor());

    if (m_pToolbar)
    {
        m_pToolbar->EnableTool(idFileSave, ed && ed->GetModified());
        m_pToolbar->EnableTool(idFileSaveAllFiles, canSaveFiles);
        m_pToolbar->EnableTool(idFileSaveAll, canSaveAll);
        m_pToolbar->EnableTool(idFilePrint, Manager::Get()->GetEditorManager() && Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor());
    }

    event.Skip();
}

void MainFrame::OnEditMenuUpdateUI(wxUpdateUIEvent& event)
{
    if (Manager::isappShuttingDown())
    {
        event.Skip();
        return;
    }

    cbEditor* ed = NULL;
    EditorBase* eb = NULL;
    bool hasSel = false;
    bool canUndo = false;
    bool canRedo = false;
    bool canPaste = false;
    bool canCut = false;
    bool canSelAll = false;
    int eolMode = -1;

    if (Manager::Get()->GetEditorManager() && !Manager::isappShuttingDown())
    {
        ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
        eb = Manager::Get()->GetEditorManager()->GetActiveEditor();
    }

    wxMenuBar* mbar = GetMenuBar();

    if (ed)
        eolMode = ed->GetControl()->GetEOLMode();
    if (eb)
    {
        canUndo = eb->CanUndo();
        canRedo = eb->CanRedo();
        hasSel = eb->HasSelection();
        canPaste = eb->CanPaste();
        canCut = !eb->IsReadOnly() && hasSel;
        canSelAll = eb->CanSelectAll();
    }

    mbar->Enable(idEditUndo, canUndo);
    mbar->Enable(idEditRedo, canRedo);
    mbar->Enable(idEditClearHistory, canUndo || canRedo);
    mbar->Enable(idEditCut, canCut);
    mbar->Enable(idEditCopy, hasSel);
    mbar->Enable(idEditPaste, canPaste);
    mbar->Enable(idEditSwapHeaderSource, ed);
    mbar->Enable(idEditGotoMatchingBrace, ed);
    mbar->Enable(idEditHighlightMode, ed);
    mbar->Enable(idEditSelectAll, canSelAll);
    mbar->Enable(idEditBookmarks, ed);
    mbar->Enable(idEditFolding, ed);
    mbar->Enable(idEditEOLMode, ed);
    mbar->Enable(idEditEncoding, ed);
    mbar->Enable(idEditSpecialCommands, ed);
    mbar->Enable(idEditSpecialCommandsCase, ed && hasSel);
    mbar->Enable(idEditCommentSelected, ed);
    mbar->Enable(idEditAutoComplete, ed);
    mbar->Enable(idEditUncommentSelected, ed);
    mbar->Enable(idEditToggleCommentSelected, ed);
    mbar->Enable(idEditStreamCommentSelected, ed);
    mbar->Enable(idEditBoxCommentSelected, ed);


    if (ed)
    {
        // OK... this was the strangest/silliest/most-frustrating bug ever in the computer programs history...
        // Under wxGTK it seems that if you try to Check() a menu item if its container Menu is disabled,
        // you enter an endless message loop eating 100% CPU...
        // DARN!
        // This fixes the dreaded 'linux-hang-on-close-project' bug.
        mbar->Check(idEditEOLCRLF, eolMode == wxSCI_EOL_CRLF);
        mbar->Check(idEditEOLCR, eolMode == wxSCI_EOL_CR);
        mbar->Check(idEditEOLLF, eolMode == wxSCI_EOL_LF);

        bool defenc = ed && (ed->GetEncoding() == wxFONTENCODING_SYSTEM || ed->GetEncoding() == wxLocale::GetSystemEncoding());

        mbar->Check(idEditEncodingDefault, defenc);
        mbar->Check(idEditEncodingUseBom, ed && ed->GetUseBom());
        mbar->Check(idEditEncodingAscii, ed && ed->GetEncoding() == wxFONTENCODING_ISO8859_1);
        mbar->Check(idEditEncodingUtf7, ed && ed->GetEncoding() == wxFONTENCODING_UTF7);
        mbar->Check(idEditEncodingUtf8, ed && ed->GetEncoding() == wxFONTENCODING_UTF8);
        mbar->Check(idEditEncodingUnicode, ed && ed->GetEncoding() == wxFONTENCODING_UNICODE);
        mbar->Check(idEditEncodingUtf16, ed && ed->GetEncoding() == wxFONTENCODING_UTF16);
        mbar->Check(idEditEncodingUtf32, ed && ed->GetEncoding() == wxFONTENCODING_UTF32);
        mbar->Check(idEditEncodingUnicode16BE, ed && ed->GetEncoding() == wxFONTENCODING_UTF16BE);
        mbar->Check(idEditEncodingUnicode16LE, ed && ed->GetEncoding() == wxFONTENCODING_UTF16LE);
        mbar->Check(idEditEncodingUnicode32BE, ed && ed->GetEncoding() == wxFONTENCODING_UTF32BE);
        mbar->Check(idEditEncodingUnicode32LE, ed && ed->GetEncoding() == wxFONTENCODING_UTF32LE);

        wxMenu* hl = 0;
        mbar->FindItem(idEditHighlightModeText, &hl);
        if (hl)
            mbar->Check(hl->FindItem(ed->GetColourSet()->GetLanguageName(ed->GetLanguage())), true);
    }

    if (m_pToolbar)
    {
        m_pToolbar->EnableTool(idEditUndo, canUndo);
        m_pToolbar->EnableTool(idEditRedo, canRedo);
        m_pToolbar->EnableTool(idEditCut, canCut);
        m_pToolbar->EnableTool(idEditCopy, hasSel);
        m_pToolbar->EnableTool(idEditPaste, canPaste);
    }

    event.Skip();
}

void MainFrame::OnViewMenuUpdateUI(wxUpdateUIEvent& event)
{
    if (Manager::isappShuttingDown())
    {
        event.Skip();
        return;
    }
    wxMenuBar* mbar = GetMenuBar();
    cbEditor* ed = Manager::Get()->GetEditorManager() ? Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor() : 0;
    bool manVis = m_LayoutManager.GetPane(Manager::Get()->GetProjectManager()->GetNotebook()).IsShown();

    mbar->Check(idViewManager, manVis);
    mbar->Check(idViewLogManager, m_LayoutManager.GetPane(m_pInfoPane).IsShown());
    mbar->Check(idViewStatusbar, GetStatusBar() && GetStatusBar()->IsShown());
    mbar->Check(idViewScriptConsole, m_LayoutManager.GetPane(m_pScriptConsole).IsShown());
    mbar->Check(idViewFullScreen, IsFullScreen());
    mbar->Enable(idViewFocusEditor, ed);

    // toolbars
    mbar->Check(idViewToolMain, m_LayoutManager.GetPane(m_pToolbar).IsShown());
    wxMenu* viewToolbars = 0;
    GetMenuBar()->FindItem(idViewToolMain, &viewToolbars);
    if (viewToolbars)
    {
        for (size_t i = 0; i < viewToolbars->GetMenuItemCount(); ++i)
        {
            wxMenuItem* item = viewToolbars->GetMenuItems()[i];
            wxString pluginName = m_PluginIDsMap[item->GetId()];
            if (!pluginName.IsEmpty())
            {
                cbPlugin* plugin = Manager::Get()->GetPluginManager()->FindPluginByName(pluginName);
                if (plugin)
                    item->Check(m_LayoutManager.GetPane(m_PluginsTools[plugin]).IsShown());
            }
        }
    }

    event.Skip();
}

void MainFrame::OnSearchMenuUpdateUI(wxUpdateUIEvent& event)
{
    if (Manager::isappShuttingDown())
    {
        event.Skip();
        return;
    }
    cbEditor* ed = Manager::Get()->GetEditorManager() ? Manager::Get()->GetEditorManager()->GetBuiltinEditor(Manager::Get()->GetEditorManager()->GetActiveEditor()) : 0;

    bool enableGotoChanged = false;

    if (ed)
        enableGotoChanged = Manager::Get()->GetConfigManager(_T("editor"))->ReadBool(_T("/margin/use_changebar"), true) && (ed->CanUndo() || ed->CanRedo());

    wxMenuBar* mbar = GetMenuBar();

    // 'Find' and 'Replace' are always enabled for (find|replace)-in-files
    mbar->Enable(idSearchFind, ed);
    mbar->Enable(idSearchFindNext, ed);
    mbar->Enable(idSearchFindPrevious, ed);
    mbar->Enable(idSearchReplace, ed);
    mbar->Enable(idSearchGotoLine, ed);
    mbar->Enable(idSearchGotoNextChanged, enableGotoChanged);
    mbar->Enable(idSearchGotoPreviousChanged, enableGotoChanged);


//    if (m_pToolbar)
//    {
//        m_pToolbar->EnableTool(idSearchFind, ed);
//        m_pToolbar->EnableTool(idSearchReplace, ed);
//    }

    event.Skip();
}

void MainFrame::OnProjectMenuUpdateUI(wxUpdateUIEvent& event)
{
    if (Manager::isappShuttingDown())
    {
        event.Skip();
        return;
    }
    cbProject* prj = Manager::Get()->GetProjectManager() ? Manager::Get()->GetProjectManager()->GetActiveProject() : 0L;
    wxMenuBar* mbar = GetMenuBar();

    bool canCloseProject = (ProjectManager::CanShutdown() && EditorManager::CanShutdown());
    mbar->Enable(idFileCloseProject, prj && canCloseProject);
    mbar->Enable(idFileCloseAllProjects, prj && canCloseProject);
    mbar->Enable(idFileSaveProject, prj && prj->GetModified() && canCloseProject);
    mbar->Enable(idFileSaveProjectAs, prj && canCloseProject);
    mbar->Enable(idFileSaveProjectAllProjects, prj && canCloseProject);
    mbar->Enable(idFileSaveProjectTemplate, prj && canCloseProject);

    event.Skip();
}

void MainFrame::OnEditorUpdateUI(CodeBlocksEvent& event)
{
    if (Manager::isappShuttingDown())
    {
        event.Skip();
        return;
    }

    if (Manager::Get()->GetEditorManager() && event.GetEditor() == Manager::Get()->GetEditorManager()->GetActiveEditor())
        DoUpdateStatusBar();

    event.Skip();
}

void MainFrame::OnToggleBar(wxCommandEvent& event)
{
    wxWindow* win = 0;
    if (event.GetId() == idViewManager)
        win = Manager::Get()->GetProjectManager()->GetNotebook();
    else if (event.GetId() == idViewLogManager)
        win = m_pInfoPane;
    else if (event.GetId() == idViewToolMain)
        win = m_pToolbar;
    else
    {
        wxString pluginName = m_PluginIDsMap[event.GetId()];
        if (!pluginName.IsEmpty())
        {
            cbPlugin* plugin = Manager::Get()->GetPluginManager()->FindPluginByName(pluginName);
            if (plugin)
                win = m_PluginsTools[plugin];
        }
    }

    if (win)
    {
        // use last visible size as BestSize, Logs & others does no longer "forget" it's size
        if (!event.IsChecked())
             m_LayoutManager.GetPane(win).BestSize(win->GetSize());

        m_LayoutManager.GetPane(win).Show(event.IsChecked());
        DoUpdateLayout();
    }
}

void MainFrame::OnToggleStatusBar(wxCommandEvent& event)
{
    wxStatusBar* sb = GetStatusBar();
    if (sb)
    {
        sb->Hide();
        SetStatusBar(0);
        sb->Destroy();
    }
    else
        DoCreateStatusBar();

    DoUpdateStatusBar();
}

void MainFrame::OnFocusEditor(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager() ? Manager::Get()->GetEditorManager()->GetBuiltinEditor(Manager::Get()->GetEditorManager()->GetActiveEditor()) : 0;
    if (ed)
        ed->GetControl()->SetFocus();
}

void MainFrame::OnSwitchTabs(wxCommandEvent& event)
{
    // Get the notebook from the editormanager:
    cbAuiNotebook* nb = Manager::Get()->GetEditorManager()->GetNotebook();
    if (!nb)
        return;

    // Create container and add all open editors:
    wxSwitcherItems items;
    items.AddGroup(_("Open files"), wxT("editors"));
    for (size_t i = 0; i < nb->GetPageCount(); ++i)
    {
        wxString title = nb->GetPageText(i);
        wxWindow* window = nb->GetPage(i);

        items.AddItem(title, title, i, nb->GetPageBitmap(i)).SetWindow(window);
    }

    // Select the focused editor:
    int idx = items.GetIndexForFocus();
    if (idx != wxNOT_FOUND)
        items.SetSelection(idx);

    // Create the switcher dialog
    wxSwitcherDialog dlg(items, wxGetApp().GetTopWindow());

    // Ctrl+Tab workaround for non windows platforms:
    if      (platform::cocoa)
        dlg.SetModifierKey(WXK_ALT);
    else if (platform::gtk)
        dlg.SetExtraNavigationKey(wxT(','));

    // Finally show the dialog:
    int answer = dlg.ShowModal();

    // If necessary change the selected editor:
    if ((answer == wxID_OK) && (dlg.GetSelection() != -1))
    {
        wxSwitcherItem& item = items.GetItem(dlg.GetSelection());
        wxWindow* win = item.GetWindow();
        if (win)
        {
            nb->SetSelection(item.GetId());
            win->SetFocus();
        }
    }
}

void MainFrame::OnToggleFullScreen(wxCommandEvent& event)
{
    ShowFullScreen( !IsFullScreen(), wxFULLSCREEN_NOTOOLBAR// | wxFULLSCREEN_NOSTATUSBAR
                    | wxFULLSCREEN_NOBORDER | wxFULLSCREEN_NOCAPTION );

    // Create full screen-close button if we're in full screen
    if ( IsFullScreen() )
    {
        //
        // Show the button to the bottom-right of the container
        //
        wxSize containerSize = GetClientSize();
        wxSize buttonSize = m_pCloseFullScreenBtn->GetSize();

        // Align
        m_pCloseFullScreenBtn->Move( containerSize.GetWidth() - buttonSize.GetWidth(),
                    containerSize.GetHeight() - buttonSize.GetHeight() );

        m_pCloseFullScreenBtn->Show( true );
        m_pCloseFullScreenBtn->Raise();
    }
    else
    {
        m_pCloseFullScreenBtn->Show( false );
    }
}

void MainFrame::OnPluginInstalled(CodeBlocksEvent& event)
{
    PluginsUpdated(event.GetPlugin(), Installed);
}

void MainFrame::OnPluginUninstalled(CodeBlocksEvent& event)
{
    if (Manager::IsAppShuttingDown())
        return;
    PluginsUpdated(event.GetPlugin(), Uninstalled);
}

void MainFrame::OnPluginLoaded(CodeBlocksEvent& event)
{
    cbPlugin* plug = event.GetPlugin();
    if (plug)
    {
        DoAddPlugin(plug);
        const PluginInfo* info = Manager::Get()->GetPluginManager()->GetPluginInfo(plug);
        wxString msg = info ? info->title : wxString(_("<Unknown plugin>"));
        #if wxCHECK_VERSION(2, 9, 0)
        Manager::Get()->GetLogManager()->DebugLog(F(_T("%s plugin activated"), msg.wx_str()));
        #else
        Manager::Get()->GetLogManager()->DebugLog(F(_T("%s plugin activated"), msg.c_str()));
        #endif
    }
}

void MainFrame::OnPluginUnloaded(CodeBlocksEvent& event)
{
    if (Manager::IsAppShuttingDown())
        return;

    cbPlugin* plugin = event.GetPlugin();

    // remove toolbar, if any
    if (m_PluginsTools[plugin])
    {
        m_LayoutManager.DetachPane(m_PluginsTools[plugin]);
        m_PluginsTools[plugin]->Destroy();
        m_PluginsTools.erase(plugin);
        DoUpdateLayout();
    }

    PluginsUpdated(plugin, Unloaded);
}

void MainFrame::OnSettingsEnvironment(wxCommandEvent& event)
{
    bool tbarsmall = m_SmallToolBar;
    bool needRestart = false;

    EnvironmentSettingsDlg dlg(this, m_LayoutManager.GetArtProvider());
    PlaceWindow(&dlg);
    if (dlg.ShowModal() == wxID_OK)
    {
        DoUpdateEditorStyle();
        DoUpdateLayoutColours();

        m_SmallToolBar = Manager::Get()->GetConfigManager(_T("app"))->ReadBool(_T("/environment/toolbar_size"), true);
        needRestart = m_SmallToolBar != tbarsmall;
        Manager::Get()->GetLogManager()->NotifyUpdate();
        ShowHideStartPage();
    }
    if (needRestart)
        cbMessageBox(_("Code::Blocks needs to be restarted for the changes to take effect."), _("Information"), wxICON_INFORMATION);
}

void MainFrame::OnGlobalUserVars(wxCommandEvent& event)
{
    Manager::Get()->GetUserVariableManager()->Configure();
}

void MainFrame::OnSettingsEditor(wxCommandEvent& event)
{
    Manager::Get()->GetEditorManager()->Configure();
}

void MainFrame::OnSettingsCompilerDebugger(wxCommandEvent& event)
{
    CompilerSettingsDlg dlg(this);
    PlaceWindow(&dlg);
    dlg.ShowModal();
}

void MainFrame::OnSettingsPlugins(wxCommandEvent& event)
{
    Manager::Get()->GetPluginManager()->Configure();
}

void MainFrame::OnSettingsScripting(wxCommandEvent& event)
{
    ScriptingSettingsDlg dlg(this);
    if (dlg.ShowModal() == wxID_OK)
        RunStartupScripts();
}

void MainFrame::OnProjectActivated(CodeBlocksEvent& event)
{
    DoUpdateAppTitle();
    event.Skip();
}

void MainFrame::OnProjectOpened(CodeBlocksEvent& event)
{
    ShowHideStartPage(true);
    event.Skip();
}

void MainFrame::OnEditorOpened(CodeBlocksEvent& event)
{
    DoUpdateAppTitle();
    event.Skip();
}

void MainFrame::OnEditorActivated(CodeBlocksEvent& event)
{
    DoUpdateAppTitle();
    DoUpdateStatusBar();
    event.Skip();
}

void MainFrame::OnEditorClosed(CodeBlocksEvent& event)
{
    DoUpdateAppTitle();
    DoUpdateStatusBar();
    event.Skip();
}

void MainFrame::OnEditorSaved(CodeBlocksEvent& event)
{
    DoUpdateAppTitle();
    event.Skip();
}

void MainFrame::OnEditorModified(CodeBlocksEvent& event)
{
    DoUpdateAppTitle();
    event.Skip();
}

void MainFrame::OnProjectClosed(CodeBlocksEvent& event)
{
    ShowHideStartPage();
    event.Skip();
}

void MainFrame::OnPageChanged(wxNotebookEvent& event)
{
    DoUpdateAppTitle();
    event.Skip();
}

void MainFrame::OnShiftTab(wxCommandEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor(); // Must make sure it's cbEditor and not EditorBase
    if (ed)
        ed->DoUnIndent();
}

void MainFrame::OnRequestDockWindow(CodeBlocksDockEvent& event)
{
    if (Manager::isappShuttingDown())
        return;

    wxAuiPaneInfo info;
    wxString name = event.name;
    if (name.IsEmpty())
    {
        static int idx = 1;
        name = wxString::Format(_T("UntitledPane%d"), idx++);
    }
// TODO (mandrav##): Check for existing pane with the same name
    info = info.Name(name);
    info = info.Caption(event.title.IsEmpty() ? name : event.title);
    switch (event.dockSide)
    {
        case CodeBlocksDockEvent::dsLeft: info = info.Left(); break;
        case CodeBlocksDockEvent::dsRight: info = info.Right(); break;
        case CodeBlocksDockEvent::dsTop: info = info.Top(); break;
        case CodeBlocksDockEvent::dsBottom: info = info.Bottom(); break;
        case CodeBlocksDockEvent::dsFloating: info = info.Float(); break;

        default: break;
    }
    info = info.Show(event.shown);
    info = info.BestSize(event.desiredSize);
    info = info.FloatingSize(event.floatingSize);
    info = info.MinSize(event.minimumSize);
    info = info.Layer(event.stretch ? 1 : 0);
    if (event.row != -1)
        info = info.Row(event.row);
    if (event.column != -1)
        info = info.Position(event.column);
    info = info.CloseButton(event.hideable ? true : false);
    m_LayoutManager.AddPane(event.pWindow, info);
    DoUpdateLayout();
}

void MainFrame::OnRequestUndockWindow(CodeBlocksDockEvent& event)
{
    m_LayoutManager.DetachPane(event.pWindow);
    DoUpdateLayout();
}

void MainFrame::OnRequestShowDockWindow(CodeBlocksDockEvent& event)
{
    m_LayoutManager.GetPane(event.pWindow).Show();
    DoUpdateLayout();

    CodeBlocksDockEvent evt(cbEVT_DOCK_WINDOW_VISIBILITY);
    evt.pWindow = event.pWindow;
    Manager::Get()->ProcessEvent(evt);
}

void MainFrame::OnRequestHideDockWindow(CodeBlocksDockEvent& event)
{
    m_LayoutManager.GetPane(event.pWindow).Hide();
    DoUpdateLayout();

    CodeBlocksDockEvent evt(cbEVT_DOCK_WINDOW_VISIBILITY);
    evt.pWindow = event.pWindow;
    Manager::Get()->ProcessEvent(evt);
}

void MainFrame::OnDockWindowVisibility(CodeBlocksDockEvent& event)
{
//    if (m_ScriptConsoleID != -1 && event.GetId() == m_ScriptConsoleID)
//        ShowHideScriptConsole();
}

void MainFrame::OnLayoutQuery(CodeBlocksLayoutEvent& event)
{
    event.layout = !m_LastLayoutName.IsEmpty() ? m_LastLayoutName : gDefaultLayout;
    event.StopPropagation();
}

void MainFrame::OnLayoutSwitch(CodeBlocksLayoutEvent& event)
{
    LoadViewLayout(event.layout, true);
}

void MainFrame::OnAddLogWindow(CodeBlocksLogEvent& event)
{
    if (Manager::IsAppShuttingDown())
        return;
    wxWindow* p = event.window;
    if (p)
        m_pInfoPane->AddNonLogger(p, event.title, event.icon);
    else
    {
        p = event.logger->CreateControl(m_pInfoPane);
        if (p)
            m_pInfoPane->AddLogger(event.logger, p, event.title, event.icon);
    }
    Manager::Get()->GetLogManager()->NotifyUpdate();
}

void MainFrame::OnRemoveLogWindow(CodeBlocksLogEvent& event)
{
    if (Manager::IsAppShuttingDown())
        return;
    if (event.window)
        m_pInfoPane->RemoveNonLogger(event.window);
    else
        m_pInfoPane->DeleteLogger(event.logger);
}

void MainFrame::OnSwitchToLogWindow(CodeBlocksLogEvent& event)
{
    if (event.window)
        m_pInfoPane->ShowNonLogger(event.window);
    else if (event.logger)
        m_pInfoPane->Show(event.logger);
}

void MainFrame::OnShowLogManager(CodeBlocksLogEvent& event)
{
    if (!Manager::Get()->GetConfigManager(_T("message_manager"))->ReadBool(_T("/auto_hide"), false))
        return;

    m_LayoutManager.GetPane(m_pInfoPane).Show(true);
    DoUpdateLayout();
}

void MainFrame::OnHideLogManager(CodeBlocksLogEvent& event)
{
    if (!Manager::Get()->GetConfigManager(_T("message_manager"))->ReadBool(_T("/auto_hide"), false) ||
           m_AutoHideLockCounter > 0)
        return;

    m_LayoutManager.GetPane(m_pInfoPane).Show(false);
    DoUpdateLayout();
}

void MainFrame::OnLockLogManager(CodeBlocksLogEvent& event)
{
    if (!Manager::Get()->GetConfigManager(_T("message_manager"))->ReadBool(_T("/auto_hide"), false))
        return;
    ++m_AutoHideLockCounter;
}

void MainFrame::OnUnlockLogManager(CodeBlocksLogEvent& event)
{
    if (!Manager::Get()->GetConfigManager(_T("message_manager"))->ReadBool(_T("/auto_hide"), false) &&
           m_AutoHideLockCounter > 0)
        return;
    if (--m_AutoHideLockCounter == 0)
    {
        m_LayoutManager.GetPane(m_pInfoPane).Show(false);
        DoUpdateLayout();
    }
}

void MainFrame::StartupDone()
{
    m_StartupDone = true;
    DoUpdateLayout();
}
