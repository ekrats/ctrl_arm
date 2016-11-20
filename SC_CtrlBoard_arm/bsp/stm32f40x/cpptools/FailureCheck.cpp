#pragma once
#include "FailureCheck.h"

void FailureCheck::ResetFaulture()
{
    if(!listLock)
    {
        listLock = true;
        failureList.Begin();
        Failure * tmp = failureList.Next();
        while (tmp != NULL)
        {
            tmp->Reset();
            tmp = failureList.Next();
        }
        fault = 0;
        listLock = false;
    }
}

void FailureCheck::UnlockFaulture()
{
    if(!listLock)
    {
        listLock = true;
        failureList.Begin();
        Failure * tmp = failureList.Next();
        while (tmp != NULL)
        {
            tmp->Unlock();
            tmp = failureList.Next();
        }
        faultLock = 0;
        listLock = false;
    }
}


void FailureCheck::UpdateFaultState()
{
    if(!listLock)
    {
        listLock = true;
        failureList.Begin();
        Failure * tmp = failureList.Next();
        int i = 0;

        while (tmp != NULL)
        {
            if(tmp->IsOccurred())
            {
               fault |= (1 << i);
            }
            if(tmp->IsLocked())
            {
                faultLock |= (1 << i);
            }
            ++i;
            tmp = failureList.Next();
        }
        listLock = false;
    }
}
