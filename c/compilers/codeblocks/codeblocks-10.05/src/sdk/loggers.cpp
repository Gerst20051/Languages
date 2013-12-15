/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include "sdk_precomp.h"
#ifndef CB_PRECOMP
    #include <wx/listctrl.h>
    #include <wx/textctrl.h>
    #include "cbexception.h"
    #include "manager.h"
    #include "configmanager.h"
#endif

#include <wx/clipbrd.h>
#include <wx/dataobj.h>

#include "loggers.h"

TextCtrlLogger::TextCtrlLogger(bool fixedPitchFont)
    : control(0), fixed(fixedPitchFont)
{
}

void TextCtrlLogger::CopyContentsToClipboard(bool selectionOnly)
{
    if (control && control->GetValue().IsEmpty() == false && wxTheClipboard->Open())
    {
        wxTheClipboard->SetData(new wxTextDataObject(selectionOnly ? control->GetStringSelection() : control->GetValue()));
        wxTheClipboard->Close();
    }
}

void TextCtrlLogger::UpdateSettings()
{
    if (!control)
        return;

    control->SetBackgroundColour(*wxWHITE);

    int size = Manager::Get()->GetConfigManager(_T("message_manager"))->ReadInt(_T("/log_font_size"), platform::macosx ? 10 : 8);

    wxFont default_font(size, fixed ? wxFONTFAMILY_MODERN : wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    wxFont bold_font(default_font);
    wxFont italic_font(default_font);

    bold_font.SetWeight(wxFONTWEIGHT_BOLD);

    wxFont bigger_font(bold_font);
    bigger_font.SetPointSize(size + 2);

    wxFont small_font(default_font);
    small_font.SetPointSize(size - 4);

    italic_font.SetStyle(wxFONTSTYLE_ITALIC);

    // might try alternatively
    //italic_font.SetStyle(wxFONTSTYLE_SLANT);

    for(unsigned int i = 0; i < num_levels; ++i)
    {
        style[i].SetFont(default_font);
        style[i].SetAlignment(wxTEXT_ALIGNMENT_DEFAULT);
        style[i].SetTextColour(*wxBLACK);
        style[i].SetBackgroundColour(*wxWHITE);

        // is it necessary to do that?
        //style[i].SetFlags(...);
    }

    style[caption].SetAlignment(wxTEXT_ALIGNMENT_CENTRE);
    bigger_font.SetUnderlined(true);
    style[caption].SetFont(bigger_font);

    style[success].SetTextColour(*wxBLUE);

    style[warning].SetFont(italic_font);

    style[error].SetFont(bold_font);
    style[error].SetTextColour(*wxRED);

    style[critical].SetFont(bold_font);
    style[critical].SetTextColour(*wxWHITE);
    style[critical].SetBackgroundColour(*wxRED);
    style[spacer].SetFont(small_font);

    // Tell control about the font change
    control->SetFont(default_font);
} // end of UpdateSettings

void TextCtrlLogger::Append(const wxString& msg, Logger::level lv)
{
    if(!control)
        return;

    ::temp_string.assign(msg);
    ::temp_string.append(_T("\n"));

    if(lv == caption)
    {
        control->SetDefaultStyle(style[info]);
        control->AppendText(::newline_string);

        control->SetDefaultStyle(style[lv]);
        control->AppendText(::temp_string);

        control->SetDefaultStyle(style[spacer]);
        control->AppendText(::newline_string);
    }
    else
    {
        control->SetDefaultStyle(style[lv]);
        control->AppendText(::temp_string);
    }
}


void TextCtrlLogger::Clear()
{
    if(control)
        control->Clear();
}

wxWindow* TextCtrlLogger::CreateControl(wxWindow* parent)
{
    if (!control)
        control = new wxTextCtrl(parent, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH | wxTE_NOHIDESEL | wxTE_AUTO_URL);
    return control;
}


void TimestampTextCtrlLogger::Append(const wxString& msg, Logger::level lv)
{
    if(!control)
        return;

    wxDateTime timestamp(wxDateTime::UNow());
    ::temp_string.Printf(_T("[%2.2d:%2.2d:%2.2d.%3.3d] %s\n"), timestamp.GetHour(), timestamp.GetMinute(), timestamp.GetSecond(), timestamp.GetMillisecond(), msg.c_str());

    control->SetDefaultStyle(style[lv]);
    control->AppendText(::temp_string);
}

ListCtrlLogger::ListCtrlLogger(const wxArrayString& titles, const wxArrayInt& widths, bool fixedPitchFont)
    : control(0), fixed(fixedPitchFont),
    titles(titles), widths(widths)
{
    cbAssert(titles.GetCount() == widths.GetCount());
}


void ListCtrlLogger::CopyContentsToClipboard(bool selectionOnly)
{
    if (control && control->GetItemCount() > 0 && wxTheClipboard->Open())
    {
        wxString text;
        if (selectionOnly)
        {
            int i = control->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
            text = GetItemAsText(i);
        }
        else
        {
            for (int i = 0; i < control->GetItemCount(); ++i)
            {
                text << GetItemAsText(i);
                if (platform::windows)
                {
                    text << _T('\r'); // Add CR for Windows clipboard
                }
                text << _T('\n');
            }
        }
        wxTheClipboard->SetData(new wxTextDataObject(text));
        wxTheClipboard->Close();
    }
}

wxString ListCtrlLogger::GetItemAsText(long item) const
{
    wxString text;

    wxListItem info;
    info.m_itemId = item;
    info.m_mask = wxLIST_MASK_TEXT;

    for (size_t i = 0; i < titles.GetCount(); ++i)
    {
        info.m_col = i;
        control->GetItem(info);
        text << info.m_text << _T('|');
    }
    return text;
} // end of GetItemAsText

void ListCtrlLogger::UpdateSettings()
{
    if (!control)
        return;

    int size = Manager::Get()->GetConfigManager(_T("message_manager"))->ReadInt(_T("/log_font_size"), platform::macosx ? 10 : 8);
    wxFont default_font(size, fixed ? wxFONTFAMILY_MODERN : wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    wxFont bold_font(default_font);
    wxFont italic_font(default_font);

    bold_font.SetWeight(wxFONTWEIGHT_BOLD);

    wxFont bigger_font(bold_font);
    bigger_font.SetPointSize(size + 2);

    wxFont small_font(default_font);
    small_font.SetPointSize(size - 4);

    italic_font.SetStyle(wxFONTSTYLE_ITALIC);

    for(unsigned int i = 0; i < num_levels; ++i)
    {
        style[i].font = default_font;
        style[i].colour = *wxBLACK;
    }

    style[caption].font = bigger_font;
    style[success].colour = *wxBLUE;
    style[failure].colour = wxColour(0x00, 0x00, 0xa0);

    style[warning].font = italic_font;
    style[warning].colour = wxColour(0x00, 0x00, 0xa0); // navy blue

    style[error].colour = *wxRED;

    style[critical].font = bold_font;
    style[critical].colour = wxColour(0x0a, 0x00, 0x00); // maroon

    style[spacer].font = small_font;
    style[pagetitle] = style[caption];

    // Tell control and items about the font change
    control->SetFont(default_font);
    for (int i = 0; i < control->GetItemCount(); ++i)
    {
        wxFont font = control->GetItemFont(i);
        font.SetPointSize(size);
        control->SetItemFont( i, font );
    }//for
} // end of UpdateSettings

void ListCtrlLogger::Append(const wxString& msg, Logger::level lv)
{
    if (!control)
        return;

    int idx = control->GetItemCount();

    control->Freeze();
    control->InsertItem(idx, msg);
    control->SetItemFont(idx, style[lv].font);
    control->SetItemTextColour(idx, style[lv].colour);
    control->Thaw();
}

void ListCtrlLogger::Append(const wxArrayString& colValues, Logger::level lv)
{
    if (!control)
        return;

    if (colValues.GetCount() == 0 || colValues.GetCount() > titles.GetCount())
        return;

    control->Freeze();
    Append(colValues[0], lv);
    int idx = control->GetItemCount() - 1;
    for (size_t i = 1; i < colValues.GetCount(); ++i)
    {
        control->SetItem(idx, i, colValues[i]);
    }
    control->Thaw();
}

size_t ListCtrlLogger::GetItemsCount() const
{
    return control?control->GetItemCount():0;
}

void ListCtrlLogger::Clear()
{
    if(control)
    {
        control->DeleteAllItems();
    }
}

wxWindow* ListCtrlLogger::CreateControl(wxWindow* parent)
{
    if (control)
        return control;

    control = new wxListCtrl(parent, -1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
    for (size_t i = 0; i < titles.GetCount(); ++i)
    {
        control->InsertColumn(i, titles[i], wxLIST_FORMAT_LEFT, widths[i]);
    }
    return control;
}


CSS::CSS() :
    caption     (_T("font-size: 12pt;")),
    info        (wxEmptyString),
    warning     (_T("margin-left: 2em;")),
    success     (wxEmptyString),
    error       (_T("margin-left: 2em; border-left: 1px solid red;")),
    critical    (_T("color: red; font-weight: bold;")),
    failure     (_T("color: maroon;")),
    pagetitle   (_T("font-size: 16pt;")),
    spacer      (wxEmptyString),
    asterisk    (_T("font-family: Arial, Helvetica, \"Bitstream Vera Sans\", sans;"))
{
}

CSS::operator wxString()
{
    wxString tmp(_T('\0'), 300);
    return tmp;
}

HTMLFileLogger::HTMLFileLogger(const wxString& filename)
{
    Open(filename);
}



void HTMLFileLogger::Append(const wxString& msg, Logger::level lv)
{
    fputs(wxSafeConvertWX2MB(msg), f.fp());
    fputs(::newline_string.mb_str(), f.fp());
}

typedef const char* wxStringSucks;


void HTMLFileLogger::Open(const wxString& filename)
{
    FileLogger::Open(filename);
    fputs("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en-US\" lang=\"en-US\">\n<head>\n<title>Build log</title>\n<style>\n", f.fp());

    if(!!css.asterisk)
        fprintf(f.fp(), "* { %s }\n", (wxStringSucks) wxSafeConvertWX2MB(css.asterisk));
    if(!!css.pagetitle)
        fprintf(f.fp(), "h1 { %s }\n", (wxStringSucks) wxSafeConvertWX2MB(css.pagetitle));
    if(!!css.caption)
        fprintf(f.fp(), "h2 { %s }\n", (wxStringSucks) wxSafeConvertWX2MB(css.caption));
    if(!!css.info)
        fprintf(f.fp(), ".info { %s }\n", (wxStringSucks) wxSafeConvertWX2MB(css.info));
    if(!!css.warning)
        fprintf(f.fp(), ".warn { %s }\n", (wxStringSucks) wxSafeConvertWX2MB(css.warning));
    if(!!css.error)
        fprintf(f.fp(), ".error { %s }\n", (wxStringSucks) wxSafeConvertWX2MB(css.error));
    if(!!css.success)
        fprintf(f.fp(), ".success { %s }\n", (wxStringSucks) wxSafeConvertWX2MB(css.success));
    if(!!css.failure)
        fprintf(f.fp(), ".failure { %s }\n", (wxStringSucks) wxSafeConvertWX2MB(css.failure));
    if(!!css.critical)
        fprintf(f.fp(), ".critical { %s }\n", (wxStringSucks) wxSafeConvertWX2MB(css.critical));
    fputs("</style>\n</head>\n\n<body>", f.fp());
}

void HTMLFileLogger::Close()
{
    fputs("</body>\n</html>\n", f.fp());
    FileLogger::Close();
}



