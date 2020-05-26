#include <iostream>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

using namespace std;

class Content
{
public:
  Content()
  {
    i = 0;
  }

  void SetContent(int j)
  {
    i = j;
  }

  virtual ~Content() {}

  void ShowContent()
  {
    std::cout << "Show_Content: " << i << std::endl;
  }

  bool Serialize(const char *pFilePath);
  bool Deserialize(const char *pFilePath);

private:
  int i;
};

bool Content::Serialize(const char *pFilePath)
{
  // 打开文件，不存在就创建
  int fd = open(pFilePath, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if (fd == -1)
  {
    cout << "Open error!" << endl;
    return false;
  }

  cout << "open fd:" << fd << endl;

  // 写入
  if (write(fd, &i, sizeof(int)) == -1)
  {
    close(fd);
    cout << "Write error!" << endl;
    return false;
  }

  cout << "writed size:" << fd << endl;

  // 关闭
  if (close(fd) == -1)
  {
    cout << "Close error!" << endl;
    return false;
  }

  cout << "Serialize success!" << endl;
  return true;
}

bool Content::Deserialize(const char *pFilePath)
{
  // 打开已序列化的文件
  int fd = open(pFilePath, O_RDONLY);
  if (fd == -1)
  {
    close(fd);
    cout << "Open error!" << endl;
    return false;
  }

  if (read(fd, &i, sizeof(int)) == -1)
  {
    close(fd);
    cout << "Read error!" << endl;
    return false;
  }

  if (close(fd) == -1)
  {
    cout << "Close error!" << endl;
    return false;
  }

  cout << "Deserialize success!" << endl;
  cout << "a.i = " << i << endl;
  return true;
}

int main()
{
  Content a;
  a.SetContent(10);

  {
    if (a.Serialize("data") == false)
    {
      cout << "Serialize error!" << endl;
    }
  }

  cout << "----------------" << endl;

  {
    if (a.Deserialize("data") == false)
    {
      cout << "Deserialize error!" << endl;
    }
  }

  return 0;
}