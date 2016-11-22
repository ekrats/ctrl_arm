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
//CanApp can(sc.shareData);



Event<int> event;

void ScManager::Init(void)
{
	//------------------------------------------------------
	// ARMÈí¼þ°æ±¾
	//------------------------------------------------------
	
	FaultCheckModuleInit();
}

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

void sc_init1(void)
{
//	can.Init();
//	sc.SetCan(&can);
	
	ad_init();
	fault_init();
	sc.Init();
}

void ScManager::StatusUpdata(void)
{
	shareData.ad.u_in = sc.inVolt.GetAverageRealValue();
	shareData.ad.i_out = sc.batCurr.GetAverageRealValue();
	shareData.ad.u_out = sc.outVolt.GetAverageRealValue();
	shareData.ad.u_cfly = sc.cFlyVolt.GetAverageRealValue();
	shareData.ad.temp_igpt1 = sc.igbt1Temp.GetRealValue();
	shareData.ad.temp_igpt2 = sc.igbt2Temp.GetRealValue();

}

void ScManager::SetCan(CanApp* can)
{
	this->pCan = can;
}

void ScManager::Run(void)
{
	
}


void task_entry1(void)
{
	sc.RefreshAdData();
	
	sc.FastCheck();
	
	
	
}


void state_control(void)
{
	sc.Run();
}

void status_updata(void)
{
	sc.SlowCheck();
	
	sc.StatusUpdata();
	
	fault_check();
}

void relays_refresh(void)
{
	sc.RelayRun();
}

void * GetShareDataPtr1(void)
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
