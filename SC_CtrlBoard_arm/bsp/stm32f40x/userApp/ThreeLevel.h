#pragma  once
#include "stdint.h"
#include "FailureCheck.h"
#include "Contactor.h"
#include "Counter.h"
#include "OnOff.h"
#include "NTC.h"
#include "DCTransducer.h"
#include "Bat.h"
#include "Product.h"
#include "support.h"
#include "DataStruct.h"

enum FAN_STATE
{
    FAN_STOP,
    FAN_SPEED_HALH,
    FAN_SPEED_FULL,
};

class ThreeLevel : public Product, public FailureCheck
{
public:
	ThreeLevel()
	{
		faultKeepTime.SetRelayTime(30000);
		restartTime.SetRelayTime(100);
	}
	
	void Init();
	void BatControl();
    void StateControl();
	void RefreshLocalData();
	void RefreshCanData();
	void FanLogic();
	void CanCheck();
    virtual void On();
    virtual void Off();
    virtual void Run();
    virtual void FastCheck();
    virtual void SlowCheck();
    virtual void SetSharedData(void * sharedData);
    virtual void FaultCheckModuleInit(){};
    virtual void RefreshRelay();
	
public:
	ScData * scData;
		
private:
	Bat bat;

private:
	Relay faultKeepTime;
    Relay restartTime;

private:
	SC_START_MODE startMode;
	FAN_STATE fanState;

	int duty1;
	int duty2;
	
	int fanDuty;
	int fanRunTime;

	uint32_t sys_fault;
	uint32_t sys_lock;

	bool isConstP;
	bool fanSteady;

};
