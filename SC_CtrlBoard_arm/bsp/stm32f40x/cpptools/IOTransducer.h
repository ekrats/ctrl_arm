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
    // 摘要:
    //    缓存区数据值的总和.
    int sum;

public:
    // 摘要: 
    //     使用指定缓存区长度初始化 IOTransducer 类的新实例。
    //
    // 参数:
    //   length
    //     缓存区长度，默认为60
    IOTransducer(int length = 60) : ITransducer()
    {
        this->length = length;
        buffer = new int[length];
    }

    ~IOTransducer(void)
    {
        delete buffer;
    }

    // 摘要: 
    //     将数据源指针的数据复制到数据缓存区，并更新缓存区指针，计算缓存区数据累加和。
    virtual void Update();

    // 摘要: 
    //     获取传感器当前时刻的真值。
    //
    // 返回结果:
    //    传感器当前时刻的真值，若缓存区大部分数据为1，则返回1，否则返回0。
    virtual int GetRealValue();

    // 摘要: 
    //     根据给定真值获取对应的IO值。
    //
    // 参数: 
    //   realValue:
    //     给定真值。
    //
    // 返回结果: 
    //     转换后的IO值。
    virtual int GetAcqValue(int realValue);
};

/******************* (C) COPYRIGHT 2014 TongYe **********END OF FILE****/
