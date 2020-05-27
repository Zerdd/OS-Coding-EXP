#include <iostream>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

using namespace std;

class ContentA
{
public:
    ContentA()
    {
        i = 0;
    }

    void SetContentA(int j)
    {
        i = j;
    }

    virtual ~ContentA() {}

    void ShowContentA()
    {
        cout << "Show_Content_A: " << i << endl;
    }

    bool Serialize(int) const;
    bool Deserialize(int);

private:
    int i;
};

bool ContentA::Serialize(int fd) const
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

bool ContentA::Deserialize(int fd)
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

struct Serialized
{
    int nType; // 0 for A; 1 for B
    void *pObj;
};

class Serizlizer
{
public:
    Serizlizer() {}
    virtual ~Serizlizer() {}

    bool Serialize(const char *pFilePath, const std::vector<Serialized> &v);
    bool Deserialize(const char *pFilePath, std::vector<Serialized> &v);
};

bool Serizlizer::Serialize(const char *pFilePath, const std::vector<Serialized> &v)
{
    int fd = open(pFilePath, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        cout << "Open Error!" << endl;
        return false;
    }

    for (int i = 0; i < v.size(); i++)
    {
        if (write(fd, &(v[i].nType), sizeof(int)) == -1)
        {
            cout << "Write Error!" << endl;
            close(fd);
            return false;
        }

        // For Content_A
        if (v[i].nType == 0)
        {
            ContentA *p = (ContentA *)(v[i].pObj);

            if (p->Serialize(fd) == false)
            {
                return false;
            }
        }
        // For Content_B
        else if (v[i].nType == 1)
        {
            ContentB *p = (ContentB *)(v[i].pObj);

            if (p->Serialize(fd) == false)
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
            break;
        }

        if (type == 0)
        {
            ContentA *p;
            p = new ContentA();
            p->Deserialize(fd);

            Serialized s;
            s.nType = type;
            s.pObj = p;

            v.push_back(s);
        }
        else if (type == 1)
        {
            ContentB *p;
            p = new ContentB();
            p->Deserialize(fd);

            Serialized s;
            s.nType = type;
            s.pObj = p;

            v.push_back(s);
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
        ContentA a1, a2;
        ContentB b1, b2;
        a1.SetContentA(11);
        a2.SetContentA(12);
        b1.SetContentB(21, 22);
        b2.SetContentB(23, 24);

        Serialized sa1, sa2, sb1, sb2;

        sa1.nType = sa2.nType = 0;
        sb1.nType = sb2.nType = 1;

        sa1.pObj = &a1;
        sa2.pObj = &a2;
        sb1.pObj = &b1;
        sb2.pObj = &b2;

        vector<Serialized> v;
        v.push_back(sa1);
        v.push_back(sa2);
        v.push_back(sb1);
        v.push_back(sb2);

        Serizlizer s;
        s.Serialize("data3", v);
    }

    {
        vector<Serialized> v;
        Serizlizer s;
        s.Deserialize("data3", v);

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