#ifndef PLUGINCOUNTER_H
#define PLUGINCOUNTER_H

#include <vector>
#include <string>

using namespace std;

class pluginCounter
{
private:
    vector<string> vStrNames;
public:
    pluginCounter(/* args */);
    ~pluginCounter();

    bool getPluginNames(vector<string> &vStrNames);
};

typedef pluginCounter pc_t;

#endif
