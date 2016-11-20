#pragma once
#include "DCTransducer.h"
#include "IOTransducer.h"
#include "Contactor.h"
#include "List.h"
#include "PT100.h"
#include "can_app_def.h"
#include "DataStruct.h"

class Sc
{
public:
    Sc(void);
    ~Sc(void)
    {
    }
protected:
	List<ITransducer*> sensorList;
	List<ITransducer*> ioList;
	List<Contactor*> ContactList;
	List<PT100*> pt100List;
public:
	DCTransducer input1Current;
	DCTransducer input2Current;
	DCTransducer outCurrent;
	DCTransducer inputAc1Voltage;
	DCTransducer inputBusVoltage;
	DCTransducer output1Voltage;
	DCTransducer output2Voltage;
	DCTransducer output3Voltage;

	PT100 module1Temp;
	PT100 module2Temp;
	PT100 cap1Temp;
	PT100 cap2Temp;

	IOTransducer 	cabinet;
	IOTransducer 	inFuse1;
	IOTransducer 	inFuse2;
	IOTransducer 	outFuse1;
	IOTransducer 	outFuse2;
	IOTransducer 	outFuse3;
	IOTransducer 	outFuse4;
	IOTransducer	out1Knife;
	IOTransducer	out2Knife;
	IOTransducer	out3Knife;
	IOTransducer	outLockFuse;

	Contactor	mainCon1;
	Contactor	mainCon2;
	Contactor	preCon;
	Contactor	outCon1;
	Contactor	outCon2;
	Contactor	outCon3;
	
	bool	sensorListLock;
	bool	ioListLock;
	bool	ContactListLock;
	bool	pt100ListLock;
};
