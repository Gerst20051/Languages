////////////////////////////////////////////////////////////////////////////
// Name:        wxscintilla.cpp
// Purpose:     A wxWidgets implementation of Scintilla.  This class is the
//              one meant to be used directly by wx applications.  It does not
//              derive directly from the Scintilla classes, but instead
//              delegates most things to the real Scintilla class.
//              This allows the use of Scintilla without polluting the
//              namespace with all the classes and identifiers from Scintilla.
//
// Author:      Robin Dunn
//
// Created:     13-Jan-2000
// RCS-ID:      $Id: wxscintilla.cpp 6163 2010-02-15 11:15:27Z mortenmacfly $
// Copyright:   (c) 2004 wxCode
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////
#include <limits>
#include <ctype.h>

#include "ScintillaWX.h"
#include "wx/wxscintilla.h"

#include <wx/wx.h>
#include <wx/tokenzr.h>
#include <wx/mstream.h>
#include <wx/image.h>
#include <wx/file.h>

#ifdef __WXGTK__
    #include <wx/dcbuffer.h>
#endif

//----------------------------------------------------------------------

const wxChar* wxSCINameStr = wxT("SCIwindow");

#ifdef MAKELONG
#undef MAKELONG
#endif

#define MAKELONG(a, b) ((a) | ((b) << 16))


static long wxColourAsLong(const wxColour& co)
{
    return (((long)co.Blue()  << 16) |
            ((long)co.Green() <<  8) |
            ((long)co.Red()));
}

static wxColour wxColourFromLong(long c)
{
    wxColour clr;
    clr.Set((unsigned char)(c & 0xff),
            (unsigned char)((c >> 8) & 0xff),
            (unsigned char)((c >> 16) & 0xff));
    return clr;
}


static wxColour wxColourFromSpec(const wxString& spec)
{
    // spec should be a colour name or "#RRGGBB"
    if (spec.GetChar(0) == wxT('#')) {

        long red, green, blue;
        red = green = blue = 0;
        spec.Mid(1,2).ToLong(&red,   16);
        spec.Mid(3,2).ToLong(&green, 16);
        spec.Mid(5,2).ToLong(&blue,  16);
        return wxColour((unsigned char)red,
                        (unsigned char)green,
                        (unsigned char)blue);
    }
    else
        return wxColour(spec);
}

//----------------------------------------------------------------------

DEFINE_EVENT_TYPE( wxEVT_SCI_CHANGE )
DEFINE_EVENT_TYPE( wxEVT_SCI_STYLENEEDED )
DEFINE_EVENT_TYPE( wxEVT_SCI_CHARADDED )
DEFINE_EVENT_TYPE( wxEVT_SCI_SAVEPOINTREACHED )
DEFINE_EVENT_TYPE( wxEVT_SCI_SAVEPOINTLEFT )
DEFINE_EVENT_TYPE( wxEVT_SCI_ROMODIFYATTEMPT )
DEFINE_EVENT_TYPE( wxEVT_SCI_KEY )
DEFINE_EVENT_TYPE( wxEVT_SCI_DOUBLECLICK )
DEFINE_EVENT_TYPE( wxEVT_SCI_UPDATEUI )
DEFINE_EVENT_TYPE( wxEVT_SCI_MODIFIED )
DEFINE_EVENT_TYPE( wxEVT_SCI_MACRORECORD )
DEFINE_EVENT_TYPE( wxEVT_SCI_MARGINCLICK )
DEFINE_EVENT_TYPE( wxEVT_SCI_NEEDSHOWN )
DEFINE_EVENT_TYPE( wxEVT_SCI_PAINTED )
DEFINE_EVENT_TYPE( wxEVT_SCI_USERLISTSELECTION )
DEFINE_EVENT_TYPE( wxEVT_SCI_URIDROPPED )
DEFINE_EVENT_TYPE( wxEVT_SCI_DWELLSTART )
DEFINE_EVENT_TYPE( wxEVT_SCI_DWELLEND )
DEFINE_EVENT_TYPE( wxEVT_SCI_START_DRAG )
DEFINE_EVENT_TYPE( wxEVT_SCI_DRAG_OVER )
DEFINE_EVENT_TYPE( wxEVT_SCI_DO_DROP )
DEFINE_EVENT_TYPE( wxEVT_SCI_ZOOM )
DEFINE_EVENT_TYPE( wxEVT_SCI_HOTSPOT_CLICK )
DEFINE_EVENT_TYPE( wxEVT_SCI_HOTSPOT_DCLICK )
DEFINE_EVENT_TYPE( wxEVT_SCI_CALLTIP_CLICK )
DEFINE_EVENT_TYPE( wxEVT_SCI_AUTOCOMP_SELECTION )
DEFINE_EVENT_TYPE( wxEVT_SCI_INDICATOR_CLICK )
DEFINE_EVENT_TYPE( wxEVT_SCI_INDICATOR_RELEASE )
DEFINE_EVENT_TYPE( wxEVT_SCI_AUTOCOMP_CANCELLED )
DEFINE_EVENT_TYPE( wxEVT_SCI_AUTOCOMP_CHARDELETED )
DEFINE_EVENT_TYPE( wxEVT_SCI_SETFOCUS )
DEFINE_EVENT_TYPE( wxEVT_SCI_KILLFOCUS )


BEGIN_EVENT_TABLE(wxScintilla, wxControl)
    EVT_PAINT                   (wxScintilla::OnPaint)
    EVT_SCROLLWIN               (wxScintilla::OnScrollWin)
    EVT_SCROLL                  (wxScintilla::OnScroll)
    EVT_SIZE                    (wxScintilla::OnSize)
    EVT_LEFT_DOWN               (wxScintilla::OnMouseLeftDown)
    // Let Scintilla see the double click as a second click
    EVT_LEFT_DCLICK             (wxScintilla::OnMouseLeftDown)
    EVT_MOTION                  (wxScintilla::OnMouseMove)
    EVT_LEFT_UP                 (wxScintilla::OnMouseLeftUp)
#if defined(__WXGTK__) || defined(__WXMAC__)
    EVT_RIGHT_UP                (wxScintilla::OnMouseRightUp)
#else
    EVT_CONTEXT_MENU            (wxScintilla::OnContextMenu)
#endif
    EVT_MOUSEWHEEL              (wxScintilla::OnMouseWheel)
    EVT_MIDDLE_UP               (wxScintilla::OnMouseMiddleUp)
    EVT_CHAR                    (wxScintilla::OnChar)
    EVT_KEY_DOWN                (wxScintilla::OnKeyDown)
    EVT_KILL_FOCUS              (wxScintilla::OnLoseFocus)
    EVT_SET_FOCUS               (wxScintilla::OnGainFocus)
    EVT_SYS_COLOUR_CHANGED      (wxScintilla::OnSysColourChanged)
    EVT_ERASE_BACKGROUND        (wxScintilla::OnEraseBackground)
    EVT_MENU_RANGE              (10, 16, wxScintilla::OnMenu)
    EVT_LISTBOX_DCLICK          (wxID_ANY, wxScintilla::OnListBox)
END_EVENT_TABLE()


IMPLEMENT_CLASS(wxScintilla, wxControl)
IMPLEMENT_DYNAMIC_CLASS(wxScintillaEvent, wxCommandEvent)

#ifdef LINK_LEXERS
// forces the linking of the lexer modules
int Scintilla_LinkLexers();
#endif

//----------------------------------------------------------------------
// Constructor and Destructor

wxScintilla::wxScintilla (wxWindow *parent,
                          wxWindowID id,
                          const wxPoint& pos,
                          const wxSize& size,
                          long style,
                          const wxString& name)
{
    m_swx = NULL;
    Create(parent, id, pos, size, style, name);
}


bool wxScintilla::Create (wxWindow *parent,
                          wxWindowID id,
                          const wxPoint& pos,
                          const wxSize& size,
                          long style,
                          const wxString& name)
{
#ifdef __WXMAC__
    style |= wxVSCROLL | wxHSCROLL;
#endif
    if (!wxControl::Create (parent, id, pos, size,
                            style | wxWANTS_CHARS | wxCLIP_CHILDREN,
                            wxDefaultValidator, name)) {
        wxSafeShowMessage(wxT("wxScintilla"),wxT("Could not create a new wxControl instance."));
        return false;
    }

#ifdef LINK_LEXERS
    Scintilla_LinkLexers();
#endif
    m_swx = new ScintillaWX(this);
    if (!m_swx)
        wxSafeShowMessage(wxT("wxScintilla"),wxT("Could not create a new ScintillaWX instance."));
    m_stopWatch.Start();
    m_lastKeyDownConsumed = false;
    // we need this here, because wxEvent::GetTimestamp() returns negative values on some systems
    // and therefore blocks mousewheel events. Big thanks to DrewBoo
    m_lastWheelTimestamp = std::numeric_limits<long>::min();
    m_vScrollBar = NULL;
    m_hScrollBar = NULL;
#if wxUSE_UNICODE
    // Put Scintilla into unicode (UTF-8) mode
    SetCodePage(wxSCI_CP_UTF8);
#endif

    #if wxVERSION_NUMBER >= 2800
        SetInitialSize(size);
    #else
        SetBestFittingSize(size);
    #endif

    // Reduces flicker on GTK+/X11
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    // FIXME: back-port from wx29 (svn) to wx289
//    // Make sure it can take the focus
//    SetCanFocus(true);

    return true;
}


wxScintilla::~wxScintilla()
{
    delete m_swx;
    m_swx = 0;
}


//----------------------------------------------------------------------
// Send message to Scintilla
wxIntPtr wxScintilla::SendMsg (int msg, wxUIntPtr wp, wxIntPtr lp) const
{
    return m_swx->WndProc(msg, wp, lp);
}

//----------------------------------------------------------------------

// Set the vertical scrollbar to use instead of the ont that's built-in.
void wxScintilla::SetVScrollBar (wxScrollBar* bar)
{
    m_vScrollBar = bar;
    if (bar != NULL) {
        // ensure that the built-in scrollbar is not visible
        SetScrollbar(wxVERTICAL, 0, 0, 0);
    }
}


// Set the horizontal scrollbar to use instead of the ont that's built-in.
void wxScintilla::SetHScrollBar (wxScrollBar* bar)
{
    m_hScrollBar = bar;
    if (bar != NULL) {
        // ensure that the built-in scrollbar is not visible
        SetScrollbar(wxHORIZONTAL, 0, 0, 0);
    }
}

//----------------------------------------------------------------------
// BEGIN generated section.  The following code is automatically generated
//       by gen_iface.py from the contents of Scintilla.iface.  Do not edit
//       this file.  Edit wxscintilla.cpp.in or gen_iface.py instead and regenerate.

// Add text to the document at current position.
void wxScintilla::AddText (const wxString& text)
{
    wxWX2MBbuf buf = (wxWX2MBbuf)wx2sci(text);
    SendMsg(SCI_ADDTEXT, strlen(buf), (sptr_t)(const char*)buf);
}

// Add text to the document w/length parameter, this allows for binary data to be added.
void wxScintilla::AddText (const int length, const wxString& text)
{
    wxWX2MBbuf buf = (wxWX2MBbuf)wx2sci(text);
    SendMsg(SCI_ADDTEXT, length, (sptr_t)(const char*)buf);
}

// Add array of cells to document.
void wxScintilla::AddStyledText (const wxMemoryBuffer& data)
{
    SendMsg(SCI_ADDSTYLEDTEXT, data.GetDataLen(), (sptr_t)data.GetData());
}

// Insert string at a position.
void wxScintilla::InsertText (int pos, const wxString& text)
{
    SendMsg(SCI_INSERTTEXT, pos, (sptr_t)(const char*)wx2sci(text));
}

// Delete all text in the document.
void wxScintilla::ClearAll()
{
    SendMsg(SCI_CLEARALL, 0, 0);
}

// Set all style bytes to 0, remove all folding information.
void wxScintilla::ClearDocumentStyle()
{
    SendMsg(SCI_CLEARDOCUMENTSTYLE, 0, 0);
}

// Returns the number of bytes in the document.
int wxScintilla::GetLength() const
{
    return SendMsg(SCI_GETLENGTH, 0, 0);
}

// Returns the character byte at the position.
int wxScintilla::GetCharAt (int pos) const
{
    return (unsigned char)SendMsg(SCI_GETCHARAT, pos, 0);
}

// Returns the position of the caret.
int wxScintilla::GetCurrentPos() const
{
    return SendMsg(SCI_GETCURRENTPOS, 0, 0);
}

// Returns the position of the opposite end of the selection to the caret.
int wxScintilla::GetAnchor() const
{
    return SendMsg(SCI_GETANCHOR, 0, 0);
}

// Returns the style byte at the position.
int wxScintilla::GetStyleAt (int pos) const
{
    return (unsigned char)SendMsg(SCI_GETSTYLEAT, pos, 0);
}

// Redoes the next action on the undo history.
void wxScintilla::Redo()
{
    SendMsg(SCI_REDO, 0, 0);
}

// Choose between collecting actions into the undo
// history and discarding them.
void wxScintilla::SetUndoCollection (bool collectUndo)
{
    SendMsg(SCI_SETUNDOCOLLECTION, collectUndo, 0);
}

// Choose between collecting actions into the changes
// history and discarding them.
void wxScintilla::SetChangeCollection (bool collectChange)
{
    SendMsg(SCI_SETCHANGECOLLECTION, collectChange, 0);
}

    // Find a changed line, if fromLine > toLine search is performed backwards.
int wxScintilla::FindChangedLine (const int fromLine, const int toLine) const
{
    return SendMsg(SCI_GETCHANGEDLINE, fromLine, toLine);
}

// Select all the text in the document.
void wxScintilla::SelectAll()
{
    SendMsg(SCI_SELECTALL, 0, 0);
}

// Remember the current position in the undo history as the position
// at which the document was saved.
void wxScintilla::SetSavePoint()
{
    SendMsg(SCI_SETSAVEPOINT, 0, 0);
}

// Retrieve a buffer of cells.
wxMemoryBuffer wxScintilla::GetStyledText (int startPos, int endPos)
{
        wxMemoryBuffer buf;
        if (endPos < startPos) {
            int temp = startPos;
            startPos = endPos;
            endPos = temp;
        }
        int len = endPos - startPos;
        if (!len) return buf;
        TextRange tr;
        tr.lpstrText = (char*)buf.GetWriteBuf(len*2+1);
        tr.chrg.cpMin = startPos;
        tr.chrg.cpMax = endPos;
        len = SendMsg(SCI_GETSTYLEDTEXT, 0, (sptr_t)&tr);
        buf.UngetWriteBuf(len);
        return buf;
}

// Are there any redoable actions in the undo history?
bool wxScintilla::CanRedo() const
{
    return SendMsg(SCI_CANREDO, 0, 0) != 0;
}

// Retrieve the line number at which a particular marker is located.
int wxScintilla::MarkerLineFromHandle (int handle)
{
    return SendMsg(SCI_MARKERLINEFROMHANDLE, handle, 0);
}

// Delete a marker.
void wxScintilla::MarkerDeleteHandle (int handle)
{
    SendMsg(SCI_MARKERDELETEHANDLE, handle, 0);
}

// Is undo history being collected?
bool wxScintilla::GetUndoCollection() const
{
    return SendMsg(SCI_GETUNDOCOLLECTION, 0, 0) != 0;
}

// Are white space characters currently visible?
// Returns one of SCWS_* constants.
int wxScintilla::GetViewWhiteSpace() const
{
    return SendMsg(SCI_GETVIEWWS, 0, 0);
}

// Make white space characters invisible, always visible or visible outside indentation.
void wxScintilla::SetViewWhiteSpace (int viewWS)
{
    SendMsg(SCI_SETVIEWWS, viewWS, 0);
}

// Find the position from a point within the window.
int wxScintilla::PositionFromPoint (wxPoint pt) const
{
    return SendMsg(SCI_POSITIONFROMPOINT, pt.x, pt.y);
}

// Find the position from a point within the window but return
// INVALID_POSITION if not close to text.
int wxScintilla::PositionFromPointClose (int x, int y)
{
    return SendMsg(SCI_POSITIONFROMPOINTCLOSE, x, y);
}

// Set caret to start of a line and ensure it is visible.
void wxScintilla::GotoLine (int line)
{
    SendMsg(SCI_GOTOLINE, line, 0);
}

// Set caret to a position and ensure it is visible.
void wxScintilla::GotoPos (int pos)
{
    SendMsg(SCI_GOTOPOS, pos, 0);
}

// Set the selection anchor to a position. The anchor is the opposite
// end of the selection from the caret.
void wxScintilla::SetAnchor (int posAnchor)
{
    SendMsg(SCI_SETANCHOR, posAnchor, 0);
}

// Retrieve the text of the line containing the caret.
// Returns the index of the caret on the line.
wxString wxScintilla::GetCurLine(int* linePos)
{
        int len = LineLength(GetCurrentLine());
        if (!len) {
            if (linePos)  *linePos = 0;
            return wxEmptyString;
        }

        wxMemoryBuffer mbuf(len+1);
        char* buf = (char*)mbuf.GetWriteBuf(len+1);

        int pos = SendMsg(SCI_GETCURLINE, len+1, (sptr_t)buf);
        mbuf.UngetWriteBuf(len);
        mbuf.AppendByte(0);
        if (linePos)  *linePos = pos;
        return sci2wx(buf);
}

// Retrieve the position of the last correctly styled character.
int wxScintilla::GetEndStyled() const
{
    return SendMsg(SCI_GETENDSTYLED, 0, 0);
}

// Convert all line endings in the document to one mode.
void wxScintilla::ConvertEOLs (int eolMode)
{
    SendMsg(SCI_CONVERTEOLS, eolMode, 0);
}

// Retrieve the current end of line mode - one of CRLF, CR, or LF.
int wxScintilla::GetEOLMode() const
{
    return SendMsg(SCI_GETEOLMODE, 0, 0);
}

// Set the current end of line mode.
void wxScintilla::SetEOLMode (int eolMode)
{
    SendMsg(SCI_SETEOLMODE, eolMode, 0);
}

// Set the current styling position to pos and the styling mask to mask.
// The styling mask can be used to protect some bits in each styling byte from modification.
void wxScintilla::StartStyling (int pos, int mask)
{
    SendMsg(SCI_STARTSTYLING, pos, mask);
}

// Change style from current styling position for length characters to a style
// and move the current styling position to after this newly styled segment.
void wxScintilla::SetStyling (int length, int style)
{
    SendMsg(SCI_SETSTYLING, length, style);
}

// Is drawing done first into a buffer or direct to the screen?
bool wxScintilla::GetBufferedDraw() const
{
    return SendMsg(SCI_GETBUFFEREDDRAW, 0, 0) != 0;
}

// If drawing is buffered then each line of text is drawn into a bitmap buffer
// before drawing it to the screen to avoid flicker.
void wxScintilla::SetBufferedDraw (bool buffered)
{
    SendMsg(SCI_SETBUFFEREDDRAW, buffered, 0);
}

// Change the visible size of a tab to be a multiple of the width of a space character.
void wxScintilla::SetTabWidth (int tabWidth)
{
    SendMsg(SCI_SETTABWIDTH, tabWidth, 0);
}

// Retrieve the visible size of a tab.
int wxScintilla::GetTabWidth() const
{
    return SendMsg(SCI_GETTABWIDTH, 0, 0);
}

// Set the code page used to interpret the bytes of the document as characters.
void wxScintilla::SetCodePage (int codePage)
{
#if wxUSE_UNICODE
    wxASSERT_MSG (codePage == wxSCI_CP_UTF8,
                  wxT("Only wxSCI_CP_UTF8 may be used when wxUSE_UNICODE is on."));
#else
    wxASSERT_MSG (codePage != wxSCI_CP_UTF8,
                  wxT("wxSCI_CP_UTF8 may not be used when wxUSE_UNICODE is off."));
#endif
    SendMsg(SCI_SETCODEPAGE, codePage);
}

