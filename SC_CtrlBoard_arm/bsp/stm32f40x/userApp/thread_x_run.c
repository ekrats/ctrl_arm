#include "thread_x_run.h"
#include <rtthread.h>
#include "stm32f4xx.h"
#include "board.h"
#include "ScManagerExtern.h"
#include "logicApp.h"
#include "io.h"

/*******************************************************************************
* Function Name  : rt_cycle_thread_entry
* Description    : 
* Input          :  .
* Output         : None
* Return         : None
*******************************************************************************/
__IO uint8_t enDog = 1;
static void rt_cycle_thread_entry(void* parameter)
{
	static uint8_t timeDelay = 0;
	//------------------------------------------------------
	// 系统指示灯初始化
	//------------------------------------------------------
	bsp_io_init();
	while(1)
	{
		rt_thread_delay(RT_TICK_PER_SECOND/100);

#if IWDG_EN
		if(enDog)	IWDG_ReloadCounter();
#endif
		if(++timeDelay % 50 == 0)
		{
			timeDelay = 0;
			//------------------------------------------------------
			// 系统运行灯
			//------------------------------------------------------
			stm32_hw_io_toggle(1);
		}
	}
}

static void cycle_thread_creat(uint8_t priority)
{
	rt_thread_t tid;
	
	//------- init thread
	tid = rt_thread_create("cycle",
		rt_cycle_thread_entry, RT_NULL,
		512, priority, 10);
	//------- start up thread
	if(tid != RT_NULL)
		rt_thread_startup(tid);
}

void rt_app_thread_x(void)
{
	gbch_init();
	
	logic_thread_creat(12);
	
	cycle_thread_creat(30);
	
	adio_thread_creat(2);
}
