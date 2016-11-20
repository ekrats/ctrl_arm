/**
  *************************************************************************
  * This file is part of Platform
  * COPYRIGHT (C) 1999-2014, TongYe R&D Team
  * @file    rs485.c
  * @brief   This file provides all the rs485 firmware functions.
  * Change Logs:
  * Date           Author       Notes
  * 2014/09/14     Compiler        the first version
  *************************************************************************
  */

#include "stm32f4xx.h"
#include "rs485.h"
#include "serial_ex.h"
#include "string.h"
#include "hardware_crc32.h"

#define UART_BAUDRATE                   115200

#define UART_IRQHandler                 USART6_IRQHandler
#define UART_IRQ   		                USART6_IRQn
#define UART_GPIO_TX		            GPIO_Pin_6
#define UART_TX_PIN_SOURCE              GPIO_PinSource6
#define UART_TX_PIN_GPIO_AF             GPIO_AF_USART6

#define UART_GPIO_RX		            GPIO_Pin_7
#define UART_RX_PIN_SOURCE              GPIO_PinSource7
#define UART_RX_PIN_GPIO_AF             GPIO_AF_USART6
#define UART_GPIO			            GPIOC
#define UART_GPIO_RCC                   RCC_AHB1Periph_GPIOC
#define RCC_APBPeriph_UART	            RCC_APB2Periph_USART6
#define RCC_APBPeriph_UART_ClockCmd     RCC_APB2PeriphClockCmd

#define RS485_DE_GPIO_RCC               RCC_AHB1Periph_GPIOG
#define RS485_DE_GPIO_PORT              (GPIOG)
#define RS485_DE_PIN                    (GPIO_Pin_9)

#define UART_RX_DMA_RCC                 RCC_AHB1Periph_DMA2
#define RCC_APBPeriph_DMA_ClockCmd      RCC_AHB1PeriphClockCmd
#define UART_RX_DMA_CHANNEL             DMA_Channel_5
#define UART_RX_DMA_STREAM              DMA2_Stream1
#define UART_RX_DMA_FLAG_FEIF           DMA_FLAG_FEIF1
#define UART_RX_DMA_FLAG_DMEIF          DMA_FLAG_DMEIF1
#define UART_RX_DMA_FLAG_TEIF           DMA_FLAG_TEIF1
#define UART_RX_DMA_FLAG_HTIF           DMA_FLAG_HTIF1
#define UART_RX_DMA_FLAG_TCIF           DMA_FLAG_TCIF1
    
#define UART_TX_DMA_RCC                 RCC_AHB1Periph_DMA2
#define UART_TX_DMA_CHANNEL             DMA_Channel_5
#define UART_TX_DMA_STREAM              DMA2_Stream6
#define UART_TX_DMA_IRQ                 DMA2_Stream6_IRQn
#define UART_TX_DMA_STREAM_IRQHANDLER   DMA2_Stream6_IRQHandler
#define UART_TX_DMA_FLAG_FEIF           DMA_FLAG_FEIF6
#define UART_TX_DMA_FLAG_DMEIF          DMA_FLAG_DMEIF6
#define UART_TX_DMA_FLAG_TEIF           DMA_FLAG_TEIF6
#define UART_TX_DMA_FLAG_HTIF           DMA_FLAG_HTIF6
#define UART_TX_DMA_FLAG_TCIF           DMA_FLAG_TCIF6
#define UART_TX_DMA_IT_TEIF             DMA_IT_TEIF6
#define UART_TX_DMA_IT_TCIF             DMA_IT_TCIF6

#define UART_RX_TIMER                   TIM4
#define UART_RX_TIMER_RCC               RCC_APB1Periph_TIM4
#define RCC_APBPeriph_TIM_TIMCmd        RCC_APB1PeriphClockCmd

#define UART_RX_TIMER_IRQ               TIM4_IRQn
#define UART_RX_TIMER_IT_SOURCE         TIM_IT_Update
#define UART_RX_TIMER_FLAG_UPDATE       TIM_FLAG_Update
#define UART_RX_TIMER_IRQHANDLER        TIM4_IRQHandler

struct stm32_serial_int_rx uart6_int_rx;
struct stm32_serial_dma_tx uart6_dma_tx;
struct stm32_serial_dma_rx uart6_dma_rx;
struct stm32_serial_device uart6 =
{
    USART6,
    &uart6_int_rx,
    &uart6_dma_tx,
    &uart6_dma_rx
};

