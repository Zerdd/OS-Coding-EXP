#include "IPrintPlugin.h"

using namespace std;

class CPrintPlugin : public IPrintPlugin
{
private:
    int FUNC_ID;

public:
    CPrintPlugin()
    {
        FUNC_ID = 2;
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

extern "C" void createOkbj(ipp_t **ppPlugin)
{
    static CPrintPlugin plugin;
    *ppPlugin = &plugin;
}