/*!
 * This is a part of the TongYe Foundation Classes C++ library.
 * COPYRIGHT (C) 1999-2014, TongYe Corporation All rights reserved.
 * @file    ITransducer.h
 * @brief   This file contains definition of class ITransducer
 * Change Logs:
 * Date           Author       Notes
 * 2014/04/12     Coase        the first version
 */

#ifndef __ITRANSDUCER__H__
#define __ITRANSDUCER__H__
#include <stdio.h>
#include "support.h"

class ITransducer
{
public:
    // 摘要: 
    //     初始化 ITransducer 类的新实例。
    ITransducer()
    {
        index = 0;
    }

    // 摘要: 
    //     将数据源指针的数据复制到数据缓存区，并更新缓存区指针，对新数据执行简单的一阶滤波操作。
    virtual void Update()
    {
        instantaneousValue = *dataSource;
        buffer[index] = instantaneousValue;
        filterValue = Filter(filterValue, instantaneousValue, filterPara);
        UpdateIndex();
    }

    // 摘要: 
    //     获取传感器当前时刻的原始AD采样的瞬时值。
    //
    // 返回结果:
    //    原始AD采样的瞬时值
    inline int GetInstantaneousValue()
    {
        return instantaneousValue;
    }

    // 摘要: 
    //     获取传感器当前时刻的真值。
    // 返回结果:
    //    传感器当前时刻的真值
    virtual int GetRealValue() = 0;

    // 摘要: 
    //     根据给定真值获取对应的AD采样值。
    //
    // 参数: 
    //   realValue:
    //     给定真值。
    //
    // 返回结果: 
    //     转换后的AD采样值。。
    virtual int GetAcqValue(int realValue) = 0;


    // 摘要: 
    //     获取AD采样值的滤波值。
    //
    // 返回结果: 
    //     AD采样值的滤波值。
    virtual int GetFilterValue()
    {
        return filterValue;
    }

    // 摘要: 
    //     初始化 ITransducer 类。
    //
    // 参数: 
    //   dataSource:
    //     数据源指针。
    //
    //   range:
    //     传感器量程，此量程为正负量程，若不需要负量程，则实际量程为range的一倍.
    //
    //   filterPara:
    //     一阶滤波参数.范围为0-100
    void Init(int * dataSource, int range, int filterPara)
    {
        this->dataSource = dataSource;
        this->range = range;
        if ((filterPara >= 0) && (filterPara <= 100))
        {
            this->filterPara = filterPara;
        }
    }

    // 摘要: 
    //     初始化 ITransducer 类。
    //
    // 参数: 
    //   dataSource:
    //     数据源指针。
    void Init(int * dataSource)
    {
        this->dataSource = dataSource;
    }
    
    // 摘要: 
    //     拷贝构造函数，使用transducer初始化 ITransducer 类的新实例。
    //
    // 参数:
    //   transducer
    //     拷贝源的引用
    ITransducer(const ITransducer & transducer)
    {
        Init(&transducer);
    }

    // 摘要: 
    //     拷贝构造函数，使用transducer初始化 ITransducer 类的新实例。
    //
    // 参数:
    //   transducer
    //     拷贝源指针
    ITransducer(const ITransducer  * const transducer)
    {
        Init(transducer);
    }

    // 摘要: 
    //     重载等号操作符。使用已有的ITransducer类初始化 ITransducer 类的新实例。
    //
    // 参数:
    //   transducer
    //     源引用
    ITransducer &operator = (const ITransducer & transducer)
    {
        if (this != &transducer)
        {
            Init(&transducer);
        }
        return *this;
    }

    // 摘要: 
    //     析构 ITransducer 类的实例。
    ~ITransducer()
    {
        delete buffer;
    }
	
    // 摘要: 
    //     设置一阶滤波参数。
    //
    // 参数:
    //   value
    //     一阶滤波参数，范围为0-100
    inline void SetParameter(int value)
	{
        if ((value >= 0) && (value <= 100))
        {
            this->filterPara = value;
        }
	}

protected:
    // 摘要: 
    //     使用transducer初始化 ITransducer 类的新实例。
    //
    // 参数:
    //   transducer
    //     初始化源指针
    void Init(const ITransducer * transducer)
    {
        this->length = transducer->length;
        this->buffer = new int[length];
        dataSource = NULL;
        this->filterPara = transducer->filterPara;
        this->range = transducer->range;
    }

    // 摘要: 
    //     更新新数据在缓冲区中的下标。
    void UpdateIndex()
    {
        ++index;
        if (index >= length)
        {
            index = 0;
        }
    }

protected:
    // 摘要: 
    //     数据缓存区，存放一段时间的AD采样值。
    int * buffer;

    // 摘要: 
    //     数据缓存长度。
    int length;

    // 摘要: 
    //     最新数据在缓冲区中的下标。
    int index;

    // 摘要: 
    //     数据源指针。
    int * dataSource;

    // 摘要: 
    //     传感器量程。
    int range;

    // 摘要: 
    //     一阶滤波参数。
    int filterPara;

    // 摘要: 
    //     当前时刻的原始AD采样的瞬时值。
    int instantaneousValue;
    // 摘要: 
    //     当前时刻的原始AD采样的滤波值。
    int filterValue;
};


#endif

/******************* (C) COPYRIGHT 2014 TongYe **********END OF FILE****/


/*@}*/

