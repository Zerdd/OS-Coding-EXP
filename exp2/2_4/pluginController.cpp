#include <dlfcn.h>
#include <string>
#include <vector>
#include "IPrintPlugin.h"
#include "pluginCounter.h"
#include "pluginController.h"

using namespace std;

pluginController::pluginController(/* args */)
{
}

pluginController::~pluginController()
{
}

bool pct_t::InitController(void)
{
    pc_t plugin_counter;                // init plugin counter
    plugin_counter.getPluginINFO();     // get every plugin path from plugin/

    // if there is no plugin, return
    vector<string> vStrPluginNames = plugin_counter.getVNames();
    if (vStrPluginNames.empty())
    {
        cout << "Get plugin path error!" << endl;
        return false;
    }

    for (unsigned int i = 0; i < vStrPluginNames.size(); i++)
    {
        typedef int (*PLUGINCREATE)(IPrintPlugin**);
        PLUGINCREATE CreateProc;

        ipp_t *pPlugin = NULL;

        void* handle = dlopen(vStrPluginNames[i].c_str(), RTLD_LAZY); // open DL
        
        if (!handle)
        {
            v_pluginHandle.push_back(handle);

            CreateProc = (PLUGINCREATE)dlsym(handle, "createObj");

            if (!CreateProc)
            {
                (CreateProc)(&pPlugin);

                if (!pPlugin)
                {
                    v_plugin.push_back(pPlugin);
                }
            }
        }
        
    }
    
    return true;
}

bool pct_t::UninitController(void)
{
    for (unsigned int i = 0; i < v_pluginHandle.size(); i++)
    {
        dlclose(v_pluginHandle[i]);
    }

    return true;
}

bool pct_t::ProcessPrint(int FuncID)
{
    for (unsigned i = 0; i < v_plugin.size(); i++)
    {
        if (v_plugin[i]->getID() == FuncID)
        {
            v_plugin[i]->printSC();
            break;
        }       
    }
    
    return true;
}

bool pct_t::ProcessHelp(void)
{
    for (unsigned i = 0; i < v_plugin.size(); i++)
    {
        v_plugin[i]->help(); 
    }

    return true;
}