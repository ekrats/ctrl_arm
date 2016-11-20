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
    // ժҪ: 
    //     ��ʼ�� ITransducer �����ʵ����
    ITransducer()
    {
        index = 0;
    }

    // ժҪ: 
    //     ������Դָ������ݸ��Ƶ����ݻ������������»�����ָ�룬��������ִ�м򵥵�һ���˲�������
    virtual void Update()
    {
        instantaneousValue = *dataSource;
        buffer[index] = instantaneousValue;
        filterValue = Filter(filterValue, instantaneousValue, filterPara);
        UpdateIndex();
    }

    // ժҪ: 
    //     ��ȡ��������ǰʱ�̵�ԭʼAD������˲ʱֵ��
    //
    // ���ؽ��:
    //    ԭʼAD������˲ʱֵ
    inline int GetInstantaneousValue()
    {
        return instantaneousValue;
    }

    // ժҪ: 
    //     ��ȡ��������ǰʱ�̵���ֵ��
    // ���ؽ��:
    //    ��������ǰʱ�̵���ֵ
    virtual int GetRealValue() = 0;

    // ժҪ: 
    //     ���ݸ�����ֵ��ȡ��Ӧ��AD����ֵ��
    //
    // ����: 
    //   realValue:
    //     ������ֵ��
    //
    // ���ؽ��: 
    //     ת�����AD����ֵ����
    virtual int GetAcqValue(int realValue) = 0;


    // ժҪ: 
    //     ��ȡAD����ֵ���˲�ֵ��
    //
    // ���ؽ��: 
    //     AD����ֵ���˲�ֵ��
    virtual int GetFilterValue()
    {
        return filterValue;
    }

    // ժҪ: 
    //     ��ʼ�� ITransducer �ࡣ
    //
    // ����: 
    //   dataSource:
    //     ����Դָ�롣
    //
    //   range:
    //     ���������̣�������Ϊ�������̣�������Ҫ�����̣���ʵ������Ϊrange��һ��.
    //
    //   filterPara:
    //     һ���˲�����.��ΧΪ0-100
    void Init(int * dataSource, int range, int filterPara)
    {
        this->dataSource = dataSource;
        this->range = range;
        if ((filterPara >= 0) && (filterPara <= 100))
        {
            this->filterPara = filterPara;
        }
    }

    // ժҪ: 
    //     ��ʼ�� ITransducer �ࡣ
    //
    // ����: 
    //   dataSource:
    //     ����Դָ�롣
    void Init(int * dataSource)
    {
        this->dataSource = dataSource;
    }
    
    // ժҪ: 
    //     �������캯����ʹ��transducer��ʼ�� ITransducer �����ʵ����
    //
    // ����:
    //   transducer
    //     ����Դ������
    ITransducer(const ITransducer & transducer)
    {
        Init(&transducer);
    }

    // ժҪ: 
    //     �������캯����ʹ��transducer��ʼ�� ITransducer �����ʵ����
    //
    // ����:
    //   transducer
    //     ����Դָ��
    ITransducer(const ITransducer  * const transducer)
    {
        Init(transducer);
    }

    // ժҪ: 
    //     ���صȺŲ�������ʹ�����е�ITransducer���ʼ�� ITransducer �����ʵ����
    //
    // ����:
    //   transducer
    //     Դ����
    ITransducer &operator = (const ITransducer & transducer)
    {
        if (this != &transducer)
        {
            Init(&transducer);
        }
        return *this;
    }

    // ժҪ: 
    //     ���� ITransducer ���ʵ����
    ~ITransducer()
    {
        delete buffer;
    }
	
    // ժҪ: 
    //     ����һ���˲�������
    //
    // ����:
    //   value
    //     һ���˲���������ΧΪ0-100
    inline void SetParameter(int value)
	{
        if ((value >= 0) && (value <= 100))
        {
            this->filterPara = value;
        }
	}

protected:
    // ժҪ: 
    //     ʹ��transducer��ʼ�� ITransducer �����ʵ����
    //
    // ����:
    //   transducer
    //     ��ʼ��Դָ��
    void Init(const ITransducer * transducer)
    {
        this->length = transducer->length;
        this->buffer = new int[length];
        dataSource = NULL;
        this->filterPara = transducer->filterPara;
        this->range = transducer->range;
    }

    // ժҪ: 
    //     �����������ڻ������е��±ꡣ
    void UpdateIndex()
    {
        ++index;
        if (index >= length)
        {
            index = 0;
        }
    }

protected:
    // ժҪ: 
    //     ���ݻ����������һ��ʱ���AD����ֵ��
    int * buffer;

    // ժҪ: 
    //     ���ݻ��泤�ȡ�
    int length;

    // ժҪ: 
    //     ���������ڻ������е��±ꡣ
    int index;

    // ժҪ: 
    //     ����Դָ�롣
    int * dataSource;

    // ժҪ: 
    //     ���������̡�
    int range;

    // ժҪ: 
    //     һ���˲�������
    int filterPara;

    // ժҪ: 
    //     ��ǰʱ�̵�ԭʼAD������˲ʱֵ��
    int instantaneousValue;
    // ժҪ: 
    //     ��ǰʱ�̵�ԭʼAD�������˲�ֵ��
    int filterValue;
};


#endif

/******************* (C) COPYRIGHT 2014 TongYe **********END OF FILE****/


/*@}*/

