/*!
 * This is a part of the TongYe Foundation Classes C++ library.
 * COPYRIGHT (C) 1999-2014, TongYe Corporation All rights reserved.
 * @file    Product.h
 * @brief   This file contains definition of class Product
 * Change Logs:
 * Date           Author       Notes
 * 2014/07/19     Coase        the first version
 * 2015/06/05     Jacky        v1.1
 *      add function RefreshTransducerData
 *
 */

#pragma once
#include "Device.h"
#include "RelayModule.h"
#include "ITransducer.h"

class Product : public Device
{
public:
    // 摘要: 
    //     初始化 Product 类的新实例。
    Product()
    {
    }
    // 摘要: 
    //     删除 Product 类的实例。
    ~Product()
    {
    }

    // 摘要: 
    //     设置共享数据。
    //
    // 参数:
    //   sharedData
    //     共享数据指针
    virtual void SetSharedData(void * sharedData) = 0;

    // 摘要: 
    //     更新AD传感器列表中所有传感器数据。
    inline virtual void RefreshADData()
    {
        this->RefreshTransducerData(sensorList);
    }

    // 摘要: 
    //     更新IO传感器列表中所有传感器数据。
    inline virtual void RefreshIOData()
    {
        this->RefreshTransducerData(ioList);
    }

    // 摘要: 
    //     刷新所有延时模块。必须保证10MS的运行周期。
    virtual void RefreshRelay()
    {
        relays.Refresh();
    }

    // 摘要: 
    //     查询产品是否运行
    //
    // 返回结果:
    //     产品是否运行的状态
    inline bool IsRun()
    {
        return isStartEnable;
    }

protected:
    // 摘要: 
    //     AD传感器列表。
    List<ITransducer*> sensorList;

    // 摘要: 
    //     IO传感器列表。
    List<ITransducer*> ioList;

    // 摘要: 
    //     延时模块。
    RelayModule relays;
private:
    // 摘要: 
    //     刷新传感器列表中所有传感器的数据。
    //
    // 参数:
    //   list
    //     传感器列表。
    void RefreshTransducerData(List<ITransducer*> &list);

};

/******************* (C) COPYRIGHT 2014 TongYe **********END OF FILE****/
