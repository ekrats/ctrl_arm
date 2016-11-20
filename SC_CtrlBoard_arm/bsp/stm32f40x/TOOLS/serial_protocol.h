/**
  *************************************************************************
  * This file is part of CM80A
  * COPYRIGHT (C) 1999-2013, TongYe Inverter Development Team
  * @file    serial_protocol.h
  * @brief   This file contains all the functions prototypes for the serial  
  *          protocol firmware library.
  * Change Logs:
  * Date           Author       Notes
  * 2013/10/30     Coase        the first version
  * 2016/05/19     Coase        the current version
  *************************************************************************
  */

#ifndef __SERIAL_PROTOCOL__H__
#define __SERIAL_PROTOCOL__H__
#include <stdint.h>

typedef enum 
{
    illegal_length,
    checksum_failure,
}serial_protocol_exception;

typedef struct _serial_protocol_ serial_protocol;
typedef void (*fptr_put_buffer)(serial_protocol * protocol, void * buffer, int length);
typedef int (* fptr_get_output_data)(serial_protocol * protocol, void * source_data, int length);
typedef void (*fptr_call_back)(void * buffer, int length);
typedef void (*fptr_exception)(serial_protocol_exception exception);
#define SOM ('*')
#define RX_BUFFER_SIZE 512
#define TX_BUFFER_SIZE 10240
/* void BIT_SET(Mask, Addr); */
#define BIT_SET(Mask, Addr)             ((Addr) |= (Mask)) //((void *) 0) //asm(bfset    Mask,Addr)

/* void BIT_CLEAR(Mask, Addr); */
#define BIT_CLEAR(Mask, Addr)           ((Addr) &= ~(Mask))//((void *) 0)//asm(bfclr    Mask,Addr)


enum protocol_status
{
    is_last_start_of_byte = 1,
    is_message_started = 2,
    is_data_length_getted = 4,
};

struct _serial_protocol_
{
    uint8_t rx_buffer[RX_BUFFER_SIZE];
    uint8_t tx_buffer[TX_BUFFER_SIZE];
    uint32_t checksum;
    int next_position;
    int rx_length;
    uint32_t status; 
    fptr_put_buffer put_buffer;
    fptr_get_output_data get_output_data;
    fptr_call_back call_back;
    fptr_exception except;
};
void init_serial_protocol(serial_protocol * protocol);
#endif

/******************** (C) COPYRIGHT 2013 TongYe ***********END OF FILE****/
