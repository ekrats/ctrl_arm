/***********************************************************************************/
/* can.h                                                                    */
/*                                                                                 */
/***********************************************************************************/
#ifndef __CAN_H__
#define __CAN_H__
//-----------------------------------------------------------------------------------
#include "stm32f4xx.h"
//-----------------------------------------------------------------------------------

#define  BSP_TIM_RCC           (RCC_APB1Periph_TIM4)
#define  BSP_TIM               (TIM4) 
#define  BSP_TIM_IRQ           (TIM4_IRQn)

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

#define BSP_USE_CAN1      (1)
#define BSP_USE_CAN2      (1)


#define RCC_CANPeriph_PORT (RCC_AHB1Periph_GPIOB)

#if(BSP_USE_CAN1 == 1)
#define RCC_Periph_CAN1    (RCC_APB1Periph_CAN1)
#define CAN1_Pin_RX        (GPIO_Pin_8)
#define CAN1_Pin_TX        (GPIO_Pin_9)
#define CAN1Port           (GPIOB)
#define CAN1_PinSource_RX  (GPIO_PinSource8)
#define CAN1_PinSource_TX  (GPIO_PinSource9)
#endif

#if(BSP_USE_CAN2 == 1)
#define RCC_Periph_CAN2    (RCC_APB1Periph_CAN2)
#define CAN2_Pin_RX        (GPIO_Pin_5)
#define CAN2_Pin_TX        (GPIO_Pin_6)
#define CAN2Port           (GPIOB)
#define CAN2_PinSource_RX  (GPIO_PinSource5)
#define CAN2_PinSource_TX  (GPIO_PinSource6)
#endif

int  System_HW_Can_Init(void);
void can_services_init(void);
uint32_t can_send_buffer(uint8_t port, CanTxMsg* msg);

#ifdef __cplusplus
}
#endif

#endif 

/***********************END OF FILE****/
