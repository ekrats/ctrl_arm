#include "ScManager.h"
#include "DataStruct.h"

static ScFailure canLostWarn(2000,200,1000);
static ScFailure m1TempFailWarn(2000,200,6000);
static ScFailure m2TempFailWarn(2000,200,6000);
static ScFailure cap1TempFailWarn(2000,200,6000);

static ScFailure cap2TempFailWarn(2000,200,6000);
static ScFailure mainCon1FbWarn(2000,1000,6000);
static ScFailure mainCon2FbWarn(2000,1000,6000);
static ScFailure preConFbWarn(2000,1000,6000);

static ScFailure outCon1FbWarn(2000,1000,6000);
static ScFailure outCon2FbWarn(2000,1000,6000);
static ScFailure outCon3FbWarn(2000,1000,6000);
static ScFailure ac1VoltFailWarn(2000,600000,6000);

static ScFailure busVoltFailWarn(2000,200,6000);
static ScFailure m1CurrFailWarn(2000,200,6000);
static ScFailure m2CurrFailWarn(2000,200,6000);
static ScFailure ac1VoltUnderWarn(2000,200,6000);

static ScFailure dcdc1OnFailWarn(2000,3000,6000);
static ScFailure dcdc2OnFailWarn(2000,3000,6000);
static ScFailure dcdc3OnFailWarn(2000,3000,6000);
static ScFailure dcdc4OnFailWarn(2000,3000,6000);

static ScFailure inFuse1FailFault(0,1000,1000);
static ScFailure inFuse2FailFault(0,1000,1000);
static ScFailure ac1VoltOverFault(180000,500,1000);
static ScFailure busOverFault(180000,500,1000);

static ScFailure busUnderFault(180000,500,1000);
static ScFailure m1TempOverFault(180000,1000,1000);
static ScFailure m2TempOverFault(180000,1000,1000);
static ScFailure cap1TempOverFault(180000,1000,1000);

static ScFailure cap2TempOverFault(180000,1000,1000);
static ScFailure m1CurrFailFault(180000,1000,1000);
static ScFailure m2CurrFailFault(180000,1000,1000);
static ScFailure m1CurrOverFault(180000,200,1000);

static ScFailure m2CurrOverFault(180000,200,1000);

static ScFailure dc1CurrOverFault(180000,200,1000);
static ScFailure dc1VoltOverFault(180000,500,1000);
static ScFailure dc1VoltFailFault(180000,2000,1000);
static ScFailure out1FuseFailFault(0,500,1000);

static ScFailure dc1OffFailFault(180000,2000,1000);
static ScFailure dc1FailFault(180000,200,1000);
static ScFailure out2FuseFailFault(0,500,1000);
static ScFailure dc2OffFailFault(180000,2000,1000);

static ScFailure dc2FailFault(180000,200,1000);
static ScFailure dc2CurrOverFault(180000,200,1000);
static ScFailure dc2VoltOverFault(180000,200,1000);
static ScFailure dc2VoltFailFault(180000,200,1000);

static ScFailure dc3VoltOverFault(180000,200,1000);
static ScFailure dc3VoltFailFault(180000,200,1000);
static ScFailure out3FuseFailFault(0,500,1000);
static ScFailure dc3OffFailFault(180000,2000,1000);

static ScFailure dc3FailFault(180000,200,1000);
static ScFailure out4FuseFailFault(0,500,1000);
static ScFailure dc4OffFailFault(180000,2000,1000);
static ScFailure dc4FailFault(180000,200,1000);

void ScManager::FaultCheckModuleInit(void)
{
	warnList.Add(&canLostWarn);
	warnList.Add(&m1TempFailWarn);
	warnList.Add(&m2TempFailWarn);
	warnList.Add(&cap1TempFailWarn);
	
	warnList.Add(&cap2TempFailWarn);
	warnList.Add(&mainCon1FbWarn);
	warnList.Add(&mainCon2FbWarn);
	warnList.Add(&preConFbWarn);
	
	warnList.Add(&outCon1FbWarn);
	warnList.Add(&outCon2FbWarn);
	warnList.Add(&outCon3FbWarn);
	warnList.Add(&ac1VoltFailWarn);
	
	warnList.Add(&busVoltFailWarn);
	warnList.Add(&m1CurrFailWarn);
	warnList.Add(&m2CurrFailWarn);
	warnList.Add(&ac1VoltUnderWarn);
	
	warnList.Add(&dcdc1OnFailWarn);
	warnList.Add(&dcdc2OnFailWarn);
	warnList.Add(&dcdc3OnFailWarn);
	warnList.Add(&dcdc4OnFailWarn);
	
	faultList.Add(&inFuse1FailFault);
	faultList.Add(&inFuse2FailFault);
	faultList.Add(&ac1VoltOverFault);
	faultList.Add(&busOverFault);

	faultList.Add(&busUnderFault);
	faultList.Add(&m1TempOverFault);
	faultList.Add(&m2TempOverFault);
	faultList.Add(&cap1TempOverFault);

	faultList.Add(&cap2TempOverFault);
	faultList.Add(&m1CurrFailFault);
	faultList.Add(&m2CurrFailFault);
	faultList.Add(&m1CurrOverFault);

	faultList.Add(&m2CurrOverFault);
}

