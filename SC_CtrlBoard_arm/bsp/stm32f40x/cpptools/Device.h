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
    // 摘要: 
    //     初始化 Device 类的新实例。
    Device()
    {
        isStartEnable = false;
    }

    // 摘要: 
    //     删除 Device 类的实例。
    ~Device()
    {

    }

    // 摘要: 
    //     运行设备。将设备设置为运行状态。
    virtual inline void On()
    {
        isStartEnable = true;
    }

    // 摘要: 
    //     停止运行。将设备设置为停止运行状态。
    virtual inline void Off()
    {
        isStartEnable = false;
    }

    // 摘要: 
    //     设备运行。根据设备的运行状态执行运行或停止逻辑。
    virtual void Run() = 0;
protected:
    // 摘要: 
    //     设备的运行状态。
    bool isStartEnable;
    bool res;
};

/******************* (C) COPYRIGHT 2014 TongYe **********END OF FILE****/
