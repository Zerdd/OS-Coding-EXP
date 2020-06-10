#include <iostream>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

using namespace std;

// 各种类的ID
#define Class_A 0
#define Class_B 1
#define Class_C 2

// 序列化内容的基类，以纯虚函数形式实现
class Content_Base
{
public:
    Content_Base(void) {}                          // 构造函数
    virtual ~Content_Base() {}                     // 析构函数

    virtual bool Serialize(int fd) = 0;            // 序列化函数
    virtual Content_Base *Deserialize(int fd) = 0; // 反序列化函数，返回子类的类型
    virtual bool getType(int &nType) = 0;          // 返回是哪个类
};

// A类
class ContentA : public Content_Base
{
public:
    ContentA(void)                  // 构造函数
    {
        x = 0;
    }

    virtual ~ContentA() {}          // 析构函数

    explicit ContentA(int i)        // 赋值
    {
        x = i;
    }

    void ShowContentA()             // 输出类内内容
    {
        cout << "Show_Content_A: " << x << endl;
    }

    virtual bool Serialize(int);            // 序列化到指定文件
    virtual bool getType(int &nType);       // 返回类ID
    virtual Content_Base *Deserialize(int); // 反序列化，返回基类类型

private:
    int x;
};

bool ContentA::Serialize(int fd)
{
    // 判断文件描述符是否合法
    if (fd == -1)
    {
        cout << "invlaid fd!" << endl;
        return false;
    }

    // 写入
    if (write(fd, &x, sizeof(int)) == -1)
    {
        cout << "write error!" << endl;
        return false;
    }

    return true;
}

Content_Base *ContentA::Deserialize(int fd)
{
    // 判断文件描述符是否合法
    if (fd == -1)
    {
        cout << "invlaid fd!" << endl;
        return NULL;
    }

    // 定义一个指向A类的指针
    ContentA *p = new ContentA();

    // 读取内容
    int r = read(fd, &(p->x), sizeof(int));
    if ((r == 0) || (r == -1))
    {
        cout << "read Content_A.x error!" << endl;
        delete p;
        return NULL;
    }

    return p;
}

bool ContentA::getType(int &nType)
{
    nType = Class_A;
    return true;
}

// B类，构造形同A类
class ContentB : public Content_Base
{
public:
    ContentB(void)
    {
        x = 0;
        y = 0;
    }

    explicit ContentB(int i, int j)
    {
        x = i;
        y = j;
    }

    virtual ~ContentB() {}

    void ShowContentB()
    {
        cout << "Show_Content_B: " << x << ", " << y << endl;
    }

    virtual bool Serialize(int);
    virtual bool getType(int &);
    virtual Content_Base *Deserialize(int fd);

private:
    int x, y;
};

bool ContentB::Serialize(int fd)
{
    if (fd == -1)
    {
        cout << "invlaid fd!" << endl;
        return false;
    }

    if (write(fd, &x, sizeof(int)) == -1)
    {
        cout << "write error!" << endl;
        return false;
    }

    if (write(fd, &y, sizeof(int)) == -1)
    {
        cout << "write error!" << endl;
        return false;
    }

    return true;
}

Content_Base * ContentB::Deserialize(int fd)
{
        if (fd == -1)
        {
            cout << "invlaid fd!" << endl;
            return NULL;
        }

        ContentB *p = new ContentB();

        int r = read(fd, &(p->x), sizeof(int));
        if ((r == 0) || (r == -1))
        {
            cout << "read Content_B.x error!" << endl;
            delete p;
            return NULL;
        }

        r = read(fd, &(p->y), sizeof(int));
        if ((r == 0) || (r == -1))
        {
            cout << "read Content_B.y error!" << endl;
            delete p;
            return NULL;
        }

        return p;
    }

bool ContentB::getType(int &nType)
{
    nType = Class_B;
    return true;
}

// C类，构造形同C类
class ContentC : public Content_Base
{
private:
    int x, y, z;

public:
    ContentC(void)
    {
        x = y = z = 0;
    }

    explicit ContentC(int i, int j, int k)
    {
        x = i;
        y = j;
        z = k;
    }

    virtual ~ContentC() {}

    void ShowContentC(void)
    {
        cout << "Show_Content_C: " << x << ", " << y << ", " << z << endl;
    }

    virtual bool Serialize(int);
    virtual Content_Base *Deserialize(int);
    virtual bool getType(int &);
};

bool ContentC::Serialize(int fd)
{
    if (fd == -1)
    {
        cout << "invlaid fd!" << endl;
        return false;
    }

    if (write(fd, &x, sizeof(int)) == -1)
    {
        cout << "write error!" << endl;
        return false;
    }

    if (write(fd, &y, sizeof(int)) == -1)
    {
        cout << "write error!" << endl;
        return false;
    }

    if (write(fd, &z, sizeof(int)) == -1)
    {
        cout << "write error!" << endl;
        return false;
    }

    return true;
}

