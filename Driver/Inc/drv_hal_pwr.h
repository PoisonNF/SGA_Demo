#ifndef __DRV_PWR_H_
#define __DRV_PWR_H_

#include "drv_hal_conf.h"


void Drv_PWR_EnterSleepMode(void);
void Drv_PWR_EnterStopMode(void);
void Drv_PWR_EnterStandByMode(void);
void Drv_PWR_PVD_Init(uint32_t _ulPVDLevel);
void Drv_PVD_IRQHandler(void);

#endif
