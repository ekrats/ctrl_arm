/**
  *************************************************************************
  * This file is part of GBC01
  * COPYRIGHT (C) 1999-2013, TongYe Inverter Development Team
  * @file    ad.c
  * @brief   This file provides all the ad firmware functions.
  * Change Logs:
  * Date           Author       Notes
  * 2016/05/11     Coase        the first version
  *************************************************************************
  */

/* Includes -------------------------------------------------------------*/

#include "ad.h"
#include <rtthread.h>
#include "static_mem.h"

#define ADC_DR_OFFSET      ((u32)0x4C)
#define ADC1_DR_ADRESS     ((u32)(ADC1_BASE + ADC_DR_OFFSET))
#define ADC2_DR_ADRESS     ((u32)(ADC2_BASE + ADC_DR_OFFSET))
//-------------------------------------------------------------------------------------------------
//ADC公共数据寄存器地址
#define  ADC_CDR_ADDRESS   (ADC_BASE+0x08)//((uint32_t)0x40012308) 
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//ADC相关RCC
#define ADC_RCC (RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2 | RCC_APB2Periph_ADC3)
#define ADC_GPIO_RCC (RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOA)
#define ADC_DMA_RCC (RCC_AHB1Periph_DMA2)
//-------------------------------------------------------------------------------------------------
//ADC管脚及EXTI触发ADC管脚
#define ADC_PORT1 (GPIOA)
#define ADC_PORT1_PIN (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3) 
#define ADC_PORT3 (GPIOC)
#define ADC_PORT3_PIN (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3)


//-------------------------------------------------------------------------------------------------
//触发ADC极性(边缘)
#define ADC_EXTI_POLARITY (EXTI_Trigger_Falling)//EXTI_Trigger_Rising
#define ADC_TRIGGER_POLARITY (ADC_ExternalTrigConvEdge_Rising)

//-------------------------------------------------------------------------------------------------
//ADC的DMA
#define ADC_DMA_RCC     (RCC_AHB1Periph_DMA2)
#define ADC1_DMA_Stream  (DMA2_Stream4)
#define ADC1_DMA_Channel (DMA_Channel_0)
#define ADC2_DMA_Stream  (DMA2_Stream3)
#define ADC2_DMA_Channel (DMA_Channel_1)
//-------------------------------------------------------------------------------------------------
//ADC中断源及优先级
#define ADC_DMA_IRQ (DMA2_Stream4_IRQn)
#define ADC_DMA_PRE_PRIO (1)
#define ADC_DMA_SUB_PRIO (2)
#define ADC_DMA_BUFFER_LENGTH (96)//96//wx 110
#define ADC1_DMA_BUFFER_LENGTH (32)//32

static u16 adc_value[3 * ADC1_DMA_BUFFER_LENGTH];
static u16 iin_value[ADC_DMA_BUFFER_LENGTH];

//-------------------------------------------------------------------------------------------------
//ADC采样时间
#define ADC_SAMPLE_TIME (ADC_SampleTime_15Cycles)
//-------------------------------------------------------------------------------------------------

/* Private macro ---------------------------------------------------------*/
/* Private variables -----------------------------------------------------*/
static int * ad_buffer;
//===============================================================================
//ADC 相关 RCC config
//===============================================================================
static void add_rcc_config(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); 

	//ADC RCC
	RCC_APB2PeriphClockCmd(ADC_RCC, ENABLE);
	//ADC_GPIO RCC
	RCC_AHB1PeriphClockCmd(ADC_GPIO_RCC, ENABLE);
	//ADC_DMA RCC
	RCC_AHB1PeriphClockCmd(ADC_DMA_RCC, ENABLE);
}

//===============================================================================
//ADC EXTI  config
//===============================================================================
static void adc_exti_config(void)
{  
    EXTI_InitTypeDef   EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line11;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//EXTI_Mode_Event; 
    EXTI_InitStructure.EXTI_Trigger = ADC_EXTI_POLARITY;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}

//===============================================================================
//ADC 相关 GPIO config
//===============================================================================
static void adc_gpio_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    
	GPIO_InitStructure.GPIO_Pin = ADC_PORT1_PIN;
	GPIO_Init(ADC_PORT1, &GPIO_InitStructure);
    
	GPIO_InitStructure.GPIO_Pin = ADC_PORT3_PIN;
	GPIO_Init(ADC_PORT3, &GPIO_InitStructure);
    
    /* Configure pin as output */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource11);	
}

