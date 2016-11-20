/**
  *************************************************************************
  * This file is part of TDPS04
  * COPYRIGHT (C) 1999-2014, TongYe R&D Team
  * @file    io.h
  * @brief   This file contains all the functions prototypes for the   
  *          io firmware library.
  * Change Logs:
  * Date           Author       Notes
  * 2014/09/11     Coase        the first version
  *************************************************************************
  */

#ifndef __IO__H__
#define __IO__H__

#ifdef __cplusplus
extern "C" {
#endif
    enum digital_output_index
    {
        led1,
        led2,
        led3,
        led4,
    };
    
    enum digital_intput_index
    {
        io_in0 = 0,
        io_in1,
        io_in2,
        io_in3,
        io_in4,
        io_in5,
        io_in6,
        io_in7,
        io_in8,
        io_in9,
        io_in10,
        io_in11
    };
    
    enum hardware_fault_index
    {
        u110_min = io_in11 + 1,
        u24_min,
        ubus_max,
        u24_max,
        u110_max
    };

    void bsp_io_init(void);
    int ** get_input_address(void);
    int ** get_output_address(void);
    
    void stm32_hw_io_on(int led);
    void stm32_hw_io_off(int led);
    void stm32_hw_io_toggle(int led);

#ifdef __cplusplus
}
#endif

#endif

/******************* (C) COPYRIGHT 2014 TongYe **********END OF FILE****/


/*@}*/

