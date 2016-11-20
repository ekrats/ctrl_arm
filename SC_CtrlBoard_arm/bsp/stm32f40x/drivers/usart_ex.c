#include <stdbool.h>
#include "serial_util.h"
#include "pcmaster.h"

#define USART1_RX_DMA_CHANNEL            DMA_Channel_4
#define USART1_RX_DMA_STREAM             DMA2_Stream5
#define USART1_RX_DMA_FLAG_FEIF          DMA_FLAG_FEIF5
#define USART1_RX_DMA_FLAG_DMEIF         DMA_FLAG_DMEIF5
#define USART1_RX_DMA_FLAG_TEIF          DMA_FLAG_TEIF5
#define USART1_RX_DMA_FLAG_HTIF          DMA_FLAG_HTIF5
#define USART1_RX_DMA_FLAG_TCIF          DMA_FLAG_TCIF5

#define USART1_TX_DMA_STREAM             DMA2_Stream7
#define USART1_TX_DMA_CHANNEL            DMA_Channel_4
#define USART1_TX_DMA_FLAG_FEIF          DMA_FLAG_FEIF7
#define USART1_TX_DMA_FLAG_DMEIF         DMA_FLAG_DMEIF7
#define USART1_TX_DMA_FLAG_TEIF          DMA_FLAG_TEIF7
#define USART1_TX_DMA_FLAG_HTIF          DMA_FLAG_HTIF7
#define USART1_TX_DMA_FLAG_TCIF          DMA_FLAG_TCIF7

typedef enum
{
    use_console,
    use_freemaster,
    use_ptu,
    use_hmi,
}console_type;

u8 txBuffer[256] = {0};
u8 serial1_rx_buffer[512] = {0};
struct rt_semaphore sem_serial_rx;
int rx_length = 0;

void mem_printf(const char *buffer, int size);

static void Timer_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_DeInit(TIM7);
    TIM_TimeBaseStructure.TIM_Period = 80; 
    TIM_TimeBaseStructure.TIM_Prescaler = (168-1);
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);

    TIM_ClearFlag(TIM7, TIM_FLAG_Update);
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
}

DMA_InitTypeDef  DMA_InitStructure;
static void DMA_Configuration(void)
{
    DMA_DeInit(USART1_TX_DMA_STREAM);
    DMA_InitStructure.DMA_PeripheralBaseAddr = ((uint32_t)uart1.uart_device + 0x04);
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_InitStructure.DMA_Channel = USART1_TX_DMA_CHANNEL;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;  
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)txBuffer;
    DMA_InitStructure.DMA_BufferSize = (uint16_t)1;
    DMA_Init(USART1_TX_DMA_STREAM, &DMA_InitStructure); 

    DMA_DeInit(USART1_RX_DMA_STREAM);
    DMA_InitStructure.DMA_Channel = USART1_RX_DMA_CHANNEL;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; 
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)serial1_rx_buffer;
    DMA_InitStructure.DMA_BufferSize = (uint16_t)sizeof(serial1_rx_buffer);
    DMA_Init(USART1_RX_DMA_STREAM, &DMA_InitStructure);
}

void reset_dma_address(void)
{
    USART_DMACmd(uart1.uart_device, USART_DMAReq_Rx , DISABLE);
    DMA_Cmd(USART1_RX_DMA_STREAM, DISABLE);
    DMA_DeInit(USART1_RX_DMA_STREAM);
    DMA_Init(USART1_RX_DMA_STREAM, &DMA_InitStructure);
    USART1_RX_DMA_STREAM->NDTR = sizeof(serial1_rx_buffer);
    DMA_Cmd(USART1_RX_DMA_STREAM, ENABLE);
    USART_DMACmd(uart1.uart_device, USART_DMAReq_Rx , ENABLE);
}

void TIM7_IRQHandler(void)
{
    extern void timer7_isr(void);
    /* enter interrupt */
    rt_interrupt_enter();

    rx_length = sizeof(serial1_rx_buffer) - USART1_RX_DMA_STREAM->NDTR;
    
    reset_dma_address();
    if(sem_serial_rx.parent.parent.flag != 0)
    {
        rt_sem_release(&sem_serial_rx);
    }

    TIM_Cmd(TIM7, DISABLE);
    TIM_ClearFlag(TIM7, TIM_FLAG_Update);
    /* leave interrupt */
    rt_interrupt_leave();
}

