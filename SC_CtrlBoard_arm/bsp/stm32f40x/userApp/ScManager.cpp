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
	
	sc.inVolt.Init(&buffer[u_in_index], 13533, 95);
	sc.cFlyVolt.Init(&buffer[u_cfly_index], 13533, 80);
	sc.outVolt.Init(&buffer[u_out_index], 13533, 95);
	sc.batCurr.Init(&buffer[i_out_index], 4500, 95);
	sc.iOutPeak.Init(&buffer[i_out_peak_index], 4500, 95);
	sc.igbt1Temp.Init(&buffer[temp1_index], 0, 5);
	sc.igbt2Temp.Init(&buffer[temp2_index], 0, 5);

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
void task_entry(void)
{
	sc.RefreshAdData();
	
	
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
