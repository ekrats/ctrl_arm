#pragma once
#include <stdio.h>

class OnOff
{
public:

    // 摘要: 
    //     初始化 OnOff 类的新实例。
    OnOff()
    {
        destination = NULL;
    }

    // 摘要: 
    //     析构 OnOff 类的实例。
    ~OnOff()
    {
    }

    // 摘要: 
    //     使用指定的目标地址初始化 OnOff 类。
    //
    // 参数:
    //   destination
    //     开关指令写入的目标地址。
    inline void Init(int * destination)
    {
        this->destination = destination;
    }

    // 摘要: 
    //     关闭。
    inline virtual void On()
    {
        if (NULL != destination)
        {
            *destination = 1;
        }
    }

    // 摘要: 
    //     开启。
    inline virtual void Off()
    {
        if (NULL != destination)
        {
            *destination = 0;
        }
    }

    // 摘要: 
    //     翻转状态。
    inline virtual void Toggle()
    {
        if (NULL != destination)
        {
            *destination ^= 1;
        }
    }

    // 摘要: 
    //     闭合。
    inline virtual void Close()
    {
        On();
    }

    // 摘要: 
    //     开启。
    inline virtual void Open()
    {
        Off();
    }

protected:
    // 摘要: 
    //     目标地址指针。开关指令写入的目的地。
    int * destination;
};

