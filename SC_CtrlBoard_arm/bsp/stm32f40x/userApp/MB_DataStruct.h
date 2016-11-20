#ifndef	 __MB_DATASTRUCT_H__
#define	 __MB_DATASTRUCT_H__
//#include "stm32f4xx.h"
#include "stm32f4xx.h"

#define BAT_STATE_IDLE				0x0010
#define BAT_STATE_MANUAL			0x0020
#define BAT_STATE_MANUAL_UP			0x0030
#define BAT_STATE_MANUAL_DOWN		0x0040
#define BAT_STATE_AUTO				0x0050
#define BAT_STATE_FAULT				0x0060

#define BAT_OUT_IDLE				0x0001
#define BAT_OUT_MANUAL				0x0002
#define BAT_OUT_MANUAL_UP			0x0003
#define BAT_OUT_MANUAL_DOWN			0x0004
#define BAT_OUT_AUTO				0x0005
#define BAT_OUT_FAULT				0x0006

#define CHARGE_BASE_T						10//ms

typedef struct{
	u16 cbHwVersionW;
	u16 cbARMVersionW;
	u16 cbFPGAVersionW;
}Version_STYP;



typedef struct{
	u16 res01:  			3;
	u16 fanOnN:  			1;			
	u16 lowLoadN:  			1;			
	u16 igbt1OverTemprN: 	1;	
	u16 igbt2OverTemprN: 	1;	
	u16 capVoltFailN:		1;	
	u16 gridCapVolFailN:	1;	
	u16 flyCapBalanceN:		1;	
	u16 capVoltLowN:		1;	
	u16 res02:				4;
	u16 sysFailN:			1;	
	u16 chargeOnN:			1;	
	u16 floatChargeN:		1;	
	u16 chargeIFailN:		1;	
	u16 chargeQFailN:		1;	
	u16 dischargeOnN:		1;	
	u16 floatDischargeN:	1;	
	u16 dischargeIFailN:	1;	
	u16 dischargeQFailN:	1;	
	u16 currDirectN:		1;	
	u16 swOverCurrN:		1;	
	u16 hwOverCurrN:		1;	
	u16 swGridOverVoltN:	1;
	u16 hwGridOverVoltN:	1;
	u16 swCapOverVoltN:		1;
	u16 hwCapOverVoltN:		1;
	u16 syncFailN:			1;
}CB_Flags_STYP;

typedef struct{
	u8 ord;       //充放电指令，0：待机  1：启动充电  2：启动放电
    u8 res0;
    u16 rateCurr;
    u8 ctrlMode;//1:恒流、恒功率、恒压模式
    u8 res3;
    u8 res4;
    u8 res5;
}Charge_Cmd_STYP, *PCharge_Cmd_STYP;

typedef struct{
	u8 res01[2];
	CB_Flags_STYP flags;
	s16 currW;
	u16 flyingCapvoltW;
	u8 igbt1Tempr;						
	u8 igbt2Tempr;
	u16 gridCapvoltW;
	u16 capvoltW;
	u8 res02[8];
}CAN_CB_Status_STYP, *PCAN_CB_Status_STYP;

typedef struct{
	u16 chargeVoltW;
	u16 chargeCurrW;
	u16 chargeVoltOverW;
	u16 chargeCurrOverW;
	u16 dischargeVoltW;
	u16 dischargeCurrW;
	u16 dischargeVoltOverW;		
	u16 dischargeCurrOverW;		
	u16 flyingCapVoltOverW;
	u8 IGBT1TemprOver;
	u8 IGBT2TemprOver;
	u8 res[4];
}CB_Para_STYP, *PCB_Para_STYP;

typedef struct{
	u16 a;
	s16 b;
}Adjust_STYP;

typedef struct{
	Adjust_STYP	u_in;
	Adjust_STYP u_cfly;
	Adjust_STYP u_out;
	Adjust_STYP i_out;
	Adjust_STYP temp_igbt1;
	Adjust_STYP temp_igbt2;
}CB_Adjust_STYP;

#endif


