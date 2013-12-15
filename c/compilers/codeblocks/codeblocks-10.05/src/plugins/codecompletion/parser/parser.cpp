/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision: 6099 $
 * $Id: parser.cpp 6099 2010-01-22 13:45:29Z jenslody $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/codecompletion/parser/parser.cpp $
 */

#include <sdk.h>
#include <wx/app.h>
#include <wx/filename.h>
#include <wx/tokenzr.h>
#include <wx/intl.h>
#include <wx/progdlg.h>
#include "parser.h"

#include "../classbrowser.h"
#include "../classbrowserbuilderthread.h"

#ifndef STANDALONE
    #include <configmanager.h>
    #include <logmanager.h>
    #include <editormanager.h>
    #include <manager.h>
    #include <globals.h>
    #include <infowindow.h>
#endif // STANDALONE

#include <queue>

#ifndef CB_PRECOMP
      #include "editorbase.h"
#endif

#define PARSER_DEBUG_OUTPUT 0

#if PARSER_DEBUG_OUTPUT
    #define TRACE(format, args...)\
    Manager::Get()->GetLogManager()->DebugLog(F( format , ## args))
#else
    #define TRACE(format, args...)
#endif

static const char CACHE_MAGIC[] = "CCCACHE_1_3";
static const int batch_timer_delay = 500;
static const int reparse_timer_delay = 250;

int PARSER_END = wxNewId();
static int idPool = wxNewId();
int TIMER_ID = wxNewId();
int BATCH_TIMER_ID = wxNewId();

BEGIN_EVENT_TABLE(Parser, wxEvtHandler)
END_EVENT_TABLE()

Parser::Parser(wxEvtHandler* parent)
    : m_Options(),
    m_BrowserOptions(),
    m_pParent(parent),
#ifndef STANDALONE
    m_pImageList(0L),
#endif
    m_UsingCache(false),
    m_Pool(this, idPool, 1), // in the meanwhile it'll have to be forced to 1
    m_pTokensTree(0),
    m_pTempTokensTree(0),
    m_NeedsReparse(false),
    m_IsBatch(false),
    m_pClassBrowser(0),
    m_TreeBuildingStatus(0),
    m_TreeBuildingTokenIdx(0),
    m_Timer(this, TIMER_ID),
    m_BatchTimer(this,BATCH_TIMER_ID),
    m_StopWatchRunning(false),
    m_LastStopWatchTime(0),
    m_IgnoreThreadEvents(false),
    m_ShuttingDown(false)
{
    m_pTokensTree = new TokensTree;
    m_pTempTokensTree = new TokensTree;
    m_LocalFiles.clear();
    m_GlobalIncludes.clear();
    ReadOptions();
#ifndef STANDALONE
    m_pImageList = new wxImageList(16, 16);
    wxBitmap bmp;
    wxString prefix;
    prefix = ConfigManager::GetDataFolder() + _T("/images/codecompletion/");
    // bitmaps must be added by order of PARSER_IMG_* consts
    bmp = cbLoadBitmap(prefix + _T("class_folder.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_CLASS_FOLDER
    bmp = cbLoadBitmap(prefix + _T("class.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_CLASS
    bmp = cbLoadBitmap(prefix + _T("class_private.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_CLASS_PRIVATE
    bmp = cbLoadBitmap(prefix + _T("class_protected.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_CLASS_PROTECTED
    bmp = cbLoadBitmap(prefix + _T("class_public.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_CLASS_PUBLIC
    bmp = cbLoadBitmap(prefix + _T("ctor_private.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_CTOR_PRIVATE
    bmp = cbLoadBitmap(prefix + _T("ctor_protected.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_CTOR_PROTECTED
    bmp = cbLoadBitmap(prefix + _T("ctor_public.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_CTOR_PUBLIC
    bmp = cbLoadBitmap(prefix + _T("dtor_private.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_DTOR_PRIVATE
    bmp = cbLoadBitmap(prefix + _T("dtor_protected.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_DTOR_PROTECTED
    bmp = cbLoadBitmap(prefix + _T("dtor_public.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_DTOR_PUBLIC
    bmp = cbLoadBitmap(prefix + _T("method_private.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_FUNC_PRIVATE
    bmp = cbLoadBitmap(prefix + _T("method_protected.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_FUNC_PRIVATE
    bmp = cbLoadBitmap(prefix + _T("method_public.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_FUNC_PUBLIC
    bmp = cbLoadBitmap(prefix + _T("var_private.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_VAR_PRIVATE
    bmp = cbLoadBitmap(prefix + _T("var_protected.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_VAR_PROTECTED
    bmp = cbLoadBitmap(prefix + _T("var_public.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_VAR_PUBLIC
    bmp = cbLoadBitmap(prefix + _T("preproc.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_PREPROCESSOR
    bmp = cbLoadBitmap(prefix + _T("enum.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_ENUM
    bmp = cbLoadBitmap(prefix + _T("enum_private.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_ENUM_PRIVATE
    bmp = cbLoadBitmap(prefix + _T("enum_protected.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_ENUM_PROTECTED
    bmp = cbLoadBitmap(prefix + _T("enum_public.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_ENUM_PUBLIC
    bmp = cbLoadBitmap(prefix + _T("enumerator.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_ENUMERATOR
    bmp = cbLoadBitmap(prefix + _T("namespace.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_NAMESPACE
    bmp = cbLoadBitmap(prefix + _T("typedef.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_TYPEDEF
    bmp = cbLoadBitmap(prefix + _T("typedef_private.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_TYPEDEF_PRIVATE
    bmp = cbLoadBitmap(prefix + _T("typedef_protected.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_TYPEDEF_PROTECTED
    bmp = cbLoadBitmap(prefix + _T("typedef_public.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_TYPEDEF_PUBLIC
    bmp = cbLoadBitmap(prefix + _T("symbols_folder.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_SYMBOLS_FOLDER
    bmp = cbLoadBitmap(prefix + _T("vars_folder.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_VARS_FOLDER
    bmp = cbLoadBitmap(prefix + _T("funcs_folder.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_FUNCS_FOLDER
    bmp = cbLoadBitmap(prefix + _T("enums_folder.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_ENUMS_FOLDER
    bmp = cbLoadBitmap(prefix + _T("preproc_folder.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_PREPROC_FOLDER
    bmp = cbLoadBitmap(prefix + _T("others_folder.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_OTHERS_FOLDER
    bmp = cbLoadBitmap(prefix + _T("typedefs_folder.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_TYPEDEF_FOLDER
    bmp = cbLoadBitmap(prefix + _T("macro.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_MACRO
    bmp = cbLoadBitmap(prefix + _T("macro_private.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_MACRO_PRIVATE
    bmp = cbLoadBitmap(prefix + _T("macro_protected.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_MACRO_PROTECTED
    bmp = cbLoadBitmap(prefix + _T("macro_public.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_MACRO_PUBLIC
    bmp = cbLoadBitmap(prefix + _T("macro_folder.png"), wxBITMAP_TYPE_PNG);
    m_pImageList->Add(bmp); // PARSER_IMG_MACRO_FOLDER
#endif // STANDALONE
    ConnectEvents();
}

Parser::~Parser()
{
    m_ShuttingDown = true;
    if (m_pClassBrowser && m_pClassBrowser->GetParserPtr() == this)
        m_pClassBrowser->UnlinkParser();
    m_TreeBuildingStatus = 0;
    m_pClassBrowser = 0;

    Clear(); // Clear also disconnects the events
#ifndef STANDALONE
    Delete(m_pImageList);
    Delete(m_pTempTokensTree);
    Delete(m_pTokensTree);
#endif // STANDALONE
}

void Parser::ConnectEvents()
{
    Connect(-1, -1, cbEVT_THREADTASK_ALLDONE,
            (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)
            &Parser::OnAllThreadsDone);
    Connect(TIMER_ID, -1, wxEVT_TIMER,
            (wxObjectEventFunction)(wxEventFunction)(wxTimerEventFunction)
            &Parser::OnTimer);
    Connect(BATCH_TIMER_ID, -1, wxEVT_TIMER,
            (wxObjectEventFunction)(wxEventFunction)(wxTimerEventFunction)
            &Parser::OnBatchTimer);
}

void Parser::DisconnectEvents()
{
    Disconnect(-1,BATCH_TIMER_ID,wxEVT_TIMER);
    Disconnect(-1,TIMER_ID, wxEVT_TIMER);
    Disconnect(-1,-1,cbEVT_THREADTASK_ALLDONE);
}

void Parser::ReadOptions()
{
#ifdef STANDALONE
    m_Options.followLocalIncludes    = true;
    m_Options.followGlobalIncludes   = false;
    m_Options.caseSensitive          = true;
    m_Options.wantPreprocessor       = false;
    m_Options.useSmartSense          = true;
    m_Options.whileTyping            = true;
    m_BrowserOptions.showInheritance = false;
    m_BrowserOptions.expandNS        = false;
    m_BrowserOptions.viewFlat        = false;
    m_BrowserOptions.displayFilter   = bdfWorkspace;
#else // !STANDALONE
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("code_completion"));

    // one-time default settings change: upgrade everyone
    bool force_all_on = !cfg->ReadBool(_T("/parser_defaults_changed"), false);
    if (force_all_on)
    {
        cfg->Write(_T("/parser_defaults_changed"), (bool)true);

        cfg->Write(_T("/parser_follow_local_includes"), true);
        cfg->Write(_T("/parser_follow_global_includes"), true);
        cfg->Write(_T("/want_preprocessor"), true);
    }

    //m_Pool.SetConcurrentThreads(cfg->ReadInt(_T("/max_threads"), 1)); // Ignore it in the meanwhile

    m_Options.followLocalIncludes  = cfg->ReadBool(_T("/parser_follow_local_includes"), true);
    m_Options.followGlobalIncludes = cfg->ReadBool(_T("/parser_follow_global_includes"), true);
    m_Options.caseSensitive        = cfg->ReadBool(_T("/case_sensitive"), false);
    m_Options.useSmartSense        = cfg->ReadBool(_T("/use_SmartSense"), true);
    m_Options.whileTyping          = cfg->ReadBool(_T("/while_typing"), true);
    m_Options.wantPreprocessor     = cfg->ReadBool(_T("/want_preprocessor"), true);

    m_BrowserOptions.showInheritance = cfg->ReadBool(_T("/browser_show_inheritance"), false);
    m_BrowserOptions.expandNS        = cfg->ReadBool(_T("/browser_expand_ns"), false);
    m_BrowserOptions.treeMembers     = cfg->ReadBool(_T("/browser_tree_members"), true);
    m_BrowserOptions.displayFilter   = (BrowserDisplayFilter)cfg->ReadInt(_T("/browser_display_filter"), bdfWorkspace);
    m_BrowserOptions.sortType        = (BrowserSortType)cfg->ReadInt(_T("/browser_sort_type"), bstKind);
#endif // STANDALONE
}

void Parser::WriteOptions()
{
#ifndef STANDALONE
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("code_completion"));

    cfg->Write(_T("/max_threads"),                   (int)GetMaxThreads());
    cfg->Write(_T("/parser_follow_local_includes"),  m_Options.followLocalIncludes);
    cfg->Write(_T("/parser_follow_global_includes"), m_Options.followGlobalIncludes);
    cfg->Write(_T("/case_sensitive"),                m_Options.caseSensitive);
    cfg->Write(_T("/use_SmartSense"),                m_Options.useSmartSense);
    cfg->Write(_T("/while_typing"),                  m_Options.whileTyping);
    cfg->Write(_T("/want_preprocessor"),             m_Options.wantPreprocessor);

    cfg->Write(_T("/browser_show_inheritance"),      m_BrowserOptions.showInheritance);
    cfg->Write(_T("/browser_expand_ns"),             m_BrowserOptions.expandNS);
    cfg->Write(_T("/browser_tree_members"),          m_BrowserOptions.treeMembers);
    cfg->Write(_T("/browser_display_filter"),        m_BrowserOptions.displayFilter);
    cfg->Write(_T("/browser_sort_type"),             m_BrowserOptions.sortType);
#endif // STANDALONE
}

bool Parser::CacheNeedsUpdate()
{
    if (m_UsingCache)
    {
        wxCriticalSectionLocker lock(s_MutexProtection);
        return m_pTokensTree->m_Modified;
    }
    return true;
}

unsigned int Parser::GetFilesCount()
{
    wxCriticalSectionLocker lock(s_MutexProtection);
    return m_pTokensTree->m_FilesMap.size();
}

bool Parser::Done()
{
    wxCriticalSectionLocker lock(s_mutexListProtection);
    return m_Pool.Done();
}

#ifndef STANDALONE
void Parser::SetTokenKindImage(int kind, const wxBitmap& bitmap, const wxBitmap& mask)
{
    if (kind < PARSER_IMG_MIN || kind > PARSER_IMG_MAX)
        return;
#ifdef __WXMSW__
    m_pImageList->Replace(kind, bitmap, mask);
#endif
}

void Parser::SetTokenKindImage(int kind, const wxBitmap& bitmap, const wxColour& maskColour)
{
    if (kind < PARSER_IMG_MIN || kind > PARSER_IMG_MAX)
        return;
    m_pImageList->Replace(kind, bitmap);//, maskColour);
}

void Parser::SetTokenKindImage(int kind, const wxIcon& icon)
{
    if (kind < PARSER_IMG_MIN || kind > PARSER_IMG_MAX)
        return;
    m_pImageList->Replace(kind, icon);
}

int Parser::GetTokenKindImage(Token* token)
{
    if (!token)
        return PARSER_IMG_NONE;

    switch (token->m_TokenKind)
    {
        case tkPreprocessor:      return PARSER_IMG_PREPROCESSOR;

        case tkEnum:
            switch (token->m_Scope)
            {
                case tsPublic:    return PARSER_IMG_ENUM_PUBLIC;
                case tsProtected: return PARSER_IMG_ENUM_PROTECTED;
                case tsPrivate:   return PARSER_IMG_ENUM_PRIVATE;
                default:          return PARSER_IMG_ENUM;
            }

        case tkEnumerator:        return PARSER_IMG_ENUMERATOR;

        case tkClass:
            switch (token->m_Scope)
            {
                case tsPublic:    return PARSER_IMG_CLASS_PUBLIC;
                case tsProtected: return PARSER_IMG_CLASS_PROTECTED;
                case tsPrivate:   return PARSER_IMG_CLASS_PRIVATE;
                default:          return PARSER_IMG_CLASS_PUBLIC;
            }

        case tkNamespace:         return PARSER_IMG_NAMESPACE;

        case tkTypedef:
            switch (token->m_Scope)
            {
                case tsPublic:    return PARSER_IMG_TYPEDEF_PUBLIC;
                case tsProtected: return PARSER_IMG_TYPEDEF_PROTECTED;
                case tsPrivate:   return PARSER_IMG_TYPEDEF_PRIVATE;
                default:          return PARSER_IMG_TYPEDEF;
            }

        case tkMacro:
            switch (token->m_Scope)
            {
                case tsPublic:    return PARSER_IMG_MACRO_PUBLIC;
                case tsProtected: return PARSER_IMG_MACRO_PROTECTED;
                case tsPrivate:   return PARSER_IMG_MACRO_PRIVATE;
                default:          return PARSER_IMG_MACRO;
            }

        case tkConstructor:
            switch (token->m_Scope)
            {
                case tsProtected: return PARSER_IMG_CTOR_PROTECTED;
                case tsPrivate:   return PARSER_IMG_CTOR_PRIVATE;
                default:          return PARSER_IMG_CTOR_PUBLIC;
            }

        case tkDestructor:
            switch (token->m_Scope)
            {
                case tsProtected: return PARSER_IMG_DTOR_PROTECTED;
                case tsPrivate:   return PARSER_IMG_DTOR_PRIVATE;
                default:          return PARSER_IMG_DTOR_PUBLIC;
            }

        case tkFunction:
            switch (token->m_Scope)
            {
                case tsProtected: return PARSER_IMG_FUNC_PROTECTED;
                case tsPrivate:   return PARSER_IMG_FUNC_PRIVATE;
                default:          return PARSER_IMG_FUNC_PUBLIC;
            }

        case tkVariable:
            switch (token->m_Scope)
            {
                case tsProtected: return PARSER_IMG_VAR_PROTECTED;
                case tsPrivate:   return PARSER_IMG_VAR_PRIVATE;
                default:          return PARSER_IMG_VAR_PUBLIC;
            }

        default:                  return PARSER_IMG_NONE;
    }
}
#endif // STANDALONE

Token* Parser::FindTokenByName(const wxString& name, bool globalsOnly, short int kindMask) const
{
    wxCriticalSectionLocker lock(s_MutexProtection);
    int result = m_pTokensTree->TokenExists(name, -1, kindMask);
    return m_pTokensTree->at(result);
}

Token* Parser::FindChildTokenByName(Token* parent, const wxString& name, bool useInheritance, short int kindMask) const
{
    if (!parent)
        return FindTokenByName(name, false, kindMask);

    Token* result = 0;
    {
        wxCriticalSectionLocker lock(s_MutexProtection);
        result = m_pTokensTree->at(m_pTokensTree->TokenExists(name, parent->GetSelf(), kindMask));
    }

    if (!result && useInheritance)
    {
        // no reason for a critical section here:
        // it will only recurse to itself.
        // the critical section above is sufficient
        TokenIdxSet::iterator it;
        for (it = parent->m_DirectAncestors.begin(); it != parent->m_DirectAncestors.end(); ++it)
        {
            Token* ancestor = m_pTokensTree->at(*it);
            result = FindChildTokenByName(ancestor, name, true, kindMask);
            if (result)
                break;
        }
    }
    return result;
}

size_t Parser::FindMatches(const wxString& s, TokenList& result, bool caseSensitive, bool is_prefix)
{
    result.clear();
    TokenIdxSet tmpresult;
    wxCriticalSectionLocker lock(s_MutexProtection);
    if (!m_pTokensTree->FindMatches(s, tmpresult, caseSensitive, is_prefix))
        return 0;

    TokenIdxSet::iterator it;
    for (it = tmpresult.begin(); it != tmpresult.end(); ++it)
    {
        Token* token = m_pTokensTree->at(*it);
        if (token)
        result.push_back(token);
    }
    return result.size();
}

size_t Parser::FindMatches(const wxString& s, TokenIdxSet& result, bool caseSensitive, bool is_prefix)
{
    result.clear();
    TokenIdxSet tmpresult;
    wxCriticalSectionLocker lock(s_MutexProtection);
    if (!m_pTokensTree->FindMatches(s, tmpresult, caseSensitive, is_prefix))
        return 0;

    TokenIdxSet::iterator it;
    for (it = tmpresult.begin(); it != tmpresult.end(); ++it)
    {
        Token* token = m_pTokensTree->at(*it);
        if (token)
        //result.push_back(token);
        result.insert(*it);
    }
    return result.size();
}

void Parser::LinkInheritance(bool tempsOnly)
{
    wxCriticalSectionLocker lock(s_MutexProtection);
    (tempsOnly ? m_pTempTokensTree :  m_pTokensTree)->RecalcData();
}

void Parser::MarkFileTokensAsLocal(const wxString& filename, bool local, void* userData)
{
    wxCriticalSectionLocker lock(s_MutexProtection);
    m_pTokensTree->MarkFileTokensAsLocal(filename, local, userData);
}

bool Parser::ParseBuffer(const wxString& buffer, bool isLocal, bool bufferSkipBlocks, bool isTemp)
{
    ParserThreadOptions opts;
    opts.wantPreprocessor     = m_Options.wantPreprocessor;
    opts.followLocalIncludes  = m_Options.followLocalIncludes;
    opts.followGlobalIncludes = m_Options.followGlobalIncludes;
    opts.useBuffer            = true;
    opts.isTemp               = isTemp;
    opts.bufferSkipBlocks     = bufferSkipBlocks;
    opts.handleFunctions      = false;

    return Parse(buffer, isLocal, opts);
}

void Parser::BatchParse(const wxArrayString& filenames)
{
    m_BatchTimer.Stop();
    m_IsBatch = true;
    m_Pool.BatchBegin();

    Manager::Get()->GetLogManager()->DebugLog(F(_T("Batch-parsing %d file(s)..."), filenames.GetCount()));
    for (unsigned int i = 0; i < filenames.GetCount(); ++i)
        Parse(filenames[i]); // defer loading until later

    // Allow future parses to take place in this same run
    m_BatchTimer.Start(batch_timer_delay,wxTIMER_ONE_SHOT);
}

bool Parser::Parse(const wxString& filename, bool isLocal, LoaderBase* loader)
{
    ParserThreadOptions opts;
    opts.wantPreprocessor      = m_Options.wantPreprocessor;
    opts.useBuffer             = false;
    opts.bufferSkipBlocks      = false;
    opts.bufferSkipOuterBlocks = false;
    opts.followLocalIncludes   = m_Options.followLocalIncludes;
    opts.followGlobalIncludes  = m_Options.followGlobalIncludes;
    opts.loader                = loader; // maybe 0 at this point

    return Parse(UnixFilename(filename), isLocal, opts);
}

bool Parser::Parse(const wxString& bufferOrFilename, bool isLocal, ParserThreadOptions& opts)
{
    wxString buffOrFile = bufferOrFilename;
    bool result = false;
    do
    {
        if (!opts.useBuffer)
        {
            wxCriticalSectionLocker lock(s_MutexProtection);

            bool canparse = !m_pTokensTree->IsFileParsed(buffOrFile);
            if (canparse)
                canparse = m_pTokensTree->ReserveFileForParsing(buffOrFile, true) != 0;

            if (!canparse)
            {
               if (opts.loader) // if a loader is already open at this point, the caller must clean it up
                   Manager::Get()->GetLogManager()->DebugLog(_T("Parse() : CodeCompletion Plugin: FileLoader memory leak likely while loading file ")+bufferOrFilename);
               break;
            }

            if (!opts.loader) // this should always be true (memory will leak if a loader has already been initialized before this point)
                opts.loader = Manager::Get()->GetFileManager()->Load(bufferOrFilename, true);
        }

        TRACE(_T("Parse() : Creating task for: %s"), buffOrFile.wx_str());
        ParserThread* thread = new ParserThread(this,
                                                buffOrFile,
                                                isLocal,
                                                opts,
                                                m_pTokensTree);
        if (opts.useBuffer)
        {
            result = thread->Parse();
            LinkInheritance(true);
            delete thread;
            break;
        }

        bool use_timer = m_BatchTimer.IsRunning();
        if (!m_IsBatch && wxThread::IsMain())
        {
            use_timer = true;
            m_IsBatch = true;
            m_Pool.BatchBegin();
        }

        TRACE(_T("Parse() : Parsing %s"), buffOrFile.wx_str());

        if (m_IgnoreThreadEvents)
            m_IgnoreThreadEvents = false;

        #ifdef CODECOMPLETION_PROFILING
        StartStopWatch();
        m_BatchTimer.Stop();
        thread->Parse();
        #else
        m_Pool.AddTask(thread, true);
        #endif

        // For every parse, reset the countdown to -batch_timer_delay.
        // This will give us a tolerance period before the next parse job is queued.
        if (use_timer)
            m_BatchTimer.Start(batch_timer_delay, wxTIMER_ONE_SHOT);
        result = true;
    } while(false);

    return result;
}

bool Parser::ParseBufferForFunctions(const wxString& buffer)
{
    ParserThreadOptions opts;
    opts.wantPreprocessor = m_Options.wantPreprocessor;
    opts.useBuffer        = true;
    opts.bufferSkipBlocks = true;
    opts.handleFunctions  = true;

    ParserThread thread(this,
                        buffer,
                        false,
                        opts,
                        m_pTempTokensTree);

    return thread.Parse();
}

bool Parser::ParseBufferForUsingNamespace(const wxString& buffer, wxArrayString& result)
{
    ParserThreadOptions opts;

    ParserThread thread(this,
                        wxEmptyString,
                        false,
                        opts,
                        m_pTempTokensTree);

    return thread.ParseBufferForUsingNamespace(buffer, result);
}

bool Parser::RemoveFile(const wxString& filename)
{
    if (!Done())
        return false; // Can't alter the tokens tree if parsing has not finished

    bool result = false;
    wxString file = UnixFilename(filename);
    {
        wxCriticalSectionLocker lock(s_MutexProtection);
        size_t index = m_pTokensTree->GetFileIndex(file);
        result = m_pTokensTree->m_FilesStatus.count(index);

        m_pTokensTree->RemoveFile(filename);
        m_pTokensTree->m_FilesMap.erase(index);
        m_pTokensTree->m_FilesStatus.erase(index);
        m_pTokensTree->m_FilesToBeReparsed.erase(index);
        m_pTokensTree->m_Modified = true;
    }

    return result;
}

bool Parser::Reparse(const wxString& filename, bool isLocal)
{
    if (!Done())
        return false; // if still parsing, exit with error

    wxString file = UnixFilename(filename);
    if (isLocal)
        m_LocalFiles.insert(filename);
    else
        m_LocalFiles.erase(filename);

    {
        wxCriticalSectionLocker lock(s_MutexProtection);
        m_pTokensTree->FlagFileForReparsing(file);
    }

    m_NeedsReparse = true;
    m_Timer.Start(reparse_timer_delay,wxTIMER_ONE_SHOT);

    return true;
}

void Parser::Clear()
{
    DisconnectEvents();
    TerminateAllThreads();
    Manager::ProcessPendingEvents();

    m_IncludeDirs.Clear();
    m_pTokensTree->clear();
    m_pTempTokensTree->clear();

    m_LocalFiles.clear();
    m_GlobalIncludes.clear();

    if (!m_ShuttingDown)
    {
        Manager::ProcessPendingEvents();
        ConnectEvents();
    }

    m_UsingCache = false;
}

bool Parser::ReadFromCache(wxInputStream* f)
{
    bool result = false;
    wxCriticalSectionLocker lock(s_MutexProtection);

    char CACHE_MAGIC_READ[] = "           ";
    m_pTokensTree->clear(); // Clear data

    // File format is like this:
    //
    // CACHE_MAGIC
    // Number of parsed files
    // Number of tokens
    // Parsed files
    // Tokens
    // EOF

//  Begin loading process
    do
    {

        // keep a backup of include dirs
        if (f->Read(CACHE_MAGIC_READ, sizeof(CACHE_MAGIC_READ)).LastRead() != sizeof(CACHE_MAGIC_READ) ||
            strncmp(CACHE_MAGIC, CACHE_MAGIC_READ, sizeof(CACHE_MAGIC_READ) != 0))
            break;
        int fcount = 0, actual_fcount = 0;
        int tcount = 0, actual_tcount = 0;
        int idx;
        if (!LoadIntFromFile(f, &fcount))
            break;
        if (!LoadIntFromFile(f, &tcount))
            break;
        if (fcount < 0)
            break;
        if (tcount < 0)
            break;

        wxString file;
        int nonempty_token = 0;
        Token* token = 0;
        do // do while-false block
        {
            // Filenames
            int i;
            for (i = 0; i < fcount && !f->Eof(); ++i)
            {
                if (!LoadIntFromFile(f,&idx)) // Filename index
                    break;
                if (idx != i)
                    break;
                if (!LoadStringFromFile(f,file)) // Filename data
                    break;
                if (!idx)
                    file.Clear();
                if (file.IsEmpty())
                    idx = 0;
                m_pTokensTree->m_FilenamesMap.insert(file);
                actual_fcount++;
            }
            result = (actual_fcount == fcount);
            if (!result)
                break;
            if (tcount)
                m_pTokensTree->m_Tokens.resize(tcount,0);
            // Tokens
            for (i = 0; i < tcount && !f->Eof(); ++i)
            {
                token = 0;
                if (!LoadIntFromFile(f, &nonempty_token))
                break;
                if (nonempty_token != 0)
                {
                    token = new Token();
                    if (!token->SerializeIn(f))
                    {
                        delete token;
                        token = 0;
                        break;
                    }
                    m_pTokensTree->insert(i,token);
                }
                ++actual_tcount;
            }
            if (actual_tcount != tcount)
                break;
            m_pTokensTree->RecalcFreeList();
            result = true;
        } while(false);

    } while(false);

//  End loading process

    if (result)
        m_UsingCache = true;
    else
        m_pTokensTree->clear();

    m_pTokensTree->m_Modified = false;

    return result;
}

bool Parser::WriteToCache(wxOutputStream* f)
{
    bool result = false;
    wxCriticalSectionLocker lock(s_MutexProtection);
//  Begin saving process

    size_t tcount = m_pTokensTree->m_Tokens.size();
    size_t fcount = m_pTokensTree->m_FilenamesMap.size();
    size_t i = 0;

    // write cache magic
    f->Write(CACHE_MAGIC, sizeof(CACHE_MAGIC));

    SaveIntToFile(f, fcount); // num parsed files
    SaveIntToFile(f, tcount); // num tokens

    // Filenames
    for (i = 0; i < fcount; ++i)
    {
        SaveIntToFile(f,i);
        SaveStringToFile(f,m_pTokensTree->m_FilenamesMap.GetString(i));
    }

    // Tokens

    for (i = 0; i < tcount; ++i)
    {
        TRACE(_T("WriteToCache() : Token #%d, offset %d"),i,f->TellO());
        Token* token = m_pTokensTree->at(i);
        SaveIntToFile(f,(token!=0) ? 1 : 0);
        if (token)
            token->SerializeOut(f);
    }

    result = true;

    if (result)
        m_pTokensTree->m_Modified = false;

//  End saving process
    return result;
}

void Parser::TerminateAllThreads()
{
    m_IgnoreThreadEvents = true;
    m_Pool.AbortAllTasks();
}

void Parser::AddIncludeDir(const wxString& file)
{
    wxString base = file;
    if (base.Last() == wxFILE_SEP_PATH)
        base.RemoveLast();

    if (m_IncludeDirs.Index(base) == wxNOT_FOUND)
    {
        TRACE(_T("AddIncludeDir() : Adding %s"), base.wx_str());
        m_IncludeDirs.Add(base);
    }
}

wxString Parser::FindFirstFileInIncludeDirs(const wxString& file)
{
    wxString FirstFound = m_GlobalIncludes.GetItem(file);
    if (FirstFound.IsEmpty())
    {
        wxArrayString FoundSet = FindFileInIncludeDirs(file,true);
        if (FoundSet.GetCount())
        {
            FirstFound = UnixFilename(FoundSet[0]);
            m_GlobalIncludes.AddItem(file, FirstFound);
        }
    }
    return FirstFound;
}

wxArrayString Parser::FindFileInIncludeDirs(const wxString& file, bool firstonly)
{
    wxArrayString FoundSet;
    for (size_t idxSearch = 0; idxSearch < m_IncludeDirs.GetCount(); ++idxSearch)
    {
        wxString base = m_IncludeDirs[idxSearch];
        wxFileName tmp = file;
        NormalizePath(tmp,base);
        wxString fullname = tmp.GetFullPath();
        if (wxFileExists(fullname))
        {
            FoundSet.Add(fullname);
            if (firstonly)
                break;
        }
    }

    TRACE(_T("FindFileInIncludeDirs() : Searching %s"), file.wx_str());
    TRACE(_T("FindFileInIncludeDirs() : Found %d"), FoundSet.GetCount());

    return FoundSet;
}

void Parser::OnAllThreadsDone(CodeBlocksEvent& event)
{
    if (m_IgnoreThreadEvents)
        return;

    EndStopWatch();

    wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, PARSER_END);
    evt.SetClientData(this);
    wxPostEvent(m_pParent, evt);
}

wxString Parser::GetFullFileName(const wxString& src, const wxString& tgt, bool isGlobal)
{
    wxCriticalSectionLocker lock(s_mutexListProtection);
    wxString fullname(_T("")); // Initialize with Empty String
    if (isGlobal)
    {
        fullname = FindFirstFileInIncludeDirs(tgt);
        if (fullname.IsEmpty())
        {
            // not found; check this case:
            //
            // we had entered the previous file like this: #include <gl/gl.h>
            // and it now does this: #include "glext.h"
            // glext.h was correctly not found above but we can now search
            // for gl/glext.h.
            // if we still not find it, it's not there. A compilation error
            // is imminent (well, almost - I guess the compiler knows a little better ;).
            wxString base = wxFileName(src).GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
            fullname = FindFirstFileInIncludeDirs(base + tgt);
        }
    }

    // NOTE: isGlobal is always true. The following code never executes...

    else // local files are more tricky, since they depend on two filenames
    {
        wxFileName fname(tgt);
        wxFileName source(src);
        if (NormalizePath(fname,source.GetPath(wxPATH_GET_VOLUME)))
        {
            fullname = fname.GetFullPath();
            if (!wxFileExists(fullname))
                fullname.Clear();
        }
    }

    return fullname;
}

void Parser::DoParseFile(const wxString& filename, bool isGlobal)
{
    if (m_IgnoreThreadEvents)
        return;

    if (   (!isGlobal && !m_Options.followLocalIncludes)
        || ( isGlobal && !m_Options.followGlobalIncludes) )
        return;

    if (filename.IsEmpty())
        return;

    LoaderBase* loader = 0; // defer loading until later
    Parse(filename, !isGlobal, loader);
}

void Parser::StartStopWatch()
{
    if (!m_StopWatchRunning)
    {
        m_StopWatchRunning = true;
        m_StopWatch.Start();
    }
}

void Parser::EndStopWatch()
{
    if (m_StopWatchRunning)
    {
        m_StopWatch.Pause();
        m_StopWatchRunning = false;
        m_LastStopWatchTime = m_StopWatch.Time();
    }
}

long Parser::EllapsedTime()
{
    return m_StopWatchRunning ? m_StopWatch.Time() : 0;
}

long Parser::LastParseTime()
{
    return m_LastStopWatchTime;
}

void Parser::OnTimer(wxTimerEvent& event)
{
    ReparseModifiedFiles();
    event.Skip();
}

void Parser::OnBatchTimer(wxTimerEvent& event)
{
#ifndef CODECOMPLETION_PROFILING
    Manager::Get()->GetLogManager()->DebugLog(_T("Starting batch parsing..."));
    if (m_IsBatch)
    {
        m_IsBatch = false;
        StartStopWatch();
        m_Pool.BatchEnd();
    }
#else
    EndStopWatch();
    if (m_LastStopWatchTime > batch_timer_delay)
        m_LastStopWatchTime -= batch_timer_delay;
    else
        m_LastStopWatchTime = 0;
    CodeBlocksEvent evt;
    OnAllThreadsDone(evt);
#endif
}

bool Parser::ReparseModifiedFiles()
{
    if (!m_NeedsReparse || !m_Pool.Done())
        return false;

    Manager::Get()->GetLogManager()->DebugLog(_T("Reparsing saved files..."));
    m_NeedsReparse = false;
    std::queue<wxString> files_list;
    {
        wxCriticalSectionLocker lock(s_MutexProtection);
        TokenFilesSet::iterator it;

        // loop two times so that we reparse modified *header* files first
        // because they usually hold definitions which need to exist
        // when we parse the normal source files...
        for (it = m_pTokensTree->m_FilesToBeReparsed.begin(); it != m_pTokensTree->m_FilesToBeReparsed.end(); ++it)
        {
            m_pTokensTree->RemoveFile(*it);
            wxString filename = m_pTokensTree->m_FilenamesMap.GetString(*it);
            if (FileTypeOf(filename) == ftSource) // ignore source files (*.cpp etc)
                continue;
            files_list.push(filename);
        }
        for (it = m_pTokensTree->m_FilesToBeReparsed.begin(); it != m_pTokensTree->m_FilesToBeReparsed.end(); ++it)
        {
            m_pTokensTree->RemoveFile(*it);
            wxString filename = m_pTokensTree->m_FilenamesMap.GetString(*it);
            if (FileTypeOf(filename) != ftSource) // ignore non-source files (*.h etc)
                continue;
            files_list.push(filename);
        }
    }

    while (!files_list.empty())
    {
        wxString& filename = files_list.front();
        Parse(filename, m_LocalFiles.count(filename));
        files_list.pop();
    }
    return true;
}

size_t Parser::FindTokensInFile(const wxString& fileName, TokenIdxSet& result, short int kindMask)
{
    result.clear();
    wxString file = UnixFilename(fileName);
    TokenIdxSet tmpresult;

    wxCriticalSectionLocker lock(s_MutexProtection);
    TRACE(_T("Parser::FindTokensInFile() : Searching for file '%s' in tokens tree..."), file.wx_str());
    if ( !m_pTokensTree->FindTokensInFile(file, tmpresult, kindMask) )
        return 0;

    for (TokenIdxSet::iterator it = tmpresult.begin(); it != tmpresult.end(); ++it)
    {
        Token* token = m_pTokensTree->at(*it);
        if (token)
            result.insert(*it);
    }
    return result.size();
}
