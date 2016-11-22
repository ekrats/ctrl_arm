#pragma  once
#include "Product.h"
#include "FailureCheck.h"
#include "Contactor.h"
#include "Counter.h"
#include "OnOff.h"
#include "PID.h"
#include "NTC.h"
#include "DCTransducer.h"
#include "DataStruct.h"

enum SC_STATE
{
    SC_STOP,
    SC_SOFT_START,
    SC_CURR_LIMITED,
    SC_POWER_LIMITED,
};

enum SC_START_MODE
{
    SC_START_NONE,
	SC_START_CHARGE,
    SC_START_DISCHARGE,
};

class ScBat : public Product, public FailureCheck
{
public:
	ScBat()
	{
		pidVoltage.proportionalGain = 150;
		pidVoltage.integralGain = 2;
		pidVoltage.integralCorrection = 1;
		pidVoltage.outMax = 5;
		pidVoltage.outMin = -10;

		pidIOutPeak.proportionalGain = 60;
		pidIOutPeak.integralGain = 0;
		pidIOutPeak.integralCorrection = 0;
		pidIOutPeak.outMax = 500;
		pidIOutPeak.outMin = -500;
		
		maxPGain = 30;
		minPGain = 20;
		
		pidBatteryCurrent.proportionalGain = maxPGain;
		pidBatteryCurrent.integralGain = 2;
		pidBatteryCurrent.derivativeGain = 20;
		pidBatteryCurrent.integralCorrection = 1;
		pidBatteryCurrent.outMax = 5;
		pidBatteryCurrent.outMin = -5;
		
		pidCfly.proportionalGain = 1000;
		pidCfly.integralGain = 0;
		pidCfly.integralCorrection = 0;
		pidCfly.outMax = 5;
		pidCfly.outMin = -5;
		
		pidCfly.Reset();
		pidVoltage.Reset();
		pidIOutPeak.Reset();
		pidBatteryCurrent.Reset();
		
		kpInRipple = 7;
		//kiInRipple = 0;
		thresholdDuty = 16000;
		iTarget = 200;
	}

private:
	SC_STATE state;
	SC_START_MODE startMode;

	ScData * scData;
public:
	void Init();
	void RefreshState();
	int GetDuty1(){ return duty1; };
    int GetDuty2(){ return duty2; };
	virtual void On();
    virtual void Off();
    virtual void Run();
	virtual void FastCheck();
    virtual void SlowCheck();
    virtual void SetSharedData(void * sharedData);
    virtual void FaultCheckModuleInit();
    virtual void UpdateFaultState();
    virtual void RefreshRelay();
	inline  void StartMode(SC_START_MODE mode ) { startMode = mode ;};
	
	void SetUOutTarget(int value)
	{
		if ((value < 900) && (value > 0))
		{
			uTarget = value;
		}
	}
	
	void SetIOutTarget(int value)
	{
		if ((value <= 225) && (value > 0))
		{
			iTarget = value;
		}
	}
private:
    PID pidVoltage;
    PID pidBatteryCurrent;
    PID pidIOutPeak;
    PID pidCfly;

    int pidMode;
	int ctrlMode;
private:
    int thresholdDuty;
    int lastDuty;
    int presentDuty;

    int maxPGain;
    int minPGain;

    int uOutFilt;
    int uReference;
    int uTarget;

    int uInRipple;
	int uInDerivative;
    int kpInRipple;
    //int kiInRipple;

    int iTarget;
    int iReference;
    int duty1;
    int duty2;
	int filterDuty;
    int cFlyDuty;
    int iPeakDuty;
    int dutyVoltage;
	int dutyBattary;
	
	int cutTime;

public:
	DCTransducer inVolt;
	DCTransducer cFlyVolt;
	DCTransducer outVolt;
	DCTransducer batCurr;
	DCTransducer iOutPeak;

	NTC igbt1Temp;
	NTC igbt2Temp;

private:
	void ChargeCtrl();
	void DisChargeCtrl();

	void UInCheckFast();
	void IOutCheckFast();
	void UOutCheckFast();
	void IOCheck();

	void UInCheck();
	void IOutCheck();
	void UOutCheck();

private:
	Failure canLost;
	Failure iBatMinSlow;
	Failure iBatMaxSlow;
	Failure uBatMinSlow;
	Failure uBatMaxSlow;
	Failure uInMaxSlow;
	Failure uInMinSlow;
	Failure igbt1TempOver;
	Failure igbt2TempOver;
	Failure iBatMaxHW;
	Failure uBatMaxHW;
	Failure iBatMinFast;
	Failure iBatMaxFast;
	Failure uBatMinFast;
	Failure uBatMaxFast;
	Failure uInMaxFast;
	Failure uInMinFast;
	Failure shortCurr;
	Failure uCfly;

	Relay canLostHoldTime;
    Relay iBatMinHoldTime;
    Relay iBatMaxHoldTime;
    Relay uBatMinHoldTime;
    Relay uBatMaxHoldTime;
    Relay uInMaxHoldTime;
    Relay uInMinHoldTime;
	Relay igbt1TOverHoldTime;
	Relay igbt2TOverHoldTime;
	
	Counter iBatMinCount;
	Counter iBatMaxCount;
	Counter uBatMinCount;
	Counter uBatMaxCount;
	Counter uInMaxCount;
	Counter uInMinCount;
};

