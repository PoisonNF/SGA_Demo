/****************************************************************************

* Sigma团队

* 文件名: drv_hal_delay.c

* 内容简述：延时驱动文件

* 文件历史：

* 版本号	    日期	  作者		     说明
*   		2024-05-31	鲍程璐		提供FreeRTOS下的微秒级延时函数

*  2.4		2023-05-12	鲍程璐		解决FreeRTOS开始调度器前无法使用延时的问题

* 2.3.1		2023-05-05	鲍程璐		毫秒延时函数回退

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
	
#ifdef RTT_ENABLE
	/* RT-Thread 延时函数 */
	rt_thread_mdelay(_ulVal);
#endif

#ifdef FREERTOS_ENABLE
	/* FreeRTOS 延时函数 */
    if(xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED)	/* 如果未开启调度器，使用HAL库延时函数 */
        HAL_Delay(_ulVal);
    else
        osDelay(_ulVal);
#endif

#if !defined(FREERTOS_ENABLE) && !defined(RTT_ENABLE)
	/* 裸机 延时函数 */
	HAL_Delay(_ulVal);
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
#if !defined(FREERTOS_ENABLE)
	/* 延时小于50us使用普通延时法较为精准 */
	if(_ulVal < 50)
	{
		int i,j;

		for(i = 0; i < _ulVal; i++)
			for(j = 0; j < 8; j++);
	}
	else	/* 大于50us使用查询法较为精准 */
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
#endif

#ifdef FREERTOS_ENABLE
    uint32_t ulCnt, ulDelayCnt ,ulStart = 0;

    ulStart = DWT_CYCCNT; /* 刚进入时的计数器值 */
    ulCnt = 0;
    ulDelayCnt = _ulVal * (SystemCoreClock / 1000000);     /* 需要的节拍数 */  /* SystemCoreClock :系统时钟频率 */   

    while(ulCnt < ulDelayCnt)
    {
        ulCnt = DWT_CYCCNT - ulStart; 
        /* 求减过程中，如果发生第一次32位计数器重新计数，依然可以正确计算 */       
    }
#endif
}
#endif
