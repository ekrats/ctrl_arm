/**
  *************************************************************************
  * @file    para_set.h
  * @brief   This file contains all the parameters for the project
  *          
  * Change Logs:
  * Date			Author			Notes
  * 2016/05/24		方波				the first version
  *************************************************************************
  */

#ifndef __PARASET__H__
#define __PARASET__H__

/*****************************************************************************
*采样量程
*****************************************************************************/
#define uInUV_Range			4000		//输入UV采样量程
#define uInVW_Range			4000		//输入VW采样量程
#define uBus_Range			4000		//母线电压采样量程
#define Earth_Range        4000       //接地故障电压采样量程

/*****************************************************************************
*保护值
*****************************************************************************/
#define uInMinL1_Value		2970		//输入欠压值1
#define uInMinL2_Value		2640		//输入欠压值2
#define uInMaxL1_Value		6050		//输入过压值1
#define uInMaxL2_Value		6600		//输入过压值2
//#define uBusMin_Value		1000		//母线欠压值1
//#define uBusMin_Value		1000		//母线欠压值2
#define uBusMaxL1_Value		6050		//母线过压值1
#define uBusMaxL2_Value		6600		//母线过压值2
#define batTempMax_Value    90         //蓄电池温上限高
#define batTempMin_Value    -50        //蓄电池温度下限

/*****************************************************************************
*额定值
*****************************************************************************/
#define uBusRatedValue      6000       //母线额定值







#endif
