#ifndef __DRV_WDG_H_
#define __DRV_WDG_H_

#include "drv_hal_conf.h"

/* 独立看门狗结构体 */
typedef struct
{
    IWDG_HandleTypeDef  tIWDGHandle;
	uint16_t			usResetTime;	/* 复位时间(ms)，最大值26,208ms */
}tagIWDG_T;

void Drv_IWDG_Init(tagIWDG_T *_tIWDG);
void Drv_IWDG_Feed(tagIWDG_T *_tIWDG);

#endif
