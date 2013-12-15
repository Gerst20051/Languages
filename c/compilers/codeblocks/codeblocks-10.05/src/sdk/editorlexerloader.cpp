/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision: 6032 $
 * $Id: editorlexerloader.cpp 6032 2010-01-03 12:41:45Z biplab $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/sdk/editorlexerloader.cpp $
 */

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include "globals.h"
    #include "manager.h"
    #include "logmanager.h"
    #include <wx/dynarray.h>
    #include <wx/regex.h>
    #include <wx/wxscintilla.h>
#endif

#include "editorcolourset.h"
#include "editorlexerloader.h"
#include "filemanager.h"

EditorLexerLoader::EditorLexerLoader(EditorColourSet* target)
    : m_pTarget(target)
{
	//ctor
}

EditorLexerLoader::~EditorLexerLoader()
{
	//dtor
}

void EditorLexerLoader::Load(LoaderBase* loader)
{
    Manager::Get()->GetLogManager()->Log(_("Loading ") + wxFileName(loader->FileName()).GetName());

    TiXmlDocument doc;
    doc.Parse(loader->GetData());

    if (doc.Error())
    {
        Manager::Get()->GetLogManager()->Log(_("Failed."));
        Manager::Get()->GetLogManager()->Log(_("TinyXML error: ") + cbC2U(doc.ErrorDesc()));
        return;
    }

    TiXmlElement* root;
    TiXmlElement* lexer;

    root = doc.FirstChildElement("CodeBlocks_lexer_properties");
    if (!root)
    {
        // old tag
        root = doc.FirstChildElement("Code::Blocks_lexer_properties");
        if (!root)
        {
            Manager::Get()->GetLogManager()->Log(_("Not a valid Code::Blocks lexer file..."));
            return;
        }
    }
    lexer = root->FirstChildElement("Lexer");
    if (!lexer)
    {
        Manager::Get()->GetLogManager()->Log(_("No 'Lexer' element in file..."));
        return;
    }

    DoLexer(lexer);
}

void EditorLexerLoader::DoLexer(TiXmlElement* node)
{
    if (!node->Attribute("name") || !node->Attribute("index"))
    {
    	Manager::Get()->GetLogManager()->Log(_("No name or index..."));
        return;
    }

    wxString name = wxString( node->Attribute("name"), wxConvUTF8 );
    int lexer = atol(node->Attribute("index"));
    wxString masks = wxString ( node->Attribute("filemasks"), wxConvUTF8 );
    HighlightLanguage style = m_pTarget->AddHighlightLanguage(lexer, name);
    if (style == HL_NONE)
        return; // wasn't added
    m_pTarget->SetFileMasks(style, masks);
//    LOGSTREAM << "Found lexer: " << name << " (" << style << ")\n";

    DoStyles(style, node);
    DoKeywords(style, node);
    DoSampleCode(style, node);
    DoLangAttributes(style, node);
}

void EditorLexerLoader::DoStyles(HighlightLanguage language, TiXmlElement* node)
{
    TiXmlElement* style = node->FirstChildElement("Style");
    while (style)
    {
        if (style->Attribute("name") && style->Attribute("index"))
        {
            wxString name = wxString ( style->Attribute("name"), wxConvUTF8 );
            wxString index = wxString ( style->Attribute("index"), wxConvUTF8 ); // comma-separated indices
            wxString fg = wxString ( style->Attribute("fg"), wxConvUTF8 );
            wxString bg = wxString ( style->Attribute("bg"), wxConvUTF8 );
            bool bold = style->Attribute("bold") ? atol(style->Attribute("bold")) != 0 : false;
            bool italics = style->Attribute("italics") ? atol(style->Attribute("italics")) != 0 : false;
            bool underlined = style->Attribute("underlined") ? atol(style->Attribute("underlined")) != 0 : false;

            // break-up arrays
            wxArrayString indices = GetArrayFromString(index, _T(","));
            wxArrayString fgarray = GetArrayFromString(fg, _T(","));
            wxArrayString bgarray = GetArrayFromString(bg, _T(","));

            wxColour fgcolour = wxNullColour;
            if (fgarray.GetCount() == 3)
            {
                long R=0, G=0, B=0;
                fgarray[0].ToLong(&R);
                fgarray[1].ToLong(&G);
                fgarray[2].ToLong(&B);
                fgcolour.Set((unsigned char)R,(unsigned char)G,(unsigned char)B);
            }
            wxColour bgcolour = wxNullColour;
            if (bgarray.GetCount() == 3)
            {
                long R=0, G=0, B=0;
                bgarray[0].ToLong(&R);
                bgarray[1].ToLong(&G);
                bgarray[2].ToLong(&B);
                bgcolour.Set((unsigned char)R,(unsigned char)G,(unsigned char)B);
            }

            for (size_t i = 0; i < indices.GetCount(); ++i)
            {
            	if (indices[i].IsEmpty())
                    continue;
                long value = 0;
                indices[i].ToLong(&value);
//                LOGSTREAM << _("Adding style: ") << name << _T("(") << value << _T(")\n");
                m_pTarget->AddOption(language, name, value,
                                    fgcolour,
                                    bgcolour,
                                    bold,
                                    italics,
                                    underlined,
                                    value >= 0);
            }
        }
        style = style->NextSiblingElement("Style");
    }
}