// Set use palette (SCI_SETUSEPALETTE) not supported

// Set the symbol used for a particular marker number,
// and optionally the fore and background colours.
void wxScintilla::MarkerDefine (int markerNumber, int markerSymbol,
                const wxColour& foreground,
                const wxColour& background)
{
    SendMsg(SCI_MARKERDEFINE, markerNumber, markerSymbol);
    if (foreground.Ok())
        MarkerSetForeground(markerNumber, foreground);
    if (background.Ok())
        MarkerSetBackground(markerNumber, background);
}

// Set the foreground colour used for a particular marker number.
void wxScintilla::MarkerSetForeground (int markerNumber, const wxColour& fore)
{
    SendMsg(SCI_MARKERSETFORE, markerNumber, wxColourAsLong(fore));
}

// Set the background colour used for a particular marker number.
void wxScintilla::MarkerSetBackground (int markerNumber, const wxColour& back)
{
    SendMsg(SCI_MARKERSETBACK, markerNumber, wxColourAsLong(back));
}

// Add a marker to a line, returning an ID which can be used to find or delete the marker.
int wxScintilla::MarkerAdd (int line, int markerNumber)
{
    return SendMsg(SCI_MARKERADD, line, markerNumber);
}

// Delete a marker from a line.
void wxScintilla::MarkerDelete (int line, int markerNumber)
{
    SendMsg(SCI_MARKERDELETE, line, markerNumber);
}

// Delete all markers with a particular number from all lines.
void wxScintilla::MarkerDeleteAll (int markerNumber)
{
    SendMsg(SCI_MARKERDELETEALL, markerNumber, 0);
}

// Get a bit mask of all the markers set on a line.
int wxScintilla::MarkerGet (int line)
{
    return SendMsg(SCI_MARKERGET, line, 0);
}

// Find the next line after lineStart that includes a marker in mask.
int wxScintilla::MarkerNext (int lineStart, int markerMask)
{
    return SendMsg(SCI_MARKERNEXT, lineStart, markerMask);
}

// Find the previous line before lineStart that includes a marker in mask.
int wxScintilla::MarkerPrevious (int lineStart, int markerMask)
{
    return SendMsg(SCI_MARKERPREVIOUS, lineStart, markerMask);
}

// Define a marker from a bitmap
void wxScintilla::MarkerDefineBitmap (int markerNumber, const wxBitmap& bmp)
{
        // convert bmp to a xpm in a string
        wxMemoryOutputStream strm;
        wxImage img = bmp.ConvertToImage();
        if (img.HasAlpha())
            img.ConvertAlphaToMask();
        img.SaveFile(strm, wxBITMAP_TYPE_XPM);
        size_t len = strm.GetSize();
        char* buff = new char[len+1];
        strm.CopyTo(buff, len);
        buff[len] = 0;
        SendMsg(SCI_MARKERDEFINEPIXMAP, markerNumber, (sptr_t)buff);
        delete [] buff;

}

// Add a set of markers to a line.
void wxScintilla::MarkerAddSet (int line, int set)
{
    SendMsg(SCI_MARKERADDSET, line, set);
}

// Set the alpha used for a marker that is drawn in the text area, not the margin.
void wxScintilla::MarkerSetAlpha (int markerNumber, int alpha)
{
    SendMsg(SCI_MARKERSETALPHA, markerNumber, alpha);
}

// Set a margin to be either numeric or symbolic.
void wxScintilla::SetMarginType (int margin, int marginType)
{
    SendMsg(SCI_SETMARGINTYPEN, margin, marginType);
}

// Retrieve the type of a margin.
int wxScintilla::GetMarginType (int margin) const
{
    return SendMsg(SCI_GETMARGINTYPEN, margin, 0);
}

// Set the width of a margin to a width expressed in pixels.
void wxScintilla::SetMarginWidth (int margin, int pixelWidth)
{
    SendMsg(SCI_SETMARGINWIDTHN, margin, pixelWidth);
}

// Retrieve the width of a margin in pixels.
int wxScintilla::GetMarginWidth (int margin) const
{
    return SendMsg(SCI_GETMARGINWIDTHN, margin, 0);
}

// Set a mask that determines which markers are displayed in a margin.
void wxScintilla::SetMarginMask (int margin, int mask)
{
    SendMsg(SCI_SETMARGINMASKN, margin, mask);
}

// Retrieve the marker mask of a margin.
int wxScintilla::GetMarginMask (int margin) const
{
    return SendMsg(SCI_GETMARGINMASKN, margin, 0);
}

// Make a margin sensitive or insensitive to mouse clicks.
void wxScintilla::SetMarginSensitive (int margin, bool sensitive)
{
    SendMsg(SCI_SETMARGINSENSITIVEN, margin, sensitive);
}

// Retrieve the mouse click sensitivity of a margin.
bool wxScintilla::GetMarginSensitive (int margin) const
{
    return SendMsg(SCI_GETMARGINSENSITIVEN, margin, 0) != 0;
}

// Clear all the styles and make equivalent to the global default style.
void wxScintilla::StyleClearAll()
{
    SendMsg(SCI_STYLECLEARALL, 0, 0);
}

// Set the foreground colour of a style.
void wxScintilla::StyleSetForeground (int style, const wxColour& fore)
{
    SendMsg(SCI_STYLESETFORE, style, wxColourAsLong(fore));
}

// Set the background colour of a style.
void wxScintilla::StyleSetBackground (int style, const wxColour& back)
{
    SendMsg(SCI_STYLESETBACK, style, wxColourAsLong(back));
}

// Set a style to be bold or not.
void wxScintilla::StyleSetBold (int style, bool bold)
{
    SendMsg(SCI_STYLESETBOLD, style, bold);
}

// Set a style to be italic or not.
void wxScintilla::StyleSetItalic (int style, bool italic)
{
    SendMsg(SCI_STYLESETITALIC, style, italic);
}

// Set the size of characters of a style.
void wxScintilla::StyleSetSize (int style, int sizePoints)
{
    SendMsg(SCI_STYLESETSIZE, style, sizePoints);
}

// Set the font of a style.
void wxScintilla::StyleSetFaceName (int style, const wxString& fontName)
{
    SendMsg(SCI_STYLESETFONT, style, (sptr_t)(const char*)wx2sci(fontName));
}

// Set a style to have its end of line filled or not.
void wxScintilla::StyleSetEOLFilled (int style, bool filled)
{
    SendMsg(SCI_STYLESETEOLFILLED, style, filled);
}

// Reset the default style to its state at startup
void wxScintilla::StyleResetDefault()
{
    SendMsg(SCI_STYLERESETDEFAULT, 0, 0);
}

// Set a style to be underlined or not.
void wxScintilla::StyleSetUnderline (int style, bool underline)
{
    SendMsg(SCI_STYLESETUNDERLINE, style, underline);
}

// Get the foreground colour of a style.
wxColour wxScintilla::StyleGetForeground(int style) const
{
    long c = SendMsg(SCI_STYLEGETFORE, style, 0);
    return wxColourFromLong(c);
}

// Get the background colour of a style.
wxColour wxScintilla::StyleGetBackground(int style) const
{
    long c = SendMsg(SCI_STYLEGETBACK, style, 0);
    return wxColourFromLong(c);
}

// Get is a style bold or not.
bool wxScintilla::StyleGetBold(int style) const
{
    return SendMsg(SCI_STYLEGETBOLD, style, 0) != 0;
}

// Get is a style italic or not.
bool wxScintilla::StyleGetItalic(int style) const
{
    return SendMsg(SCI_STYLEGETITALIC, style, 0) != 0;
}

// Get the size of characters of a style.
int wxScintilla::StyleGetSize(int style) const
{
    return SendMsg(SCI_STYLEGETSIZE, style, 0);
}

// Get the font facename of a style
wxString wxScintilla::StyleGetFaceName(int style)
{
     long msg = SCI_STYLEGETFONT;
     long len = SendMsg(msg, style, 0);
     wxMemoryBuffer mbuf(len+1);
     char* buf = (char*)mbuf.GetWriteBuf(len+1);
     SendMsg(msg, style, (sptr_t)buf);
     mbuf.UngetWriteBuf(len);
     mbuf.AppendByte(0);
     return sci2wx(buf);
}

// Get is a style to have its end of line filled or not.
bool wxScintilla::StyleGetEOLFilled(int style) const
{
    return SendMsg(SCI_STYLEGETEOLFILLED, style, 0) != 0;
}

// Get is a style underlined or not.
bool wxScintilla::StyleGetUnderline(int style) const
{
    return SendMsg(SCI_STYLEGETUNDERLINE, style, 0) != 0;
}

// Get is a style mixed case, or to force upper or lower case.
int wxScintilla::StyleGetCase(int style) const
{
    return SendMsg(SCI_STYLEGETCASE, style, 0);
}

// Get the character get of the font in a style.
int wxScintilla::StyleGetCharacterSet(int style) const
{
    return SendMsg(SCI_STYLEGETCHARACTERSET, style, 0);
}

// Get is a style visible or not.
bool wxScintilla::StyleGetVisible(int style) const
{
    return SendMsg(SCI_STYLEGETVISIBLE, style, 0) != 0;
}

// Get is a style changeable or not (read only).
// Experimental feature, currently buggy.
bool wxScintilla::StyleGetChangeable(int style) const
{
    return SendMsg(SCI_STYLEGETCHANGEABLE, style, 0) != 0;
}

// Get is a style a hotspot or not.
bool wxScintilla::StyleGetHotSpot(int style) const
{
    return SendMsg(SCI_STYLEGETHOTSPOT, style, 0) != 0;
}

// Set a style to be mixed case, or to force upper or lower case.
void wxScintilla::StyleSetCase (int style, int caseForce)
{
    SendMsg(SCI_STYLESETCASE, style, caseForce);
}

// Set a style to be a hotspot or not.
void wxScintilla::StyleSetHotSpot (int style, bool hotspot)
{
    SendMsg(SCI_STYLESETHOTSPOT, style, hotspot);
}

// Set the foreground colour of the main and additional selections and whether to use this setting.
void wxScintilla::SetSelForeground(bool useSetting, const wxColour& fore)
{
    SendMsg(SCI_SETSELFORE, useSetting, wxColourAsLong(fore));
}

// Set the background colour of the main and additional selections and whether to use this setting.
void wxScintilla::SetSelBackground(bool useSetting, const wxColour& back)
{
    SendMsg(SCI_SETSELBACK, useSetting, wxColourAsLong(back));
}

// Get the alpha of the selection.
int wxScintilla::GetSelAlpha () const
{
    return SendMsg(SCI_GETSELALPHA, 0, 0);
}

// Set the alpha of the selection.
void wxScintilla::SetSelAlpha (int alpha)
{
    SendMsg(SCI_SETSELALPHA, alpha, 0);
}

// Is the selection end of line filled?
bool wxScintilla::GetSelEOLFilled() const
{
    return SendMsg(SCI_GETSELEOLFILLED, 0, 0) != 0;
}

// Set the selection to have its end of line filled or not.
void wxScintilla::SetSelEOLFilled(bool filled)
{
    SendMsg(SCI_SETSELEOLFILLED, filled, 0);
}

// Set the foreground colour of the caret.
void wxScintilla::SetCaretForeground (const wxColour& fore)
{
    SendMsg(SCI_SETCARETFORE, wxColourAsLong(fore), 0);
}

// When key+modifier combination km is pressed perform msg.
void wxScintilla::CmdKeyAssign (int key, int modifiers, int cmd)
{
    SendMsg(SCI_ASSIGNCMDKEY, MAKELONG(key, modifiers), cmd);
}

// When key+modifier combination km is pressed do nothing.
void wxScintilla::CmdKeyClear (int key, int modifiers)
{
    SendMsg(SCI_CLEARCMDKEY, MAKELONG(key, modifiers));
}

// Drop all key mappings.
void wxScintilla::CmdKeyClearAll()
{
    SendMsg(SCI_CLEARALLCMDKEYS, 0, 0);
}

// Set the styles for a segment of the document.
void wxScintilla::SetStyleBytes (int length, char* styleBytes)
{
    SendMsg(SCI_SETSTYLINGEX, length, (sptr_t)styleBytes);
}

// Set a style to be visible or not.
void wxScintilla::StyleSetVisible (int style, bool visible)
{
    SendMsg(SCI_STYLESETVISIBLE, style, visible);
}

// Get the time in milliseconds that the caret is on and off.
int wxScintilla::GetCaretPeriod() const
{
    return SendMsg(SCI_GETCARETPERIOD, 0, 0);
}

// Get the time in milliseconds that the caret is on and off. 0 = steady on.
void wxScintilla::SetCaretPeriod (int periodMilliseconds)
{
    SendMsg(SCI_SETCARETPERIOD, periodMilliseconds, 0);
}

// Set the set of characters making up words for when moving or selecting by word.
// First sets defaults like SetCharsDefault.
void wxScintilla::SetWordChars (const wxString& characters)
{
    SendMsg(SCI_SETWORDCHARS, 0, (sptr_t)(const char*)wx2sci(characters));
}

// Start a sequence of actions that is undone and redone as a unit.
// May be nested.
void wxScintilla::BeginUndoAction()
{
    SendMsg(SCI_BEGINUNDOACTION, 0, 0);
}

// End a sequence of actions that is undone and redone as a unit.
void wxScintilla::EndUndoAction()
{
    SendMsg(SCI_ENDUNDOACTION, 0, 0);
}

// Set an indicator to plain, squiggle or TT.
void wxScintilla::IndicatorSetStyle (int indic, int style)
{
    SendMsg(SCI_INDICSETSTYLE, indic, style);
}

// Retrieve the style of an indicator.
int wxScintilla::IndicatorGetStyle (int indic) const
{
    return SendMsg(SCI_INDICGETSTYLE, indic, 0);
}

// Set the foreground colour of an indicator.
void wxScintilla::IndicatorSetForeground (int indic, const wxColour& fore)
{
    SendMsg(SCI_INDICSETFORE, indic, wxColourAsLong(fore));
}

// Retrieve the foreground colour of an indicator.
wxColour wxScintilla::IndicatorGetForeground (int indic) const
{
    long c = SendMsg(SCI_INDICGETFORE, indic, 0);
    return wxColourFromLong(c);
}

// Set an indicator to draw under text or over(default).
void wxScintilla::IndicatorSetUnder(int indic, bool under)
{
    SendMsg(SCI_INDICSETUNDER, indic, under);
}

// Retrieve whether indicator drawn under or over text.
bool wxScintilla::IndicatorGetUnder(int indic) const
{
    return SendMsg(SCI_INDICGETUNDER, indic, 0) != 0;
}

// Set the foreground colour of all whitespace and whether to use this setting.
void wxScintilla::SetWhitespaceForeground (bool useSetting, const wxColour& fore)
{
    SendMsg(SCI_SETWHITESPACEFORE, useSetting, wxColourAsLong(fore));
}

// Set the background colour of all whitespace and whether to use this setting.
void wxScintilla::SetWhitespaceBackground (bool useSetting, const wxColour& back)
{
    SendMsg(SCI_SETWHITESPACEBACK, useSetting, wxColourAsLong(back));
}

// Set the size of the dots used to mark space characters.
void wxScintilla::SetWhitespaceSize(int size)
{
    SendMsg(SCI_SETWHITESPACESIZE, size, 0);
}

// Get the size of the dots used to mark space characters.
int wxScintilla::GetWhitespaceSize() const
{
    return SendMsg(SCI_GETWHITESPACESIZE, 0, 0);
}

// Divide each styling byte into lexical class bits (default: 5) and indicator
// bits (default: 3). If a lexer requires more than 32 lexical states, then this
// is used to expand the possible states.
void wxScintilla::SetStyleBits (int bits)
{
    SendMsg(SCI_SETSTYLEBITS, bits, 0);
}

// Retrieve number of bits in style bytes used to hold the lexical state.
int wxScintilla::GetStyleBits() const
{
    return SendMsg(SCI_GETSTYLEBITS, 0, 0);
}

// Used to hold extra styling information for each line.
void wxScintilla::SetLineState (int line, int state)
{
    SendMsg(SCI_SETLINESTATE, line, state);
}

// Retrieve the extra styling information for a line.
int wxScintilla::GetLineState (int line) const
{
    return SendMsg(SCI_GETLINESTATE, line, 0);
}

// Retrieve the last line number that has line state.
int wxScintilla::GetMaxLineState() const
{
    return SendMsg(SCI_GETMAXLINESTATE, 0, 0);
}

// Is the background of the line containing the caret in a different colour?
bool wxScintilla::GetCaretLineVisible() const
{
    return SendMsg(SCI_GETCARETLINEVISIBLE, 0, 0) != 0;
}

// Display the background of the line containing the caret in a different colour.
void wxScintilla::SetCaretLineVisible (bool show)
{
    SendMsg(SCI_SETCARETLINEVISIBLE, show, 0);
}

// Get the colour of the background of the line containing the caret.
wxColour wxScintilla::GetCaretLineBackground() const
{
    long c = SendMsg(SCI_GETCARETLINEBACK, 0, 0);
    return wxColourFromLong(c);
}

// Set the colour of the background of the line containing the caret.
void wxScintilla::SetCaretLineBackground (const wxColour& back)
{
    SendMsg(SCI_SETCARETLINEBACK, wxColourAsLong(back), 0);
}

// Set a style to be changeable or not (read only).
// Experimental feature, currently buggy.
void wxScintilla::StyleSetChangeable (int style, bool changeable)
{
    SendMsg(SCI_STYLESETCHANGEABLE, style, changeable);
}

// Display a auto-completion list.
// The lenEntered parameter indicates how many characters before
// the caret should be used to provide context.
void wxScintilla::AutoCompShow (int lenEntered, const wxString& itemList)
{
    SendMsg(SCI_AUTOCSHOW, lenEntered, (sptr_t)(const char*)wx2sci(itemList));
}

// Remove the auto-completion list from the screen.
void wxScintilla::AutoCompCancel()
{
    SendMsg(SCI_AUTOCCANCEL, 0, 0);
}

// Is there an auto-completion list visible?
bool wxScintilla::AutoCompActive()
{
    return SendMsg(SCI_AUTOCACTIVE, 0, 0) != 0;
}

// Retrieve the position of the caret when the auto-completion list was displayed.
int wxScintilla::AutoCompPosStart()
{
    return SendMsg(SCI_AUTOCPOSSTART, 0, 0);
}

// User has selected an item so remove the list and insert the selection.
void wxScintilla::AutoCompComplete()
{
    SendMsg(SCI_AUTOCCOMPLETE, 0, 0);
}

// Define a set of character that when typed cancel the auto-completion list.
void wxScintilla::AutoCompStops (const wxString& characterSet)
{
    SendMsg(SCI_AUTOCSTOPS, 0, (long)(const char*)wx2sci(characterSet));
}

// Change the separator character in the string setting up an auto-completion list.
// Default is space but can be changed if items contain space.
void wxScintilla::AutoCompSetSeparator (int separatorCharacter)
{
    SendMsg(SCI_AUTOCSETSEPARATOR, separatorCharacter, 0);
}

// Retrieve the auto-completion list separator character.
int wxScintilla::AutoCompGetSeparator() const
{
    return SendMsg(SCI_AUTOCGETSEPARATOR, 0, 0);
}

// Select the item in the auto-completion list that starts with a string.
void wxScintilla::AutoCompSelect (const wxString& text)
{
    SendMsg(SCI_AUTOCSELECT, 0, (sptr_t)(const char*)wx2sci(text));
}

// Should the auto-completion list be cancelled if the user backspaces to a
// position before where the box was created.
void wxScintilla::AutoCompSetCancelAtStart (bool cancel)
{
    SendMsg(SCI_AUTOCSETCANCELATSTART, cancel, 0);
}

