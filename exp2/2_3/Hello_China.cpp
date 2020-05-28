#include <iostream>

using namespace std;

const int FUNC_ID = 1;

extern "C" void printSC()
{
    cout << "Hello China!" << endl;
}

extern "C" void Help()
{
    cout << "Function ID: " << FUNC_ID << "This function used to print HC" << endl;
}

extern "C" int getID(void)
{
    return FUNC_ID;
}