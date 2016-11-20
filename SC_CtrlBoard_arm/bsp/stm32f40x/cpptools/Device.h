/*!
 * This is a part of the TongYe Foundation Classes C++ library.
 * COPYRIGHT (C) 1999-2014, TongYe Corporation All rights reserved.
 * @file    Device.h
 * @brief   This file contains definition of class Device
 * Change Logs:
 * Date           Author       Notes
 * 2014/07/19     Coase        the first version
 */


#pragma once

class Device
{
public:
    // ժҪ: 
    //     ��ʼ�� Device �����ʵ����
    Device()
    {
        isStartEnable = false;
    }

    // ժҪ: 
    //     ɾ�� Device ���ʵ����
    ~Device()
    {

    }

    // ժҪ: 
    //     �����豸�����豸����Ϊ����״̬��
    virtual inline void On()
    {
        isStartEnable = true;
    }

    // ժҪ: 
    //     ֹͣ���С����豸����Ϊֹͣ����״̬��
    virtual inline void Off()
    {
        isStartEnable = false;
    }

    // ժҪ: 
    //     �豸���С������豸������״ִ̬�����л�ֹͣ�߼���
    virtual void Run() = 0;
protected:
    // ժҪ: 
    //     �豸������״̬��
    bool isStartEnable;
    bool res;
};

/******************* (C) COPYRIGHT 2014 TongYe **********END OF FILE****/
