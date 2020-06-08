#include "IPrintPlugin.h"
#include "pluginController.h"
#include "pluginCounter.h"
#include <string.h>

using namespace std;

int main(int argc, char *argv[])
{
    // 检查参数
    if (argc != 2)
    {
        cout << "Please input correct argc!" << endl;
        return 0;
    }
    
    pct_t pc;               // 实例化一个插件控制器
    
    // 初始化插件控制器
    if (pc.InitController() == false)
    {
        cout << "Init plugin controller error!" << endl;
        return 0;
    }   

    // 用户输入help
    if (strcmp(argv[1], "help") == 0)
    {
        if (pc.ProcessHelp() == false)
        {
            cout << "ProcessHelp() error!" << endl;
            return 0;
        }
    }
    else
    {
        int FuncID = atoi(argv[1]);

        pc.ProcessPrint(FuncID);
    }

    pc.UninitController();
    
    return 0;
}