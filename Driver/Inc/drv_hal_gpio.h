#ifndef __DRV_GPIO_H_
#define __DRV_GPIO_H_

#include "drv_hal.h"

typedef struct
{
	GPIO_InitTypeDef 	tGPIOInit;
	GPIO_TypeDef* 		tGPIOPort;
	uint8_t				ulPriority;		/* 中断优先级，0-15 */
	uint8_t 			ulSubPriority;	/* 中断子优先级，0-15 */
}tagGPIO_T;

void Drv_GPIO_Set(tagGPIO_T *_tGPIO);
void Drv_GPIO_Reset(tagGPIO_T *_tGPIO);
void Drv_GPIO_Write(tagGPIO_T *_tGPIO, GPIO_PinState _tGPIOPin);
GPIO_PinState Drv_GPIO_Read(tagGPIO_T *_tGPIO);
void Drv_GPIO_Init(tagGPIO_T *_tGPIO, uint8_t _ucNum);
void Drv_GPIO_DeInit(tagGPIO_T *_tGPIO);

void Drv_GPIO_EXTI_IRQHandler(tagGPIO_T *_tGPIO);
void Drv_GPIO_NVICDisable(tagGPIO_T *_tGPIO);
void Drv_GPIO_NVICConfig(tagGPIO_T *_tGPIO);

#endif
