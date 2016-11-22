#include "ScBat.h"
#include "ad.h"

void ScBat::Init()
{
	int * ad_buffer = get_ad_data_buffer();
	
	sensorList.Add(&inVolt);
	sensorList.Add(&cFlyVolt);
	sensorList.Add(&outVolt);
	sensorList.Add(&batCurr);
	sensorList.Add(&iOutPeak);
	
	inVolt.Init(&ad_buffer[u_in_index], 13533, 95);
	cFlyVolt.Init(&ad_buffer[u_cfly_index], 13533, 80);
	outVolt.Init(&ad_buffer[u_out_index], 13533, 95);
	batCurr.Init(&ad_buffer[i_out_index], 4500, 95);
	iOutPeak.Init(&ad_buffer[i_out_peak_index], 4500, 95);
	igbt1Temp.Init(&ad_buffer[temp1_index], 0, 5);
	igbt2Temp.Init(&ad_buffer[temp2_index], 0, 5);
	
}

void ScBat::RefreshState()
{
	Product::RefreshTransducerData(sensorList);
	
	igbt1Temp.Update();
	igbt2Temp.Update();
	
	FastCheck();
	
	UpdateFaultState();
}

void ScBat::On()
{
	if (!isStartEnable)
	{
		Device::On();
		state = SC_SOFT_START;
	}
	
	switch (state)
	{
		case SC_SOFT_START:
			iReference += 10;
			if (iTarget <= iReference)
			{
				iReference = iTarget;
				state = SC_CURR_LIMITED;
			}
			break;
			
		case SC_CURR_LIMITED:
			if (iTarget > (iReference + 5))
			{
				iReference += 5;
			}
			else if (iTarget < (iReference - 5))
			{
				iReference -= 5;
			}
			else
			{
				iReference = iTarget;
			}
				
			break;
		case SC_POWER_LIMITED:
			break;
		default:
			break;
	}
}

void ScBat::Off()
{
	if (isStartEnable)
	{
		Device::Off();
		state = SC_STOP;
		duty1 = 0;
		duty2 = 0;
		cFlyDuty = 0;
		presentDuty = 0;
		iReference = 0;
		pidVoltage.Reset();
		pidBatteryCurrent.Reset();
		pidIOutPeak.Reset();
		pidCfly.Reset();
	}
	uReference = outVolt.GetIRealValue();
	
}

void ScBat::Run()
{
	if (!isStartEnable)
	{
		return;
	}
	
	switch (startMode)
	{
		case SC_START_CHARGE:
			ChargeCtrl();
			break;
		case SC_START_DISCHARGE:
			DisChargeCtrl();
			break;
		default:
			break;
	}
	
}

int lastUIn;

int lowRate = 45;
int highRate = 55;

int lowVolt = 0;
int highVolt = 0;

void ScBat::ChargeCtrl()
{
	int iBat = 0;
	int uOut = 0;
	int uIn = 0;
	int uInFilt = 0;
	int uCFly = 0;
	
	iBat = batCurr.GetIRealValue();
	uOut = outVolt.GetIRealValue();
	uOutFilt = outVolt.GetAverageRealValue();
	uIn = inVolt.GetIRealValue();
	uInFilt = inVolt.GetAverageRealValue();
	uCFly = cFlyVolt.GetIRealValue();
	
	lowVolt = ((lowRate * uInFilt+50)/100);
	highVolt = ((highRate * uInFilt+50)/100);
	
	uInDerivative = lastUIn - uIn;
	lastUIn = uIn;
	
	uInRipple = (uIn - uInFilt) * 12000 / uIn;
	if (uInRipple > 900)
	{
		uInRipple = 900;
	}
	else if (uInRipple < -900)
	{
		uInRipple = -900;
	}
	
	filterDuty = (filterDuty * 4 + duty1 + 2) / 5;
	if(filterDuty < 4500)
    {
        pidBatteryCurrent.proportionalGain = maxPGain;
    }
    else if(filterDuty > 13500)
    {
        pidBatteryCurrent.proportionalGain = minPGain;
    }
    else
    {
        int k = (minPGain - maxPGain) / 2;
        
        pidBatteryCurrent.proportionalGain = filterDuty * k / 4500 + maxPGain - k;
    }
	
	pidVoltage.feedback = uOut;
    pidVoltage.Update();
    
    pidIOutPeak.reference = iOutPeak.GetAverageRealValue();
    pidIOutPeak.feedback =  iOutPeak.GetIRealValue();;
    pidIOutPeak.Update(0);

    pidBatteryCurrent.reference = iReference;
    pidBatteryCurrent.feedback = iBat;
    pidBatteryCurrent.Update();
    
    pidCfly.reference = 0;
    pidCfly.feedback = uCFly - (uInFilt / 2);
    pidCfly.Update();
	
	dutyVoltage = pidVoltage.out / 200;
    dutyBattary = pidBatteryCurrent.out / 200;
    cFlyDuty    = pidCfly.out / 200;
    iPeakDuty    = pidIOutPeak.out;
	
	if (dutyBattary < dutyVoltage)
	{
		//ºãÁ÷¿ØÖÆ
        presentDuty = dutyBattary;
        pidVoltage.Reset(presentDuty * 200);
//        isSteady = true;
//        pidMode = 1;
	}
	else
	{
		//ºãÑ¹¿ØÖÆ
        presentDuty = dutyVoltage;
        iPeakDuty = 0;
        pidBatteryCurrent.Reset(presentDuty * 200);
//        pidMode = 3;
	}
	
	if (presentDuty > 500)
	{
		presentDuty -= (uInRipple / kpInRipple + uInDerivative * uOutFilt / 1400);
	}
	
	if (presentDuty > 1000)
	{
		presentDuty += iPeakDuty;
	}
	
	if (presentDuty > thresholdDuty)
	{
		presentDuty = thresholdDuty;
	}

	duty1 = presentDuty;
	
	if (duty1 >= 432)
	{
		int tmpDuty = cFlyDuty;
		int flag = 1;
		if (tmpDuty < 0)
		{
			flag = -1;
			tmpDuty = -tmpDuty;
		}
		
		if(tmpDuty > duty1 / 10)
		{
			duty2 = duty1 - duty1 * flag / 10;
		}
		else
		{
			duty2 = duty1 - cFlyDuty;
		}
	}
	else
	{
		duty2 = duty1;
		
		if(uCFly < lowVolt)//·É¿çµçÈÝÆ«µÍ
		{
			if(++cutTime >= 5)
			{
				cutTime = 0;
				duty1 = 360;//432->6us   216-->3us
				duty2 = 0;
			}
		}
		else if(uCFly > highVolt)//·É¿çµçÈÝÆ«¸ß
		{
			if(++cutTime >= 5)
			{
				cutTime = 0;
				duty1 = 0;//432->6us   216-->3us
				duty2 = 360;
			}
		}
		else
		{
			duty1 = 0; 
			duty2 = 0;
			cutTime = 0;
		}
	}
}

