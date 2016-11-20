#include "Relay.h"

void Relay::Reset()
{
    presentTime = 0;
    m_result = false;
    condition = false;
}

void Relay::Start()
{
    condition = true;
}

void Relay::Stop()
{
    Reset();
}

void Relay::Refresh()
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

void Relay::SetRelayTime( int milliseconds )
{
    this->delayTime = milliseconds / cycleTime;
}
