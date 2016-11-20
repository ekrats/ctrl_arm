/*!
 * This is a part of the TongYe Foundation Classes C++ library.
 * COPYRIGHT (C) 1999-2014, TongYe Corporation All rights reserved.
 * @file    FailureCheck.h
 * @brief   This file contains definition of class FailureCheck
 * Change Logs:
 * Date           Author       Notes
 * 2014/08/01     Coase        the first version
 */



#pragma once
#include "Failure.h"
#include "RelayModule.h"

class FailureCheck
{
public:
    // 摘要: 
    //     初始化 FailureCheck 类的新实例。
    FailureCheck()
    {
        listLock = 0;
    }

    // 摘要: 
    //     析构 FailureCheck 类。
    ~FailureCheck()
    {
    }

    // 摘要: 
    //     快速检测。
    virtual void FastCheck() = 0;

    // 摘要: 
    //     慢速检测。
    virtual void SlowCheck() = 0;

    // 摘要: 
    //     故障检测模块初始化。初始化故障列表，将所有故障加入列表。
    virtual void FaultCheckModuleInit() = 0;

    // 摘要: 
    //     重置故障列表中的所有故障。
    virtual void ResetFaulture();

    // 摘要: 
    //     解锁故障列表中的所有故障，将所有列表中的故障的已发生次数置零。
    virtual void UnlockFaulture();
    
    // 摘要: 
    //     更新故障的状态到fault值中，故障所对应的bit位为加入故障列表的顺序。
    //     更新故障的锁定状态到faultLock值中，故障所对应的bit位为加入故障列表的顺序。
    virtual void UpdateFaultState();


    // 摘要: 
    //     获取故障列表中所有故障状态的集合值。
    //
    // 返回结果:
    //     故障列表中所有故障状态的集合值。
    inline unsigned int GetFault()
    {
        return fault;
    }

    // 摘要: 
    //    获取故障列表中所有故障锁定状态的集合值。
    //
    // 返回结果:
    //     故障列表中所有故障锁定状态的集合值。
    inline unsigned int GetLock()
    {
        return faultLock;
    }

    // 摘要: 
    //    将当前发生的故障锁定。
    inline void Lock()
    {
        faultLock = fault;
    }
	
    // 摘要: 
    //    锁定指定故障。
    //
    // 参数:
    //   value
    //     指定的锁定位。
    inline void Lock(unsigned int value)
    {
        faultLock |= value;
    }
	
    // 摘要: 
    //    解锁指定故障。
    //
    // 参数:
    //   value
    //     指定的解锁位。
    inline void Unlock(unsigned int value)
    {
        faultLock &= ~value;
    }

protected:
    // 摘要: 
    //    故障列表。用于存放用户定义的故障，以便于统一操作。
    List<Failure*> failureList;

    // 摘要: 
    //    故障列表中所有故障的故障状态集合。
    //    fault中的BIT位顺序为加入故障列表的顺序。
    unsigned int fault;

    // 摘要: 
    //    故障列表中所有故障的故障锁定状态集合。
    //    faultLock中的BIT位顺序为加入故障列表的顺序。
    unsigned int faultLock;

    // 摘要: 
    //    故障列表锁。操作列表时锁定，操作完毕后解锁。
    bool listLock;
};

/******************* (C) COPYRIGHT 2014 TongYe **********END OF FILE****/
