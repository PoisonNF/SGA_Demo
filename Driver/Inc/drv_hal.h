#ifndef __DRV_HAL_H_
#define __DRV_HAL_H_

#ifdef USE_DEBUG
#define DBG_INFO(...) Drv_HAL_PrintLog("INFO", __FUNCTION__, __LINE__, __VA_ARGS__)
#define DBG_BUF(...) Drv_HAL_PrintBuf("BUFF",__VA_ARGS__)
#else
#define DBG_INFO(...) 
#define DBG_BUF(...) 
#endif
#define DBG_ERROR(...) dbg_print_log("ERROR", __FUNCTION__, __LINE__, __VA_ARGS__)

/* Hal¿â°üº¬ */
#ifdef STM32F1_SGA_ENABLE
	#include "stm32f1xx_hal.h"
	
	#define STM32F1XX_HAL_DRIVER
#endif
	
#ifdef STM32L4_SGA_ENABLE
	#include "stm32l4xx_hal.h"
#endif

void Drv_HAL_PrintBuf(const char *_cpMsg, ...);
void Drv_HAL_PrintLog(const char *_cpMsg,const char *_cpFunc, const int _iLine,const char *_cpFmt,...);

void Drv_HAL_ProgramInfoOutput(void);
void Drv_HAL_IncTick(void);
void Drv_HAL_SysTickEnable(void);
void Drv_HAL_SysTickDisable(void);
void Drv_HAL_Init(void);
void Drv_HAL_Error(char *_cpFile, uint32_t _ulLine);

#endif
