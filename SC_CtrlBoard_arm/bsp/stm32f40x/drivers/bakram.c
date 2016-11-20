
/***************************************************************************

Copyright (C), 1999-2014, Tongye Tech. Co., Ltd.

* @file           Bsp_bakram.c
* @author      
* @version        V1.0.0
* @date           2014-04-28
* @brief          备份寄存器操作相关

History:          // Revision Records

<Author>             <time>       <version >            <desc>

Tongye BCU Team    2014-09-02       V1.0.0             初次建立

***************************************************************************/

/* Includes ------------------------------------------------------------------*/

#define DSA_BKP

#include <stm32f4xx.h>
#include <rtthread.h>
#include "bakram.h"

uint8_t*  _pbram;  //访问BakRAM指针

static void BKP_RCC_Configuration(void);

/*******************************************************************************
* Function Name  : System_hw_bakramInit
* Description    : 初始化后备SRAM
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void System_hw_bakramInit(void)
{
	BKP_RCC_Configuration();
	//---------------------------------------------------------
	// 初始化访问BakRAM指针
	//---------------------------------------------------------
	_pbram = (u8 *)BKPSRAM_BASE;

	return ;
}

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    :  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

static void BKP_RCC_Configuration(void)
{
	/* Enable the PWR clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* Allow access to RTC */
	PWR_BackupAccessCmd(ENABLE);

	/* Enable BKPRAM Clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_BKPSRAM, ENABLE);
}

/*******************************************************************************
* Function Name  : void pbram(void)
* Description    :  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void clrbram(u16 addr,u16 size)
{
	//---------------------------------------------------------
	// 清0备份寄存区域
	//---------------------------------------------------------
	rt_memset(_pbram+addr, 0, size);
}

#include "finsh.h"
FINSH_FUNCTION_EXPORT(clrbram, clrbram(u16 addr,u16 size))
///******************* (C) COPYRIGHT 2014 Shenzhen Tongye **********END OF FILE****/

