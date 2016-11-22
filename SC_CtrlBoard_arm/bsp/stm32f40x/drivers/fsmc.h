/********************************************************************
	created:	2011/08/17
	created:	17:8:2011   9:04
	filename: 	f:\work\STM32-CM80A-Slave\startup\fsmc.h
	file path:	f:\work\STM32-CM80A-Slave\startup
	file base:	fsmc
	file ext:	h
	author:		Coase Dahl
	
	purpose:	
*********************************************************************/
#ifndef PROJECT_CM80A_SLAVE_FSMC_H
#define PROJECT_CM80A_SLAVE_FSMC_H

#ifdef __cplusplus
extern "C" {
#endif

#define FPGA_ADDRESS   (0x60000000)

void fsmc_fpga_init(void);
void update_fpga_data(void * buffer);

#ifdef __cplusplus
}
#endif
	
#endif
//////////////////////////////////////////////////////////////////////////
