#include "PID.h"

PID::PID(void)
{
    derivativeOut = 0;
    integralOut = 0;
    outPreSat = 0;
    proportionalOut = 0;
    saturatedErr = 0;
}

PID::~PID(void)
{
}

void PID::Reset(int out)
{
    derivativeOut = 0;
    integralOut = out;
}


void PID::Reset()
{
    derivativeOut = 0;
    integralOut = 0;
    outPreSat = 0;
    proportionalOut = 0;
    saturatedErr = 0;
}

void PID::Update()
{
    derivativeOut = err * derivativeGain;
    err = reference - feedback;
    proportionalOut = proportionalGain * err;
    integralOut += err * integralGain;
    if (integralOut > 40000000)
    {
        integralOut = 40000000;
    }
    else if (integralOut < -40000000)
    {
        integralOut = -40000000;
    }
    
    out = (proportionalOut + integralOut - derivativeOut);
}


void PID::Update(int x)
{
    err = reference - feedback;
    proportionalOut = proportionalGain * err;
    integralOut = integralOut
        + (integralGain * proportionalOut
        + integralCorrection * saturatedErr) / INTEGER_MAGNIFICATIONS;
    derivativeOut = derivativeGain * (proportionalOut - preOut) / INTEGER_MAGNIFICATIONS;

    outPreSat = proportionalOut + integralOut + derivativeOut;

    outPreSat = (outPreSat + HALF_INTEGER_MAGNIFICATIONS) / INTEGER_MAGNIFICATIONS;

    out = outPreSat;
    if (outPreSat > outMax)
    {
        out = outMax;
    }
    else if (outPreSat < outMin)
    {
        out = outMin;
    }

    saturatedErr = (out - outPreSat) * INTEGER_MAGNIFICATIONS;
    preOut = proportionalOut;
}