struct rt_device uart6_device;
static void rs485_net_tx_entry( void* parameter );
static void rs485_net_rx_entry( void* parameter );

static void RCC_Configuration(void)
{
    
    /* Enable UART_DEVICE GPIO clocks */
    RCC_AHB1PeriphClockCmd(UART_GPIO_RCC, ENABLE);
    /* Enable UART_DEVICE clock */
    RCC_APBPeriph_UART_ClockCmd(RCC_APBPeriph_UART, ENABLE);
    /* DMA clock enable */
    RCC_APBPeriph_DMA_ClockCmd(UART_TX_DMA_RCC, ENABLE);
    /* TIM clock enable */
    RCC_APBPeriph_TIM_TIMCmd(UART_RX_TIMER_RCC, ENABLE);
}

static void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    /* Configure UART_DEVICE Rx/tx PIN */
    GPIO_InitStructure.GPIO_Pin = UART_GPIO_RX | UART_GPIO_TX | RS485_DE_PIN;
    GPIO_Init(UART_GPIO, &GPIO_InitStructure);

    /* Connect alternate function */
    GPIO_PinAFConfig(UART_GPIO, UART_TX_PIN_SOURCE, UART_TX_PIN_GPIO_AF);
    GPIO_PinAFConfig(UART_GPIO, UART_RX_PIN_SOURCE, UART_RX_PIN_GPIO_AF);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin = RS485_DE_PIN;
    GPIO_Init(RS485_DE_GPIO_PORT, &GPIO_InitStructure);
}
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Enable the UARTx Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = UART_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable the UART_RX_TIMER Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = UART_RX_TIMER_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable the DMA Tx Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = UART_TX_DMA_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void DMA_Configuration(void)
{
    DMA_InitTypeDef  DMA_InitStructure;
    DMA_DeInit(UART_TX_DMA_STREAM);
    DMA_InitStructure.DMA_PeripheralBaseAddr = ((uint32_t)uart6.uart_device + 0x04);
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_InitStructure.DMA_Channel = UART_TX_DMA_CHANNEL;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;  
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)uart6_dma_tx.data_node_mem_pool;
    DMA_InitStructure.DMA_BufferSize = (uint16_t)1;
    DMA_Init(UART_TX_DMA_STREAM, &DMA_InitStructure); 
    DMA_ITConfig(UART_TX_DMA_STREAM, DMA_IT_TC | DMA_IT_TE, ENABLE); 

    DMA_DeInit(UART_RX_DMA_STREAM);
    DMA_InitStructure.DMA_Channel = UART_RX_DMA_CHANNEL;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; 
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)uart6_int_rx.rx_buffer;
    DMA_InitStructure.DMA_BufferSize = (uint16_t)256;
    DMA_Init(UART_RX_DMA_STREAM, &DMA_InitStructure);
}

static void Timer_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_DeInit(UART_RX_TIMER);
    TIM_TimeBaseStructure.TIM_Period = 80; 
    TIM_TimeBaseStructure.TIM_Prescaler = (84-1);
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(UART_RX_TIMER, &TIM_TimeBaseStructure);

    TIM_ClearFlag(UART_RX_TIMER, UART_RX_TIMER_FLAG_UPDATE);
    TIM_ITConfig(UART_RX_TIMER, UART_RX_TIMER_IT_SOURCE, ENABLE);
}

static void rs485_tx()
{
    GPIO_SetBits(RS485_DE_GPIO_PORT, RS485_DE_PIN);
}
static void rs485_rx()
{
    GPIO_ResetBits(RS485_DE_GPIO_PORT, RS485_DE_PIN);
}

static void thread_init()
{
    rt_thread_t thread;

    thread = rt_thread_create("inv_rx",
        rs485_net_rx_entry, RT_NULL,
        1024, 8, 5);
    if (thread != RT_NULL)
        rt_thread_startup(thread);

    thread = rt_thread_create("inv_tx",
        rs485_net_tx_entry, RT_NULL,
        1024, 8, 5);
    if (thread != RT_NULL)
        rt_thread_startup(thread);
}


