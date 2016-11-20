/**
  *************************************************************************
  * This file is part of Platform
  * COPYRIGHT (C) 1999-2015, TongYe R&D Team
  * @file    update_application.h
  * @brief   This file contains all the functions prototypes for the
  *  update_application firmware library.
  * Change Logs:
  * Date           Author       Notes
  * 2015/05/27     Coase        the first version
  *************************************************************************
  */


#ifndef __UPDATE_APP__H__
#define __UPDATE_APP__H__

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
    typedef struct _FILE_INFO_
    {
        uint32_t unix_time;
        uint32_t app_size;
        uint32_t app_crc;
    }APPLICATION_UPDATE_INFO;

    uint32_t set_file_buffer(uint8_t * buffer, int length);
    uint32_t start_update(uint8_t * buffer, int length);
    uint32_t check_updated(void);


#ifdef __cplusplus
}
#endif

#endif


/******************* (C) COPYRIGHT 2015 TongYe **********END OF FILE****/


/*@}*/
