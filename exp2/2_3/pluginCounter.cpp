#include <iostream>
#include <dirent.h>
#include <string.h>
#include "pluginCounter.h"

pluginCounter::pluginCounter() {}
pluginCounter::~pluginCounter() {}

bool pc_t::getPluginNames(vector<string> &vStrNames)
{
    DIR *dp;
    struct dirent *dirp;

    dp = opendir("./plugin/");
    if (dp == 0)
    {
        cout << "open dir error!" << endl;
        return false;
    }

    while ((dirp = readdir(dp)) != NULL)
    {
        // skip "." and ".."
        if ((strcmp(dirp->d_name, ".")) == 0 || (strcmp(dirp->d_name, "..")) == 0)
        {
            continue;
        }

        string str = "./plugin/";
        str += dirp->d_name;
        vStrNames.push_back(str);
    }

    closedir(dp);
    
    return true;
}