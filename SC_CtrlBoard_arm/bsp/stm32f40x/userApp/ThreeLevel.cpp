#include "ThreeLevel.h"
#include "ScManagerExtern.h"
#include "fsmc.h"
#include "record.h"
#include "canApp.h"
#include "io.h"
#include "ad.h"
#include "fsmc.h"

ThreeLevel tl;
ScData shareData;

CanApp can;
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
	
	scData->fpga.duty1 = this->duty1;
	scData->fpga.duty2 = this->duty2;
	if (startMode == 2)
	{
		scData->fpga.discharge = true;
	}
	else
	{
		scData->fpga.discharge = false;
	}
	
	set_target_current(i_control);
	
	scData->fpga.enable = scBat.IsRun();
	
	update_fpga_data(&scData->fpga);
}

void ThreeLevel::StateControl()
{
	int ctrlOrd = scData->canAppBuf.dcdcCmd.ord;
	
	RefreshData();
	
	scBat.SlowCheck();
	
	RefreshRelay();
	
	if ((sys_fault & faultMast) == 0
	  &&(sys_lock & faultMast) == 0
	  && (ctrlOrd > 0)
	  && ctrlOrd == startMode)
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
	
	if (ctrlOrd == 1)
	{
		startMode = SC_START_CHARGE;
	}
	else if (ctrlOrd == 2)
	{
		startMode = SC_START_DISCHARGE;
	}
	else
	{
		startMode = SC_START_NONE;
	}
}

void ThreeLevel::On()
{
	if (!isStartEnable)
	{
		scBat.StartMode(startMode);
		restartTime.Start();
	}
	
	if (restartTime.GetResult())
	{
		Product::On();
		
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