//===============================================================================
//ADC config
//===============================================================================
static void adc_config(void)
{
	ADC_InitTypeDef ADC_InitStructure = {0};
	ADC_CommonInitTypeDef ADC_CommonInitStructure = {0};
	//ADC Common Init
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//ADC时钟预分频2
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//DMA模式
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);
	//ADC 1,2,3 Init
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    
    //ADC1
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//触发后只转换一轮
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Falling;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;//ADC_ExternalTrigConv_Ext_IT11;
 	ADC_InitStructure.ADC_NbrOfConversion = 3;//一个ADC转换通道数	  
   
	ADC_Init(ADC1, &ADC_InitStructure); 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SAMPLE_TIME);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SAMPLE_TIME);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 3, ADC_SAMPLE_TIME);
    
    //ADC2
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC2, &ADC_InitStructure);
    ADC_RegularChannelConfig(ADC2, ADC_Channel_1, 1, ADC_SampleTime_15Cycles);//wx ADC_SampleTime_3Cycles
    
    //ADC3
	ADC_Init(ADC3, &ADC_InitStructure);
    ADC_InjectedSequencerLengthConfig(ADC3, 2);
	ADC_InjectedChannelConfig(ADC3, ADC_Channel_10, 1, ADC_SampleTime_480Cycles);
	ADC_InjectedChannelConfig(ADC3, ADC_Channel_11, 2, ADC_SampleTime_480Cycles);
    //Enable ADC1，ADC2，ADC3
	ADC_Cmd(ADC1, ENABLE);
	ADC_Cmd(ADC2, ENABLE);
	ADC_Cmd(ADC3, ENABLE);
}

//===============================================================================
//ADC DMA config
//===============================================================================
static DMA_InitTypeDef DMA_InitStructure = { 0 };

static void adc_dma_config()
{
    //ADC1为主，ADC2与ADC3为从。所以只需配置ADC1的DMA 
    DMA_DeInit(ADC1_DMA_Stream);// 
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)ADC1_DR_ADRESS;
    DMA_InitStructure.DMA_Memory0BaseAddr = (u32)adc_value;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize = 3 * ADC1_DMA_BUFFER_LENGTH;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_Channel = ADC1_DMA_Channel;//DMA_Channel_0;//ADC1的DMAchannel
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;	
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;	
    DMA_Init(ADC1_DMA_Stream, &DMA_InitStructure);
    
    DMA_DeInit(ADC2_DMA_Stream);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)ADC2_DR_ADRESS;
    DMA_InitStructure.DMA_Memory0BaseAddr = (u32)iin_value;
    DMA_InitStructure.DMA_Channel = ADC2_DMA_Channel;
    DMA_InitStructure.DMA_BufferSize = ADC_DMA_BUFFER_LENGTH;
    DMA_Init(ADC2_DMA_Stream, &DMA_InitStructure);
    
    //使能ADC的DMA
    DMA_Cmd(ADC1_DMA_Stream, ENABLE);
    DMA_Cmd(ADC2_DMA_Stream, ENABLE);
    
	DMA_ClearITPendingBit(ADC1_DMA_Stream, DMA_IT_TCIF4 | DMA_IT_FEIF4
		| DMA_IT_DMEIF4 | DMA_IT_TEIF4 | DMA_IT_HTIF4 | DMA_IT_TCIF4);
    //使能DMA传输完成中断
    DMA_ITConfig(ADC1_DMA_Stream, DMA_IT_TC, ENABLE);
    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
    ADC_DMARequestAfterLastTransferCmd(ADC2, ENABLE);
    ADC_DMACmd(ADC1, ENABLE);
    ADC_DMACmd(ADC2, ENABLE);
}


//===============================================================================
//ADC NVIC config
//===============================================================================
static void adc_nvic_config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    //Configure and enable ADC interrupt 
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ADC_DMA_PRE_PRIO;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = ADC_DMA_SUB_PRIO;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 
}

void adc_watchdog_config(void)
{
    ADC_AnalogWatchdogSingleChannelConfig(ADC2, ADC_Channel_1);
    ADC_AnalogWatchdogThresholdsConfig(ADC2, 4095, 0);
    ADC_AnalogWatchdogCmd(ADC2, ADC_AnalogWatchdog_SingleRegEnable);
}

