/***********************************************************************************/
/*can_driver.c                                                                     */
/*                                                                                 */
/***********************************************************************************/
#include "can.h"
#include "can_app_def.h"
#include "rtthread.h"
#include "ScManagerExtern.h"
//-----------------------------------------------------------------------------------

static void Can_GPIO_Configuration(void);
static void Can_Configuration(void);
static void Can_Filter_Configuration(void);
static void Can_NVIC_Configuration(void);

/* Private function prototypes -----------------------------------------------*/


/*******************************************************************************
* Function Name  : static void Can_GPIO_Configuration(void)
* Description    :
*                   
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

static void Can_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};

	//---------------------------------------------------------------
	// 开启GPIO时钟，CAN1时钟和CAN2时钟
	//---------------------------------------------------------------
	RCC_AHB1PeriphClockCmd(RCC_CANPeriph_PORT, ENABLE);

#if(BSP_USE_CAN1 == 1)
	RCC_APB1PeriphClockCmd(RCC_Periph_CAN1, ENABLE);

	//---------------------------------------------------------------
	// 配置CAN1收发GPIO管脚
	//---------------------------------------------------------------
	GPIO_InitStructure.GPIO_Pin   = CAN1_Pin_RX | CAN1_Pin_TX;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(CAN1Port, &GPIO_InitStructure);

	GPIO_PinAFConfig(CAN1Port, CAN1_PinSource_RX, GPIO_AF_CAN1);
	GPIO_PinAFConfig(CAN1Port, CAN1_PinSource_TX, GPIO_AF_CAN1); 
#endif

#if(BSP_USE_CAN2 == 1)
	RCC_APB1PeriphClockCmd(RCC_Periph_CAN2, ENABLE);

	//---------------------------------------------------------------
	// 配置CAN2收发GPIO管脚
	//---------------------------------------------------------------
	GPIO_InitStructure.GPIO_Pin   = CAN2_Pin_RX | CAN2_Pin_TX;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(CAN2Port, &GPIO_InitStructure);

	GPIO_PinAFConfig(CAN2Port, CAN2_PinSource_RX, GPIO_AF_CAN2);
	GPIO_PinAFConfig(CAN2Port, CAN2_PinSource_TX, GPIO_AF_CAN2); 
#endif
}

/*******************************************************************************
* Function Name  : static void Can_Configuration(void)
* Description    : 
*                   
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

static void Can_Configuration(void)
{
	CAN_InitTypeDef CAN_InitStructure = {0};

#if(BSP_USE_CAN1 == 1)
	//---------------------------------------------------------------
	// 反初始化CAN1
	//---------------------------------------------------------------
	CAN_DeInit(CAN1);
	//---------------------------------------------------------------
	// 置缺省值至相关寄存器
	//---------------------------------------------------------------
	CAN_StructInit(&CAN_InitStructure);
	//---------------------------------------------------------------
	// 初始化CAN, 置相关寄存器，
	//---------------------------------------------------------------
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = ENABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	//---------------------------------------------------------------
	// CAN clocked at 42 MHz
	// 42M/(7*(1+4+1)) = 500Kbps
	// CAN_InitStructure.CAN_SJW  = CAN_SJW_1tq;
	// CAN_InitStructure.CAN_BS1  = CAN_BS1_12tq;
	// CAN_InitStructure.CAN_BS2  = CAN_BS2_1tq;
	// CAN_InitStructure.CAN_Prescaler = 6;  
	//---------------------------------------------------------------
	CAN_InitStructure.CAN_SJW  = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1  = CAN_BS1_3tq;
	CAN_InitStructure.CAN_BS2  = CAN_BS2_3tq;
	CAN_InitStructure.CAN_Prescaler = 6;

	CAN_Init(CAN1, &CAN_InitStructure);
#endif

#if(BSP_USE_CAN2 == 1)
	//---------------------------------------------------------------
	// 反初始化CAN2
	//---------------------------------------------------------------
	CAN_DeInit(CAN2);
	//---------------------------------------------------------------
	// 置缺省值至相关寄存器
	//---------------------------------------------------------------
	CAN_StructInit(&CAN_InitStructure);
	//---------------------------------------------------------------
	// 初始化CAN, 置相关寄存器，
	//---------------------------------------------------------------
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = ENABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	//---------------------------------------------------------------
	// CAN clocked at 42 MHz
	// 42M/(7*(1+4+1)) = 1Mbps
	// CAN_InitStructure.CAN_SJW  = CAN_SJW_1tq;
	// CAN_InitStructure.CAN_BS1  = CAN_BS1_4tq;
	// CAN_InitStructure.CAN_BS2  = CAN_BS2_1tq;
	// CAN_InitStructure.CAN_Prescaler = 7;
	//---------------------------------------------------------------
	CAN_InitStructure.CAN_SJW  = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1  = CAN_BS1_3tq;
	CAN_InitStructure.CAN_BS2  = CAN_BS2_3tq;
	CAN_InitStructure.CAN_Prescaler = 6;

	CAN_Init(CAN2, &CAN_InitStructure);
#endif
}

u16 local_id;
void init_can_id(void)
{
    u16 Keytemp;
    Keytemp = GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_15);
    Keytemp |= GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_14) << 1;
    Keytemp |= GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_13) << 2;
    Keytemp |= GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_12) << 3;
    
    switch (Keytemp)
    {
    case CAN_ID_C1:
        local_id = CAN_ID_C1;
        break;
    case CAN_ID_C2:
        local_id = CAN_ID_C2;
        break;
    case CAN_ID_C3:
        local_id = CAN_ID_C3;
        break;
    case CAN_ID_C4:
        local_id = CAN_ID_C4;
        break;
    case CAN_ID_C5:
        local_id = CAN_ID_C5;
        break;
    case CAN_ID_C6:
        local_id = CAN_ID_C6;
        break;
    case CAN_ID_C7:
        local_id = CAN_ID_C7;
        break;
    case CAN_ID_C8:
        local_id = CAN_ID_C8;
        break;
    case CAN_ID_C9:
        local_id = CAN_ID_C9;
        break;
    case CAN_ID_C10:
        local_id = CAN_ID_C10;
        break;
    case CAN_ID_C11:
        local_id = CAN_ID_C11;
        break;
    case CAN_ID_C12:
        local_id = CAN_ID_C12;
        break;
    case CAN_ID_C13:
        local_id = CAN_ID_C13;
        break;
    case CAN_ID_C14:
        local_id = CAN_ID_C14;
        break;
    case CAN_ID_C15:
        local_id = CAN_ID_C15;
        break;
    default:
        
        break;
    }

}

/*******************************************************************************
* Function Name  : static void Can_Filter_Configuration(void)
* Description    : 
*                   
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

static void Can_Filter_Configuration(void)
{
	CAN_FilterInitTypeDef CAN_FilterInitStructure = {0};

	CAN_FilterInitStructure.CAN_FilterMode           = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale          = CAN_FilterScale_32bit;
#if(BSP_USE_CAN1 == 1)
//	//----------------------------------------------------------------
//	// CAN1接收全部数据
//	//----------------------------------------------------------------
//	CAN_FilterInitStructure.CAN_FilterNumber         = 0;
//	CAN_FilterInitStructure.CAN_FilterIdHigh         = 0x0000;
//	CAN_FilterInitStructure.CAN_FilterIdLow          = 0x0000;
//	CAN_FilterInitStructure.CAN_FilterMaskIdHigh     = 0x0000;
//	CAN_FilterInitStructure.CAN_FilterMaskIdLow      = 0x0000; 
//	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
//	CAN_FilterInitStructure.CAN_FilterActivation     = ENABLE;
//	CAN_FilterInit(&CAN_FilterInitStructure);

	////---------------------------------------------------------------
	//// CAN的滤波器设定
	////---------------------------------------------------------------
	CAN_FilterInitStructure.CAN_FilterMode           = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale          = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh     = CAN_SINK_ID_MASK;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow      = 0x0;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation     = ENABLE;
	//-------------------------------------------------------------
	// CAN1滤波器设定   
	//-------------------------------------------------------------
	//第一个滤波器
	CAN_FilterInitStructure.CAN_FilterNumber         = 0;
	CAN_FilterInitStructure.CAN_FilterIdHigh         = (local_id<<3);
	CAN_FilterInitStructure.CAN_FilterIdLow          = 0x0;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	//第二个滤波器
	CAN_FilterInitStructure.CAN_FilterNumber         = 1;
	CAN_FilterInitStructure.CAN_FilterIdHigh         = (CAN_ID_Cx<<3);
	CAN_FilterInitStructure.CAN_FilterIdLow          = 0x0;
	CAN_FilterInit(&CAN_FilterInitStructure);

	//第三个滤波器
	CAN_FilterInitStructure.CAN_FilterNumber 		 = 2;//loop;     
    CAN_FilterInitStructure.CAN_FilterIdHigh 		 = (CAN_ID_ALL << 3);//Sink is all 
    CAN_FilterInitStructure.CAN_FilterIdLow 		 = 0x0000;
    CAN_FilterInit(&CAN_FilterInitStructure);
	
	//第四个滤波器
	if (local_id <= CAN_ID_C4)
	{
		CAN_FilterInitStructure.CAN_FilterNumber         = 3;
		CAN_FilterInitStructure.CAN_FilterIdHigh         = (CAN_ID_M1CX<<3);
		CAN_FilterInitStructure.CAN_FilterIdLow          = 0x0;
		CAN_FilterInit(&CAN_FilterInitStructure);
	}
	else if (local_id >= CAN_ID_C9)
	{
		CAN_FilterInitStructure.CAN_FilterNumber         = 3;
		CAN_FilterInitStructure.CAN_FilterIdHigh         = (CAN_ID_M2CX<<3);
		CAN_FilterInitStructure.CAN_FilterIdLow          = 0x0;
		CAN_FilterInit(&CAN_FilterInitStructure);
	}

#endif

#if(BSP_USE_CAN2 == 1)    
//	//----------------------------------------------------------------
//	// CAN2接收全部数据
//	//----------------------------------------------------------------
//	CAN_FilterInitStructure.CAN_FilterNumber         = 14;
//	CAN_FilterInitStructure.CAN_FilterIdHigh         = 0x0000;
//	CAN_FilterInitStructure.CAN_FilterIdLow          = 0x0000;
//	CAN_FilterInitStructure.CAN_FilterMaskIdHigh     = 0x0000;
//	CAN_FilterInitStructure.CAN_FilterMaskIdLow      = 0x0000; 
//	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
//	CAN_FilterInitStructure.CAN_FilterActivation     = ENABLE;
//	CAN_FilterInit(&CAN_FilterInitStructure);

	//---------------------------------------------------------------
	// CAN的滤波器设定
	//---------------------------------------------------------------
	CAN_FilterInitStructure.CAN_FilterMode           = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale          = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh     = CAN_SINK_ID_MASK;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow      = 0x0;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation     = ENABLE;
	////-------------------------------------------------------------
	//// CAN2滤波器设定   
	////-------------------------------------------------------------
	//第一个滤波器
	CAN_FilterInitStructure.CAN_FilterNumber         = 14;
	CAN_FilterInitStructure.CAN_FilterIdHigh         = (local_id<<3);
	CAN_FilterInitStructure.CAN_FilterIdLow          = 0x0;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	//第二个滤波器
	CAN_FilterInitStructure.CAN_FilterNumber         = 15;
	CAN_FilterInitStructure.CAN_FilterIdHigh         = (CAN_ID_Cx<<3);
	CAN_FilterInitStructure.CAN_FilterIdLow          = 0x0;
	CAN_FilterInit(&CAN_FilterInitStructure);

	//第三个滤波器
	CAN_FilterInitStructure.CAN_FilterNumber 		 = 16;//loop;     
    CAN_FilterInitStructure.CAN_FilterIdHigh 		 = (CAN_ID_ALL << 3);//Sink is all 
    CAN_FilterInitStructure.CAN_FilterIdLow 		 = 0x0000;
    CAN_FilterInit(&CAN_FilterInitStructure);
	
	//第四个滤波器
	if (local_id <= CAN_ID_C4)
	{
		CAN_FilterInitStructure.CAN_FilterNumber         = 17;
		CAN_FilterInitStructure.CAN_FilterIdHigh         = (CAN_ID_M1CX<<3);
		CAN_FilterInitStructure.CAN_FilterIdLow          = 0x0;
		CAN_FilterInit(&CAN_FilterInitStructure);
	}
	else if (local_id >= CAN_ID_C9)
	{
		CAN_FilterInitStructure.CAN_FilterNumber         = 17;
		CAN_FilterInitStructure.CAN_FilterIdHigh         = (CAN_ID_M2CX<<3);
		CAN_FilterInitStructure.CAN_FilterIdLow          = 0x0;
		CAN_FilterInit(&CAN_FilterInitStructure);
	}

#endif
}

/*******************************************************************************
* Function Name  : Can_NVIC_Configuration
* Description    : 
*                   
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

static void Can_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure = {0};

#if(BSP_USE_CAN1 == 1)
	//-------------------------------------------------------------
	// 配置CAN1中断向量表   
	//-------------------------------------------------------------
	NVIC_InitStructure.NVIC_IRQChannel                   = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

#if(BSP_USE_CAN2 == 1)
	//-------------------------------------------------------------
	// 配置CAN2中断向量表   
	//-------------------------------------------------------------
	NVIC_InitStructure.NVIC_IRQChannel                   = CAN2_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
}

/*******************************************************************************
* Function Name  : System_HW_Can_Init
* Description    :
*                   
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

int System_HW_Can_Init(void)
{
	//---------------------------------------------------------------
	// CAN GPIO管脚配置
	//---------------------------------------------------------------
	Can_GPIO_Configuration();

	//---------------------------------------------------------------
	// CAN控制器配置
	//---------------------------------------------------------------
	Can_Configuration();

	//---------------------------------------------------------------
	// CAN ID配置
	//---------------------------------------------------------------
	init_can_id();
	
	//---------------------------------------------------------------
	// CAN滤波器配置
	//---------------------------------------------------------------
	Can_Filter_Configuration();

	//---------------------------------------------------------------
	// CAN中断向量表配置
	//---------------------------------------------------------------
	Can_NVIC_Configuration();

	return 1;
}


#include "rtthread.h"
#include "static_mem.h"

struct rt_mailbox mb_can1_rx;
struct rt_mailbox mb_can2_rx;
char can1_rx_mb_pool[256];
char can2_rx_mb_pool[256];

#include "candef.h"

struct rt_mailbox mb_can_tx;
char can_tx_mb_pool[1024];
int send_cnt = 0;
static void can_tx_services_entry( void* parameter)
{
    CAN_TX_TYPEDEF * tx_info;
    uint8_t transmit_mailbox;
    while(1)
    {
        if(RT_EOK == rt_mb_recv(&mb_can_tx, (rt_uint32_t * )&tx_info, RT_WAITING_FOREVER))
        {
            int retry_times = CAN_SEND_RETRYTIMES;
            
            if((tx_info->CANx != NULL) && (CAN_GetFlagStatus(tx_info->CANx, CAN_FLAG_BOF) == SET))
            {
                return;
            }
            
            while(retry_times--)
            {
                send_cnt++;
                transmit_mailbox = CAN_Transmit(tx_info->CANx, &tx_info->data);
                if(CAN_TxStatus_NoMailBox == transmit_mailbox)//发送队列已满
                {
                    rt_thread_delay(1); 
                }
                else
                {
                    break;
                }
            }
        }
        
        rt_free(tx_info);
    }
}

void can_services_init()
{
    struct rt_thread * can_services_thread;
    rt_mb_init(&mb_can_tx, "mbct", &can_tx_mb_pool, 
                sizeof(can_tx_mb_pool) / 4, RT_IPC_FLAG_FIFO);
    
    rt_mb_init(&mb_can1_rx, "mbcr1", &can1_rx_mb_pool, 
                sizeof(can1_rx_mb_pool) / 4, RT_IPC_FLAG_FIFO);
    
    rt_mb_init(&mb_can2_rx, "mbcr2", &can2_rx_mb_pool, 
                sizeof(can2_rx_mb_pool) / 4, RT_IPC_FLAG_FIFO);
    
    can_services_thread = rt_thread_create("can_tx",
                                    can_tx_services_entry, RT_NULL,
                                    1024, 9, 10);
    if (can_services_thread != RT_NULL)
    {
        rt_thread_startup(can_services_thread);
    }
	
    //rt_sem_init(&sem_serial_rx, "usart_rx", 0, 1);
}

static CanRxMsg can1_rx_msgs[64];
static CanRxMsg can2_rx_msgs[64];
static int can1_rx_msgs_index = 0;
static int can2_rx_msgs_index = 0;

/*****************************************************************
can1  FIFO0接收中断
*****************************************************************/
void CAN1_RX0_IRQHandler(void)
{
    rt_interrupt_enter();
	if(CAN_GetITStatus(CAN1, CAN_IT_FMP0))
	{
		CAN_Receive(CAN1, CAN_FIFO0, &can1_rx_msgs[can1_rx_msgs_index]);//can使用FIFO0中断接收;
		if(-RT_EFULL == rt_mb_send(&mb_can1_rx, (rt_uint32_t)&can1_rx_msgs[can1_rx_msgs_index]))
		{
		}
		/*若有需要可插入接收队列*/
		if(++can1_rx_msgs_index == 64)
		{
			can1_rx_msgs_index = 0;
		}
	}
	rt_interrupt_leave();
}

