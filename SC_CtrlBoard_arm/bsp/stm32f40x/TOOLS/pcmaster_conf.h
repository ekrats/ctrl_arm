/**
  *************************************************************************
  * This file is part of platform
  * COPYRIGHT (C) 1999-2016, TongYe Inverter Development Team
  * @file    pcmaster_def.h
  * @brief   This file contains all the functions prototypes for the   
  *          pcmaster configuration firmware library.
  * Change Logs:
  * Date           Author       Notes
  * 2016/01/16     Íõ  öÎ        the first version
  *************************************************************************
  */


#ifndef __PCMASTER_CONF__H__
#define __PCMASTER_CONF__H__

#ifdef __cplusplus
extern "C" {
#endif

#define PC_MASTER_REC_BUFF_LEN        0x2000 /* PC Matser Recorder buffer length */
#define PC_MASTER_APPCMD_BUFF_LEN     0x04 /* Application Command buffer length */
#define PC_MASTER_RECORDER_TIME_BASE  0x807D /* A sampling period preset by the board application (value 0x814D = 333us, 0x8, us) */
#define PC_MASTER_GLOB_VERSION_MAJOR  0x05 /* Board firmware version major number */
#define PC_MASTER_GLOB_VERSION_MINOR  0x00 /* Board firmware version minor number */
#define PCMDRV_BUFFER_SIZE            0x80 /* Size of board data buffer */

    typedef const struct
    {
        unsigned int pc_master_rec_buff_len;
        unsigned int pc_master_appcmd_buff_len;
        unsigned int pc_master_recorder_time_base;
        unsigned int pc_master_glob_version_major;
        unsigned int pc_master_glob_version_minor;
        unsigned int pcmdrv_buffer_size;
    }PCMASTER_CONFIGURATION;

    PCMASTER_CONFIGURATION pcmaster_conf =
    {
        PC_MASTER_REC_BUFF_LEN,
        PC_MASTER_APPCMD_BUFF_LEN,
        PC_MASTER_RECORDER_TIME_BASE,
        PC_MASTER_GLOB_VERSION_MAJOR,
        PC_MASTER_GLOB_VERSION_MINOR,
        PCMDRV_BUFFER_SIZE
    };

#ifdef __cplusplus
}
#endif

#endif


/******************* (C) COPYRIGHT 2016 TongYe **********END OF FILE****/




/*@}*/


