#include <dlfcn.h>
#include <string.h>
#include <iostream>
#include "pluginCounter.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Please input correct argc!" << endl;
        return 0;
    }
    
    // 执行help命令
    if(strcmp(argv[1], "help") == 0)
    {
        vector<string> vStrNames;
        pc_t pc;

        if (pc.getPluginNames(vStrNames) == false)
        {
            cout << "Get plugin names error!" << endl;
            return 0;
        }

        for (int i = 0; i < vStrNames.size(); i++)
        {
            void *handle = dlopen(vStrNames[i].c_str(), RTLD_LAZY);
            if (handle == 0)
            {
                cout << "dlopen error!" << endl;
                return 0;
            }

            typedef void (*FUNC_HELP)();

            FUNC_HELP dl_help = (FUNC_HELP)dlsym(handle, "help");
            if (dl_help == 0)
            {
                cout << "dlsym error!" << endl;
                return 0;
            }

            (dl_help)();
            
            dlclose(handle);
        }
    }

    int FuncID = atoi(argv[1]);

    return 0;
}