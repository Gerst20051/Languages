/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision: 6058 $
 * $Id: token.cpp 6058 2010-01-07 12:04:49Z mortenmacfly $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/codecompletion/parser/token.cpp $
 */

#include <sdk.h>
#include <globals.h>
#include "logmanager.h"
#include "manager.h"
#include "token.h"
#include <wx/intl.h>
#include <wx/tokenzr.h>

#define TOKEN_DEBUG_OUTPUT 0

#if TOKEN_DEBUG_OUTPUT
    #include <wx/stopwatch.h>
    #define TRACE(format, args...)\
    Manager::Get()->GetLogManager()->DebugLog(F( format , ## args))
#else
    #define TRACE(format, args...)
#endif

inline void SaveTokenIdxSetToFile(wxOutputStream* f,const TokenIdxSet& data)
{
    SaveIntToFile(f, (int)(data.size()));
    for (TokenIdxSet::iterator it = data.begin(); it != data.end(); it++)
    {
        int num = *it;
        SaveIntToFile(f, num);
    }
}

bool LoadTokenIdxSetFromFile(wxInputStream* f,TokenIdxSet* data)
{
    if (!data)
        return false;

    bool result = true;
    data->clear();
    int size = 0;
    do
    {
        if (!LoadIntFromFile(f, &size))
        {
            result = false;
            break;
        }
        int num = 0;
        for (int i = 0; i < size; i++)
        {
            if (!LoadIntFromFile(f,&num))
            {
                result = false;
                break;
            }
            data->insert(num);
        }
    } while (false);
    return result;
}

Token::Token()
    :
    m_FileIdx(0),
    m_Line(0),
    m_ImplFileIdx(0),
    m_ImplLine(0),
    m_ImplLineStart(0),
    m_ImplLineEnd(0),
    m_Scope(tsUndefined),
    m_TokenKind(tkUndefined),
    m_IsOperator(false),
    m_IsLocal(false),
    m_IsTemp(false),
    m_ParentIndex(-1),
    m_pUserData(0),
    m_pTree(0),
    m_Self(-1)
{
    m_Ticket = GetTokenTicket();
}

Token::Token(const wxString& name, unsigned int file, unsigned int line) :
    m_Name(name),
    m_FileIdx(file),
    m_Line(line),
    m_ImplFileIdx(0),
    m_ImplLine(0),
    m_ImplLineStart(0),
    m_ImplLineEnd(0),
    m_Scope(tsUndefined),
    m_TokenKind(tkUndefined),
    m_IsOperator(false),
    m_IsLocal(false),
    m_IsTemp(false),
    m_ParentIndex(-1),
    m_pUserData(0),
    m_pTree(0),
    m_Self(-1)
{
    //ctor
    m_Ticket = GetTokenTicket();
}

Token::~Token()
{
    //dtor
}

unsigned long Token::GetTokenTicket()
{
    static wxCriticalSection s_TicketProtection;
    static unsigned long ticket = 256; // Reserve some space for the class browser
    wxCriticalSectionLocker lock(s_TicketProtection);
    return ticket++;
}

wxString Token::GetParentName()
{
    wxString parentname = _T("");
    wxCriticalSectionLocker* lock = 0;
    if (m_pTree)
        lock = new wxCriticalSectionLocker(s_MutexProtection);
    Token* parent = GetParentToken();
    if (parent)
        parentname = parent->m_Name;
    if (lock)
        delete lock;
    return parentname;
}

wxString Token::DisplayName() const
{
    wxString result;
    if      (m_TokenKind == tkClass)
        return result << _T("class ")     << m_Name << m_StrippedArgs << _T(" {...}");
    else if (m_TokenKind == tkNamespace)
        return result << _T("namespace ") << m_Name << _T(" {...}");
    else if (m_TokenKind == tkEnum)
        return result << _T("enum ")      << m_Name << _T(" {...}");
    else if (m_TokenKind == tkTypedef)
    {
        result << _T("typedef");

        if (!m_Type.IsEmpty())
            result << _T(" ") << m_Type;

        if (result.Find('*', true) != wxNOT_FOUND)
        {
            result.RemoveLast();
            return result << m_Name << _T(")") <<  m_Args;
        }
        return result << _T(" ") << m_Name;
    }
    else if (m_TokenKind == tkPreprocessor)
    {
        result << _T("#define ") << m_Name << m_Args;
        if (!m_Type.IsEmpty())
            return result << _T(" ") << m_Type;
    }

    // else
    if (!m_Type.IsEmpty())
        result << m_Type << _T(" ");

    if (m_TokenKind == tkEnumerator)
        return     result << GetNamespace() << m_Name << _T("=") << m_Args;

    return result << GetNamespace() << m_Name << m_Args;
}

Token* Token::GetParentToken()
{
    Token* the_token = 0;
    if (!m_pTree)
        return 0;
    the_token = m_pTree->at(m_ParentIndex);
    return the_token;
}

wxString Token::GetFilename() const
{
    if (!m_pTree)
        return wxString(_T(""));
    return m_pTree->GetFilename(m_FileIdx);
}

wxString Token::GetImplFilename() const
{
    if (!m_pTree)
        return wxString(_T(""));
    return m_pTree->GetFilename(m_ImplFileIdx);
}

bool Token::MatchesFiles(const TokenFilesSet& files)
{
    if (!files.size())
        return true;

    if (!m_FileIdx && !m_ImplFileIdx)
        return true;

    if ((m_FileIdx && files.count(m_FileIdx)) || (m_ImplFileIdx && files.count(m_ImplFileIdx)))
        return true;

    return false;
}

wxString Token::GetNamespace() const
{
    const wxString dcolon(_T("::"));
    wxString res;
    Token* parentToken = m_pTree->at(m_ParentIndex);
    while (parentToken)
    {
        res.Prepend(dcolon);
        res.Prepend(parentToken->m_Name);
        parentToken = parentToken->GetParentToken();
    }
    return res;
}

void Token::AddChild(int child)
{
    if (child >= 0)
        m_Children.insert(child);
}

bool Token::InheritsFrom(int idx) const
{
    if (idx < 0 || !m_pTree)
        return false;

    Token* token = m_pTree->at(idx);
    if (!token)
        return false;

    for (TokenIdxSet::iterator it = m_DirectAncestors.begin(); it != m_DirectAncestors.end(); it++)
    {
        int idx2 = *it;
        Token* ancestor = m_pTree->at(idx2);

        if (!ancestor)
            continue;

        if (ancestor == token || ancestor->InheritsFrom(idx)) // ##### is this intended?
            return true;
    }
    return false;
}

wxString Token::GetTokenKindString() const
{
    switch (m_TokenKind)
    {
        case tkClass:        return _T("class");
        case tkNamespace:    return _T("namespace");
        case tkTypedef:      return _T("typedef");
        case tkEnum:         return _T("enum");
        case tkEnumerator:   return _T("enumerator");
        case tkFunction:     return _T("function");
        case tkConstructor:  return _T("constructor");
        case tkDestructor:   return _T("destructor");
        case tkPreprocessor: return _T("preprocessor");
        case tkMacro:        return _T("macro");
        case tkVariable:     return _T("variable");
        default:             return wxEmptyString; // tkUndefined
    }
}

wxString Token::GetTokenScopeString() const
{
    switch (m_Scope)
    {
        case tsPrivate:   return _T("private");
        case tsProtected: return _T("protected");
        case tsPublic:    return _T("public");
        default:          return wxEmptyString;
    }
}

bool Token::SerializeIn(wxInputStream* f)
{
    bool result = true;
    do
    {
        if (!LoadIntFromFile(f, (int*)&m_Self))
        {
            result = false;
            break;
        }
        if (!LoadIntFromFile(f, (int*)&m_ParentIndex))
        {
            result = false;
            break;
        }
        if (m_ParentIndex < 0)
            m_ParentIndex = -1;

        if (!LoadStringFromFile(f, m_Type))
        {
            result = false;
            break;
        }
        if (!LoadStringFromFile(f, m_ActualType))
        {
            result = false;
            break;
        }
        if (!LoadStringFromFile(f, m_Name))
        {
            result = false;
            break;
        }
        if (!LoadStringFromFile(f, m_Args))
        {
            result = false;
            break;
        }
        if (!LoadStringFromFile(f, m_AncestorsString))
        {
            result = false;
            break;
        }
        if (!LoadIntFromFile(f, (int*)&m_FileIdx))
        {
            result = false;
            break;
        }
        if (!LoadIntFromFile(f, (int*)&m_Line))
        {
            result = false;
            break;
        }
        if (!LoadIntFromFile(f, (int*)&m_ImplFileIdx))
        {
            result = false;
            break;
        }
        if (!LoadIntFromFile(f, (int*)&m_ImplLine))
        {
            result = false;
            break;
        }
        if (!LoadIntFromFile(f, (int*)&m_Scope))
        {
            result = false;
            break;
        }
        if (!LoadIntFromFile(f, (int*)&m_TokenKind))
        {
            result = false;
            break;
        }
        if (!LoadIntFromFile(f, (int*)&m_IsOperator))
        {
            result = false;
            break;
        }
        if (!LoadIntFromFile(f, (int*)&m_IsLocal))
        {
            result = false;
            break;
        }
        if (!LoadTokenIdxSetFromFile(f, &m_Ancestors))
        {
            result = false;
            break;
        }
        if (!LoadTokenIdxSetFromFile(f, &m_Children))
        {
            result = false;
            break;
        }
        if (!LoadTokenIdxSetFromFile(f, &m_Descendants))
        {
            result = false;
            break;
        }
    } while (false);
    return result;
}

bool Token::SerializeOut(wxOutputStream* f)
{
    SaveIntToFile(f, m_Self);
    SaveIntToFile(f, m_ParentIndex);
    SaveStringToFile(f, m_Type);
    SaveStringToFile(f, m_ActualType);
    SaveStringToFile(f, m_Name);
    SaveStringToFile(f, m_Args);
    SaveStringToFile(f, m_AncestorsString);
    SaveIntToFile(f, m_FileIdx);
    SaveIntToFile(f, m_Line);
    SaveIntToFile(f, m_ImplFileIdx);
    SaveIntToFile(f, m_ImplLine);
    SaveIntToFile(f, m_Scope);
    SaveIntToFile(f, m_TokenKind);
    SaveIntToFile(f, m_IsOperator ? 1 : 0);
    SaveIntToFile(f, m_IsLocal ? 1 : 0);

    SaveTokenIdxSetToFile(f, m_Ancestors);
    SaveTokenIdxSetToFile(f, m_Children);
    SaveTokenIdxSetToFile(f, m_Descendants);
    return true;
}

// *** TokensTree ***

TokensTree::TokensTree() :
    m_Modified(false)
{
    m_Tokens.clear();
    m_Tree.clear();

    m_FilenamesMap.clear();
    m_FilesMap.clear();
    m_FilesToBeReparsed.clear();
    m_FreeTokens.clear();

    m_TopNameSpaces.clear();
    m_GlobalNameSpace.clear();

    m_FilesStatus.clear();
}


TokensTree::~TokensTree()
{
    clear();
}

void TokensTree::clear()
{
    m_Tree.clear();

    m_FilenamesMap.clear();
    m_FilesMap.clear();
    m_FilesToBeReparsed.clear();
    m_FreeTokens.clear();

    m_TopNameSpaces.clear();
    m_GlobalNameSpace.clear();

    m_FilesStatus.clear();

    size_t i;
    for (i = 0;i < m_Tokens.size(); i++)
    {
        Token* token = m_Tokens[i];
        if (token)
            delete token;
    }
    m_Tokens.clear();
}

size_t TokensTree::size()
{
    return m_Tokens.size();
}

size_t TokensTree::realsize()
{
    if (m_Tokens.size() <= m_FreeTokens.size())
        return 0;

    return m_Tokens.size() - m_FreeTokens.size();
}

int TokensTree::insert(Token* newToken)
{
    if (!newToken)
        return -1;

    return AddToken(newToken, -1);
}

int TokensTree::insert(int loc, Token* newToken)
{
    if (!newToken)
        return -1;

    return AddToken(newToken, loc);
}

int TokensTree::erase(int loc)
{
    if (!m_Tokens[loc])
        return 0;

    RemoveToken(loc);
    return 1;
}

void TokensTree::erase(Token* oldToken)
{
    RemoveToken(oldToken);
}

int TokensTree::TokenExists(const wxString& name, int parent, short int kindMask)
{
    int idx = m_Tree.GetItemNo(name);
    if (!idx)
        return -1;

    TokenIdxSet::iterator it;
    TokenIdxSet& curList = m_Tree.GetItemAtPos(idx);
    int result = -1;
    for (it = curList.begin(); it != curList.end(); it++)
    {
        result = *it;
        if (result < 0 || (size_t)result >= m_Tokens.size())
            continue;

        Token* curToken = m_Tokens[result];
        if (!curToken)
            continue;

        if (   (   (parent < 0)
                || (curToken->m_ParentIndex == parent) )
            && (curToken->m_TokenKind & kindMask) )
        {
            return result;
        }
    }

    return -1;
}

size_t TokensTree::FindMatches(const wxString& s, TokenIdxSet& result, bool caseSensitive, bool is_prefix, short int kindMask)
{
    result.clear();

    set<size_t> lists;
    int numitems = m_Tree.FindMatches(s, lists, caseSensitive, is_prefix);
    if (!numitems)
        return 0;

    // now the lists contains indexes to all the matching keywords
    // first loop will find all the keywords
    for (set<size_t>::iterator it = lists.begin(); it != lists.end(); it++)
    {
        TokenIdxSet* curset = &(m_Tree.GetItemAtPos(*it));
        // second loop will get all the items mapped by the same keyword,
        // for example, we have ClassA::foo, ClassB::foo ...
        if (curset)
        {
            for (TokenIdxSet::iterator it2 = curset->begin(); it2 != curset->end(); it2++)
            {
                Token* token = at(*it);
                if (   token
                    && (   (kindMask == tkUndefined)
                        || (token->m_TokenKind & kindMask) ) )
                    result.insert(*it2);
            }
        }
    }
    return result.size();
}

size_t TokensTree::FindTokensInFile(const wxString& file, TokenIdxSet& result, short int kindMask)
{
    result.clear();

    // get file idx
    if (!m_FilenamesMap.HasItem(file))
        return 0;

    int idx = m_FilenamesMap.GetItemNo(file);

    // now get the tokens set matching this file idx
    TokenFilesMap::iterator itf = m_FilesMap.find(idx);
    if (itf == m_FilesMap.end())
        return 0;

    // loop all results and add to final result set after filtering on token kind
    TokenIdxSet& tokens = itf->second;
    for (TokenIdxSet::iterator it = tokens.begin(); it != tokens.end(); ++it)
    {
        Token* token = at(*it);
        if (token && (kindMask & token->m_TokenKind))
            result.insert(*it);
    }
    TRACE(_T("TokensTree::FindTokensInFile() : Found %d results for file '%s'."), result.size(), file.wx_str());
    return result.size();
}

int TokensTree::AddToken(Token* newToken, int fileIndex)
{
    if (!newToken)
        return -1;

    const wxString & name = newToken->m_Name;

    static TokenIdxSet tmpTokens = TokenIdxSet();

    // Insert the token's name and the token in the (inserted?) list
    size_t idx = m_Tree.AddItem(name, tmpTokens);
    TokenIdxSet& curList = m_Tree.GetItemAtPos(idx);

    int newItem = AddTokenToList(newToken, fileIndex);
    curList.insert(newItem);

    size_t fileIdx = (fileIndex<0) ? newToken->m_FileIdx : (size_t)fileIndex;
    m_FilesMap[fileIdx].insert(newItem);

    // Add Token (if applicable) to the namespaces indexes
    if (newToken->m_ParentIndex < 0)
    {
        newToken->m_ParentIndex = -1;
        m_GlobalNameSpace.insert(newItem);
        if (newToken->m_TokenKind == tkNamespace)
            m_TopNameSpaces.insert(newItem);
    }

    // All done!
    return newItem;
}

void TokensTree::RemoveToken(int idx)
{
    if (idx<0 || (size_t)idx >= m_Tokens.size())
        return;
    RemoveToken(m_Tokens[idx]);
}

void TokensTree::RemoveToken(Token* oldToken)
{
    if (!oldToken)
        return;

    int idx = oldToken->m_Self;
    if (m_Tokens[idx]!=oldToken)
        return;

    // Step 1: Detach token from its parent

    Token* parentToken = 0;
    if ((size_t)(oldToken->m_ParentIndex) >= m_Tokens.size())
        oldToken->m_ParentIndex = -1;
    if (oldToken->m_ParentIndex >= 0)
        parentToken = m_Tokens[oldToken->m_ParentIndex];
    if (parentToken)
        parentToken->m_Children.erase(idx);

    TokenIdxSet nodes;
    TokenIdxSet::iterator it;

    // Step 2: Detach token from its ancestors

    nodes = (oldToken->m_DirectAncestors);
    for (it = nodes.begin();it!=nodes.end(); it++)
    {
        int ancestoridx = *it;
        if (ancestoridx < 0 || (size_t)ancestoridx >= m_Tokens.size())
            continue;
        Token* ancestor = m_Tokens[ancestoridx];
        if (ancestor)
            ancestor->m_Descendants.erase(idx);
    }
    oldToken->m_Ancestors.clear();
    oldToken->m_DirectAncestors.clear();

    // Step 3: Remove children

    nodes = (oldToken->m_Children); // Copy the list to avoid interference
    for (it = nodes.begin();it!=nodes.end(); it++)
        RemoveToken(*it);
    // m_Children SHOULD be empty by now - but clear anyway.
    oldToken->m_Children.clear();

    // Step 4: Remove descendants

    nodes = oldToken->m_Descendants; // Copy the list to avoid interference
    for (it = nodes.begin();it!=nodes.end(); it++)
    {
        if (*it == idx) // that should not happen, we can not be our own descendant, but in fact that can happen with boost
        {
            Manager::Get()->GetLogManager()->DebugLog(_T("Break out the loop to remove descendants, to avoid a crash. We can not be our own descendant!"));
            break;
        }
         RemoveToken(*it);
    }
    // m_Descendants SHOULD be empty by now - but clear anyway.
    oldToken->m_Descendants.clear();

    // Step 5: Detach token from the SearchTrees

    int idx2 = m_Tree.GetItemNo(oldToken->m_Name);
    if (idx2)
    {
        TokenIdxSet& curList = m_Tree.GetItemAtPos(idx2);
        curList.erase(idx);
    }

    // Now, from the global namespace (if applicable)
    if (oldToken->m_ParentIndex == -1)
    {
        m_GlobalNameSpace.erase(idx);
        m_TopNameSpaces.erase(idx);
    }

    // Step 6: Finally, remove it from the list.

    RemoveTokenFromList(idx);
}

int TokensTree::AddTokenToList(Token* newToken, int forceidx)
{
    if (!newToken)
        return -1;

    int result = -1;

    if (forceidx >= 0) // Reading from cache?
    {
        if ((size_t)forceidx >= m_Tokens.size())
        {
            int max = 250*((forceidx + 250) / 250);
            m_Tokens.resize((max),0); // fill next 250 items with null-values
        }
        m_Tokens[forceidx] = newToken;
        result = forceidx;
    }
    else // For real-time parsing
    {
        if (m_FreeTokens.size())
        {
            result = m_FreeTokens[m_FreeTokens.size() - 1];
            m_FreeTokens.pop_back();
            m_Tokens[result] = newToken;
        }
        else
        {
            result = m_Tokens.size();
            m_Tokens.push_back(newToken);
        }
    }

    newToken->m_pTree = this;
    newToken->m_Self = result;
    // Clean up extra string memory

    newToken->m_Type.Shrink();
    newToken->m_Name.Shrink();
    newToken->m_Args.Shrink();
    newToken->m_AncestorsString.Shrink();

    return result;
}

void TokensTree::RemoveTokenFromList(int idx)
{
    if (idx < 0 || (size_t)idx >= m_Tokens.size())
        return;
    Token* oldToken = m_Tokens[idx];
    if (oldToken)
    {
        m_Tokens[idx] = 0;
        m_FreeTokens.push_back(idx);
        m_FilesToBeReparsed.insert(oldToken->m_FileIdx);
        delete oldToken;
    }
}

void TokensTree::RemoveFile(const wxString& filename)
{
    int fileIndex = GetFileIndex(filename);
    RemoveFile(fileIndex);
}

void TokensTree::RemoveFile(int fileIndex)
{
    if (fileIndex <=0 )
        return;
    TokenIdxSet& the_list = m_FilesMap[fileIndex];
    TokenIdxSet::iterator it;
    for (it = the_list.begin(); it != the_list.end();it++)
    {
        int idx = *it;
        if (idx < 0 || (size_t)idx > m_Tokens.size())
            continue;
        Token* the_token = at(idx);

        if (!the_token)
            continue;

        // do not remove token lightly...
        // only if both its decl filename and impl filename are either empty or match this file
        // if one of those filenames do not match the above criteria
        // just clear the relevant info, leaving the token where it is...

        bool match1 = the_token->m_FileIdx     == 0 || (int)the_token->m_FileIdx     == fileIndex;
        bool match2 = the_token->m_ImplFileIdx == 0 || (int)the_token->m_ImplFileIdx == fileIndex;
        if (match1 && match2)
            RemoveToken(the_token); // safe to remove the token
        else
        {
            // do not remove token, just clear the matching info
            if (match1)
            {
                the_token->m_FileIdx = 0;
                the_token->m_Line = 0;
            }
            else if (match2)
            {
                the_token->m_ImplFileIdx = 0;
                the_token->m_ImplLine = 0;
            }
        }
    }
    the_list.clear();
}

void TokensTree::FreeTemporaries()
{
    for (int i = m_Tokens.size() -1;i >= 0;i--)
    {
        Token* token = m_Tokens[i];
        if (token && token->m_IsTemp)
            RemoveToken(token);
    }
}

void TokensTree::RecalcFreeList()
{
    m_FreeTokens.clear();
    for (int i = m_Tokens.size() -1;i >= 0;i--)
    {
        if (!m_Tokens[i])
            m_FreeTokens.push_back(i);
    }
}

void TokensTree::RecalcData()
{
#if TOKEN_DEBUG_OUTPUT
    wxStopWatch sw;
#endif

    TRACE(_T("RecalcData() : Calculating full inheritance tree."));
    // first loop to convert ancestors string to token indices for each token
    for (size_t i = 0; i < size(); ++i)
    {
        Token* token = at(i);
        if (!token)
            continue;

        if (!(token->m_TokenKind & (tkClass | tkTypedef | tkEnum)))
            continue;
        if (token->m_AncestorsString.IsEmpty())
            continue;
        // only local symbols might change inheritance
//        if (!token->m_IsLocal)
//            continue;

        token->m_DirectAncestors.clear();
        token->m_Ancestors.clear();

        TRACE(_T("RecalcData() : Token %s, Ancestors %s"),
              token->m_Name.wx_str(),
              token->m_AncestorsString.wx_str());

        wxStringTokenizer tkz(token->m_AncestorsString, _T(","));
        while (tkz.HasMoreTokens())
        {
            wxString ancestor = tkz.GetNextToken();
            if (ancestor.IsEmpty() || ancestor == token->m_Name)
                continue;

            TRACE(_T("RecalcData() : Ancestor %s"), ancestor.wx_str());

            // ancestors might contain namespaces, e.g. NS::Ancestor
            if (ancestor.Find(_T("::")) != wxNOT_FOUND)
            {
                Token* ancestorToken = 0;
                wxStringTokenizer anctkz(ancestor, _T("::"));
                while (anctkz.HasMoreTokens())
                {
                    wxString ns = anctkz.GetNextToken();
                    if (!ns.IsEmpty())
                    {
                        int ancestorIdx = TokenExists(ns, ancestorToken ? ancestorToken->GetSelf() : -1, tkNamespace | tkClass | tkTypedef);
                        ancestorToken = at(ancestorIdx);
//                        ancestorToken = token->HasChildToken(ns, tkNamespace | tkClass);
                        if (!ancestorToken) // unresolved
                            break;
                    }
                }
                if (ancestorToken && ancestorToken != token && ancestorToken->m_TokenKind == tkClass)
                {
                    TRACE(_T("RecalcData() : Resolved to %s"), ancestorToken->m_Name.wx_str());
                    token->m_Ancestors.insert(ancestorToken->GetSelf());
                    ancestorToken->m_Descendants.insert(i);
                    TRACE(_T("RecalcData() :  + '%s'"), ancestorToken->m_Name.wx_str());

                }
                else
                    TRACE(_T("RecalcData() :  ! '%s' (unresolved)"), ancestor.wx_str());
            }
            else // no namespaces in ancestor
            {
                // accept multiple matches for inheritance
                TokenIdxSet result;
                FindMatches(ancestor, result, true, false);
                for (TokenIdxSet::iterator it = result.begin(); it != result.end(); it++)
                {
                    Token* ancestorToken = at(*it);
                    // only classes take part in inheritance
                    if (   ancestorToken
                        && (ancestorToken != token)
                        && (   (ancestorToken->m_TokenKind == tkClass)
                            || (ancestorToken->m_TokenKind == tkEnum)
                            || (ancestorToken->m_TokenKind == tkTypedef) ) )
                    {
                        token->m_Ancestors.insert(*it);
                        ancestorToken->m_Descendants.insert(i);
                        TRACE(_T("RecalcData() :  + '%s'"), ancestorToken->m_Name.wx_str());
                    }
                }
#if TOKEN_DEBUG_OUTPUT
                if (result.empty())
                    TRACE(_T("RecalcData() :  ! '%s' (unresolved)"), ancestor.wx_str());
#endif
            }
        }

        token->m_DirectAncestors = token->m_Ancestors;

        if (!token->m_IsLocal) // global symbols are linked once
        {
            TRACE(_T("RecalcData() : Removing ancestor string from %s"), token->m_Name.wx_str(), token->m_Name.wx_str());
            token->m_AncestorsString.Clear();
        }
    }

#if TOKEN_DEBUG_OUTPUT
    TRACE(_T("RecalcData() : First iteration took : %ld ms"), sw.Time());
    sw.Start();
#endif

    // second loop to calculate full inheritance for each token
    for (size_t i = 0; i < size(); ++i)
    {
        Token* token = at(i);
        if (!token)
            continue;

        if (!(token->m_TokenKind & (tkClass | tkTypedef | tkEnum)))
            continue;

        // recalc
        TokenIdxSet result;
        for (TokenIdxSet::iterator it = token->m_Ancestors.begin(); it != token->m_Ancestors.end(); it++)
            RecalcFullInheritance(*it, result);

        // now, add the resulting set to ancestors set
        for (TokenIdxSet::iterator it = result.begin(); it != result.end(); it++)
        {
            Token* ancestor = at(*it);
            if (ancestor)
            {
                token->m_Ancestors.insert(*it);
                ancestor->m_Descendants.insert(i);
            }
        }

#if TOKEN_DEBUG_OUTPUT
        if (token)
        {
            // debug loop
            TRACE(_T("RecalcData() : Ancestors for %s:"), token->m_Name.wx_str());
            for (TokenIdxSet::iterator it = token->m_Ancestors.begin(); it != token->m_Ancestors.end(); it++)
            {
                Token* anc_token = at(*it);
                if (anc_token)
                    TRACE(_T("RecalcData() :  + %s"), anc_token->m_Name.wx_str());
                else
                    TRACE(_T("RecalcData() :  + NULL?!"));
            }

        }
#endif
    }
#if TOKEN_DEBUG_OUTPUT
    TRACE(_T("RecalcData() : Second iteration took : %ld ms"), sw.Time());
#endif

    TRACE(_T("RecalcData() : Full inheritance calculated."));
}

// caches the inheritance info for each token (recursive function)
void TokensTree::RecalcFullInheritance(int parentIdx, TokenIdxSet& result)
{
    // no parent token? no ancestors...
    if (parentIdx == -1)
        return;

    // no parent token? no ancestors...
    Token* ancestor = at(parentIdx);
    if (!ancestor)
        return;

    // only classes take part in inheritance
    if (!(ancestor->m_TokenKind & (tkClass | tkTypedef)))
        return;

    TRACE(_T("RecalcFullInheritance() : Anc: '%s'"), ancestor->m_Name.wx_str());

    // for all its ancestors
    for (TokenIdxSet::iterator it = ancestor->m_Ancestors.begin(); it != ancestor->m_Ancestors.end(); it++)
    {
        if (*it != -1 && // not global scope
            *it != parentIdx && // not the same token (avoid infinite loop)
            result.find(*it) == result.end()) // not already in the set (avoid infinite loop)
        {
            // add it to the set
            result.insert(*it);
            // and recurse for its ancestors
            RecalcFullInheritance(*it, result);
        }
    }
}

Token* TokensTree::GetTokenAt(int idx)
{
    if (idx < 0 || (size_t)idx >= m_Tokens.size())
        return 0;

    return m_Tokens[idx];
}

size_t TokensTree::GetFileIndex(const wxString& filename)
{
    size_t result = m_FilenamesMap.insert(filename);
    // Insert does not alter the tree if the filename is already found.
    return result;
}

const wxString TokensTree::GetFilename(size_t idx) const
{
    wxString result = m_FilenamesMap.GetString(idx);
    return result;
}

bool TokensTree::IsFileParsed(const wxString& filename)
{
    size_t index = GetFileIndex(filename);

    bool parsed = (   m_FilesMap.count(index)
                   && (m_FilesStatus[index]!=fpsNotParsed)
                   && !m_FilesToBeReparsed.count(index) );

    return parsed;
}

void TokensTree::MarkFileTokensAsLocal(const wxString& filename, bool local, void* userData)
{
    MarkFileTokensAsLocal(GetFileIndex(filename), local, userData);
}

void TokensTree::MarkFileTokensAsLocal(size_t file, bool local, void* userData)
{
    if (file == 0)
        return;

    TokenIdxSet& tokens = m_FilesMap[file];
    for (TokenIdxSet::iterator it = tokens.begin(); it != tokens.end(); ++it)
    {
        Token* token = m_Tokens.at(*it);
        if (token)
        {
            token->m_IsLocal = local;
            token->m_pUserData = userData;
        }
    }
}

size_t TokensTree::ReserveFileForParsing(const wxString& filename,bool preliminary)
{
    size_t index = GetFileIndex(filename);
    if (   m_FilesToBeReparsed.count(index)
        && (  !m_FilesStatus.count(index)
            || m_FilesStatus[index]==fpsDone) )
    {
        RemoveFile(filename);
        m_FilesToBeReparsed.erase(index);
        m_FilesStatus[index]=fpsNotParsed;
    }
    if (m_FilesStatus.count(index))
    {
        FileParsingStatus status = m_FilesStatus[index];
        if (preliminary)
        {
            if (status >= fpsAssigned)
                return 0; // Already assigned
        }
        else
        {
            if (status > fpsAssigned)
                return 0; // No parsing needed
        }
    }
    m_FilesToBeReparsed.erase(index);
    m_FilesStatus[index]=preliminary ? fpsAssigned : fpsBeingParsed; // Reserve file
    return index;
}

void TokensTree::FlagFileForReparsing(const wxString& filename)
{
    m_FilesToBeReparsed.insert(GetFileIndex(filename));
}

void TokensTree::FlagFileAsParsed(const wxString& filename)
{
    m_FilesStatus[GetFileIndex(filename)]=fpsDone;
}
