/**
  *************************************************************************
  * This file is part of platform
  * COPYRIGHT (C) 1999-2016, TongYe Inverter Development Team
  * @file    iap_service.h
  * @brief   This file contains all the functions prototypes for the   
  *          iap_service firmware library.
  * Change Logs:
  * Date           Author       Notes
  * 2016/06/21     Íõ  öÎ        the first version
  *************************************************************************
  */


#ifndef __IAP_SERVICE__H__
#define __IAP_SERVICE__H__

#ifdef __cplusplus
extern "C" {
#endif
typedef enum
{
    IAP_NULL,
    IAP_CMD_CHECK_FILE = 3,
    IAP_CMD_SET_INFO,
    IAP_CMD_WRITE_FILE
}iap_service_cmd;
    
typedef struct _iap_message
{
    int length;
    int cmd;
    char data[256];
}iap_message;

void iap_service_init(void);
void * get_iap_mailbox(void);

#ifdef __cplusplus
}
#endif

#endif


/******************* (C) COPYRIGHT 2016 TongYe **********END OF FILE****/




/*@}*/


