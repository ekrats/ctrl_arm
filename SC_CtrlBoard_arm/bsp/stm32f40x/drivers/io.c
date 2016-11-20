/**
  *************************************************************************
  * This file is part of TDPS04
  * COPYRIGHT (C) 1999-2014, TongYe R&D Team
  * @file    io.c
  * @brief   This file provides all the io firmware functions.
  * Change Logs:
  * Date           Author       Notes
  * 2015/05/06     Coase        the first version
  *************************************************************************
  */

#include "stdint.h"
#include "stm32f4xx.h"
#include "io.h"
#include "static_mem.h"

#define DIGITAL_INPUT_LENGTH (4)
#define DIGITAL_OUTPUT_LENGTH (5)
//static int ** input_buffer;
//static int ** output_buffer;
static int * input_buffer[4];
static int * output_buffer[5];

#define IN_PORT (GPIOG)
#define IN_PORT_PIN (GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15)
 
#define OUT_PORT_PIN (GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7)
#define OUT_PORT (GPIOA)

#define OUT_FAN_PIN (GPIO_Pin_0)
#define OUT_FAN (GPIOB)

void io_rcc_config()
{
    //IO_GPIO RCC
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
}

void io_gpio_config()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /* Configure Button pin as input */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    GPIO_InitStructure.GPIO_Pin = IN_PORT_PIN;
    GPIO_Init(IN_PORT, &GPIO_InitStructure);
    
    /* Configure pin as output */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_InitStructure.GPIO_Pin = OUT_PORT_PIN;
    GPIO_Init(OUT_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = OUT_FAN_PIN;
    GPIO_Init(OUT_FAN, &GPIO_InitStructure);
}

void * get_bit_band_addr(void * addr, int bit_index)
{
    u32 source_addr = (u32)addr;
    u32 base_addr = 0;
    u32 base_bit_band_addr = 0;
    if(source_addr < SRAM1_BASE)
    {
        base_addr = CCMDATARAM_BASE;
        base_bit_band_addr = CCMDATARAM_BB_BASE;
    }
    else if(source_addr < SRAM2_BASE)
    {
        base_addr = SRAM1_BASE;
        base_bit_band_addr = SRAM1_BB_BASE;
    }
    else if(source_addr < SRAM3_BASE)
    {
        base_addr = SRAM2_BASE;
        base_bit_band_addr = SRAM2_BB_BASE;
    }
    else if(source_addr < PERIPH_BASE)
    {
        base_addr = SRAM3_BASE;
        base_bit_band_addr = SRAM3_BB_BASE;
    }
    else if(source_addr < BKPSRAM_BASE)
    {
        base_addr = PERIPH_BASE;
        base_bit_band_addr = PERIPH_BB_BASE;
    }
    else
    {
        base_addr = BKPSRAM_BASE;
        base_bit_band_addr = BKPSRAM_BB_BASE;
    }

    return (void *)(base_bit_band_addr + ((source_addr - base_addr) << 5) + (bit_index << 2));
}

void bsp_io_init( void )
{
    io_rcc_config();
    io_gpio_config();
    //input_buffer = (int **) static_malloc(DIGITAL_INPUT_LENGTH * 4);
    //output_buffer = (int **) static_malloc(DIGITAL_OUTPUT_LENGTH * 4);
    
    input_buffer[3] = (int*)get_bit_band_addr((void *)&GPIOG->IDR, GPIO_PinSource12);//Kx
    input_buffer[2] = (int*)get_bit_band_addr((void *)&GPIOG->IDR, GPIO_PinSource13);//Kx
    input_buffer[1] = (int*)get_bit_band_addr((void *)&GPIOG->IDR, GPIO_PinSource14);//Kx
    input_buffer[0] = (int*)get_bit_band_addr((void *)&GPIOG->IDR, GPIO_PinSource15);//Kx
    
    output_buffer[0] = (int*)get_bit_band_addr((void *)&GPIOA->ODR, GPIO_PinSource4);//Led
    output_buffer[1] = (int*)get_bit_band_addr((void *)&GPIOA->ODR, GPIO_PinSource5);//Led
    output_buffer[2] = (int*)get_bit_band_addr((void *)&GPIOA->ODR, GPIO_PinSource6);//Led
	output_buffer[3] = (int*)get_bit_band_addr((void *)&GPIOA->ODR, GPIO_PinSource7);//Led
	
	output_buffer[4] = (int*)get_bit_band_addr((void *)&GPIOB->ODR, GPIO_PinSource0);//Fan
	
    stm32_hw_io_off(0);
    stm32_hw_io_off(1);
    stm32_hw_io_off(2);
    stm32_hw_io_off(3);
	stm32_hw_io_on(4);
}

int ** get_input_address(void)
{
    return input_buffer;
}

int ** get_output_address(void)
{
    return output_buffer;
}

void stm32_hw_io_on(int led)
{
    if(led < DIGITAL_OUTPUT_LENGTH)
    {
        *output_buffer[led] = 0;
    }
}

void stm32_hw_io_off(int led)
{
    if(led < DIGITAL_OUTPUT_LENGTH)
    {
        *output_buffer[led] = 1;
    }
}
void stm32_hw_io_toggle(int led)
{
    if(led < DIGITAL_OUTPUT_LENGTH)
    {
        *output_buffer[led] = !(*output_buffer[led]);
    }
}
/******************* (C) COPYRIGHT 2014 TongYe **********END OF FILE****/


/*@}*/

