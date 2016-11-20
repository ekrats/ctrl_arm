/*!
 * This is a part of the TongYe Foundation Classes C++ library.
 * COPYRIGHT (C) 1999-2014, TongYe Corporation All rights reserved.
 * @file    Product.h
 * @brief   This file contains definition of class Product
 * Change Logs:
 * Date           Author       Notes
 * 2014/07/19     Coase        the first version
 * 2015/06/05     Jacky        v1.1
 *      add function RefreshTransducerData
 *
 */

#pragma once
#include "Device.h"
#include "RelayModule.h"
#include "ITransducer.h"

class Product : public Device
{
public:
    // ժҪ: 
    //     ��ʼ�� Product �����ʵ����
    Product()
    {
    }
    // ժҪ: 
    //     ɾ�� Product ���ʵ����
    ~Product()
    {
    }

    // ժҪ: 
    //     ���ù������ݡ�
    //
    // ����:
    //   sharedData
    //     ��������ָ��
    virtual void SetSharedData(void * sharedData) = 0;

    // ժҪ: 
    //     ����AD�������б������д��������ݡ�
    inline virtual void RefreshADData()
    {
        this->RefreshTransducerData(sensorList);
    }

    // ժҪ: 
    //     ����IO�������б������д��������ݡ�
    inline virtual void RefreshIOData()
    {
        this->RefreshTransducerData(ioList);
    }

    // ժҪ: 
    //     ˢ��������ʱģ�顣���뱣֤10MS���������ڡ�
    virtual void RefreshRelay()
    {
        relays.Refresh();
    }

    // ժҪ: 
    //     ��ѯ��Ʒ�Ƿ�����
    //
    // ���ؽ��:
    //     ��Ʒ�Ƿ����е�״̬
    inline bool IsRun()
    {
        return isStartEnable;
    }

protected:
    // ժҪ: 
    //     AD�������б�
    List<ITransducer*> sensorList;

    // ժҪ: 
    //     IO�������б�
    List<ITransducer*> ioList;

    // ժҪ: 
    //     ��ʱģ�顣
    RelayModule relays;
private:
    // ժҪ: 
    //     ˢ�´������б������д����������ݡ�
    //
    // ����:
    //   list
    //     �������б�
    void RefreshTransducerData(List<ITransducer*> &list);

};

/******************* (C) COPYRIGHT 2014 TongYe **********END OF FILE****/