void rs485_init()
{
    USART_InitTypeDef USART_InitStructure;

    RCC_Configuration();

    GPIO_Configuration();

    NVIC_Configuration();

    DMA_Configuration();

    Timer_Configuration();


    USART_InitStructure.USART_BaudRate = UART_BAUDRATE ;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(uart6.uart_device, &USART_InitStructure);

    /* register usart2 */
    uart6_dma_tx.dma_channel = UART_TX_DMA_STREAM;
    uart6_dma_tx.DMA_FLAG = UART_TX_DMA_FLAG_HTIF | UART_TX_DMA_FLAG_TCIF;
    uart6_dma_rx.dma_channel = UART_RX_DMA_STREAM;
    uart6_dma_rx.dmaConfig = * UART_RX_DMA_STREAM;
    uart6.tim = UART_RX_TIMER;
    rs485_tx();
    
    rs485_rx();

    rt_hw_serial_register(&uart6_device, "485",
        RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_STREAM,
        &uart6);

    /* Enable USARTx DMA Tx request */
    USART_DMACmd(uart6.uart_device, USART_DMAReq_Tx , ENABLE);
    USART_DMACmd(uart6.uart_device, USART_DMAReq_Rx , ENABLE);
    DMA_Cmd(UART_RX_DMA_STREAM, ENABLE);
    /* enable interrupt */
    USART_ITConfig(uart6.uart_device, USART_IT_IDLE, ENABLE);
    USART_Cmd(uart6.uart_device, ENABLE);

    thread_init();
    //rt_memset(&dataset, 0, sizeof(dataset));
    //dataset.out.header = 0xf0; 
    //dataset.out.cmd = 1; 
    //dataset.out.res = 3; 
    //dataset.out.version = 100;
    //dataset.out.hardware_version = 100;
}

struct rt_semaphore sem_485_tx;
struct rt_semaphore sem_network_rx;

void rs485_inv_irq( void )
{
    rt_sem_release(&sem_485_tx);
}

static void rs485_net_rx_entry( void* parameter )
{
    rt_sem_init(&sem_485_tx, "net_tx", 0, 1);

    while(1)
    {
        rt_sem_take(&sem_485_tx, RT_WAITING_FOREVER);
        while (USART_GetFlagStatus(uart6.uart_device, USART_FLAG_TC) == RESET);
        rs485_rx();
    }
}

u8 network_tx_buffer[64] = "hello world\r\n";
int net_rx_length = 0;
int err_cnt = 0;
int ot_cnt = 0;
int rt_cnt = 0;
static void rs485_net_tx_entry( void* parameter )
{
    extern u32 digital_input[];
    extern u32 digital_output[];
    rt_sem_init(&sem_network_rx, "net_tx", 0, 1);

    while(1)
    {
        if(-RT_ETIMEOUT != rt_sem_take(&sem_network_rx, RT_WAITING_FOREVER))
        {
            rs485_tx();
            rs485_dma_tx(&uart6_device, (const char *)network_tx_buffer, 13);
        }
    }
}

void UART_RX_TIMER_IRQHANDLER(void)
{
    /* enter interrupt */
    rt_interrupt_enter();
    net_rx_length = uart6.dma_rx->dmaConfig.NDTR - uart6.dma_rx->dma_channel->NDTR;
    reset_rx_dma(&uart6);
    rt_sem_release(&sem_network_rx);

    TIM_Cmd(UART_RX_TIMER, DISABLE);
    TIM_ClearFlag(UART_RX_TIMER, TIM_FLAG_Update);
    /* leave interrupt */
    rt_interrupt_leave();
}

void UART_TX_DMA_STREAM_IRQHANDLER(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    if(DMA_GetITStatus(UART_TX_DMA_STREAM, UART_TX_DMA_IT_TEIF) == SET)
    {
        DMA_ClearITPendingBit(UART_TX_DMA_STREAM, UART_TX_DMA_IT_TEIF);
    }

    if(DMA_GetITStatus(UART_TX_DMA_STREAM, UART_TX_DMA_IT_TCIF) == SET)
    {
        DMA_ClearITPendingBit(UART_TX_DMA_STREAM, UART_TX_DMA_IT_TCIF);
    }
    rt_sem_release(&sem_485_tx);
    /* leave interrupt */
    rt_interrupt_leave();
}

void UART_IRQHandler(void)
{
    if(USART_GetITStatus(uart6.uart_device, USART_IT_IDLE) != RESET)
    {
        USART_ReceiveData(uart6.uart_device);
        TIM_SetCounter(UART_RX_TIMER, 0);
        TIM_Cmd(UART_RX_TIMER, ENABLE);
    }
}

/******************* (C) COPYRIGHT 2014 TongYe **********END OF FILE****/

/*@}*/

