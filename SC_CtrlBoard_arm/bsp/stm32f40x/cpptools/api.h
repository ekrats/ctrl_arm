/*!
 * This is a part of the TongYe Foundation Classes C++ library.
 * COPYRIGHT (C) 1999-2015, TongYe Corporation All rights reserved.
 * @file    api.h
 * @brief   This file contains interface of public functions
 * Change Logs:
 * Date           Author       Notes
 * 2015/05/12     Coase        the first version
 */

#pragma once

#include "Relay.h"

inline bool IsMaximizing(int value, int valueLimited, Relay * relay)
{
    if (value > valueLimited)
    {
        relay->Start();
    }
    else
    {
        relay->Stop();
    }

    return relay->GetResult();
}

inline bool IsMaximizing(int value, int valueLimited)
{
    return (value > valueLimited);
}

inline bool IsMaximizing(int value, unsigned int valueLimited, bool isAbsolute = true)
{
    int tmp = valueLimited;
    return ((value > tmp) || (value < -tmp));
}

inline bool IsMaximizing(int value, int valueLimited, int * count, int countLimited)
{
    if (value > valueLimited)
    {
        (*count)++;
    }
    else
    {
        *count = 0;
    }

    return (*count > countLimited);
}

inline bool IsMinimizing(int value, int valueLimited, Relay * relay)
{
    if (value < valueLimited)
    {
        relay->Start();
    }
    else
    {
        relay->Stop();
    }

    return relay->GetResult();
}

inline bool IsMinimizing(int value, int valueLimited)
{
    return (value < valueLimited);
}

inline bool IsMinimizing(int value, int valueLimited, int * count, int countLimited)
{
    if (value < valueLimited)
    {
        (*count)++;
    }
    else
    {
        *count = 0;
    }

    return (*count > countLimited);
}