// Retrieve whether auto-completion cancelled by backspacing before start.
bool wxScintilla::AutoCompGetCancelAtStart() const
{
    return SendMsg(SCI_AUTOCGETCANCELATSTART, 0, 0) != 0;
}

// Define a set of characters that when typed will cause the autocompletion to
// choose the selected item.
void wxScintilla::AutoCompSetFillUps (const wxString& characterSet)
{
    SendMsg(SCI_AUTOCSETFILLUPS, 0, (sptr_t)(const char*)wx2sci(characterSet));
}

// Should a single item auto-completion list automatically choose the item.
void wxScintilla::AutoCompSetChooseSingle (bool chooseSingle)
{
    SendMsg(SCI_AUTOCSETCHOOSESINGLE, chooseSingle, 0);
}

// Retrieve whether a single item auto-completion list automatically choose the item.
bool wxScintilla::AutoCompGetChooseSingle() const
{
    return SendMsg(SCI_AUTOCGETCHOOSESINGLE, 0, 0) != 0;
}

// Set whether case is significant when performing auto-completion searches.
void wxScintilla::AutoCompSetIgnoreCase (bool ignoreCase)
{
    SendMsg(SCI_AUTOCSETIGNORECASE, ignoreCase, 0);
}

// Retrieve state of ignore case flag.
bool wxScintilla::AutoCompGetIgnoreCase() const
{
    return SendMsg(SCI_AUTOCGETIGNORECASE, 0, 0) != 0;
}

// Display a list of strings and send notification when user chooses one.
void wxScintilla::UserListShow (int listType, const wxString& itemList)
{
    SendMsg(SCI_USERLISTSHOW, listType, (sptr_t)(const char*)wx2sci(itemList));
}

// Set whether or not autocompletion is hidden automatically when nothing matches.
void wxScintilla::AutoCompSetAutoHide (bool autoHide)
{
    SendMsg(SCI_AUTOCSETAUTOHIDE, autoHide, 0);
}

// Retrieve whether or not autocompletion is hidden automatically when nothing matches.
bool wxScintilla::AutoCompGetAutoHide() const
{
    return SendMsg(SCI_AUTOCGETAUTOHIDE, 0, 0) != 0;
}

// Set whether or not autocompletion deletes any word characters
// after the inserted text upon completion.
void wxScintilla::AutoCompSetDropRestOfWord (bool dropRestOfWord)
{
    SendMsg(SCI_AUTOCSETDROPRESTOFWORD, dropRestOfWord, 0);
}

// Retrieve whether or not autocompletion deletes any word characters
// after the inserted text upon completion.
bool wxScintilla::AutoCompGetDropRestOfWord() const
{
    return SendMsg(SCI_AUTOCGETDROPRESTOFWORD, 0, 0) != 0;
}

// Register an image for use in autocompletion lists.
void wxScintilla::RegisterImage (int type, const wxBitmap& bmp)
{
        // convert bmp to a xpm in a string
        wxMemoryOutputStream strm;
        wxImage img = bmp.ConvertToImage();
        if (img.HasAlpha())
            img.ConvertAlphaToMask();
        img.SaveFile(strm, wxBITMAP_TYPE_XPM);
        size_t len = strm.GetSize();
        char* buff = new char[len+1];
        strm.CopyTo(buff, len);
        buff[len] = 0;
        SendMsg(SCI_REGISTERIMAGE, type, (sptr_t)buff);
        delete [] buff;

}

// Clear all the registered images.
void wxScintilla::ClearRegisteredImages()
{
    SendMsg(SCI_CLEARREGISTEREDIMAGES, 0, 0);
}

// Retrieve the auto-completion list type-separator character.
int wxScintilla::AutoCompGetTypeSeparator() const
{
    return SendMsg(SCI_AUTOCGETTYPESEPARATOR, 0, 0);
}

// Change the type-separator character in the string setting up an auto-completion list.
// Default is '?' but can be changed if items contain '?'.
void wxScintilla::AutoCompSetTypeSeparator (int separatorCharacter)
{
    SendMsg(SCI_AUTOCSETTYPESEPARATOR, separatorCharacter, 0);
}

// Set the maximum width, in characters, of auto-completion and user lists.
// Set to 0 to autosize to fit longest item, which is the default.
void wxScintilla::AutoCompSetMaxWidth (int characterCount)
{
    SendMsg(SCI_AUTOCSETMAXWIDTH, characterCount, 0);
}

// Get the maximum width, in characters, of auto-completion and user lists.
int wxScintilla::AutoCompGetMaxWidth() const
{
    return SendMsg(SCI_AUTOCGETMAXWIDTH, 0, 0);
}

// Set the maximum height, in rows, of auto-completion and user lists.
// The default is 5 rows.
void wxScintilla::AutoCompSetMaxHeight (int rowCount)
{
    SendMsg(SCI_AUTOCSETMAXHEIGHT, rowCount, 0);
}

// Set the maximum height, in rows, of auto-completion and user lists.
int wxScintilla::AutoCompGetMaxHeight() const
{
    return SendMsg(SCI_AUTOCGETMAXHEIGHT, 0, 0);
}

// Set the number of spaces used for one level of indentation.
void wxScintilla::SetIndent (int indentSize)
{
    SendMsg(SCI_SETINDENT, indentSize, 0);
}

// Retrieve indentation size.
int wxScintilla::GetIndent() const
{
    return SendMsg(SCI_GETINDENT, 0, 0);
}

// Indentation will only use space characters if useTabs is false, otherwise
// it will use a combination of tabs and spaces.
void wxScintilla::SetUseTabs (bool useTabs)
{
    SendMsg(SCI_SETUSETABS, useTabs, 0);
}

// Retrieve whether tabs will be used in indentation.
bool wxScintilla::GetUseTabs() const
{
    return SendMsg(SCI_GETUSETABS, 0, 0) != 0;
}

// Change the indentation of a line to a number of columns.
void wxScintilla::SetLineIndentation (int line, int indentSize)
{
    SendMsg(SCI_SETLINEINDENTATION, line, indentSize);
}

// Retrieve the number of columns that a line is indented.
int wxScintilla::GetLineIndentation (int line) const
{
    return SendMsg(SCI_GETLINEINDENTATION, line, 0);
}

// Retrieve the position before the first non indentation character on a line.
int wxScintilla::GetLineIndentPosition (int line) const
{
    return SendMsg(SCI_GETLINEINDENTPOSITION, line, 0);
}

// Retrieve the column number of a position, taking tab width into account.
int wxScintilla::GetColumn (int pos) const
{
    return SendMsg(SCI_GETCOLUMN, pos, 0);
}

// Show or hide the horizontal scroll bar.
void wxScintilla::SetUseHorizontalScrollBar (bool show)
{
    SendMsg(SCI_SETHSCROLLBAR, show, 0);
}

// Is the horizontal scroll bar visible?
bool wxScintilla::GetUseHorizontalScrollBar() const
{
    return SendMsg(SCI_GETHSCROLLBAR, 0, 0) != 0;
}

// Show or hide indentation guides.
void wxScintilla::SetIndentationGuides (int indentView)
{
    SendMsg(SCI_SETINDENTATIONGUIDES, indentView, 0);
}

// Are the indentation guides visible?
int wxScintilla::GetIndentationGuides() const
{
    return SendMsg(SCI_GETINDENTATIONGUIDES, 0, 0);
}

// Set the highlighted indentation guide column.
// 0 = no highlighted guide.
void wxScintilla::SetHighlightGuide (int column)
{
    SendMsg(SCI_SETHIGHLIGHTGUIDE, column, 0);
}

// Get the highlighted indentation guide column.
int wxScintilla::GetHighlightGuide() const
{
    return SendMsg(SCI_GETHIGHLIGHTGUIDE, 0, 0);
}

// Get the position after the last visible characters on a line.
int wxScintilla::GetLineEndPosition (int line) const
{
    return SendMsg(SCI_GETLINEENDPOSITION, line, 0);
}

// Get the code page used to interpret the bytes of the document as characters.
int wxScintilla::GetCodePage() const
{
    return SendMsg(SCI_GETCODEPAGE, 0, 0);
}

// Get the foreground colour of the caret.
wxColour wxScintilla::GetCaretForeground() const
{
    long c = SendMsg(SCI_GETCARETFORE, 0, 0);
    return wxColourFromLong(c);
}

// Get use palette (SCI_GETUSEPALETTE) not supported

// In read-only mode?
bool wxScintilla::GetReadOnly() const
{
    return SendMsg(SCI_GETREADONLY, 0, 0) != 0;
}

// Sets the position of the caret.
void wxScintilla::SetCurrentPos (int pos)
{
    SendMsg(SCI_SETCURRENTPOS, pos, 0);
}

// Sets the position that starts the selection - this becomes the anchor.
void wxScintilla::SetSelectionStart(int pos)
{
    SendMsg(SCI_SETSELECTIONSTART, pos, 0);
}

// Returns the position at the start of the selection.
int wxScintilla::GetSelectionStart() const
{
    return SendMsg(SCI_GETSELECTIONSTART, 0, 0);
}

// Sets the position that ends the selection - this becomes the currentPosition.
void wxScintilla::SetSelectionEnd(int pos)
{
    SendMsg(SCI_SETSELECTIONEND, pos, 0);
}

// Returns the position at the end of the selection.
int wxScintilla::GetSelectionEnd() const
{
    return SendMsg(SCI_GETSELECTIONEND, 0, 0);
}

// Sets the print magnification added to the point size of each style for printing.
void wxScintilla::SetPrintMagnification (int magnification)
{
    SendMsg(SCI_SETPRINTMAGNIFICATION, magnification, 0);
}

// Returns the print magnification.
int wxScintilla::GetPrintMagnification() const
{
    return SendMsg(SCI_GETPRINTMAGNIFICATION, 0, 0);
}

// Modify colours when printing for clearer printed text.
void wxScintilla::SetPrintColourMode (int mode)
{
    SendMsg(SCI_SETPRINTCOLOURMODE, mode, 0);
}

// Returns the print colour mode.
int wxScintilla::GetPrintColourMode() const
{
    return SendMsg(SCI_GETPRINTCOLOURMODE, 0, 0);
}

// Find some text in the document.
int wxScintilla::FindText (int minPos, int maxPos,
                           const wxString& text,
                           int flags,
                           int* lengthFound)
{
    TextToFind  ft;
    ft.chrg.cpMin = minPos;
    ft.chrg.cpMax = maxPos;
    wxWX2MBbuf buf = (wxWX2MBbuf)wx2sci(text);
    ft.lpstrText = (char*)(const char*)buf;

    int ret = SendMsg(SCI_FINDTEXT, flags, (sptr_t)&ft);
    if (lengthFound)
        *lengthFound = ft.chrgText.cpMax - ft.chrgText.cpMin;

    return ret;
}

// On Windows, will draw the document into a display context such as a printer.
 int wxScintilla::FormatRange (bool doDraw,
                               int startPos,
                               int endPos,
                               wxDC* draw,
                               wxDC* target,
                               wxRect renderRect,
                               wxRect pageRect)
{
    RangeToFormat fr;

    if (endPos < startPos) {
       int temp = startPos;
       startPos = endPos;
       endPos = temp;
    }
    fr.hdc = draw;
    fr.hdcTarget = target;
    fr.rc.top = renderRect.GetTop();
    fr.rc.left = renderRect.GetLeft();
    fr.rc.right = renderRect.GetRight();
    fr.rc.bottom = renderRect.GetBottom();
    fr.rcPage.top = pageRect.GetTop();
    fr.rcPage.left = pageRect.GetLeft();
    fr.rcPage.right = pageRect.GetRight();
    fr.rcPage.bottom = pageRect.GetBottom();
    fr.chrg.cpMin = startPos;
    fr.chrg.cpMax = endPos;

    return SendMsg(SCI_FORMATRANGE, doDraw, (sptr_t)&fr);
}

// Retrieve the display line at the top of the display.
int wxScintilla::GetFirstVisibleLine() const
{
    return SendMsg(SCI_GETFIRSTVISIBLELINE, 0, 0);
}

// Retrieve the contents of a line.
wxString wxScintilla::GetLine (int line) const {
         int len = LineLength(line);
         if (!len) return wxEmptyString;

         wxMemoryBuffer mbuf(len+1);
         char* buf = (char*)mbuf.GetWriteBuf(len+1);
         SendMsg(SCI_GETLINE, line, (sptr_t)buf);
         mbuf.UngetWriteBuf(len);
         mbuf.AppendByte(0);
         return sci2wx(buf);
}

// Returns the number of lines in the document. There is always at least one.
int wxScintilla::GetLineCount() const
{
    return SendMsg(SCI_GETLINECOUNT, 0, 0);
}

// Sets the size in pixels of the left margin.
void wxScintilla::SetMarginLeft (int pixelWidth)
{
    SendMsg(SCI_SETMARGINLEFT, 0, pixelWidth);
}

// Returns the size in pixels of the left margin.
int wxScintilla::GetMarginLeft() const
{
    return SendMsg(SCI_GETMARGINLEFT, 0, 0);
}

// Sets the size in pixels of the right margin.
void wxScintilla::SetMarginRight (int pixelWidth)
{
    SendMsg(SCI_SETMARGINRIGHT, 0, pixelWidth);
}

// Returns the size in pixels of the right margin.
int wxScintilla::GetMarginRight() const
{
    return SendMsg(SCI_GETMARGINRIGHT, 0, 0);
}

// Is the document different from when it was last saved?
bool wxScintilla::GetModify() const
{
    return SendMsg(SCI_GETMODIFY, 0, 0) != 0;
}

// Select a range of text.
void wxScintilla::SetSelectionVoid(int startPos, int endPos)
{
    SendMsg(SCI_SETSEL, startPos, endPos);
}

// Retrieve the selected text.
wxString wxScintilla::GetSelectedText()
{
/* C::B begin */
    /* Notice that this function used to use 'GetSelection(&start, &end);'
     * to calculate the selected text length - which works fine when using
     * simple selection, but when using multiple selection, or even rectangular
     * selection it may cause a crash. */
    int len (0);

    // determine the selected text range
    len = SendMsg(SCI_GETSELTEXT, 0, 0);
/* C::B end */
    if (!len) return wxEmptyString;

    wxMemoryBuffer mbuf(len+2);
    char* buf = (char*)mbuf.GetWriteBuf(len+1);
    SendMsg(SCI_GETSELTEXT, 0, (sptr_t)buf);
    mbuf.UngetWriteBuf(len);
    mbuf.AppendByte(0);
    return sci2wx(buf);
}

// Retrieve a range of text.
wxString wxScintilla::GetTextRange (int startPos, int endPos)
{
    if (endPos < startPos) {
        int temp = startPos;
        startPos = endPos;
        endPos = temp;
    }
    int   len  = endPos - startPos;
    if (!len) return wxEmptyString;
    wxMemoryBuffer mbuf(len+1);
    char* buf = (char*)mbuf.GetWriteBuf(len);
    TextRange tr;
    tr.lpstrText = buf;
    tr.chrg.cpMin = startPos;
    tr.chrg.cpMax = endPos;
    SendMsg(SCI_GETTEXTRANGE, 0, (sptr_t)&tr);
    mbuf.UngetWriteBuf(len);
    mbuf.AppendByte(0);
    return sci2wx(buf);
}

// Draw the selection in normal style or with selection highlighted.
void wxScintilla::HideSelection (bool normal)
{
    SendMsg(SCI_HIDESELECTION, normal, 0);
}

// Retrieve the line containing a position.
int wxScintilla::LineFromPosition (int pos) const
{
    return SendMsg(SCI_LINEFROMPOSITION, pos, 0);
}

// Retrieve the position at the start of a line.
int wxScintilla::PositionFromLine (int line) const
{
    return SendMsg(SCI_POSITIONFROMLINE, line, 0);
}

// Scroll horizontally and vertically.
void wxScintilla::LineScroll (int columns, int lines)
{
    SendMsg(SCI_LINESCROLL, columns, lines);
}

// Ensure the caret is visible.
void wxScintilla::EnsureCaretVisible()
{
    SendMsg(SCI_SCROLLCARET, 0, 0);
}

// Replace the selected text with the argument text.
void wxScintilla::ReplaceSelection (const wxString& text)
{
    SendMsg(SCI_REPLACESEL, 0, (sptr_t)(const char*)wx2sci(text));
}

// Set to read only or read write.
void wxScintilla::SetReadOnly (bool readOnly)
{
    SendMsg(SCI_SETREADONLY, readOnly, 0);
}

// Will a paste succeed?
bool wxScintilla::CanPaste() const
{
    return SendMsg(SCI_CANPASTE, 0, 0) != 0;
}

// Are there any undoable actions in the undo history?
bool wxScintilla::CanUndo() const
{
    return SendMsg(SCI_CANUNDO, 0, 0) != 0;
}

// Delete the undo history.
void wxScintilla::EmptyUndoBuffer(bool collectChangeHistory)
{
    SendMsg(SCI_EMPTYUNDOBUFFER, collectChangeHistory, 0);
}

// Undo one action in the undo history.
void wxScintilla::Undo()
{
    SendMsg(SCI_UNDO, 0, 0);
}

// Cut the selection to the clipboard.
void wxScintilla::Cut()
{
    SendMsg(SCI_CUT, 0, 0);
}

// Copy the selection to the clipboard.
void wxScintilla::Copy()
{
    SendMsg(SCI_COPY, 0, 0);
}

// Paste the contents of the clipboard into the document replacing the selection.
void wxScintilla::Paste()
{
    SendMsg(SCI_PASTE, 0, 0);
}

// Clear the selection.
void wxScintilla::Clear()
{
    SendMsg(SCI_CLEAR, 0, 0);
}

// Replace the contents of the document with the argument text.
void wxScintilla::SetText (const wxString& text)
{
    SendMsg(SCI_SETTEXT, 0, (sptr_t)(const char*)wx2sci(text));
}

// Retrieve all the text in the document.
wxString wxScintilla::GetText() const {
         int len  = GetTextLength();
         wxMemoryBuffer mbuf(len+1);   // leave room for the null...
         char* buf = (char*)mbuf.GetWriteBuf(len+1);
         SendMsg(SCI_GETTEXT, len+1, (sptr_t)buf);
         mbuf.UngetWriteBuf(len);
         mbuf.AppendByte(0);
         return sci2wx(buf);
}

// Retrieve the number of characters in the document.
int wxScintilla::GetTextLength() const
{
    return SendMsg(SCI_GETTEXTLENGTH, 0, 0);
}

// Set to overtype (true) or insert mode.
void wxScintilla::SetOvertype (bool overtype)
{
    SendMsg(SCI_SETOVERTYPE, overtype, 0);
}

// Returns true if overtype mode is active otherwise false is returned.
bool wxScintilla::GetOvertype() const
{
    return SendMsg(SCI_GETOVERTYPE, 0, 0) != 0;
}

// Set the width of the insert mode caret.
void wxScintilla::SetCaretWidth (int pixelWidth)
{
    SendMsg(SCI_SETCARETWIDTH, pixelWidth, 0);
}

// Returns the width of the insert mode caret.
int wxScintilla::GetCaretWidth() const
{
    return SendMsg(SCI_GETCARETWIDTH, 0, 0);
}

// Sets the position that starts the target which is used for updating the
// document without affecting the scroll position.
void wxScintilla::SetTargetStart (int pos)
{
    SendMsg(SCI_SETTARGETSTART, pos, 0);
}

// Get the position that starts the target.
int wxScintilla::GetTargetStart() const
{
    return SendMsg(SCI_GETTARGETSTART, 0, 0);
}

// Sets the position that ends the target which is used for updating the
// document without affecting the scroll position.
void wxScintilla::SetTargetEnd (int pos)
{
    SendMsg(SCI_SETTARGETEND, pos, 0);
}

// Get the position that ends the target.
int wxScintilla::GetTargetEnd() const
{
    return SendMsg(SCI_GETTARGETEND, 0, 0);
}

// Replace the target text with the argument text.
// Text is counted so it can contain NULs.
// Returns the length of the replacement text.

