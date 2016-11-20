/***************************************************************************

Copyright (C), 1999-2015, Tongye Tech. Co., Ltd.

* @file           Bsp_rtc.c
* @author         SCH-Team
* @version        V1.0.0
* @date           2015-03-11
* @brief          Timer manage

History:          // Revision Records

<Author>             <time>       <version >            <desc>

SCH-Team           2015-03-11       V1.0.0            Timer manage

***************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include <rtthread.h>
#include <stm32f4xx.h>
#include "Bsp_rtc.h"
#include "rtc.h"
#include "time.h"


SYS_TIME_RTC TIME_RTC = {0};
RUN_TIME_STYP run_time = {0};
/*******************************************************************************
* Function Name  : void Bsp_get_time(void)
* Description    : 获取系统RTC时钟
*                   
* Input          : - none.
* Output         : None
* Return         : None
*******************************************************************************/

void Bsp_get_time(void)
{
	time_t now;
	struct tm* ti;

	/* get current time */
	time(&now);

	ti = localtime(&now);

	TIME_RTC.Year  = ti->tm_year;
	TIME_RTC.Month = ti->tm_mon + 1;
	TIME_RTC.Day   = ti->tm_mday;
//	TIME_RTC.Week  = ti->tm_wday;
	TIME_RTC.Hour  = ti->tm_hour;
	TIME_RTC.Min   = ti->tm_min;
	TIME_RTC.Sec   = ti->tm_sec;
}

void Bsp_set_time(TIME_STYP * time)
{
	uint16_t  Year = time->year + 1900;
	uint8_t  Month = time->mon;
	uint8_t  Day = time->mday;
	uint8_t  Hour = time->hour;
	uint8_t  Min = time->min;
	uint8_t  Sec = time->sec;
	
	set_date(Year, Month, Day);
	set_time(Hour, Min, Sec);
}

int sec = 0;
void Bsp_get_RunTime(void)
{
	//500毫秒进一次
	if (++sec == 120)
	{
		sec = 0;
		run_time.min++;
		if (run_time.min == 60)
		{
			run_time.min = 0;
			run_time.hour++;
			if (run_time.hour == 24)
			{
				run_time.hour = 0;
				run_time.day++;
				if (run_time.day == 0xffff)
					run_time.day = 0;
			}
		}
	}		
}

void * get_rtc_t(void)
{
    SYS_TIME_RTC * p = &TIME_RTC;
	return p;
}

#ifdef RT_USING_FINSH
#include "finsh.h"

void list_run_time()
{
    rt_kprintf("%02d-%02d-%02d\n", run_time.day, run_time.hour, run_time.min);
}

FINSH_FUNCTION_EXPORT(list_run_time, show run time.)
#endif
/******************* (C) COPYRIGHT 2012 Shenzhen Tongye *****END OF FILE****/


