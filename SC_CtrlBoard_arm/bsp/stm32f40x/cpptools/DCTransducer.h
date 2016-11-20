
/*!
 * This is a part of the TongYe Foundation Classes C++ library.
 * COPYRIGHT (C) 1999-2014, TongYe Corporation All rights reserved.
 * @file    DCTransducer.h
 * @brief   This file contains definition of class DCTransducer
 * Change Logs:
 * Date           Author       Notes
 * 2014/04/12     Coase        the first version
 */

#ifndef __DCTRANSDUCER__H__
#define __DCTRANSDUCER__H__

#pragma once
#include "itransducer.h"
class DCTransducer :
    public ITransducer
{
private:
    int sum;
    int validValue;
public:
    DCTransducer(int length = 60) : ITransducer()
    {
        this->length = length;
        buffer = new int[length];
        sum = 0;
    }

    ~DCTransducer(void)
    {
    }
    DCTransducer(const ITransducer & transducer) : ITransducer(transducer)
    {
    }

    DCTransducer(const ITransducer * const transducer) : ITransducer(transducer)
    {
    }

    DCTransducer &operator=(const ITransducer & transducer)
    {
        if (this != &transducer)
        {
            Init(&transducer);
        }
        return *this;
    }
    virtual void Update();
    virtual int GetRealValue() ;
    int GetIRealValue() ;
    int GetAverageRealValue();
    virtual int GetAcqValue(int realValue);
};



#endif

/******************* (C) COPYRIGHT 2014 TongYe **********END OF FILE****/


/*@}*/


