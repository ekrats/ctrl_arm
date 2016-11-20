#include "RelayModule.h"

void RelayModule::Reset()
{
    if(!listLock)
    {
        listLock = true;
        list.Begin();
        Relay * tmp = list.Next();
        while (tmp != NULL)
        {
            tmp->Reset();
            tmp = list.Next();
        }
        listLock = false;
    }
}

void RelayModule::Refresh()
{
    if(!listLock)
    {
        listLock = true;
        list.Begin();
        Relay * tmp = list.Next();
        while (tmp != NULL)
        {
            tmp->Refresh();
            tmp = list.Next();
        }
        listLock = false;
    }
}

void RelayModule::Register( Relay * relay )
{
    list.Add(relay);
}
