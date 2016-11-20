/*!
 * This is a part of the TongYe Foundation Classes C++ library.
 * COPYRIGHT (C) 1999-2014, TongYe Corporation All rights reserved.
 * @file    link.h
 * @brief   This file contains definition of class link
 * Change Logs:
 * Date           Author       Notes
 * 2014/04/11     Coase        the first version
 */

#ifndef __CPP__LINK__H__
#define __CPP__LINK__H__
#include <stdio.h>

template<typename T>
class Link
{
public:
    T eletem;
    Link * next;

    Link(const T & element, Link * next = NULL)
    {
        this->eletem = element;
        this->next = next;
    }
    Link(Link * next = NULL)
    {
        this->next = next;
    }
};
#endif

/******************* (C) COPYRIGHT 2014 TongYe **********END OF FILE****/


/*@}*/

