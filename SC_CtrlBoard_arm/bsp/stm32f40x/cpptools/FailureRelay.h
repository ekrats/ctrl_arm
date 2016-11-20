/*!
 * This is a part of the TongYe Foundation Classes C++ library.
 * COPYRIGHT (C) 1999-2014, TongYe Corporation All rights reserved.
 * @file    Relay.h
 * @brief   This file contains definition of class Relay
 * Change Logs:
 * Date           Author       Notes
 * 2014/04/14     Coase        the first version
 */



#pragma once
class FailureRelay
{
public:
    /**
     * @brief constructed function
     *
     * @param milliseconds delay time in milliseconds
     * @param cycleTime  execute cycle
     *
     * @return NULL
     */
    FailureRelay(int milliseconds = 1000, int cycleTime = 100)
    {
        if (cycleTime > 0)
        {
            this->cycleTime = cycleTime;
        }
        else
        {
            this->cycleTime = 10;
        }
        this->delayTime = milliseconds / cycleTime;
        Reset();
    }
    ~FailureRelay(void)
    {
        ;
    }

    // ժҪ: 
    //     ��ȡ��ʱ�������״̬��
    //
    // ���ؽ��:
    //    ��ʱ�������״̬��
    inline bool GetResult()
    {
        return m_result;
    }

    /**
     * @brief This function update the relay
     *
     * @return void
     */
    void Refresh();

    // ժҪ: 
    //     ���ü�ʱ����
    void Reset();
    /**
     * @brief This function reset delay time
     *
     * @param milliseconds delay time in milliseconds
     *
     * @return void
     */
    void SetRelayTime(int milliseconds);

    // ժҪ: 
    //     ��ʼ��ʱ��
    void Start();

    // ժҪ: 
    //     ֹͣ��ʱ��
    void Stop();
	
	void SetStatusT(void);
	
	void SetStatusF(void);
private:
	// ժҪ: 
    //     ��ǰ����״̬��
	int frontStatus;

    // ժҪ: 
    //     ��ǰ����ʱ�䡣
    int presentTime;

    // ժҪ: 
    //     ��ʱʱ�䡣
    int delayTime;

    // ժҪ: 
    //     ѭ��ִ�м��ʱ�䡣
    int cycleTime;

    // ժҪ: 
    //     ��ʱ��������������
    bool condition;

    // ժҪ: 
    //     ��ʱ�������״̬��
    bool m_result;
};

/******************* (C) COPYRIGHT 2014 TongYe **********END OF FILE****/
