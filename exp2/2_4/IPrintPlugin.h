// @biref 供插件实现功能的虚基类

#pragma once

class IPrintPlugin
{
private:
    /* data */
public:
    IPrintPlugin(/* args */);       // 构筑函数
    virtual ~IPrintPlugin();        // 虚构函数

    virtual void help() = 0;        // 打印帮助信息
    virtual void printSC() = 0;     // 打印信息
    virtual int  getID() = 0;       // 返回插件ID
};

typedef IPrintPlugin ipp_t;