void ScManager::RefreshFaultList(void)
{
    if(!faultListLock)
    {
        faultListLock = true;
        faultList.Begin();
        ScFailure * tmp = NULL;

        while ((tmp = faultList.Next()) != NULL)
        {
            tmp->UpdateScFailureState();
        }
        faultListLock = false;
    }
}

void ScManager::UpdateFaultState(void)
{
    if(!faultListLock)
    {
        faultListLock = true;
        faultList.Begin();
        Failure * tmp = NULL;
        int i = 0;

        while ((tmp = faultList.Next()) != NULL)
        {
            if(tmp->IsOccurred())
            {
                shareData.output.fault.fault_u32 |= (1 << i);
            }
			else
			{
				shareData.output.fault.fault_u32 &= ~(1 << i);
			}
            if(tmp->IsLocked())
            {
                //sharedData.output.state.status_bit.defect = 1;
            }
            ++i;
        }
        faultListLock = false;
    }
}

void ScManager::RefreshWarnList(void)
{
	if(!warnListLock)
    {
        warnListLock = true;
        warnList.Begin();
        ScFailure * tmp = NULL;

        while ((tmp = warnList.Next()) != NULL)
        {
            tmp->UpdateScFailureState();
        }
        warnListLock = false;
    }
}

void ScManager::UpdateWarnState(void)
{
    if(!warnListLock)
    {
        warnListLock = true;
        warnList.Begin();
        Failure * tmp = NULL;
        int i = 0;

        while ((tmp = warnList.Next()) != NULL)
        {
            if(tmp->IsOccurred())
            {
                shareData.output.warn.warn_u32 |= (1 << i);
            }
			else
			{
				shareData.output.warn.warn_u32 &= ~(1 << i);
			}
            if(tmp->IsLocked())
            {
                //sharedData.output.state.status_bit.defect = 1;
            }
            ++i;
        }
        warnListLock = false;
    }
}

/*******************************************************************************
* Function Name  :  
* Description    :
*
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ScManager::RefreshAdData(void)
{
    if (!sensorListLock)
	{
		sensorListLock = true;
		sensorList.Begin();
		ITransducer * tmp = NULL;
		while ((tmp = sensorList.Next()) != NULL)
		{
			tmp->Update();
		}
		sensorListLock = false;
	}
}
/*******************************************************************************
* Function Name  :  
* Description    :
*
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ScManager::RefreshPt100Data(void)
{
	if (!pt100ListLock)
	{
		pt100ListLock = true;
		pt100List.Begin();
		PT100 * tmp = NULL;
		while ((tmp = pt100List.Next()) != NULL)
		{
			tmp->Update();
		}
		pt100ListLock = false;
	}
}


/*******************************************************************************
* Function Name  :  
* Description    :
*
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ScManager::FaultRelayRun(void)
{
	if(!faultListLock)
    {
        faultListLock = true;
		faultList.Begin();
		ScFailure * tmp = NULL;
		while ((tmp = faultList.Next()) != NULL)
		{
			tmp->RefreshRelays();
		}
		faultListLock = false;
	}
}

/*******************************************************************************
* Function Name  :  
* Description    :
*
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ScManager::WarnRelayRun(void)
{
	if(!warnListLock)
    {
        warnListLock = true;
		warnList.Begin();
		ScFailure * tmp = NULL;
		while ((tmp = warnList.Next()) != NULL)
		{
			tmp->RefreshRelays();
		}
		warnListLock = false;
	}
}

/*******************************************************************************
* Function Name  :  
* Description    :
*
*
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ScManager::RelayRun(void)
{
    relays.Refresh();
	WarnRelayRun();
	FaultRelayRun();
}

void ScManager::SlowCheck(void)
{
	MonitorCheckSlow();
	
	RefreshWarnList();
	RefreshFaultList();
	
	UpdateFaultState();
	UpdateWarnState();
}

void ScManager::MonitorCheckSlow(void)
{
	
}
