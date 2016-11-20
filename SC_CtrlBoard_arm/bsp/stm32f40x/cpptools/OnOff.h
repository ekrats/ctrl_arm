#pragma once
#include <stdio.h>

class OnOff
{
public:

    // ժҪ: 
    //     ��ʼ�� OnOff �����ʵ����
    OnOff()
    {
        destination = NULL;
    }

    // ժҪ: 
    //     ���� OnOff ���ʵ����
    ~OnOff()
    {
    }

    // ժҪ: 
    //     ʹ��ָ����Ŀ���ַ��ʼ�� OnOff �ࡣ
    //
    // ����:
    //   destination
    //     ����ָ��д���Ŀ���ַ��
    inline void Init(int * destination)
    {
        this->destination = destination;
    }

    // ժҪ: 
    //     �رա�
    inline virtual void On()
    {
        if (NULL != destination)
        {
            *destination = 1;
        }
    }

    // ժҪ: 
    //     ������
    inline virtual void Off()
    {
        if (NULL != destination)
        {
            *destination = 0;
        }
    }

    // ժҪ: 
    //     ��ת״̬��
    inline virtual void Toggle()
    {
        if (NULL != destination)
        {
            *destination ^= 1;
        }
    }

    // ժҪ: 
    //     �պϡ�
    inline virtual void Close()
    {
        On();
    }

    // ժҪ: 
    //     ������
    inline virtual void Open()
    {
        Off();
    }

protected:
    // ժҪ: 
    //     Ŀ���ַָ�롣����ָ��д���Ŀ�ĵء�
    int * destination;
};

