#ifndef __DRV_CONF_H_
#define __DRV_CONF_H_

#define DRIVER_VERSION	"2022/09/03 V1.1.4"

/* RT-Thread开关 使用RTT时需解除注释，且在工程中导入RTT相关内核 */ 
//#define RTT_ENABLE               

#define PRINTF_UART	USART1				/* printf使用的串口 */

/* STM32芯片选择 */
#ifdef STM32F1_SGA_ENABLE
	#define SYSTEM_CLOCK 	72			/* 系统主频时钟：72，单位：M */
	
	/* rtt相关 */
	#define STM32_SRAM_SIZE 64
	#define STM32F10X_PIN_NUMBERS 100
#endif

#ifdef STM32L4_SGA_ENABLE
	#define SYSTEM_CLOCK 	80			/* 系统主频时钟：80，单位：M */
	
	/* rtt相关 */
	#define STM32_SRAM_SIZE 64
	#define STM32L4X_PIN_NUMBERS 64
#endif

/* 启动模块 */
#define DRV_HAL_ENABLE
#define DRV_HAL_GPIO_ENABLE
#define DRV_HAL_DELAY_ENABLE
#define DRV_HAL_TIMER_ENABLE
#define DRV_HAL_SPI_ENABLE
#define DRV_HAL_ADC_ENABLE
#define DRV_HAL_UART_ENABLE
#define	DRV_HAL_IIC_SOFT_ENABLE
#define	DRV_HAL_PWM_ENABLE
#define	DRV_HAL_ADC_ENABLE

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

/* C语言标准库 */
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* I/O初始化头文件 */
#include "bsp_io.h"

#endif
