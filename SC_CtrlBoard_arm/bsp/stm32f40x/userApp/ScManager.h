#include "Sc.h"
#include "Failure.h"
#include "RelayModule.h"
#include "DataStruct.h"
#include "canApp.h"
#include "ScBat.h"
#include "ScFailure.h"

class ScManager : public Sc
{
public:
	ScManager() { ; } 
	~ScManager(){}

	ScData shareData;
		
	void Init(void);
	void Run(void);
	void RelayRun(void);
	void RefreshAdData(void);
	void RefreshPt100Data(void);
	void MonitorStatusUpdata(void);
	void FaultDeal(void);
	
	void SetCan(CanApp* pCan);
	void SlowCheck(void);
	void MonitorCheckSlow(void);
	void FaultCheckModuleInit(void);
		
private:
	List<ScFailure *> warnList;
	List<ScFailure *> faultList;

	CanApp *pCan;

	RelayModule relays;
	Relay systemResetRelay;
	
	void WarnRelayRun(void);
	void FaultRelayRun(void);
	void RefreshWarnList(void);
	void RefreshFaultList(void);
	void UpdateFaultState(void);
	void UpdateWarnState(void);
    
	bool warnListLock;
	bool faultListLock;
};
