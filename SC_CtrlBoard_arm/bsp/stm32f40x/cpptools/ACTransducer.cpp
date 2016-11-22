#include "ACTransducer.h"
#include "math.h"

ACTransducer::ACTransducer(int length)
{
    this->length = length;
    buffer = new int[length];
    sumSquare = 0;
    fLength = length;
}

ACTransducer::~ACTransducer(void)
{
    delete buffer;
}

void ACTransducer::Update()
{
    int lastInstaneousValue = buffer[index];
    instantaneousValue = *dataSource;
    sumSquare += ((instantaneousValue * instantaneousValue) 
        - (lastInstaneousValue * lastInstaneousValue));
    buffer[index] = instantaneousValue;
    validValue = GetRealValue();
    UpdateIndex();
}

int ACTransducer::GetRealValue()
{
    return (int)sqrt(sumSquare / fLength) * range / 2048;
}

int ACTransducer::GetAcqValue(int realValue)
{
    return 0;
}

