/**
  *************************************************************************
  * This file is part of Platform
  * COPYRIGHT (C) 1999-2015, TongYe Corporation All rights reserved.
  * @file    hd_crc32.h
  * @brief   This file contains all the functions prototypes for the   
  *          hardware crc32 firmware library.
  * Change Logs:
  * Date           Author       Notes
  * 2015/01/05     Coase        the first version
  *************************************************************************
  */

#include "stm32f4xx.h"
#include "rtthread.h"
#include "stdint.h"
#ifndef __HD_CRC32__H__
#define __HD_CRC32__H__

#ifdef __cplusplus
extern "C" {
#endif
    /**
     * @brief STM32FxxӲ��CRC���ܳ�ʼ��
     *
     *
     * @return NULL
     */
    void hardware_crc32_init(void);

    /**
     * @brief ��ȡ4�ֽ�CRCУ����
     *
     * @param buffer ��Ҫ����CRCУ��������ݻ���
     * @param size  ���벻С��4,��Ϊ4��������
     *
     * @return 
     */
    u32 get_crc(void* buffer, int size);

    /**
     * @brief ���CRCУ������buffer��ĩ��λ
     *
     * @param buffer ��Ҫ����CRCУ��������ݻ���
     * @param size ������ڵ���8,��Ϊ4��������
     *
     * @return null
     */
    rt_inline void add_crc(void* buffer, int size)
    {
        size -= 4;
        *((u32*)((u8* )buffer + size)) = get_crc(buffer, size);
    }

    /**
     * @brief ���CRCУ������buffer֮��
     *
     * @param buffer ��Ҫ����CRCУ��������ݻ���, 
     *  �������Ԥ��4�ֽڿռ��Է�Խ��
     * @param size   ������ڵ���4,��Ϊ4��������
     *
     * @return null
     */
    rt_inline void add_crc_at_end(void* buffer, int size)
    {
        *((u32*)((u8* )buffer + size)) = get_crc(buffer, size);
    }


    /**
     * @brief У��CRC����
     *
     * @param buffer ��Ҫ����CRCУ��������ݻ���,��ĩ��λΪУ����
     * @param size ������ڵ���8,��Ϊ4��������
     *
     * @return null
     */
    rt_inline rt_bool_t check_crc(void * buffer, int size)
    {
        if(size > 4)
        {
            return ( (*(u32 *)((u8*)buffer + size - 4)) == get_crc(buffer, size - 4) );
        }
        else
        {
            return RT_FALSE;
        }
    }
#ifdef __cplusplus
}
#endif

#endif

/******************* (C) COPYRIGHT 2015 TongYe **********END OF FILE****/


/*@}*/

