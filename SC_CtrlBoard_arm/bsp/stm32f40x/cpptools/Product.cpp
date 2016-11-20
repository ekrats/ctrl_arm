#include "Product.h"

void Product::RefreshTransducerData(List<ITransducer*> &list)
{
    list.Begin();
    ITransducer * tmp = list.Next();
    while ((tmp) != NULL)
    {
        tmp->Update();
        tmp = list.Next();
    }
}

