#include "Failure.h"

Failure::Failure( int times /*= 3*/ )
{
    this->maxTimes = times;
    isOccurred = false;
    isLocked = false;
}

void Failure::Encounter()
{
    if (!isOccurred)
    {
        ++presentTimes;
    }
    isOccurred = true;
    
    isLocked = (presentTimes >= maxTimes);
}

void Failure::Degrade()
{
    if (presentTimes > 0)
    {
        --presentTimes;
    }
}

void Failure::Unlock()
{
    isLocked = false;
    presentTimes = 0;
    Reset();
}

