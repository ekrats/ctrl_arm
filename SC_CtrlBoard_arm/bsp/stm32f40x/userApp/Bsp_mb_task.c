/**
  *************************************************************************
  * This file is part of modbus
  * COPYRIGHT (C) 1999-2014, TongYe R&D Team
  * @file    Bsp_mb_tsk.c
  * @brief   This file provides all the modbus task functions.
  *************************************************************************
  */
#include "Bsp_mb_task.h"
#include "portmbapp.h"
#include <stdbool.h>
#include "mbconfig.h"
#include "DataStruct.h"
#include "string.h"
#include "math.h"
#include "logicApp.h"
#include "ScManagerExtern.h"

#if MB_SLAVE_RTU_ENABLED > 0 || MB_SLAVE_ASCII_ENABLED > 0
static void thread_entry_ModbusSlavePoll(void* parameter);
#endif

static void ModbusMasterTask(void* parameter);

//---------------------------------------------------------------------------
// modbus用户变量定义
//---------------------------------------------------------------------------
USHORT HmiRegAddress;
uint16_t MbTimeOutCnt = 100;  
char mbsd;
void Bsp_Modbus_Cycle(void)
{
	static bool lock = false;
	static uint8_t VersionInit = 0;
	ScData * p = GetShareDataPtr();
	//---------------- 状态机锁  ----------------//	
	if (lock) return ;
	lock = true; 
	
	//---------------------------------------------------
	// modbus 500ms通讯故障判断
	//---------------------------------------------------
	if(MbTimeOutCnt >= 5)
	{
		stm32_hw_io_off(1);
		p->status.status_bit.HmiFault = false;
	}
	else
	{
		MbTimeOutCnt++;
		stm32_hw_io_toggle(1);
		p->status.status_bit.HmiFault = true;  //通讯异常	
	}
	
	//--------------------------------------------------------------
	// DSP向触摸屏输出
	//--------------------------------------------------------------
	p->hmi.mbStatus.Event = GetEvent();
	PDHMI(Reg)->mbStatus = p->hmi.mbStatus;
	PDHMI(Reg)->cb1Status = p->cb1Status;
	PDHMI(Reg)->cb2Status = p->cb2Status;
	PDHMI(Reg)->cb3Status = p->cb3Status;
	PDHMI(Reg)->cb4Status = p->cb4Status;
	
	PDHMI(Reg)->mbPara = p->hmi.mbPara;
	
	PDHMI(Reg)->Modue1_Para = p->cb1Para;
	PDHMI(Reg)->Modue2_Para = p->cb2Para;
	PDHMI(Reg)->Modue3_Para = p->cb3Para;
	PDHMI(Reg)->Modue4_Para = p->cb4Para;
	
	PDHMI(Reg)->mbAdj = p->hmi.mbAdj;
	
	PDHMI(Reg)->cb1Adj = p->cb1Adj;
	PDHMI(Reg)->cb2Adj = p->cb2Adj;
	PDHMI(Reg)->cb3Adj = p->cb3Adj;
	PDHMI(Reg)->cb4Adj = p->cb4Adj;
	if (VersionInit == 0)
	{
		PDHMI(Reg)->version = p->hmi.version;
	}

	//--------------------------------------------------------------
	// 触摸屏向Monitor输出
	//--------------------------------------------------------------	
	
	//---------------- 解锁  ----------------//
	lock = false;
	
}

