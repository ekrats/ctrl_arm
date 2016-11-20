/**
  *************************************************************************
  * This file is part of GBC01
  * COPYRIGHT (C) 1999-2013, TongYe Inverter Development Team
  * @file    ad.h
  * @brief   This file contains all the functions prototypes for the   
  *          ad firmware library.
  * Change Logs:
  * Date           Author       Notes
  * 2016/05/11     Coase        the first version
  *************************************************************************
  */

#ifndef __GBC01_AD__H__
#define __GBC01_AD__H__

#include "stm32f4xx_conf.h"

#ifdef __cplusplus
extern "C" {
#endif


enum ad_data_buffer_index
{
    u_in_index,
    u_cfly_index,
    u_out_index,
    i_out_index,
    i_out_peak_index,
    temp1_index,
    temp2_index,
};

void adc_init(void);
int * get_ad_data_buffer(void);
void set_target_current(int current_ref);
void start_injected_ad(void);
void adc_update(void);
void adc_i_update(void);


#ifdef __cplusplus
}
#endif

#endif

/******************* (C) COPYRIGHT 2013 TongYe **********END OF FILE****/


/*@}*/

