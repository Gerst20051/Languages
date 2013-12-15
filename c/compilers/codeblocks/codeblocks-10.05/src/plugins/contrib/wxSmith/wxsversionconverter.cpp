/*
* This file is part of wxSmith plugin for Code::Blocks Studio
* Copyright (C) 2006-2007  Bartlomiej Swiecki
*
* wxSmith is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* wxSmith is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with wxSmith. If not, see <http://www.gnu.org/licenses/>.
*
* $Revision: 4850 $
* $Id: wxsversionconverter.cpp 4850 2008-01-29 21:45:49Z byo $
* $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/wxSmith/wxsversionconverter.cpp $
*/

#include "wxsversionconverter.h"
#include "wxsproject.h"
#include "wxscoder.h"

#include <globals.h>
#include <wx/string.h>
#include <tinyxml/tinywxuni.h>

// TODO: This is same as in wxsproject.h, make one set instead of two
namespace
{
    const int CurrentVersion = 1;
    const char* CurrentVersionStr = "1";
}

const wxsVersionConverter& wxsVersionConverter::Get()
{
    static wxsVersionConverter Singleton;
    return Singleton;
}

bool wxsVersionConverter::DetectOldConfig(TiXmlElement* Node,wxsProject* Project) const
{
    // New wxSmith style has resources put into <resources> node
    // and configuration to <gui> node
    if ( Node->FirstChildElement("dialog")        ) return true;
    if ( Node->FirstChildElement("frame")         ) return true;
    if ( Node->FirstChildElement("panel")         ) return true;
    if ( Node->FirstChildElement("configuration") ) return true;

    return false;
}

TiXmlElement* wxsVersionConverter::ConvertFromOldConfig(TiXmlElement* ConfigNode,TiXmlDocument* Doc,wxsProject* Project) const
{
    if ( cbMessageBox(_("This project uses old wxSmith configuration format\n"
                        "Would you like me to convert to new one?\n"),
                      _("wxSmith: Converting from old format"),
                      wxYES_NO) != wxID_YES ) return 0;

    TiXmlElement* NewConfig = Doc->InsertEndChild(TiXmlElement("wxSmith"))->ToElement();
    TiXmlElement* Resources = NewConfig->InsertEndChild(TiXmlElement("resources"))->ToElement();
    NewConfig->SetAttribute("version",CurrentVersionStr);
    for ( TiXmlElement* Node = ConfigNode->FirstChildElement(); Node; Node = Node->NextSiblingElement() )
    {
        wxString NodeName = cbC2U(Node->Value());
        if ( NodeName == _T("configuration") )
        {
            const char* AppSrc  = Node->Attribute("app_src_file");
            const char* Main    = Node->Attribute("main_resource");
            const char* InitAll = Node->Attribute("init_all_handlers");

            if ( AppSrc )
            {
                TiXmlElement* GUINode = NewConfig->InsertEndChild(TiXmlElement("gui"))->ToElement();
                GUINode->SetAttribute("name","wxWidgets");
                GUINode->SetAttribute("src",AppSrc);
                GUINode->SetAttribute("main",Main?Main:"");
                GUINode->SetAttribute("init_handlers",InitAll?InitAll:"necessary");
                GUINode->SetAttribute("language","CPP");
            }
        }
        else
        {
            if ( NodeName == _T("dialog") ||
                 NodeName == _T("frame") ||
                 NodeName == _T("panel") )
            {
                const char* Wxs   = Node->Attribute("wxs_file");
                const char* Class = Node->Attribute("class");
                const char* Src   = Node->Attribute("src_file");
                const char* Hdr   = Node->Attribute("header_file");
                const char* Xrc   = Node->Attribute("xrc_file");
                const char* Mode  = Node->Attribute("edit_mode");

                if ( Wxs && Class && Src && Hdr && Mode )
                {
                    if ( cbC2U(Mode) == _T("Source") ) Xrc = 0;
                    TiXmlElement* Res = Resources->InsertEndChild(TiXmlElement(
                        NodeName == _T("dialog") ? "wxDialog" :
                        NodeName == _T("frame")  ? "wxFrame" :
                                                   "wxPanel" ))->ToElement();

                    Res->SetAttribute("wxs",cbU2C(_T("wxsmith/")+cbC2U(Wxs)));
                    Res->SetAttribute("src",Src);
                    Res->SetAttribute("hdr",Hdr);
                    if ( Xrc ) Res->SetAttribute("xrc",Xrc);
                    Res->SetAttribute("name",Class);
                    Res->SetAttribute("language","CPP");

                    ConvertOldWxsFile(Project->GetProjectPath()+_T("wxsmith/")+cbC2U(Wxs),Xrc!=0);
                    AdoptOldSourceFile(Project->GetProjectPath()+cbC2U(Src),cbC2U(Class));
                }
            }
        }
    }
    return NewConfig;
}