void RegAddressGet(USHORT Address)
{
	ScData * p = GetShareDataPtr();
	HmiRegAddress = Address - 1;
	switch(HmiRegAddress){
			case HMI_ADDR_CHARGE_EN:
				p->hmi.HMI_WAddr_flag[ADDR_CHARGE_EN] = true;
				p->hmi.CtrCmd.ChargeEn = PDHMI(Reg)->CtrCmd.ChargeEn;
				break;
			
			case HMI_ADDR_CHARGE_CMD :
				p->hmi.HMI_WAddr_flag[ADDR_CHARGE_CMD] = true;
				p->hmi.CtrCmd.ChargeCmd = PDHMI(Reg)->CtrCmd.ChargeCmd;
				break;
			
			case HMI_ADDR_DEV_TEST :
				p->hmi.HMI_WAddr_flag[ADDR_DEV_TEST] = true;
				p->hmi.CtrCmd.DevTestCmd = PDHMI(Reg)->CtrCmd.DevTestCmd;
				break;
			
			case HMI_ADDR_SLEEP :
				p->hmi.HMI_WAddr_flag[ADDR_SLEEP] = true;
				p->hmi.CtrCmd.SleepCmd = PDHMI(Reg)->CtrCmd.SleepCmd;
				break;
			
			case HMI_ADDR_RESTART :
				p->hmi.HMI_WAddr_flag[ADDR_RESTART] = true;
				p->hmi.CtrCmd.RestartCmd = PDHMI(Reg)->CtrCmd.RestartCmd;
				break;
			
			case HMI_ADDR_RESTORE :
				p->hmi.HMI_WAddr_flag[ADDR_RESTORE] = true;
				p->hmi.CtrCmd.RestoreParCmd = PDHMI(Reg)->CtrCmd.RestoreParCmd;
				break;
			
			case HMI_ADDR_CLEAR :
				p->hmi.HMI_WAddr_flag[ADDR_CLEAR] = true;
				p->hmi.CtrCmd.clearCmd = PDHMI(Reg)->CtrCmd.clearCmd;
				break;
			
			case HMI_ADDR_W_MB_PARA :
				p->hmi.HMI_WAddr_flag[ADDR_W_MB_PARA] = true;
				p->hmi.mbWrPara = PDHMI(Reg)->mbWrPara;
				break;
			
			case HMI_ADDR_W_CB1_PARA :
				p->hmi.HMI_WAddr_flag[ADDR_W_CB1_PARA] = true;
				p->cb1WPara = PDHMI(Reg)->Modue1_WPara;
				break;
			
			case HMI_ADDR_W_CB2_PARA :
				p->hmi.HMI_WAddr_flag[ADDR_W_CB2_PARA] = true;
				p->cb2WPara = PDHMI(Reg)->Modue2_WPara;
				break;
			
			case HMI_ADDR_W_CB3_PARA :
				p->hmi.HMI_WAddr_flag[ADDR_W_CB3_PARA] = true;
				p->cb3WPara = PDHMI(Reg)->Modue3_WPara;
				break;
			
			case HMI_ADDR_W_CB4_PARA :
				p->hmi.HMI_WAddr_flag[ADDR_W_CB4_PARA] = true;
				p->cb4WPara = PDHMI(Reg)->Modue4_WPara;
				break;
			
			case HMI_ADDR_W_MB_RTC :
				p->hmi.HMI_WAddr_flag[ADDR_W_MB_RTC] = true;
				p->hmi.Time = PDHMI(Reg)->Time;
				break;
			
			case HMI_ADDR_W_MB_ADJ :
				p->hmi.HMI_WAddr_flag[ADDR_W_MB_ADJ] = true;
				p->hmi.mbWrAdj = PDHMI(Reg)->mbWrAdj;
				break;
			
			case HMI_ADDR_W_CB1_ADJ :
				p->hmi.HMI_WAddr_flag[ADDR_W_CB1_ADJ] = true;
				p->cb1WrAdj = PDHMI(Reg)->cb1WrAdj;
				break;
			
			case HMI_ADDR_W_CB2_ADJ :
				p->hmi.HMI_WAddr_flag[ADDR_W_CB2_ADJ] = true;
				p->cb2WrAdj = PDHMI(Reg)->cb2WrAdj;
				break;
			
			case HMI_ADDR_W_CB3_ADJ :
				p->hmi.HMI_WAddr_flag[ADDR_W_CB3_ADJ] = true;
				p->cb3WrAdj = PDHMI(Reg)->cb3WrAdj;
				break;
			
			case HMI_ADDR_W_CB4_ADJ :
				p->hmi.HMI_WAddr_flag[ADDR_W_CB4_ADJ] = true;
				p->cb4WrAdj = PDHMI(Reg)->cb4WrAdj;
				break;
			
			case HMI_ADDR_R_CB1_ADJ :
				p->hmi.HMI_WAddr_flag[ADDR_R_CB1_ADJ] = true;
				break;
			
			case HMI_ADDR_R_CB2_ADJ :
				p->hmi.HMI_WAddr_flag[ADDR_R_CB2_ADJ] = true;
				break;
			
			case HMI_ADDR_R_CB3_ADJ :
				p->hmi.HMI_WAddr_flag[ADDR_R_CB3_ADJ] = true;
				break;
			
			case HMI_ADDR_R_CB4_ADJ :
				p->hmi.HMI_WAddr_flag[ADDR_R_CB4_ADJ] = true;
				break;
			default:
				break;
		}
}

/*******************************************************************************
* Function Name  : void Task_MBSlave_create(uint8_t priority) 
* Description    :  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Task_MBSlave_create(uint8_t priority) 
{
	rt_thread_t init_thread;
#if MB_SLAVE_RTU_ENABLED > 0 || MB_SLAVE_ASCII_ENABLED > 0
	//------- init thread
	init_thread = rt_thread_create("mbslave",
		thread_entry_ModbusSlavePoll, RT_NULL,
		512, priority, 20);
	//------- start up thread
	if(init_thread != RT_NULL)
		rt_thread_startup(init_thread);
#endif

	//------- init thread
	init_thread = rt_thread_create("mbtask",
		ModbusMasterTask, RT_NULL,
		512, priority, 20);
	//------- start up thread
	if(init_thread != RT_NULL)
		rt_thread_startup(init_thread);
}

/*******************************************************************************
* Function Name  : void thread_entry_ModbusSlavePoll(void) 
* Description    :  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#if MB_SLAVE_RTU_ENABLED > 0 || MB_SLAVE_ASCII_ENABLED > 0
static void thread_entry_ModbusSlavePoll(void* parameter)
{
	eMBInit(MB_RTU, 0x01, 1, 115200,  MB_PAR_NONE);
	eMBEnable();
	while (1)
	{
		eMBPoll();
	}
}
#endif

/*******************************************************************************
* Function Name  : static void ModbusMasterTask(void* parameter)
* Description    :  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void ModbusMasterTask(void* parameter)
{
	while (1)
	{
		rt_thread_delay(RT_TICK_PER_SECOND / 10);		
		Bsp_Modbus_Cycle();
	}
}

