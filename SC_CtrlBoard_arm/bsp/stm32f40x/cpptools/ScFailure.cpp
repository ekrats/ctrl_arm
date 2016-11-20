#include "ScFailure.h"

void ScFailure::OccurNow()
{
	occur = true;
}

void ScFailure::OccurStop()
{
	conRelay.Stop();
	occur = false;
}

void ScFailure::OccurredClear()
{
	OccurStop();
	if (lockRelay.GetResult())
	{
		unLock = true;
	}
}

void ScFailure::UpdateScFailureState()
{
	if (occur)
	{
		conRelay.Start();
		disRelay.Stop();
		if (conRelay.GetResult())
		{
			Encounter();
			lockRelay.Start();
		}
	}
	else if (unLock)
	{
		if (IsOccurred() && isLocked==false)
		{
			disRelay.Start();
			if (disRelay.GetResult())
			{
				Reset();
				lockRelay.Stop();
				unLock = false;
			}
		}
	}
}
