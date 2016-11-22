#include "logicApp.h"
#include <rtthread.h>
#include "stm32f4xx.h"
#include "ScManagerExtern.h"
#include "flash_storage_at45dbxx.h"
#include "static_mem.h"
#include "ad.h"
#include "pcmaster.h"
#include "fsmc.h"
#include "record.h"
#include "DataStruct.h"

ALIGN(RT_ALIGN_SIZE)
struct rt_thread thread_logic;

struct rt_semaphore sem_ad_task;

static void rt_thread_entry_logic(void* parameter)
{
	ScData * p = (ScData *)GetShareDataPtr();
	storage_map* map = get_storage_map();
	int num = (map[RECORD_FAULT].end_page + 1 - map[RECORD_FAULT].start_page) / map[RECORD_FAULT].page_per_record;
	if (map[RECORD_FAULT].current_id < num)
	{
	  num = map[RECORD_FAULT].current_id;
	}
	p->output.data.faultCnt = num;
	sc_init();
	flash_record_init();
	rt_thread_delay(2000);
	p->status.status_bit.rParFlag = true;
	p->status.status_bit.rAdjFlag = true;
	
    while(1)
    {
		state_entry();
		UserRecord();
		rt_thread_delay(10);
    }
}

ALIGN(RT_ALIGN_SIZE)
static rt_uint32_t thread_ad_stack[1024];
struct rt_thread thread_ad;
static void rt_thread_entry_ad(void* parameter)
{
	adc_init();
	rt_sem_init(&sem_ad_task, "ad", 1, 1);
    rt_thread_delay(1000);
    
    ADC_SoftwareStartConv(ADC2);
	while(1)
    {
		if(-RT_ETIMEOUT == rt_sem_take(&sem_ad_task, 5))
        {
            ADC_SoftwareStartConv(ADC1);
            adc_i_update();
        }
		adc_update();
		bat_entry();
		//update_fpga_data(fpga_buffer);
		pcmaster_record();
    }
}

void logic_thread_creat(uint8_t priority)
{
	 //------- init ad thread
    rt_thread_init(&thread_logic,
        "logic",
        rt_thread_entry_logic,
        RT_NULL,
        static_malloc(0x800),
        0x800, priority, 10);
    rt_thread_startup(&thread_logic);
}

void adio_thread_creat(uint8_t priority)
{
	//------- init ad thread
    rt_thread_init(&thread_ad,
        "ad",
        rt_thread_entry_ad,
        RT_NULL,
        thread_ad_stack,//static_malloc(0x800)
        1024 * 4, priority, 10);
    rt_thread_startup(&thread_ad);
}
