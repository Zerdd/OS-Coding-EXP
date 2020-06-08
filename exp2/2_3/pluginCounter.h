#ifndef PLUGINCOUNTER_H
#define PLUGINCOUNTER_H

#include <vector>
#include <string>

using namespace std;

class pluginCounter
{
private:
    vector<string> vStrNames;    // plugin/下所有插件的路径
    vector<int>    vIntID;       // plugin/下所有插件的ID
public:
    pluginCounter(/* args */);   // 构造函数
    ~pluginCounter();            // 析构函数

    bool getPluginINFO();        // 获取plugin/里所有插件的路径&ID
    vector<string> getVNames();  // 返回所有插件的路径
    vector<int>    getVID();     // 返回所有插件的ID
};

typedef pluginCounter pc_t;

#endif
