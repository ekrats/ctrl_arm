/**
*************************************************************************
* This file is part of CM80A
* COPYRIGHT (C) 1999-2013, TongYe Inverter Development Team
* @file    serial_comm_util.c
* @brief   This file provides all the serial comm util firmware functions.
* Change Logs:
* Date           Author       Notes
* 2013/10/31     Coase        the first version
* 2014/5         hepj         for cm80a v1.5
*************************************************************************
*/

#include "serial_util.h"
#include "flash_storage_at45dbxx.h"
#include "crc16.h"
#include "string.h"
#include "iap_service.h"
#include "ScManagerExtern.h"
#include "DataStruct.h"

serial_protocol protocol = { 0 };
int cmd_code = 0;
static uint8_t usart_tx_data[TX_BUFFER_SIZE - 1];//4224+1
uint16_t tx_length = 0;
static int file_write_index = 0;

void check_file_call_back(int state_code)
{
    usart_tx_data[0] = state_code;
    tx_length = 0;
    
    tx_length = protocol.get_output_data(&protocol, usart_tx_data, tx_length + 1);
    mem_printf((char*)protocol.tx_buffer, tx_length);
}

u8 fault1_buff[21];
u8 fault2_buff[257];
u8 fault3_buff[257];
u8 fault4_buff[257];
u8 fault5_buff[257];
static void call_back(void * buffer, int length)
{
    ScData * p = (ScData *)GetShareDataPtr();
	uint8_t * data = (uint8_t*)buffer;
    int rx_file_index = 0;
    switch (data[0])
    {
	case 0xD0://调试指令
        usart_tx_data[0] = 0x00;
        tx_length = 0;	//无返回数据（不包括状态字节）
        break;
	//充电指令
	case 0xE1:
		{
			int cmd = data[3] + (data[4] << 8);
			if (cmd == 1)
			{
				p->canAppBuf.dcdcCmd.ord = 1;
			}
			else
			{
				p->canAppBuf.dcdcCmd.ord = 0;
			}
			usart_tx_data[0] = 0x00;
			usart_tx_data[1] = 0xE1;
			tx_length = 1;	//无返回数据（不包括状态字节）
			break;
		}
	//放电指令
	case 0xE2:
		{
			int cmd = data[3] + (data[4] << 8);
			if (cmd == 1)
			{
				p->canAppBuf.dcdcCmd.ord = 2;
			}
			else
			{
				p->canAppBuf.dcdcCmd.ord = 0;
			}
			usart_tx_data[0] = 0x00;
			usart_tx_data[1] = 0xE2;
			tx_length = 1;	//无返回数据（不包括状态字节）
			break;
		}
	//重起指令
	case 0xE3:
		p->canAppBuf.dcdcCmd.ord = 0;
		rt_thread_delay(RT_TICK_PER_SECOND/8);
		NVIC_SystemReset();
		return;
	//状态数据
	case 0xc1:
		{
			int state_size = sizeof(CAN_CB_Status_STYP);
			rt_uint8_t * state_buff;
			rt_memcpy(&state_buff[1], (rt_uint8_t *)&p->cbStatus, state_size);
			length = protocol.get_output_data(&protocol, &state_buff[0], state_size+1);
			mem_printf((const char *)protocol.tx_buffer, length);
		}
		return;
		
	//充电参数
	case 0xc2:
		{
			int par_size = sizeof(CB_Para_STYP);
			rt_uint8_t * par_buff;
			rt_memcpy(&par_buff[1], (rt_uint8_t *)&p->cbPara, par_size);
			length = protocol.get_output_data(&protocol, &par_buff[0], par_size+1);
			mem_printf((const char *)protocol.tx_buffer, length);
		}
		return;
		
	//AD参数
	case 0xc3:
		{
			int ad_size = sizeof(CB_Adjust_STYP);
			rt_uint8_t * ad_buff;
			rt_memcpy(&ad_buff[1], (rt_uint8_t *)&p->cbAdj, ad_size);
			length = protocol.get_output_data(&protocol, &ad_buff[0], ad_size+1);
			mem_printf((const char *)protocol.tx_buffer, length);
		}
		return;
		
	//系统参数设置
	case 0xA0:
		{
			rt_memcpy(&p->cbWrPara, &data[4], sizeof(CB_Para_STYP));
			if (rt_memcmp((const void *)&p->cbWrPara, (const void *)&p->cbPara, sizeof(CB_Para_STYP)))
			{
				p->cbPara = p->cbWrPara;
				p->status.status_bit.wParFlag  = true;
			}
		}
		usart_tx_data[0] = 0x00;
        tx_length = 0;	//无返回数据（不包括状态字节）
		break;
		
	//AD校准参数设置
	case 0xA1:
		{
			rt_memcpy(&p->cbWrAdj, &data[4], sizeof(CB_Adjust_STYP));
			if (rt_memcmp((const void *)&p->cbWrAdj, (const void *)&p->cbAdj, sizeof(CB_Adjust_STYP)))
			{
				p->cbAdj = p->cbWrAdj;
				p->status.status_bit.wAdjFlag  = true;
			}
		}
		usart_tx_data[0] = 0x00;
        tx_length = 0;	//无返回数据（不包括状态字节）
		break;
		
    case 0xcc:
		{
			extern struct rt_semaphore sem_reset_cpu;
            iap_message * msg = (iap_message *)rt_malloc(sizeof(iap_message));
            msg->cmd = IAP_CMD_CHECK_FILE;
            msg->length = 0;
            *(u32*)&msg->data = (u32)check_file_call_back;
            *(u32*)&msg->data[4] = (u32)&sem_reset_cpu;
            file_write_index = 0;
            rt_mb_send((rt_mailbox_t)get_iap_mailbox(), (rt_uint32_t)msg);
		}
		return;

	case 0xd1://读取故障记录的总数
    {
		int map_index = RECORD_FAULT;
		storage_map* map = get_storage_map();
		int num = (map[map_index].end_page + 1 - map[map_index].start_page) / map[map_index].page_per_record;
		if (map[map_index].current_id < num)
		{
		  num = map[map_index].current_id;
		}

		usart_tx_data[1] = num;
		usart_tx_data[2] = num >> 8;
		tx_length = 2;
    }
    break;

	case 0xE0://读取故障记录
	{
		int fault_index = data[1] + (data[2] << 8);
		int rx_index = data[3] + (data[4] << 8);
		if (rx_index == 1)
		{
			uint8_t * read_buf = rt_malloc(2050);
			rt_device_t fs = rt_device_find("fs");
			read_buf[0] = 0;
			if (fs != RT_NULL)
			{
				rt_device_read(fs, (RECORD_FAULT << 16) + fault_index, read_buf, 2048);
			}
			rt_memcpy(&fault1_buff[1], &read_buf[0], 8);
			rt_memcpy(&fault1_buff[9], &read_buf[1032], 12);
			rt_memcpy(&fault2_buff[1], &read_buf[8], 256);
			rt_memcpy(&fault3_buff[1], &read_buf[264], 256);
			rt_memcpy(&fault4_buff[1], &read_buf[520], 256);
			rt_memcpy(&fault5_buff[1], &read_buf[776], 256);
			
			fault1_buff[0] = 0x00;
			length = protocol.get_output_data(&protocol, &fault1_buff[0], 21);
			mem_printf((const char *)protocol.tx_buffer, length);

			rt_free(read_buf);
		}
		else if (rx_index == 2)
		{
			fault2_buff[0] = 0x00;
			length = protocol.get_output_data(&protocol, &fault2_buff[0], 257);
			mem_printf((const char *)protocol.tx_buffer, length);
		}
		else if (rx_index == 3)
		{
			fault3_buff[0] = 0x00;
			length = protocol.get_output_data(&protocol, &fault3_buff[0], 257);
			mem_printf((const char *)protocol.tx_buffer, length);
		}
		else if (rx_index == 4)
		{
			fault4_buff[0] = 0x00;
			length = protocol.get_output_data(&protocol, &fault4_buff[0], 257);
			mem_printf((const char *)protocol.tx_buffer, length);
		}
		else if (rx_index == 5)
		{
			fault5_buff[0] = 0x00;
			length = protocol.get_output_data(&protocol, &fault5_buff[0], 257);
			mem_printf((const char *)protocol.tx_buffer, length);
		}
		
		return;
	}
		
    case 'u':
		{
			iap_message * msg = (iap_message *)rt_malloc(sizeof(iap_message));
            msg->cmd = IAP_CMD_SET_INFO;
            msg->length = length - 2;
            rt_memcpy(msg->data, (uint8_t *)data + 2, length - 2);
            usart_tx_data[0] = 0x00;
            file_write_index = 0;
            tx_length = 0;
            rt_mb_send((rt_mailbox_t)get_iap_mailbox(), (rt_uint32_t)msg);
			break;
		}

    case 'f':
		{
			rx_file_index = (data[2] + (data[3] << 8));
			if (rx_file_index == file_write_index)
			{
				iap_message * msg = (iap_message *)rt_malloc(sizeof(iap_message));
				msg->cmd = IAP_CMD_WRITE_FILE;
				msg->length = length - 4;
				rt_memcpy(msg->data, (uint8_t *)data + 4, length - 4);
				usart_tx_data[0] = 0x00;
				file_write_index++;
				rt_mb_send((rt_mailbox_t)get_iap_mailbox(), (rt_uint32_t)msg);
			}
			else if (rx_file_index < file_write_index)
			{
				usart_tx_data[0] = 0x0;
			}
			else
			{
				usart_tx_data[0] = 0x81;
			}
			tx_length = 0;
			break;
		}

    default://其它情况，不响应，直接返回
        return;
    }
    tx_length = protocol.get_output_data(&protocol, usart_tx_data, tx_length + 1);
    mem_printf((char*)protocol.tx_buffer, tx_length);
}

