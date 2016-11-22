#include "ScManager.h"
#include "DataStruct.h"

static ScFailure canLost(2000,200,1000);
static ScFailure iBatMinSlow(2000,200,1000);
static ScFailure iBatMaxSlow(2000,200,1000);
static ScFailure uBatMinSlow(2000,200,1000);

static ScFailure uBatMaxSlow(2000,200,1000);
static ScFailure uInMaxSlow(2000,200,1000);
static ScFailure uInMinSlow(2000,200,1000);
static ScFailure igbt1TempOver(2000,200,1000);

static ScFailure igbt2TempOver(2000,200,1000);

static ScFailure iBatMaxHW(2000,200,1000);
static ScFailure uBatMaxHW(2000,200,1000);
static ScFailure iBatMinFast(2000,200,1000);
static ScFailure iBatMaxFast(2000,200,1000);
static ScFailure uBatMinFast(2000,200,1000);
static ScFailure uBatMaxFast(2000,200,1000);
static ScFailure uInMaxFast(2000,200,1000);
static ScFailure uInMinFast(2000,200,1000);
static ScFailure shortCurr(2000,200,1000);
static ScFailure uCfly(2000,200,1000);

void ScManager::FaultCheckModuleInit(void)
{
	slowList.Add(&canLost);
	slowList.Add(&iBatMinSlow);
	slowList.Add(&iBatMaxSlow);
	slowList.Add(&uBatMinSlow);
	
	slowList.Add(&uBatMaxSlow);
	slowList.Add(&uInMaxSlow);
	slowList.Add(&uInMinSlow);
	slowList.Add(&igbt1TempOver);
	slowList.Add(&igbt2TempOver);
	
}

void ScManager::RefreshFastList(void)
{
    if(!fastListLock)
    {
        fastListLock = true;
        fastList.Begin();
        ScFailure * tmp = NULL;

        while ((tmp = fastList.Next()) != NULL)
        {
            tmp->UpdateScFailureState();
        }
        fastListLock = false;
    }
}

void ScManager::UpdateFastState(void)
{
    if(!fastListLock)
    {
        fastListLock = true;
        fastList.Begin();
        Failure * tmp = NULL;
        int i = 0;

        while ((tmp = fastList.Next()) != NULL)
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
        fastListLock = false;
    }
}

void ScManager::RefreshSlowList(void)
{
	if(!slowListLock)
    {
        slowListLock = true;
        slowList.Begin();
        ScFailure * tmp = NULL;

        while ((tmp = slowList.Next()) != NULL)
        {
            tmp->UpdateScFailureState();
        }
        slowListLock = false;
    }
}

void ScManager::UpdateSlowState(void)
{
    if(!slowListLock)
    {
        slowListLock = true;
        slowList.Begin();
        Failure * tmp = NULL;
        int i = 0;

        while ((tmp = slowList.Next()) != NULL)
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
        slowListLock = false;
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
//    if (!sensorListLock)
//	{
//		sensorListLock = true;
//		sensorList.Begin();
//		ITransducer * tmp = NULL;
//		while ((tmp = sensorList.Next()) != NULL)
//		{
//			tmp->Update();
//		}
//		sensorListLock = false;
//	}
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
	if(!fastListLock)
    {
        fastListLock = true;
		fastList.Begin();
		ScFailure * tmp = NULL;
		while ((tmp = fastList.Next()) != NULL)
		{
			tmp->RefreshRelays();
		}
		fastListLock = false;
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
	if(!slowListLock)
    {
        slowListLock = true;
		slowList.Begin();
		ScFailure * tmp = NULL;
		while ((tmp = slowList.Next()) != NULL)
		{
			tmp->RefreshRelays();
		}
		slowListLock = false;
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
	
}

void ScManager::FastCheck(void)
{
	
}
