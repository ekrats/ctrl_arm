/*!
 * This is a part of the TongYe Foundation Classes C++ library.
 * COPYRIGHT (C) 1999-2014, TongYe Corporation All rights reserved.
 * @file    PID.h
 * @brief   This file contains definition of class PID
 * Change Logs:
 * Date           Author       Notes
 * 2014/07/15     Coase        the first version
 */



#pragma once
template<typename T>
class PIDData 
{
public:
    T reference;
    T feedback;
    T proportionalGain;
    T integralGain;
    T integralCorrection;
    T derivativeGain;
    T outMax;
    T outMin;
    T preOut;
    T out;

protected:
    T err;
    T saturatedErr;
    T proportionalOut;
    T integralOut ;
    T derivativeOut;
    T outPreSat;
};

#define INTEGER_MAGNIFICATIONS 100
#define HALF_INTEGER_MAGNIFICATIONS 50 //用于四舍五入计算

class PID : public PIDData<int>
{
public:
    PID(void);
    ~PID(void);
    void Reset();
    void Reset(int out);
    void Update();
    void Update(int);
};

/******************* (C) COPYRIGHT 2014 TongYe **********END OF FILE****/