Content_Base *ContentC::Deserialize(int fd)
{
    if (fd == -1)
    {
        cout << "invlaid fd!" << endl;
        return NULL;
    }

    ContentC *p = new ContentC();

    int r = read(fd, &(p->x), sizeof(int));
    if ((r == 0) || (r == -1))
    {
        cout << "read Content_C.x error!" << endl;
        delete p;
        return NULL;
    }

    r = read(fd, &(p->y), sizeof(int));
    if ((r == 0) || (r == -1))
    {
        cout << "read Content_C.y error!" << endl;
        delete p;
        return NULL;
    }

    r = read(fd, &(p->z), sizeof(int));
    if ((r == 0) || (r == -1))
    {
        cout << "read Content_C.z error!" << endl;
        delete p;
        return NULL;
    }

    return p;
}

bool ContentC::getType(int &nType)
{
    nType = Class_C;
    return true;
}

/*----------------------------------------------------------------*/

// 序列化器
class Serizlizer
{
public:
    Serizlizer() {}              // 构造函数
    virtual ~Serizlizer() {}     // 析构函数

    // 将内容序列化至给定文件中
    bool Serialize(const char *pFilePath, const std::vector<Content_Base *> &v);
    // 将序列化内容读取至变量中
    bool Deserialize(const char *pFilePath, std::vector<Content_Base *> &v);
    // 注册各类的类型
    void CheckIN(Content_Base *pSerialized);

private:
    vector<Content_Base *> S_Serizlized;    // 注册类
};

bool Serizlizer::Serialize(const char *pFilePath, const std::vector<Content_Base *> &v)
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
        // 获取类ID
        int type;
        v[i]->getType(type);

        // 写入类ID
        if (write(fd, &type, sizeof(int)) == -1)
        {
            cout << "Write Type Error!" << endl;
            close(fd);
            return false;
        }

        v[i]->Serialize(fd); // 多态（对应各类的序列化函数）
    }

    // 关闭
    if (close(fd) == -1)
    {
        cout << "Close Error!" << endl;
        return false;
    }

    return true;
}

bool Serizlizer::Deserialize(const char *pFilePath, std::vector<Content_Base *> &v)
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
        if ((r == 0) || (r == -1))
        {
            // cout << "read type error!" << endl;s
            break;
        }

        int nType;  // 匹配是哪一个类

        // 循环所有类的类型
        for (int i = 0; i < S_Serizlized.size(); i++)
        {
            // 获取类ID
            S_Serizlized[i]->getType(nType);

            // matched
            if (nType == type)
            {
                // 调用对应的反序列化函数
                Content_Base *p = S_Serizlized[i]->Deserialize(fd);
                if (p != NULL)
                {
                    v.push_back(p);
                }
            }
        }
    }

    // 关闭
    if (close(fd) == -1)
    {
        cout << "Close Error!" << endl;
        return false;
    }

    return true;
}

void Serizlizer::CheckIN(Content_Base *pSerialized)
{
    S_Serizlized.push_back(pSerialized);
}

int main()
{
    // 序列化段
    {
        // 实例化A, B, C类并赋值
        ContentA a1(1),          a2(2);
        ContentB b1(21, 22),     b2(23, 24);
        ContentC c1(31, 32, 33), c2(34, 35, 36);

        // 压入容器
        vector<Content_Base *> v;
        v.push_back(&a1);
        v.push_back(&a2);
        v.push_back(&b1);
        v.push_back(&b2);
        v.push_back(&c1);
        v.push_back(&c2);

        // 定义序列化器并序列化
        Serizlizer s;
        s.Serialize("data4", v);
    }

    // 反序列化段
    {
        // 定义各类并注册进序列化器里
        ContentA a;
        ContentB b;
        ContentC c;
        Serizlizer s;
        s.CheckIN(&a);
        s.CheckIN(&b);
        s.CheckIN(&c);

        // 反序列化
        vector<Content_Base *> v;
        s.Deserialize("data4", v);

        // 输出对应类
        for (int i = 0; i <= v.size(); i++)
        {
            ContentA *pa = dynamic_cast<ContentA *>(v[i]);
            if (pa != NULL)
            {
                pa->ShowContentA();
            }

            ContentB *pb = dynamic_cast<ContentB *>(v[i]);
            if (pb != NULL)
            {
                pb->ShowContentB();
            }

            ContentC *pc = dynamic_cast<ContentC *>(v[i]);
            if (pc != NULL)
            {
                pc->ShowContentC();
            }
        }
    }

    return 0;
}