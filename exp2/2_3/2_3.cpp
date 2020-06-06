#include <dlfcn.h>
#include <string.h>
#include <iostream>
#include "pluginCounter.h"

using namespace std;

int FuncID = 0;

void dl_help(vector<string> vStrNames);                         // 路径里所有动态库里执行help()
void dl_printSC(vector<string> vStrNames, vector<int> vIntID);  // 根据FuncID决定那个动态库执行printSC()

int main(int argc, char *argv[])
{
    // 检查参数
    if (argc != 2)
    {
        cout << "Please input correct argc!" << endl;
        return 0;
    }

    vector<string> vStrNames;
    vector<int> vIntID;
    pc_t pc;

    // 获取plunin/里所有插件信息
    if (pc.getPluginINFO() == false)
    {
        cout << "Get plugin names error!" << endl;
        return 0;
    }

    vStrNames = pc.getVNames();
    vIntID = pc.getVID();

    // 执行help()
    if (strcmp(argv[1], "help") == 0)
    {
        dl_help(vStrNames);
    }
    // 需要执行哪一个插件
    else if (strcmp(argv[1], "1") == 0 || strcmp(argv[1], "2") == 0)
    {
        FuncID = atoi(argv[1]); // 转换整型
        dl_printSC(vStrNames, vIntID);
    }
    // 第二个参数不正确
    else
    {
        cout << "Please input correct cmd!" << endl;
    }

    return 0;
}

void dl_help(vector<string> vStrNames)
{
    // 遍历所有插件执行help()
    for (int i = 0; i < vStrNames.size(); i++)
    {
        // 打开动态库
        void *handle = dlopen(vStrNames[i].c_str(), RTLD_LAZY);
        if (handle == 0)
        {
            cout << "dlopen error in help()!" << endl;
            return;
        }

        // 映射函数
        typedef void (*FUNC_HELP)();
        FUNC_HELP dl_help = (FUNC_HELP)dlsym(handle, "help");
        if (dl_help == 0)
        {
            cout << "dlsym error!" << endl;
            return;
        }

        // 执行函数
        (dl_help)();

        // 关闭动态库
        dlclose(handle);
    }
}

void dl_printSC(vector<string> vStrNames, vector<int> vIntID)
{
    // 遍历动态库
    for (int i = 0; i < vStrNames.size(); i++)
    {
        // 遇到要执行的动态库
        if (vIntID[i] == FuncID)
        {
            // 打开动态库
            void *handle = dlopen(vStrNames[i].c_str(), RTLD_LAZY);
            if (handle == 0)
            {
                cout << "dlopen error in printSC()!" << endl;
                return;
            }

            // 映射函数
            typedef void (*FUNC_PRINTSC)();
            FUNC_PRINTSC dl_printsc = (FUNC_PRINTSC)dlsym(handle, "printSC");
            if (dl_printsc == 0)
            {
                cout << "dlsym error!" << endl;
                return;
            }

            // 执行函数
            (dl_printsc)();

            // 关闭动态库
            dlclose(handle);
        }
    }
}
