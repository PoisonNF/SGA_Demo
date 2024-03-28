/****************************************************************************

* Sigma团队

* 文件名: drv_hal_wdg.c

* 内容简述：WDG看门狗驱动文件

* 文件历史：

* 版本号	日期		作者		说明
*  3.0	 2024-01-26	  鲍程璐	适配STM32F4系列

*  2.5   2023-06-03	  鲍程璐	创建该文件

****************************************************************************/
#include "drv_hal_conf.h"

#ifdef DRV_HAL_WDG_ENABLE

/**
 * @brief 独立看门狗配置函数
 * @param _tIWDG-独立看门狗结构体指针
 * @note  F1溢出时间计算公式 Tout = Prescaler / 40 * Reload (ms)
 * 		  F4溢出时间计算公式 Tout = Prescaler / 32 * Reload (ms)
 *        每个重载值都是按照2,048（最大值的一半）算出来的
 * @retval Null 
*/
static void S_IWDG_Config(tagIWDG_T *_tIWDG)
{
    /* 根据溢出时间选择合适的预分频值和重载值 */
#ifdef STM32F1_SGA_ENABLE
    if(_tIWDG->usResetTime > 13107)
	{
		_tIWDG->tIWDGHandle.Init.Prescaler = IWDG_PRESCALER_256;
		_tIWDG->tIWDGHandle.Init.Reload = (float)_tIWDG->usResetTime / 6.4f;
	}
	else if(_tIWDG->usResetTime > 6553)
	{
		_tIWDG->tIWDGHandle.Init.Prescaler = IWDG_PRESCALER_128;
		_tIWDG->tIWDGHandle.Init.Reload = (float)_tIWDG->usResetTime / 3.2f;
	}
	else if(_tIWDG->usResetTime > 3276)
	{
		_tIWDG->tIWDGHandle.Init.Prescaler = IWDG_PRESCALER_64;
		_tIWDG->tIWDGHandle.Init.Reload = (float)_tIWDG->usResetTime / 1.6f;
	}
	else if(_tIWDG->usResetTime > 1638)
	{
		_tIWDG->tIWDGHandle.Init.Prescaler = IWDG_PRESCALER_32;
		_tIWDG->tIWDGHandle.Init.Reload = (float)_tIWDG->usResetTime / 0.8f;
	}
	else if(_tIWDG->usResetTime > 819)
	{
		_tIWDG->tIWDGHandle.Init.Prescaler = IWDG_PRESCALER_16;
		_tIWDG->tIWDGHandle.Init.Reload = (float)_tIWDG->usResetTime / 0.4f;
	}
	else if(_tIWDG->usResetTime > 409)
	{
		_tIWDG->tIWDGHandle.Init.Prescaler = IWDG_PRESCALER_8;
		_tIWDG->tIWDGHandle.Init.Reload = (float)_tIWDG->usResetTime / 0.2f;
	}
	else
	{
		_tIWDG->tIWDGHandle.Init.Prescaler = IWDG_PRESCALER_4;
		_tIWDG->tIWDGHandle.Init.Reload = (float)_tIWDG->usResetTime / 0.1f;
	}

#endif

#ifdef STM32F4_SGA_ENABLE
	if(_tIWDG->usResetTime > 16384)
	{
		_tIWDG->tIWDGHandle.Init.Prescaler = IWDG_PRESCALER_256;
		_tIWDG->tIWDGHandle.Init.Reload = _tIWDG->usResetTime / 8;		
	}
	else if(_tIWDG->usResetTime > 8192)
	{
		_tIWDG->tIWDGHandle.Init.Prescaler = IWDG_PRESCALER_128;
		_tIWDG->tIWDGHandle.Init.Reload = _tIWDG->usResetTime / 4;
	}
	else if(_tIWDG->usResetTime > 4096)
	{
		_tIWDG->tIWDGHandle.Init.Prescaler = IWDG_PRESCALER_64;
		_tIWDG->tIWDGHandle.Init.Reload = _tIWDG->usResetTime / 2;
	}
	else if(_tIWDG->usResetTime > 2048)
	{
		_tIWDG->tIWDGHandle.Init.Prescaler = IWDG_PRESCALER_32;
		_tIWDG->tIWDGHandle.Init.Reload = _tIWDG->usResetTime;
	}
	else if(_tIWDG->usResetTime > 1024)
	{
		_tIWDG->tIWDGHandle.Init.Prescaler = IWDG_PRESCALER_16;
		_tIWDG->tIWDGHandle.Init.Reload = _tIWDG->usResetTime * 2;
	}
	else if(_tIWDG->usResetTime > 512)
	{
		_tIWDG->tIWDGHandle.Init.Prescaler = IWDG_PRESCALER_8;
		_tIWDG->tIWDGHandle.Init.Reload = _tIWDG->usResetTime * 4;
	}
	else
	{
		_tIWDG->tIWDGHandle.Init.Prescaler = IWDG_PRESCALER_4;
		_tIWDG->tIWDGHandle.Init.Reload = _tIWDG->usResetTime * 8;		
	}

#endif
    _tIWDG->tIWDGHandle.Instance = IWDG;
}

/**
 * @brief 独立看门狗初始化
 * @param _tIWDG-独立看门狗结构体指针
 * @retval Null 
*/
void Drv_IWDG_Init(tagIWDG_T *_tIWDG)
{
	S_IWDG_Config(_tIWDG);

    HAL_IWDG_Init(&_tIWDG->tIWDGHandle);
}

/**
 * @brief 独立看门狗喂狗
 * @param _tIWDG-独立看门狗结构体指针
 * @retval Null 
*/
void Drv_IWDG_Feed(tagIWDG_T *_tIWDG)
{
	HAL_IWDG_Refresh(&_tIWDG->tIWDGHandle);
}

#endif