void CAN2_RX0_IRQHandler(void)
{
    rt_interrupt_enter();
	if(CAN_GetITStatus(CAN2, CAN_IT_FF0)) 
    { 
        CAN_ClearITPendingBit(CAN2, CAN_IT_FF0); 
    } 
    else if(CAN_GetITStatus(CAN2, CAN_IT_FOV0)) 
    { 
        CAN_ClearITPendingBit(CAN2, CAN_IT_FOV0); 
    } 
    else
    { 
        CAN_Receive(CAN2, CAN_FIFO0, &can2_rx_msgs[can2_rx_msgs_index]);//can使用FIFO0中断接收;
        /*若有需要可插入接收队列*/
        rt_mb_send(&mb_can2_rx, (rt_uint32_t)&can2_rx_msgs[can2_rx_msgs_index]);
        if(++can2_rx_msgs_index == 64)
        {
            can2_rx_msgs_index = 0;
        }
    }
	rt_interrupt_leave();
}


uint32_t can_send_buffer(uint8_t port, CanTxMsg* msg)
{
    CAN_TX_TYPEDEF * tx_buffer = RT_NULL;
    
	tx_buffer = rt_malloc(sizeof(CAN_TX_TYPEDEF));
	if(tx_buffer != RT_NULL)
	{
		tx_buffer->header.port = port;
		tx_buffer->data = *msg;
		tx_buffer->CANx = (port == 1 ? CAN1 : CAN2);
		rt_mb_send(&mb_can_tx, (rt_uint32_t)tx_buffer);
	}
	else
	{
		return 1;
	}
    return 0;
}


/*********************end of file******************************/
