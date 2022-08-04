#ifndef __OCD_ADS1256_H_
#define __OCD_ADS1256_H_

#include "drv_hal_conf.h"

typedef struct
{
	tagSPI_T 	tSPI;
	tagGPIO_T	tDrdyGPIO;
	tagGPIO_T	tCsGPIO;
}tagADS1256_T;

typedef struct
{
	uint8_t ucID;
	uint8_t ucDataOrder ;
	uint8_t ucAutoCalibration;
	uint8_t ucBufferStatus ;	
	uint8_t ucMuxValue;
	uint8_t ucPgaValue;
	uint8_t ucDataRate;
}tagADS1256Status_T;

void OCD_ADS1256_Init(tagADS1256_T *_tADS1256);
uint8_t OCD_ADS1256_Check(tagADS1256_T *_tADS1256);
uint8_t OCD_ADS1256_ReadStatus(tagADS1256_T *_tADS1256, tagADS1256Status_T *ptr);
float OCD_ADS1256_ReadOneChannelVoltage(tagADS1256_T *_tADS1256, uint8_t _ucChannel);

#endif
