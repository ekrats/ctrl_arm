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
	bat.Init();
	
	bat.FaultCheckModuleInit();
	
	bat.SetSharedData(&shareData);
	
	SetSharedData(&shareData);
}

void sc_init()
{
	can.Init();
	
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

void ThreeLevel::RefreshLocalData()
{
	scData->ad.i_out = bat.batCurr.GetAverageRealValue();
	scData->ad.i_peak = bat.iOutPeak.GetAverageRealValue();
	scData->ad.u_in = bat.inVolt.GetAverageRealValue();
	scData->ad.u_cfly = bat.cFlyVolt.GetAverageRealValue();
	scData->ad.u_out = bat.outVolt.GetAverageRealValue();
	
	scData->ad.temp_igpt1 = bat.igbt1Temp.GetRealValue();
	scData->ad.temp_igpt2 = bat.igbt2Temp.GetRealValue();
}

void ThreeLevel::RefreshCanData()
{
	scData->output.fault.fault_u32 = this->sys_fault;
	scData->output.warn.warn_u32 = (bat.GetFault() >> 16);
	scData->output.data.duty1 = this->duty1;
	scData->output.data.duty2 = this->duty2;
	scData->cbStatus.capvoltW = scData->ad.u_out;
	scData->cbStatus.currW = scData->ad.i_out;
	scData->cbStatus.flyingCapvoltW = scData->ad.u_cfly;
	scData->cbStatus.gridCapvoltW = scData->ad.u_in;
	scData->cbStatus.igbt1Tempr = scData->ad.temp_igpt1;
	scData->cbStatus.igbt2Tempr = scData->ad.temp_igpt2;
	scData->cbStatus.flags.sysFailN = sys_fault > 0;
	scData->cbStatus.flags.chargeOnN = (isStartEnable && (startMode == 1));
	scData->cbStatus.flags.floatChargeN = bat.IsOutputSteady() && (startMode == 1);
	scData->cbStatus.flags.dischargeOnN = (isStartEnable && (startMode == 2));
	scData->cbStatus.flags.fanOnN = (fanState > 0);
	scData->cbStatus.flags.floatDischargeN = bat.IsOutputSteady() && (startMode == 2);
	scData->cbStatus.flags.flyCapBalanceN = scData->output.fault.fault_bit.u_cfly_max_slow || scData->output.fault.fault_bit.u_fly_max_hw;
	scData->cbStatus.flags.hwCapOverVoltN = scData->output.fault.fault_bit.u_bat_max_hw;
	scData->cbStatus.flags.hwOverCurrN = scData->output.fault.fault_bit.i_bat_max_hw;
	scData->cbStatus.flags.igbt1OverTemprN = scData->output.fault.fault_bit.igbt1_temp_over;
	scData->cbStatus.flags.igbt2OverTemprN = scData->output.fault.fault_bit.igbt2_temp_over;
	scData->cbStatus.flags.swCapOverVoltN = scData->output.fault.fault_bit.u_bat_max_fast
										|| scData->output.fault.fault_bit.u_bat_max_slow;
	scData->cbStatus.flags.swGridOverVoltN = scData->output.fault.fault_bit.u_in_max_fast
										|| scData->output.fault.fault_bit.u_in_max_slow;
	scData->cbStatus.flags.swOverCurrN = scData->output.fault.fault_bit.i_bat_max_fast
										|| scData->output.fault.fault_bit.i_bat_max_slow;					
}

int i_control = 250;
void ThreeLevel::BatControl()
{
	FastCheck();
	
	if ((sys_fault & faultMast) != 0
	  &&(sys_lock & faultMast) != 0)
	{
		if (IsRun())
		{
			start_record_fault();
		}
		this->Off();
	}
	
	this->Run();
	
	set_target_current(i_control);
	
	update_fpga_data(&bat.fpga);
}

void ThreeLevel::StateControl()
{
	int ctrlOrd = scData->canAppBuf.dcdcCmd.ord;
	
	RefreshLocalData();
	
	SlowCheck();
	
	RefreshRelay();
	
	if ((sys_fault & faultMast) == 0
	  &&(sys_lock & faultMast) == 0
	  && (ctrlOrd > 0)
	  && fanSteady == true
	  && ctrlOrd == startMode)
	{
		this->On();
		if (startMode == 1)
		{
			bat.SetIOutTarget(scData->cbPara.chargeCurrW);
			bat.SetUOutTarget(scData->cbPara.chargeVoltW);
		}
		else if (startMode == 2)
		{
			bat.SetIOutTarget(scData->cbPara.dischargeCurrW);
			bat.SetUOutTarget(scData->cbPara.dischargeVoltW);
		}
		faultKeepTime.Stop();
		bat.fpga.unlock = false;
	}
	else
	{
		this->Off();
		faultKeepTime.Start();
		if (faultKeepTime.GetResult() 
			&& (sys_fault != 0)
			&& (sys_lock == 0))
		{
			bat.ResetFaulture();
			faultKeepTime.Reset();
			//fpga.unlock = true;
		}
	}
	
	RefreshCanData();
	
	FanLogic();
	
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
		bat.StartMode(startMode);
		bat.ConstPowerMode(isConstP);
		restartTime.Start();
	}
	
	if (restartTime.GetResult())
	{
		Product::On();
		bat.On();
	}
}

