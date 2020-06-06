#include <iostream>

using namespace std;

const int FUNC_ID = 2;

extern "C" void printSC()
{
    cout << "Hello World!" << endl;
}

extern "C" void help()
{
    cout << "Function ID: " << FUNC_ID << " - This function is used to print HW" << endl;
}

extern "C" int getID()
{
    return FUNC_ID;
}
