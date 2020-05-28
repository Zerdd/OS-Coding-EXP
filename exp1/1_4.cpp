#include <iostream>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

using namespace std;

#define Class_A 0
#define Class_B 1
#define Class_C 2

class Content_Base
{
public:
    Content_Base(void) {}
    virtual ~Content_Base() {}

    virtual bool Serialize(int fd) = 0;            // 之类的序列化函数
    virtual Content_Base *Deserialize(int fd) = 0; // 返回子类的类型
    virtual bool getType(int &nType) = 0;          // 返回是哪个类
};

class ContentA : public Content_Base
{
public:
    ContentA(void)
    {
        x = 0;
    }

    explicit ContentA(int i)
    {
        x = i;
    }

    virtual ~ContentA() {}

    void ShowContentA()
    {
        cout << "Show_Content_A: " << x << endl;
    }

    virtual bool Serialize(int);
    virtual bool getType(int &nType);
    virtual Content_Base *Deserialize(int);

private:
    int x;
};

bool ContentA::Serialize(int fd)
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

    return true;
}

Content_Base *ContentA::Deserialize(int fd)
{
    if (fd == -1)
    {
        cout << "invlaid fd!" << endl;
        return NULL;
    }

    ContentA *p = new ContentA();

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

class Serizlizer
{
public:
    Serizlizer() {}
    virtual ~Serizlizer() {}

    bool Serialize(const char *pFilePath, const std::vector<Content_Base *> &v);
    bool Deserialize(const char *pFilePath, std::vector<Content_Base *> &v);
    void CheckIN(Content_Base *pSerialized);

private:
    vector<Content_Base *> S_Serizlized;
};

bool Serizlizer::Serialize(const char *pFilePath, const std::vector<Content_Base *> &v)
{
    int fd = open(pFilePath, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        cout << "Open Error!" << endl;
        return false;
    }

    for (int i = 0; i < v.size(); i++)
    {
        int type;
        v[i]->getType(type);

        if (write(fd, &type, sizeof(int)) == -1)
        {
            cout << "Write Type Error!" << endl;
            close(fd);
            return false;
        }

        v[i]->Serialize(fd); // 多态
    }

    if (close(fd) == -1)
    {
        cout << "Close Error!" << endl;
        return false;
    }

    return true;
}

bool Serizlizer::Deserialize(const char *pFilePath, std::vector<Content_Base *> &v)
{
    int fd = open(pFilePath, O_RDONLY);

    if (fd == -1)
    {
        cout << "Open Error!" << endl;
        return false;
    }

    while (true)
    {
        int type;
        int r = read(fd, &type, sizeof(int));
        if ((r == 0) || (r == -1))
        {
            // cout << "read type error!" << endl;s
            break;
        }

        int nType;
        for (int i = 0; i < S_Serizlized.size(); i++)
        {
            S_Serizlized[i]->getType(nType);
            if (nType == type)
            {
                Content_Base *p = S_Serizlized[i]->Deserialize(fd);
                if (p != NULL)
                {
                    v.push_back(p);
                }
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

void Serizlizer::CheckIN(Content_Base *pSerialized)
{
    S_Serizlized.push_back(pSerialized);
}

int main()
{
    {
        ContentA a1(1),          a2(2);
        ContentB b1(21, 22),     b2(23, 24);
        ContentC c1(31, 32, 33), c2(34, 35, 36);

        vector<Content_Base *> v;
        v.push_back(&a1);
        v.push_back(&a2);
        v.push_back(&b1);
        v.push_back(&b2);
        v.push_back(&c1);
        v.push_back(&c2);

        Serizlizer s;
        s.Serialize("data4", v);
    }

    {
        ContentA a;
        ContentB b;
        ContentC c;

        Serizlizer s;
        s.CheckIN(&a);
        s.CheckIN(&b);
        s.CheckIN(&c);

        vector<Content_Base *> v;
        s.Deserialize("data4", v);

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