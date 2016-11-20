/**
  *************************************************************************
  * This file is part of Platform
  * COPYRIGHT (C) 1999-2014, TongYe R&D Team
  * @file    rs485.h
  * @brief   This file contains all the functions prototypes for the   
  *          rs485 firmware library.
  * Change Logs:
  * Date           Author       Notes
  * 2014/09/14     Compiler        the first version
  *************************************************************************
  */

#ifndef __RS485_NET__H__
#define __RS485_NET__H__

#ifdef __cplusplus
extern "C" {
#endif

    void rs485_init(void);
    void rs485_irq(void);
    void rs485_entry( void );
    void timer5_isr(void);
    void set_485_tx_address(unsigned char * address);
    void set_485_rx_address( unsigned char * address );
#ifdef __cplusplus
}
#endif

#endif

/******************* (C) COPYRIGHT 2014 TongYe **********END OF FILE****/


/*@}*/

