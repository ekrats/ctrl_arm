#include "Sc.h"

Sc::Sc(void)
{
	sensorList.Add(&inVolt);
	sensorList.Add(&cFlyVolt);
	sensorList.Add(&outVolt);
	sensorList.Add(&batCurr);
	sensorList.Add(&iOutPeak);
	
}


