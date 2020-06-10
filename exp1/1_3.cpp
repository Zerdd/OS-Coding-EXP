#include <iostream>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

using namespace std;

// A类
class ContentA
{
public:
    ContentA()                  // 构造函数
    {
        i = 0;
    }

    virtual ~ContentA() {}      // 析构函数

    void SetContentA(int j)     // 赋值
    {
        i = j;
    }

    void ShowContentA()         // 输出类内内容
    {
        cout << "Show_Content_A: " << i << endl;
    }

    bool Serialize(int) const;  // 序列化类内变量至给定文件中
    bool Deserialize(int);      // 从给定文件中读取序列化内容

private:
    int i;
};

bool ContentA::Serialize(int fd) const
{
    // 判断文件描述符是否合法
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

bool ContentA::Deserialize(int fd)
{
    // 判断文件描述符是否合法
    if (fd == -1)
    {
        cout << "invlaid fd!" << endl;
        return false;
    }

    // 读取内容
    int r = read(fd, &i, sizeof(int));
    if ((r == 0) || (r == -1))
    {
        return false;
    }

    return true;
}

// B类，构造形同A类
class ContentB
{
public:
    ContentB()
    {
        i = 0;
    }

    void SetContentB(int m, int n)
    {
        i = m;
        j = n;
    }

    virtual ~ContentB() {}

    void ShowContentB()
    {
        cout << "Show_Content_B: " << i << ", " << j << endl;
    }

    bool Serialize(int) const;
    bool Deserialize(int);

private:
    int i, j;
};

bool ContentB::Serialize(int fd) const
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

    if (write(fd, &j, sizeof(int)) == -1)
    {
        cout << "write error!" << endl;
        return false;
    }

    return true;
}

bool ContentB::Deserialize(int fd)
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

    r = read(fd, &j, sizeof(int));
    if ((r == 0) || (r == -1))
    {
        return false;
    }

    return true;
}

/*----------------------------------------------------------------*/

// 聚集所有类
struct Serialized
{
    int nType;  // 类标识符，0 for A; 1 for B
    void *pObj; // 指向序列化的类
};

// 序列化器
class Serizlizer
{
public:
    Serizlizer() {}              // 构造函数
    virtual ~Serizlizer() {}     // 析构函数

    // 将内容序列化至给定文件中
    bool Serialize(const char *pFilePath, const std::vector<Serialized> &v);
    // 将序列化内容读取至变量中
    bool Deserialize(const char *pFilePath, std::vector<Serialized> &v);
};

bool Serizlizer::Serialize(const char *pFilePath, const std::vector<Serialized> &v)
{
    // 打开文件
    int fd = open(pFilePath, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        cout << "Open Error!" << endl;
        return false;
    }

    // 遍历所有需要序列化的内容
    for (int i = 0; i < v.size(); i++)
    {
        // 先写入类标识符
        if (write(fd, &(v[i].nType), sizeof(int)) == -1)
        {
            cout << "Write Error!" << endl;
            close(fd);
            return false;
        }

        // A类
        if (v[i].nType == 0)
        {
            ContentA *p = (ContentA *)(v[i].pObj);  // 强转为A类

            if (p->Serialize(fd) == false)          // 序列化
            {
                return false;
            }
        }
        // B类
        else if (v[i].nType == 1)
        {
            ContentB *p = (ContentB *)(v[i].pObj);  // 强转为B类

            if (p->Serialize(fd) == false)          // 序列化
            {
                return false;
            }
        }
        
    }

    if (close(fd) == -1)
    {
        cout << "Close Error!" << endl;
        return false;
    }

    return true;
}

bool Serizlizer::Deserialize(const char *pFilePath, std::vector<Serialized> &v)
{
    // 打开文件
    int fd = open(pFilePath, O_RDONLY);
    if (fd == -1)
    {
        cout << "Open Error!" << endl;
        return false;
    }

    // 循环读取直至文件结束
    while (true)
    {
        // 读取类的类型
        int type;
        int r = read(fd, &type, sizeof(int));

        // 文件结束
        if ((r == 0) || (r == -1))
        {
            break;
        }

        // A类
        if (type == 0)
        {
            // 定义一个A类的临时变量
            ContentA *p;
            p = new ContentA();

            // 反序列化至临时变量中
            p->Deserialize(fd);

            // 定义一个临时聚集类并初始化
            Serialized s;
            s.nType = type;
            s.pObj = p;

            // 保存
            v.push_back(s);
        }
        // B类
        else if (type == 1)
        {
            // 处理方式形同A类
            ContentB *p;
            p = new ContentB();
            p->Deserialize(fd);

            Serialized s;
            s.nType = type;
            s.pObj = p;

            v.push_back(s);
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
        // 实例化A，B类并赋值
        ContentA a1, a2;
        ContentB b1, b2;
        a1.SetContentA(11);
        a2.SetContentA(12);
        b1.SetContentB(21, 22);
        b2.SetContentB(23, 24);

        // 定义聚集类并分别将A, B两个类写入其中
        Serialized sa1, sa2, sb1, sb2;

        sa1.nType = sa2.nType = 0;
        sb1.nType = sb2.nType = 1;

        sa1.pObj = &a1;
        sa2.pObj = &a2;
        sb1.pObj = &b1;
        sb2.pObj = &b2;

        // 写入容器方便序列化
        vector<Serialized> v;
        v.push_back(sa1);
        v.push_back(sa2);
        v.push_back(sb1);
        v.push_back(sb2);

        // 定义序列化器并序列化
        Serizlizer s;
        s.Serialize("data3", v);
    }

    // 反序列化段
    {
        // 反序列化至容器v里
        vector<Serialized> v;
        Serizlizer s;
        s.Deserialize("data3", v);

        // 输出对应类
        for (int i = 0; i < v.size(); i++)
        {
            if (v[i].nType == 0)
            {
                ContentA *p = (ContentA *)(v[i].pObj);
                p->ShowContentA();
            }
            else if (v[i].nType == 1)
            {
                ContentB *p = (ContentB *)(v[i].pObj);
                p->ShowContentB();
            }
            
        }
    }

    return 0;
}