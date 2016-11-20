#include "Sc.h"

Sc::Sc(void)
{
	sensorList.Add(&input1Current);
	sensorList.Add(&input2Current);
	sensorList.Add(&outCurrent);
	sensorList.Add(&inputAc1Voltage);
	sensorList.Add(&inputBusVoltage);
	sensorList.Add(&output1Voltage);
	sensorList.Add(&output2Voltage);
	sensorList.Add(&output3Voltage);
	
	pt100List.Add(&module1Temp);
	pt100List.Add(&module2Temp);
	pt100List.Add(&cap1Temp);
	pt100List.Add(&cap2Temp);
	
	ioList.Add(&cabinet);
	ioList.Add(&inFuse1);
	ioList.Add(&inFuse2);
	ioList.Add(&outFuse1);
	ioList.Add(&outFuse2);
	ioList.Add(&outFuse3);
	ioList.Add(&outFuse4);
	ioList.Add(&out1Knife);
	ioList.Add(&out2Knife);
	ioList.Add(&out3Knife);
	ioList.Add(&outLockFuse);
	
	ContactList.Add(&mainCon1);
	ContactList.Add(&mainCon2);
	ContactList.Add(&preCon);
	ContactList.Add(&outCon1);
	ContactList.Add(&outCon2);
	ContactList.Add(&outCon3);
}
