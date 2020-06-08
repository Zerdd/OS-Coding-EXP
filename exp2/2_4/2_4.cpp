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
    
    pct_t pc; // init a plugin controller
    pc.InitController();

    if (strcmp(argv[1], "help") == 0)
    {
        pc.ProcessHelp();
    }
    else
    {
        int FuncID = atoi(argv[1]);

        pc.ProcessPrint(FuncID);
    }

    pc.UninitController();
    
    return 0;
}