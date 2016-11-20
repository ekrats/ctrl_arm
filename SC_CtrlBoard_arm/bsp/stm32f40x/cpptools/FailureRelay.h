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

    // 摘要: 
    //     获取计时器的输出状态。
    //
    // 返回结果:
    //    计时器的输出状态。
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

    // 摘要: 
    //     重置计时器。
    void Reset();
    /**
     * @brief This function reset delay time
     *
     * @param milliseconds delay time in milliseconds
     *
     * @return void
     */
    void SetRelayTime(int milliseconds);

    // 摘要: 
    //     开始计时。
    void Start();

    // 摘要: 
    //     停止计时。
    void Stop();
	
	void SetStatusT(void);
	
	void SetStatusF(void);
private:
	// 摘要: 
    //     当前故障状态。
	int frontStatus;

    // 摘要: 
    //     当前计数时间。
    int presentTime;

    // 摘要: 
    //     延时时间。
    int delayTime;

    // 摘要: 
    //     循环执行间隔时间。
    int cycleTime;

    // 摘要: 
    //     延时器的启动条件。
    bool condition;

    // 摘要: 
    //     延时器的输出状态。
    bool m_result;
};

/******************* (C) COPYRIGHT 2014 TongYe **********END OF FILE****/
