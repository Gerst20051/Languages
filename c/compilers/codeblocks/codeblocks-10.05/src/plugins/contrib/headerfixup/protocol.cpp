/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include "protocol.h"

//(*InternalHeaders(Protocol)
#include <wx/string.h>
#include <wx/intl.h>
//*)

#include <wx/event.h>
#include <wx/gdicmn.h>
#include <wx/utils.h>
#include <wx/window.h>


// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----

//(*IdInit(Protocol)
const long Protocol::ID_LBL_PROTOCOL = wxNewId();
const long Protocol::ID_TXT_PROTOCOL = wxNewId();
//*)

// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----

BEGIN_EVENT_TABLE(Protocol,wxScrollingDialog)
  //(*EventTable(Protocol)
  //*)
END_EVENT_TABLE()

// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----

Protocol::Protocol(wxWindow* parent,wxWindowID id)
{
  //(*Initialize(Protocol)
  Create(parent, wxID_ANY, _("Header Fixup - Protocol"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T("wxID_ANY"));
  sizMain = new wxBoxSizer(wxVERTICAL);
  sizProtocol = new wxStaticBoxSizer(wxVERTICAL, this, _("Protocol"));
  lblProtocol = new wxStaticText(this, ID_LBL_PROTOCOL, _("Protocol for last operation:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_LBL_PROTOCOL"));
  sizProtocol->Add(lblProtocol, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
  m_Protocol = new wxTextCtrl(this, ID_TXT_PROTOCOL, wxEmptyString, wxDefaultPosition, wxSize(480,240), wxTE_MULTILINE|wxTE_READONLY, wxDefaultValidator, _T("ID_TXT_PROTOCOL"));
  m_Protocol->SetToolTip(_("This is the full log of the parser operations."));
  sizProtocol->Add(m_Protocol, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  sizMain->Add(sizProtocol, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  m_OK = new wxButton(this, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("wxID_OK"));
  m_OK->SetDefault();
  m_OK->SetToolTip(_("Click to exit the protocol and return to C::B."));
  sizMain->Add(m_OK, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  SetSizer(sizMain);
  sizMain->Fit(this);
  sizMain->SetSizeHints(this);
  Center();
  
  Connect(wxID_OK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Protocol::OnBtnOKClick);
  //*)
}// Protocol

// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----

void Protocol::OnBtnOKClick(wxCommandEvent& event)
{
  EndModal(wxID_OK);
}// OnBtnOkClick

void Protocol::SetProtocol(const wxArrayString& Protocol)
{
    if (m_Protocol)
    {
        m_Protocol->Freeze();

        const size_t COUNT(Protocol.GetCount());
        for ( size_t i(0); i != COUNT; ++i )
        {
            if ( Protocol[i].StartsWith(wxT("+")) )
            {
                m_Protocol->SetDefaultStyle(wxTextAttr(wxNullColour,wxColour(130,255,130)));
                m_Protocol->AppendText(Protocol[i]);
            }
            else
            {
                m_Protocol->SetDefaultStyle(wxTextAttr(wxNullColour,*wxWHITE));
                m_Protocol->AppendText(Protocol[i]);
            }
        }

        m_Protocol->Thaw();
    }
}// SetProtocol
