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
    // 摘要: 
    //     锁定故障。
    inline void Lock()
    {
        isLocked = true;
    }

    // 摘要: 
    //     故障是否锁定。
    //
    // 返回结果:
    //     若故障已锁定，则返回TRUE，否则返回FALSE。
    inline bool IsLocked()
    {
        return isLocked;
    }

    // 摘要: 
    //     是否发生故障。
    //
    // 返回结果:
    //     若已发生故障，则返回TRUE，否则返回FALSE。
    inline bool IsOccurred()
    {
        return isOccurred;
    }

    // 摘要: 
    //     设置故障允许最大发生的次数。
    //
    // 参数:
    //   times
    //     最大允许发生的次数，若故障不允许重复，则设置值为0。
    inline void SetLimitedTimes(int times)
    {
        maxTimes = times;
    }

    ~Failure(void)
    {
    }


    // 摘要: 
    //     清除故障状态。
    inline void Reset()
    {
        isOccurred = false;
    }

public:
    // 摘要: 
    //     使用指定次数初始化 Failure 类的新实例。
    //
    // 参数:
    //   times
    //     最大允许发生的次数，默认为3次。
    Failure(int times = 3);

    // 摘要: 
    //     产生一次故障。增加故障次数，并根据当前已发生的次数判断是否锁定故障。
    virtual void Encounter();

    // 摘要:
    //     减少故障发生的次数。
    void Degrade();

    // 摘要:
    //     解锁并重置计数。
    void Unlock();
protected:
    // 摘要:
    //     是否发生故障。
    bool isOccurred;

    // 摘要:
    //     是否锁定。
    bool isLocked;

    // 摘要:
    //     允许发生的最大次数。
    unsigned int maxTimes;

    // 摘要:
    //     当前时刻已发生的次数。
    unsigned int presentTimes;
};

/******************* (C) COPYRIGHT 2014 TongYe **********END OF FILE****/
