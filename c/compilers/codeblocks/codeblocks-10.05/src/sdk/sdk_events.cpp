/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision: 5152 $
 * $Id: sdk_events.cpp 5152 2008-07-21 19:19:51Z mandrav $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/sdk/sdk_events.cpp $
 */

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include "sdk_events.h"
    #include "cbproject.h"
    #include "editorbase.h"
    #include "cbplugin.h"
    #include "logmanager.h"
#endif


IMPLEMENT_DYNAMIC_CLASS(CodeBlocksEvent, wxEvent)
IMPLEMENT_DYNAMIC_CLASS(CodeBlocksDockEvent, wxEvent)
IMPLEMENT_DYNAMIC_CLASS(CodeBlocksLayoutEvent, wxEvent)
IMPLEMENT_DYNAMIC_CLASS(CodeBlocksLogEvent, wxEvent)


CodeBlocksLogEvent::CodeBlocksLogEvent(wxEventType commandType, Logger* logger, const wxString& title, wxBitmap *icon)
	: wxEvent(wxID_ANY, commandType),
	logger(logger), logIndex(-1), icon(icon), title(title), window(0)
{
	// special case for add
	if (commandType == cbEVT_ADD_LOG_WINDOW && logger)
	{
		if (Manager::Get()->GetLogManager()->FindIndex(logger) == LogManager::invalid_log)
		{
			logIndex = Manager::Get()->GetLogManager()->SetLog(logger);
			cbAssert(logIndex != LogManager::invalid_log);
			Manager::Get()->GetLogManager()->Slot(logIndex).title = title;
			Manager::Get()->GetLogManager()->Slot(logIndex).icon = icon;
			return;
		}
	}

	logIndex = Manager::Get()->GetLogManager()->FindIndex(logger);
}

CodeBlocksLogEvent::CodeBlocksLogEvent(wxEventType commandType, int logIndex, const wxString& title, wxBitmap *icon)
	: wxEvent(wxID_ANY, commandType),
	logger(0), logIndex(logIndex), icon(icon), title(title), window(0)
{
	logger = Manager::Get()->GetLogManager()->Slot(logIndex).GetLogger();
}

CodeBlocksLogEvent::CodeBlocksLogEvent(wxEventType commandType, wxWindow* window, const wxString& title, wxBitmap *icon)
	: wxEvent(wxID_ANY, commandType),
	logger(0), logIndex(-1), icon(icon), title(title), window(window)
{
}

CodeBlocksLogEvent::CodeBlocksLogEvent(const CodeBlocksLogEvent& rhs)
	: logger(rhs.logger), logIndex(rhs.logIndex), icon(rhs.icon), title(rhs.title), window(rhs.window)
{
}


