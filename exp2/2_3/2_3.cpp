#include <iostream>
#include <dlfcn.h>
#include <dirent.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 

using namespace std;

class foo
{
private:
    DIR *pDir;
    struct dirent *pDirent;
    char* path;

public:
    foo(char *p) { path = p;}
    virtual ~foo() {}

    bool getID();
    bool getHelp();
    bool printSC();
    bool loopDir(char* func);
};

bool foo::getHelp()
{
    return (loopDir("help")) ? true : false;
}

bool foo::getID()
{
    return (loopDir("ID")) ? true : false;
}

bool foo::printSC()
{
    return (loopDir("print")) ? true : false;
}

// TODO transplant loopDir()
bool foo::loopDir(char* func)
{
    pDir = opendir(path);
    if (pDir == NULL)
    {
        cout << "open dir error!" << endl;
        return false;
    }

    while ((pDirent = readdir(pDir)) != NULL)
    {
        // skip "." and ".."
        if ((strcmp(pDirent->d_name, ".")) == 0 || (strcmp(pDirent->d_name, "..")) == 0)
        {
            continue;
        }
        
        string str = "./";
        str += (string)(pDirent->d_name);
        void *handle = dlopen(&str[0], RTLD_LAZY);
        if (0 == handle)
        {
            cout << "dlopen error!" << endl;
            return false;
        }
    }
    
}

int main()
{
    DIR *pDir;
    struct dirent *pDirent;
    char CDir[100];
    char *error;

    pDir = opendir("./plugin");
    if (pDir == NULL)
    {
        cout << "Open dir error!" << endl;
        return 0;
    }

    chdir("./plugin/");
    cout << "当前工作目录: " << getcwd(CDir, sizeof(CDir)) << endl << endl;

    while ((pDirent = readdir(pDir)) != NULL)
    {
        // 跳过.和..两个目录
        // cout << "File name: " << pDirent->d_name << endl;
        if (strlen(pDirent->d_name) <= 2)
        {
            continue;
        }

        // 打开动态链接库
        string str = "./";
        str += (string)(pDirent->d_name);
        void *handle = dlopen(&str[0], RTLD_LAZY);
        if (0 == handle)
        {
            cout << "dlopen error" << endl;
            fprintf (stderr, "%s \n", dlerror());
            return 0;
        }

        // 映射动态链接库的函数
        typedef void (*Fun)();

        Fun f1 = (Fun)dlsym(handle, "printSC");

        if (0 == f1)
        {
            cout << "f1 error" << endl;
            char *str = dlerror();
            cout << str << endl;
            return 0;
        }

        (*f1)();

        // 卸载动态链接库
        dlclose(handle);
    }

    chdir("./");

    if ((closedir(pDir)) == -1)
    {
        cout << "Close dir error!" << endl;
        return 0;
    }

    return 0;
}