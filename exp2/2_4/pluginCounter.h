#ifndef PLUGINCOUNTER_H
#define PLUGINCOUNTER_H

#include <vector>
#include <string>

using namespace std;

class pluginCounter
{
private:
    vector<string> vStrNames;    // plugin/下所有插件的路径
public:
    pluginCounter(/* args */);
    ~pluginCounter();

    bool getPluginINFO();        // 获取plugin/里所有插件的路径&ID
    vector<string> getVNames();  // 返回所有插件的路径
    vector<int>    getVID();     // 返回所有插件的ID
};

typedef pluginCounter pc_t;

#endif
