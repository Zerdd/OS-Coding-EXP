#include <iostream>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

using namespace std;

// 序列化类
class Content
{
public:
  Content()                     // 构造函数
  {
    i = 0;
  }

  virtual ~Content() {}         // 析构函数

  void SetContent(int j)        // 赋值
  {
    i = j;
  }

  void ShowContent()            // 输出类内变量
  {
    std::cout << "Show_Content: " << i << std::endl;
  }

  bool Serialize(const char *pFilePath);   // 序列化函数
  bool Deserialize(const char *pFilePath); // 反序列化函数

private:
  int i;
};

// @brief 序列化内容到给定路径中
bool Content::Serialize(const char *pFilePath)
{
  // 打开文件，不存在就创建
  int fd = open(pFilePath, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if (fd == -1)
  {
    cout << "Open error!" << endl;
    return false;
  }

  // 写入
  if (write(fd, &i, sizeof(int)) == -1)
  {
    close(fd);
    cout << "Write error!" << endl;
    return false;
  }

  // 关闭文件
  if (close(fd) == -1)
  {
    cout << "Close error!" << endl;
    return false;
  }

  cout << "Serialize success!" << endl;
  return true;
}

// @brief 从给定的路径中反序列化
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

  // 读取文件至i
  if (read(fd, &i, sizeof(int)) == -1)
  {
    close(fd);
    cout << "Read error!" << endl;
    return false;
  }

  // 关闭文件
  if (close(fd) == -1)
  {
    cout << "Close error!" << endl;
    return false;
  }

  cout << "Deserialize success!" << endl;
  cout << "i = " << i << endl;
  return true;
}

int main()
{
  // 序列化代码段
  {
    Content se;             // 实例化一个序列化对象
    se.SetContent(10);      // 给对象赋值

    // 序列化到data文件里
    if (se.Serialize("data") == false)
    {
      cout << "Serialize error!" << endl;
    }
  }

  cout << "----------------" << endl;

  // 反序列化代码段
  {
    Content de;             // 实例化一个反序列化的存储对象

    // 从data文件里反序列化
    if (de.Deserialize("data") == false)
    {
      cout << "Deserialize error!" << endl;
    }
  }

  return 0;
}