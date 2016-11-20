#ifndef _logic_app_h_
#define _logic_app_h_

#include <rtthread.h>
#include "stm32f4xx.h"

void logic_thread_creat(uint8_t priority);
void adio_thread_creat(uint8_t priority);

#endif