void ScBat::DisChargeCtrl()
{
	int iBat = 0;
	int uIn = 0;
	int uInFilt = 0;
	int uCFly = 0;
	
	iBat = batCurr.GetIRealValue();
	uOutFilt = outVolt.GetAverageRealValue();
	uIn = inVolt.GetIRealValue();
	uInFilt = inVolt.GetAverageRealValue();
	uCFly = cFlyVolt.GetIRealValue();
	
	pidVoltage.feedback = uIn;
    pidVoltage.Update();

    pidBatteryCurrent.reference = iReference;
    pidBatteryCurrent.feedback = -iBat;
    pidBatteryCurrent.Update();
    
    pidCfly.reference = 0;
    pidCfly.feedback = uCFly - (uInFilt / 2);
    pidCfly.Update();
	
	dutyVoltage = pidVoltage.out / 200;
    dutyBattary = pidBatteryCurrent.out / 200;
    cFlyDuty    = pidCfly.out / 200;
	
	if (dutyBattary < dutyVoltage)
	{
		//ºãÁ÷¿ØÖÆ
        presentDuty = dutyBattary;
        pidVoltage.Reset(presentDuty * 200);
//        isSteady = true;
//        pidMode = 1;
	}
	else
	{
		//ºãÑ¹¿ØÖÆ
        presentDuty = dutyVoltage;
        iPeakDuty = 0;
        pidBatteryCurrent.Reset(presentDuty * 200);
//        pidMode = 3;
	}
	
	if (presentDuty > thresholdDuty)
	{
		presentDuty = thresholdDuty;
	}

	duty1 = presentDuty;
	
	if (duty1 >= 432)
	{
		int tmpDuty = cFlyDuty;
		int flag = 1;
		if (tmpDuty < 0)
		{
			flag = -1;
			tmpDuty = -tmpDuty;
		}
		
		if(tmpDuty > duty1 / 10)
		{
			duty2 = duty1 - duty1 * flag / 10;
		}
		else
		{
			duty2 = duty1 - cFlyDuty;
		}
	}
	else
	{
		duty1 = 0; 
		duty2 = 0;
	}
}

void ScBat::FaultCheckModuleInit()
{
	failureList.Add(&canLost);
	failureList.Add(&iBatMinSlow);
	failureList.Add(&iBatMaxSlow);
	failureList.Add(&uBatMinSlow);
	failureList.Add(&uBatMaxSlow);
	failureList.Add(&uInMaxSlow);
	failureList.Add(&uInMinSlow);
	failureList.Add(&igbt1TempOver);
	failureList.Add(&igbt2TempOver);
	failureList.Add(&iBatMaxHW);
	failureList.Add(&uBatMaxHW);
	failureList.Add(&iBatMinFast);
	failureList.Add(&iBatMaxFast);
	failureList.Add(&uBatMinFast);
	failureList.Add(&uBatMaxFast);
	failureList.Add(&uInMaxFast);
	failureList.Add(&uInMinFast);
	failureList.Add(&shortCurr);
	failureList.Add(&uCfly);
	
	relays.Register(&canLostHoldTime);
    relays.Register(&iBatMinHoldTime);
    relays.Register(&iBatMaxHoldTime);
    relays.Register(&uBatMinHoldTime);
    relays.Register(&uBatMaxHoldTime);
    relays.Register(&uInMaxHoldTime);
    relays.Register(&uInMinHoldTime);
	relays.Register(&igbt1TOverHoldTime);
    relays.Register(&igbt2TOverHoldTime);
}

void ScBat::FastCheck()
{
	if (!isStartEnable)
	{
		return;
	}
	UInCheckFast();
	IOutCheckFast();
	UOutCheckFast();
	IOCheck();
}

void ScBat::SlowCheck()
{
	UInCheck();
	IOutCheck();
	UOutCheck();
}

void ScBat::UpdateFaultState()
{
	FailureCheck::UpdateFaultState();
}

void ScBat::RefreshRelay()
{
	relays.Refresh();
}

void ScBat::UInCheckFast()
{

}

void ScBat::IOutCheckFast()
{

}

void ScBat::UOutCheckFast()
{

}

void ScBat::IOCheck()
{

}

void ScBat::UInCheck()
{

}

void ScBat::IOutCheck()
{

}

void ScBat::UOutCheck()
{

}

void ScBat::SetSharedData(void * sharedData)
{
	this->scData = (ScData *)sharedData;
}
