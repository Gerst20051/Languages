/*
	This file is part of Code Snippets, a plugin for Code::Blocks
	Copyright (C) 2007 Pecan Heber

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
// RCS-ID: $Id: version.cpp 113 2008-01-14 18:31:17Z Pecan $

#ifdef WX_PRECOMP
    #include "wx_pch.h"
#else
#endif

#include "version.h"

   #if LOGGING
	wxLogWindow*    m_pLog;
   #endif

// ----------------------------------------------------------------------------
AppVersion::AppVersion()
// ----------------------------------------------------------------------------
{
    //ctor
    m_version = VERSION;
}

// ----------------------------------------------------------------------------
AppVersion::~AppVersion()
// ----------------------------------------------------------------------------
{
    //dtor
}
// ----------------------------------------------------------------------------
//  Commit  1.1.3   2007/03/20
//          01) Add version changelog & debug logging
//          02) Create local svn; add svn props and filter line endings
//          03) Allow running as app or plugin
// ----------------------------------------------------------------------------
//  Commit  1.1.10  2007/03/21
//           4) AddTextToClipboard() & hide "Apply" from App version context menu
//           5) Add portability
//           6) Open secondary windows over parent window (App version)
//           7) Added Settings Dlg to App version
//           8) Added Main menu file open/Save (App version)
//           9) Update About menu text (App version)
//          10) Added Properties to context menu
// ----------------------------------------------------------------------------
//  Commit  1.1.17 2007/03/22
//          11) Add property examination/edit
//          12) Unused "Description" field of properties removed
//          13) Add new Text & File icons
//          14) Show first level of items on startup
//			15) Honor target source line indentation
//			    wxLeaner: http://forums.codeblocks.org/index.php/topic,5375.new.html#new
//          16) Added WX_PRECOMP to all cpp files
//          17) Check for externally changed file
// ----------------------------------------------------------------------------
//  Commit  1.1.54 2007/04/3
//          18) change Version class to AppVersion to avoid CB conflict
//          19) Added check for externally changed file to plugin version
//          20) DeDocked & DeAllocated resources when Snippet window is closed
//          21) Generate .exe along side dll in CodeSnippets-Lmsw.cbp
//          22) Add DragnDrop Tree Items within ctrl and Item/category conversion
//          23) Added context menu "Convert to Category"
//          24) Add snippet external storage management (text & files)
//          25) Allow user to do Save as well as SaveAs
//          26) Added Drag inward to edit control
//          27) Fixed DragnDrop overwriting instead of inserting.
//          28) Changed property editor to wxScintill from wxTextCtrl
//          29) Corrected wxScintilla/Edit horizontal scroll length
//          30) Set scintilla defaults as close to CodeBlccks defaults as possible
//          31) Position child windows as close to parent as wxWidgets allows
//          32) Fixed edit dlg buttons to align center horizontal
//          33) Added basic dragscrolling scintilla editor
//          34) Made edit snippets dialog non-modal to allow dragNdrop/copy/cut/paste
//          35) Made properties dialog non-modal to allow dragNdrop/copy/cut/paste
//          36) Call RequestUserAttention() when app closing & edit|properties win open
//          37) Diaable View Menu when codesnippet modeless dlg in foreground
//          38) Moved EditSnippet() & EditProperties to CodeSnippetsTreeCtrl()
//          39) chenged Edit menu item to use default scintilla editor
//          40) save external file in EditSnippetDlg()
//          41) When using external CodeSnippets, EditorDlg Help button should not show
//          42) FileLinks hand off file to scintilla editor, but leave link info in properties
//          43) EditSnippet moved to CodeSnippetsTreeCtrl
//          44) Added About to RootID context menu
//          45) Fixed Bug: Search options not remembered
//          46) Memorize floating window position on Menu->View->Hide
//          47) Move floating window to last Menu->View->Hide position
//          48) Rearrange menu needs sorting for better sense
//          49) Place Edit dialog to last close position & size
//          50) Fixed Bug: mis-shown/mis-placed view. View Snippets. Open Edit. Drag Snippet floating wind.
//              It turns transparent and stays that way. Close Edit. View Closes but Menu is still active.
//              View Snippets. Dock is now coverering CodeBlocks bec dock now has codeblocks size in .ini.
//              Looks like wxAUI bug. Saying window is shown when it's not.
//          51) Fixed Bug: When window is docked, we're recording CodeBlocks posn & size
//          52) Removed initial Linux floating window to avoid wxAUI menu bug.(cf."Other" category)
//          53) Updated codesnippets.cbp, readme.html, codesnippets.am
//          54) Fix unix error. Add const to use of wxWX2MBbuf csU2C(const wxString& str);
//  Commit  1.1.55 2007/04/10
//          55) Missing event.Skip() in OnIdle() caused debugger to hang.
//  Commit  1.1.56 2007/04/12
//          56) Avoid file changed dialog when .xml does not exist
// ----------------------------------------------------------------------------
//  Commit  1.1.72 2007/04/13
//          55) Fix:App: Not finding MSW .XML & .ini file when CodeSnippets in camel case
//          56) Initial conversion of editor dlg to EditSnippetsFrame() and wxCode editor
//          57) SetMarginWidth according to max num lines in file/snippet
//          58) Add OnScroll routines to correct horizontal scroll errors
//          59) Add Editor menu items FindNext, FindPrev, correct saveFile logic
//          60) Added context menu Copy/Paste of Snippets/Categories to any items.
//          61) Plg: Placement: Center top left corner of child over parent.
//          62) another const for snippetdlg
//          63) Add ".trash" facility for snippet removal
//          64) double click invokes edit, shift double click invokes apply
//          65) Corrected messageBox for Edit use.
//          66) EditSnippetFrame(): reset the undo history to avoid undoing to a blank page
//          67) CenterChildOnParent() moved to editConfig()
//          68) CenterChildOnParent() FindReplace dialog
//          69) "56)" commit above, Edit::Replace, Edit::GoTo, Edit::MatchBrace, messageBoxs, SetDeaultItem
//          70) ::wxMkDir(m_ConfigFolder) if it doesn't exist. CB bug.
//              cf:http://forums.codeblocks.org/index.php?topic=5619.0;topicseen
//          71) Place editor dlg windows (save file warning etc) over Snippet window
//          72) Guard GetFileModificationTime() with if wxFileExists()
//              Correct spelling of /dev/null in linux code
// ----------------------------------------------------------------------------
//  Commit  1.1.73 2007/04/13
//          73) Block re-entry to code via wxCloseEvent(s) and IsTreeBusy()
// ----------------------------------------------------------------------------
//  Commit  1.1.75 2007/04/14
//          74 ) Remove AMD64 ambiguity on finddlg wxComboBox call
//          75 ) Again: Remove AMD64 gentoo ambiguity on finddlg wxComboBox call
// ----------------------------------------------------------------------------
// Published Changes for Version 1.1:
//    Ability to save Snippets as external files
//    Ability to edit externally saved Snippet file
//    New Text vs File Link icons
//    Ability to Rearrange tree with Drag and Drop
//        Drag Snippet to Category
//        Drag Category to other Category
//        Drag Snippet to Snippet creates new Category
//        Drag Snippets/Categories outward and inward.
//    Convert To Category context menu facility
//    Added drag snippet item outward (MSW) w/ both text/file drag formats
//    Dragging snippet File Link to CB opens the file.
//    Changed text editor to Scintilla
//    Added DragScrolling to Editor
//    Allow Editor/Properties DragNDrop in both directions
//    Expand first level Snippets on Startup
//    Check for externally modified XML file
//    Free resources & save index when Snippet window closes
//    Portability
//    User specified window placement
//    User specified XML placement
//    User specified external editor
//    Save XML index file menu entry
//    About info and context menu entry
//    Snippet Properties Dialog
//    Work arounds to avoid wxAUI bugs on MSW/LINUX (cf. version.cpp)
// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//  Commit  1.2.03 2007/04/17
//          01) Add multi-edit session support
//          02) Add DragnDrop of File Links to/from CodeBlocks (MSW)
//          03) cascade subsequent edit windows, else they stack.
//          04-09 reserved for fixes
// ----------------------------------------------------------------------------
//  Commit  1.2.15 2007/04/20
//          10) changed some wxMessageBox(s) to messageBox
//          11) If file already open in editor, just focus it. (use wxTreeItemId)
//          12) messageBox(s) identify themselves as CodeSnippets msgs
//          13) Prevent window size of 0,0
//          14) Prevent multiple instances of CodeSnippets
//          15) Add DragnDrop of File Links to/from CodeBlocks (GTK)
//          16 - 19) reserved for fixes
// ----------------------------------------------------------------------------
//  Commit  1.2.29 2007/04/23
//          20) Removed old editSnippetDlg class
//          21) Enabled Properties edit cut/copy/paste/undo/redo/selectAll
//          22) Fixed: can't force close/switch when menu item active
//          23) Add Index File backup facility
//          24) stahta01 changes 2007/4/21 for wxGTK2.8
//          25) Add ".trash" option to delete external file (ask to delete file)
//          26) Corrected messageBox placement for floating vs docked window (again)
//          27) Fixed Termination/OnActivate crash
//          28) Fixed bug on Linux, the executable folder wx being reported incorrectly
//          29-32) reserved
// ----------------------------------------------------------------------------
//  Commit  1.2.42 2007/04/27
//          33) Add dragscrolling options to settingsdlg() & snippetsconfig()
//          34) snippetsconfig() globally manages all config items for all classes
//          35) Fixes to allow docked window to restart at  previous position
//          35) More hacking to work around wxAUI docked/floating windows deficiencies
//          35) Added settings preference for window type & mouse scrolling
//          36) Corrections for linux settings preferences
//          37) Corrected path for codesnippets; add wxLearner gtkx.h includes for *nix
//          38) Fixes to OnUpdateUI routine when switching between window types
//          39) Verfied wx2.8.3 fixed closing Docked window crash
//          40) Added wxProcess::Exists() to check on external snippets window
//          41) Fixed wx2.8.3 RightClick not selecting tree item
//          42) Fixed external snippet storage folder not being recorded
//          43-49) reserved
// ----------------------------------------------------------------------------
//  Commit  1.2.51 2007/04/28
//          50) On startup, wait for user to open external window with view/snippets menu
//          51) Fixed CB fullscreen editor focus. Make editors modal while they have focus.
//          52-55) reserved
// ----------------------------------------------------------------------------
//  Commit  1.2.59 2007/05/1
//          56) Restore statusbar text on idle
//          57) Pass independent window a keepAlive Pid. ping it to save data.
//          58) Add plgn communications to independent windows via memory mapped file
//          59) Clean up temp memory mapped file on termination
// ----------------------------------------------------------------------------
//  Commit  1.2.61 2007/05/2
//          60) Change modal editors to wxFRAME_FLOAT_ON_PARENT (works much better)
//          61) UnIconize and focus hidden|iconsized editors when user re-selects tree item
//          62) Corrections to FindAppPath() when pgm started without .exe .bat etc.
// ----------------------------------------------------------------------------
//  Commit  1.2.63 2007/05/3
//          63) Incorporate mappedmemoryfile fixes by Tim S
// ----------------------------------------------------------------------------
//  Commit  1.2.64
//          64) Reverted to .63 after linux makefile barfed on preprocessor flags
// ----------------------------------------------------------------------------
//  Commit  1.2.65 2007/05/5
//          65) Removed or disabled all code on linux which depended on makefile preprocessor flags
// ----------------------------------------------------------------------------
//  Commit  1.2.66 2007/05/7
//          66) Modified code, cbp's and Makefile.am's to allow linux preprocessor defs
// ----------------------------------------------------------------------------
//  Commit  1.2.66 2007/05/7
//          66) Modified code, cbp's and Makefile.am's to allow linux preprocessor defs
//          1.2.66 2007/05/9
//          67) Commited w/o version change
//				Changed resource/Makefile.am LDADD to only use needed libs
// ----------------------------------------------------------------------------
//  Commit  1.2.69 2007/05/9
//          68) update.bat remove old unused commands
//          69) Set LD_LIBRARY_PATH relative to AppPath
// ----------------------------------------------------------------------------
//  Commit  1.2.70 2007/05/10
//          70) Fix duplicate menu entry
// ----------------------------------------------------------------------------
//  Commit  1.2.71 2007/05/11
//          71) Fixes for use of non-precompiled headers
// ----------------------------------------------------------------------------
//  Commit  1.2.73 2007/05/21
//          72) Add Recently Used Indexes
//          73) Fix error in category drag
// ----------------------------------------------------------------------------
//  Commit  1.2.76 2007/05/29
//          74) Fixed: Not saving xml changes before "recently used" load
//          75) Show filename base as root item label
//          Verify:If external snippets, plgn *shouldnt* save conf OR xml on exit
//              Ok: saves done by OnClose SnippetsWindow & Tree routines
//          76) Fix "Apply" menu item, misspelled _WX... preprocessor
// ----------------------------------------------------------------------------
//  Commit  1.2.77 2007/06/7
//          77) Prepend CodeBlocks --personality= argument to codesnippets.ini
// ----------------------------------------------------------------------------
//  Commit  1.2.82 2007/06/25
//          78) Use text up to first '\r' to determine if snippet is file link.
//              Allows notes to accompany file link.
//          79) Added MIME open support using Alt-double-click & "Open File" menu
//          80) Refactored EditSnippet and OpenFileLink
//          81) Add "Open Url" support
//          82) ReInstate ToolTips for wx284 using first line of snippet
// ----------------------------------------------------------------------------
//  Commit  1.2.83 2007/06/27
//          83) Fixed: Index might not be saved when changing category label
// ----------------------------------------------------------------------------
//  Commit  1.2.84 2007/06/30
//          84) Fixed: Get temp directory name from system; don't assume.
// ----------------------------------------------------------------------------
//  Commit  1.2.87 2007/07/14
//          85) changes for sdk RegisterEventSink
//          86) invoke properties dialog on "new snippet"
//          87) do not copy removed snippet to .trash when shift key down
// ----------------------------------------------------------------------------
//  Commit  1.2.89 2007/07/21
//          88) fixed missing window when switching layouts
//          89) auto show window when switching between window types
// ----------------------------------------------------------------------------
//  Commit  1.2.90 2007/07/26
//          90) Don't save snippets xml when no changes made in order to avoid
//              file time change and annoying reload msg in secondary CB.
// ----------------------------------------------------------------------------
//  Commit  1.2.92 2007/08/2
//          91) Added check for possible garbage window ptr in OnTreeCtrlEvent()
//          92) Fixed TextToFilenames() for list/text containing \r & \n
// ----------------------------------------------------------------------------
//  Commit  1.2.100 2007/10/30
//          93) Added print facility to app
//          1.2.95 2007/09/30
//          94) change GetAppWindow to GetAppFrame
//          95) Fix crash in CodeSnippetsTreeCtrl::OnIdle when plugin disabled
//          96) Fix crash in CodeSnippetsWindow::OnClose when plgn disabled & CB shuts down
//          1.2.98 2007/10/29
//          97) Fix crash in CodeSnippetsTreeCtrl::OnIdle when float/docked switch
//          98) Incorporate killerbot.cbp and ceniza valgrind mods
//          99) Implement macro substitution requested by mariocup
//         100) Stop crashes: disallow re-enabled plugin until CB reloads
// ----------------------------------------------------------------------------
//  commit  1.2.104 2007/11/29
//          101) mods to compensate for CB OpenFilesList change
//          102) fixed incorrect selection of dragged tree item
//          103) make it easier to rename a catagory
//          104) Add "U" xpm for url type snippets
// ----------------------------------------------------------------------------
//  commit  1.2.105 2007/12/01
//          105) fix Linux crash when CB terminates and window is floating
//          106) on Linux disable drag/drop out of Project panel because of
//               recurring drag/drop freeze bug
// ----------------------------------------------------------------------------
//  commit  1.2.108 2007/12/02
//          107) force close of xml in OnRelease() (Closing window causes crashes)
//          108) immediately delete canceled "New category" & "New snippet" items,
//               ignore filelinks in properties. Avoids unnecessary "delete?" dialog.
//          109) fix macro substitution in CheckForMacros() broken by mod 99.
//               Dialog substitutions must preceed CB macro substitution.
// ----------------------------------------------------------------------------
//  Commit  1.2.109 2008/01/2
//          110) Change GetMessageManager to GetLogManager for wxMAC.
// ----------------------------------------------------------------------------
//  Commit  1.2.111 2008/01/7
//          111) Add ftp://,file://,.htm,.html to url checking.
//          112) Add "(as filetype)" to Open File menu item for clarity.
// ----------------------------------------------------------------------------
//  Commit  1.2.113 2008/01/14
//          113) Fixed: When a pgm started after CodeBlocks, and an editorFrame
//               started by the context menu wax closed, the external pgm got the focus.
// ----------------------------------------------------------------------------
//          2008/03/17
//          114) Save Doc file in CodeSnippetsTreeCtrl::OnIdle(), not just mark as changed.
//          2008/03/18
//          115) Avoid " ..." only tooltip and verify XML text edit wxCancel does not save.
//          2008/03/19
//          116) Add unique id to each XML element
//          117) Add FindTreeItemBySnippetId() routine
//  Commit  1.2.118 2008/04/7
//          118) Fixed interference of SetFocus with EditLabel in tree ctrl
// ----------------------------------------------------------------------------
//  Search  1 3.__ Port of ThreadSearch to CodeSnippets
//          01) Added codesnippetsevent 2008/04/1
//          02) Good execution of ThreadSearchFrame.cpp for extended search. 2008/04/4
//          03) Dont allow multiple SnippetSearchFrames, do raise/re-focus 2008/04/5
//               Write conf changes in CodeSnippetsApp::OnClose()
//          04) Corrected OnClose sequence so that ThreadSearchFrame::OnClose
//               gets called from CodeSnippetsWindow::OnClose() which gets called
//               from CodeSnippetsApp::OnClose() 2008/04/6
//          07) Code to broadcast ThreadSearch CodeSnippets .xml index loading 2008/04/8
//          08) Added PostCodeSnippetsEvent() routine so that propagation works. 2008/04/9
//          09) Added codeblocks.dll/resources.zip search to LoadConfig()
//              to use as global data/ressources path. 2008/04/10
//          10) Add renumbering of snippet id's before saving
//              Issue NEW_INDEX event when saving file 2008/04/13
//          11) Restore ThreadSearchFrame position when created
//          12) Eliminate duplicate check box choices as in Options to get
//              some room on main panel. 2008/04/14
//          13) Fixed Thread splitter window giving no space to log when preview used.
//              Caused by UnSplit() in ThreadSearchFrame main splitter.
//          14) Switched PostCodeSnippetsEvent to finding "SCIwindow" to avoid posting
//              incorrect secondary "Flat Notebook" on wrong child path.
//          15) Event, array, and routines to search FileLinks in .xml index. 2008/04/15
//              Optimized filling FileLinksMapArray at Options change and index change
//          16) Added separate editormanager for extended search frame
//          17) Solved crashes at ThreadSearchFrame::OnClose:Destroy() by
//              removing Connects() to CodeSnippetsWindow::OnClose(). wxAUI does
//              not support wxEVT_CLOSE and it causes crashes
//          18) Activated CB notebook editor if already open, else use TS notebook
//          19) Removed all NotifyPlugins calls from TS separate edtiormanagers
//          20) Incorporate DragScroll into editor; Add DragScrollEvents processing 2008/04/26
//          21) Unsplit ThreadSearchView when closing last editor
//          22) fixed hanging "No files to searc in" cbMessageBox
//          23) SyncLoggerToPreView() to show selection in listCtrl and set focus
//              in OnTmrListCtrlUpdate() after all windows redrawn.
//          24) Set selection dirname and filename into status bar
//          25) Replace LeftMouseClick selection after RightMouseDrag
//          26) Change /CodeSnippets/images back to /ThreadSearch/Images 2008/04/28
//          26) DragScroll rescan after "Options" to catch changed windows
//          27) Removed "CheckItem" for Insert/Append idMenuViewThreadSearch
//          28) Invoke DragScroll config dlg from CodeSnippetsWindow() via event
//          29) Removed DragScroll dialog variables from CodeSnippets dialog
//          30) Change wxFileConfig to consistent local var. Solves config file data errs
//          31) Allocate EditorManagers per wxFrame 2008/05/1
//          32) EditSnippetFrame: Close all editors before deleting EditorManager
//              bec. wxWidgets deletes wxFlatNotebook before calling ~EditorManager().
//          33) RemoveEventHandler(EditorManager), else crash in ctor.
//          34) Convert editsnippetframe menu to use cbEditor calls 2008/05/2
//          35) Removed SEditorManager CreateSearchLog(). Causes Assert crashes.
//          36) Convert wyo properties dlg to use EditorManager facilities.
//          37) change SEditorManager notebook ProjectFileOptionsDlg to EditProperies dlg
//          38) Error msg for missing external editor when using properties ExtEdit button
//          39) Removed BUILDING_PLUGIN dependency to determine IsPlugin() & IsApplication()
//          40) Corrections for Linux
//          41) Create SEditorManager before calling CreateMenu in editsnippetframe
//          42) Adjustments/Fixes/Hair loss for Linux 2008/04/07
//          43) Fixed Crash in SetEditorStyleAfterFileOpen() calling cbEditor instead of ScbEditor 2008/04/08
//          44) Support --prefix="data/resource/base/directory" parameter
//          45) CodeSnippets plugin absorbs DragScrollEvents when DragScroll not loaded.  2008/05/18
//          46) Validate DragScroll exists to issue safe DragScrollEvent
//          47) Correct err in Gotoline when file already open in CB
//          48) Add "Full Search" item to root context menu 2008/05/19
//          49) Add DragnDrop to ThreadSearchFrame to open dragged in filenames
//          50) Fix mouse warping when dragging out of tree and windows overlap
//          51) Remove old edit KeyDownEvent interference from SnippetProperty class 2008/05/20
//          52) Put notebook sash back to previous user position after frame resizing
//              placed it into the middle of the frame.
//          53) Write EdNotebookSashPosn to config file. Use to set splitter window
//          54) SetMinimumSashSize so EdNotebook doesnt disappear on sash double click
//          55) Updated SeparateEditor to Rev 5076
//          56) Reset T'Search log List colume sizes to last user usage setting
//          57) Hide m_pChkShowThreadSearchToolBar option in ThreadSearchConfPanel
//              because CodeSnippets search does not need it.
//              Hide m_pRadPanelManagement conf option since Layout is not used
//          58) Fixes to ../resources/Makefile.am and codeblocks-unix.cbp 2008/06/22
//          59) Change Ctrl-L LineDelete() to LineCut() like CB 2008/06/26
//          60) Application mode allows edtiors to recede to background 2008/06/26
//          61) Fix crash when External Application should not invoke DragScroll config 2008/06/26
//          62) Add EditorsStayOnTop option 2008/06/27
//          63) Correct "--KeyAlivePid=<pid>" argument format  2008/06/28
//          64) Add config folder info to Settings dialog
//          65) Fix error msg caused by calling wxFileConfig with empty path
//          66) Update ThreadSearchFrame Help About box 2008/07/1
//          67) On ThreadSearch log double click, highlite categories but edit text snippets 2008/07/1
// ----------------------------------------------------------------------------
//  Commit 1 3.67 2008/07/12
// ----------------------------------------------------------------------------
//  Commit 1 3.68 2008/07/13
//          68) Fix standalone modules inablility to run as portable exe 2008/07/13
//  Commit 1 3.69 2008/07/14
//          69) Jens' fix for broken CheckForModifiedFiles
//  Commit 1 3.70 2008/07/20
//          70) Fix FullSearch file open crash when CodeCompletion loaded
//  Commit 1 3.72 2008/07/20
//          71) Disable Find and FindInFiles in FullSearch Preview window to prevent crashes
//          72) Disable FindInFiles in FullSearch editors (causing crashes)
// ----------------------------------------------------------------------------
//  Commit 1 3.80 2008/07/30
//          73) Center find dlg in parent frame
//          74) Invoke CheckForModifiedFiles in EditSnippetFrame EVT_ACTIVATE
//              Invoke CheckForModifiedFiles in ThreadSearchFrame EVT_ACTIVATE
//          75) Pass URL/URI in dragndrop as file for browsers
//          76) App:Fix activating first instance when starting secondary instances, then exit
//          77) {enter} in snippets properties label simulates OK
//          78) Fixed: App: CodeSnippets XML Index is not being saved OnClose [x].(2008/07/30)
//          79) Fixed: Loop caused by cross-frame event posting in EVT_ACTIVATE routines
//          80) Speed up .cbp compile time by sharing .obj directory betwn plugin & standalone
// ----------------------------------------------------------------------------
//  Commit 1 3.92
//          81) App:Added RemoveFileFromRecentHistory routine
//          82) Allow user to specify new filename at "Load Index File..."
//          83) Adjust CodeSnippet TreeCtrl font via Ctrl-MouseWheel
//          84) Adjust ThreadSearch log font via Ctrl-MouseWheel
//          85) Delete temporary files created when editing xml text items
//          86) After external file mod and reload, call ScbEditor::SetEditorStyleAfterFileOpen()
//              to re-establish margin style, width, etc.
//          87) Add file dragndrop to EditSnippetFrame
//          88) Add wxLog::EnableLog(false) for release app to stop wxWidget wxMessageBox's
//              when user starts CodeSnippets.exe without a CodeBlocks .conf file.
//          89) Removed memoryMappedFile code for compatibility with non-linux unixes
//          90) Fixed missing events when CS switched from External to docked/floating
//          91) Auto start CS app if running in previous session.
//              Auto show docked or floating when switching from app
//          92) Ask user if s/he really wants multiple invocations of CS from same folder.
// ----------------------------------------------------------------------------
//  Commit 1 3.98 2009/04/26
//          93) When passing URL, remove anything pass \r or \n
//          94) Fixed: A dragged URL item to a snippet did not save. CS did not know it had changed.
//          97) Added zoom functions to Apps DragScroll class
//          98) Added GetCBConfigDir() to call routines that check for user APPDATA var
// ----------------------------------------------------------------------------
//  Commit 1 3.99 2009/04/29
//          99) App: check for standalone execution before closing in OnClose() to
//              avoid error "can't delete default.conf.cbTemp"
// ----------------------------------------------------------------------------
//  Commit 1.3.102 2009/05/26
//          100) Remove app default.conf.cbtemp from /APPDATA/codesnippets in OnClose;
//          101) Correct .cbtemp name to .cbTemp for linux
//          102) Don't raise focus when ::wxActiveWindow != CodeSnippetsTreeCtrl in OnEnterWindow()
// ----------------------------------------------------------------------------
//  Commit 1.3.103 2009/06/1
//          103) Fix crash when backspace\cut after Ctrl-A
// ----------------------------------------------------------------------------
//  Commit  1.3.105 2009/06/8
//          104) Set dragged textdata like dragged filename for FireFox3
//          105) Replace home grown messageBox with GenericMessaageBox
// ----------------------------------------------------------------------------
//  Commit  1.3.107 2009/06/22
//          106) Add "Settings..." to plugin ">" button menu.
//          107) Use APPDATA envar on first run since CodeBlocks returns ""
//               for config folder in OnAttach().
// ----------------------------------------------------------------------------
//  Commit 1.3.118 2009/11/8
//          107) re-enable context menu for editors. Needs more work
//          108) Added config option to turn off tooltips
//          109) Added Save/Close to context menu and filtered entries
//          110) Correct behavior of Close() for editor tabs in SEditorManager
//          111) Import CB resouces when using external codesnippets folder
//          112) Validate dir structure before saving .xml file (SaveItemsToFile)
//          113) Double click or Enter key on category expands or collapses tree item
//          114) Fix behavior of docked/floating window at OnRelease()
//          115) Call macro replace only if text has some of "$%["
//          116) Add support for vendors other than "codeblocks"
//          117) Fix erroneous double entry into "Save Index As" routine
//          118) Fix crash in SEditorManager::CheckForExternallyModifiedFiles();
// ----------------------------------------------------------------------------
//  Commit 1.3.120 2010/02/11
//          119) Fix crash when double clicking previously opened snippet
//          120) Copy fixes 5995 & 6000 from CB EditorManager to SEditormanager
// ----------------------------------------------------------------------------
//  Bugs
//      2008/04/8
//          If floating wxAUI window is docked with Edit's open, they disappear. But
//            "void CodeSnippetsTreeCtrl::EditSnippet(SnippetItemData* pSnippetItemData, wxString fileName)"
//            still thinks they're open, but cannot show them.
//            Eg. Drag docked CodeSnippets to floating. Open a file. Re-dockit.
//            Editors disappear. Show(true) didnt work;
//
// ----------------------------------------------------------------------------
//  ToDo
//          Update Wiki for FullSearch instructions
//          Enable Editor context options, esp. ThreadSearch item
//
//  CodeSnippets
//          Search should continue to next matching item, not just stop (F3?)
//          Help should invoke browser for wiki article
//          Update wiki for Re-arranging Tree items/multi-editing/.trash
//              backup, wrapper execution, Drag'nDrop
//
//  Other
//          Wierd but Ok Department: Dragging a file within .trash asks to "Delete file?"
//              But who'll do that? Reason: OnEndTreeItemDrag() calls RemoveItem() from .trash;
//          Bug: 2007/08/2 Cannot fix
//              wxGTK 2.8.4 cashes when dragging fast out of Mgt/File trees
//              bug was reported way back in wxGTK 2.1.0
//          Dragging file out of file panels does not work on Linux. Causes GTK to freeze.
//          2008/08/21
//          First drag of item to a category does not work sometimes. second does.
//          This seems to be ok with wx2.8.10
//          2008/10/11
//          After dragging item, next edit uses dragged item, not newly clicked item
//          This seems to be fixed with wx2.8.10
//          2009/01/10
//          double clicking for new edit item caused old edit item window to appear.
//          This seems to be fixed with wx2.8.10
//
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