static void except(serial_protocol_exception exception)
{
    switch (exception)
    {
    case illegal_length:
        usart_tx_data[0] = 0x81;
        break;
    case checksum_failure:
        usart_tx_data[0] = 0x82;
        break;
    default:
        break;
    }
}

void serial_comm_init(void)
{
    init_serial_protocol(&protocol);
    protocol.call_back = call_back;
    protocol.except = except;
}


/******************** cmd request ********************************************/


debug_control_t debug_instruction;


#include "version.h"
#include <stdio.h>
#include <rtdef.h>
static char version_info[192] = { 0 };

char * get_board_info()
{
    sprintf(version_info, "%d.%d.%d.%d build at %s by %s use RTT v%d.%d.%d and HD v%d.%d for %s\r\n",
        MAJOR_VERSION_NUMBER, MINOR_VERSION_NUMBER,
        REVISION_NUMBER, BUILD_NUMBER, BUILD_TIME,
        BUILDER_NAME,
        (int)RT_VERSION, (int)RT_SUBVERSION, (int)RT_REVISION,
        1, 0, PROJECT_NAME);
    return version_info;
}

char * get_short_version()
{
    sprintf(version_info, "%d.%02d", MAJOR_VERSION_NUMBER, REVISION_NUMBER);
    return version_info;
}
#include "stm32f4xx.h"

