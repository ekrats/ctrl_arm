/**
  *************************************************************************
  * This file is part of platform
  * COPYRIGHT (C) 1999-2016, TongYe Inverter Development Team
  * @file    iap_service.c
  * @brief   This file provides all the iap_service firmware functions.
  * Change Logs:
  *************************************************************************
  */


#include <rtthread.h>
#include "iap_service.h"
#include "static_mem.h"
#include "update_app.h"

static struct rt_mailbox iap_mb;
char iap_svr_mb_pool[16];

void * get_iap_mailbox(void)
{
    return &iap_mb;
}

static void iap_service_thread_entry(void* parameter)
{
    iap_message *msg;
    while (1)
    {
        rt_mb_recv(&iap_mb, (rt_uint32_t *)&msg, RT_WAITING_FOREVER);
        switch(msg->cmd)
        {
            case IAP_CMD_CHECK_FILE:
                if (check_updated() == RT_EOK)
                {
                    ((void(*)(int))(*(uint32_t *)msg->data))(0x0);
                    rt_sem_release((rt_sem_t) (*(uint32_t *)&msg->data[4]));
                }
                else
                {
                    ((void(*)(int))(*(uint32_t *)msg->data))(0x81);
                }
                break;
            case IAP_CMD_SET_INFO:
                start_update((uint8_t *)msg->data, msg->length);
                break;
            case IAP_CMD_WRITE_FILE:
                rt_enter_critical();
                set_file_buffer((uint8_t *)msg->data, msg->length);
                rt_exit_critical();
                break;
            default:
                break;
        }
        rt_free(msg);
    }
}

static struct rt_thread iap_service_thread;

void iap_service_init(void)
{
    rt_err_t result;
    rt_mb_init(&iap_mb, "iapsvr", static_malloc(0x80), 
                0x80 / 4, RT_IPC_FLAG_FIFO);
    
    result = rt_thread_init(&iap_service_thread, "iap_svr", iap_service_thread_entry,
        RT_NULL, 
        static_malloc(0x800),
        0x800,
        1,
        10);
    if (result == RT_EOK)
    {
        rt_thread_startup(&iap_service_thread);//Æô¶¯Ïß³Ì
    }
}
/******************* (C) COPYRIGHT 2016 TongYe **********END OF FILE****/




/*@}*/
