/********************************************************************
	created:	2011/08/16
	created:	16:8:2011   16:17
	filename: 	fsmc.c
	author:		Coase Dahl
	2014 -3 - 20  hepj changed for stm32f407
	purpose:
	*********************************************************************/
#include "stm32f4xx_rcc.h"
#include "fsmc.h"

static void fsmc_gpio_configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// Enable the GPIO Clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | 
		                   RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG, ENABLE);

	//common gpio configuration
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	/*  Data lines configuration */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0
		| GPIO_Pin_1
		| GPIO_Pin_8
		| GPIO_Pin_9
		| GPIO_Pin_10
		| GPIO_Pin_14
		| GPIO_Pin_15;	
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource15, GPIO_AF_FSMC);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7
		| GPIO_Pin_8
		| GPIO_Pin_9
		| GPIO_Pin_10
		| GPIO_Pin_11
		| GPIO_Pin_12
		| GPIO_Pin_13
		| GPIO_Pin_14
		| GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/* Address lines configuration */
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource0, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource1, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource2, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource3, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource4, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource5, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource12, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource13, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource14, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource15, GPIO_AF_FSMC);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0
		| GPIO_Pin_1
		| GPIO_Pin_2
		| GPIO_Pin_3
		| GPIO_Pin_4
		| GPIO_Pin_5
		| GPIO_Pin_12
		| GPIO_Pin_13
		| GPIO_Pin_14
		| GPIO_Pin_15;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOG, GPIO_PinSource0, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource1, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource2, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource3, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource4, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource5, GPIO_AF_FSMC);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0
		| GPIO_Pin_1
		| GPIO_Pin_2
		| GPIO_Pin_3
		| GPIO_Pin_4
		| GPIO_Pin_5;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_FSMC);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11
		| GPIO_Pin_12
		| GPIO_Pin_13;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOE, GPIO_PinSource2, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource3, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource4, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_FSMC);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2
		| GPIO_Pin_3
		| GPIO_Pin_4
		| GPIO_Pin_5
		| GPIO_Pin_6;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* NOE and NWE configuration */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* NE1 configuration 悬空*/
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_FSMC);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* NWAIT configuration */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_FSMC);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//GPIO_Mode_IPU;//原上拉输入
	GPIO_Init(GPIOD, &GPIO_InitStructure);	
}

static void FSMC_FPGA_Init(void)
{
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure = {0};
    FSMC_NORSRAMTimingInitTypeDef  p  = {0};
    
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
	//-----------------------------------------------------------------------------
	// FPGA
	//-----------------------------------------------------------------------------
    p.FSMC_AddressSetupTime      = 0x02;//0x0A;
    p.FSMC_AddressHoldTime       = 0x08;//0x0A;
    p.FSMC_DataSetupTime         = 0x0F;//0x08;
    p.FSMC_BusTurnAroundDuration = 0x0F;//0x08;
    p.FSMC_CLKDivision           = 0x0F;
    p.FSMC_DataLatency           = 0;
    p.FSMC_AccessMode            = FSMC_AccessMode_A;
    
    FSMC_NORSRAMInitStructure.FSMC_Bank               = FSMC_Bank1_NORSRAM1;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux     = FSMC_DataAddressMux_Disable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType         = FSMC_MemoryType_NOR;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth    = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode    = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait   = FSMC_AsynchronousWait_Disable;  
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode           = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive   = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation     = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal         = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode       = FSMC_ExtendedMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst         = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct     = &p;
    
    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}

unsigned short * fpga_version;
unsigned short * fpga_control;
unsigned short * fpga_feedback;
unsigned short * fpga_freq;
unsigned short * fpga_max_duty;
unsigned short * fpga_duty;
unsigned short * fan_duty;
unsigned short * fpga_duty2;
unsigned short * fpga_phase;

void fsmc_fpga_init(void)
{
	fsmc_gpio_configuration();
	FSMC_FPGA_Init();
	
    fpga_version =  (u16 *) (FPGA_ADDRESS + 1 * 2);
    fpga_control =  (u16 *) (FPGA_ADDRESS + 2 * 2);
    fpga_feedback = (u16 *) (FPGA_ADDRESS + 3 * 2);
    fpga_freq =     (u16 *) (FPGA_ADDRESS + 4 * 2);
    fpga_max_duty = (u16 *) (FPGA_ADDRESS + 5 * 2);
    fpga_duty =     (u16 *) (FPGA_ADDRESS + 6 * 2);
    fpga_duty2 =    (u16 *) (FPGA_ADDRESS + 0xD * 2);
    fpga_phase =    (u16 *) (FPGA_ADDRESS + 0xE * 2);
	fan_duty =     (u16 *) (FPGA_ADDRESS + 0xF * 2);
}

void update_fpga_data(void * buffer)
{
    u16 * tmp = (u16 *)buffer;
    u16 * tmp1 = (u16 *)fpga_version;
    if(tmp[3] >= 18000)
    {
        *fpga_freq = tmp[3];
        *fpga_max_duty = tmp[3] * 9 / 10;
    }
    
    tmp[0] = *fpga_version;
    tmp[2] = *fpga_feedback;
    //tmp[3] = *fpga_freq;
    //tmp[4] = *fpga_max_duty;
    tmp[6] = tmp1[6];
    tmp[7] = tmp1[7];
    *fpga_control = tmp[1];
    *fpga_duty = tmp[5];
    *fpga_duty2 = tmp[12];
    *fpga_phase = tmp[13];
	*fan_duty = tmp[14];
}

//////////////////////////////////////////////////////////////////////////
