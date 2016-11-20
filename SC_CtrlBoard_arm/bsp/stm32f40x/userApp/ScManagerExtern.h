#ifndef __ScManagerExtern__h__
#define __ScManagerExtern__h__

#include "record.h"
#include "stm32f4xx_can.h"
#include "DataStruct.h"

#ifdef __cplusplus
extern "C" {
#endif

void sc_init(void);
void state_control(void);
void io_output(void);
void stm32_hw_io_on(int led);
void stm32_hw_io_off(int led);
void stm32_hw_io_toggle(int led);
void relays_refresh(void);
void task_entry(void);
void status_updata(void);
void * GetShareDataPtr(void);
void gbch_init(void);
void can_led(void);
void can_fault(void);
void SendCan(uint8_t frame);					//CANÅ¼·¢Ö¡·¢ËÍº¯Êý
void iap_send(uint8_t frame, uint16_t lenth);
void check_file_call_back(int state_code);
void CanApp_TriggerStream_Server(void);
void Bsp_can_send_trigger_event(void);
void can_rx_interface(uint8_t port, CanRxMsg* msg);
void can_rx_thread_init(void);
void can_thread_init(void);
void  System_hw_timerInit(void);
int GetEvent(void);
void PutEvent(int e);

#ifdef __cplusplus
}
#endif

#endif
