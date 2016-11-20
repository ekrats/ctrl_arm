#pragma once
#include "itransducer.h"
class NTC :
    public ITransducer
{
private:
    int sum;
    void InitBuffer();
public:
    NTC(int length = 60) : ITransducer()
    {
        this->length = length;
        buffer = new int[length];
        sum = 0;
    }
    ~NTC(void)
    {

    }
    NTC(const ITransducer & transducer) : ITransducer(transducer)
    {
    }

    NTC(const ITransducer * const transducer) : ITransducer(transducer)
    {
    }

    NTC &operator=(const ITransducer & transducer)
    {
        if (this != &transducer)
        {
            Init(&transducer);
        }
        return *this;
    }

    virtual int GetRealValue();
    virtual int GetAcqValue(int realValue);
    virtual void Update();
};

