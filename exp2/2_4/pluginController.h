#ifndef PLUGINCONTROLLER_H
#define PLUGINCONTROLLER_H

#include <vector>

using namespace std;

class IPrintPlugin;

class pluginController
{
private:
    vector<void *> v_pluginHandle; // for DL handle
    vector<IPrintPlugin*> v_plugin; // for plugin
public:
    pluginController(/* args */);
    ~pluginController();

    bool InitController(void);
    bool UninitController(void);

    bool ProcessHelp(void);
    bool ProcessPrint(int Func_ID);
};

typedef pluginController pct_t;

#endif
