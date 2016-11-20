/*
* FreeModbus Libary: STM32 Port
* Copyright (C) 2013 Armink <armink.ztl@gmail.com>
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*
* File: $Id: porttimer.c,v 1.60 2013/08/13 15:07:05 Armink $
*/

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#if MB_SLAVE_RTU_ENABLED > 0 || MB_SLAVE_ASCII_ENABLED > 0

static void Mb_TIMx_Configration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure = {0};
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = { 0 };
	rt_uint16_t overtime;

	//TIM_Load = 单字节位数 * 超时字节数 * 定时器频率 / 设定波特率
	//           整个运算，凡遇到小数，一律进位
	overtime = (rt_uint16_t)((10 * 3.5 * 10000 / 115200) + 1);

	if (overtime <= 5)
		overtime = 5;
	/* TIM clock enable [TIM定时器允许]*/
	RCC_APB2PeriphClockCmd(MODBUS_USE_TIM_RCC, ENABLE);
	//---------------------------------------------------------------------------
	// 初始化定时器，TIM
	//---------------------------------------------------------------------------
	TIM_DeInit(MODBUS_USE_TIM);//复位TIM定时器
	/*----------------------------------------------------------------
	TIM Configuration: Output Compare Timing Mode:
	TIM4CLK = 84 MHz, Prescaler = 8400, TIM counter clock = 10 KHz
	----------------------------------------------------------------*/
	/* TIM configuration */
	TIM_TimeBaseStructure.TIM_Period = overtime;
	TIM_TimeBaseStructure.TIM_Prescaler = 8400 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(MODBUS_USE_TIM, &TIM_TimeBaseStructure);
	TIM_ARRPreloadConfig(MODBUS_USE_TIM, ENABLE);
	/* Clear TIM  update pending flag[清除TIM 溢出中断标志] */
	TIM_ClearFlag(MODBUS_USE_TIM, TIM_FLAG_Update);
	/* Enable TIM  Update interrupt [TIM 溢出中断允许]*/
	TIM_ITConfig(MODBUS_USE_TIM, TIM_IT_Update, ENABLE);
	TIM_ClearFlag(MODBUS_USE_TIM, TIM_FLAG_Update);
	/* Enable TIMx IRQChannel */
	NVIC_InitStructure.NVIC_IRQChannel                   = MODBUS_USE_TIM_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;  
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

/*******************************************************************************
* Function Name  : MODBUS_USE_TIM_IRQ_Handler
* Description    :
* Input          : None
* Return         : None
* Author		 : Xiou
* changlog		 :
*******************************************************************************/
extern u32 TemDataCnter;
extern volatile USHORT usRcvBufferPos;
#define MB_SER_PDU_SIZE_MAX     256
void MODBUS_USE_TIM_IRQ_Handler(void)
{
	if (TIM_GetITStatus(MODBUS_USE_TIM, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(MODBUS_USE_TIM, TIM_IT_Update);
		//清零关闭定时器
		TIM_SetCounter(MODBUS_USE_TIM, 0);		
		TIM_Cmd(MODBUS_USE_TIM, DISABLE);

		/* 超时判断，DMA接收长度不变则一帧数据接收完毕 ，否则等待下一次USART空闲中断重启计时*/
		if (TemDataCnter == DMA_GetCurrDataCounter(MB_RX_DMA))
		{
			/* Disable the USART RX DMA request */
			USART_DMACmd(MODBUS_USE_USART, USART_DMAReq_Rx, DISABLE);
			/* Disable the DMA  RX Streams */
			DMA_Cmd(MB_RX_DMA, DISABLE);

			/* Read the DMA RX Stream receive data number */
			usRcvBufferPos = MB_BUFFER_SIZE - DMA_GetCurrDataCounter(MB_RX_DMA);

			/* Clear all falgs of DMA RX Streams */
			DMA_ClearFlag(MB_RX_DMA, DMA_FLAG_FEIF1 | DMA_FLAG_DMEIF1 | 
				DMA_FLAG_TEIF1 |  DMA_FLAG_HTIF1 | 
				DMA_FLAG_TCIF1);
			/* Set the DMA RX Stream buffer lenth */
			MB_RX_DMA->NDTR = MB_BUFFER_SIZE;
			/* Enable the USART RX DMA request */
			DMA_Cmd(MB_RX_DMA, ENABLE);
			USART_DMACmd(MODBUS_USE_USART, USART_DMAReq_Rx, ENABLE);

			if((usRcvBufferPos < MB_SER_PDU_SIZE_MAX) && (usRcvBufferPos > 4))
				xMBPortEventPost(EV_FRAME_RECEIVED);
		}
	}
}

/* ----------------------- Start implementation -----------------------------*/
BOOL xMBPortTimersInit(void)
{
	Mb_TIMx_Configration();
	return TRUE;
}

#endif
