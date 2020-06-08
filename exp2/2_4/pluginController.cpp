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
    pc_t plugin_counter;                // 实例化一个插件路径遍历器
    plugin_counter.getPluginINFO();     // 获取plugin/下所有的插件路径

    // 目录若为空则返回
    vector<string> vStrPluginNames = plugin_counter.getVNames();
    if (vStrPluginNames.empty())
    {
        cout << "Get plugin path error!" << endl;
        return false;
    }

    // 遍历所有动态链接库
    for (unsigned int i = 0; i < vStrPluginNames.size(); i++)
    {
        cout << vStrPluginNames[i] << endl; // for debug

        typedef int (*PLUGIN_CREATE)(ipp_t**);
        PLUGIN_CREATE CreateProc;

        ipp_t *pPlugin = NULL;

        // void *handle = dlopen("~/OS-Coding-EXP/exp2/2_4/plugin/libHC.so", RTLD_LAZY); // for debug
        void *handle = dlopen(vStrPluginNames[i].c_str(), RTLD_LAZY); // open DL
        if (handle != NULL)
        {
            v_pluginHandle.push_back(handle);

            CreateProc = (PLUGIN_CREATE)dlsym(handle, "createObj");

            if (CreateProc != NULL)
            {
                (CreateProc)(&pPlugin);

                if (pPlugin != NULL)
                {
                    v_plugin.push_back(pPlugin);
                }
            }
        }
        else
        {
            cout << "dlopen()又尼玛是返回NULL辣!" << endl;
            return false;
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
    for (unsigned int i = 0; i < v_plugin.size(); i++)
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
    for (unsigned int i = 0; i < v_plugin.size(); i++)
    {
        v_plugin[i]->help(); 
    }

    return true;
}