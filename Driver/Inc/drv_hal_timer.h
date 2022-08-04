#ifndef __DRV_TIMER_H_
#define __DRV_TIMER_H_

#include "drv_hal_conf.h"

typedef struct
{
	TIM_HandleTypeDef 	tTimerHandle;
	uint8_t				ulPriority;		/* 中断优先级，0-15 */
	uint8_t 			ulSubPriority;	/* 中断子优先级，0-15 */
}tagTIM_T;

void Drv_Timer_Reset(tagTIM_T *_tTimer);
void Drv_Timer_Enable(tagTIM_T *_tTimer);
void Drv_Timer_Disable(tagTIM_T *_tTimer);
void Drv_Timer_Init(tagTIM_T *_tTimer);
void Drv_Timer_IRQHandler(tagTIM_T *_tTimer);

#endif