void ADC_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();
    if (SET == ADC_GetITStatus(ADC2, ADC_IT_AWD))
    {
        ADC_ITConfig(ADC2, ADC_IT_AWD, DISABLE);

        ADC_ClearFlag(ADC2, ADC_FLAG_AWD);
        ADC_ClearITPendingBit(ADC2, ADC_IT_AWD);
        //分析采样结果
        GPIO_SetBits(GPIOC, GPIO_Pin_8);
        {
            //脉冲封锁
            int i = 42;
            while(i-->0);
        }
        GPIO_ResetBits(GPIOC, GPIO_Pin_8);
    }
    /* leave interrupt */
    rt_interrupt_leave();
}

//===============================================================================
//ADC 硬件初始化
//===============================================================================
//struct rt_semaphore sem_ad_proc = {0};
void adc_init(void)
{
    ad_buffer = static_malloc(7 * 4);
    ad_buffer[0] = 0;
    ad_buffer[1] = 0;
    ad_buffer[2] = 0;
    ad_buffer[3] = 0;
	add_rcc_config();
	adc_gpio_config();
    adc_exti_config();
	adc_config();
    adc_watchdog_config();
    adc_nvic_config();
    adc_dma_config();
}

void adc_update(void)
{
    int i = 0;
    u16 * p = adc_value;
    int sum1 = 0;
    int sum2 = 0;
    int sum3 = 0;

    for(; i < ADC1_DMA_BUFFER_LENGTH; i++)
    {
        sum1 = sum1 + *p++;
        sum2 = sum2 + *p++;
        sum3 = sum3 + *p++;
    }
    
    ad_buffer[u_cfly_index] = sum1 / ADC1_DMA_BUFFER_LENGTH;  //;adc_value[0];
    ad_buffer[u_out_index] =  sum2 / ADC1_DMA_BUFFER_LENGTH;  //;adc_value[1];
    ad_buffer[u_in_index] =   sum3 / ADC1_DMA_BUFFER_LENGTH;  //;adc_value[2];
}

void adc_i_update(void)
{
    int i = 0;
    int sum = 0;
    for(; i < ADC_DMA_BUFFER_LENGTH; i++)
    {
        sum += iin_value[i];
    }
    sum = (sum + ADC_DMA_BUFFER_LENGTH / 2) / ADC_DMA_BUFFER_LENGTH;
    ad_buffer[i_out_index] = sum - 2048;
    ad_buffer[i_out_peak_index] = sum - 2048;
}

int max_i_out = 0;
void EXTI15_10_IRQHandler(void)
{
    extern struct rt_semaphore sem_ad_task;
    if(EXTI_GetITStatus(EXTI_Line11) != RESET)
    {
        //if(GPIOC->IDR & GPIO_Pin_11)//
        {
            int i = 0;
            int sum = 0;
            int tmp = 0;
            max_i_out = iin_value[0];
            for(; i < ADC_DMA_BUFFER_LENGTH; i++)
            {
                tmp = iin_value[i];
                sum += tmp;
                if(tmp > max_i_out)
                {
                    max_i_out = tmp;
                }
            }
            sum = (sum + ADC_DMA_BUFFER_LENGTH / 2) / ADC_DMA_BUFFER_LENGTH;
            ad_buffer[i_out_index] = sum - 2048;
            ad_buffer[i_out_peak_index] = max_i_out - 2048;
            
            rt_sem_release(&sem_ad_task);
        }

        /* Clear the EXTI line 11 pending bit */
        EXTI_ClearITPendingBit(EXTI_Line11);
    }
}

void start_injected_ad(void)
{
    ad_buffer[temp1_index] = ADC3->JDR1;
    ad_buffer[temp2_index] = ADC3->JDR2;
    ADC_SoftwareStartInjectedConv(ADC3);
}

int * get_ad_data_buffer(void)
{
    return ad_buffer;
}

void set_target_current(int current_ref)
{
    int tmp = (current_ref * 2048 + 225) / 450;
    if((tmp > 2047) || (tmp < 0))
    {
        tmp = 2047;
    }
    ADC_AnalogWatchdogThresholdsConfig(ADC2, 2048 + tmp, 2048 - tmp);
}



/******************* (C) COPYRIGHT 2013 TongYe **********END OF FILE****/

/*@}*/

