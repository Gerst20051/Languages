/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision: 6125 $
 * $Id: sc_plugin.cpp 6125 2010-01-31 03:56:32Z biplab $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/sdk/scripting/bindings/sc_plugin.cpp $
 */

#include <sdk_precomp.h>
#include "sc_plugin.h"
#include <manager.h>
#include <scriptingmanager.h>
#include <wx/menu.h>

#include <map>

namespace ScriptBindings {
namespace ScriptPluginWrapper {

// struct and map for mapping script plugins to menu callbacks
struct MenuCallback
{
    SquirrelObject object;
    int menuIndex;
};
typedef std::map<int, MenuCallback> ModuleMenuCallbacks;
ModuleMenuCallbacks s_MenuCallbacks;

// master list of registered script plugins
typedef std::map<wxString, SquirrelObject> ScriptPlugins;
ScriptPlugins s_ScriptPlugins;

// list of registered script plugins menubar items
typedef std::map<wxString, MenuItemsManager> ScriptPluginsMenus;
ScriptPluginsMenus s_ScriptPluginsMenus;

////////////////////////////////////////////////////////////////////////////////
// ask the script plugin what menus to add in the menubar
// and return an integer array of the menu IDs
////////////////////////////////////////////////////////////////////////////////
wxArrayInt CreateMenu(const wxString& name)
{
    wxArrayInt ret;

    ScriptPlugins::iterator it = s_ScriptPlugins.find(name);
    if (it == s_ScriptPlugins.end())
        return ret;
    SquirrelObject& o = it->second;

    ScriptPluginsMenus::iterator itm = s_ScriptPluginsMenus.find(name);
    if (itm == s_ScriptPluginsMenus.end())
    {
        itm = s_ScriptPluginsMenus.insert(s_ScriptPluginsMenus.end(), std::make_pair(name, MenuItemsManager(false)));
    }
    MenuItemsManager& mi = itm->second;

    SqPlus::SquirrelFunction<wxArrayString&> f(o, "GetMenu");
    if (f.func.IsNull())
        return ret;

    wxArrayString arr;
    try
    {
        arr = f();
    }
    catch (SquirrelError e)
    {
        cbMessageBox(cbC2U(e.desc), _("Script error"), wxICON_ERROR);
        return ret;
    }

    if (arr.GetCount())
    {
        for (size_t i = 0; i < arr.GetCount(); ++i)
        {
            int id = wxNewId();
            mi.CreateFromString(arr[i], id);

            ret.Add(id);

            MenuCallback callback;
            callback.object = it->second;
            callback.menuIndex = i;
            s_MenuCallbacks.insert(s_MenuCallbacks.end(), std::make_pair(id, callback));
        }
    }

    return ret;
}

////////////////////////////////////////////////////////////////////////////////
// ask the script plugin what items to add in the context menu
// and return an integer array of the menu IDs
////////////////////////////////////////////////////////////////////////////////
wxArrayInt CreateModuleMenu(const ModuleType typ, wxMenu* menu, const FileTreeData* data)
{
    wxArrayInt ret;

    ScriptPlugins::iterator it;
    for (it = s_ScriptPlugins.begin(); it != s_ScriptPlugins.end(); ++it)
    {
        SquirrelObject& o = it->second;
        SqPlus::SquirrelFunction<wxArrayString&> f(o, "GetModuleMenu");
        if (f.func.IsNull())
            continue;

        wxArrayString arr;
        try
        {
            arr = f(typ, data);
        }
        catch (SquirrelError e)
        {
            cbMessageBox(cbC2U(e.desc), _("Script error"), wxICON_ERROR);
            continue;
        }

        if (arr.GetCount()==1) // exactly one menu entry
        {
            int id = wxNewId();
            menu->Append(id, arr[0]);
            ret.Add(id);

            MenuCallback callback;
            callback.object = it->second;
            callback.menuIndex = 0;
            s_MenuCallbacks.insert(s_MenuCallbacks.end(), std::make_pair(id, callback));
        }
        else if (arr.GetCount()>1) // more menu entries -> create sub-menu
        {
            wxMenu* sub = new wxMenu;
            for (size_t i = 0; i < arr.GetCount(); ++i)
            {
                int id = wxNewId();
                sub->Append(id, arr[i]);

                ret.Add(id);

                MenuCallback callback;
                callback.object = it->second;
                callback.menuIndex = i;
                s_MenuCallbacks.insert(s_MenuCallbacks.end(), std::make_pair(id, callback));
            }
            menu->Append(-1, it->first, sub);
        }
    }
    return ret;
}

////////////////////////////////////////////////////////////////////////////////
// callback for script plugins menubar entries
////////////////////////////////////////////////////////////////////////////////
void OnScriptMenu(int id)
{
    ModuleMenuCallbacks::iterator it;
    it = s_MenuCallbacks.find(id);
    if (it != s_MenuCallbacks.end())
    {
        MenuCallback& callback = it->second;
        SqPlus::SquirrelFunction<void> f(callback.object, "OnMenuClicked");
        if (!f.func.IsNull())
        {
            try
            {
                f(callback.menuIndex);
            }
            catch (SquirrelError e)
            {
                cbMessageBox(cbC2U(e.desc), _("Script error"), wxICON_ERROR);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// callback for script plugins context menu entries
////////////////////////////////////////////////////////////////////////////////
void OnScriptModuleMenu(int id)
{
    ModuleMenuCallbacks::iterator it;
    it = s_MenuCallbacks.find(id);
    if (it != s_MenuCallbacks.end())
    {
        MenuCallback& callback = it->second;
        SqPlus::SquirrelFunction<void> f(callback.object, "OnModuleMenuClicked");
        if (!f.func.IsNull())
        {
            try
            {
                f(callback.menuIndex);
            }
            catch (SquirrelError e)
            {
                cbMessageBox(cbC2U(e.desc), _("Script error"), wxICON_ERROR);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// register a script plugin (script-bound function)
////////////////////////////////////////////////////////////////////////////////
SQInteger RegisterPlugin(HSQUIRRELVM v)
{
    // get squirrel object to register from stack
    SquirrelObject o;
    o.AttachToStackObject(2);

    // first verify that there is a member function to retrieve the plugin info
    if (!o.Exists("GetPluginInfo"))
        return sq_throwerror(v, "Not a script plugin!");

    // ask for its registration name
    SqPlus::SquirrelFunction<PluginInfo&> f(o, "GetPluginInfo");
    PluginInfo& info = f();
    wxString s = info.name;

    // look if a script plugin with the same name already exists
    ScriptPlugins::iterator it = s_ScriptPlugins.find(s);
    if (it != s_ScriptPlugins.end())
    {
        // already exists; release the old one
        s_ScriptPlugins.erase(it);
        Manager::Get()->GetLogManager()->Log(_("Script plugin unregistered: ") + s);
    }

    // finally, register this script plugin
    it = s_ScriptPlugins.insert(s_ScriptPlugins.end(), std::make_pair(s, o));
    Manager::Get()->GetLogManager()->Log(_("Script plugin registered: ") + s);

    Manager::Get()->GetScriptingManager()->RegisterScriptPlugin(s, CreateMenu(s));

    // this function returns nothing on the squirrel stack
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// get a script plugin squirrel object (script-bound function)
////////////////////////////////////////////////////////////////////////////////
SQInteger GetPlugin(HSQUIRRELVM v)
{
    StackHandler sa(v);

    // get the script plugin's name
    const wxString& name = *SqPlus::GetInstance<wxString,false>(v, 2);

    // search for it in the registered script plugins list
    ScriptPlugins::iterator it = s_ScriptPlugins.find(name);
    if (it != s_ScriptPlugins.end())
    {
        // found; return the squirrel object
        return sa.Return(it->second);
    }

    // not found; return nothing
    return sa.Return();
}

////////////////////////////////////////////////////////////////////////////////
// execute a script plugin (script-bound function)
////////////////////////////////////////////////////////////////////////////////
int ExecutePlugin(const wxString& name)
{
    // look for script plugin
    ScriptPlugins::iterator it = s_ScriptPlugins.find(name);
    if (it != s_ScriptPlugins.end())
    {
        // found; execute it
        SquirrelObject& o = it->second;
        SqPlus::SquirrelFunction<int> f(o, "Execute");
        if (!f.func.IsNull())
        {
            try
            {
                f();
            }
            catch (SquirrelError e)
            {
                cbMessageBox(cbC2U(e.desc), _("Script error"), wxICON_ERROR);
            }
        }
    }
    return -1;
}

}; // namespace ScriptPluginWrapper

// base script plugin class
const char* s_cbScriptPlugin =
    "class cbScriptPlugin\n"
    "{\n"
    "    info = PluginInfo();\n"
    "    constructor()\n"
    "    {\n"
    "        info.name = _T(\"cbScriptPlugin\");\n"
    "        info.title = _T(\"Base script plugin\");\n"
    "        info.version = _T(\"0.1a\");\n"
    "        info.license = _T(\"GPL\");\n"
    "    }\n"
    "    function GetPluginInfo()\n"
    "    {\n"
    "        return info;\n"
    "    }\n"
    "    function GetMenu()\n"
    "    {\n"
    "        return wxArrayString();\n"
    "    }\n"
    "    function GetModuleMenu(who,data)\n"
    "    {\n"
    "        return wxArrayString();\n"
    "    }\n"
    "    function Execute()\n"
    "    {\n"
    "        LogDebug(info.name + _T(\"::Run() : not implemented\"));\n"
    "        return -1;\n"
    "    }\n"
    "    function OnMenuClicked(index)\n"
    "    {\n"
    "        LogDebug(info.name + _T(\": menu clicked: \") + index);\n"
    "    }\n"
    "    function OnModuleMenuClicked(index)\n"
    "    {\n"
    "        LogDebug(info.name + _T(\": module menu clicked: \") + index);\n"
    "    }\n"
    "}\n";

////////////////////////////////////////////////////////////////////////////////
// register the script plugin framework
////////////////////////////////////////////////////////////////////////////////
void Register_ScriptPlugin()
{
    SqPlus::RegisterGlobal(&ScriptPluginWrapper::ExecutePlugin, "ExecutePlugin");
    SquirrelVM::CreateFunctionGlobal(&ScriptPluginWrapper::GetPlugin, "GetPlugin", "*");
    SquirrelVM::CreateFunctionGlobal(&ScriptPluginWrapper::RegisterPlugin, "RegisterPlugin", "*");

    // load base script plugin

    // WARNING: we CANNOT use ScriptingManager::LoadBuffer() because we have reached here
    // by a call from inside ScriptingManager's constructor. This would cause an infinite
    // loop and the app would die with a stack overflow. We got to load the script manually...
    // we also have to disable the printfunc for a while

    SQPRINTFUNCTION oldPrintFunc = sq_getprintfunc(SquirrelVM::GetVMPtr());
    sq_setprintfunc(SquirrelVM::GetVMPtr(), 0);

    // compile and run script
    SquirrelObject script;
    try
    {
        script = SquirrelVM::CompileBuffer(s_cbScriptPlugin, "cbScriptPlugin");
        SquirrelVM::RunScript(script);
    }
    catch (SquirrelError e)
    {
        cbMessageBox(wxString::Format(_("Failed to register script plugins framework.\n\n%s"),
                                        cbC2U(e.desc).c_str()),
                    _("Script compile error"), wxICON_ERROR);
    }

    // restore the printfunc
    sq_setprintfunc(SquirrelVM::GetVMPtr(), oldPrintFunc);
}

}; // namespace ScriptBindings

