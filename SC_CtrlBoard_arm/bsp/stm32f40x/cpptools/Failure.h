/*!
 * This is a part of the TongYe Foundation Classes C++ library.
 * COPYRIGHT (C) 1999-2014, TongYe Corporation All rights reserved.
 * @file    Failure.h
 * @brief   This file contains definition of class Failure
 * Change Logs:
 * Date           Author       Notes
 * 2014/04/09     Coase        the first version
 */

#pragma once
#include "List.h"

class Failure
{
public:
    // ժҪ: 
    //     �������ϡ�
    inline void Lock()
    {
        isLocked = true;
    }

    // ժҪ: 
    //     �����Ƿ�������
    //
    // ���ؽ��:
    //     ���������������򷵻�TRUE�����򷵻�FALSE��
    inline bool IsLocked()
    {
        return isLocked;
    }

    // ժҪ: 
    //     �Ƿ������ϡ�
    //
    // ���ؽ��:
    //     ���ѷ������ϣ��򷵻�TRUE�����򷵻�FALSE��
    inline bool IsOccurred()
    {
        return isOccurred;
    }

    // ժҪ: 
    //     ���ù�������������Ĵ�����
    //
    // ����:
    //   times
    //     ����������Ĵ����������ϲ������ظ���������ֵΪ0��
    inline void SetLimitedTimes(int times)
    {
        maxTimes = times;
    }

    ~Failure(void)
    {
    }


    // ժҪ: 
    //     �������״̬��
    inline void Reset()
    {
        isOccurred = false;
    }

public:
    // ժҪ: 
    //     ʹ��ָ��������ʼ�� Failure �����ʵ����
    //
    // ����:
    //   times
    //     ����������Ĵ�����Ĭ��Ϊ3�Ρ�
    Failure(int times = 3);

    // ժҪ: 
    //     ����һ�ι��ϡ����ӹ��ϴ����������ݵ�ǰ�ѷ����Ĵ����ж��Ƿ��������ϡ�
    virtual void Encounter();

    // ժҪ:
    //     ���ٹ��Ϸ����Ĵ�����
    void Degrade();

    // ժҪ:
    //     ���������ü�����
    void Unlock();
protected:
    // ժҪ:
    //     �Ƿ������ϡ�
    bool isOccurred;

    // ժҪ:
    //     �Ƿ�������
    bool isLocked;

    // ժҪ:
    //     ����������������
    unsigned int maxTimes;

    // ժҪ:
    //     ��ǰʱ���ѷ����Ĵ�����
    unsigned int presentTimes;
};

/******************* (C) COPYRIGHT 2014 TongYe **********END OF FILE****/
