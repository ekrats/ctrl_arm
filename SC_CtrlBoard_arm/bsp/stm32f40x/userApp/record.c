/**
  *************************************************************************
  * This file is part of platform
  * COPYRIGHT (C) 1999-2015, TongYe Inverter Development Team
  * @file    record.c
  * @brief   This file provides all the record firmware functions.
  * Change Logs:
  * Date           Author       Notes
  * 2015/12/29     renliang        the first version
  *************************************************************************
  */


#include "stdint.h"
#include "ad.h"
#include "record.h"
#include "rtthread.h"
#include "ScManagerExtern.h"
#include "flash_storage_at45dbxx.h"
#include "DataStruct.h"
#include "crc16.h"

#define CARRIER_RATIO 64
#define RECORD_LENGTH_AFTER_FAULT 16
#define CHANNEL_RATIO 8

extern Config_STYP config;

typedef struct 
{
    int16_t ad_origine[CHANNEL_RATIO][CARRIER_RATIO];
    sc_warn_t	warn;
	sc_fault_t fault;
}sc_fault_info;

int * ad_data_buf = RT_NULL;
fault_record_t * sc_record = RT_NULL;
sc_fault_info * sc_fault = RT_NULL;

void flash_record_init(void)
{
    int bch_record_size = sizeof(fault_record_t) + sizeof(sc_fault_info);
    sc_record = (fault_record_t *)rt_malloc(bch_record_size);
    if (sc_record != RT_NULL)
    {
        rt_memset(sc_record, 0, bch_record_size);
        sc_record->carrier_ratio = CARRIER_RATIO;
        sc_fault = (sc_fault_info *)&sc_record->record_buffer;
    }
    ad_data_buf = get_ad_data_buffer();
}

void push_ad_record(void)
{
    int index = sc_record->record_index;
    if (sc_fault == RT_NULL)
    {
        return;
    }
    
    sc_fault->ad_origine[0][index] = 1;//(int16_t)ad_data_buf[0];
    sc_fault->ad_origine[1][index] = 2;//(int16_t)ad_data_buf[1];
    sc_fault->ad_origine[2][index] = 3;//(int16_t)ad_data_buf[2];
    sc_fault->ad_origine[3][index] = 4;//(int16_t)ad_data_buf[3];
    sc_fault->ad_origine[4][index] = 5;//(int16_t)ad_data_buf[4];
    sc_fault->ad_origine[5][index] = 6;//(int16_t)ad_data_buf[5];
    sc_fault->ad_origine[6][index] = 7;//(int16_t)ad_data_buf[6];
    sc_fault->ad_origine[7][index] = 8;//(int16_t)ad_data_buf[7];

    ++sc_record->record_index;
    if (sc_record->record_index >= sc_record->carrier_ratio)
    {
        sc_record->record_index = 0;
    }

    if (sc_record->need_record)
    {
        ++sc_record->index_after_fault;
        if (RECORD_LENGTH_AFTER_FAULT == sc_record->index_after_fault)
        {
            int bch_record_size = sizeof(sc_fault_info);
            rt_device_t fs = rt_device_find("fs");
            rt_uint8_t *tmp = (rt_uint8_t *)rt_malloc(bch_record_size);
            if (tmp != RT_NULL)
            {
                int i = 0;
                uint16_t * p = (uint16_t *)sc_fault;
                uint16_t * p1 = (uint16_t *)tmp;
                for (; i < CHANNEL_RATIO; i++)
                {
                    int pos = sc_record->carrier_ratio - sc_record->record_index;
                    rt_memcpy(p1, p + sc_record->record_index, pos * 2);
                    rt_memcpy(p1 + pos, p, sc_record->record_index * 2);
                    p1 += sc_record->carrier_ratio;
                    p += sc_record->carrier_ratio;
                }
                {
                    int address_offset = (uint32_t)&sc_fault->warn - (uint32_t)sc_fault;
                    int comm_data_length = sizeof(sc_warn_t) + sizeof(sc_fault_t);
                    rt_memcpy(tmp + address_offset, &sc_fault->warn, comm_data_length);
                }
                rt_device_write(fs, RECORD_FAULT, tmp, bch_record_size);
            }

            sc_record->need_record = 0;
            sc_record->index_after_fault = 0;
        }
    }
}

void start_record_fault(void)
{
    ScData * p = GetShareDataPtr();

	if (sc_record != RT_NULL)
    {
        sc_record->need_record = 1;
        rt_memcpy(&sc_fault->warn, &p->output.warn, sizeof(sc_warn_t));
		rt_memcpy(&sc_fault->fault, &p->output.fault, sizeof(sc_fault_t));
    }
}

void start_read_fault(void)
{
	uint8_t * read_buf = rt_malloc(2050);
	rt_device_t fs = rt_device_find("fs");
	read_buf[7] = 0;
	if (fs != RT_NULL)
	{
		rt_device_read(fs, RECORD_FAULT << 16, &read_buf[0], 2048);
	}
	rt_free(read_buf);
}

