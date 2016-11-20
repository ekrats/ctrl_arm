/**
  *************************************************************************
  * This file is part of platform
  * COPYRIGHT (C) 1999-2015, TongYe Inverter Development Team
  * @file    record.h
  * @brief   This file contains all the functions prototypes for the   
  *          record firmware library.
  * Change Logs:
  * Date           Author       Notes
  * 2015/12/29     Íõ  öÎ        the first version
  *************************************************************************
  */


#ifndef __RECORD__H__
#define __RECORD__H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

    
    typedef struct
    {
        int carrier_ratio;
        int record_index;
        int need_record;
        int index_after_fault;
        int * record_buffer;
    }fault_record_t;

    void UserRecord(void);
    void flash_record_init(void);
    void start_record_fault(void);
#ifdef __cplusplus
}
#endif

#endif


/******************* (C) COPYRIGHT 2015 TongYe **********END OF FILE****/




/*@}*/


