// @brief 插件控制器，用于访问所有外部插件

#ifndef PLUGINCONTROLLER_H
#define PLUGINCONTROLLER_H

#include <vector>

using namespace std;

class IPrintPlugin;

class pluginController
{
private:
    vector<void *> v_pluginHandle;  // 用于存放所有的句柄
    vector<IPrintPlugin*> v_plugin; // 用于存放所有的插件

public:
    pluginController(/* args */);   // 构造函数
    ~pluginController();            // 虚构函数

    bool InitController(void);      // 初始化控制器，获取所有的插件句柄及其函数
    bool UninitController(void);    // 关闭所有动态库

    bool ProcessHelp(void);         // 执行help()
    bool ProcessPrint(int Func_ID); // 根据Func_ID执行printSC()
};

typedef pluginController pct_t;

#endif
