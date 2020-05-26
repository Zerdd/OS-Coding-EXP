#include <iostream>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

using namespace std;

class CA_LL
{
public:

  int i = 1;

  bool Serialize(const char *pFilePath)
  {
    // 打开文件，不存在就创建
    // if (fd = open(pFilePath, O_WRONLY | O_CREAT | O_TRUNC) == -1)   
    int fd = open(pFilePath, O_WRONLY | O_CREAT | O_TRUNC);
    if (fd == -1 ) {
      cout << "Open error!" << endl;
      return false;
    }

    cout << "open fd:" << fd << endl;

    // 写入
    if (write(fd, &i, sizeof(int)) == -1) {
      close(fd);
      cout << "Write error!" << endl;
      return false;
    }

    cout << "writed size:" << fd << endl;

    // 关闭
    if (close(fd) == -1) {
      cout << "Close error!" << endl;
      return false;
    }

    cout << "Serialize success!" << endl;
    return true;
  }


  bool Deserialize(const char *pFilePath)
  {
    int fd = open(pFilePath, O_RDONLY);
    if (fd == -1) {
      close(fd);
      cout << "Open error!" << endl;
      return false;
    }

    if (read(fd, &i, sizeof(int)) == -1) {
      close(fd);
      cout << "Read error!" << endl;
      return false;
    }

    if (close(fd) == -1) {
      cout << "Close error!" << endl;
      return false;
    }

    cout << "Deserialize success!" << endl;
    cout << "a.i = " << i << endl;
    return true;
  }
};

int main()
{
  CA_LL a;
  a.i = 20;

  if (a.Serialize("data") == false) {
    cout << "Serialize error!" << endl;  
  }

  if (a.Deserialize("data") == false) {
    cout << "Deserialize error!" << endl;
  }

  return 0;
}