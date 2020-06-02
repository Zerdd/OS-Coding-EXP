#include <iostream>

using namespace std;

const int FUNC_ID = 2;

extern "C" void printSC()
{
    cout << "Hello Worlid!" << endl;
}

extern "C" void Help()
{
    cout << "Function ID: " << FUNC_ID << "This function used to print HW" << endl;
}

extern "C" int getID(void)
{
    return FUNC_ID;
}