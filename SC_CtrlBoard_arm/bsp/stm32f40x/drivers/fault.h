#ifndef __fault__h__
#define __fault__h__

#include "stm32f4xx.h"

#ifdef __cplusplus
extern "C" {
#endif
    
#define LOG_RING_LENGTH     20

    void fault_init(void);
    void fault_check(void);
    char* get_fault(int index);
    char* get_log(int index);
    uint32_t get_log_id(void);

#ifdef __cplusplus
}
#endif

#endif

/******************* (C) COPYRIGHT 2015 TongYe **********END OF FILE****/

/*@}*/
