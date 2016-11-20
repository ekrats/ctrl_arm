#include "IOTransducer.h"

void IOTransducer::Update()
{
    int tmp = 0;
    instantaneousValue = *dataSource;
    sum = sum + instantaneousValue - buffer[index];
    buffer[index] = instantaneousValue;
    tmp = sum * 10 / length;

    //ÂË²¨Ëã·¨
    if (tmp >= 7)
    {
        filterValue = 1;
    }
    else if (tmp <= 3)
    {
        filterValue = 0;
    }

    UpdateIndex();
}

int IOTransducer::GetAcqValue( int realValue )
{
    return (realValue ? 1 : 0);
}

int IOTransducer::GetRealValue()
{
    return filterValue;
}
