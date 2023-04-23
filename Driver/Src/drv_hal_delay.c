/****************************************************************************

* Sigma团队

* 文件名: drv_hal_delay.c

* 内容简述：延时驱动文件

* 文件历史：

* 版本号	    日期	  作者		     说明
*  2.3		2023-04-23	鲍程璐		提供更加精准的延时函数

*  2.2		2023-03-29	鲍程璐		适配FreeRTOS,调用其API

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
	/* RT-Thread 延时函数 */
	#ifdef RTT_ENABLE
	rt_thread_mdelay(_ulVal);
	#endif

	/* FreeRTOS 延时函数 */
	#ifdef FREERTOS_ENABLE
	osDelay(_ulVal);
	#endif

	/* 裸机 延时函数 */
	#if !defined(FREERTOS_ENABLE) && !defined(RTT_ENABLE)
	Drv_Delay_Us(_ulVal * 1000);
	#endif
}

/**
 * @brief 延时函数(us)
 * @param _ulVal-延时时间有效值（单位us）
 * @retval Null
 * @note 延时us数越小会极其不精准，特别是在10us以下时，极短时间的延时建议使用RTOS内部的us级延时函数
*/
void Drv_Delay_Us(uint32_t _ulVal)
{
	/* 延时小于50us使用普通延时法较为精准 */
	if(_ulVal < 50)
	{
		int i,j;

		for(i = 0; i < _ulVal; i++)
			for(j = 0; j < 8; j++);
	}
	else if(_ulVal < 1000)		/* 大于50us使用查询法较为精准 */
	{
		uint32_t ulTemp;
		
		SysTick->LOAD = 9*_ulVal;
		SysTick->VAL  = 0X00;	/* 清空计数器 */
		SysTick->CTRL = 0X01;	/* 使能，减到零是无动作，采用外部时钟源 */

		do
			ulTemp = SysTick->CTRL;							/* 读取当前倒计数值 */
		while((ulTemp & 0x01) && (!(ulTemp & (1<<16))));	/* 等待时间到达 */

		SysTick->CTRL = 0x00; 	/* 关闭计数器 */
		SysTick->VAL  = 0X00; 	/* 清空计数器 */
	}
}
#endif
