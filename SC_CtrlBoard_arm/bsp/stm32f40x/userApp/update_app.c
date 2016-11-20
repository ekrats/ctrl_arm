/**
  *************************************************************************
  * This file is part of Platform
  * COPYRIGHT (C) 1999-2015, TongYe R&D Team
  * @file    update_application.c
  * @brief   This file provides all the update application firmware 
  * functions.
  * Change Logs:
  * Date           Author       Notes
  * 2015/05/27     Coase        the first version
  *************************************************************************
  */

#include "update_app.h"
#include "hardware_crc32.h"
#include "own_flash.h"

static APPLICATION_UPDATE_INFO new_app_info = { 0 };
static int file_length = 0;
static const uint32_t app_start_address = UPDATE_ADDRESS + APPLICATION_OFFSET;
static const uint32_t app_info_address = UPDATE_ADDRESS;

uint32_t start_update(uint8_t * buffer, int length)
{
    uint8_t * tmp = rt_malloc(length);
    uint32_t rtn = 1;
    RT_ASSERT(tmp != RT_NULL);
    
    rt_memcpy(tmp, buffer, length);
    if (check_crc(tmp, length))
    {
        new_app_info = *(APPLICATION_UPDATE_INFO *)tmp;
        own_flash_erase(app_info_address, APPLICATION_OFFSET + new_app_info.app_size);
        own_flash_write((uint32_t *)app_info_address, (uint32_t *)&new_app_info, sizeof(APPLICATION_UPDATE_INFO) / 4);
        rtn = 0;
    }
    file_length = 0;
    rt_free(tmp);
    return rtn;
}

uint32_t set_file_buffer(uint8_t * buffer, int length)
{
    uint8_t * tmp = rt_malloc(length);
    uint32_t rtn = 1;
    
    RT_ASSERT(tmp != RT_NULL);
    
    rt_memcpy(tmp, buffer, length);
    if (check_crc(tmp, length))
    {
        int data_length = (length - 4);
        if(own_flash_write((uint32_t *)(app_start_address + file_length),  (uint32_t *)tmp, data_length / 4) == 0)
        {
            rtn =  0;
        }
        else
        {
            rtn =  1;
        }
        file_length += data_length;
    }
    rt_free(tmp);
    return rtn;
}

uint32_t check_updated()
{
    if(rt_tick_get() < 2000)
    {
        return RT_EOK;
    }
    if(new_app_info.app_size > 0 && new_app_info.app_size < 0x80000)
    {
        if(own_flash_check((uint32_t * )app_start_address, new_app_info.app_size / 4, new_app_info.app_crc) != 0)
        {
            return 1;
        }
    }
    else
    {
        return RT_ETIMEOUT;
    }
    return RT_EOK;
}

/******************* (C) COPYRIGHT 2015 TongYe **********END OF FILE****/


/*@}*/