void ThreeLevel::Off()
{
	if (isStartEnable)
	{
		Product::Off();
		restartTime.Reset();
	}
	bat.Off();
}

void ThreeLevel::Run()
{
	bat.Run();
	
	
}

void ThreeLevel::FastCheck()
{
	bat.RefreshState();
	
	sys_fault = (bat.GetFault() & faultMast);
	sys_lock = (bat.GetLock() & faultMast);
}

void ThreeLevel::SlowCheck()
{
	bat.SlowCheck();
	
	//CanCheck();
}

void ThreeLevel::FanLogic()
{
	switch (fanState)
	{
		case FAN_STOP:
			if (shareData.canAppBuf.dcdcCmd.ord > 0)
			{
				fanState = FAN_SPEED_FULL;
			}
			else if (shareData.ad.temp_igpt1 > 40 || shareData.ad.temp_igpt2 > 40)
			{
				fanState = FAN_SPEED_HALH;
			}
			else if (shareData.ad.temp_igpt1 > 50 || shareData.ad.temp_igpt2 > 50)
			{
				fanState = FAN_SPEED_FULL;
			}
			else
			{
				fanState = FAN_STOP;
			}
			//停止 继电器关断， 调速板占空比 0
			fanDuty = 0;
			stm32_hw_io_on(4);
			fanRunTime = 0;
			break;
		case FAN_SPEED_HALH:
			if (shareData.canAppBuf.dcdcCmd.ord > 0)
			{
				fanState = FAN_SPEED_FULL;
			}
			else if (shareData.ad.temp_igpt1 < 35 && shareData.ad.temp_igpt2 < 35)
			{
				fanState = FAN_STOP;
			}
			else if (shareData.ad.temp_igpt1 > 50 || shareData.ad.temp_igpt2 > 50)
			{
				fanState = FAN_SPEED_FULL;
			}
			else
			{
				fanState = FAN_SPEED_HALH;
			}
			//半速 继电器关断，调速板占空比 50%
			fanDuty = 3600;
			stm32_hw_io_on(4);
			if (fanRunTime < 10000)
				fanRunTime++;
			break;
		case FAN_SPEED_FULL:
			if ((shareData.ad.temp_igpt1 < 45 && shareData.ad.temp_igpt2 < 45)
			 && (shareData.canAppBuf.dcdcCmd.ord == 0))
			{
				fanState = FAN_STOP;
			}
			else
			{
				fanState = FAN_SPEED_FULL;
			}
			//全速 继电器导通，调速板占空比 0
			fanDuty = 0;
			stm32_hw_io_off(4);
			if (fanRunTime < 10000)
				fanRunTime++;
			break;
		default:
			break;
	}
	
	//风机启动1S之后，进入稳定状态
	if ((fanState != FAN_STOP) && (fanRunTime > 100))
	{
		fanSteady = true;
	}
	else
	{
		fanSteady = false;
	}
}

void ThreeLevel::RefreshRelay()
{
	bat.RefreshRelay();
	faultKeepTime.Refresh();
	restartTime.Refresh();
}

void ThreeLevel::CanCheck()
{
	if (++can.CanCnt > 200)
	{
		scData->canAppBuf.dcdcCmd.ord = 0;
		scData->status.status_bit.CanFault = true;
	}
}

void ThreeLevel::SetSharedData(void * sharedData)
{
	this->scData = (ScData *)sharedData;
}

void * GetShareDataPtr(void)
{
	return (void *)&shareData;
}
