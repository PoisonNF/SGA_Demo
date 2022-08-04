#ifndef __DRV_ADC_H_
#define __DRV_ADC_H_

#include "drv_hal_conf.h"
 
typedef struct
{
	ADC_HandleTypeDef  		tADCHandle;
	ADC_ChannelConfTypeDef	tADCChannel; 
	tagGPIO_T				tGPIO;
	uint8_t					ucAverageNum;
}tagADC_T;	 

float Drv_ADC_PollGetValue(tagADC_T *_tADC);
uint16_t Drv_ADC_PollGetData(tagADC_T *_tADC);
void Drv_ADC_Init(tagADC_T *_tADC, uint8_t _ucNum);

#endif
