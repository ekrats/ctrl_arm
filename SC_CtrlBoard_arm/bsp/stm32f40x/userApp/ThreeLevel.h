#pragma  once
#include "stdint.h"
#include "FailureCheck.h"
#include "Contactor.h"
#include "Counter.h"
#include "OnOff.h"
#include "NTC.h"
#include "DCTransducer.h"
#include "ScBat.h"
#include "Product.h"
#include "support.h"
#include "DataStruct.h"

class ThreeLevel : public Product//, public FailureCheck
{
public:
	ThreeLevel()
	{
	}
	
	void Init();
	void BatControl();
    void StateControl();
	void RefreshData();
    virtual void On();
    virtual void Off();
    virtual void Run();
    virtual void SetSharedData(void * sharedData);
    virtual void RefreshRelay();
	
private:
	ScBat scBat;
	fpga_data fpga;

private:
	Relay faultKeepTime;
    Relay restartTime;

private:
	int duty1;
	int duty2;

	uint32_t sys_fault;
	uint32_t sys_lock;

public:
	ScData * scData;
};
