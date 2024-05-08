#ifndef __DRV_CONF_H_
#define __DRV_CONF_H_

#define DRIVER_VERSION	"2024/05/08 V3.2"

/* RT-Thread���� ʹ��RTTʱ����ע�ͣ����ڹ����е���RTT����ں� */ 
//#define RTT_ENABLE   

#ifdef RTT_ENABLE
#include <rtthread.h>		/* RTT���ͷ�ļ� */
#include "threadpool.h"		/* threadpoolͷ�ļ� */ 
#endif

/* FreeRTOS���� ʹ��ʱ����ע�ͣ����ڹ����е���FreeRTOS����ں� */ 
//#define FREERTOS_ENABLE

#ifdef FREERTOS_ENABLE
#include "cmsis_os.h"		/* FreeRTOS���ͷ�ļ� */
#include "croutine.h"
#include "event_groups.h"
#include "stream_buffer.h"
#include "threadpool.h"		/* threadpoolͷ�ļ� */
#endif

#define PRINTF_UART	USART1				/* printfʹ�õĴ��� */

/* STM32оƬѡ�� */
#ifdef STM32F1_SGA_ENABLE
	#define SYSTEM_CLOCK 	72			/* ϵͳ��Ƶʱ�ӣ�72����λ��M */
#endif

#ifdef STM32F4_SGA_ENABLE
	#define SYSTEM_CLOCK 	168			/* ϵͳ��Ƶʱ�ӣ�168����λ��M */
#endif

#ifdef STM32L4_SGA_ENABLE
	#define SYSTEM_CLOCK 	80			/* ϵͳ��Ƶʱ�ӣ�80����λ��M */
#endif

/* ����ģ�� */
#define DRV_HAL_ENABLE
#define DRV_HAL_GPIO_ENABLE
#define DRV_HAL_DELAY_ENABLE
#define DRV_HAL_TIMER_ENABLE
#define DRV_HAL_SPI_ENABLE
#define DRV_HAL_SPI_SOFT_ENABLE 
#define DRV_HAL_ADC_ENABLE
#define DRV_HAL_UART_ENABLE
#define	DRV_HAL_IIC_SOFT_ENABLE
#define	DRV_HAL_PWM_ENABLE
#define	DRV_HAL_ADC_ENABLE
#define DRV_HAL_DAC_ENABLE
#define DRV_HAL_WDG_ENABLE
#define DRV_HAL_PWR_ENABLE
#define DRV_HAL_FLASH_ENABLE
#define DRV_HAL_CAN_ENABLE

#include "drv_hal.h"

#ifdef DRV_HAL_DELAY_ENABLE
#include "drv_hal_delay.h"
#endif

#ifdef DRV_HAL_GPIO_ENABLE
#include "drv_hal_gpio.h"
#endif

#ifdef DRV_HAL_PWM_ENABLE
#include "drv_hal_pwm.h"
#endif

#ifdef DRV_HAL_SPI_ENABLE
#include "drv_hal_spi.h"
#endif

#ifdef DRV_HAL_SPI_SOFT_ENABLE
#include "drv_hal_spi_soft.h"
#endif

#ifdef DRV_HAL_TIMER_ENABLE
#include "drv_hal_timer.h"
#endif

#ifdef DRV_HAL_UART_ENABLE
#include "drv_hal_uart.h"
#endif

#ifdef DRV_HAL_IIC_SOFT_ENABLE
#include "drv_hal_iic_soft.h"
#endif

#ifdef DRV_HAL_ADC_ENABLE
#include "drv_hal_adc.h"
#endif

#ifdef DRV_HAL_DAC_ENABLE
#include "drv_hal_dac.h"
#endif

#ifdef DRV_HAL_WDG_ENABLE
#include "drv_hal_wdg.h"
#endif

#ifdef DRV_HAL_PWR_ENABLE
#include "drv_hal_pwr.h"
#endif

#ifdef DRV_HAL_FLASH_ENABLE
#include "drv_hal_flash.h"
#endif

#ifdef DRV_HAL_CAN_ENABLE
#include "drv_hal_can.h"
#endif


/* C���Ա�׼�� */
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>


#endif
