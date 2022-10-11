#ifndef __DRV_UART_H_
#define __DRV_UART_H_

#include "drv_hal_conf.h"

/* 串口接收信息结构体 */
typedef struct
{
	uint16_t			usRxCnt;			/* 接收数据计数器 */
	uint16_t			usRxLenth;			/* 接收数据长度 */
	uint16_t			usRxMAXLenth;		/* 接收数据最大长度 */
	uint8_t				ucpRxBuffer[1];		/* 接收Buffer */
	uint8_t				*ucpRxCache;		/* 接收缓冲区 */
}tagUartRxInfo_T;

/* 串口DMA配置 */
typedef struct
{
	DMA_HandleTypeDef	tDMATx;				/* DMA发送句柄 */
	DMA_HandleTypeDef	tDMARx;				/* DMA接收句柄 */

	uint8_t				DMARxCplt;			/* DMA接收完成标志*/
	uint8_t 			DMATxCplt;			/* DMA发送完成标志*/

	bool 				bTxEnable;			/* 发送使能符号 */
	bool				bRxEnable;			/* 接收使能符号 */

	uint8_t				ulDMAPriority;		/* DMA中断优先级，0-15 */
	uint8_t 			ulDMASubPriority;	/* DMA中断子优先级，0-15 */
}tagDMAUart_T;

/* 串口设备结构体 */
typedef struct
{
	UART_HandleTypeDef 	tUARTHandle;	/* STM32内部串口设备指针 */
	tagDMAUart_T		tUartDMA;		/* 串口DMA配置 */
	tagUartRxInfo_T		tRxInfo;		/* 串口接收信息 */
	tagGPIO_T			tGPIO[2];		/* GPIO句柄 */
	uint8_t				ulPriority;		/* 中断优先级，0-15 */
	uint8_t 			ulSubPriority;	/* 中断子优先级，0-15 */
}tagUART_T;

/* 串口重映射表
	USART1	full remap (TX/PB6, RX/PB7)
			no remap (TX/PA9, RX/PA10)
	USART2	full remap (CTS/PD3, RTS/PD4, TX/PD5, RX/PD6, CK/PD7)
			no remap (CTS/PA0, RTS/PA1, TX/PA2, RX/PA3, CK/PA4)
	USART3	full remap (TX/PD8,  RX/PD9,  CK/PD10, CTS/PD11, RTS/PD12)
			partial	remap (TX/PC10, RX/PC11, CK/PC12, CTS/PB13, RTS/PB14)
			no remap (TX/PB10, RX/PB11, CK/PB12, CTS/PB13, RTS/PB14)
*/

void Drv_Uart_ITInit(tagUART_T *_tUART);
void Drv_Uart_DMAInit(tagUART_T *_tUART);
void Drv_Uart_Transmit(tagUART_T *_tUART, uint8_t *_ucpTxData, uint16_t _uspSize);
void Drv_Uart_ReceIT_Enable(tagUART_T *_tUART, uint8_t *_ucpBuffer, uint16_t _uspSize);

void Drv_Uart_IRQHandler(tagUART_T *_tUART);
void Drv_Uart_DMA_Handler(tagUART_T *_tUART);

#endif
