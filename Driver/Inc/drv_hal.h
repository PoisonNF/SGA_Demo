#ifndef __DRV_HAL_H_
#define __DRV_HAL_H_

#ifdef USE_DEBUG
#define DBG_INFO(...) Drv_HAL_PrintLog("INFO", __FUNCTION__, __LINE__, __VA_ARGS__)
#define DBG_BUF(...) Drv_HAL_PrintBuf("BUFF",__VA_ARGS__)
#else
#define DBG_INFO(...) 
#define DBG_BUF(...) 
#endif
#define DBG_ERROR(...) Drv_HAL_PrintLog("ERROR", __FUNCTION__, __LINE__, __VA_ARGS__)

#define DEFAULT(config,value)   do{					    \
                                    if(config == 0)		\
                                        config = value;	\
                                }					    \
                                while(0)

/* Hal库包含 */
#ifdef STM32F1_SGA_ENABLE
	#include "stm32f1xx_hal.h"
	
	#define STM32F1XX_HAL_DRIVER
#endif

#ifdef STM32F4_SGA_ENABLE
	#include "stm32f4xx_hal.h"
#endif
	
#ifdef STM32L4_SGA_ENABLE
	#include "stm32l4xx_hal.h"
#endif

#define bool	_Bool
#define true	1
#define false	0

/* DWT数据观察点与跟踪相关 */
#define  DEM_CR      *(volatile uint32_t *)0xE000EDFC
#define  DWT_CR      *(volatile uint32_t *)0xE0001000
#define  DWT_CYCCNT  *(volatile uint32_t *)0xE0001004
#define  DEM_CR_TRCENA                   (1 << 24)
#define  DWT_CR_CYCCNTENA                (1 <<  0)

void Drv_HAL_PrintBuf(const char *_cpMsg, ...);
void Drv_HAL_PrintLog(const char *_cpMsg,const char *_cpFunc, const int _iLine,const char *_cpFmt,...);

void Drv_HAL_ProgramInfoOutput(void);
void Drv_HAL_IncTick(void);
void Drv_HAL_SysTickEnable(void);
void Drv_HAL_SysTickDisable(void);
void Drv_HAL_Init(void);
void Drv_HAL_Error(char *_cpFile, uint32_t _ulLine);

#endif
