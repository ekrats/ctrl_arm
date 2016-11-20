/*!
 * This is a part of the TongYe Foundation Classes C++ library.
 * COPYRIGHT (C) 1999-2014, TongYe Corporation All rights reserved.
 * @file    Contactor.h
 * @brief   This file contains definition of class Contactor
 * Change Logs:
 * Date           Author       Notes
 * 2014/10/23     Coase        the first version
 * 2015/05/25     Coase        current version
 * modify IsClosed function , return feedback not cmd
 */

#pragma  once
#include "OnOff.h"
#include "Relay.h"
#include "IOTransducer.h"

class Contactor : public OnOff , public IOTransducer, public Relay
{
public:

    // ժҪ: 
    //     ʹ��ָ�����������ȳ�ʼ�� Contactor �����ʵ����
    //
    // ����:
    //   length
    //     ���������ȣ�Ĭ��Ϊ60
    Contactor(int length = 60) : IOTransducer(length)
    {
        NormalOpen();
        isAbnormal = false;
    }


    ~Contactor()
    {
    }

    // ժҪ: 
    //     ���ýӴ�������Ϊ����ʽ�Ӵ�����
    inline void NormalOpen()
    {
        isNormalOpen = true;
    }

    // ժҪ: 
    //     ���ýӴ�������Ϊ����ʽ�Ӵ�����
    inline void NormalClose()
    {
        isNormalOpen = false;
    }

    // ժҪ: 
    //     �Ӵ����Ƿ��й��ϡ�
    //
    // ���ؽ��: 
    //     �Ӵ����Ĺ���״̬��
    inline bool IsFault()
    {
        return isAbnormal;
    }

    // ժҪ: 
    //     �Ӵ����Ƿ��бպϷ�����
    //
    // ���ؽ��: 
    //     �Ӵ����ıպ�״̬��
    inline bool IsWithFeedback()
    {
        return isWithFeedback;
    }

    // ժҪ: 
    //     �Ӵ����Ƿ�պϡ�
    //
    // ���ؽ��: 
    //     �Ӵ����ıպ�״̬��
    inline bool IsClosed()
    {
        return filterValue;
    }

    // ժҪ: 
    //     ���ݸ����ķ����ź�Դָ���Լ����ղ���Ŀ��ָ���ʼ���Ӵ�����
    //
    // ����: 
    //   feedBackAddress:
    //     �����ź�����Դָ�롣
    //   destination:
    //     �Ӵ������ղ�����Ŀ���ַָ�롣
    //   checkTime:
    //     �����������ʱʱ�䣬��λ��MS��
    void Init(int * feedBackAddress, int * destination,
        int checkTime)
    {
        this->destination = destination;
        IOTransducer::Init(feedBackAddress);
        SetRelayTime(checkTime);
    }

    // ժҪ: 
    //     ��⴫�����Ƿ��й��ϡ�
    void Check()
    {
        if (isNormalOpen)
        {
            NormalOpenLogic();
        }
        else
        {
            NormalCloseLogic();
        }
    }

private:
    // ժҪ: 
    //     �Ƿ��ǳ���ʽ�Ӵ�����
    bool isNormalOpen;

    // ժҪ: 
    //     �������Ƿ�������
    bool isAbnormal;

    // ժҪ: 
    //     �Ƿ��з����źš�
    bool isWithFeedback;
    bool padding;

    // ժҪ: 
    //     ����ʽ�Ӵ�������߼���
    void NormalOpenLogic();

    // ժҪ: 
    //     ����ʽ�Ӵ�������߼���
    void NormalCloseLogic();
};

/******************* (C) COPYRIGHT 2014 TongYe **********END OF FILE****/
