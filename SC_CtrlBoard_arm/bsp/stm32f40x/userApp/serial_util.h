/**
  *************************************************************************
  * This file is part of CM80A
  * COPYRIGHT (C) 1999-2013, TongYe Inverter Development Team
  * @file    serial_comm_util.h
  * @brief   This file contains all the functions prototypes for the   
  *          serial_comm_util firmware library.
  * Change Logs:
  * Date           Author       Notes
  * 2013/10/31     Coase        the first version
  * 2016/05/19     Coase        the current version
  *************************************************************************
  */

#ifndef __SERIAL_UTIL__H__
#define __SERIAL_UTIL__H__

#include "stm32f4xx.h"
#include "serial_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

extern serial_protocol protocol;

typedef struct
{
	u8 ignore_condition : 1;//忽略限制条件，强制进入下个状态（强制逻辑）
	u8 reset_mcu : 1; //复位MCU
	u8 enable_precharger : 1;//进入预充电（建议移除）
	u8 enable_igbt : 1;//闭合预充电IGBT
	u8 bch_on : 1;//启动充电机
	u8 enable_main_contactor : 1;//闭合主接触器
	u8 enable_inverter : 1;//允许启动逆变
	u8 stop_bch : 1;

	u8 stop : 1;//停机
	u8 clear_fault : 1; //1执行清故障
	u8 clear_lock : 1;  //1执行清锁
	u8 force_false : 1;//nu
	u8 debug_enable : 1;// = 0 所有位无效
	u8 pwmEnable : 1;//nu
	u8 res : 2;

	u16 fan_speed;
}debug_control_t;
extern debug_control_t debug_instruction;

void serial_comm_init(void);
void mem_printf(const char *buffer, int size);

#ifdef __cplusplus
}
#endif

#endif

/******************* (C) COPYRIGHT 2013 TongYe **********END OF FILE****/


/*@}*/


