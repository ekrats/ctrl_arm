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
* File: $Id: portserial.c,v 1.60 2013/08/13 15:07:05 Armink $
*/

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

#if MB_SLAVE_RTU_ENABLED > 0 || MB_SLAVE_ASCII_ENABLED > 0
/* ----------------------- static functions ---------------------------------*/
//tatic void prvvUARTTxReadyISR(void);
//static void prvvUARTRxISR(void);
/* ----------------------- Start implementation -----------------------------*/

void vMBPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable)
{
	if (xRxEnable)
	{
		SLAVE_RS485_RECEIVE_MODE;
		USART_ITConfig(MODBUS_USE_USART, USART_IT_IDLE, ENABLE);
	}
	else
	{
		SLAVE_RS485_SEND_MODE;
		USART_ITConfig(MODBUS_USE_USART, USART_IT_IDLE, DISABLE);
	}
	
	if(xTxEnable)
	{
		DMA_ITConfig(MB_TX_DMA, DMA_IT_TC, ENABLE);
	}
	else
	{
		DMA_ITConfig(MB_TX_DMA, DMA_IT_TC, DISABLE);
	}
}

void vMBPortClose(void)
{
	USART_ITConfig(MODBUS_USE_USART, USART_IT_TC | USART_IT_RXNE, DISABLE);
	USART_Cmd(MODBUS_USE_USART, DISABLE);
}

/*******************************************************************************
* Function Name  : USART_DMA_Configuration
* Description    : 配置串口发送DMA
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern volatile UCHAR ucRTUBuf[256];
static void USART_DMA_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef     NVIC_InitStructure = {0};
	
	RCC_AHB1PeriphClockCmd(MB_UART_DMA_CLK, ENABLE);  
	
	/* fill init structure */
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= MB_USART_DR_Base;
	DMA_InitStructure.DMA_Memory0BaseAddr 		= (u32)ucRTUBuf;
	DMA_InitStructure.DMA_PeripheralInc 			= DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc 					= DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize 			= DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode 								= DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority 						= DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode 						= DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold 			= DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst 				= DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst 		= DMA_PeripheralBurst_Single;
	/* DMA1 Stream3 Config */
	DMA_DeInit(MB_TX_DMA);
	DMA_InitStructure.DMA_Channel    					= MB_TX_DMA_CHANNEL;
	DMA_InitStructure.DMA_DIR        					= DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize 					= MB_BUFFER_SIZE;
	DMA_Init(MB_TX_DMA, &DMA_InitStructure);
	DMA_ClearFlag(MB_TX_DMA, DMA_FLAG_TCIF3);
	/* Enable DMA Stream Transfer Complete interrupt */
	DMA_ITConfig(MB_TX_DMA, DMA_IT_TC, ENABLE);

	/* fill init structure */
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= MB_USART_DR_Base;
	DMA_InitStructure.DMA_Memory0BaseAddr 		= (u32)ucRTUBuf;
	DMA_InitStructure.DMA_PeripheralInc 			= DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc 					= DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize 			= DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode 								= DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority 						= DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode 						= DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold 			= DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst 				= DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst 		= DMA_PeripheralBurst_Single;
	/* DMA1 Stream51 Config */
	DMA_DeInit(MB_RX_DMA);

	DMA_InitStructure.DMA_Channel    					= DMA_Channel_4;
	DMA_InitStructure.DMA_DIR        					= DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize 					= MB_BUFFER_SIZE;
	DMA_Init(MB_RX_DMA, &DMA_InitStructure);
	
	/* Enable USARTx DMA TX request */
	USART_DMACmd(MODBUS_USE_USART, USART_DMAReq_Tx, ENABLE);
	/* Enable USARTx DMA RX request */
	USART_DMACmd(MODBUS_USE_USART, USART_DMAReq_Rx, ENABLE);
	/* DMA Stream enable */
	DMA_Cmd(MB_RX_DMA, ENABLE);
	
	/* Enable DMA IRQChannel */
	NVIC_InitStructure.NVIC_IRQChannel                   = (MB_TX_DMAIRQ);
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART_DMA_IRQ_PrePriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = USART_DMA_IRQ_SubPriority;  
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