int wxScintilla::ReplaceTarget (const wxString& text)
{
    wxWX2MBbuf buf = (wxWX2MBbuf)wx2sci(text);
    return SendMsg(SCI_REPLACETARGET, strlen(buf), (sptr_t)(const char*)buf);
}

// Replace the target text with the argument text after \d processing.
// Text is counted so it can contain NULs.
// Looks for \d where d is between 1 and 9 and replaces these with the strings
// matched in the last search operation which were surrounded by \( and \).
// Returns the length of the replacement text including any change
// caused by processing the \d patterns.

int wxScintilla::ReplaceTargetRE (const wxString& text)
{
    wxWX2MBbuf buf = (wxWX2MBbuf)wx2sci(text);
    return SendMsg(SCI_REPLACETARGETRE, strlen(buf), (sptr_t)(const char*)buf);
}

// Search for a counted string in the target and set the target to the found
// range. Text is counted so it can contain NULs.
// Returns length of range or -1 for failure in which case target is not moved.

int wxScintilla::SearchInTarget (const wxString& text)
{
    wxWX2MBbuf buf = (wxWX2MBbuf)wx2sci(text);
    return SendMsg(SCI_SEARCHINTARGET, strlen(buf), (sptr_t)(const char*)buf);
}

// Set the search flags used by SearchInTarget.
void wxScintilla::SetSearchFlags (int flags)
{
    SendMsg(SCI_SETSEARCHFLAGS, flags, 0);
}

// Get the search flags used by SearchInTarget.
int wxScintilla::GetSearchFlags() const
{
    return SendMsg(SCI_GETSEARCHFLAGS, 0, 0);
}

// Show a call tip containing a definition near position pos.
void wxScintilla::CallTipShow (int pos, const wxString& definition)
{
    SendMsg(SCI_CALLTIPSHOW, pos, (long)(const char*)wx2sci(definition));
}

// Remove the call tip from the screen.
void wxScintilla::CallTipCancel()
{
    SendMsg(SCI_CALLTIPCANCEL, 0, 0);
}

// Is there an active call tip?
bool wxScintilla::CallTipActive()
{
    return SendMsg(SCI_CALLTIPACTIVE, 0, 0) != 0;
}

// Retrieve the position where the caret was before displaying the call tip.
int wxScintilla::CallTipPosAtStart()
{
    return SendMsg(SCI_CALLTIPPOSSTART, 0, 0);
}

// Highlight a segment of the definition.
void wxScintilla::CallTipSetHighlight (int start, int end)
{
    SendMsg(SCI_CALLTIPSETHLT, start, end);
}

// Set the background colour for the call tip.
void wxScintilla::CallTipSetBackground (const wxColour& back)
{
    SendMsg(SCI_CALLTIPSETBACK, wxColourAsLong(back), 0);
}

// Set the foreground colour for the call tip.
void wxScintilla::CallTipSetForeground (const wxColour& fore)
{
    SendMsg(SCI_CALLTIPSETFORE, wxColourAsLong(fore), 0);
}

// Set the foreground colour for the highlighted part of the call tip.
void wxScintilla::CallTipSetForegroundHighlight (const wxColour& fore)
{
    SendMsg(SCI_CALLTIPSETFOREHLT, wxColourAsLong(fore), 0);
}

// Enable use of STYLE_CALLTIP and set call tip tab size in pixels.
void wxScintilla::CallTipUseStyle (int tabSize)
{
    SendMsg(SCI_CALLTIPUSESTYLE, tabSize, 0);
}

// Find the display line of a document line taking hidden lines into account.
int wxScintilla::VisibleFromDocLine (int line)
{
    return SendMsg(SCI_VISIBLEFROMDOCLINE, line, 0);
}

// Find the document line of a display line taking hidden lines into account.
int wxScintilla::DocLineFromVisible (int lineDisplay)
{
    return SendMsg(SCI_DOCLINEFROMVISIBLE, lineDisplay, 0);
}

// The number of display lines needed to wrap a document line
int wxScintilla::WrapCount (int line)
{
    return SendMsg(SCI_WRAPCOUNT, line, 0);
}

// Set the fold level of a line.
// This encodes an integer level along with flags indicating whether the
// line is a header and whether it is effectively white space.
void wxScintilla::SetFoldLevel (int line, int level)
{
    SendMsg(SCI_SETFOLDLEVEL, line, level);
}

// Retrieve the fold level of a line.
int wxScintilla::GetFoldLevel (int line) const
{
    return SendMsg(SCI_GETFOLDLEVEL, line, 0);
}

// Find the last child line of a header line.
int wxScintilla::GetLastChild (int line, int level) const
{
    return SendMsg(SCI_GETLASTCHILD, line, level);
}

// Find the parent line of a child line.
int wxScintilla::GetFoldParent (int line) const
{
    return SendMsg(SCI_GETFOLDPARENT, line, 0);
}

// Make a range of lines visible.
void wxScintilla::ShowLines (int lineStart, int lineEnd)
{
    SendMsg(SCI_SHOWLINES, lineStart, lineEnd);
}

// Make a range of lines invisible.
void wxScintilla::HideLines (int lineStart, int lineEnd)
{
    SendMsg(SCI_HIDELINES, lineStart, lineEnd);
}

// Is a line visible?
bool wxScintilla::GetLineVisible (int line) const
{
    return SendMsg(SCI_GETLINEVISIBLE, line, 0) != 0;
}

// Show the children of a header line.
void wxScintilla::SetFoldExpanded (int line, bool expanded)
{
    SendMsg(SCI_SETFOLDEXPANDED, line, expanded);
}

// Is a header line expanded?
bool wxScintilla::GetFoldExpanded (int line) const
{
    return SendMsg(SCI_GETFOLDEXPANDED, line, 0) != 0;
}

// Switch a header line between expanded and contracted.
void wxScintilla::ToggleFold (int line)
{
    SendMsg(SCI_TOGGLEFOLD, line, 0);
}

// Ensure a particular line is visible by expanding any header line hiding it.
void wxScintilla::EnsureVisible (int line)
{
    SendMsg(SCI_ENSUREVISIBLE, line, 0);
}

// Set some style options for folding.
void wxScintilla::SetFoldFlags (int flags)
{
    SendMsg(SCI_SETFOLDFLAGS, flags, 0);
}

// Ensure a particular line is visible by expanding any header line hiding it.
// Use the currently set visibility policy to determine which range to display.
void wxScintilla::EnsureVisibleEnforcePolicy (int line)
{
    SendMsg(SCI_ENSUREVISIBLEENFORCEPOLICY, line, 0);
}

// Sets whether a tab pressed when caret is within indentation indents.
void wxScintilla::SetTabIndents (bool tabIndents)
{
    SendMsg(SCI_SETTABINDENTS, tabIndents, 0);
}

// Does a tab pressed when caret is within indentation indent?
bool wxScintilla::GetTabIndents() const
{
    return SendMsg(SCI_GETTABINDENTS, 0, 0) != 0;
}

// Sets whether a backspace pressed when caret is within indentation unindents.
void wxScintilla::SetBackSpaceUnIndents (bool bsUnIndents)
{
    SendMsg(SCI_SETBACKSPACEUNINDENTS, bsUnIndents, 0);
}

// Does a backspace pressed when caret is within indentation unindent?
bool wxScintilla::GetBackSpaceUnIndents() const
{
    return SendMsg(SCI_GETBACKSPACEUNINDENTS, 0, 0) != 0;
}

// Sets the time the mouse must sit still to generate a mouse dwell event.
void wxScintilla::SetMouseDwellTime (int periodMilliseconds)
{
    SendMsg(SCI_SETMOUSEDWELLTIME, periodMilliseconds, 0);
}

// Retrieve the time the mouse must sit still to generate a mouse dwell event.
int wxScintilla::GetMouseDwellTime() const
{
    return SendMsg(SCI_GETMOUSEDWELLTIME, 0, 0);
}

// Get position of start of word.
int wxScintilla::WordStartPosition (int pos, bool onlyWordCharacters)
{
    return SendMsg(SCI_WORDSTARTPOSITION, pos, onlyWordCharacters);
}

// Get position of end of word.
int wxScintilla::WordEndPosition (int pos, bool onlyWordCharacters)
{
    return SendMsg(SCI_WORDENDPOSITION, pos, onlyWordCharacters);
}

// Sets whether text is word wrapped.
void wxScintilla::SetWrapMode (int mode)
{
    SendMsg(SCI_SETWRAPMODE, mode, 0);
}

// Retrieve whether text is word wrapped.
int wxScintilla::GetWrapMode() const
{
    return SendMsg(SCI_GETWRAPMODE, 0, 0);
}

// Set the display mode of visual flags for wrapped lines.
void wxScintilla::SetWrapVisualFlags (int wrapVisualFlags)
{
    SendMsg(SCI_SETWRAPVISUALFLAGS, wrapVisualFlags, 0);
}

// Retrive the display mode of visual flags for wrapped lines.
int wxScintilla::GetWrapVisualFlags() const
{
    return SendMsg(SCI_GETWRAPVISUALFLAGS, 0, 0);
}

// Set the location of visual flags for wrapped lines.
void wxScintilla::SetWrapVisualFlagsLocation (int wrapVisualFlagsLocation)
{
    SendMsg(SCI_SETWRAPVISUALFLAGSLOCATION, wrapVisualFlagsLocation, 0);
}

// Retrive the location of visual flags for wrapped lines.
int wxScintilla::GetWrapVisualFlagsLocation() const
{
    return SendMsg(SCI_GETWRAPVISUALFLAGSLOCATION, 0, 0);
}

// Set the start indent for wrapped lines.
void wxScintilla::SetWrapStartIndent (int indent)
{
    SendMsg(SCI_SETWRAPSTARTINDENT, indent, 0);
}

// Retrive the start indent for wrapped lines.
int wxScintilla::GetWrapStartIndent() const
{
    return SendMsg(SCI_GETWRAPSTARTINDENT, 0, 0);
}

// Sets how wrapped sublines are placed. Default is fixed.
void wxScintilla::SetWrapIndentMode(int mode)
{
    SendMsg(SCI_SETWRAPINDENTMODE, mode, 0);
}

// Retrieve how wrapped sublines are placed. Default is fixed.
int wxScintilla::GetWrapIndentMode() const
{
    return SendMsg(SCI_GETWRAPINDENTMODE, 0, 0);
}

// Sets the degree of caching of layout information.
void wxScintilla::SetLayoutCache (int mode)
{
    SendMsg(SCI_SETLAYOUTCACHE, mode, 0);
}

// Retrieve the degree of caching of layout information.
int wxScintilla::GetLayoutCache() const
{
    return SendMsg(SCI_GETLAYOUTCACHE, 0, 0);
}

// Sets the document width assumed for scrolling.
void wxScintilla::SetScrollWidth (int pixelWidth)
{
    SendMsg(SCI_SETSCROLLWIDTH, pixelWidth, 0);
}

// Retrieve the document width assumed for scrolling.
int wxScintilla::GetScrollWidth() const
{
    return SendMsg(SCI_GETSCROLLWIDTH, 0, 0);
}

// Sets whether the maximum width line displayed is used to set scroll width.
void wxScintilla::SetScrollWidthTracking(bool tracking)
{
    SendMsg(SCI_SETSCROLLWIDTHTRACKING, tracking, 0);
}

// Retrieve whether the scroll width tracks wide lines.
bool wxScintilla::GetScrollWidthTracking() const
{
    return SendMsg(SCI_GETSCROLLWIDTHTRACKING, 0, 0) != 0;
}

// Measure the pixel width of some text in a particular style.
// NUL terminated text argument.
// Does not handle tab or control characters.
int wxScintilla::TextWidth (int style, const wxString& text)
{
    return SendMsg(SCI_TEXTWIDTH, style, (sptr_t)(const char*)wx2sci(text));
}

// Sets the scroll range so that maximum scroll position has
// the last line at the bottom of the view (default).
// Setting this to false allows scrolling one page below the last line.
void wxScintilla::SetEndAtLastLine (bool endAtLastLine)
{
    SendMsg(SCI_SETENDATLASTLINE, endAtLastLine, 0);
}

// Retrieve whether the maximum scroll position has the last
// line at the bottom of the view.
bool wxScintilla::GetEndAtLastLine() const
{
    return SendMsg(SCI_GETENDATLASTLINE, 0, 0) != 0;
}

// Retrieve the height of a particular line of text in pixels.
int wxScintilla::TextHeight (int line)
{
    return SendMsg(SCI_TEXTHEIGHT, line, 0);
}

// Show or hide the vertical scroll bar.
void wxScintilla::SetUseVerticalScrollBar (bool show)
{
    SendMsg(SCI_SETVSCROLLBAR, show, 0);
}

// Is the vertical scroll bar visible?
bool wxScintilla::GetUseVerticalScrollBar() const
{
    return SendMsg(SCI_GETVSCROLLBAR, 0, 0) != 0;
}

// Add text to the document at current position.
void wxScintilla::AppendText (const wxString& text)
{
    wxWX2MBbuf buf = (wxWX2MBbuf)wx2sci(text);
    SendMsg(SCI_APPENDTEXT, strlen(buf), (sptr_t)(const char*)buf);
}

// Append a string to the end of the document without changing the selection.
void wxScintilla::AppendText (int length, const wxString& text)
{
    SendMsg(SCI_APPENDTEXT, length, (sptr_t)(const char*)wx2sci(text));
}

// Is drawing done in two phases with backgrounds drawn before foregrounds?
bool wxScintilla::GetTwoPhaseDraw() const
{
    return SendMsg(SCI_GETTWOPHASEDRAW, 0, 0) != 0;
}

// In twoPhaseDraw mode, drawing is performed in two phases, first the background
// and then the foreground. This avoids chopping off characters that overlap the next run.
void wxScintilla::SetTwoPhaseDraw (bool twoPhase)
{
    SendMsg(SCI_SETTWOPHASEDRAW, twoPhase, 0);
}

// Choose the quality level for text from the FontQuality enumeration.
void wxScintilla::SetFontQuality(int fontQuality)
{
    SendMsg(SCI_SETFONTQUALITY, fontQuality, 0);
}

// Retrieve the quality level for text.
int wxScintilla::GetFontQuality() const
{
    return SendMsg(SCI_GETFONTQUALITY, 0, 0);
}

// Scroll so that a display line is at the top of the display.
void wxScintilla::SetFirstVisibleLine(int lineDisplay)
{
    SendMsg(SCI_SETFIRSTVISIBLELINE, lineDisplay, 0);
}

// Make the target range start and end be the same as the selection range start and end.
void wxScintilla::TargetFromSelection()
{
    SendMsg(SCI_TARGETFROMSELECTION, 0, 0);
}

// Join the lines in the target.
void wxScintilla::LinesJoin()
{
    SendMsg(SCI_LINESJOIN, 0, 0);
}

// Split the lines in the target into lines that are less wide than pixelWidth
// where possible.
void wxScintilla::LinesSplit (int pixelWidth)
{
    SendMsg(SCI_LINESSPLIT, pixelWidth, 0);
}

// Set the colours used as a chequerboard pattern in the fold margin
void wxScintilla::SetFoldMarginColour (bool useSetting, const wxColour& back)
{
    SendMsg(SCI_SETFOLDMARGINCOLOUR, useSetting, wxColourAsLong(back));
}
void wxScintilla::SetFoldMarginHiColour (bool useSetting, const wxColour& fore)
{
    SendMsg(SCI_SETFOLDMARGINHICOLOUR, useSetting, wxColourAsLong(fore));
}

// Move caret down one line.
void wxScintilla::LineDown()
{
    SendMsg(SCI_LINEDOWN, 0, 0);
}

// Move caret down one line extending selection to new caret position.
void wxScintilla::LineDownExtend()
{
    SendMsg(SCI_LINEDOWNEXTEND, 0, 0);
}

// Move caret up one line.
void wxScintilla::LineUp()
{
    SendMsg(SCI_LINEUP, 0, 0);
}

// Move caret up one line extending selection to new caret position.
void wxScintilla::LineUpExtend()
{
    SendMsg(SCI_LINEUPEXTEND, 0, 0);
}

// Move caret left one character.
void wxScintilla::CharLeft()
{
    SendMsg(SCI_CHARLEFT, 0, 0);
}

// Move caret left one character extending selection to new caret position.
void wxScintilla::CharLeftExtend()
{
    SendMsg(SCI_CHARLEFTEXTEND, 0, 0);
}

// Move caret right one character.
void wxScintilla::CharRight()
{
    SendMsg(SCI_CHARRIGHT, 0, 0);
}

// Move caret right one character extending selection to new caret position.
void wxScintilla::CharRightExtend()
{
    SendMsg(SCI_CHARRIGHTEXTEND, 0, 0);
}

// Move caret left one word.
void wxScintilla::WordLeft()
{
    SendMsg(SCI_WORDLEFT, 0, 0);
}

// Move caret left one word extending selection to new caret position.
void wxScintilla::WordLeftExtend()
{
    SendMsg(SCI_WORDLEFTEXTEND, 0, 0);
}

// Move caret right one word.
void wxScintilla::WordRight()
{
    SendMsg(SCI_WORDRIGHT, 0, 0);
}

// Move caret right one word extending selection to new caret position.
void wxScintilla::WordRightExtend()
{
    SendMsg(SCI_WORDRIGHTEXTEND, 0, 0);
}

// Move caret to first position on line.
void wxScintilla::Home()
{
    SendMsg(SCI_HOME, 0, 0);
}

// Move caret to first position on line extending selection to new caret position.
void wxScintilla::HomeExtend()
{
    SendMsg(SCI_HOMEEXTEND, 0, 0);
}

// Move caret to last position on line.
void wxScintilla::LineEnd()
{
    SendMsg(SCI_LINEEND, 0, 0);
}

// Move caret to last position on line extending selection to new caret position.
void wxScintilla::LineEndExtend()
{
    SendMsg(SCI_LINEENDEXTEND, 0, 0);
}

// Move caret to first position in document.
void wxScintilla::DocumentStart()
{
    SendMsg(SCI_DOCUMENTSTART, 0, 0);
}

// Move caret to first position in document extending selection to new caret position.
void wxScintilla::DocumentStartExtend()
{
    SendMsg(SCI_DOCUMENTSTARTEXTEND, 0, 0);
}

// Move caret to last position in document.
void wxScintilla::DocumentEnd()
{
    SendMsg(SCI_DOCUMENTEND, 0, 0);
}

// Move caret to last position in document extending selection to new caret position.
void wxScintilla::DocumentEndExtend()
{
    SendMsg(SCI_DOCUMENTENDEXTEND, 0, 0);
}

// Move caret one page up.
void wxScintilla::PageUp()
{
    SendMsg(SCI_PAGEUP, 0, 0);
}

// Move caret one page up extending selection to new caret position.
void wxScintilla::PageUpExtend()
{
    SendMsg(SCI_PAGEUPEXTEND, 0, 0);
}

// Move caret one page down.
void wxScintilla::PageDown()
{
    SendMsg(SCI_PAGEDOWN, 0, 0);
}

// Move caret one page down extending selection to new caret position.
void wxScintilla::PageDownExtend()
{
    SendMsg(SCI_PAGEDOWNEXTEND, 0, 0);
}

// Switch from insert to overtype mode or the reverse.
void wxScintilla::EditToggleOvertype()
{
    SendMsg(SCI_EDITTOGGLEOVERTYPE, 0, 0);
}

// Cancel any modes such as call tip or auto-completion list display.
void wxScintilla::Cancel()
{
    SendMsg(SCI_CANCEL, 0, 0);
}

// Delete the selection or if no selection, the character before the caret.
void wxScintilla::DeleteBack()
{
    SendMsg(SCI_DELETEBACK, 0, 0);
}

// If selection is empty or all on one line replace the selection with a tab character.
// If more than one line selected, indent the lines.
void wxScintilla::Tab()
{
    SendMsg(SCI_TAB, 0, 0);
}