void reset_device()
{
    //关闭所有中断相应
    __set_FAULTMASK(1);
    //系统复位
    NVIC_SystemReset();

}

void full_version()
{
    rt_kprintf(get_board_info());
}

#include "finsh.h"
FINSH_FUNCTION_EXPORT(full_version, show full version information);
FINSH_FUNCTION_EXPORT(reset_device, reset device);


#define CPU_USAGE_CALC_TICK    10
#define CPU_USAGE_LOOP        100

static rt_uint8_t  cpu_usage_major = 0, cpu_usage_minor= 0;
static rt_uint32_t total_count = 0;

static void cpu_usage_idle_hook()
{
	rt_tick_t tick;
	rt_uint32_t count;
	volatile rt_uint32_t loop;

	if (total_count == 0)
	{
		/* get total count */
		rt_enter_critical();
		tick = rt_tick_get();
		while(rt_tick_get() - tick < CPU_USAGE_CALC_TICK)
		{
			total_count ++;
			loop = 0;
			while (loop < CPU_USAGE_LOOP) loop ++;
		}
		rt_exit_critical();
	}

	count = 0;
	/* get CPU usage */
	tick = rt_tick_get();
	while (rt_tick_get() - tick < CPU_USAGE_CALC_TICK)
	{
		count ++;
		loop  = 0;
		while (loop < CPU_USAGE_LOOP) loop ++;
	}

	/* calculate major and minor */
	if (count < total_count)
	{
		count = total_count - count;
		cpu_usage_major = (count * 100) / total_count;
		cpu_usage_minor = ((count * 100) % total_count) * 100 / total_count;
	}
	else
	{
		total_count = count;

		/* no CPU usage */
		cpu_usage_major = 0;
		cpu_usage_minor = 0;
	}
}

void cpu_usage_get(rt_uint8_t *major, rt_uint8_t *minor)
{
	RT_ASSERT(major != RT_NULL);
	RT_ASSERT(minor != RT_NULL);

	*major = cpu_usage_major;
	*minor = cpu_usage_minor;
}

void cpu_usage_init()
{
	/* set idle thread hook */
	rt_thread_idle_sethook(cpu_usage_idle_hook);
}

static void cpu(void)
{
	static rt_uint8_t cpu_usage_init_flag = 0;

	if(cpu_usage_init_flag == 0)
	{
		cpu_usage_init_flag = 1;
		cpu_usage_init();
		rt_thread_delay(RT_TICK_PER_SECOND);
	}
	rt_kprintf("CPU use perecent : %d.%d%\r\n",cpu_usage_major, cpu_usage_minor );	
}

FINSH_FUNCTION_EXPORT(cpu, show the cpu useage);

#ifdef RT_USING_FINSH
#include "finsh.h"
long erase_at45(void)
{
    rt_device_t at45 = RT_NULL;
    struct ty_at45_erase_set erase_set;
    erase_set.type = AT45_ERASE_CHIP;
    at45 = rt_device_find("flash");
    if(at45 != RT_NULL)
    {
        rt_device_control(at45, RT_DEVICE_CTRL_BLK_ERASE, &erase_set);
    }
    return 0;
}

FINSH_FUNCTION_EXPORT(erase_at45, erase full chip of at45);
#endif

/******************** (C) COPYRIGHT 2013 TongYe ***********END OF FILE****/

