#pragma once
#include "itransducer.h"
class PT100 :
    public ITransducer
{
private:
    int sum;
    void InitBuffer();
public:
    PT100(int length = 60) : ITransducer()
    {
        this->length = length;
        buffer = new int[length];
        sum = 0;
    }
    ~PT100(void)
    {

    }
    PT100(const ITransducer & transducer) : ITransducer(transducer)
    {
    }

    PT100(const ITransducer * const transducer) : ITransducer(transducer)
    {
    }

    PT100 &operator=(const ITransducer & transducer)
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