// Dedent the selected lines.
void wxScintilla::BackTab()
{
    SendMsg(SCI_BACKTAB, 0, 0);
}

// Insert a new line, may use a CRLF, CR or LF depending on EOL mode.
void wxScintilla::NewLine()
{
    SendMsg(SCI_NEWLINE, 0, 0);
}

// Insert a Form Feed character.
void wxScintilla::FormFeed()
{
    SendMsg(SCI_FORMFEED, 0, 0);
}

// Move caret to before first visible character on line.
// If already there move to first character on line.
void wxScintilla::VCHome()
{
    SendMsg(SCI_VCHOME, 0, 0);
}

// Like VCHome but extending selection to new caret position.
void wxScintilla::VCHomeExtend()
{
    SendMsg(SCI_VCHOMEEXTEND, 0, 0);
}

// Magnify the displayed text by increasing the sizes by 1 point.
void wxScintilla::ZoomIn()
{
    SendMsg(SCI_ZOOMIN, 0, 0);
}

// Make the displayed text smaller by decreasing the sizes by 1 point.
void wxScintilla::ZoomOut()
{
    SendMsg(SCI_ZOOMOUT, 0, 0);
}

// Delete the word to the left of the caret.
void wxScintilla::DelWordLeft()
{
    SendMsg(SCI_DELWORDLEFT, 0, 0);
}

// Delete the word to the right of the caret.
void wxScintilla::DelWordRight()
{
    SendMsg(SCI_DELWORDRIGHT, 0, 0);
}

// Delete the word to the right of the caret, but not the trailing non-word characters.
void wxScintilla::DelWordRightEnd()
{
    SendMsg(SCI_DELWORDRIGHTEND, 0, 0);
}

// Cut the line containing the caret.
void wxScintilla::LineCut()
{
    SendMsg(SCI_LINECUT, 0, 0);
}

// Delete the line containing the caret.
void wxScintilla::LineDelete()
{
    SendMsg(SCI_LINEDELETE, 0, 0);
}

// Switch the current line with the previous.
void wxScintilla::LineTranspose()
{
    SendMsg(SCI_LINETRANSPOSE, 0, 0);
}

// Duplicate the current line.
void wxScintilla::LineDuplicate()
{
    SendMsg(SCI_LINEDUPLICATE, 0, 0);
}

// Transform the selection to lower case.
void wxScintilla::LowerCase()
{
    SendMsg(SCI_LOWERCASE, 0, 0);
}

// Transform the selection to upper case.
void wxScintilla::UpperCase()
{
    SendMsg(SCI_UPPERCASE, 0, 0);
}

// Scroll the document down, keeping the caret visible.
void wxScintilla::LineScrollDown()
{
    SendMsg(SCI_LINESCROLLDOWN, 0, 0);
}

// Scroll the document up, keeping the caret visible.
void wxScintilla::LineScrollUp()
{
    SendMsg(SCI_LINESCROLLUP, 0, 0);
}

// Delete the selection or if no selection, the character before the caret.
// Will not delete the character before at the start of a line.
void wxScintilla::DeleteBackNotLine()
{
    SendMsg(SCI_DELETEBACKNOTLINE, 0, 0);
}

// Move caret to first position on display line.
void wxScintilla::HomeDisplay()
{
    SendMsg(SCI_HOMEDISPLAY, 0, 0);
}

// Move caret to first position on display line extending selection to
// new caret position.
void wxScintilla::HomeDisplayExtend()
{
    SendMsg(SCI_HOMEDISPLAYEXTEND, 0, 0);
}

// Move caret to last position on display line.
void wxScintilla::LineEndDisplay()
{
    SendMsg(SCI_LINEENDDISPLAY, 0, 0);
}

// Move caret to last position on display line extending selection to new
// caret position.
void wxScintilla::LineEndDisplayExtend()
{
    SendMsg(SCI_LINEENDDISPLAYEXTEND, 0, 0);
}

// These are like their namesakes Home(Extend)?, LineEnd(Extend)?, VCHome(Extend)?
// except they behave differently when word-wrap is enabled:
// They go first to the start / end of the display line, like (Home|LineEnd)Display
// The difference is that, the cursor is already at the point, it goes on to the start
// or end of the document line, as appropriate for (Home|LineEnd|VCHome)(Extend)?.
void wxScintilla::HomeWrap()
{
    SendMsg(SCI_HOMEWRAP, 0, 0);
}
void wxScintilla::HomeWrapExtend()
{
    SendMsg(SCI_HOMEWRAPEXTEND, 0, 0);
}
void wxScintilla::LineEndWrap()
{
    SendMsg(SCI_LINEENDWRAP, 0, 0);
}
void wxScintilla::LineEndWrapExtend()
{
    SendMsg(SCI_LINEENDWRAPEXTEND, 0, 0);
}
void wxScintilla::VCHomeWrap()
{
    SendMsg(SCI_VCHOMEWRAP, 0, 0);
}
void wxScintilla::VCHomeWrapExtend()
{
    SendMsg(SCI_VCHOMEWRAPEXTEND, 0, 0);
}

// Copy the line containing the caret.
void wxScintilla::LineCopy()
{
    SendMsg(SCI_LINECOPY, 0, 0);
}

// Move the caret inside current view if it's not there already.
void wxScintilla::MoveCaretInsideView()
{
    SendMsg(SCI_MOVECARETINSIDEVIEW, 0, 0);
}

// How many characters are on a line, including end of line characters?
int wxScintilla::LineLength (int line) const
{
    return SendMsg(SCI_LINELENGTH, line, 0);
}

// Highlight the characters at two positions.
void wxScintilla::BraceHighlight (int pos1, int pos2)
{
    SendMsg(SCI_BRACEHIGHLIGHT, pos1, pos2);
}

// Highlight the character at a position indicating there is no matching brace.
void wxScintilla::BraceBadLight (int pos)
{
    SendMsg(SCI_BRACEBADLIGHT, pos, 0);
}

// Find the position of a matching brace or INVALID_POSITION if no match.
int wxScintilla::BraceMatch (int pos)
{
    return SendMsg(SCI_BRACEMATCH, pos, 0);
}

// Are the end of line characters visible?
bool wxScintilla::GetViewEOL() const
{
    return SendMsg(SCI_GETVIEWEOL, 0, 0) != 0;
}

// Make the end of line characters visible or invisible.
void wxScintilla::SetViewEOL (bool visible)
{
    SendMsg(SCI_SETVIEWEOL, visible, 0);
}

// Retrieve a pointer to the document object.
void* wxScintilla::GetDocPointer()
{
    return (void*)SendMsg(SCI_GETDOCPOINTER);
}

// Change the document object used.
void wxScintilla::SetDocPointer (void* docPointer)
{
    SendMsg(SCI_SETDOCPOINTER, 0, (sptr_t)docPointer);
}

// Set which document modification events are sent to the container.
void wxScintilla::SetModEventMask (int mask)
{
    SendMsg(SCI_SETMODEVENTMASK, mask, 0);
}

// Retrieve the column number which text should be kept within.
int wxScintilla::GetEdgeColumn() const
{
    return SendMsg(SCI_GETEDGECOLUMN, 0, 0);
}

// Set the column number of the edge.
// If text goes past the edge then it is highlighted.
void wxScintilla::SetEdgeColumn (int column)
{
    SendMsg(SCI_SETEDGECOLUMN, column, 0);
}

// Retrieve the edge highlight mode.
int wxScintilla::GetEdgeMode() const
{
    return SendMsg(SCI_GETEDGEMODE, 0, 0);
}

// The edge may be displayed by a line (EDGE_LINE) or by highlighting text that
// goes beyond it (EDGE_BACKGROUND) or not displayed at all (EDGE_NONE).
void wxScintilla::SetEdgeMode (int mode)
{
    SendMsg(SCI_SETEDGEMODE, mode, 0);
}

// Retrieve the colour used in edge indication.
wxColour wxScintilla::GetEdgeColour() const
{
    long colour = SendMsg(SCI_GETEDGECOLOUR, 0, 0);
    return wxColourFromLong(colour);
}

// Change the colour used in edge indication.
void wxScintilla::SetEdgeColour (const wxColour& edgeColour)
{
    SendMsg(SCI_SETEDGECOLOUR, wxColourAsLong(edgeColour), 0);
}

// Sets the current caret position to be the search anchor.
void wxScintilla::SearchAnchor()
{
    SendMsg(SCI_SEARCHANCHOR, 0, 0);
}

// Find some text starting at the search anchor.
// Does not ensure the selection is visible.
int wxScintilla::SearchNext (int flags, const wxString& text)
{
    return SendMsg(SCI_SEARCHNEXT, flags, (sptr_t)(const char*)wx2sci(text));
}

// Find some text starting at the search anchor and moving backwards.
// Does not ensure the selection is visible.
int wxScintilla::SearchPrev (int flags, const wxString& text)
{
    return SendMsg(SCI_SEARCHPREV, flags, (sptr_t)(const char*)wx2sci(text));
}

// Retrieves the number of lines completely visible.
int wxScintilla::LinesOnScreen() const
{
    return SendMsg(SCI_LINESONSCREEN, 0, 0);
}

// Set whether a pop up menu is displayed automatically when the user presses
// the wrong mouse button.
void wxScintilla::UsePopUp (bool allowPopUp)
{
    SendMsg(SCI_USEPOPUP, allowPopUp, 0);
}

// Is the selection rectangular? The alternative is the more common stream selection.
bool wxScintilla::SelectionIsRectangle() const
{
    return SendMsg(SCI_SELECTIONISRECTANGLE, 0, 0) != 0;
}

// Set the zoom level. This number of points is added to the size of all fonts.
// It may be positive to magnify or negative to reduce.
void wxScintilla::SetZoom (int zoom)
{
    SendMsg(SCI_SETZOOM, zoom, 0);
}

// Retrieve the zoom level.
int wxScintilla::GetZoom() const
{
    return SendMsg(SCI_GETZOOM, 0, 0);
}

// Create a new document object.
// Starts with reference count of 1 and not selected into editor.
void* wxScintilla::CreateDocument()
{
    return (void*)SendMsg(SCI_CREATEDOCUMENT, 0, 0);
}

// Extend life of document.
void wxScintilla::AddRefDocument (void* docPointer)
{
    SendMsg(SCI_ADDREFDOCUMENT, 0, (sptr_t)docPointer);
}

// Release a reference to the document, deleting document if it fades to black.
void wxScintilla::ReleaseDocument (void* docPointer)
{
    SendMsg(SCI_RELEASEDOCUMENT, 0, (sptr_t)docPointer);
}

// Get which document modification events are sent to the container.
int wxScintilla::GetModEventMask() const
{
    return SendMsg(SCI_GETMODEVENTMASK, 0, 0);
}

// Change internal focus flag.
void wxScintilla::SetSCIFocus (bool focus)
{
    SendMsg(SCI_SETFOCUS, focus, 0);
}

// Get internal focus flag.
bool wxScintilla::GetSCIFocus() const
{
    return SendMsg(SCI_GETFOCUS, 0, 0) != 0;
}

// Change error status - 0 = OK.
void wxScintilla::SetStatus (int statusCode)
{
    SendMsg(SCI_SETSTATUS, statusCode, 0);
}

// Get error status.
int wxScintilla::GetStatus() const
{
    return SendMsg(SCI_GETSTATUS, 0, 0);
}

// Set whether the mouse is captured when its button is pressed.
void wxScintilla::SetMouseDownCaptures (bool captures)
{
    SendMsg(SCI_SETMOUSEDOWNCAPTURES, captures, 0);
}

// Get whether mouse gets captured.
bool wxScintilla::GetMouseDownCaptures() const
{
    return SendMsg(SCI_GETMOUSEDOWNCAPTURES, 0, 0) != 0;
}

// Sets the cursor to one of the SC_CURSOR* values.
void wxScintilla::SetSCICursor (int cursorType)
{
    SendMsg(SCI_SETCURSOR, cursorType, 0);
}

// Get cursor type.
int wxScintilla::GetSCICursor() const
{
    return SendMsg(SCI_GETCURSOR, 0, 0);
}

// Change the way control characters are displayed:
// If symbol is < 32, keep the drawn way, else, use the given character.
void wxScintilla::SetControlCharSymbol (int symbol)
{
    SendMsg(SCI_SETCONTROLCHARSYMBOL, symbol, 0);
}

// Get the way control characters are displayed.
int wxScintilla::GetControlCharSymbol() const
{
    return SendMsg(SCI_GETCONTROLCHARSYMBOL, 0, 0);
}

// Move to the previous change in capitalisation.
void wxScintilla::WordPartLeft()
{
    SendMsg(SCI_WORDPARTLEFT, 0, 0);
}

// Move to the previous change in capitalisation extending selection
// to new caret position.
void wxScintilla::WordPartLeftExtend()
{
    SendMsg(SCI_WORDPARTLEFTEXTEND, 0, 0);
}

// Move to the change next in capitalisation.
void wxScintilla::WordPartRight()
{
    SendMsg(SCI_WORDPARTRIGHT, 0, 0);
}

// Move to the next change in capitalisation extending selection
// to new caret position.
void wxScintilla::WordPartRightExtend()
{
    SendMsg(SCI_WORDPARTRIGHTEXTEND, 0, 0);
}

// Set the way the display area is determined when a particular line
// is to be moved to by Find, FindNext, GotoLine, etc.
void wxScintilla::SetVisiblePolicy (int visiblePolicy, int visibleSlop)
{
    SendMsg(SCI_SETVISIBLEPOLICY, visiblePolicy, visibleSlop);
}

// Delete back from the current position to the start of the line.
void wxScintilla::DelLineLeft()
{
    SendMsg(SCI_DELLINELEFT, 0, 0);
}

// Delete forwards from the current position to the end of the line.
void wxScintilla::DelLineRight()
{
    SendMsg(SCI_DELLINERIGHT, 0, 0);
}

// Get and Set the xOffset (ie, horizonal scroll position).
void wxScintilla::SetXOffset (int newOffset)
{
    SendMsg(SCI_SETXOFFSET, newOffset, 0);
}
int wxScintilla::GetXOffset() const
{
    return SendMsg(SCI_GETXOFFSET, 0, 0);
}

// Set the last x chosen value to be the caret x position.
void wxScintilla::ChooseCaretX()
{
    SendMsg(SCI_CHOOSECARETX, 0, 0);
}

// Set the focus to this Scintilla widget (GTK+ specific)
// Grab focus (SCI_GRABFOCUS) not supported

// Set the way the caret is kept visible when going sideway.
// The exclusion zone is given in pixels.
void wxScintilla::SetXCaretPolicy (int caretPolicy, int caretSlop)
{
    SendMsg(SCI_SETXCARETPOLICY, caretPolicy, caretSlop);
}

// Set the way the line the caret is on is kept visible.
// The exclusion zone is given in lines.
void wxScintilla::SetYCaretPolicy (int caretPolicy, int caretSlop)
{
    SendMsg(SCI_SETYCARETPOLICY, caretPolicy, caretSlop);
}

// Set printing to line wrapped (SC_WRAP_WORD) or not line wrapped (SC_WRAP_NONE).
void wxScintilla::SetPrintWrapMode (int mode)
{
    SendMsg(SCI_SETPRINTWRAPMODE, mode, 0);
}

// Is printing line wrapped?
int wxScintilla::GetPrintWrapMode() const
{
    return SendMsg(SCI_GETPRINTWRAPMODE, 0, 0);
}

// Set a fore colour for active hotspots.
void wxScintilla::SetHotspotActiveForeground (bool useSetting, const wxColour& fore)
{
    SendMsg(SCI_SETHOTSPOTACTIVEFORE, useSetting, wxColourAsLong(fore));
}

// Get the fore colour for active hotspots.
wxColour wxScintilla::GetHotspotActiveForeground() const
{
    long c = SendMsg(SCI_GETHOTSPOTACTIVEFORE, 0, 0);
    return wxColourFromLong(c);
}

// Set a back colour for active hotspots.
void wxScintilla::SetHotspotActiveBackground (bool useSetting, const wxColour& back)
{
    SendMsg(SCI_SETHOTSPOTACTIVEBACK, useSetting, wxColourAsLong(back));
}

// Get the back colour for active hotspots.
wxColour wxScintilla::GetHotspotActiveBackground() const
{
    long c = SendMsg(SCI_GETHOTSPOTACTIVEBACK, 0, 0);
    return wxColourFromLong(c);
}

// Enable / Disable underlining active hotspots.
void wxScintilla::SetHotspotActiveUnderline (bool underline)
{
    SendMsg(SCI_SETHOTSPOTACTIVEUNDERLINE, underline, 0);
}

// Get whether underlining for active hotspots.
bool wxScintilla::GetHotspotActiveUnderline() const
{
    return SendMsg(SCI_GETHOTSPOTACTIVEUNDERLINE, 0, 0) != 0;
}

// Limit hotspots to single line so hotspots on two lines don't merge.
void wxScintilla::SetHotspotSingleLine (bool singleLine)
{
    SendMsg(SCI_SETHOTSPOTSINGLELINE, singleLine, 0);
}

// Get the HotspotSingleLine property
bool wxScintilla::GetHotspotSingleLine() const
{
    return SendMsg(SCI_GETHOTSPOTSINGLELINE, 0, 0) != 0;
}

// Move caret between paragraphs (delimited by empty lines).
void wxScintilla::ParaDown()
{
    SendMsg(SCI_PARADOWN, 0, 0);
}
void wxScintilla::ParaDownExtend()
{
    SendMsg(SCI_PARADOWNEXTEND, 0, 0);
}
void wxScintilla::ParaUp()
{
    SendMsg(SCI_PARAUP, 0, 0);
}
void wxScintilla::ParaUpExtend()
{
    SendMsg(SCI_PARAUPEXTEND, 0, 0);
}

// Given a valid document position, return the previous position taking code
// page into account. Returns 0 if passed 0.
int wxScintilla::PositionBefore (int pos)
{
    return SendMsg(SCI_POSITIONBEFORE, pos, 0);
}

// Given a valid document position, return the next position taking code
// page into account. Maximum value returned is the last position in the document.
int wxScintilla::PositionAfter (int pos)
{
    return SendMsg(SCI_POSITIONAFTER, pos, 0);
}

// Copy a range of text to the clipboard. Positions are clipped into the document.
void wxScintilla::CopyRange (int start, int end)
{
    SendMsg(SCI_COPYRANGE, start, end);
}

// Copy argument text to the clipboard.
void wxScintilla::CopyText (int length, const wxString& text)
{
    SendMsg(SCI_COPYTEXT, length, (sptr_t)(const char*)wx2sci(text));
}

// Set the selection mode to stream (SC_SEL_STREAM) or rectangular (SC_SEL_RECTANGLE/SC_SEL_THIN) or
// by lines (SC_SEL_LINES).
void wxScintilla::SetSelectionMode (int mode)
{
    SendMsg(SCI_SETSELECTIONMODE, mode, 0);
}

// Get the mode of the current selection.
int wxScintilla::GetSelectionMode() const
{
    return SendMsg(SCI_GETSELECTIONMODE, 0, 0);
}

// Retrieve the position of the start of the selection at the given line (INVALID_POSITION if no selection on this line).
int wxScintilla::GetLineSelStartPosition (int line)
{
    return SendMsg(SCI_GETLINESELSTARTPOSITION, line, 0);
}

// Retrieve the position of the end of the selection at the given line (INVALID_POSITION if no selection on this line).
int wxScintilla::GetLineSelEndPosition (int line)
{
    return SendMsg(SCI_GETLINESELENDPOSITION, line, 0);
}

// Move caret down one line, extending rectangular selection to new caret position.
void wxScintilla::LineDownRectExtend()
{
    SendMsg(SCI_LINEDOWNRECTEXTEND, 0, 0);
}

