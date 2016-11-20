/*!
 * This is a part of the TongYe Foundation Classes C++ library.
 * COPYRIGHT (C) 1999-2014, TongYe Corporation All rights reserved.
 * @file    FailureCheck.h
 * @brief   This file contains definition of class FailureCheck
 * Change Logs:
 * Date           Author       Notes
 * 2014/08/01     Coase        the first version
 */



#pragma once
#include "Failure.h"
#include "RelayModule.h"

class FailureCheck
{
public:
    // ժҪ: 
    //     ��ʼ�� FailureCheck �����ʵ����
    FailureCheck()
    {
        listLock = 0;
    }

    // ժҪ: 
    //     ���� FailureCheck �ࡣ
    ~FailureCheck()
    {
    }

    // ժҪ: 
    //     ���ټ�⡣
    virtual void FastCheck() = 0;

    // ժҪ: 
    //     ���ټ�⡣
    virtual void SlowCheck() = 0;

    // ժҪ: 
    //     ���ϼ��ģ���ʼ������ʼ�������б������й��ϼ����б�
    virtual void FaultCheckModuleInit() = 0;

    // ժҪ: 
    //     ���ù����б��е����й��ϡ�
    virtual void ResetFaulture();

    // ժҪ: 
    //     ���������б��е����й��ϣ��������б��еĹ��ϵ��ѷ����������㡣
    virtual void UnlockFaulture();
    
    // ժҪ: 
    //     ���¹��ϵ�״̬��faultֵ�У���������Ӧ��bitλΪ��������б��˳��
    //     ���¹��ϵ�����״̬��faultLockֵ�У���������Ӧ��bitλΪ��������б��˳��
    virtual void UpdateFaultState();


    // ժҪ: 
    //     ��ȡ�����б������й���״̬�ļ���ֵ��
    //
    // ���ؽ��:
    //     �����б������й���״̬�ļ���ֵ��
    inline unsigned int GetFault()
    {
        return fault;
    }

    // ժҪ: 
    //    ��ȡ�����б������й�������״̬�ļ���ֵ��
    //
    // ���ؽ��:
    //     �����б������й�������״̬�ļ���ֵ��
    inline unsigned int GetLock()
    {
        return faultLock;
    }

    // ժҪ: 
    //    ����ǰ�����Ĺ���������
    inline void Lock()
    {
        faultLock = fault;
    }
	
    // ժҪ: 
    //    ����ָ�����ϡ�
    //
    // ����:
    //   value
    //     ָ��������λ��
    inline void Lock(unsigned int value)
    {
        faultLock |= value;
    }
	
    // ժҪ: 
    //    ����ָ�����ϡ�
    //
    // ����:
    //   value
    //     ָ���Ľ���λ��
    inline void Unlock(unsigned int value)
    {
        faultLock &= ~value;
    }

protected:
    // ժҪ: 
    //    �����б����ڴ���û�����Ĺ��ϣ��Ա���ͳһ������
    List<Failure*> failureList;

    // ժҪ: 
    //    �����б������й��ϵĹ���״̬���ϡ�
    //    fault�е�BITλ˳��Ϊ��������б��˳��
    unsigned int fault;

    // ժҪ: 
    //    �����б������й��ϵĹ�������״̬���ϡ�
    //    faultLock�е�BITλ˳��Ϊ��������б��˳��
    unsigned int faultLock;

    // ժҪ: 
    //    �����б����������б�ʱ������������Ϻ������
    bool listLock;
};

/******************* (C) COPYRIGHT 2014 TongYe **********END OF FILE****/
