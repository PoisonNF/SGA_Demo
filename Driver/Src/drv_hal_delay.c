/****************************************************************************

* Sigma团队

* 文件名: drv_hal_delay.c

* 内容简述：延时驱动文件

* 文件历史：

* 版本号	日期		作者		说明
* 1.1.8		2022-10-22	鲍程璐		适配RTT避免使用hal延时函数造成阻塞,新增us延时函数

* 1.0.0a 	2020-02-22	李环宇		创建该文件

****************************************************************************/
#include "drv_hal_conf.h"

#ifdef DRV_HAL_DELAY_ENABLE
/**
 * @brief 延时函数(ms)
 * @param _ulVal-延时时间有效值（单位ms）
 * @retval Null
*/
void Drv_Delay_Ms(uint32_t _ulVal)
{
	#ifdef RTT_ENABLE
	rt_thread_mdelay(_ulVal);
	#else
	HAL_Delay(_ulVal);
	#endif 
}

/**
 * @brief 延时函数(us)
 * @param _ulVal-延时时间有效值（单位us）
 * @retval Null
*/
void Drv_Delay_Us(uint32_t _ulVal)
{
	int i,j;
	for(i = 0; i < _ulVal; i++)
		for(j = 0; j < 12; j++);
}
#endif
