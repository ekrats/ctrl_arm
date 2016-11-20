/*
* FreeModbus Libary: BARE Port
* Copyright (C) 2006 Christian Walter <wolti@sil.at>
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
* File: $Id: port.h ,v 1.60 2013/08/13 15:07:05 Armink add Master Functions $
*/

#ifndef _PORT_H
#define _PORT_H

#include <stm32f4xx_conf.h>
#define PR_BEGIN_EXTERN_C           extern "C" {
#define  PR_END_EXTERN_C            }
#include "mbconfig.h"
#include <rthw.h>
#include <rtthread.h>

#include <assert.h>
#include <inttypes.h>

//∆¡±Œ÷–∂œ±£ª§
//#define ENABLE_CRITICAL

#define MODBUS_USE_TIM							(TIM9)
#define MODBUS_USE_TIM_RCC          (RCC_APB2Periph_TIM9)
#define MODBUS_USE_TIM_IRQ          (TIM1_BRK_TIM9_IRQn)
#define MODBUS_USE_TIM_IRQ_Handler  (TIM1_BRK_TIM9_IRQHandler)

#define MODBUS_USE_USART            (USART3)
#define MB_USART_IRQ                (USART3_IRQn)
#define MB_USART_RCC                (RCC_APB1Periph_USART3)

#define MB_GPIO_TX                  (GPIO_Pin_10)
#define MB_GPIO_TX_SOURCE           (GPIO_PinSource10)
#define MB_USART_GPIO               (GPIOB)
#define MB_GPIO_RCC                 (RCC_AHB1Periph_GPIOB)
#define MB_GPIO_RX                  (GPIO_Pin_11)
#define MB_GPIO_RX_SOURCE           (GPIO_PinSource11)
#define MB_GPIO_RX_P                (GPIOB)
#define MB_USART_AF                 (GPIO_AF_USART3)

#define MB_RX_DMA										(DMA1_Stream1)
#define MB_TX_DMA										(DMA1_Stream3)
#define MB_RX_DMAIRQ								(DMA1_Stream1_IRQn)
#define MB_TX_DMAIRQ								(DMA1_Stream3_IRQn)

#define MB_RX_DMA_CHANNEL           (DMA_Channel_4)
#define MB_TX_DMA_CHANNEL           (DMA_Channel_4)

#define MB_GPIO_DE_RCC              (RCC_AHB1Periph_GPIOB)
#define MB_GPIO_DE                  (GPIO_Pin_12)
#define MB_DE_GPIO                  (GPIOB)

#define MB_UART_DMA_CLK							(RCC_AHB1Periph_DMA1)
#define MB_USART_DR_OFFSET          (0x04)
#define MB_USART_DR_Base           	(USART3_BASE + MB_USART_DR_OFFSET)//0x40004804

#define	MB_BUFFER_SIZE							(512)

#define	MB_USART_DMA_IRQ						(DMA1_Stream3_IRQn)
#define	USART_DMA_IRQ_PrePriority   (1)
#define USART_DMA_IRQ_SubPriority		(2)


#define	MB_Usart_IRQHandler					(USART3_IRQHandler)

#define INLINE

#define SLAVE_RS485_RECEIVE_MODE    GPIO_SetBits(MB_DE_GPIO,MB_GPIO_DE)
#define SLAVE_RS485_SEND_MODE       GPIO_ResetBits(MB_DE_GPIO,MB_GPIO_DE)

#define MASTER_RS485_RECEIVE_MODE   GPIO_SetBits(MB_DE_GPIO,MB_GPIO_DE)
#define MASTER_RS485_SEND_MODE      GPIO_ResetBits(MB_DE_GPIO,MB_GPIO_DE)

#ifdef ENABLE_CRITICAL
#define ENTER_CRITICAL_SECTION()    EnterCriticalSection()
#define EXIT_CRITICAL_SECTION()     ExitCriticalSection()
#else
#define ENTER_CRITICAL_SECTION()
#define EXIT_CRITICAL_SECTION()
#endif

void EnterCriticalSection(void);
void ExitCriticalSection(void);

typedef uint8_t BOOL;

typedef unsigned char UCHAR;
typedef char    CHAR;

typedef uint16_t USHORT;
typedef int16_t SHORT;

typedef uint32_t ULONG;
typedef int32_t LONG;

#ifndef TRUE
#define TRUE            1
#endif

#ifndef FALSE
#define FALSE           0
#endif

#endif
