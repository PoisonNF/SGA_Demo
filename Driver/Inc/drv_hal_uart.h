#ifndef __DRV_UART_H_
#define __DRV_UART_H_

#include "drv_hal_conf.h"

/* 串口设备结构体 */
typedef struct
{
	UART_HandleTypeDef 	tUARTHandle;	/* STM32内部串口设备指针 */
	tagGPIO_T			tGPIO[2];
	uint8_t				ulPriority;		/* 中断优先级，0-15 */
	uint8_t 			ulSubPriority;	/* 中断子优先级，0-15 */
}tagUART_T;

void Drv_Uart_ITInit(tagUART_T *_tUART);
void Drv_Uart_Transmit(tagUART_T *_tUART, uint8_t *_ucpTxData, uint16_t _uspSize);
void Drv_Uart_ReceIT_Enable(tagUART_T *_tUART, uint8_t *_ucpBuffer, uint16_t _uspSize);

void Drv_Uart_IRQHandler(tagUART_T *_tUART);

#endif