void EditorLexerLoader::DoKeywords(HighlightLanguage language, TiXmlElement* node)
{
    TiXmlElement* keywords = node->FirstChildElement("Keywords");
    if (!keywords)
        return;
    DoSingleKeywordNode(language, keywords, _T("Language"));
    DoSingleKeywordNode(language, keywords, _T("Documentation"));
    DoSingleKeywordNode(language, keywords, _T("User"));
    DoSingleKeywordNode(language, keywords, _T("Set"));
}

void EditorLexerLoader::DoSingleKeywordNode(HighlightLanguage language, TiXmlElement* node, const wxString& nodename)
{
    TiXmlElement* keywords = node->FirstChildElement(nodename.mb_str());
    while (keywords)
    {
    //    LOGSTREAM << "Found " << nodename << '\n';
        int keyidx = keywords->Attribute("index") ? atol(keywords->Attribute("index")) : -1;
    //    LOGSTREAM << "keyidx=" << keyidx << '\n';
        if (keyidx != -1)
        {
            // the lexer file contains keywords indented - remove the extra spacing and EOLs
            wxRegEx regex(_T("[[:space:]]+"));
            wxString value(keywords->Attribute("value"), wxConvUTF8);
            regex.Replace(&value, _T(" "));

            #if wxCHECK_VERSION(2, 9, 0)
            m_pTarget->SetKeywords(language, keyidx, value );
            #else
            m_pTarget->SetKeywords(language, keyidx, wxString ( value, wxConvUTF8 ) );
            #endif
        }

        keywords = keywords->NextSiblingElement(nodename.mb_str());
    }
}

void EditorLexerLoader::DoSampleCode(HighlightLanguage language, TiXmlElement* node)
{
    TiXmlElement* sample = node->FirstChildElement("SampleCode");
    if (!sample)
        return;
    wxString code = wxString ( sample->Attribute("value"), wxConvUTF8 );
    if (code.IsEmpty())
        return;
    int breakLine = sample->Attribute("breakpoint_line") ? atol(sample->Attribute("breakpoint_line")) : -1;
    int debugLine = sample->Attribute("debug_line") ? atol(sample->Attribute("debug_line")) : -1;
    int errorLine = sample->Attribute("error_line") ? atol(sample->Attribute("error_line")) : -1;
    m_pTarget->SetSampleCode(language, code, breakLine, debugLine, errorLine);
}

void EditorLexerLoader::DoLangAttributes(HighlightLanguage language, TiXmlElement* node)
{
    TiXmlElement* attribs = node->FirstChildElement("LanguageAttributes");
    if ( !attribs )
        return;

    bool CaseSensitive = attribs->Attribute("CaseSensitive") ? atol(attribs->Attribute("CaseSensitive")) != 0 : false;
    m_pTarget->SetCaseSensitivity(language, CaseSensitive);


    CommentToken token;
    token.lineComment = wxString( attribs->Attribute("LineComment"), wxConvUTF8 );
    token.streamCommentStart = wxString( attribs->Attribute("StreamCommentStart"), wxConvUTF8 );
    token.streamCommentEnd = wxString( attribs->Attribute("StreamCommentEnd"), wxConvUTF8 );
    token.boxCommentStart = wxString( attribs->Attribute("BoxCommentStart"), wxConvUTF8 );
    token.boxCommentMid = wxString( attribs->Attribute("BoxCommentMid"), wxConvUTF8 );
    token.boxCommentEnd = wxString( attribs->Attribute("BoxCommentEnd"), wxConvUTF8 );

    m_pTarget->SetCommentToken(language, token);
}
