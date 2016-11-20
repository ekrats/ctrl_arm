#ifndef _DataStruct_h_
#define _DataStruct_h_

#include "stdint.h"
#include "can_app_def.h"

#define FanIn_Temp_Over					85
#define FanOut_Temp_Over				85

#define CHARGE_BASE_T						10//ms
#define PRE_CHARGE_DELAY1			(60000/CHARGE_BASE_T)//预充电时间
#define PRE_CHARGE_DELAY2			(500/CHARGE_BASE_T)

#define SYS_STATE_STOP			    0x0100
#define SYS_STATE_PRECHARGE			0x0200
#define SYS_STATE_IDLE				0x0300
#define SYS_STATE_FAULT				0x0400
#define SYS_STATE_TEST				0x0500
#define SYS_STATE_SLEEP				0x0600

#define SYS_OUT_STOP			    0x1000
#define SYS_OUT_PRECHARGE			0x2000
#define SYS_OUT_IDLE				0x3000
#define SYS_OUT_FAULT				0x4000
#define SYS_OUT_TEST				0x5000
#define SYS_OUT_SLEEP 				0x6000

#define CF_HW_VERSION				0x1002// 10 广州　　　02 SC02
#define CF_SW_VERSION				100

typedef struct{
	int16_t i_in1;
	int16_t i_in2;
	int16_t i_out;
	int16_t u_ac1;
	int16_t u_bus;
	int16_t u_out1;
	int16_t u_out2;
	int16_t u_out3;
	int16_t temp_module1;
	int16_t temp_module2;
	int16_t temp_cap1;
	int16_t temp_cap2;
}ad_data_t;

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
        uint32_t in_fuse1_fail   :1;
        uint32_t in_fuse2_fail   :1;
        uint32_t ac1_volt_over   :1;
        uint32_t bus_over	     :1;
		
        uint32_t bus_under       :1;
        uint32_t m1_temp_over    :1;
        uint32_t m2_temp_over    :1;
        uint32_t cap1_temp_over  :1;
		
        uint32_t cap2_temp_over  :1; 
        uint32_t m1_curr_fail    :1;
        uint32_t m2_curr_fail    :1;
        uint32_t m1_curr_over    :1;
		
        uint32_t m2_curr_over    :1;
		
		uint32_t res			 :19;
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

typedef struct{
	ad_data_t	ad;
	sc_info_t	output;
	sc_status_t status;
	CAN_CB_Status_STYP	cbStatus;
	CB_Para_STYP	cbPara;
	CB_Para_STYP	cbWrPara;
	CB_Adjust_STYP	cbAdj;
	CB_Adjust_STYP	cbWrAdj;
	//HMI_TYPE 	hmi;
	canAppBuf_config canAppBuf;
}ScData;

typedef struct{
	CB_Para_STYP cbPara;
	CB_Adjust_STYP cbAdj;
	u16 hwVersionW;
	u16 swVersionW;
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
	CF_HW_VERSION,\
	CF_SW_VERSION,\
	}

#endif

