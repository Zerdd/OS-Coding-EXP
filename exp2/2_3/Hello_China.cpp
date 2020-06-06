#include <iostream>

using namespace std;

const int FUNC_ID = 1;

extern "C" void printSC()
{
    cout << "Hello China!" << endl;
}

extern "C" void help()
{
    cout << "Function ID: " << FUNC_ID << " - This function is used to print HC" << endl;
}

extern "C" int getID()
{
    return FUNC_ID;
}
