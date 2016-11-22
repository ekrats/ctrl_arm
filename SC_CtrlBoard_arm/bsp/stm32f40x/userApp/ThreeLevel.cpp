#include "ThreeLevel.h"
#include "ScManagerExtern.h"
#include "record.h"
#include "canApp.h"
#include "io.h"
#include "ad.h"
#include "fsmc.h"

ThreeLevel tl;
ScData shareData;
CanApp can(shareData);
Config_STYP config = Config_STYP_DEFAULT;

int faultMast = 0xFFFF;

void ThreeLevel::Init()
{
	scBat.Init();
	
	scBat.FaultCheckModuleInit();
	
	SetSharedData(&shareData);
}

void sc_init()
{
	tl.Init();
}

void bat_entry(void)
{
	tl.BatControl();
}

void state_entry(void)
{
	tl.StateControl();
}

void ThreeLevel::RefreshData()
{
	scData->ad.i_out = scBat.batCurr.GetAverageRealValue();
	scData->ad.i_peak = scBat.iOutPeak.GetAverageRealValue();
	scData->ad.u_in = scBat.inVolt.GetAverageRealValue();
	scData->ad.u_cfly = scBat.cFlyVolt.GetAverageRealValue();
	scData->ad.u_out = scBat.outVolt.GetAverageRealValue();
	
	scData->ad.temp_igpt1 = scBat.igbt1Temp.GetRealValue();
	scData->ad.temp_igpt2 = scBat.igbt2Temp.GetRealValue();
}

int i_control = 250;
void ThreeLevel::BatControl()
{
	scBat.RefreshState();
	
	sys_fault = scBat.GetFault();
	sys_lock = scBat.GetLock();
	
	if ((sys_fault & faultMast) == 0
	  &&(sys_lock & faultMast) == 0)
	{
		if (IsRun())
		{
			start_record_fault();
		}
		this->Off();
	}
	
	this->Run();
	
	fpga.duty1 = this->duty1;
	fpga.duty2 = this->duty2;
	
	set_target_current(i_control);
	
	fpga.enable = scBat.IsRun();
	
}

void ThreeLevel::StateControl()
{
	RefreshData();
	
	scBat.SlowCheck();
	
	RefreshRelay();
	
	if ((sys_fault & faultMast) == 0
	  &&(sys_lock & faultMast) == 0)
	{
		this->On();
		faultKeepTime.Stop();
	}
	else
	{
		this->Off();
		faultKeepTime.Start();
		if (faultKeepTime.GetResult() && (sys_fault != 0))
		{
			scBat.ResetFaulture();
		}
	}
}

void ThreeLevel::On()
{
	if (!isStartEnable)
	{
		restartTime.Start();
	}
	
	if (restartTime.GetResult())
	{
		Product::On();
		//ģʽ
		scBat.On();
	}
}

void ThreeLevel::Off()
{
	if (isStartEnable)
	{
		Product::Off();
		restartTime.Reset();
	}
	scBat.Off();
}

void ThreeLevel::Run()
{
	scBat.Run();
	
	this->duty1 = scBat.GetDuty1();
	this->duty2 = scBat.GetDuty2();
}

void ThreeLevel::RefreshRelay()
{
	scBat.RefreshRelay();
	faultKeepTime.Refresh();
	restartTime.Refresh();
}

void ThreeLevel::SetSharedData(void * sharedData)
{
	this->scData = (ScData *)sharedData;
}

void * GetShareDataPtr(void)
{
	return (void *)&shareData;
}