// Move caret up one line, extending rectangular selection to new caret position.
void wxScintilla::LineUpRectExtend()
{
    SendMsg(SCI_LINEUPRECTEXTEND, 0, 0);
}

// Move caret left one character, extending rectangular selection to new caret position.
void wxScintilla::CharLeftRectExtend()
{
    SendMsg(SCI_CHARLEFTRECTEXTEND, 0, 0);
}

// Move caret right one character, extending rectangular selection to new caret position.
void wxScintilla::CharRightRectExtend()
{
    SendMsg(SCI_CHARRIGHTRECTEXTEND, 0, 0);
}

// Move caret to first position on line, extending rectangular selection to new caret position.
void wxScintilla::HomeRectExtend()
{
    SendMsg(SCI_HOMERECTEXTEND, 0, 0);
}

// Move caret to before first visible character on line.
// If already there move to first character on line.
// In either case, extend rectangular selection to new caret position.
void wxScintilla::VCHomeRectExtend()
{
    SendMsg(SCI_VCHOMERECTEXTEND, 0, 0);
}

// Move caret to last position on line, extending rectangular selection to new caret position.
void wxScintilla::LineEndRectExtend()
{
    SendMsg(SCI_LINEENDRECTEXTEND, 0, 0);
}

// Move caret one page up, extending rectangular selection to new caret position.
void wxScintilla::PageUpRectExtend()
{
    SendMsg(SCI_PAGEUPRECTEXTEND, 0, 0);
}

// Move caret one page down, extending rectangular selection to new caret position.
void wxScintilla::PageDownRectExtend()
{
    SendMsg(SCI_PAGEDOWNRECTEXTEND, 0, 0);
}

// Move caret to top of page, or one page up if already at top of page.
void wxScintilla::StutteredPageUp()
{
    SendMsg(SCI_STUTTEREDPAGEUP, 0, 0);
}

// Move caret to top of page, or one page up if already at top of page, extending selection to new caret position.
void wxScintilla::StutteredPageUpExtend()
{
    SendMsg(SCI_STUTTEREDPAGEUPEXTEND, 0, 0);
}

// Move caret to bottom of page, or one page down if already at bottom of page.
void wxScintilla::StutteredPageDown()
{
    SendMsg(SCI_STUTTEREDPAGEDOWN, 0, 0);
}

// Move caret to bottom of page, or one page down if already at bottom of page, extending selection to new caret position.
void wxScintilla::StutteredPageDownExtend()
{
    SendMsg(SCI_STUTTEREDPAGEDOWNEXTEND, 0, 0);
}

// Move caret left one word, position cursor at end of word.
void wxScintilla::WordLeftEnd()
{
    SendMsg(SCI_WORDLEFTEND, 0, 0);
}

// Move caret left one word, position cursor at end of word, extending selection to new caret position.
void wxScintilla::WordLeftEndExtend()
{
    SendMsg(SCI_WORDLEFTENDEXTEND, 0, 0);
}

// Move caret right one word, position cursor at end of word.
void wxScintilla::WordRightEnd()
{
    SendMsg(SCI_WORDRIGHTEND, 0, 0);
}

// Move caret right one word, position cursor at end of word, extending selection to new caret position.
void wxScintilla::WordRightEndExtend()
{
    SendMsg(SCI_WORDRIGHTENDEXTEND, 0, 0);
}

// Set the set of characters making up whitespace for when moving or selecting by word.
// Should be called after SetWordChars.
void wxScintilla::SetWhitespaceChars (const wxString& characters)
{
    SendMsg(SCI_SETWHITESPACECHARS, 0, (sptr_t)(const char*)wx2sci(characters));
}

// Reset the set of characters for whitespace and word characters to the defaults.
void wxScintilla::SetCharsDefault()
{
    SendMsg(SCI_SETCHARSDEFAULT, 0, 0);
}

// Get currently selected item position in the auto-completion list
int wxScintilla::AutoCompGetCurrent()
{
    return SendMsg(SCI_AUTOCGETCURRENT, 0, 0);
}

// Get currently selected item text in the auto-completion list
// Returns the length of the item text
wxString wxScintilla::AutoCGetCurrentText()
{
    int len (0);

    // determine the selected text range
    len = SendMsg(SCI_AUTOCGETCURRENTTEXT, 0, 0);
    if (!len) return wxEmptyString;

    wxMemoryBuffer mbuf(len+2);
    char* buf = (char*)mbuf.GetWriteBuf(len+1);
    SendMsg(SCI_AUTOCGETCURRENTTEXT, 0, (sptr_t)buf);
    mbuf.UngetWriteBuf(len);
    mbuf.AppendByte(0);
    return sci2wx(buf);
}

// Enlarge the document to a particular size of text bytes.
void wxScintilla::Allocate (int bytes)
{
    SendMsg(SCI_ALLOCATE, bytes, 0);
}

// Target as UTF8 (SCI_TARGETASUTF8) not supported

// Set lenght for encode (SCI_SETLENGTHFORENCODE) not supported

// Set endoce from UTF8 (SCI_ENCODEDFROMUTF8) not supported

// Find the position of a column on a line taking into account tabs and
// multi-byte characters. If beyond end of line, return line end position.
int wxScintilla::FindColumn (int line, int column)
{
    return SendMsg(SCI_ENCODEDFROMUTF8, line, column);
}

// Can the caret preferred x position only be changed by explicit movement commands?
bool wxScintilla::GetCaretSticky () const
{
    return SendMsg(SCI_GETCARETSTICKY, 0, 0) != 0;
}

// Stop the caret preferred x position changing when the user types.
void wxScintilla::SetCaretSticky (bool useCaretStickyBehaviour)
{
    SendMsg(SCI_SETCARETSTICKY, useCaretStickyBehaviour, 0);
}

// Switch between sticky and non-sticky: meant to be bound to a key.
void wxScintilla::ToggleCaretSticky ()
{
    SendMsg(SCI_TOGGLECARETSTICKY, 0, 0);
}

// Enable/Disable convert-on-paste for line endings
void wxScintilla::SetPasteConvertEndings (bool convert)
{
    SendMsg(SCI_SETPASTECONVERTENDINGS, convert, 0);
}

// Get convert-on-paste setting
bool wxScintilla::GetPasteConvertEndings () const
{
    return SendMsg(SCI_GETPASTECONVERTENDINGS, 0, 0) != 0;
}

// Duplicate the selection. If selection empty duplicate the line containing the caret.
void wxScintilla::SelectionDuplicate ()
{
    SendMsg(SCI_SELECTIONDUPLICATE, 0, 0);
}

// Set background alpha of the caret line.
void wxScintilla::SetCaretLineBackAlpha(int alpha)
{
    SendMsg(SCI_SETCARETLINEBACKALPHA, alpha, 0);
}

// Get the background alpha of the caret line.
int wxScintilla::GetCaretLineBackAlpha () const
{
    return SendMsg(SCI_GETCARETLINEBACKALPHA, 0, 0);
}

// Set the style of the caret to be drawn.
void wxScintilla::SetCaretStyle(int caretStyle)
{
    SendMsg(SCI_SETCARETSTYLE, caretStyle, 0);
}

// Returns the current style of the caret.
int wxScintilla::GetCaretStyle() const
{
    return SendMsg(SCI_GETCARETSTYLE, 0, 0);
}

// Set the indicator used for IndicatorFillRange and IndicatorClearRange
void wxScintilla::SetIndicatorCurrent(int indicator)
{
    SendMsg(SCI_SETINDICATORCURRENT, indicator, 0);
}

// Get the current indicator
int wxScintilla::GetIndicatorCurrent() const
{
    return SendMsg(SCI_GETINDICATORCURRENT, 0, 0);
}

// Set the value used for IndicatorFillRange
void wxScintilla::SetIndicatorValue(int value)
{
    SendMsg(SCI_SETINDICATORVALUE, value, 0);
}

// Get the current indicator vaue
int wxScintilla::GetIndicatorValue() const
{
    return SendMsg(SCI_GETINDICATORVALUE, 0, 0);
}

// Turn a indicator on over a range.
void wxScintilla::IndicatorFillRange(int position, int fillLength)
{
    SendMsg(SCI_INDICATORFILLRANGE, position, fillLength);
}

// Turn a indicator off over a range.
void wxScintilla::IndicatorClearRange(int position, int clearLength)
{
    SendMsg(SCI_INDICATORCLEARRANGE, position, clearLength);
}

// Are any indicators present at position?
int wxScintilla::IndicatorAllOnFor(int position)
{
    return SendMsg(SCI_INDICATORALLONFOR, position, 0);
}

// What value does a particular indicator have at at a position?
int wxScintilla::IndicatorValueAt(int indicator, int position)
{
    return SendMsg(SCI_INDICATORVALUEAT, indicator, position);
}

// Where does a particular indicator start?
int wxScintilla::IndicatorStart(int indicator, int position)
{
    return SendMsg(SCI_INDICATORSTART, indicator, position);
}

// Where does a particular indicator end?
int wxScintilla::IndicatorEnd(int indicator, int position)
{
    return SendMsg(SCI_INDICATOREND, indicator, position);
}

// Set number of entries in position cache
void wxScintilla::SetPositionCacheSize(int size)
{
    SendMsg(SCI_SETPOSITIONCACHE, size, 0);
}

// How many entries are allocated to the position cache?
int wxScintilla::GetPositionCacheSize() const
{
    return SendMsg(SCI_GETPOSITIONCACHE, 0, 0);
}

// Copy the selection, if selection empty copy the line with the caret
void wxScintilla::CopyAllowLine()
{
    SendMsg(SCI_COPYALLOWLINE, 0, 0);
}

// Compact the document buffer and return a read-only pointer to the
// characters in the document.
// defined later as wxUIntPtr wxScintilla::GetCharacterPointer() const;
/*
int wxScintilla::GetCharacterPointer() const
{
    return SendMsg(SCI_GETCHARACTERPOINTER, 0, 0);
}
*/

// Always interpret keyboard input as Unicode
void wxScintilla::SetKeysUnicode(bool keysUnicode)
{
    SendMsg(SCI_SETKEYSUNICODE, keysUnicode, 0);
}

// Are keys always interpreted as Unicode?
bool wxScintilla::GetKeysUnicode() const
{
    return SendMsg(SCI_GETKEYSUNICODE, 0, 0) != 0;
}

// Set the alpha fill colour of the given indicator.
void wxScintilla::IndicSetAlpha(int indicator, int alpha)
{
    SendMsg(SCI_INDICSETALPHA, indicator, alpha);
}

// Get the alpha fill colour of the given indicator.
int wxScintilla::IndicGetAlpha(int indicator) const
{
    return SendMsg(SCI_INDICGETALPHA, indicator, 0);
}

// Set extra ascent for each line
void wxScintilla::SetExtraAscent(int extraAscent)
{
    SendMsg(SCI_SETEXTRAASCENT, extraAscent, 0);
}

// Get extra ascent for each line
int wxScintilla::GetExtraAscent() const
{
    return SendMsg(SCI_GETEXTRAASCENT, 0, 0);
}

// Set extra descent for each line
void wxScintilla::SetExtraDescent(int extraDescent)
{
    SendMsg(SCI_SETEXTRADESCENT, extraDescent, 0);
}

// Get extra descent for each line
int wxScintilla::GetExtraDescent() const
{
    return SendMsg(SCI_GETEXTRADESCENT, 0, 0);
}

// Which symbol was defined for markerNumber with MarkerDefine
int wxScintilla::MarkerSymbolDefined(int markerNumber)
{
    return SendMsg(SCI_MARKERSYMBOLDEFINED, markerNumber, 0);
}

// Set the text in the text margin for a line
void wxScintilla::MarginSetText(int line, const wxString& text)
{
    SendMsg(SCI_MARGINSETTEXT, line, (sptr_t)(const char*)wx2sci(text));
}

// Get the text in the text margin for a line
wxString wxScintilla::MarginGetText(int line) const
{
    int len = LineLength(line);
    if (!len) return wxEmptyString;

    wxMemoryBuffer mbuf(len+1);
    char* buf = (char*)mbuf.GetWriteBuf(len+1);
    SendMsg(SCI_MARGINGETTEXT, (sptr_t)buf);
    mbuf.UngetWriteBuf(len);
    mbuf.AppendByte(0);
    return sci2wx(buf);
}

// Set the style number for the text margin for a line
void wxScintilla::MarginSetStyle(int line, int style)
{
    SendMsg(SCI_MARGINSETSTYLE, line, style);
}

// Get the style number for the text margin for a line
int wxScintilla::MarginGetStyle(int line) const
{
    return SendMsg(SCI_MARGINGETSTYLE, line, 0);
}

// Set the style in the text margin for a line
void wxScintilla::MarginSetStyles(int line, const wxString& styles)
{
    SendMsg(SCI_MARGINSETSTYLES, line, (sptr_t)(const char*)wx2sci(styles));
}

// Get the styles in the text margin for a line
wxString wxScintilla::MarginGetStyles(int line) const
{
    int len = LineLength(line);
    if (!len) return wxEmptyString;

    wxMemoryBuffer mbuf(len+1);
    char* buf = (char*)mbuf.GetWriteBuf(len+1);
    SendMsg(SCI_MARGINGETSTYLES, (sptr_t)buf);
    mbuf.UngetWriteBuf(len);
    mbuf.AppendByte(0);
    return sci2wx(buf);
}

// Clear the margin text on all lines
void wxScintilla::MarginTextClearAll()
{
    SendMsg(SCI_MARGINTEXTCLEARALL, 0, 0);
}

// Get the start of the range of style numbers used for margin text
void wxScintilla::MarginSetStyleOffset(int style)
{
    SendMsg(SCI_MARGINSETSTYLEOFFSET, style, 0);
}

// Get the start of the range of style numbers used for margin text
int wxScintilla::MarginGetStyleOffset() const
{
    return SendMsg(SCI_MARGINGETSTYLEOFFSET, 0, 0);
}

// Set the annotation text for a line
void wxScintilla::AnnotationSetText(int line, const wxString& text)
{
    SendMsg(SCI_ANNOTATIONSETTEXT, line, (sptr_t)(const char*)wx2sci(text));
}

// Get the annotation text for a line
wxString wxScintilla::AnnotationGetText(int line) const
{
    int len = LineLength(line);
    if (!len) return wxEmptyString;

    wxMemoryBuffer mbuf(len+1);
    char* buf = (char*)mbuf.GetWriteBuf(len+1);
    SendMsg(SCI_ANNOTATIONGETTEXT, (sptr_t)buf);
    mbuf.UngetWriteBuf(len);
    mbuf.AppendByte(0);
    return sci2wx(buf);
}

// Set the style number for the annotations for a line
void wxScintilla::AnnotationSetStyle(int line, int style)
{
    SendMsg(SCI_ANNOTATIONSETSTYLE, line, style);
}

// Get the style number for the annotations for a line
int wxScintilla::AnnotationGetStyle(int line) const
{
    return SendMsg(SCI_ANNOTATIONGETSTYLE, line, 0);
}

// Set the annotation styles for a line
void wxScintilla::AnnotationSetStyles(int line, const wxString& styles)
{
    SendMsg(SCI_ANNOTATIONSETSTYLES, line, (sptr_t)(const char*)wx2sci(styles));
}

// Get the annotation styles for a line
wxString wxScintilla::AnnotationGetStyles(int line) const
{
    int len = LineLength(line);
    if (!len) return wxEmptyString;

    wxMemoryBuffer mbuf(len+1);
    char* buf = (char*)mbuf.GetWriteBuf(len+1);
    SendMsg(SCI_ANNOTATIONGETSTYLES, (sptr_t)buf);
    mbuf.UngetWriteBuf(len);
    mbuf.AppendByte(0);
    return sci2wx(buf);
}

// Get the number of annotation lines for a line
int wxScintilla::AnnotationGetLines(int line) const
{
    return SendMsg(SCI_ANNOTATIONGETLINES, line, 0);
}

// Clear the annotations from all lines
void wxScintilla::AnnotationClearAll()
{
    SendMsg(SCI_ANNOTATIONCLEARALL, 0, 0);
}

// Set the visibility for the annotations for a view
void wxScintilla::AnnotationSetVisible(int visible)
{
    SendMsg(SCI_ANNOTATIONSETVISIBLE, visible, 0);
}

// Get the visibility for the annotations for a view
int wxScintilla::AnnotationGetVisible() const
{
    return SendMsg(SCI_ANNOTATIONGETVISIBLE, 0, 0);
}

// Get the start of the range of style numbers used for annotations
void wxScintilla::AnnotationSetStyleOffset(int style)
{
    SendMsg(SCI_ANNOTATIONSETSTYLEOFFSET, style, 0);
}

// Get the start of the range of style numbers used for annotations
int wxScintilla::AnnotationGetStyleOffset() const
{
    return SendMsg(SCI_ANNOTATIONGETSTYLEOFFSET, 0, 0);
}

// Add a container action to the undo stack
void wxScintilla::AddUndoAction(int token, int flags)
{
    SendMsg(SCI_ADDUNDOACTION, token, flags);
}

// Find the position of a character from a point within the window.
int wxScintilla::CharPositionFromPoint(int x, int y)
{
    return SendMsg(SCI_CHARPOSITIONFROMPOINT, x, y);
}

// Find the position of a character from a point within the window.
// Return INVALID_POSITION if not close to text.
int wxScintilla::CharPositionFromPointClose(int x, int y)
{
    return SendMsg(SCI_CHARPOSITIONFROMPOINTCLOSE, x, y);
}

// Set whether multiple selections can be made
void wxScintilla::SetMultipleSelection(bool multipleSelection)
{
    SendMsg(SCI_SETMULTIPLESELECTION, multipleSelection, 0);
}

// Whether multiple selections can be made
bool wxScintilla::GetMultipleSelection() const
{
    return SendMsg(SCI_GETMULTIPLESELECTION, 0, 0) != 0;
}

// Set whether typing can be performed into multiple selections
void wxScintilla::SetAdditionalSelectionTyping(bool additionalSelectionTyping)
{
    SendMsg(SCI_SETADDITIONALSELECTIONTYPING, additionalSelectionTyping, 0);
}

// Whether typing can be performed into multiple selections
bool wxScintilla::GetAdditionalSelectionTyping() const
{
    return SendMsg(SCI_GETADDITIONALSELECTIONTYPING, 0, 0) != 0;
}

// Set whether additional carets will blink
void wxScintilla::SetAdditionalCaretsBlink(bool additionalCaretsBlink)
{
    SendMsg(SCI_SETADDITIONALCARETSBLINK, additionalCaretsBlink, 0);
}

// Whether additional carets will blink
bool wxScintilla::GetAdditionalCaretsBlink() const
{
    return SendMsg(SCI_GETADDITIONALCARETSBLINK, 0, 0) != 0;
}

// Set whether additional carets are visible
void wxScintilla::SetAdditionalCaretsVisible(bool additionalCaretsBlink)
{
    SendMsg(SCI_SETADDITIONALCARETSVISIBLE, additionalCaretsBlink, 0);
}

// Whether additional carets are visible
bool wxScintilla::GetAdditionalCaretsVisible() const
{
    return SendMsg(SCI_GETADDITIONALCARETSVISIBLE, 0, 0) != 0;
}

// How many selections are there?
int wxScintilla::GetSelections() const
{
    return SendMsg(SCI_GETSELECTIONS, 0, 0);
}

// Clear selections to a single empty stream selection
void wxScintilla::ClearSelections()
{
    SendMsg(SCI_CLEARSELECTIONS, 0, 0);
}

// Set a simple selection
int wxScintilla::SetSelectionInt(int caret, int anchor)
{
    return SendMsg(SCI_SETSELECTION, caret, anchor);
}

// Add a selection
int wxScintilla::AddSelection(int caret, int anchor)
{
    return SendMsg(SCI_ADDSELECTION, caret, anchor);
}

// Set the main selection
void wxScintilla::SetMainSelection(int selection)
{
    SendMsg(SCI_SETMAINSELECTION, selection, 0);
}