//默认一个从机 串口1 波特率可设置  奇偶检验可设置
BOOL xMBPortSerialInit(UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits,
					   eMBParity eParity)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//======================时钟初始化=======================================

	/* Enable USART3 GPIO clocks */
	RCC_AHB1PeriphClockCmd(MB_GPIO_RCC|MB_GPIO_DE_RCC, ENABLE);
	/* Enable USART3 clock */
	RCC_APB1PeriphClockCmd(MB_USART_RCC, ENABLE);

	/* Connect alternate function */
	GPIO_PinAFConfig(MB_USART_GPIO, MB_GPIO_TX_SOURCE, MB_USART_AF);
	GPIO_PinAFConfig(MB_USART_GPIO, MB_GPIO_RX_SOURCE, MB_USART_AF);

	//======================IO初始化=======================================	
	/* Configure USART Tx and Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

	GPIO_InitStructure.GPIO_Pin   = MB_GPIO_RX;
	GPIO_Init(MB_USART_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = MB_GPIO_TX;
	GPIO_Init(MB_USART_GPIO, &GPIO_InitStructure);

	//配置485发送和接收模式
	//    TODO   暂时先写B13 等之后组网测试时再修改
	/* Configure RS485DE  as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin   = MB_GPIO_DE;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(MB_DE_GPIO, &GPIO_InitStructure);
	//======================串口初始化=======================================
	USART_InitStructure.USART_BaudRate = ulBaudRate;
	//设置校验模式
	switch (eParity)
	{
	case MB_PAR_NONE: //无校验
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		break;
	case MB_PAR_ODD: //奇校验
		USART_InitStructure.USART_Parity = USART_Parity_Odd;
		USART_InitStructure.USART_WordLength = USART_WordLength_9b;
		break;
	case MB_PAR_EVEN: //偶校验
		USART_InitStructure.USART_Parity = USART_Parity_Even;
		USART_InitStructure.USART_WordLength = USART_WordLength_9b;
		break;
	default:
		return FALSE;
	}

	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_HardwareFlowControl =
		USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	if (ucPORT != 1)
		return FALSE;

	ENTER_CRITICAL_SECTION(); //关全局中断

	USART_Init(MODBUS_USE_USART, &USART_InitStructure);
	USART_ITConfig(MODBUS_USE_USART, USART_IT_IDLE, ENABLE);
	USART_Cmd(MODBUS_USE_USART, ENABLE);

	USART_DMA_Configuration();
	
	//=====================中断初始化======================================
	//设置NVIC优先级分组为Group2：0-3抢占式优先级，0-3的响应式优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = MB_USART_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	EXIT_CRITICAL_SECTION(); //开全局中断

	SLAVE_RS485_RECEIVE_MODE;
	return TRUE;
}

BOOL xMBPortSerialPutByte(CHAR ucByte)
{
	USART_SendData(MODBUS_USE_USART, ucByte);
	return TRUE;
}

BOOL xMBPortSerialGetByte(CHAR * pucByte)
{
	*pucByte = USART_ReceiveData(MODBUS_USE_USART);
	return TRUE;
}

/*******************************************************************************
* Function Name  : Bsp_Modbus_Irqhandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u32 TemDataCnter = 0;
static void Bsp_Modbus_Irqhandler(void)
{
	u16 temp = 0;
	//接收中断
	if (USART_GetITStatus(MODBUS_USE_USART, USART_IT_IDLE) == SET)
	{
		/*清除中断标志*/
		temp = MODBUS_USE_USART->SR;
		temp = MODBUS_USE_USART->DR;
		
		TemDataCnter = DMA_GetCurrDataCounter(MB_RX_DMA);
		MODBUS_USE_TIM->CNT = 0;
		TIM_Cmd(MODBUS_USE_TIM, ENABLE);
	}
	temp++;
}

extern void Mb_DMAIRQ_Exc(void);
void DMA1_Stream3_IRQHandler(void)
{
	if(DMA_GetITStatus(MB_TX_DMA, DMA_IT_TEIF3) == SET)
	{
		DMA_ClearITPendingBit(MB_TX_DMA, DMA_IT_TEIF3);
	}

	if(DMA_GetITStatus(MB_TX_DMA, DMA_IT_TCIF3) == SET)
	{
		DMA_ClearITPendingBit(MB_TX_DMA, DMA_IT_TCIF3);
	}
	
	Mb_DMAIRQ_Exc();
	
}
#ifndef RT_USING_UART3
void MB_Usart_IRQHandler(void)
{
	/* enter interrupt */
	rt_interrupt_enter();
	
	Bsp_Modbus_Irqhandler();
	/* leave interrupt */
	rt_interrupt_leave();
}
#endif

/* ----------------------- Start implementation -----------------------------*/
void EnterCriticalSection(void)
{
	//关闭全局中断
	__disable_irq();
}

void ExitCriticalSection(void)
{
	//开启全局中断
	__enable_irq();
}

#endif
