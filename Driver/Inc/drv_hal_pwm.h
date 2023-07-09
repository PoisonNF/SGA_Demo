#ifndef __DRV_PWM_H_
#define __DRV_PWM_H_

#include "drv_hal_conf.h"

#define MAX_RELOAD		1000000

typedef struct
{
	TIM_HandleTypeDef 	tPWMHandle;		/* STM32内部PWM设备句柄 */
	TIM_OC_InitTypeDef	tPWMChannel;	/* STM32内部PWM通道句柄 */
	uint32_t			ulFreq;			/* 频率(Hz) */
	float				fDuty;			/* 占空比(%) */
	uint8_t				ucChannel;		/* 通道 */
	tagGPIO_T			tGPIO;			/* GPIO设置 */
}tagPWM_T;

/*  定时器重映射表
	TIM1 	full remap (ETR/PE7,  CH1/PE9, CH2/PE11, CH3/PE13, CH4/PE14, BKIN/PE15, CH1N/PE8,  CH2N/PE10, CH3N/PE12)
			partial remap (ETR/PA12, CH1/PA8, CH2/PA9,  CH3/PA10, CH4/PA11, BKIN/PA6,  CH1N/PA7,  CH2N/PB0,  CH3N/PB1)
			no remap (ETR/PA12, CH1/PA8, CH2/PA9,  CH3/PA10, CH4/PA11, BKIN/PB12, CH1N/PB13, CH2N/PB14, CH3N/PB15)

	TIM2	full remap (CH1/ETR/PA15, CH2/PB3, CH3/PB10, CH4/PB11)
			partial remap2 (CH1/ETR/PA0,  CH2/PA1, CH3/PB10, CH4/PB11)
			partial remap1 (CH1/ETR/PA15, CH2/PB3, CH3/PA2,  CH4/PA3)
			no remap (CH1/ETR/PA0,  CH2/PA1, CH3/PA2,  CH4/PA3)

	TIM3 	full remap (CH1/PC6, CH2/PC7, CH3/PC8, CH4/PC9)
			partial remap (CH1/PB4, CH2/PB5, CH3/PB0, CH4/PB1)
			no remap (CH1/PA6, CH2/PA7, CH3/PB0, CH4/PB1)

	TIM4 	full remap (TIM4_CH1/PD12, TIM4_CH2/PD13, TIM4_CH3/PD14, TIM4_CH4/PD15)
			no remap (TIM4_CH1/PB6,  TIM4_CH2/PB7,  TIM4_CH3/PB8,  TIM4_CH4/PB9)

	TIM5CH4 enable TIM5_CH4 is connected to PA3     使用完全重映射ucMode

	TIM67DMA enable TIM6_DAC1 DMA request/DMA1 Channel3, TIM7_DAC2 DMA request/DMA1 Channel4 使用完全重映射ucMode
*/

void Drv_PWM_DutyFactorSet(tagPWM_T *_tPWM,float _fDuty);
void Drv_PWM_HighLvTimeSet(tagPWM_T *_tPWM, uint16_t _usTime);
void Drv_PWM_Init(tagPWM_T *_tPWM, uint8_t _ucNum);

#endif
