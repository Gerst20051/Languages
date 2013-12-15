/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef SYMTABCONFIG_H
#define SYMTABCONFIG_H

#include "scrollingdialog.h"

class wxCommandEvent;
class wxWindow;

class SymTabConfigDlg : public wxScrollingDialog
{
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
public:
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
           SymTabConfigDlg(wxWindow* parent) :
             parent(parent), SymTabConfigDlgLoaded(false) {}
  virtual ~SymTabConfigDlg();

  int      Execute();

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
private:
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
  void EndModal   (int             retCode);

  void OnWhatToDo   (wxCommandEvent& event);
  void OnSearch     (wxCommandEvent& event);
  void OnClose      (wxCommandEvent& event);
  void OnLibraryPath(wxCommandEvent& event);
  void OnLibrary    (wxCommandEvent& event);
  void OnNM         (wxCommandEvent& event);

  void ToggleWidgets(int choice);
  void LoadSettings();
  void SaveSettings();

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

  wxWindow* parent;
  bool      SymTabConfigDlgLoaded;

  DECLARE_EVENT_TABLE()
};

#endif // SYMTABCONFIG_H
