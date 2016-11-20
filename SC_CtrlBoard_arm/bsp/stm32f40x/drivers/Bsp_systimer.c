/***************************************************************************

Copyright (C), 1999-2015, Tongye Tech. Co., Ltd.

* @file           Bsp_ad.c
* @author         SCH-Team
* @version        V1.0.0
* @date           2015-03-11
* @brief          Timer manage

History:          // Revision Records

<Author>             <time>       <version >            <desc>

xiou             	2015-03-11       V1.0.0            Timer manage

***************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include <rtthread.h>
#include <stm32f4xx.h>
#include "Bsp_systimer.h"
#include "ScManagerExtern.h"

/* Private variables ---------------------------------------------------------*/
struct rt_semaphore sem_adio_task;
struct rt_semaphore sem_logic_task;
struct rt_semaphore sem_usartTx_task;
struct rt_semaphore sem_cantask_5ms;

/* Private function prototypes -----------------------------------------------*/

static void System_TimerConfiguration(void);
static void RCC_Configuration(void);
static void NVIC_Configuration(void);


/*******************************************************************************
* Function Name  : TIM4_IRQHandler
* Description    : This function handles TIM3 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM4_IRQHandler(void)
{
	static uint8_t cnt1 = 0;
	
	/* enter interrupt */
	rt_interrupt_enter();
	
	TIM_ClearITPendingBit(BSP_TIM, TIM_IT_Update);

	if((cnt1++ % 5) == 0)
	{
		rt_sem_release(&sem_adio_task);
		Bsp_can_send_cycle_event();
	}
//	Bsp_can_send_cycle_event();
	/* leave interrupt */
	rt_interrupt_leave();
}

/*******************************************************************************
* Function Name  : System_hw_timerInit
* Description    : ��ʼ����ʱ��TIMx
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int System_hw_timerInit(void)
{
	rt_err_t rtv1,rtv2,rtv3,rtv4;

	rtv1 = rt_sem_init(&sem_adio_task,     "adio",    0,  RT_IPC_FLAG_FIFO);
	rtv2  = rt_sem_init(&sem_logic_task,   "logic",   0,  RT_IPC_FLAG_FIFO);
	rtv3  = rt_sem_init(&sem_usartTx_task, "commTx",  0,  RT_IPC_FLAG_FIFO);
	rtv4  = rt_sem_init(&sem_cantask_5ms,  "can",     0,  RT_IPC_FLAG_FIFO);

	RCC_Configuration(); 
	NVIC_Configuration();
	System_TimerConfiguration();

	if ((rtv1 == RT_EOK) && (rtv1 == RT_EOK) && (rtv2 == RT_EOK) && (rtv3 == RT_EOK) && (rtv4 == RT_EOK))
	{
		return 0;
	} 
	else
	{
		return -1;
	}
}

/*******************************************************************************
* Function Name  : System_TimerConfiguration
* Description    :  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void System_TimerConfiguration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure = {0};
	//---------------------------------------------------------------------------
	// ��ʼ����ʱ��TIM
	//---------------------------------------------------------------------------
	TIM_DeInit( BSP_TIM );//��λTIM��ʱ��
	/* ---------------------------------------------------------------
	TIM Configuration: Output Compare Timing Mode:
	TIMCLK = 84 MHz, Prescaler = 8400, TIM counter clock = 10 KHz
	--------------------------------------------------------------- */
	/* TIM configuration */
	TIM_TimeBaseStructure.TIM_Period        = 9;
	TIM_TimeBaseStructure.TIM_Prescaler     = 0;      
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;    
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;   
	TIM_TimeBaseInit(BSP_TIM, &TIM_TimeBaseStructure);

	/*--------------------------------------------------------------------- */
	/* Prescaler configuration TIM ��ʱ���趨Ϊ10KHz���������ļ�����λ��1ms */
	/*   ע�⣺��Ƶ�����ܳ���65535 ��0xFFFF��                               */
	/*--------------------------------------------------------------------- */
	TIM_PrescalerConfig(BSP_TIM, (8400 - 1), TIM_PSCReloadMode_Immediate);

	/*  */
	TIM_ARRPreloadConfig(BSP_TIM, ENABLE);
	/* Clear TIM update pending flag[���TIM ����жϱ�־] */
	TIM_ClearFlag(BSP_TIM, TIM_FLAG_Update);

	/* Enable TIM Update interrupt [TIM ����ж�����]*/
	TIM_ITConfig(BSP_TIM, TIM_IT_Update, ENABLE);  

	/* TIM enable counter [����tim����]*/
	TIM_Cmd(BSP_TIM, ENABLE);
}

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    :  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void RCC_Configuration(void)
{
	/* TIM clock enable [TIM��ʱ������]*/
	RCC_APB1PeriphClockCmd(BSP_TIM_RCC, ENABLE); 
}


/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    :  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the TIM Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel                   = BSP_TIM_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


/******************* (C) COPYRIGHT 2012 Shenzhen Tongye *****END OF FILE****/


