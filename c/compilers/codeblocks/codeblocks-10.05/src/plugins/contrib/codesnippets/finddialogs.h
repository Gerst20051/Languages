//////////////////////////////////////////////////////////////////////////////
// File:        dialogs.h
// Purpose:     Find/Replace, Goto dialogs
// Maintainer:  Otto Wyss
// Created:     2003-01-20
// RCS-ID:      $Id: finddialogs.h 58 2007-04-22 04:40:57Z Pecan $
// Copyright:   (c) 2005 wyoGuide
// Licence:     wxWindows
//////////////////////////////////////////////////////////////////////////////
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
// RCS-ID: $Id: finddialogs.h 58 2007-04-22 04:40:57Z Pecan $

#ifndef _DIALOGS_H_
#define _DIALOGS_H_

//----------------------------------------------------------------------------
// information
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// headers
//----------------------------------------------------------------------------

#include "scrollingdialog.h"     // Dialogs

//============================================================================
// declarations
//============================================================================

//----------------------------------------------------------------------------
//! myFindReplaceFlags
enum myFindReplaceFlags {
    // find/replace flags
    myFR_DOWN = 1,
    myFR_WHOLEWORD = 2,
    myFR_MATCHCASE = 4,
    myFR_FINDREGEX = 8,
    myFR_SUBFOLDER = 16,
};

//! myFindReplaceDialogStyles
enum myFindReplaceDialogStyles {
    // find/replace styles
    myFR_FINDINFILES = 1,
    myFR_REPLACEDIALOG = 2,
    myFR_NOUPDOWN      = 4,
    myFR_NOMATCHCASE   = 8,
    myFR_NOWHOLEWORD   = 16,
    myFR_NOFINDREGEX   = 32,
};


//----------------------------------------------------------------------------
//! myFindReplaceDlg
class myFindReplaceDlg: public wxScrollingDialog
{
public:

    //! constructor
    myFindReplaceDlg (wxWindow *parent,
                      const wxString &findstr = wxEmptyString,
                      const wxString &replacestr = wxEmptyString,
                      wxUint32 flags = 0,
                      long style = wxDEFAULT_DIALOG_STYLE);

    //! destructor
    ~myFindReplaceDlg ();

    // event handlers
    void OnCancel (wxCommandEvent& event);
    void OnOkay (wxCommandEvent& event);
    void OnHelp (wxCommandEvent &event);
    void OnReplace (wxCommandEvent& event);
    void OnReplaceAll (wxCommandEvent& event);

    //! get and set variables
    wxString GetFindDirectory ();
    void SetFindDirectory (const wxString &str);
    wxString GetFindFilespec ();
    void SetFindFilespec (const wxString &str);
    wxString GetFindString ();
    void SetFindString (const wxString &str);
    wxString GetReplaceString ();
    void SetReplaceString (const wxString &str);
    int GetFlags ();
    void SetFlags (int flags);

    // show dialog according to style
    int ShowModal (long style = wxDEFAULT_DIALOG_STYLE);

private:
    long m_style;

    // variables
    wxComboBox *m_finddir;
    wxTextCtrl *m_findspec;
    wxComboBox *m_findstr;
    wxComboBox *m_replacestr;
    wxCheckBox *m_matchcase;
    wxCheckBox *m_wholeword;
    wxCheckBox *m_findregex;
    wxCheckBox *m_subfolder;
    wxRadioBox *m_direction;

    // buttons
    wxButton *m_cancelButton;
    wxButton *m_findButton;
    wxButton *m_replaceButton;
    wxButton *m_replaceAllButton;

    // optional sizer items
    wxBoxSizer *m_totalpane;
    wxBoxSizer *m_findpane;
    wxBoxSizer *m_fdirsizer;
    wxBoxSizer *m_specsizer;
    wxBoxSizer *m_replsizer;
    wxBoxSizer *m_buttonpane;
    wxBoxSizer *m_optionpane;
    wxBoxSizer *m_optionsizer;
    wxBoxSizer *m_dirssizer;

    void LoadDirHistory ();
    void StoreDirHistory ();
    void UpdateDirHistory (wxString item = wxEmptyString);
    void LoadFindHistory ();
    void StoreFindHistory ();
    void UpdateFindHistory (wxString item = wxEmptyString);
    void LoadReplaceHistory ();
    void StoreReplaceHistory ();
    void UpdateReplaceHistory (wxString item = wxEmptyString);

    DECLARE_EVENT_TABLE()
};

//----------------------------------------------------------------------------
//! myGotoDlg
class myGotoDlg: public wxScrollingDialog
{
public:

    //! constructor
    myGotoDlg (wxWindow *parent,
               long style = wxDEFAULT_DIALOG_STYLE);

    // event handlers
    void OnHelp (wxCommandEvent &event);

    //! get and set variables
    int GetPosition ();
    void SetPosition (int position);

    // show dialog according to style
    int ShowModal (long style = wxDEFAULT_DIALOG_STYLE);

private:

    // variables
    wxTextCtrl *m_position;

    // buttons
    wxButton *m_cancelButton;
    wxButton *m_gotoButton;

    DECLARE_EVENT_TABLE()
};

#endif // _DIALOGS_H_

