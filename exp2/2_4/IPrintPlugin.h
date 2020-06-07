#include <iostream>

#pragma once

class IPrintPlugin
{
private:
    /* data */
public:
    IPrintPlugin(/* args */);
    ~IPrintPlugin();

    virtual void help() = 0;
    virtual void printSC() = 0;
    virtual int  getID() = 0;
};

typedef IPrintPlugin ipp_t