bool is_reconnected = true;
console_type function_switch = use_console;
static void serial_services_entry( void* parameter);

void console_services_init(void)
{
    struct rt_thread * serial_services_thread = rt_thread_create("console",
                                    serial_services_entry, RT_NULL,
                                    1024, 15, 10);
    if (serial_services_thread != RT_NULL)
    {
        rt_thread_startup(serial_services_thread);
    }
    
    /* DMA clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);    
    /* TIM clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
    
    {
        NVIC_InitTypeDef NVIC_InitStructure;
                /* Enable the TIM7 Interrupt */
        NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }
    
    Timer_Configuration();
    rt_sem_init(&sem_serial_rx, "usart_rx", 0, 1);
    DMA_Configuration();
    /* Enable USART1 DMA Tx request */
    USART_DMACmd(uart1.uart_device, USART_DMAReq_Tx , ENABLE);
    USART_DMACmd(uart1.uart_device, USART_DMAReq_Rx , ENABLE);
    DMA_Cmd(USART1_RX_DMA_STREAM, ENABLE);
    /* enable interrupt */
    USART_ITConfig(uart1.uart_device, USART_IT_RXNE, DISABLE);
    USART_ITConfig(uart1.uart_device, USART_IT_IDLE, ENABLE);
}

void $Sub$$USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
    {
        USART_ReceiveData(USART1);
        TIM_SetCounter(TIM7, 0);
        TIM_Cmd(TIM7, ENABLE);
    }
}

void hmi_update(void* buffer_address, int len);

static void serial_services_entry( void* parameter)
{
    int len = rx_length;
    u8 * buffer_address = serial1_rx_buffer;
    struct rt_serial_rx_fifo* rx_fifo;
    rx_fifo = (struct rt_serial_rx_fifo*)serial1.serial_rx;
    pcmaster_init(mem_printf);
    
    while(1)
    {
        if(-RT_ETIMEOUT == rt_sem_take(&sem_serial_rx, 2000))
        {
            //需要重新确认连接的客户端类型
            is_reconnected = true;
            continue;
        }
        else
        {
            if(is_reconnected)
            {
                is_reconnected = false;
                
                if(serial1_rx_buffer[0] == '+')//freemaster起始符+
                {
                    function_switch = use_freemaster;
                }
                else if(serial1_rx_buffer[0] == '*')//通业上位机通信协议起始符*
                {
                    function_switch = use_ptu;
                }
                else if(serial1_rx_buffer[rx_length - 1] == 0x0A)//控制台指令以换行结束
                {
                    function_switch = use_console;
                }
                else
                {
                    function_switch = use_hmi;
                }
            }
        }
        
        len = rx_length;
        buffer_address = serial1_rx_buffer;
        switch (function_switch)
        {
            case use_console:
                rx_fifo->put_index = 0;
                rx_fifo->get_index = 0;
            
                while (len--)
                {
                    rx_fifo->buffer[rx_fifo->put_index++] = *buffer_address++;
                }

                if (serial1.parent.rx_indicate != RT_NULL)
                {
                    serial1.parent.rx_indicate(&serial1.parent, rx_length);
                }
                break;
            case use_freemaster:
                //PutData((const char *)buffer_address, 0, len);//--shiliangbao 140221  ????
                pcmaster_write((const char *)buffer_address, 0, len);
                break;
            case use_ptu:
                protocol.put_buffer(&protocol, (void *)buffer_address, len);
                break;
            case use_hmi:
                
                break;
        }

    }
}

void mem_printf(const char *buffer, int size)
{
    DMA_ClearFlag(USART1_TX_DMA_STREAM, USART1_TX_DMA_FLAG_HTIF | USART1_TX_DMA_FLAG_TCIF);    
    /* disable DMA */
    DMA_Cmd(USART1_TX_DMA_STREAM, DISABLE);

    /* set buffer address */
    USART1_TX_DMA_STREAM->M0AR = (rt_uint32_t)buffer;
    /* set size */
    USART1_TX_DMA_STREAM->NDTR = size;

    /* enable DMA */
    DMA_Cmd(USART1_TX_DMA_STREAM, ENABLE);
}

