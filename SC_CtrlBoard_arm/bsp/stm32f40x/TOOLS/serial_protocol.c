/**
  *************************************************************************
  * This file is part of CM80A
  * COPYRIGHT (C) 1999-2013, TongYe Inverter Development Team
  * @file    serial_protocol.c
  * @brief   This file provides all the serial protocol firmware functions.
  * Change Logs:
  * Date           Author       Notes
  * 2013/10/30     Coase        the first version
  *************************************************************************
  */
#include "serial_protocol.h"

static void start_new_message(serial_protocol * protocol, uint8_t c)
{
    BIT_SET(is_message_started, protocol->status);
    protocol->checksum = c;
    protocol->rx_buffer[0] = c;
    protocol->next_position = 1;
    protocol->rx_length = 2;
    if (c >= 0xc0)
    {
        protocol->rx_length = (int)(((c & 0x30) >> 3) + 1);
        BIT_SET(is_data_length_getted, protocol->status);
    }
    else
    {
        BIT_CLEAR(is_data_length_getted, protocol->status);
    }
}

static void get_new_byte(serial_protocol * protocol, uint8_t c)
{
    if (protocol->status & is_message_started)
    {
        if (protocol->next_position != protocol->rx_length)
        {
            protocol->rx_buffer[protocol->next_position++] = c;
            protocol->checksum += c;
            if (!(protocol->status & is_data_length_getted))
            {
                protocol->rx_length = (int)(c + 2);
                if (protocol->rx_length > RX_BUFFER_SIZE)
                {
                    BIT_CLEAR(is_message_started | is_last_start_of_byte, protocol->status);
                    if (protocol->except != 0)
                    {
                        protocol->except(illegal_length);
                    }
                }
                BIT_SET(is_data_length_getted, protocol->status);
            }
        }
        else
        {
            protocol->checksum += c;
            if ((protocol->checksum & 0xff) == 0)
            {
                if (protocol->call_back != 0)
                {
                    protocol->call_back(protocol->rx_buffer, protocol->rx_length);
                }
            }
            else
            {
                if (protocol->except != 0)
                {
                    protocol->except(checksum_failure);
                }
            }
            BIT_CLEAR(is_message_started | is_last_start_of_byte, protocol->status);
        }
    }
}

static void put_char(serial_protocol * protocol, uint8_t c)
{
    if (!(protocol->status & is_last_start_of_byte))
    {
        if (c == SOM)
        {
            BIT_SET(is_last_start_of_byte, protocol->status);
        }
        else
        {
            get_new_byte(protocol, c);
        }
    }
    else
    {
        if (c == SOM)
        {
            get_new_byte(protocol, c);
        }
        else
        {
            start_new_message(protocol, c);
        }
        BIT_CLEAR(is_last_start_of_byte, protocol->status);
    }
}

static void put_buffer(serial_protocol * protocol, void * buffer, int length)
{
    uint8_t * source = (uint8_t *) buffer;
    while (length--)
    {
        put_char(protocol, *source++);
    }
}

static int get_output_data(serial_protocol * protocol, void * source_data, int length)
{
    int rtn = 1;
    uint8_t * source = (uint8_t *)source_data;
    int check_sum = 0;
    while (length--)
    {
        protocol->tx_buffer[rtn++] = *source;
        if (*source == SOM)
        {
            protocol->tx_buffer[rtn++] = *source;
        }
        check_sum += *source;
        source++;
    }
    check_sum = (-check_sum) & 0x00ff;
    protocol->tx_buffer[rtn++] = check_sum;
    if(check_sum == SOM)
    {
        protocol->tx_buffer[rtn++] = SOM;
    }
    return rtn;
}

void init_serial_protocol( serial_protocol * protocol )
{
    protocol->put_buffer = put_buffer;
    protocol->get_output_data = get_output_data;
    protocol->status = 0;
    protocol->next_position = 0;
    protocol->tx_buffer[0] = SOM;
}

/******************** (C) COPYRIGHT 2013 TongYe ***********END OF FILE****/
