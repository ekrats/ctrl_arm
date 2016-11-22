#pragma once
#include "itransducer.h"
class ACTransducer :
    public ITransducer
{
public:
    ACTransducer(int length = 60);

    ~ACTransducer(void);
    ACTransducer(const ITransducer & transducer) : ITransducer(transducer)
    {
    }

    ACTransducer(const ITransducer * const transducer) : ITransducer(transducer)
    {
    }

    ACTransducer &operator=(const ITransducer & transducer)
    {
        if (this != &transducer)
        {
            Init(&transducer);
        }
        return *this;
    }

    virtual void Update();
    virtual int GetRealValue();
    virtual int GetAcqValue(int realValue);
    void SetPointsPerSycle(int points);
private:
    int sumSquare;
    float fLength;
    int validValue;
};

