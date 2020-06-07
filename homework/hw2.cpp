#include <iostream>
#include <string>
#include <dirent.h>
#include <unistd.h>

using namespace std;

bool ListDir(char *path, int depth); // 列出给定路径下的所有目录及其子目录，depth为目录深度，默认为0

int main(int argc, char* argv[]) {
    string path = "."; // 默认list -R当前路径

    // 路径选择，默认为当前路径
    if (argc == 1)
    {
        cout << "You can input path which you want to list, default is correct path." << endl;
    }
    else if (argc == 2)
    {
        path = argv[1];
    }
    else
    {
        cout << "Pleace input correct path!" << endl;
        return 0;
    }
    
    // 调用主功能函数
    if ((ListDir(&path[0], 0) == false))
    {
        cout << "list dir error!" << endl;
    }
    
    return 0;
}

bool ListDir(char *path, int depth)
{
    DIR *dp;
    struct dirent *dirp;

    // 打开目录
    if ((dp = opendir(path)) == NULL)
    {
        cout << "Can't open: " << path << endl;
        return false;
    }

    // 切换到当前目录
    chdir(path);

    // 循环读取目录内各文件
    while((dirp = readdir(dp)) != NULL)
    {
        // 忽略所有隐藏目录
        if (dirp->d_name[0] == '.')
        {
            continue;
        }
        
        // 判断该文件是否为目录
        if (dirp->d_type == DT_DIR)
        {
            cout << string(depth, ' ') << dirp->d_name << "/" << endl;
            ListDir(dirp->d_name, depth + 4);  // 递归子目录，depth顺便用于格式化输出
        }
        else
        {
            cout << string(depth, ' ') << dirp->d_name << endl;
        }
    }

    // 返回父目录
    chdir("..");

    // 关闭
    closedir(dp);

    return true;
}