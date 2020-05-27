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

  bool Serialize(const char *pFilePath) const;
  bool Serialize(int) const;

  bool Deserialize(const char *pFilePath);
  bool Deserialize(int);

private:
  int i;
};

bool Content::Serialize(const char *pFilePath) const
{
  // 打开文件，不存在就创建
  int fd = open(pFilePath, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if (fd == -1)
  {
    cout << "Open error!" << endl;
    return false;
  }

  // 写入
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

bool Content::Serialize(int fd) const
{
  if (fd == -1)
  {
    cout << "invlaid fd!" << endl;
    return false;
  }

  if (write(fd, &i, sizeof(int)) == -1)
  {
    cout << "write error!" << endl;
    return false;
  }

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

  if (Deserialize(fd) == false)
  {
    cout << "Read error!" << endl;
    close(fd);
    return false;
  }

  if (close(fd) == -1)
  {
    cout << "Close error!" << endl;
    return false;
  }

  // cout << "Deserialize success!" << endl;
  // cout << "a.i = " << i << endl;
  return true;
}

bool Content::Deserialize(int fd)
{
  if (fd == -1)
  {
    cout << "invlaid fd!" << endl;
    return false;
  }

  int r = read(fd, &i, sizeof(int));
  if ((r == 0) || (r == -1))
  {
    return false;
  }

  return true;
}

/*----------------------------------------------------------------*/

class SerializerForContent
{
public:
  SerializerForContent() {}
  virtual ~SerializerForContent() {}

  bool Serialize(const char *pFilePath, const std::vector<Content> &v);
  bool Deserialize(const char *pFilePath, std::vector<Content> &v);
};

bool SerializerForContent::Serialize(const char *pFilePath, const std::vector<Content> &v)
{
  int fd = open(pFilePath, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if (fd == -1)
  {
    cout << "Open Error!" << endl;
    return false;
  }

  for (int i = 0; i < v.size(); i++)
  {
    v[i].Serialize(fd);
  }

  if (close(fd) == -1)
  {
    cout << "Close Error!" << endl;
    return false;
  }

  return true;
}

bool SerializerForContent::Deserialize(const char *pFilePath, std::vector<Content> &v)
{
  int fd = open(pFilePath, O_RDONLY);

  if (fd == -1)
  {
    cout << "Open Error!" << endl;
    return false;
  }

  while (true)
  {
    Content sth;
    if (sth.Deserialize(fd) == true)
    {
      v.push_back(sth);
    }

    else
    {
      break;
    }
  }

  if (close(fd) == -1)
  {
    cout << "Close Error!" << endl;
    return false;
  }

  return true;
}

int main()
{
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