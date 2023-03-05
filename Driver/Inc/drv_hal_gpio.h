#ifndef __DRV_GPIO_H_
#define __DRV_GPIO_H_

#include "drv_hal.h"

/** @defgroup ucAFMode_define remap
  * @brief GPIO remap
  * @{
  */
#define NO_REMAP 		0	/*不进行映射操作*/
#define PARTIAL_REMAP 	1	/*部分重映射*/
#define PARTIAL_REMAP2 	2	/*部分重映射2 仅TIM2独有*/
#define FULL_REMAP 		3	/*完全重映射*/

typedef struct
{
	GPIO_InitTypeDef 	tGPIOInit;
	GPIO_TypeDef* 		tGPIOPort;
	uint8_t				ucPriority;		/* 中断优先级，0-15 */
	uint8_t 			ucSubPriority;	/* 中断子优先级，0-15 */
	uint8_t				ucAFMode;		/* 重映射设置参数 @ref ucAFMode_define*/
}tagGPIO_T;

void Drv_GPIO_Set(tagGPIO_T *_tGPIO);
void Drv_GPIO_Reset(tagGPIO_T *_tGPIO);
void Drv_GPIO_Write(tagGPIO_T *_tGPIO, GPIO_PinState _PinStatus);
GPIO_PinState Drv_GPIO_Read(tagGPIO_T *_tGPIO);
void Drv_GPIO_Init(tagGPIO_T *_tGPIO, uint8_t _ucNum);
void Drv_GPIO_DeInit(tagGPIO_T *_tGPIO);

void Drv_GPIO_EXTI_IRQHandler(tagGPIO_T *_tGPIO);
void Drv_GPIO_NVICDisable(tagGPIO_T *_tGPIO);
void Drv_GPIO_NVICConfig(tagGPIO_T *_tGPIO);

#endif
