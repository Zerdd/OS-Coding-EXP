#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <string.h>

using namespace std;

int main()
{
    string fw("first write\n");
    string sw("second write\n");
    char IS_EOF; // 用于判断文件是否非空

    // 打开文件
    int fd = open("h1.txt", O_RDWR | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        cout << "open error!" << endl;
        return 0;
    }

    // 判断文件是否非空，是就清空
    if (read(fd, &IS_EOF, 1) == -1)
    {
        cout << "read error!" << endl;
        close(fd);
    }
    if (IS_EOF != EOF)
    {
        ftruncate(fd, 0);
    }

    // 第一次写入
    if (write(fd, &fw[0], fw.size()) == -1)
    {
        cout << "first write error!" << endl;
        close(fd);
        return 0;
    }

    // 重新使用O_APPEND标志打开文件
    close(fd);
    fd = open("h1.txt", O_RDWR | O_APPEND);
    if (fd == -1)
    {
        cout << "reopen error!" << endl;
        return 0;
    }

    // 尝试使用lseek修改光标至首部并写入
    int newPos = lseek(fd, 0, SEEK_SET);
    if (write(fd, &sw[0], sw.size()) == -1)
    {
        cout << "second write error!" << endl;
        close(fd);
        return 0;
    }

    // 输出文本内容
    system("cat h1.txt");

    close(fd);

    return 0;
}