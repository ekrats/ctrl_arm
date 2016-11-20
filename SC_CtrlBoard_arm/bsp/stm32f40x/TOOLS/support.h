/**
  *************************************************************************
  * This file is part of Platform
  * COPYRIGHT (C) 1999-2014, TongYe R&D Team
  * @file    support.h
  * @brief   This file contains all the functions prototypes for the   
  *          support firmware library.
  * Change Logs:
  * Date           Author       Notes
  * 2014/04/23     Coase        the first version
  *************************************************************************
  */

#ifndef __SUPPORT__H__
#define __SUPPORT__H__

int Filter(int filterValue, int newValue, int para);
float Filter(int filterValue, int newValue, float para);
float Filter(float filterValue, float newValue, float para);
inline int Max(int value1, int value2)
{
    return (value1 > value2) ? value1 : value2;
}

#endif

/******************* (C) COPYRIGHT 2014 TongYe **********END OF FILE****/


/*@}*/



