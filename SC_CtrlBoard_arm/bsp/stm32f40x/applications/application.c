/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 * 2014-04-27     Bernard      make code cleanup. 
 */

#include <board.h>
#include <rtthread.h>
#include <rtdevice.h>
#include "thread_x_run.h"
#include "hardware_crc32.h"
#include "flash_storage_at45dbxx.h"
#include "static_mem.h"
#include "can.h"
#include "spi_flash_at45dbxx.h"
#include "iap_service.h"
#include <string.h>
#include "serial_util.h"
#include "ScManagerExtern.h"
#include "own_flash.h"
#include "pcmaster.h"

#ifdef RT_USING_LWIP
#include <lwip/sys.h>
#include <lwip/api.h>
#include <netif/ethernetif.h>
#include "stm32f4xx_eth.h"
#endif

#ifdef RT_USING_GDB
#include <gdb_stub.h>
#endif

void console_services_init(void);
struct rt_semaphore sem_reset_cpu;

void rt_init_thread_entry(void* parameter)
{
    rt_sem_init(&sem_reset_cpu, "reset", 0, 0);
    /* GDB STUB */
#ifdef RT_USING_GDB
    gdb_set_device("uart6");
    gdb_start();
#endif

    /* LwIP Initialization */
#ifdef RT_USING_LWIP
    {
        extern void lwip_sys_init(void);

        /* register ethernetif device */
        eth_system_device_init();

        rt_hw_stm32_eth_init();

        /* init lwip system */
        lwip_sys_init();
        rt_kprintf("TCP/IP initialized!\n");
    }
#endif

    console_services_init();
    serial_comm_init();
    hardware_crc32_init();
    at45dbxx_init("flash", "at45");
    storage_distribute(32, 1, RECORD_CATALOG);
    storage_distribute(32, 1, RECORD_ADJ);
    storage_distribute(960, 1, RECORD_PAR);
    storage_distribute(3072, 8, RECORD_FAULT);
    flash_storage_init("fs", "flash");
    iap_service_init();
    
    rt_device_init_all();
	
    rt_thread_delay(200);
	
    //GUI
    if(rt_sem_take(&sem_reset_cpu, RT_WAITING_FOREVER) == RT_EOK)
    {
        if(strcmp((char *)(UPDATE_ADDRESS + APPLICATION_OFFSET) + 0x188, "DCDC") == 0)
        {
            rt_thread_delay(1000);
            //关闭所有中断相应
            __set_FAULTMASK(1);
            //系统复位
            NVIC_SystemReset();
        }
        else
        {
            //抹去
            own_flash_erase(UPDATE_ADDRESS, 0x100);
        }
    }
    //unlock_bootloader_protection();
}

static void init_thread_creat(uint8_t priority)
{
	rt_thread_t tid;
	
	tid = rt_thread_create("init",
        rt_init_thread_entry, RT_NULL,
        2048, priority, 20);

    if (tid != RT_NULL)
        rt_thread_startup(tid);
}

int rt_application_init()
{
	init_thread_creat(10);
	//所有线程入口
	rt_app_thread_x();
    
    return 0;
}

/*@}*/
