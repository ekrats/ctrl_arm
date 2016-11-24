#ifndef _DataStruct_h_
#define _DataStruct_h_

#include "stdint.h"
#include "stm32f4xx.h"

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

typedef struct{
	int16_t u_in;
	int16_t u_cfly;
	int16_t u_out;
	int16_t i_out;
	int16_t i_peak;
	int16_t temp_igpt1;
	int16_t temp_igpt2;
}ad_data_t;

typedef struct 
{
    uint16_t version;
    uint16_t ctrl_res       : 12;
    uint16_t master_node    : 1;
    uint16_t discharge      : 1;
    uint16_t unlock         : 1;
    uint16_t enable         : 1;
        
    uint16_t i_max          : 1;
    uint16_t u_fly_max      : 1;
    uint16_t u_out_max      : 1;
    uint16_t duty_max       : 1;
    uint16_t sync_warning   : 1;
    uint16_t fb_res         : 11;
    
    uint16_t freq;
    uint16_t max_duty;
    uint16_t duty1;
    uint16_t duty_q1_fb;
    uint16_t duty_q2_fb;
    uint16_t time_base;
    int16_t  res1;
    int16_t  res2;
    int16_t  res3;
    int16_t  duty2;
    int16_t  phase;
	int16_t  fan_duty;
}fpga_data;

typedef union _sc_warn_t
{
    struct 
    {
        uint32_t can_lost		  		:1;
        uint32_t m1_temp_fail  			:1;
        uint32_t m2_temp_fail     		:1;
        uint32_t cap1_temp_fail   		:1;
		
        uint32_t cap2_temp_fail   		:1;
        uint32_t main_con1_fb  			:1;
        uint32_t main_con2_fb			:1;
        uint32_t pre_con_fb	    		:1;
        
        uint32_t out1_con_fb			:1;
        uint32_t out2_con_fb	     	:1;
		uint32_t out3_con_fb	     	:1;	
        uint32_t ac1_volt_fail	    	:1;
		
        uint32_t bus_volt_fail	    	:1;
        uint32_t m1_curr_fail      		:1;
        uint32_t m2_curr_fail  			:1;
        uint32_t ac1_volt_under  		:1;
		
        uint32_t dcdc1_on_fail          :1;
        uint32_t dcdc2_on_fail			:1;
		uint32_t dcdc3_on_fail			:1;
		uint32_t dcdc4_on_fail			:1;
		uint32_t res2					:12;
    }warn_bit;
    struct
    {
        uint16_t l_low;
        uint16_t l_high;
    }warn_u16;
    uint32_t warn_u32;
}sc_warn_t;

typedef union _sc_fault_t
{
    struct 
    {
        uint32_t u_in_max_slow	  :1;
		uint32_t u_cfly_max_slow  :1;
		uint32_t i_bat_max_slow   :1;
        uint32_t i_bat_min_slow   :1;
		
        uint32_t u_bat_max_slow   :1;
		uint32_t u_in_bat_slow    :1;
        uint32_t igbt1_temp_over  :1;
        uint32_t igbt2_temp_over  :1;
		
        uint32_t i_bat_max_hw	  :1;
		uint32_t u_fly_max_hw     :1; 
        uint32_t u_bat_max_hw	  :1;
        uint32_t u_in_max_fast    :1;
		
        uint32_t i_bat_max_fast   :1;
        uint32_t i_bat_min_fast   :1;
        uint32_t u_bat_max_fast   :1;
		uint32_t u_in_bat_fast	  :1;
		
		uint32_t res		  	  :16;
    }fault_bit;
	struct
    {
        uint16_t l_low;
        uint16_t l_high;
    }fault_u16;
	uint32_t fault_u32;
}sc_fault_t;


//---------------------------------------------------------------------------------- 
//-----bch status struct(union)
typedef union _sc_status_t
{
    struct _status_bit
    {
        uint32_t		wFault				: 1 ; //警告故障
		uint32_t		sFault				: 1 ; //严重故障
		uint32_t		dcdcFault			: 1 ;
		uint32_t		charging			: 1 ;
		uint32_t		fCharging			: 1 ; //浮充
		uint32_t		CanFault			: 1 ; //CAN掉线
		uint32_t		wParFlag			: 1 ;
		uint32_t		rParFlag			: 1 ;
		uint32_t		wAdjFlag			: 1 ;
		uint32_t		rAdjFlag			: 1 ;
		uint32_t		wFaultFlag			: 1 ;
		uint32_t		rFaultFlag			: 1 ;
    }status_bit;
    struct _status_u16
    {
        uint16_t low;
        uint16_t high;
    }status_u16;
    uint32_t status_u32;
}sc_status_t;

typedef struct _sc_data_t_{
	int chargeCnt;
	int faultCnt;
}sc_data_t;

typedef struct{
	sc_warn_t	warn;
	sc_fault_t fault;
	sc_data_t   data;
}sc_info_t;

typedef struct
{
	Charge_Cmd_STYP dcdcCmd;
	CAN_CB_Status_STYP	cbStatus;
	CB_Para_STYP	cbPara;
	CB_Para_STYP	cbWrPara;
	CB_Adjust_STYP	cbAdj;
	CB_Adjust_STYP	cbWrAdj;
	uint8_t iap_info[16];
	uint8_t iap_file[256];
	uint8_t iap_check[8];
	uint8_t iap_reply[8];
	uint8_t	reBoot[8];
	uint8_t null[8];
}CanBuf_config;

typedef struct
{
	Charge_Cmd_STYP dcdcCmd;
	uint8_t iap_info[16];
	uint8_t iap_file[256];
	uint8_t iap_check[8];
	uint8_t	iap_reply[8];
	uint8_t	reBoot[8];
	uint8_t null[8];
	uint16_t iap_index;
}canAppBuf_config;

typedef struct{
	ad_data_t	ad;
	sc_info_t	output;
	sc_status_t status;
	CAN_CB_Status_STYP	cbStatus;
	CB_Para_STYP	cbPara;
	CB_Para_STYP	cbWrPara;
	CB_Adjust_STYP	cbAdj;
	CB_Adjust_STYP	cbWrAdj;
	canAppBuf_config canAppBuf;
}ScData;

typedef struct{
	CB_Para_STYP cbPara;
	CB_Adjust_STYP cbAdj;
}Config_STYP;

#define ADJUST_DEFAULT {\
	100,\
	0,\
}

#define CB_ADJUST_DEFAULT {\
	ADJUST_DEFAULT,\
	ADJUST_DEFAULT,\
	ADJUST_DEFAULT,\
	ADJUST_DEFAULT,\
	ADJUST_DEFAULT,\
	ADJUST_DEFAULT,\
}
	
#define PARA_CB_STYP_DEFAULT	{\
	7000,\
	2000,\
	7500,\
	2500,\
	0,\
	0,\
	0,\
	0,\
	13000,\
	130,\
	130,\
	{0},\
	}
												
#define Config_STYP_DEFAULT 	{\
	PARA_CB_STYP_DEFAULT,\
	CB_ADJUST_DEFAULT,\
	}

#endif

