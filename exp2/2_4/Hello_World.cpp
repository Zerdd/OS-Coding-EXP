#include <iostream>
#include "IPrintPlugin.h"

using namespace std;

int FUNC_ID = 2;

class CPrintPlugin : public ipp_t
{
private:

public:
    CPrintPlugin()
    {
    }

    virtual ~CPrintPlugin()
    {
    }

    virtual void printSC()
    {
        cout << "Hello World!" << endl;
    }

    virtual void help()
    {
        cout << "Function ID: " << FUNC_ID << " - This function is used to print HW" << endl;
    }

    virtual int getID()
    {
        return FUNC_ID;
    }
};

extern "C" void createObj(ipp_t **ppPlugin)
{
    static CPrintPlugin plugin;
    *ppPlugin = &plugin;
}