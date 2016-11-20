#include "canTask.h"
#include "stm32f4xx_can.h"
#include "rtthread.h"
#include "ScManagerExtern.h"

static u32 s_rx_timeout_flag = 0;
//-----------------------��̬�ź������߳̿ռ�------------------------------------
//ALIGN(RT_ALIGN_SIZE)
static struct rt_thread can_thread;
static rt_uint8_t can_thread_stack[CAN_THREAD_STACK];
static struct rt_thread can1_rx_thread;
static rt_uint8_t can1_rx_thread_stack[CAN_RX_THREAD_STACK];
static struct rt_thread can2_rx_thread;
static rt_uint8_t can2_rx_thread_stack[CAN_RX_THREAD_STACK];

//------------------------------------------------------------------
// ��ʼͨѶ
//------------------------------------------------------------------
uint8_t CanStartComm = 0;
//------------------------------------------------------------------
// �������߼�������¼�����
//------------------------------------------------------------------
static struct rt_event EventCanSend;
//------------------------------------------------------------------
// ��ȡ�¼��ı���
//------------------------------------------------------------------
static rt_uint32_t _ev_cansend_flag = 0;
//------------------------------------------------------------------
// �¼����� ��ʼ�������ֻ��EventCanSend��ȷ��ʼ���󣬲ſ���Send Event
//------------------------------------------------------------------
static __IO rt_err_t _ev_cansend_init = 0xFF;


/*******************************************************************************
* Function Name  :  
* Description    :
*
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Bsp_can_send_trigger_event(void)
{
	if((_ev_cansend_init == RT_EOK) && (CanStartComm == 1))
	{
		rt_event_send(&EventCanSend, CanTask_Event_Trigger);
	}
}

/*******************************************************************************
* Function Name  :  
* Description    :         
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Bsp_can_check_event(void)
{
	if(CanStartComm == 0)   return;

	rt_event_recv(&EventCanSend,
		CanTask_Event_Trigger,
		(RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR),
		RT_WAITING_FOREVER,
		&_ev_cansend_flag);
}

/*******************************************************************************
* Function Name  :  
* Description    :
*
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void Bsp_can_event_init(void)
{
	_ev_cansend_init = rt_event_init(&EventCanSend, "E_Can", RT_IPC_FLAG_FIFO);
}

//================================================================
//Function Name: 
//Description  :can�����߳���ڳ���
//Input        : 
//Output       : 
//Return       : 
//================================================================
static void can_thread_entry(void* parameter)
{
    
	for(;;)
	{
		//------------------------------------------------------
		// �����¼��ź�
		//------------------------------------------------------
		Bsp_can_check_event();

		//------------------------------------------------------
		// �������¼�
		//------------------------------------------------------
		if(_ev_cansend_flag & CanTask_Event_Trigger)
		{
			//-----------------------------------------------------
			// can�������ݷ��ͷ���
			//-----------------------------------------------------
			CanApp_TriggerStream_Server();
		}
	}
}

//================================================================
//Function Name: 
//Description  :�����̳߳�ʼ��
//Input        : 
//Output       : 
//Return       : 
//================================================================
void can_thread_init(void)
{
    rt_err_t result;
	
	Bsp_can_event_init();
	
    result = rt_thread_init(&can_thread, "can_s_tx", can_thread_entry,
        RT_NULL, &can_thread_stack[0],
        sizeof(can_thread_stack), CAN_THREAD_PRI, CAN_THREAD_SLICE);
    if (result == RT_EOK)
    {
        rt_thread_startup(&can_thread);//�����߳�
    }
    else
    {
#ifdef CAN_ERR_PRINT    
        rt_kprintf("can_tx_thread init fail\n");
#endif
    }
	CanStartComm = 1;
}

//================================================================
//Function Name: 
//Description  :can�����߳���ڳ���
//Input        : 
//Output       : 
//Return       : 
//================================================================
static void can1_rx_thread_entry(void* parameter)
{
    rt_err_t result;
    extern struct rt_mailbox mb_can1_rx;
    CanRxMsg * msg;
    while (1)
    {
        result = rt_mb_recv(&mb_can1_rx, (rt_uint32_t *)&msg, CAN_RX_TIMEOUT);
        //��ʱδ�յ�CAN����
        if (result == -RT_ETIMEOUT)
        {
            s_rx_timeout_flag = 1;
#ifdef CAN_ERR_PRINT    
            rt_kprintf("NO CAN1. CAN1 connect error\n");
#endif                    
        }
        //�յ�CAN����,���д���
        else
        {
            s_rx_timeout_flag = 0;
			can_rx_interface(1, msg);
        }
    }
}

//================================================================
//Function Name: 
//Description  :can�����߳���ڳ���
//Input        : 
//Output       : 
//Return       : 
//================================================================
static void can2_rx_thread_entry(void* parameter)
{
    rt_err_t result;
    extern struct rt_mailbox mb_can2_rx;
    CanRxMsg * msg;
    while (1)
    {
        result = rt_mb_recv(&mb_can2_rx, (rt_uint32_t *)&msg, CAN_RX_TIMEOUT);
        //��ʱδ�յ�CAN����
        if (result == -RT_ETIMEOUT)
        {
            s_rx_timeout_flag = 1;
#ifdef CAN_ERR_PRINT    
            rt_kprintf("NO CAN2. CAN2 connect error\n");
#endif                    
        }
        //�յ�CAN����,���д���
        else
        {
            s_rx_timeout_flag = 0;
			can_rx_interface(2, msg);
        }
    }
}

//================================================================
//Function Name: 
//Description  :�����̳߳�ʼ��
//Input        : 
//Output       : 
//Return       : 
//================================================================
void can_rx_thread_init(void)
{
    rt_err_t result;
    result = rt_thread_init(&can1_rx_thread, "can1_s_rx", can1_rx_thread_entry,
        RT_NULL, &can1_rx_thread_stack[0],
        sizeof(can1_rx_thread_stack), CAN_RX_THREAD_PRI, CAN_RX_THREAD_SLICE);
    if (result == RT_EOK)
    {
        rt_thread_startup(&can1_rx_thread);//�����߳�
    }
    else
    {
#ifdef CAN_ERR_PRINT    
        rt_kprintf("can1_rx_thread init fail\n");
#endif
    }

    result = rt_thread_init(&can2_rx_thread, "can2_s_rx", can2_rx_thread_entry,
        RT_NULL, &can2_rx_thread_stack[0],
        sizeof(can2_rx_thread_stack), CAN_RX_THREAD_PRI, CAN_RX_THREAD_SLICE);
    if (result == RT_EOK)
    {
        rt_thread_startup(&can2_rx_thread);//�����߳�
    }
    else
    {
#ifdef CAN_ERR_PRINT    
        rt_kprintf("can2_rx_thread init fail\n");
#endif
    }

}

