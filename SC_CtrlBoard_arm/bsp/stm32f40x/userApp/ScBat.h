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
	SC_CURR_LIMITTED,
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
		thresholdDuty = 16000;
		iTarget = 200;
		
		iBatMin1HoldTime.SetRelayTime(200);
		iBatMin2HoldTime.SetRelayTime(100);
		iBatMax1HoldTime.SetRelayTime(200);
		iBatMax2HoldTime.SetRelayTime(100);
		uBatMax1HoldTime.SetRelayTime(200);
		uBatMax2HoldTime.SetRelayTime(100);
		uInMax1HoldTime.SetRelayTime(200);
		uInMax2HoldTime.SetRelayTime(100);
		uInBat1HoldTime.SetRelayTime(200);
		uInBat2HoldTime.SetRelayTime(100);
		igbt1TOver1HoldTime.SetRelayTime(200);
		igbt1TOver2HoldTime.SetRelayTime(100);
		igbt2TOver1HoldTime.SetRelayTime(200);
		igbt2TOver2HoldTime.SetRelayTime(100);
		uCFly1HoldTime.SetRelayTime(200);
		uCFly2HoldTime.SetRelayTime(100);
	}

private:
	ScData * scData;
	fpga_data * fpga;

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
	inline  void ConstPowerMode(bool mode ) { isConstP = mode ;};
	void SetFpgaData(void * fpgadData);
	bool IsOutputSteady(){ return isSteady;}
	
	void SetUOutTarget(int value)
	{
		if ((value < 9200) && (value > 0))
		{
			uTarget = value;
			pidVoltage.reference = value;
		}
	}
	
	void SetIOutTarget(int value)
	{
		if ((value <= 2250) && (value > 0))
		{
			iTarget = value;
		}
	}
	
public:
	DCTransducer inVolt;
	DCTransducer cFlyVolt;
	DCTransducer outVolt;
	DCTransducer batCurr;
	DCTransducer iOutPeak;

	NTC igbt1Temp;
	NTC igbt2Temp;
	
private:
	Failure iBatMinSlow;
	Failure iBatMaxSlow;
	Failure uBatMaxSlow;
	Failure uInMaxSlow;
	Failure uInBatSlow;
	Failure igbt1TempOver;
	Failure igbt2TempOver;
	Failure uCfly;
	Failure iBatMaxHW;
	Failure uFlyMaxHW;
	Failure uBatMaxHW;
	Failure iBatMinFast;
	Failure iBatMaxFast;
	Failure uBatMaxFast;
	Failure uInMaxFast;
	Failure uInBatFast;

    Relay iBatMin1HoldTime;
	Relay iBatMin2HoldTime;
    Relay iBatMax1HoldTime;
	Relay iBatMax2HoldTime;
    Relay uBatMax1HoldTime;
	Relay uBatMax2HoldTime;
    Relay uInMax1HoldTime;
	Relay uInMax2HoldTime;
	Relay uInBat1HoldTime;
	Relay uInBat2HoldTime;
	Relay igbt1TOver1HoldTime;
	Relay igbt1TOver2HoldTime;
	Relay igbt2TOver1HoldTime;
	Relay igbt2TOver2HoldTime;
	Relay uCFly1HoldTime;
	Relay uCFly2HoldTime;
	
	Counter iBatMinCount;
	Counter iBatMaxCount;
	Counter uBatMaxCount;
	Counter uInMaxCount;
	Counter uInBatCount;
	
private:
    PID pidVoltage;
    PID pidBatteryCurrent;
    PID pidIOutPeak;
    PID pidCfly;

private:
	SC_STATE	state;
	SC_START_MODE startMode;

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
	void UCflyCheck();
	void TempCheck();

private:
	bool isConstP;
	bool isSteady;
};