void wxsVersionConverter::ConvertOldWxsFile(const wxString& FileName,bool UsingXrc) const
{
    TiXmlDocument Doc;
    if ( !TinyXML::LoadDocument(FileName,&Doc) ) return;

    TiXmlElement* Smith = Doc.FirstChildElement("resource");
    if ( Smith )
    {
        Smith->SetValue("wxsmith");
    }

    if ( UsingXrc && Smith )
    {
        // Need to extract extra data from any resource's item and put into <resource_extra> node
        TiXmlElement* Resource = Smith->FirstChildElement("object");
        TiXmlElement* Extra = Smith->InsertEndChild(TiXmlElement("resource_extra"))->ToElement();
        GatherExtraFromOldResourceReq(Resource,Extra,true);
    }

    TinyXML::SaveDocument(FileName,&Doc);
}

void wxsVersionConverter::GatherExtraFromOldResourceReq(TiXmlElement* Object,TiXmlElement* Extra,bool Root) const
{
    // The only extra information in old wxSmith was:
    //  * variable / member attributes of <object> node
    //  * event handlers enteries
    // These fields are extracted and put into wxs file
    if ( !strcmp(Object->Value(),"object") )
    {
        if ( Object->Attribute("class") && (Root || Object->Attribute("name")) )
        {
            TiXmlElement* ThisExtra = 0;

            // Checking if we got variable name
            if ( Object->Attribute("variable") && Object->Attribute("member") )
            {
                ThisExtra = Extra->InsertEndChild(TiXmlElement("object"))->ToElement();
                ThisExtra->SetAttribute("variable",Object->Attribute("variable"));
                ThisExtra->SetAttribute("member",Object->Attribute("member"));
            }

            // Checking for event handlers

            for ( TiXmlElement* Handler = Object->FirstChildElement("handler"); Handler; Handler = Handler->NextSiblingElement("handler") )
            {
                if ( !ThisExtra )
                {
                    ThisExtra = Extra->InsertEndChild(TiXmlElement("object"))->ToElement();
                }
                ThisExtra->InsertEndChild(*Handler);
            }

            if ( ThisExtra )
            {
                if ( Root )
                {
                    ThisExtra->SetAttribute("root","1");
                }
                else
                {
                    ThisExtra->SetAttribute("name",Object->Attribute("name"));
                    ThisExtra->SetAttribute("class",Object->Attribute("class"));
                }
            }
        }
    }

    for ( TiXmlElement* Child = Object->FirstChildElement(); Child; Child = Child->NextSiblingElement() )
    {
        GatherExtraFromOldResourceReq(Child,Extra,false);
    }
}

