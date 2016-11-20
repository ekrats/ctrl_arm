/*!
 * This is a part of the TongYe Foundation Classes C++ library.
 * COPYRIGHT (C) 1999-2014, TongYe Corporation All rights reserved.
 * @file    IOTransducer.h
 * @brief   This file contains definition of class IOTransducer
 * Change Logs:
 * Date           Author       Notes
 * 2014/10/18     Coase        the first version
 */



#pragma once
#include "ITransducer.h"

class IOTransducer : public ITransducer
{
private:
    // ժҪ:
    //    ����������ֵ���ܺ�.
    int sum;

public:
    // ժҪ: 
    //     ʹ��ָ�����������ȳ�ʼ�� IOTransducer �����ʵ����
    //
    // ����:
    //   length
    //     ���������ȣ�Ĭ��Ϊ60
    IOTransducer(int length = 60) : ITransducer()
    {
        this->length = length;
        buffer = new int[length];
    }

    ~IOTransducer(void)
    {
        delete buffer;
    }

    // ժҪ: 
    //     ������Դָ������ݸ��Ƶ����ݻ������������»�����ָ�룬���㻺���������ۼӺ͡�
    virtual void Update();

    // ժҪ: 
    //     ��ȡ��������ǰʱ�̵���ֵ��
    //
    // ���ؽ��:
    //    ��������ǰʱ�̵���ֵ�����������󲿷�����Ϊ1���򷵻�1�����򷵻�0��
    virtual int GetRealValue();

    // ժҪ: 
    //     ���ݸ�����ֵ��ȡ��Ӧ��IOֵ��
    //
    // ����: 
    //   realValue:
    //     ������ֵ��
    //
    // ���ؽ��: 
    //     ת�����IOֵ��
    virtual int GetAcqValue(int realValue);
};

/******************* (C) COPYRIGHT 2014 TongYe **********END OF FILE****/
