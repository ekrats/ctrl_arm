#include "Contactor.h"

void Contactor::NormalOpenLogic()
{
    int input = filterValue;
    if (input != *destination)
    {
        Start();
    }
    else
    {
        Stop();
    }

    isAbnormal = GetResult();
    isWithFeedback = input;
}

void Contactor::NormalCloseLogic()
{
    int input = filterValue;
    if (input == *destination)
    {
        Start();
    }
    else
    {
        Stop();
    }

    isAbnormal = GetResult();
    isWithFeedback = input;
}