void wxsVersionConverter::AdoptOldSourceFile(const wxString& FileName,const wxString& Class) const
{
    // Need to add two new sections: //(*InternalHeaders and //(*IdInit
    // to do this //(*InternalHeaders will be added before any source code
    // but after all  #xxx directives
    // //(*IdInit will be added just before //(*EventTable nearrest line
    // before that section containing BEGIN_EVENT_TABLE
    //
    // This may not be tricky enough but I hope that not much people mess
    // with code generated by wxSmith from templates ;)
    //
    // BTW we do not use wxsCodeMarks::Beg and wxsCodeMarks::End because that could
    // cause some problems with future conversion of these marks
    // (upgrade of old-wxSmith project will be done in two steps then,
    // first - convertion to version 1 of new wxsmith and then upgrading to
    // higher version so if convention of code marks will change, it will
    // break the conversion chain)

    bool IsInternalHeaders = wxsCoder::Get()->GetCode(
        FileName,
        _T("//(*InternalHeaders(") + Class + _T(")\n"),
        _T("//*)"),
        true,true).Length() != 0;

    bool IsIdInit = wxsCoder::Get()->GetCode(
        FileName,
        _T("//(*IdInit(") + Class + _T(")\n"),
        _T("//*)"),
        true,true).Length() != 0;

    if ( !IsInternalHeaders || !IsIdInit )
    {
        wxFontEncoding Encoding;
        bool UseBOM;
        wxString Content = wxsCoder::Get()->GetFullCode(FileName,Encoding,UseBOM);

        int Pos = 0;
        if ( !IsInternalHeaders )
        {
            while ( Pos<(int)Content.Len() && LineContainDirectivesOnly(Content,Pos) );

            wxString AddInternalHeaders =
                _T("//(*InternalHeaders(") + Class + _T(")\n")
                _T("//*)\n")
                _T("\n");

            Content = Content.Mid(0,Pos) + AddInternalHeaders + Content.Mid(Pos);
        }
        else
        {
            Pos = Content.Find(_T("//(*InternalHeaders(") + Class + _T(")\n"));
            int Shift = Content.Mid(Pos).Find(_T("//*)"));
            if ( Shift != wxNOT_FOUND )
            {
                Pos += Shift;
            }
        }

        if ( !IsIdInit )
        {
            int NewPos = Content.Find(_T("BEGIN_EVENT_TABLE(")+Class);
            if ( NewPos!=wxNOT_FOUND )
            {
                Pos = NewPos;
            }

            // Switching to first character in this line
            wxString Indent;
            while ( Pos>0 &&
                    Content.GetChar(Pos-1)!=_T('\n') &&
                    Content.GetChar(Pos-1)!=_T('\r') )
            {
                wxChar Ch = Content.GetChar(Pos--);
                Indent.Append((Ch==_T('\t'))?_T('\t'):_T(' '));
            }

            wxString AddIdInit =
                Indent + _T("//(*IdInit(") + Class + _T(")\n") +
                Indent + _T("//*)\n") +
                Indent + _T("\n");

            Content = Content.Mid(0,Pos) + AddIdInit + Content.Mid(Pos);
        }
        wxsCoder::Get()->PutFullCode(FileName,Content,Encoding,UseBOM);
    }
}

