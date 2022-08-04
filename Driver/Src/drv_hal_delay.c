/****************************************************************************

* Sigma团队

* 文件名: drv_hal_delay.c

* 内容简述：延时驱动文件

* 文件历史：

* 版本号	日期		作者		说明

* 1.0.0a 	2020-02-22	李环宇		创建该文件

****************************************************************************/
#include "drv_hal_conf.h"

#ifdef DRV_HAL_DELAY_ENABLE
/**
 * @brief 延时初函数(ms)
 * @param _ulVal-延时时间有效值（单位ms）
 * @retval Null
*/
void Drv_Delay_Ms(uint32_t _ulVal)
{
	HAL_Delay(_ulVal);
}
#endif
