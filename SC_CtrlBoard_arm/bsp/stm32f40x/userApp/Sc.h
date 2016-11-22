#pragma once
#include "DCTransducer.h"
#include "List.h"
#include "NTC.h"

class Sc
{
public:
    Sc(void);
    ~Sc(void)
    {
    }
protected:
	List<ITransducer*> sensorList;
public:
	DCTransducer inVolt;
	DCTransducer cFlyVolt;
	DCTransducer outVolt;
	DCTransducer batCurr;
	DCTransducer iOutPeak;

	NTC igbt1Temp;
	NTC igbt2Temp;

private:
	static Sc* _instance;

public:
	static Sc* Instance(void);
};