void sys_adj_write(CB_Adjust_STYP *adj)
{
	int bch_record_size = sizeof(CB_Adjust_STYP);
	u16 valW;
	
	rt_device_t fs = rt_device_find("fs");
	rt_uint8_t *tmp = (rt_uint8_t *)rt_malloc(bch_record_size+2);
	
	rt_memcpy(tmp, adj, bch_record_size);
	valW = CRC16_Dn_Cal(tmp, bch_record_size, 0xffff);
	rt_memcpy(tmp+bch_record_size, &valW, 2);
	rt_device_write(fs, RECORD_ADJ, tmp, bch_record_size+2);
}

void sys_adj_read(void)
{
	ScData * p = (ScData *)GetShareDataPtr();
	rt_uint8_t * read_buf = rt_malloc(512);
	int bch_record_size = sizeof(CB_Adjust_STYP);
	Adjust_STYP *adTemp;
	bool adjFalt = false;
	int i;
	
	rt_device_t fs = rt_device_find("fs");
	rt_uint8_t *tmp = (rt_uint8_t *)rt_malloc(bch_record_size);
	
	rt_device_read(fs, (RECORD_ADJ << 16) ,&read_buf[0], 512);
	
	rt_memcpy(tmp, &read_buf[8], bch_record_size+2);
	
	if (CRC16_Dn_Cal(tmp, bch_record_size+2, 0xFFFF)==0x0000)
	{
		for (i=0; i<6; i++)
		{
			adTemp = (Adjust_STYP*)tmp + i;
			if ((adTemp->a < 95) || (adTemp->a > 105)
			|| (adTemp->b < -20) || (adTemp->b > 20))
			{
				adjFalt = true;
				break;
			}
		}
	}
	else
	{
		adjFalt = true;
	}
	
	if (adjFalt)
	{
		rt_memcpy(&p->cbAdj, tmp, bch_record_size);
		p->cbWrAdj =  p->cbAdj;
	}
	else
	{
		p->cbAdj = config.cbAdj;
		p->cbWrAdj = config.cbAdj;
	}
	rt_free(tmp);
	rt_free(read_buf);
}

void sys_par_write(CB_Para_STYP *par)
{
	int bch_record_size = sizeof(CB_Para_STYP);
	u16 valW;
	
	rt_device_t fs = rt_device_find("fs");
	rt_uint8_t *tmp = (rt_uint8_t *)rt_malloc(bch_record_size+2);
	rt_memcpy(tmp, par, bch_record_size);
	valW = CRC16_Dn_Cal(tmp, bch_record_size, 0xffff);
	rt_memcpy(tmp+bch_record_size, &valW, 2);
	rt_device_write(fs, RECORD_PAR, tmp, bch_record_size+2);
}

void sys_par_read(void)
{
	ScData * p = (ScData *)GetShareDataPtr();
	int bch_record_size = sizeof(CB_Para_STYP);
	rt_uint8_t * read_buf = (rt_uint8_t *)rt_malloc(512);
	
	rt_device_t fs = rt_device_find("fs");
	rt_uint8_t *tmp = (rt_uint8_t *)rt_malloc(bch_record_size+2);
	
	rt_device_read(fs, (RECORD_PAR << 16) ,&read_buf[0], 512);
	
	rt_memcpy(tmp, &read_buf[8], bch_record_size+2);
	
	if (CRC16_Dn_Cal(tmp, bch_record_size+2, 0xFFFF)==0x0000)
	{
		rt_memcpy(&p->cbPara, tmp, bch_record_size);
		p->cbWrPara = p->cbPara;
	}
	else
	{
		p->cbWrPara = config.cbPara;
		p->cbPara = config.cbPara;
	}
	
	rt_free(tmp);
	rt_free(read_buf);
	
}

void UserRecord(void)
{
	ScData * p = (ScData *)GetShareDataPtr();
	
	if (p->status.status_bit.wParFlag)
	{
		p->status.status_bit.wParFlag = false;
		sys_par_write(&p->cbWrPara);
	}
	else if (p->status.status_bit.rParFlag)
	{
		p->status.status_bit.rParFlag = false;
		sys_par_read();
	}
	else if (p->status.status_bit.wAdjFlag)
	{
		p->status.status_bit.wAdjFlag = false;
		sys_adj_write(&p->cbWrAdj);
	}
	else if (p->status.status_bit.rAdjFlag)
	{
		p->status.status_bit.rAdjFlag = false;
		sys_adj_read();
	}
	else if (p->status.status_bit.wFaultFlag)
	{
		p->status.status_bit.wFaultFlag = false;
		start_record_fault();
	}
	else if (p->status.status_bit.rFaultFlag)
	{
		p->status.status_bit.rFaultFlag = false;
		start_read_fault();
	}
	
	push_ad_record();
		
}

/******************* (C) COPYRIGHT 2015 TongYe **********END OF FILE****/

/*@}*/