// Which selection is the main selection
int wxScintilla::GetMainSelection() const
{
    return SendMsg(SCI_GETMAINSELECTION, 0, 0);
}
void wxScintilla::SetSelectionNCaret(int selection, int pos)
{
    SendMsg(SCI_SETSELECTIONNCARET, selection, pos);
}
int wxScintilla::GetSelectionNCaret(int selection) const
{
    return SendMsg(SCI_GETSELECTIONNCARET, selection, 0);
}
void wxScintilla::SetSelectionNAnchor(int selection, int posAnchor)
{
    SendMsg(SCI_SETSELECTIONNANCHOR, selection, posAnchor);
}
int wxScintilla::GetSelectionNAnchor(int selection) const
{
    return SendMsg(SCI_GETSELECTIONNANCHOR, selection, 0);
}
void wxScintilla::SetSelectionNCaretVirtualSpace(int selection, int space)
{
    SendMsg(SCI_SETSELECTIONNCARETVIRTUALSPACE, selection, space);
}
int wxScintilla::GetSelectionNCaretVirtualSpace(int selection) const
{
    return SendMsg(SCI_GETSELECTIONNCARETVIRTUALSPACE, selection, 0);
}

void wxScintilla::SetSelectionNAnchorVirtualSpace(int selection, int space)
{
    SendMsg(SCI_SETSELECTIONNANCHORVIRTUALSPACE, selection, space);
}
int wxScintilla::GetSelectionNAnchorVirtualSpace(int selection) const
{
    return SendMsg(SCI_GETSELECTIONNANCHORVIRTUALSPACE, selection, 0);
}
// Sets the position that starts the selection - this becomes the anchor.
void wxScintilla::SetSelectionNStart(int selection, int pos)
{
    SendMsg(SCI_SETSELECTIONNSTART, selection, pos);
}

// Returns the position at the start of the selection.
int wxScintilla::GetSelectionNStart(int selection) const
{
    return SendMsg(SCI_GETSELECTIONNSTART, selection, 0);
}

// Sets the position that ends the selection - this becomes the currentPosition.
void wxScintilla::SetSelectionNEnd(int selection, int pos)
{
    SendMsg(SCI_SETSELECTIONNEND, selection, pos);
}

// Returns the position at the end of the selection.
int wxScintilla::GetSelectionNEnd(int selection) const
{
    return SendMsg(SCI_GETSELECTIONNEND, selection, 0);
}

void wxScintilla::SetRectangularSelectionCaret(int pos)
{
    SendMsg(SCI_SETRECTANGULARSELECTIONCARET, pos, 0);
}

int wxScintilla::GetRectangularSelectionCaret() const
{
    return SendMsg(SCI_GETRECTANGULARSELECTIONCARET, 0, 0);
}

void wxScintilla::SetRectangularSelectionAnchor(int posAnchor)
{
    SendMsg(SCI_SETRECTANGULARSELECTIONANCHOR, posAnchor, 0);
}

int wxScintilla::GetRectangularSelectionAnchor() const
{
    return SendMsg(SCI_GETRECTANGULARSELECTIONANCHOR, 0, 0);
}

void wxScintilla::SetRectangularSelectionCaretVirtualSpace(int space)
{
    SendMsg(SCI_SETRECTANGULARSELECTIONCARETVIRTUALSPACE, space, 0);
}

int wxScintilla::GetRectangularSelectionCaretVirtualSpace() const
{
    return SendMsg(SCI_GETRECTANGULARSELECTIONCARETVIRTUALSPACE, 0, 0);
}

void wxScintilla::SetRectangularSelectionAnchorVirtualSpace(int space)
{
    SendMsg(SCI_SETRECTANGULARSELECTIONANCHORVIRTUALSPACE, space, 0);
}

int wxScintilla::GetRectangularSelectionAnchorVirtualSpace() const
{
    return SendMsg(SCI_GETRECTANGULARSELECTIONANCHORVIRTUALSPACE, 0, 0);
}

void wxScintilla::SetVirtualSpaceOptions(int virtualSpaceOptions)
{
    SendMsg(SCI_SETVIRTUALSPACEOPTIONS, virtualSpaceOptions, 0);
}

int wxScintilla::GetVirtualSpaceOptions() const
{
    return SendMsg(SCI_GETVIRTUALSPACEOPTIONS, 0, 0);
}

// On GTK+, allow selecting the modifier key to use for mouse-based
// rectangular selection. Often the window manager requires Alt+Mouse Drag
// for moving windows.
// Valid values are SCMOD_CTRL(default), SCMOD_ALT, or SCMOD_SUPER.
void wxScintilla::SetRectangularSelectionModifier(int modifier)
{
    SendMsg(SCI_SETRECTANGULARSELECTIONMODIFIER, modifier, 0);
}

// Get the modifier key used for rectangular selection.
int wxScintilla::GetRectangularSelectionModifier() const
{
    return SendMsg(SCI_GETRECTANGULARSELECTIONMODIFIER, 0, 0);
}

// Set the foreground colour of additional selections.
// Must have previously called SetSelFore with non-zero first argument for this to have an effect.
void wxScintilla::SetAdditionalSelFore(const wxColour& fore)
{
    SendMsg(SCI_SETADDITIONALSELFORE, wxColourAsLong(fore), 0);
}

// Set the background colour of additional selections.
// Must have previously called SetSelBack with non-zero first argument for this to have an effect.
void wxScintilla::SetAdditionalSelBack(const wxColour& back)
{
    SendMsg(SCI_SETADDITIONALSELBACK, wxColourAsLong(back), 0);
}

// Set the alpha of the selection.
void wxScintilla::SetAdditionalSelAlpha(int alpha)
{
    SendMsg(SCI_SETADDITIONALSELALPHA, alpha, 0);
}

// Get the alpha of the selection.
int wxScintilla::GetAdditionalSelAlpha() const
{
    return SendMsg(SCI_GETADDITIONALSELALPHA, 0, 0);
}

// Set the foreground colour of additional carets.
void wxScintilla::SetAdditionalCaretFore(const wxColour& fore)
{
    SendMsg(SCI_SETADDITIONALCARETFORE, wxColourAsLong(fore), 0);
}

// Get the foreground colour of additional carets.
wxColour wxScintilla::GetAdditionalCaretFore() const
{
    long c = SendMsg(SCI_GETADDITIONALCARETFORE, 0, 0);
    return wxColourFromLong(c);
}

// Set the main selection to the next selection.
void wxScintilla::RotateSelection()
{
    SendMsg(SCI_ROTATESELECTION, 0, 0);
}

// Swap that caret and anchor of the main selection.
void wxScintilla::SwapMainAnchorCaret()
{
    SendMsg(SCI_SWAPMAINANCHORCARET, 0, 0);
}

wxSciFnDirect wxScintilla::GetDirectFunction()
{
    return (wxSciFnDirect)SendMsg(SCI_GETDIRECTFUNCTION , 0, 0);
}

wxIntPtr wxScintilla::GetDirectPointer() const
{
    return (wxIntPtr)SendMsg(SCI_GETDIRECTPOINTER , 0, 0);
}

wxUIntPtr wxScintilla::GetCharacterPointer() const
{
    return (wxUIntPtr)SendMsg(SCI_GETCHARACTERPOINTER , 0, 0);
}

void wxScintilla::GrabSCIFocus()
{
    SendMsg(SCI_GRABFOCUS , 0, 0);
}

void wxScintilla::LoadLexerLibrary(const wxString& path)
{
    SendMsg(SCI_LOADLEXERLIBRARY , 0, (sptr_t)(const char*)wx2sci(path));
}

void wxScintilla::SetUsePalette(bool allowPaletteUse)
{
    SendMsg(SCI_SETUSEPALETTE ,allowPaletteUse, 0);
}

bool wxScintilla::GetUsePalette()
{
    return SendMsg(SCI_GETUSEPALETTE , 0, 0) != 0;
}


// Start notifying the container of all key presses and commands.
void wxScintilla::StartRecord ()
{
    SendMsg(SCI_STARTRECORD, 0, 0);
}

// Stop notifying the container of all key presses and commands.
void wxScintilla::StopRecord ()
{
    SendMsg(SCI_STOPRECORD, 0, 0);
}

// Set the lexing language of the document.
void wxScintilla::SetLexer (int lexer)
{
    SendMsg(SCI_SETLEXER, lexer, 0);
}

// Retrieve the lexing language of the document.
int wxScintilla::GetLexer () const
{
    return SendMsg(SCI_GETLEXER, 0, 0);
}

// Colourise a segment of the document using the current lexing language.
void wxScintilla::Colourise (int start, int end)
{
    SendMsg(SCI_COLOURISE, start, end);
}

// Set up a value that may be used by a lexer for some optional feature.
void wxScintilla::SetProperty (const wxString& key, const wxString& value)
{
    SendMsg(SCI_SETPROPERTY, (sptr_t)(const char*)wx2sci(key), (sptr_t)(const char*)wx2sci(value));
}

// Set up the key words used by the lexer.
void wxScintilla::SetKeyWords(int keywordSet, const wxString& keyWords)
{
    SendMsg(SCI_SETKEYWORDS, keywordSet, (sptr_t)(const char*)wx2sci(keyWords));
}

// Set the lexing language of the document based on string name.
void wxScintilla::SetLexerLanguage(const wxString& language)
{
    SendMsg(SCI_SETLEXERLANGUAGE, 0, (sptr_t)(const char*)wx2sci(language));
}

// Retrieve a 'property' value previously set with SetProperty.
wxString wxScintilla::GetProperty (const wxString& key)
{
    int len = SendMsg(SCI_GETPROPERTY, (sptr_t)(const char*)wx2sci(key), 0);
    if (!len) return wxEmptyString;

    wxMemoryBuffer mbuf(len+1);
    char* buf = (char*)mbuf.GetWriteBuf(len+1);
    SendMsg(SCI_GETPROPERTY, (uptr_t)(const char*)wx2sci(key), (sptr_t)buf);
    mbuf.UngetWriteBuf(len);
    mbuf.AppendByte(0);
    return sci2wx(buf);
}

// Retrieve a 'property' value previously set with SetProperty,
// with '$()' variable replacement on returned buffer.
wxString wxScintilla::GetPropertyExpanded (const wxString& key)
{
    int len = SendMsg(SCI_GETPROPERTYEXPANDED, (uptr_t)(const char*)wx2sci(key), 0);
    if (!len) return wxEmptyString;

    wxMemoryBuffer mbuf(len+1);
    char* buf = (char*)mbuf.GetWriteBuf(len+1);
    SendMsg(SCI_GETPROPERTYEXPANDED, (uptr_t)(const char*)wx2sci(key), (uptr_t)buf);
    mbuf.UngetWriteBuf(len);
    mbuf.AppendByte(0);
    return sci2wx(buf);
}

// Retrieve a 'property' value previously set with SetProperty,
// interpreted as an int AFTER any '$()' variable replacement.
int wxScintilla::GetPropertyInt (const wxString& key) const
{
    return SendMsg(SCI_GETPROPERTYINT, (sptr_t)(const char*)wx2sci(key), 0);
}

// Retrieve the number of bits the current lexer needs for styling.
int wxScintilla::GetStyleBitsNeeded () const
{
    return SendMsg(SCI_GETSTYLEBITSNEEDED, 0, 0);
}

// Retrieve the name of the lexer.
wxString wxScintilla::GetLexerLanguage() const
{
    int len (0);

    // determine the lexers language string length
    len = SendMsg(SCI_GETLEXERLANGUAGE, 0, 0);
    if (!len) return wxEmptyString;

    wxMemoryBuffer mbuf(len+2);
    char* buf = (char*)mbuf.GetWriteBuf(len+1);
    SendMsg(SCI_GETLEXERLANGUAGE, 0, (sptr_t)buf);
    mbuf.UngetWriteBuf(len);
    mbuf.AppendByte(0);
    return sci2wx(buf);
}

// END of generated section
//----------------------------------------------------------------------


// Returns the line number of the line with the caret.
int wxScintilla::GetCurrentLine ()
{
    int line = LineFromPosition(GetCurrentPos());
    return line;
}


// Extract style settings from a spec-string which is composed of one or
// more of the following comma separated elements:
//
//      bold                    turns on bold
//      italic                  turns on italics
//      fore:[name or #RRGGBB]  sets the foreground colour
//      back:[name or #RRGGBB]  sets the background colour
//      face:[facename]         sets the font face name to use
//      size:[num]              sets the font size in points
//      eol                     turns on eol filling
//      underline               turns on underlining
//
void wxScintilla::StyleSetSpec (int styleNum, const wxString& spec)
{
    wxStringTokenizer tkz(spec, wxT(","));
    while (tkz.HasMoreTokens()) {
        wxString token = tkz.GetNextToken();

        wxString option = token.BeforeFirst(':');
        wxString val = token.AfterFirst(':');

        if (option == wxT("bold"))
            StyleSetBold(styleNum, true);

        else if (option == wxT("italic"))
            StyleSetItalic(styleNum, true);

        else if (option == wxT("underline"))
            StyleSetUnderline(styleNum, true);

        else if (option == wxT("eol"))
            StyleSetEOLFilled(styleNum, true);

        else if (option == wxT("size")) {
            long points;
            if (val.ToLong(&points))
                StyleSetSize(styleNum, points);
        }

        else if (option == wxT("face"))
            StyleSetFaceName(styleNum, val);

        else if (option == wxT("fore"))
            StyleSetForeground(styleNum, wxColourFromSpec(val));

        else if (option == wxT("back"))
            StyleSetBackground(styleNum, wxColourFromSpec(val));
    }
}


// Get the font of a style
wxFont wxScintilla::StyleGetFont(int style)
{
    wxFont font;
    font.SetPointSize(StyleGetSize(style));
    font.SetFaceName(StyleGetFaceName(style));
    if( StyleGetBold(style) )
        font.SetWeight(wxFONTWEIGHT_BOLD);
    else
        font.SetWeight(wxFONTWEIGHT_NORMAL);

    if( StyleGetItalic(style) )
        font.SetStyle(wxFONTSTYLE_ITALIC);
    else
        font.SetStyle(wxFONTSTYLE_NORMAL);

    return font;
}


// Set style size, face, bold, italic, and underline attributes from
// a wxFont's attributes.
void wxScintilla::StyleSetFont (int styleNum, wxFont& font)
{
#ifdef __WXGTK__
    // Ensure that the native font is initialized
    int x, y;
    GetTextExtent(wxT("X"), &x, &y, NULL, NULL, &font);
#endif
    int            size     = font.GetPointSize();
    wxString       faceName = font.GetFaceName();
    bool           bold     = font.GetWeight() == wxBOLD;
    bool           italic   = font.GetStyle() == wxITALIC;
    bool           under    = font.GetUnderlined();
    wxFontEncoding encoding = font.GetEncoding();

    StyleSetFontAttr(styleNum, size, faceName, bold, italic, under, encoding);
}

// Set all font style attributes at once.
void wxScintilla::StyleSetFontAttr (int styleNum, int size,
                                    const wxString& faceName,
                                    bool bold, bool italic,
                                    bool underline,
                                    wxFontEncoding encoding)
{
    StyleSetSize(styleNum, size);
    StyleSetFaceName(styleNum, faceName);
    StyleSetBold(styleNum, bold);
    StyleSetItalic(styleNum, italic);
    StyleSetUnderline(styleNum, underline);
    StyleSetFontEncoding(styleNum, encoding);
}


// Set the character set of the font in a style.  Converts the Scintilla
// character set values to a wxFontEncoding.
void wxScintilla::StyleSetCharacterSet (int style, int characterSet)
{
    wxFontEncoding encoding;

    // Translate the Scintilla characterSet to a wxFontEncoding
    switch (characterSet) {
        default:
        case wxSCI_CHARSET_ANSI:
        case wxSCI_CHARSET_DEFAULT:
            encoding = wxFONTENCODING_DEFAULT;
            break;

        case wxSCI_CHARSET_BALTIC:
            encoding = wxFONTENCODING_ISO8859_13;
            break;

        case wxSCI_CHARSET_CHINESEBIG5:
            encoding = wxFONTENCODING_CP950;
            break;

        case wxSCI_CHARSET_EASTEUROPE:
            encoding = wxFONTENCODING_ISO8859_2;
            break;

        case wxSCI_CHARSET_GB2312:
            encoding = wxFONTENCODING_CP936;
            break;

        case wxSCI_CHARSET_GREEK:
            encoding = wxFONTENCODING_ISO8859_7;
            break;

        case wxSCI_CHARSET_HANGUL:
            encoding = wxFONTENCODING_CP949;
            break;

        case wxSCI_CHARSET_MAC:
            encoding = wxFONTENCODING_DEFAULT;
            break;

        case wxSCI_CHARSET_OEM:
            encoding = wxFONTENCODING_DEFAULT;
            break;

        case wxSCI_CHARSET_RUSSIAN:
            encoding = wxFONTENCODING_KOI8;
            break;

        case wxSCI_CHARSET_SHIFTJIS:
            encoding = wxFONTENCODING_CP932;
            break;

        case wxSCI_CHARSET_SYMBOL:
            encoding = wxFONTENCODING_DEFAULT;
            break;

        case wxSCI_CHARSET_TURKISH:
            encoding = wxFONTENCODING_ISO8859_9;
            break;

        case wxSCI_CHARSET_JOHAB:
            encoding = wxFONTENCODING_DEFAULT;
            break;

        case wxSCI_CHARSET_HEBREW:
            encoding = wxFONTENCODING_ISO8859_8;
            break;

        case wxSCI_CHARSET_ARABIC:
            encoding = wxFONTENCODING_ISO8859_6;
            break;

        case wxSCI_CHARSET_VIETNAMESE:
            encoding = wxFONTENCODING_DEFAULT;
            break;

        case wxSCI_CHARSET_THAI:
            encoding = wxFONTENCODING_ISO8859_11;
            break;

        case wxSCI_CHARSET_CYRILLIC:
            encoding = wxFONTENCODING_ISO8859_5;
            break;

        case wxSCI_CHARSET_8859_15:
            encoding = wxFONTENCODING_ISO8859_15;
            break;
    }

    // We just have Scintilla track the wxFontEncoding for us.  It gets used
    // in Font::Create in PlatWX.cpp.  We add one to the value so that the
    // effective wxFONENCODING_DEFAULT == SC_SHARSET_DEFAULT and so when
    // Scintilla internally uses SC_CHARSET_DEFAULT we will translate it back
    // to wxFONENCODING_DEFAULT in Font::Create.
    SendMsg(SCI_STYLESETCHARACTERSET, style, encoding+1);
}


// Set the font encoding to be used by a style.
void wxScintilla::StyleSetFontEncoding(int style, wxFontEncoding encoding)
{
    SendMsg(SCI_STYLESETCHARACTERSET, style, encoding+1);
}


// Perform one of the operations defined by the wxSCI_CMD_* constants.
void wxScintilla::CmdKeyExecute (int cmd)
{
    SendMsg(cmd);
}


// Set the left and right margin in the edit area, measured in pixels.
void wxScintilla::SetMargins (int left, int right)
{
    SetMarginLeft(left);
    SetMarginRight(right);
}


// Retrieve the point in the window where a position is displayed.
wxPoint wxScintilla::PointFromPosition(int pos)
{
    int x = SendMsg(SCI_POINTXFROMPOSITION, 0, pos);
    int y = SendMsg(SCI_POINTYFROMPOSITION, 0, pos);
    return wxPoint(x, y);
}

// Retrieve the start and end positions of the current selection.
void wxScintilla::GetSelection (long *from, long *to)
{
    if ( from )
        *from = GetSelectionStart();
    if ( to )
        *to = GetSelectionEnd();
}

// kept for compatibility only
//void wxScintilla::GetSelection(int *from, int *to)
//{
//    long f, t;
//    GetSelection(&f, &t);
//    if ( from )
//        *from = f;
//    if ( to )
//        *to = t;
//}

