#include "FailureRelay.h"

void FailureRelay::Reset()
{
    presentTime = 0;
    m_result = false;
    condition = false;
}

void FailureRelay::Start()
{
    condition = true;
}

void FailureRelay::Stop()
{
    Reset();
}

void FailureRelay::Refresh()
{
    if (condition)
    {
        if(presentTime >= delayTime)
        {
            m_result = true;
        }
        else
        {
            ++presentTime;
        }
    }
}

void FailureRelay::SetRelayTime( int milliseconds )
{
    this->delayTime = milliseconds / cycleTime;
}

void FailureRelay::SetStatusT(void)
{
	if (frontStatus == false)
	{
		condition = true;
	}
	frontStatus = true;
}

void FailureRelay::SetStatusF(void)
{
	frontStatus = false;
}
