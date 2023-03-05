#ifndef __DRV_IIC_SOFT_H_
#define	__DRV_IIC_SOFT_H_

#include "drv_hal_conf.h"

#define TYPE_NUM		8	/* 字节位数 */
#define IIC_GPIO_NUM	2	/* IIC所需的GPIO数量 */

typedef struct
{
	tagGPIO_T tIICSoft[2];
}tagIICSoft_T;

void Drv_IICSoft_Init(tagIICSoft_T *_tIIC);
void Drv_IICSoft_SendByte(tagIICSoft_T *_tIIC, uint8_t ucTxData);
uint8_t Drv_IICSoft_ReadByte(tagIICSoft_T *_tIIC, uint8_t _ucAck);

void Drv_IICSoft_Start(tagIICSoft_T *_tIIC);
void Drv_IICSoft_Stop(tagIICSoft_T *_tIIC);

uint8_t Drv_IICSoft_WaitAck(tagIICSoft_T *_tIIC);

void Drv_IICSoft_Init(tagIICSoft_T *_tIIC);

#endif