bool wxsVersionConverter::LineContainDirectivesOnly(const wxString& Code,int& BeginPos) const
{
    int Pos = BeginPos;

    wxChar PreviousChar = _T('\0');

    // Processing characters in this line
    while ( Pos < (int)Code.Len() )
    {
        wxChar Ch = Code.GetChar(Pos);
        if ( Ch==_T('/') && PreviousChar==_T('/') )
        {
            // We got // comment, skipping till the end of line
            while ( ++Pos < (int)Code.Len() )
            {
                PreviousChar = Ch;
                wxChar Ch = Code.GetChar(Pos);
                if ( Ch==_T('\n') || Ch==_T('\r') )
                {
                    if ( PreviousChar == _T('\\') )
                    {
                        // Backslash removes EOL
                        if ( ++Pos < (int)Code.Len() )
                        {
                            PreviousChar = Ch;
                            Ch = Code.GetChar(Pos);
                            if ( (Ch!=_T('\n') && Ch==_T('\r')) || (Ch==PreviousChar) )
                            {
                                // One-char EOL
                                --Pos;
                            }
                        }
                    }
                    else
                    {
                        // End-Of-Line and End-Of-Comment
                        while ( Pos<(int)Code.Len() )
                        {
                            wxChar Ch = Code.GetChar(Pos);
                            if ( Ch!=_T('\n') && Ch!=_T('\r') ) break;
                            Pos++;
                        }

                        BeginPos = Pos;
                        return true;
                    }
                }
            }
            // End of file approached
            BeginPos = Pos;
            return false;
        }
        else if ( Ch==_T('*') && PreviousChar==_T('/') )
        {
            // Starting multiline comment, we skip everything till */ sequence
            while ( ++Pos < (int)Code.Len() )
            {
                PreviousChar = Ch;
                wxChar Ch = Code.GetChar(Pos);

                if ( Ch==_T('/') && PreviousChar==_T('*') )
                {
                    // End of comment, breaking
                    break;
                }
            }

            if ( Pos >= (int)Code.Len() )
            {
                // Jumping out to skip another Pos++
                break;
            }
        }
        else if ( PreviousChar==_T('/') )
        {
            // Previous char was not comment beginning
            // need to rewind to it and jump out
            Pos--;
            break;
        }
        else if ( Ch!=_T(' ') && Ch!=_T('\t') )
        {
            if ( Ch!=_T('/') )
            {
                // No white char, jumping out to find out what's this
                break;
            }
            // If it's '/', it may be a start
            // of comment, we give it a try
        }
        PreviousChar = Ch;
        Pos++;
    }

    if ( Pos<(int)Code.Len() )
    {
        wxChar Ch = Code.GetChar(Pos);
        if ( (Ch!=_T('\n')) && (Ch!=_T('\r')) )
        {
            // This is no directive, jumping out of function
            if ( Ch != _T('#') ) return false;
        }
    }

    // Searching for EOL
    bool BlockMultilineComment = false;
    while ( Pos<(int)Code.Len() )
    {
        wxChar Ch = Code.GetChar(Pos);

        if ( Ch==_T('\n') || Ch==_T('\r') )
        {
            if ( PreviousChar == _T('\\') )
            {
                // Backslash removes EOL
                if ( ++Pos >= (int)Code.Len() )
                {
                    // EOF reached
                    BeginPos = Pos;
                    return false;
                }
                PreviousChar = Ch;
                Ch = Code.GetChar(Pos);
                if ( (Ch!=_T('\n') && Ch!=_T('\r')) || (Ch==PreviousChar) )
                {
                    // One-character EOL
                    Pos--;
                }
            }
            else
            {
                break;
            }
        }
        else if ( Ch==_T('/') && PreviousChar==_T('/') )
        {
            // Have to block any multiline comment in this line
            BlockMultilineComment = true;
        }
        else if ( Ch==_T('*') && PreviousChar==_T('/') && !BlockMultilineComment )
        {
            // Searching for end of comment and return there
            bool WasAnyNL = false;
            while ( ++Pos<(int)Code.Len() )
            {
                PreviousChar = Ch;
                Ch = Code.GetChar(Pos);
                if ( Ch==_T('/') && PreviousChar==_T('*') )
                {
                    // Comment has been finished
                    if ( WasAnyNL )
                    {
                        // If there was any NL in comment, this mean that
                        // we can start scanning new line here so we return
                        // with true
                        BeginPos = ++Pos;
                        return true;
                    }
                    else
                    {
                        // There was no NL in comment, we're still inside directive
                        break;
                    }
                }
                else if ( Ch==_T('\n') || Ch==_T('\r') )
                {
                    WasAnyNL = true;
                }
            }
        }
        PreviousChar = Ch;
        Pos++;
    }

    // Skipping all NL chars left
    while ( Pos<(int)Code.Len() )
    {
        wxChar Ch = Code.GetChar(Pos);
        if ( Ch!=_T('\n') && Ch!=_T('\r') ) break;
        Pos++;
    }

    BeginPos = Pos;
    return true;
}


TiXmlElement* wxsVersionConverter::Convert(TiXmlElement* ConfigNode,TiXmlDocument* Doc,wxsProject* Project) const
{
    // Currently there's only one version of wxSmith, no need to convert
    return 0;
}
