#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include <wx/arrstr.h>
    #include <wx/filename.h>
    #include <wx/listctrl.h>
    #include "manager.h"
    #include "editormanager.h"
    #include "cbeditor.h"
#endif
#include "cbstyledtextctrl.h"

#include "ValgrindListLog.h"

namespace
{
	const int ID_List = wxNewId();
};

BEGIN_EVENT_TABLE(ValgrindListLog, wxEvtHandler)
//
END_EVENT_TABLE()

ValgrindListLog::ValgrindListLog(const wxArrayString& Titles, wxArrayInt& Widths)
    : ListCtrlLogger(Titles, Widths)
{
	//ctor
}

ValgrindListLog::~ValgrindListLog()
{
	//dtor
    if (control && !Manager::IsAppShuttingDown())
    control->RemoveEventHandler(this);

}


// TODO : use Getter instead of protected 'control'

wxWindow* ValgrindListLog::CreateControl(wxWindow* parent)
{
	ListCtrlLogger::CreateControl(parent);
    control->SetId(ID_List);
    Connect(ID_List, -1, wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
            (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction)
            &ValgrindListLog::OnDoubleClick);
	control->PushEventHandler(this);
	return control;
}

void ValgrindListLog::OnDoubleClick(wxCommandEvent& /*event*/)
{
	// go to the relevant file/line
	if (control->GetSelectedItemCount() == 0)
	{
		return;
	}
	// find selected item index
	const int Index = control->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	SyncEditor(Index);
} // end of OnDoubleClick

void ValgrindListLog::SyncEditor(int SelIndex)
{
	wxFileName Filename(control->GetItemText(SelIndex));
	wxString File;
//	if (!Filename.IsAbsolute())
//	{
//		Filename.MakeAbsolute(m_Base);
//	}
	File = Filename.GetFullPath();

	wxListItem li;
	li.m_itemId = SelIndex;
	li.m_col = 1;
	li.m_mask = wxLIST_MASK_TEXT;
	control->GetItem(li);

	long Line = 0;
	li.m_text.ToLong(&Line);
	cbEditor* Editor = Manager::Get()->GetEditorManager()->Open(File);
	if (!Line || !Editor)
	{
		return;
	}

	Line -= 1;
	Editor->Activate();
	Editor->GotoLine(Line);

	if (cbStyledTextCtrl* Control = Editor->GetControl())
	{
		Control->EnsureVisible(Line);
	}
}
