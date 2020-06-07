#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <iomanip>
#include <string.h>
#include <cmath>

using namespace std;

// 用于文件的权限判断
#define rd 'r'
#define wr 'w'
#define ex 'x'
#define sp '-'

// 用于格式化输出
int uid_len = 0;
int gid_len = 0;
int siz_len = 0;

bool getLen(char *path);     // 分别获取用户名、组名和文件大小的最长长度
bool ListDir_L(char *path);  // 主功能函数，输出给定目录下各文件的信息
string getINFO(struct stat); // 获取文件的权限信息

int main(int argc, char *argv[])
{
    string path = "."; // 默认list -l当前路径

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

    if ((getLen(&path[0]) == false))
    {
        cout << "Get getLen info error!" << endl;
    }

    if ((ListDir_L(&path[0]) == false))
    {
        cout << "list dir error!" << endl;
    }

    return 0;
}

bool getLen(char *path)
{
    DIR *dp;
    struct dirent *dirp;
    struct stat fstat;

    // 打开目录
    if ((dp = opendir(path)) == NULL)
    {
        cout << "Can't open: " << path << endl;
        return false;
    }

    // 循环读取目录内各文件
    while ((dirp = readdir(dp)) != NULL)
    {
        // 忽略所有隐藏目录
        if (dirp->d_name[0] == '.')
        {
            continue;
        }

        // 获取文件的stat信息
        if ((stat(dirp->d_name, &fstat)) == -1)
        {
            cout << "Get file stat error!" << endl;
            return false;
        }

        // 获取最长的uid位数
        struct passwd *usr;
        usr = getpwuid(fstat.st_uid);
        uid_len = (strlen(usr->pw_name) > uid_len) ? strlen(usr->pw_name) : uid_len;

        // 获取最长的gid位数
        struct group *grp;
        grp = getgrgid(fstat.st_gid);
        gid_len = (strlen(grp->gr_name) > uid_len) ? strlen(grp->gr_name) : uid_len;

        // 获取最长的文件大小位数--
        siz_len = ((int)log10(fstat.st_size) > siz_len) ? (int)log10(fstat.st_size) : siz_len;
    }

    closedir(dp);

    siz_len++;

    return true;
}

bool ListDir_L(char *path)
{
    DIR *dp;
    struct dirent *dirp;
    struct stat fstat;
    string INFO; // 存放文件权限信息

    // 打开目录
    if ((dp = opendir(path)) == NULL)
    {
        cout << "Can't open: " << path << endl;
        return false;
    }

    // 循环读取目录内各文件
    while ((dirp = readdir(dp)) != NULL)
    {
        INFO.clear(); // 清空文件权限信息

        // 跳过所有隐藏目录
        if (dirp->d_name[0] == '.')
        {
            continue;
        }

        // 获取文件的stat信息
        if ((stat(dirp->d_name, &fstat)) == -1)
        {
            cout << "Get file stat error!" << endl;
            return false;
        }

        // 文件类型&权限
        INFO = getINFO(fstat);
        cout << INFO << ' ';

        // 硬连接数
        cout << fstat.st_nlink << ' ';

        // 用户名
        struct passwd *usr;
        usr = getpwuid(fstat.st_uid);
        cout << setw(uid_len) << setfill(' ') << left << usr->pw_name << ' ';

        // 用户所在组
        struct group *grp;
        grp = getgrgid(fstat.st_gid);
        cout << setw(gid_len) << setfill(' ') << left << grp->gr_name << ' ';

        // 文件大小
        cout << setw(siz_len) << setfill(' ') << right << fstat.st_size << ' ';

        // 最后修改时间
        string month[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                          "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
        struct tm *ptime;

        ptime = localtime(&fstat.st_mtime);

        cout << setw(3) << month[ptime->tm_mon] << ' ';                   // 月份
        cout << ptime->tm_mday << ' ';                                    // 星期几
        cout << setw(2) << setfill('0') << left << ptime->tm_hour << ':'; // 小时
        cout << setw(2) << setfill('0') << right << ptime->tm_min << ' '; // 分钟

        cout << dirp->d_name << endl; // 文件名
    }

    closedir(dp);

    return 1;
}

string getINFO(struct stat fstat)
{
    string INFO; // 存放文件权限信息

    // 文件类型
    if (S_ISDIR(fstat.st_mode))
        INFO += 'd';
    else if (S_ISREG(fstat.st_mode))
        INFO += '-';
    else if (S_ISLNK(fstat.st_mode))
        INFO += 'l';
    else if (S_ISSOCK(fstat.st_mode))
        INFO += 's';
    else if (S_ISCHR(fstat.st_mode))
        INFO += 'c';
    else if (S_ISBLK(fstat.st_mode))
        INFO += 'b';
    else if (S_ISFIFO(fstat.st_mode))
        INFO += 'f';

    // 文件读写权限
    INFO += (fstat.st_mode & S_IRUSR) ? rd : sp;
    INFO += (fstat.st_mode & S_IWUSR) ? wr : sp;
    INFO += (fstat.st_mode & S_IXUSR) ? ex : sp;
    INFO += (fstat.st_mode & S_IRGRP) ? rd : sp;
    INFO += (fstat.st_mode & S_IWGRP) ? wr : sp;
    INFO += (fstat.st_mode & S_IXGRP) ? ex : sp;
    INFO += (fstat.st_mode & S_IROTH) ? rd : sp;
    INFO += (fstat.st_mode & S_IWOTH) ? wr : sp;
    INFO += (fstat.st_mode & S_IXOTH) ? ex : sp;

    /* 为了方便cout流格式化输出，以下功能的实现均已搬到lisDir() */

    // 文件的硬连接数
    // INFO += ' ';
    // INFO += to_string(fstat.st_nlink);

    // 用户名
    // struct passwd *usr;
    // usr = getpwuid(fstat.st_uid);

    // INFO += ' ';
    // INFO += usr->pw_name;

    // 用户所在组
    // struct group  *grp;
    // grp = getgrgid(fstat.st_gid);

    // INFO += ' ';
    // INFO += grp->gr_name;

    // 文件大小
    // INFO += ' ';
    // INFO += to_string(fstat.st_size);

    // 最后修改时间
    // string month[] = {"Jan", "Feb", "Mar", "Apr", "May", "June",
    //                   "July", "Aug", "Sept", "Oct", "Nov", "Dec"};
    // struct tm *ptime;

    // ptime = localtime(&fstat.st_mtime);

    // INFO += ' ';
    // INFO += month[ptime->tm_mon]; // 月份
    // INFO += ' ';
    // INFO += to_string(ptime->tm_mday); // 星期几
    // INFO += ' ';
    // INFO += to_string(ptime->tm_hour); // 小时
    // INFO += ':';
    // if (ptime->tm_min < 10)
    //     INFO += to_string(0);
    // INFO += to_string(ptime->tm_min); // 分钟

    // cout << "getINFO: " << INFO ;

    return INFO;
}
