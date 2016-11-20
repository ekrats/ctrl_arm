/*!
 * This is a part of the TongYe Foundation Classes C++ library.
 * COPYRIGHT (C) 1999-2014, TongYe Corporation All rights reserved.
 * @file    RelayModule.h
 * @brief   This file contains definition of class RelayModule
 * Change Logs:
 * Date           Author       Notes
 * 2014/11/14     Coase        the first version
 */

#pragma once
#include "Relay.h"
#include "List.h"

class RelayModule
{
public:
    RelayModule()
    {
    }
    ~RelayModule()
    {
    }
    /**
     * @brief This function add relay to relay list
     *
     * @param relay the object to be unified managed
     *
     * @return 
     */
    void Register( Relay * relay );
    /**
     * @brief This function reset all relays in relay list
     *
     *
     * @return NULL
     */
    void Reset();
    /**
     * @brief This function update all relays in relay list
     *
     * @return NULL
     */
    void Refresh();
private:
    // 摘要: 
    //     延时器列表。
    List<Relay *> list;
    // 摘要: 
    //     延时器列表锁。操作列表时锁定，操作完毕时解锁。
    bool listLock;
};

/******************* (C) COPYRIGHT 2014 TongYe **********END OF FILE****/
