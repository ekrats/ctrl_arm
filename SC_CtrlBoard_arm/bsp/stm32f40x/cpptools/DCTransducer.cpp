#include "DCTransducer.h"

int DCTransducer::GetAcqValue(int realValue)
{
    return (realValue << 11) / range;
}

int DCTransducer::GetRealValue()
{
    return ((filterValue * range) >> 11);
}

int DCTransducer::GetIRealValue()
{
    return ((instantaneousValue * range) >> 11);
}

int DCTransducer::GetAverageRealValue()
{
    return ((sum / 4 * range) / length / 512);
    //return ((sum * range) / length / 2048);
}

void DCTransducer::Update()
{
    instantaneousValue = *dataSource;
    sum = sum + instantaneousValue - buffer[index];
    buffer[index] = instantaneousValue;
    filterValue = Filter(filterValue, instantaneousValue, filterPara);
    validValue = GetRealValue();
    UpdateIndex();
}