// Scroll enough to make the given line visible
void wxScintilla::ScrollToLine (int line)
{
    m_swx->DoScrollToLine(line);
}


// Scroll enough to make the given column visible
void wxScintilla::ScrollToColumn (int column)
{
    m_swx->DoScrollToColumn(column);
}


bool wxScintilla::SaveFile (const wxString& filename)
{
    wxFile file(filename, wxFile::write);

    if (!file.IsOpened())
        return false;

    bool success = file.Write(GetText(), *wxConvCurrent);

    file.Close();

    if (success)
        SetSavePoint();

    return success;
}

bool wxScintilla::LoadFile (const wxString& filename)
{
    bool success = false;
    wxFile file(filename, wxFile::read);

    if (file.IsOpened())
    {
        wxString contents;
        // get the file size (assume it is not huge file...)
        ssize_t len = (ssize_t)file.Length();

        if (len > 0)
        {
#if wxUSE_UNICODE
            wxMemoryBuffer buffer(len+1);
            success = (file.Read(buffer.GetData(), len) == len);
            if (success) {
                ((char*)buffer.GetData())[len] = 0;
                contents = wxString(buffer, *wxConvCurrent, len);
            }
#else
            wxString buffer;
            success = (file.Read(wxStringBuffer(buffer, len), len) == len);
            contents = buffer;
#endif
        }
        else
        {
            if (len == 0)
                success = true;  // empty file is ok
            else
                success = false; // len == wxInvalidOffset
        }

        file.Close();

        if (success)
        {
            SetText(contents);
            EmptyUndoBuffer();
            SetSavePoint();
        }
    }

    return success;
}


#if wxUSE_DRAG_AND_DROP
wxDragResult wxScintilla::DoDragOver (wxCoord x, wxCoord y, wxDragResult def)
{
    return m_swx->DoDragOver(x, y, def);
}


bool wxScintilla::DoDropText (long x, long y, const wxString& data)
{
    return m_swx->DoDropText(x, y, data);
}

wxDragResult wxScintilla::DoDragEnter (wxCoord x, wxCoord y, wxDragResult def)
{
    return m_swx->DoDragOver (x, y, def);
}

void wxScintilla::DoDragLeave ()
{
    m_swx->DoDragLeave ();
}
#endif


// Raw text handling for UTF-8


void wxScintilla::AddTextRaw (const char* text)
{
    SendMsg(SCI_ADDTEXT, strlen(text), (sptr_t)text);
}

void wxScintilla::InsertTextRaw (int pos, const char* text)
{
    SendMsg(SCI_INSERTTEXT, pos, (sptr_t)text);
}

wxCharBuffer wxScintilla::GetCurLineRaw (int* linePos)
{
    int len = LineLength(GetCurrentLine());
    if (!len) {
        if (linePos)  *linePos = 0;
        wxCharBuffer empty;
        return empty;
    }

    wxCharBuffer buf(len);
    int pos = SendMsg(SCI_GETCURLINE, len, (sptr_t)buf.data());
    if (linePos)  *linePos = pos;
    return buf;
}

wxCharBuffer wxScintilla::GetLineRaw (int line)
{
    int len = LineLength(line);
    if (!len) {
        wxCharBuffer empty;
        return empty;
    }

    wxCharBuffer buf(len);
    SendMsg(SCI_GETLINE, line, (sptr_t)buf.data());
    return buf;
}

wxCharBuffer wxScintilla::GetSelectedTextRaw()
{
/* C::B begin */
    /* Notice that this function used to use 'GetSelection(&start, &end);'
     * to calculate the selected text length - which works fine when using
     * simple selection, but when using multiple selection, or even rectangular
     * selection it may cause a crash. */
    int len (0);

    // determine the selected text range
    len = SendMsg(SCI_GETSELTEXT, 0, 0);
/* C::B end */
    if (!len) {
        wxCharBuffer empty;
        return empty;
    }

    wxCharBuffer buf(len);
    SendMsg(SCI_GETSELTEXT, 0, (sptr_t)buf.data());
    return buf;
}

wxCharBuffer wxScintilla::GetTextRangeRaw (int startPos, int endPos)
{
    if (endPos < startPos) {
        int temp = startPos;
        startPos = endPos;
        endPos = temp;
    }
    int len  = endPos - startPos;
    if (!len) {
        wxCharBuffer empty;
        return empty;
    }

    wxCharBuffer buf(len);
    TextRange tr;
    tr.lpstrText = buf.data();
    tr.chrg.cpMin = startPos;
    tr.chrg.cpMax = endPos;
    SendMsg(SCI_GETTEXTRANGE, 0, (sptr_t)&tr);
    return buf;
}

void wxScintilla::SetTextRaw (const char* text)
{
    SendMsg(SCI_SETTEXT, 0, (sptr_t)text);
}

wxCharBuffer wxScintilla::GetTextRaw()
{
    int len = GetTextLength();
    wxCharBuffer buf(len); // adds 1 for NUL automatically
    SendMsg(SCI_GETTEXT, len + 1, (sptr_t)buf.data());
    return buf;
}

void wxScintilla::AppendTextRaw (const char* text)
{
    SendMsg(SCI_APPENDTEXT, strlen(text), (sptr_t)text);
}





//----------------------------------------------------------------------
// Event handlers

void wxScintilla::OnPaint (wxPaintEvent& WXUNUSED(evt))
{
#ifdef __WXGTK__
    // avoid flickering, thanks Eran for the patch
	// On Mac / Windows there is no real need for this
    wxBufferedPaintDC dc(this);
#else
    wxPaintDC dc(this);
#endif
    m_swx->DoPaint(&dc, GetUpdateRegion().GetBox());
}

void wxScintilla::OnScrollWin (wxScrollWinEvent& evt)
{
    if (evt.GetOrientation() == wxHORIZONTAL)
        m_swx->DoHScroll(evt.GetEventType(), evt.GetPosition());
    else
        m_swx->DoVScroll(evt.GetEventType(), evt.GetPosition());
}

void wxScintilla::OnScroll (wxScrollEvent& evt)
{
    wxScrollBar* sb = wxDynamicCast(evt.GetEventObject(), wxScrollBar);
    if (sb) {
        if (sb->IsVertical())
            m_swx->DoVScroll(evt.GetEventType(), evt.GetPosition());
        else
            m_swx->DoHScroll(evt.GetEventType(), evt.GetPosition());
    }
}

void wxScintilla::OnSize (wxSizeEvent& WXUNUSED(evt))
{
    if (m_swx) {
        wxSize sz = GetClientSize();
        m_swx->DoSize(sz.x, sz.y);
    }
}

void wxScintilla::OnMouseLeftDown (wxMouseEvent& evt)
{
    SetFocus();
    wxPoint pt = evt.GetPosition();
    m_swx->DoLeftButtonDown(Point(pt.x, pt.y), m_stopWatch.Time(),
                      evt.ShiftDown(), evt.ControlDown(), evt.AltDown());
}

void wxScintilla::OnMouseMove (wxMouseEvent& evt)
{
    wxPoint pt = evt.GetPosition();
    m_swx->DoLeftButtonMove(Point(pt.x, pt.y));
}

void wxScintilla::OnMouseLeftUp (wxMouseEvent& evt)
{
    wxPoint pt = evt.GetPosition();
    m_swx->DoLeftButtonUp(Point(pt.x, pt.y), m_stopWatch.Time(),
                      evt.ControlDown());
}


void wxScintilla::OnMouseRightUp (wxMouseEvent& evt)
{
    wxPoint pt = evt.GetPosition();
    m_swx->DoContextMenu(Point(pt.x, pt.y));
}


void wxScintilla::OnMouseMiddleUp (wxMouseEvent& evt)
{
    wxPoint pt = evt.GetPosition();
    m_swx->DoMiddleButtonUp(Point(pt.x, pt.y));
}

void wxScintilla::OnContextMenu (wxContextMenuEvent& evt)
{
    wxPoint pt = evt.GetPosition();
    ScreenToClient(&pt.x, &pt.y);
    /*
      Show context menu at event point if it's within the window,
      or at caret location if not
    */
    wxHitTest ht = this->HitTest(pt);
    if (ht != wxHT_WINDOW_INSIDE) {
        pt = this->PointFromPosition(this->GetCurrentPos());
    }
    m_swx->DoContextMenu(Point(pt.x, pt.y));
}


void wxScintilla::OnMouseWheel (wxMouseEvent& evt)
{
    // prevent having an event queue with wheel events that cannot be processed
    // reasonably fast (see ticket #9057)
    if ( m_lastWheelTimestamp <= evt.GetTimestamp() )
    {
        m_lastWheelTimestamp = m_stopWatch.Time();
        m_swx->DoMouseWheel(evt.GetWheelRotation(),
                            evt.GetWheelDelta(),
                            evt.GetLinesPerAction(),
                            evt.ControlDown(),
                            evt.IsPageScroll());
        m_lastWheelTimestamp = m_stopWatch.Time() - m_lastWheelTimestamp;
        m_lastWheelTimestamp += evt.GetTimestamp();
    }
}


void wxScintilla::OnChar (wxKeyEvent& evt)
{
    // On (some?) non-US PC keyboards the AltGr key is required to enter some
    // common characters.  It comes to us as both Alt and Ctrl down so we need
    // to let the char through in that case, otherwise if only ctrl or only
    // alt let's skip it.
    bool ctrl = evt.ControlDown();
#ifdef __WXMAC__
    // On the Mac the Alt key is just a modifier key (like Shift) so we need
    // to allow the char events to be processed when Alt is pressed.
    // TODO:  Should we check MetaDown instead in this case?
    bool alt = false;
#else
    bool alt  = evt.AltDown();
#endif
    bool skip = ((ctrl || alt) && ! (ctrl && alt));

#if wxUSE_UNICODE
    // apparently if we don't do this, Unicode keys pressed after non-char
    // ASCII ones (e.g. Enter, Tab) are not taken into account (patch 1615989)
    if (m_lastKeyDownConsumed && evt.GetUnicodeKey() > 255)
        m_lastKeyDownConsumed = false;
#endif

    if (!m_lastKeyDownConsumed && !skip) {
#if wxUSE_UNICODE
        int key = evt.GetUnicodeKey();
        bool keyOk = true;

        // if the unicode key code is not really a unicode character (it may
        // be a function key or etc., the platforms appear to always give us a
        // small value in this case) then fallback to the ascii key code but
        // don't do anything for function keys or etc.
        if (key <= 127) {
            key = evt.GetKeyCode();
            keyOk = (key <= 127);
        }
        if (keyOk) {
            m_swx->DoAddChar(key);
            return;
        }
#else
        int key = evt.GetKeyCode();
        if ( (key <= WXK_START) || (key > WXK_COMMAND) ) {
            m_swx->DoAddChar(key);
            return;
        }
#endif
    }

    evt.Skip();
}


void wxScintilla::OnKeyDown (wxKeyEvent& evt)
{
    int processed = m_swx->DoKeyDown(evt, &m_lastKeyDownConsumed);
    if (!processed && !m_lastKeyDownConsumed)
        evt.Skip();
}


void wxScintilla::OnLoseFocus (wxFocusEvent& evt)
{
    m_swx->DoLoseFocus();
    evt.Skip();
}


void wxScintilla::OnGainFocus (wxFocusEvent& evt)
{
    m_swx->DoGainFocus();
    evt.Skip();
}


void wxScintilla::OnSysColourChanged (wxSysColourChangedEvent& WXUNUSED(evt))
{
    m_swx->DoSysColourChange();
}


void wxScintilla::OnEraseBackground (wxEraseEvent& WXUNUSED(evt))
{
    // do nothing to help avoid flashing
}



void wxScintilla::OnMenu (wxCommandEvent& evt)
{
    m_swx->DoCommand(evt.GetId());
}


void wxScintilla::OnListBox (wxCommandEvent& WXUNUSED(evt))
{
    m_swx->DoOnListBox();
}


void wxScintilla::OnIdle (wxIdleEvent& evt)
{
    m_swx->DoOnIdle(evt);
}


wxSize wxScintilla::DoGetBestSize() const
{
    // What would be the best size for a wxSintilla?
    // Just give a reasonable minimum until something else can be figured out.
    return wxSize(600,440);
}


//----------------------------------------------------------------------
// Turn notifications from Scintilla into events

void wxScintilla::NotifyFocus(bool focus)
{
    wxScintillaEvent evt (focus ? wxEVT_SCI_SETFOCUS : wxEVT_SCI_KILLFOCUS, GetId());
    evt.SetEventObject(this);
    GetEventHandler()->ProcessEvent(evt);
}

void wxScintilla::NotifyChange()
{
    wxScintillaEvent evt (wxEVT_SCI_CHANGE, GetId());
    evt.SetEventObject(this);
    GetEventHandler()->ProcessEvent(evt);
}


static void SetEventText (wxScintillaEvent& evt, const char* text,
                          size_t length)
{
    if(!text) return;

    evt.SetText(sci2wx(text, length));
}


void wxScintilla::NotifyParent (SCNotification* _scn)
{
    SCNotification& scn = *_scn;
    wxScintillaEvent evt (0, GetId());

    evt.SetEventObject(this);
    evt.SetPosition(scn.position);
    evt.SetKey(scn.ch);
    evt.SetModifiers(scn.modifiers);

    switch (scn.nmhdr.code) {
    case SCN_STYLENEEDED:
        evt.SetEventType (wxEVT_SCI_STYLENEEDED);
        break;

    case SCN_CHARADDED:
        evt.SetEventType (wxEVT_SCI_CHARADDED);
        break;

    case SCN_SAVEPOINTREACHED:
        evt.SetEventType (wxEVT_SCI_SAVEPOINTREACHED);
        break;

    case SCN_SAVEPOINTLEFT:
        evt.SetEventType (wxEVT_SCI_SAVEPOINTLEFT);
        break;

    case SCN_MODIFYATTEMPTRO:
        evt.SetEventType (wxEVT_SCI_ROMODIFYATTEMPT);
        break;

    case SCN_KEY:
        evt.SetEventType (wxEVT_SCI_KEY);
        break;

    case SCN_DOUBLECLICK:
        evt.SetEventType (wxEVT_SCI_DOUBLECLICK);
        break;

    case SCN_UPDATEUI:
        evt.SetEventType (wxEVT_SCI_UPDATEUI);
        break;

    case SCN_MODIFIED:
        evt.SetEventType (wxEVT_SCI_MODIFIED);
        evt.SetModificationType(scn.modificationType);
        SetEventText(evt, scn.text, scn.length);
        evt.SetLength(scn.length);
        evt.SetLinesAdded(scn.linesAdded);
        evt.SetLine(scn.line);
        evt.SetFoldLevelNow(scn.foldLevelNow);
        evt.SetFoldLevelPrev(scn.foldLevelPrev);
        break;

    case SCN_MACRORECORD:
        evt.SetEventType (wxEVT_SCI_MACRORECORD);
        evt.SetMessage(scn.message);
        evt.SetWParam(scn.wParam);
        evt.SetLParam(scn.lParam);
        break;

    case SCN_MARGINCLICK:
        evt.SetEventType (wxEVT_SCI_MARGINCLICK);
        evt.SetMargin(scn.margin);
        break;

    case SCN_NEEDSHOWN:
        evt.SetEventType (wxEVT_SCI_NEEDSHOWN);
        evt.SetLength(scn.length);
        break;

    case SCN_PAINTED:
        evt.SetEventType (wxEVT_SCI_PAINTED);
        break;

    case SCN_AUTOCSELECTION:
        evt.SetEventType(wxEVT_SCI_AUTOCOMP_SELECTION);
        evt.SetListType(scn.listType);
        SetEventText(evt, scn.text, strlen(scn.text));
        evt.SetPosition(scn.lParam);
        break;

    case SCN_AUTOCCANCELLED:
        evt.SetEventType(wxEVT_SCI_AUTOCOMP_CANCELLED);
        break;

    case SCN_AUTOCCHARDELETED:
        evt.SetEventType(wxEVT_SCI_AUTOCOMP_CHARDELETED);
        break;

    case SCN_USERLISTSELECTION:
        evt.SetEventType(wxEVT_SCI_USERLISTSELECTION);
        evt.SetListType(scn.listType);
        SetEventText(evt, scn.text, strlen(scn.text));
        evt.SetPosition(scn.lParam);
        break;

    case SCN_URIDROPPED:
        evt.SetEventType (wxEVT_SCI_URIDROPPED);
        SetEventText(evt, scn.text, strlen(scn.text));
        break;

    case SCN_DWELLSTART:
        evt.SetEventType (wxEVT_SCI_DWELLSTART);
        evt.SetX(scn.x);
        evt.SetY(scn.y);
        break;

    case SCN_DWELLEND:
        evt.SetEventType (wxEVT_SCI_DWELLEND);
        evt.SetX(scn.x);
        evt.SetY(scn.y);
        break;

    case SCN_ZOOM:
        evt.SetEventType (wxEVT_SCI_ZOOM);
        break;

    case SCN_HOTSPOTCLICK:
        evt.SetEventType (wxEVT_SCI_HOTSPOT_CLICK);
        break;

    case SCN_HOTSPOTDOUBLECLICK:
        evt.SetEventType (wxEVT_SCI_HOTSPOT_DCLICK);
        break;

    case SCN_CALLTIPCLICK:
        evt.SetEventType (wxEVT_SCI_CALLTIP_CLICK);
        break;

    case SCN_INDICATORCLICK:
        evt.SetEventType (wxEVT_SCI_INDICATOR_CLICK);
        break;

    case SCN_INDICATORRELEASE:
        evt.SetEventType (wxEVT_SCI_INDICATOR_RELEASE);
        break;

    default:
        return;
    }

    GetEventHandler()->ProcessEvent(evt);
}


//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------

wxScintillaEvent::wxScintillaEvent (wxEventType commandType, int id)
    : wxCommandEvent(commandType, id)
{
    m_position = 0;
    m_key = 0;
    m_modifiers = 0;
    m_modificationType = 0;
    m_length = 0;
    m_linesAdded = 0;
    m_line = 0;
    m_foldLevelNow = 0;
    m_foldLevelPrev = 0;
    m_margin = 0;
    m_message = 0;
    m_wParam = 0;
    m_lParam = 0;
    m_listType = 0;
    m_x = 0;
    m_y = 0;
/* C::B begin */
    m_dragAllowMove = wxDrag_CopyOnly;
/* C::B end */
#if wxUSE_DRAG_AND_DROP
    m_dragResult = wxDragNone;
#endif
}

bool wxScintillaEvent::GetShift() const { return (m_modifiers & SCI_SHIFT) != 0; }
bool wxScintillaEvent::GetControl() const { return (m_modifiers & SCI_CTRL) != 0; }
bool wxScintillaEvent::GetAlt() const { return (m_modifiers & SCI_ALT) != 0; }


wxScintillaEvent::wxScintillaEvent (const wxScintillaEvent& event):
  wxCommandEvent(event)
{
    m_position          = event.m_position;
    m_key               = event.m_key;
    m_modifiers         = event.m_modifiers;
    m_modificationType  = event.m_modificationType;
    m_text              = event.m_text;
    m_length            = event.m_length;
    m_linesAdded        = event.m_linesAdded;
    m_line              = event.m_line;
    m_foldLevelNow      = event.m_foldLevelNow;
    m_foldLevelPrev     = event.m_foldLevelPrev;

    m_margin            = event.m_margin;

    m_message           = event.m_message;
    m_wParam            = event.m_wParam;
    m_lParam            = event.m_lParam;

    m_listType          = event.m_listType;
    m_x                 = event.m_x;
    m_y                 = event.m_y;

    m_dragText          = event.m_dragText;
    m_dragAllowMove     = event.m_dragAllowMove;
#if wxUSE_DRAG_AND_DROP
    m_dragResult        = event.m_dragResult;
#endif
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------

