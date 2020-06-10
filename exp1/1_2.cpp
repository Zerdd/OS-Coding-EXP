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
  Content()                   // 构造函数
  {
    i = 0;
  }

  virtual ~Content() {}       // 析构函数

  void SetContent(int j)      // 赋值
  {
    i = j;
  }

  void ShowContent()          // 输出序列化内容
  {
    std::cout << "Show_Content: " << i << std::endl;
  }

  bool Serialize(const char *pFilePath) const;  // 序列化函数
  bool Serialize(int) const;                    // 重载，序列化类内对象至对应文件中

  bool Deserialize(const char *pFilePath);      // 反序列化函数
  bool Deserialize(int);                        // 重载，反序列化对应文件至类内对象中

private:
  int i;
};

// @brief 调用重载函数序列化至文件中
bool Content::Serialize(const char *pFilePath) const
{
  // 打开文件，不存在就创建
  int fd = open(pFilePath, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if (fd == -1)
  {
    cout << "Open error!" << endl;
    return false;
  }

  // 调用重载函数写入
  if (Serialize(fd) == false)
  {
    cout << "Write error!" << endl;
    close(fd);
    return false;
  }

  // 关闭
  if (close(fd) == -1)
  {
    cout << "Close error!" << endl;
    return false;
  }

  cout << "Serialize success!" << endl;
  return true;
}

// @brief 将变量写入对应文件中
bool Content::Serialize(int fd) const
{
  // 文件描述符是否合法
  if (fd == -1)
  {
    cout << "invlaid fd!" << endl;
    return false;
  }

  // 写入
  if (write(fd, &i, sizeof(int)) == -1)
  {
    cout << "write error!" << endl;
    return false;
  }

  return true;
}

// @brief 反序列化函数
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
  
  // 调用重载函数反序列化
  if (Deserialize(fd) == false)
  {
    cout << "Read error!" << endl;
    close(fd);
    return false;
  }

  // 关闭
  if (close(fd) == -1)
  {
    cout << "Close error!" << endl;
    return false;
  }

  // cout << "Deserialize success!" << endl;
  // cout << "a.i = " << i << endl;
  return true;
}

// @brief 将对应文件中内容反序列化到变量中
bool Content::Deserialize(int fd)
{
  // 判断文件描述符是否合法
  if (fd == -1)
  {
    cout << "invlaid fd!" << endl;
    return false;
  }

  // 读取
  int r = read(fd, &i, sizeof(int));
  if ((r == 0) || (r == -1))
  {
    return false;
  }

  return true;
}

/*----------------------------------------------------------------*/

// 序列化器
class SerializerForContent
{
public:
  SerializerForContent() {}             // 构造函数
  virtual ~SerializerForContent() {}    // 析构函数

  // 序列化
  bool Serialize(const char *pFilePath, const std::vector<Content> &v);
  // 反序列化
  bool Deserialize(const char *pFilePath, std::vector<Content> &v);
};


// @brief 序列化同一个类的多个对象至指定文件中
bool SerializerForContent::Serialize(const char *pFilePath, const std::vector<Content> &v)
{
  // 打开/创建文件
  int fd = open(pFilePath, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if (fd == -1)
  {
    cout << "Open Error!" << endl;
    return false;
  }

  // 遍历需要序列化的容器
  for (int i = 0; i < v.size(); i++)
  {
    v[i].Serialize(fd);   // 序列化
  }

// 关闭
  if (close(fd) == -1)
  {
    cout << "Close Error!" << endl;
    return false;
  }

  return true;
}

// @brief 反序列化至指定文件中的多个对象至容器里
bool SerializerForContent::Deserialize(const char *pFilePath, std::vector<Content> &v)
{
  // 打开文件
  int fd = open(pFilePath, O_RDONLY);
  if (fd == -1)
  {
    cout << "Open Error!" << endl;
    return false;
  }

  // 读取至文件结束
  while (true)
  {
    Content sth;                         // 临时存放反序列化出来的内容
    if (sth.Deserialize(fd) == true)     // 反序列化至sth
    {
      v.push_back(sth);                  // 保存内容
    }

    else
    {
      break;
    }
  }

  // 关闭文件
  if (close(fd) == -1)
  {
    cout << "Close Error!" << endl;
    return false;
  }

  return true;
}

int main()
{
  // 序列化段
  {
    vector<Content>v;
    Content a, b, c;
    a.SetContent(10);
    b.SetContent(20);
    c.SetContent(30);

    v.push_back(a);
    v.push_back(b);
    v.push_back(c);

    SerializerForContent SC;
    SC.Serialize("data2", v);
  }

  // 反序列化段
  {
    vector<Content> v;
    SerializerForContent SC;
    SC.Deserialize("data2", v);
    for (int i = 0; i <= 2; i++)
    {
      cout << i << ": " ; v[i].ShowContent();
    }
  }

  return 0;
}