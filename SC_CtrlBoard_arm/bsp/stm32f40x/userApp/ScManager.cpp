#include "ScManager.h"
#include "can_app_def.h"
#include "ScManagerExtern.h"
#include "string.h"
#include "io.h"
#include "ad.h"
#include "fsmc.h"
#include "string.h"
#include "Event.h"
#include "fault.h"
#include "DataStruct.h"

#define DS_SC

ScManager sc;
CanApp can(sc.shareData);

Config_STYP config = Config_STYP_DEFAULT;

Event<int> event;

/*******************************************************************************
* Function Name  :  
* Description    :
*
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ScManager::Init(void)
{
	//------------------------------------------------------
	// ARM软件版本
	//------------------------------------------------------
	
	FaultCheckModuleInit();
}
/*******************************************************************************
* Function Name  :  
* Description    :
*
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int * buffer = NULL;
void ad_init(void)
{
	buffer = get_ad_data_buffer();
	
//	sc.input1Current.Init(&buffer[i_in1_index], 7500, 80);//2500/(10*0.5*2)*3/2*10
//	sc.input2Current.Init(&buffer[i_in2_index], 7500, 80);
//	sc.outCurrent.Init(&buffer[i_out_index], 6250, 80);//5000/(10*0.6*2)*10*3/2
//	sc.inputAc1Voltage.Init(&buffer[u_ac1_index], 13533, 20);//80000/(59.1*0.75*2) * 3/2 *10
//	sc.inputBusVoltage.Init(&buffer[u_bus_index], 13533, 80);
//	sc.output1Voltage.Init(&buffer[u_out1_index], 13533, 80);
//	sc.output2Voltage.Init(&buffer[u_out2_index], 13533, 80);
//	sc.output3Voltage.Init(&buffer[u_out3_index], 13533, 80);
//	sc.module1Temp.Init(&buffer[temp_module1_index], 0, 5);
//	sc.module2Temp.Init(&buffer[temp_module2_index], 0, 5);
//	sc.cap1Temp.Init(&buffer[temp_cap1_index], 0, 5);
//	sc.cap2Temp.Init(&buffer[temp_cap2_index], 0, 5);
}

/*******************************************************************************
* Function Name  :  
* Description    :
*
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void sc_init(void)
{
	can.Init();
	sc.SetCan(&can);
	
	ad_init();
	fault_init();
	sc.Init();
}

/*******************************************************************************
* Function Name  :  MonitorStatusUpdata(void)
* Description    :
*
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ScManager::MonitorStatusUpdata(void)
{
	
}

/*******************************************************************************
* Function Name  :  
* Description    :
*
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ScManager::SetCan(CanApp* can)
{
	this->pCan = can;
}

void ScManager::Run(void)
{
	
}
/*******************************************************************************
* Function Name  :  
* Description    :
*
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ScManager::FaultDeal(void)
{
	shareData.status.status_bit.wFault = shareData.output.warn.warn_u32 > 0;
	if (shareData.status.status_bit.sFault == 0 && shareData.output.fault.fault_u32 > 0)
	{
		//--------------------------------------------------------//
		//系统故障生成事件记录
		//--------------------------------------------------------//
		shareData.status.status_bit.sFault = true;
	}
	else if (shareData.status.status_bit.sFault == 1 && shareData.output.fault.fault_u32 == 0)
	{
		//--------------------------------------------------------//
		//系统故障消除事件记录
		//--------------------------------------------------------//
		shareData.status.status_bit.sFault = false;
	}
}
/*******************************************************************************
* Function Name  :  
* Description    :
*
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void data_refresh(void)
{
	sc.RefreshAdData();
	sc.RefreshPt100Data();
}

/*******************************************************************************
* Function Name  :  
* Description    :
*
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void state_control(void)
{
	sc.Run();
}
/*******************************************************************************
* Function Name  :  
* Description    :
*
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void status_updata(void)
{
	sc.SlowCheck();
	
	sc.MonitorStatusUpdata();
	
	sc.FaultDeal();
	
	fault_check();
}
/*******************************************************************************
* Function Name  :  
* Description    :
*
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void relays_refresh(void)
{
	sc.RelayRun();
}
/*******************************************************************************
* Function Name  :  
* Description    :
*
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void * GetShareDataPtr(void)
{
	ScData * p = &(sc.shareData);
	return p;
}

int GetEvent(void)
{
	int tmp = event.get();
	return tmp;
}

void PutEvent(int e)
{
	event.put(e);
}
