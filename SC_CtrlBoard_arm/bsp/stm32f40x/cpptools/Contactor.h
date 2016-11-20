/*!
 * This is a part of the TongYe Foundation Classes C++ library.
 * COPYRIGHT (C) 1999-2014, TongYe Corporation All rights reserved.
 * @file    Contactor.h
 * @brief   This file contains definition of class Contactor
 * Change Logs:
 * Date           Author       Notes
 * 2014/10/23     Coase        the first version
 * 2015/05/25     Coase        current version
 * modify IsClosed function , return feedback not cmd
 */

#pragma  once
#include "OnOff.h"
#include "Relay.h"
#include "IOTransducer.h"

class Contactor : public OnOff , public IOTransducer, public Relay
{
public:

    // 摘要: 
    //     使用指定缓存区长度初始化 Contactor 类的新实例。
    //
    // 参数:
    //   length
    //     缓存区长度，默认为60
    Contactor(int length = 60) : IOTransducer(length)
    {
        NormalOpen();
        isAbnormal = false;
    }


    ~Contactor()
    {
    }

    // 摘要: 
    //     设置接触器属性为常开式接触器。
    inline void NormalOpen()
    {
        isNormalOpen = true;
    }

    // 摘要: 
    //     设置接触器属性为常闭式接触器。
    inline void NormalClose()
    {
        isNormalOpen = false;
    }

    // 摘要: 
    //     接触器是否有故障。
    //
    // 返回结果: 
    //     接触器的故障状态。
    inline bool IsFault()
    {
        return isAbnormal;
    }

    // 摘要: 
    //     接触器是否有闭合反馈。
    //
    // 返回结果: 
    //     接触器的闭合状态。
    inline bool IsWithFeedback()
    {
        return isWithFeedback;
    }

    // 摘要: 
    //     接触器是否闭合。
    //
    // 返回结果: 
    //     接触器的闭合状态。
    inline bool IsClosed()
    {
        return filterValue;
    }

    // 摘要: 
    //     根据给定的反馈信号源指针以及开闭操作目的指针初始化接触器。
    //
    // 参数: 
    //   feedBackAddress:
    //     反馈信号数据源指针。
    //   destination:
    //     接触器开闭操作的目标地址指针。
    //   checkTime:
    //     传感器检测延时时间，单位：MS。
    void Init(int * feedBackAddress, int * destination,
        int checkTime)
    {
        this->destination = destination;
        IOTransducer::Init(feedBackAddress);
        SetRelayTime(checkTime);
    }

    // 摘要: 
    //     检测传感器是否有故障。
    void Check()
    {
        if (isNormalOpen)
        {
            NormalOpenLogic();
        }
        else
        {
            NormalCloseLogic();
        }
    }

private:
    // 摘要: 
    //     是否是常开式接触器。
    bool isNormalOpen;

    // 摘要: 
    //     传感器是否正常。
    bool isAbnormal;

    // 摘要: 
    //     是否有反馈信号。
    bool isWithFeedback;
    bool padding;

    // 摘要: 
    //     常开式接触器检测逻辑。
    void NormalOpenLogic();

    // 摘要: 
    //     常闭式接触器检测逻辑。
    void NormalCloseLogic();
};

/******************* (C) COPYRIGHT 2014 TongYe **********END OF FILE****/
