#include "Relay.h"
#include "Failure.h"

class ScFailure : public Failure
{
public:
	ScFailure(int lockTime, int conTime, int disTime):lockRelay(lockTime), 
		conRelay(conTime, 1) , disRelay(disTime)
	{
		Failure::SetLimitedTimes(lockTime);
	}
	inline int GetStatus()
	{
		return occur;
	}
	
	void OccurredClear();
	
	void OccurNow();
		
	void OccurStop();

	void UpdateScFailureState();
	
	void RefreshConRelay()
	{
		conRelay.Refresh();
	}
	
	void RefreshRelays()
	{
		disRelay.Refresh();
		lockRelay.Refresh();
	}
	
	Relay lockRelay;
	Relay conRelay;
	Relay disRelay;
protected:
	
	bool occur;
	bool unLock;
};