// app events
const wxEventType cbEVT_APP_STARTUP_DONE = wxNewEventType();
const wxEventType cbEVT_APP_START_SHUTDOWN = wxNewEventType();
// plugin events
const wxEventType cbEVT_PLUGIN_ATTACHED = wxNewEventType();
const wxEventType cbEVT_PLUGIN_RELEASED = wxNewEventType();
const wxEventType cbEVT_PLUGIN_INSTALLED = wxNewEventType();
const wxEventType cbEVT_PLUGIN_UNINSTALLED = wxNewEventType();
// editor events
const wxEventType cbEVT_EDITOR_CLOSE = wxNewEventType();
const wxEventType cbEVT_EDITOR_OPEN = wxNewEventType();
const wxEventType cbEVT_EDITOR_ACTIVATED = wxNewEventType();
const wxEventType cbEVT_EDITOR_DEACTIVATED = wxNewEventType();
const wxEventType cbEVT_EDITOR_BEFORE_SAVE = wxNewEventType();
const wxEventType cbEVT_EDITOR_SAVE = wxNewEventType();
const wxEventType cbEVT_EDITOR_MODIFIED = wxNewEventType();
const wxEventType cbEVT_EDITOR_TOOLTIP = wxNewEventType();
const wxEventType cbEVT_EDITOR_TOOLTIP_CANCEL = wxNewEventType();
const wxEventType cbEVT_EDITOR_BREAKPOINT_ADD = wxNewEventType();
const wxEventType cbEVT_EDITOR_BREAKPOINT_EDIT = wxNewEventType();
const wxEventType cbEVT_EDITOR_BREAKPOINT_DELETE = wxNewEventType();
const wxEventType cbEVT_EDITOR_UPDATE_UI = wxNewEventType();
// project events
const wxEventType cbEVT_PROJECT_CLOSE = wxNewEventType();
const wxEventType cbEVT_PROJECT_OPEN = wxNewEventType();
const wxEventType cbEVT_PROJECT_SAVE = wxNewEventType();
const wxEventType cbEVT_PROJECT_ACTIVATE = wxNewEventType();
const wxEventType cbEVT_PROJECT_BEGIN_ADD_FILES = wxNewEventType();
const wxEventType cbEVT_PROJECT_END_ADD_FILES = wxNewEventType();
const wxEventType cbEVT_PROJECT_BEGIN_REMOVE_FILES = wxNewEventType();
const wxEventType cbEVT_PROJECT_END_REMOVE_FILES = wxNewEventType();
const wxEventType cbEVT_PROJECT_FILE_ADDED = wxNewEventType();
const wxEventType cbEVT_PROJECT_FILE_REMOVED = wxNewEventType();
const wxEventType cbEVT_PROJECT_POPUP_MENU = wxNewEventType();
const wxEventType cbEVT_PROJECT_TARGETS_MODIFIED = wxNewEventType();
const wxEventType cbEVT_PROJECT_RENAMED = wxNewEventType();
const wxEventType cbEVT_WORKSPACE_CHANGED = wxNewEventType();
// build targets events
const wxEventType cbEVT_BUILDTARGET_ADDED = wxNewEventType();
const wxEventType cbEVT_BUILDTARGET_REMOVED = wxNewEventType();
const wxEventType cbEVT_BUILDTARGET_RENAMED = wxNewEventType();
const wxEventType cbEVT_BUILDTARGET_SELECTED = wxNewEventType();
// pipedprocess events
const wxEventType cbEVT_PIPEDPROCESS_STDOUT = wxNewEventType();
const wxEventType cbEVT_PIPEDPROCESS_STDERR = wxNewEventType();
const wxEventType cbEVT_PIPEDPROCESS_TERMINATED = wxNewEventType();
// thread-pool events
const wxEventType cbEVT_THREADTASK_STARTED = wxNewEventType();
const wxEventType cbEVT_THREADTASK_ENDED = wxNewEventType();
const wxEventType cbEVT_THREADTASK_ALLDONE = wxNewEventType();
// request app to dock/undock a window
const wxEventType cbEVT_ADD_DOCK_WINDOW = wxNewEventType();
const wxEventType cbEVT_REMOVE_DOCK_WINDOW = wxNewEventType();
const wxEventType cbEVT_SHOW_DOCK_WINDOW = wxNewEventType();
const wxEventType cbEVT_HIDE_DOCK_WINDOW = wxNewEventType();
// ask which is the current view layout
const wxEventType cbEVT_QUERY_VIEW_LAYOUT = wxNewEventType();
// request app to switch view layout
const wxEventType cbEVT_SWITCH_VIEW_LAYOUT = wxNewEventType();
// app notifies that a new layout has been applied
const wxEventType cbEVT_SWITCHED_VIEW_LAYOUT = wxNewEventType();
// app notifies that a docked window has been hidden/shown
const wxEventType cbEVT_DOCK_WINDOW_VISIBILITY = wxNewEventType();
// app notifies that the menubar is started being (re)created
const wxEventType cbEVT_MENUBAR_CREATE_BEGIN = wxNewEventType();
// app notifies that the menubar (re)creation ended
const wxEventType cbEVT_MENUBAR_CREATE_END = wxNewEventType();
// compiler-related events
const wxEventType cbEVT_COMPILER_STARTED = wxNewEventType();
const wxEventType cbEVT_COMPILER_FINISHED = wxNewEventType();
const wxEventType cbEVT_COMPILER_SET_BUILD_OPTIONS = wxNewEventType();
// debugger-related events
const wxEventType cbEVT_DEBUGGER_STARTED = wxNewEventType();
const wxEventType cbEVT_DEBUGGER_PAUSED = wxNewEventType();
const wxEventType cbEVT_DEBUGGER_FINISHED = wxNewEventType();

// logger-related events
const wxEventType cbEVT_ADD_LOG_WINDOW = wxNewEventType();
const wxEventType cbEVT_REMOVE_LOG_WINDOW = wxNewEventType();
const wxEventType cbEVT_SWITCH_TO_LOG_WINDOW = wxNewEventType();
const wxEventType cbEVT_SHOW_LOG_MANAGER = wxNewEventType();
const wxEventType cbEVT_HIDE_LOG_MANAGER = wxNewEventType();
const wxEventType cbEVT_LOCK_LOG_MANAGER = wxNewEventType();
const wxEventType cbEVT_UNLOCK_LOG_MANAGER = wxNewEventType();
