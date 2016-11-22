/*!
 * This is a part of the TongYe Foundation Classes C++ library.
 * COPYRIGHT (C) 1999-2014, TongYe Corporation All rights reserved.
 * @file    Counter.h
 * @brief   This file contains definition of class Counter
 * Change Logs:
 * Date           Author       Notes
 * 2014/04/17     Coase        the first version
 */

#pragma once
class Counter
{
public:
    Counter(int limited_counter = 5) : result(m_result)
    {
        this->limited_counter = limited_counter;
        this->present_counter = 0;
        m_result = false;
    }
    ~Counter(void)
    {
        ;
    }
    void Increase();
    void Decrease();
    void Reset();
    const bool & result;

private:
    int limited_counter;
    int present_counter;
    bool m_result;
};


/******************* (C) COPYRIGHT 2014 TongYe **********END OF FILE****/


/*@}*/


