/****************************************************************************

* Sigma团队

* 文件名: drv_hal_timer.c

* 内容简述：定时器Timer驱动文件

* 文件历史：

* 版本号		日期		作者		说明
*  3.0	 	2023-01-26	  鲍程璐	适配STM32F4系列

*  2.9	 	2023-11-16	  鲍程璐	定时器兼容老版初始化方式

*  2.8	 	2023-09-04	  鲍程璐	定时器设定计时简化

* 1.0.0a 	2020-02-22	  李环宇	创建该文件

****************************************************************************/
#include "drv_hal_conf.h"

#ifdef DRV_HAL_TIMER_ENABLE

/**
 * @brief 定时器时钟使能
 * @param _tTimer-Timer结构体指针
 * @retval Null
*/
static void S_TIM_CLKEnable(tagTIM_T *_tTimer) 
{	
	if(_tTimer->tTimerHandle.Instance == TIM1)
	{
		__HAL_RCC_TIM1_CLK_ENABLE();
	}
	else if(_tTimer->tTimerHandle.Instance == TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();
	}
	else if(_tTimer->tTimerHandle.Instance == TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();
	}
	else if(_tTimer->tTimerHandle.Instance == TIM4)
	{
		__HAL_RCC_TIM4_CLK_ENABLE();
	}
	else if(_tTimer->tTimerHandle.Instance == TIM5)
	{
		__HAL_RCC_TIM5_CLK_ENABLE();
	}
	else if(_tTimer->tTimerHandle.Instance == TIM6)
	{
		__HAL_RCC_TIM6_CLK_ENABLE();
	}
	else if(_tTimer->tTimerHandle.Instance == TIM7)
	{
		__HAL_RCC_TIM7_CLK_ENABLE();
	}
	else if(_tTimer->tTimerHandle.Instance == TIM8)
	{
		__HAL_RCC_TIM8_CLK_ENABLE();
	}
#ifdef STM32F4_SGA_ENABLE
	else if(_tTimer->tTimerHandle.Instance == TIM9)
	{
		__HAL_RCC_TIM9_CLK_ENABLE();
	}
	else if(_tTimer->tTimerHandle.Instance == TIM10)
	{
		__HAL_RCC_TIM10_CLK_ENABLE();
	}
	else if(_tTimer->tTimerHandle.Instance == TIM11)
	{
		__HAL_RCC_TIM11_CLK_ENABLE();
	}
	else if(_tTimer->tTimerHandle.Instance == TIM12)
	{
		__HAL_RCC_TIM12_CLK_ENABLE();
	}
	else if(_tTimer->tTimerHandle.Instance == TIM13)
	{
		__HAL_RCC_TIM13_CLK_ENABLE();
	}
	else if(_tTimer->tTimerHandle.Instance == TIM14)
	{
		__HAL_RCC_TIM14_CLK_ENABLE();
	}
#endif
}

/**
 * @brief 定时器中断配置
 * @param _tTimer-Timer结构体指针
 * @retval Null
*/
static void S_TIM_NVICConfig(tagTIM_T *_tTimer)
{
	if(_tTimer->tTimerHandle.Instance == TIM2)
	{
		HAL_NVIC_SetPriority(TIM2_IRQn, _tTimer->ucPriority, _tTimer->ucSubPriority);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
	}
	else if(_tTimer->tTimerHandle.Instance == TIM3)
	{			
		HAL_NVIC_SetPriority(TIM3_IRQn, _tTimer->ucPriority, _tTimer->ucSubPriority);
		HAL_NVIC_EnableIRQ(TIM3_IRQn);
	}
	else if(_tTimer->tTimerHandle.Instance == TIM4)
	{			
		HAL_NVIC_SetPriority(TIM4_IRQn, _tTimer->ucPriority, _tTimer->ucSubPriority);
		HAL_NVIC_EnableIRQ(TIM4_IRQn);
	}
	else if(_tTimer->tTimerHandle.Instance == TIM5)
	{			
		HAL_NVIC_SetPriority(TIM5_IRQn, _tTimer->ucPriority, _tTimer->ucSubPriority);
		HAL_NVIC_EnableIRQ(TIM5_IRQn);
	}
#ifdef STM32F1_SGA_ENABLE
	else if(_tTimer->tTimerHandle.Instance == TIM6)
	{			
		HAL_NVIC_SetPriority(TIM6_IRQn, _tTimer->ucPriority, _tTimer->ucSubPriority);
		HAL_NVIC_EnableIRQ(TIM6_IRQn);
	}
#endif

#ifdef STM32F4_SGA_ENABLE
	else if(_tTimer->tTimerHandle.Instance == TIM6)
	{			
		HAL_NVIC_SetPriority(TIM6_DAC_IRQn, _tTimer->ucPriority, _tTimer->ucSubPriority);
		HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
	}
#endif
	else if(_tTimer->tTimerHandle.Instance == TIM7)
	{			
		HAL_NVIC_SetPriority(TIM7_IRQn, _tTimer->ucPriority, _tTimer->ucSubPriority);
		HAL_NVIC_EnableIRQ(TIM7_IRQn);
	}
#ifdef STM32F4_SGA_ENABLE
	else if(_tTimer->tTimerHandle.Instance == TIM9)
	{			
		HAL_NVIC_SetPriority(TIM1_BRK_TIM9_IRQn, _tTimer->ucPriority, _tTimer->ucSubPriority);
		HAL_NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);
	}
	else if(_tTimer->tTimerHandle.Instance == TIM10)
	{			
		HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, _tTimer->ucPriority, _tTimer->ucSubPriority);
		HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
	}
	else if(_tTimer->tTimerHandle.Instance == TIM11)
	{			
		HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, _tTimer->ucPriority, _tTimer->ucSubPriority);
		HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
	}
	else if(_tTimer->tTimerHandle.Instance == TIM12)
	{			
		HAL_NVIC_SetPriority(TIM8_BRK_TIM12_IRQn, _tTimer->ucPriority, _tTimer->ucSubPriority);
		HAL_NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn);
	}
	else if(_tTimer->tTimerHandle.Instance == TIM13)
	{			
		HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn, _tTimer->ucPriority, _tTimer->ucSubPriority);
		HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);
	}
	else if(_tTimer->tTimerHandle.Instance == TIM14)
	{			
		HAL_NVIC_SetPriority(TIM8_TRG_COM_TIM14_IRQn, _tTimer->ucPriority, _tTimer->ucSubPriority);
		HAL_NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn);
	}
#endif
}

/**
 * @brief 定时器参数匹配函数
 * @param _tTimer-Timer结构体指针
 * @note 定时时间计算公式TimeOut(us) = ((Prescaler + 1) * (Period + 1)) / TimeClockFren
 * @retval Null
*/
static void S_TIM_ParamMatch(tagTIM_T *_tTimer)
{
	DEFAULT(_tTimer->tTimerHandle.Init.CounterMode,TIM_COUNTERMODE_UP);
	DEFAULT(_tTimer->tTimerHandle.Init.ClockDivision,TIM_CLOCKDIVISION_DIV1);
	DEFAULT(_tTimer->ucPriority,2);
	DEFAULT(_tTimer->ucSubPriority,2);

#ifdef STM32F4_SGA_ENABLE
	DEFAULT(_tTimer->tClockSourceConfig.ClockSource,TIM_CLOCKSOURCE_INTERNAL);	/* 时钟来源为内部时钟 */
  	DEFAULT(_tTimer->tMasterConfig.MasterOutputTrigger,TIM_TRGO_RESET);
  	DEFAULT(_tTimer->tMasterConfig.MasterSlaveMode,TIM_MASTERSLAVEMODE_DISABLE);
#endif

	/* 如果用户指定了分频系数和重载值，则退出函数 */
	if((_tTimer->tTimerHandle.Init.Period != 0) && (_tTimer->tTimerHandle.Init.Prescaler != 0))
		return;

#ifdef STM32F1_SGA_ENABLE
	/* fTimingLength限幅 */
	if(_tTimer->fTimingLength < 0)	_tTimer->fTimingLength = 0;
	else if(_tTimer->fTimingLength > 59651.413333f)	_tTimer->fTimingLength = 59651.413333f;

	/* 根据fTimingLength匹配分频系数和重载值 */
	if(_tTimer->fTimingLength > 52428)
	{
		_tTimer->tTimerHandle.Init.Prescaler = 65536-1;
		_tTimer->tTimerHandle.Init.Period = (uint32_t)(_tTimer->fTimingLength*72000/(_tTimer->tTimerHandle.Init.Prescaler+1));
	}
	else if(_tTimer->fTimingLength > 26214)
	{
		_tTimer->tTimerHandle.Init.Prescaler = 57600-1;
		_tTimer->tTimerHandle.Init.Period = (uint32_t)(_tTimer->fTimingLength*72000/(_tTimer->tTimerHandle.Init.Prescaler+1));
	}
	else if(_tTimer->fTimingLength > 6553.5f)
	{
		_tTimer->tTimerHandle.Init.Prescaler = 28800-1;
		_tTimer->tTimerHandle.Init.Period = (uint32_t)(_tTimer->fTimingLength*72000/(_tTimer->tTimerHandle.Init.Prescaler+1));
	}
	else if(_tTimer->fTimingLength >= 0)
	{
		_tTimer->tTimerHandle.Init.Prescaler = 7200-1;
		_tTimer->tTimerHandle.Init.Period = (uint32_t)(_tTimer->fTimingLength*72000/(_tTimer->tTimerHandle.Init.Prescaler+1));
	}
#endif

#ifdef STM32F4_SGA_ENABLE

	/* 根据fTimingLength匹配分频系数和重载值 */
	/* TIM1、TIM8-TIM11的时钟为APB2时钟的两倍即168M */
	if(_tTimer->tTimerHandle.Instance == TIM1 
	|| _tTimer->tTimerHandle.Instance == TIM8 
	|| _tTimer->tTimerHandle.Instance == TIM9 
	|| _tTimer->tTimerHandle.Instance == TIM10
	|| _tTimer->tTimerHandle.Instance == TIM11)
	{
		/* fTimingLength限幅 */
		if(_tTimer->fTimingLength < 0)	_tTimer->fTimingLength = 0;
		else if(_tTimer->fTimingLength > 25564.891428f)	_tTimer->fTimingLength = 25564.891428f;

		if(_tTimer->fTimingLength > 13107)
		{
			_tTimer->tTimerHandle.Init.Prescaler = 65536-1;
			_tTimer->tTimerHandle.Init.Period = (uint32_t)(_tTimer->fTimingLength*168000/(_tTimer->tTimerHandle.Init.Prescaler+1));	
		}
		else if(_tTimer->fTimingLength > 6553.5f)
		{
			_tTimer->tTimerHandle.Init.Prescaler = 33600-1;
			_tTimer->tTimerHandle.Init.Period = (uint32_t)(_tTimer->fTimingLength*168000/(_tTimer->tTimerHandle.Init.Prescaler+1));			
		}
		else if(_tTimer->fTimingLength >= 0)
		{
			_tTimer->tTimerHandle.Init.Prescaler = 16800-1;
			_tTimer->tTimerHandle.Init.Period = (uint32_t)(_tTimer->fTimingLength*168000/(_tTimer->tTimerHandle.Init.Prescaler+1));
		}	
	}
	/* TIM2-TIM7、TIM12-TIM14的时钟为APB1的时钟的两倍即84M */
	else if(_tTimer->tTimerHandle.Instance == TIM3 
		|| _tTimer->tTimerHandle.Instance == TIM4 
		|| _tTimer->tTimerHandle.Instance == TIM6 
		|| _tTimer->tTimerHandle.Instance == TIM7
		|| _tTimer->tTimerHandle.Instance == TIM12
		|| _tTimer->tTimerHandle.Instance == TIM13 
		|| _tTimer->tTimerHandle.Instance == TIM14)
	{
		/* fTimingLength限幅 */
		if(_tTimer->fTimingLength < 0)	_tTimer->fTimingLength = 0;
		else if(_tTimer->fTimingLength > 51129.782857f)	_tTimer->fTimingLength = 51129.782857f;

		if(_tTimer->fTimingLength > 26214)
		{
			_tTimer->tTimerHandle.Init.Prescaler = 65536-1;
			_tTimer->tTimerHandle.Init.Period = (uint32_t)(_tTimer->fTimingLength*84000/(_tTimer->tTimerHandle.Init.Prescaler+1));				
		}
		else if(_tTimer->fTimingLength > 13107)
		{
			_tTimer->tTimerHandle.Init.Prescaler = 33600-1;
			_tTimer->tTimerHandle.Init.Period = (uint32_t)(_tTimer->fTimingLength*84000/(_tTimer->tTimerHandle.Init.Prescaler+1));				
		}
		else if(_tTimer->fTimingLength > 6553.5f)
		{
			_tTimer->tTimerHandle.Init.Prescaler = 16800-1;
			_tTimer->tTimerHandle.Init.Period = (uint32_t)(_tTimer->fTimingLength*84000/(_tTimer->tTimerHandle.Init.Prescaler+1));			
		}
		else if(_tTimer->fTimingLength >= 0)
		{
			_tTimer->tTimerHandle.Init.Prescaler = 8400-1;
			_tTimer->tTimerHandle.Init.Period = (uint32_t)(_tTimer->fTimingLength*84000/(_tTimer->tTimerHandle.Init.Prescaler+1));
		}	
	}
	/* TIM2、TIM5为32位定时器 */
	else if(_tTimer->tTimerHandle.Instance == TIM2 
		|| _tTimer->tTimerHandle.Instance == TIM5)
	{
		/* fTimingLength限幅 */
		if(_tTimer->fTimingLength < 0)	_tTimer->fTimingLength = 0;
		else if(_tTimer->fTimingLength > 3350892579.108571f)	_tTimer->fTimingLength = 3350892579.108571f;

		if(_tTimer->fTimingLength > 1717986918)
		{
			_tTimer->tTimerHandle.Init.Prescaler = 65536-1;
			_tTimer->tTimerHandle.Init.Period = (uint32_t)(_tTimer->fTimingLength*84000/(_tTimer->tTimerHandle.Init.Prescaler+1));				
		}
		else if(_tTimer->fTimingLength > 858993459)
		{
			_tTimer->tTimerHandle.Init.Prescaler = 33600-1;
			_tTimer->tTimerHandle.Init.Period = (uint32_t)(_tTimer->fTimingLength*84000/(_tTimer->tTimerHandle.Init.Prescaler+1));		
		}
		else if(_tTimer->fTimingLength > 429496729.5f)
		{
			_tTimer->tTimerHandle.Init.Prescaler = 16800-1;
			_tTimer->tTimerHandle.Init.Period = (uint32_t)(_tTimer->fTimingLength*84000/(_tTimer->tTimerHandle.Init.Prescaler+1));	
		}
		else if(_tTimer->fTimingLength >= 0)
		{
			_tTimer->tTimerHandle.Init.Prescaler = 8400-1;
			_tTimer->tTimerHandle.Init.Period = (uint32_t)(_tTimer->fTimingLength*84000/(_tTimer->tTimerHandle.Init.Prescaler+1));
		}	
	}

#endif
}

/**
 * @brief 定时器配置
 * @param _tTimer-Timer结构体指针
 * @retval Null
*/
static void S_TIM_PramConfig(tagTIM_T *_tTimer)
{	
	if (HAL_TIM_Base_Init(&_tTimer->tTimerHandle) != HAL_OK)
	{
		Drv_HAL_Error(__FILE__, __LINE__);
	}

#ifdef STM32F4_SGA_ENABLE
	if (HAL_TIM_ConfigClockSource(&_tTimer->tTimerHandle, &_tTimer->tClockSourceConfig) != HAL_OK)
 	{
    	Drv_HAL_Error(__FILE__, __LINE__);
  	}
	if (HAL_TIMEx_MasterConfigSynchronization(&_tTimer->tTimerHandle, &_tTimer->tMasterConfig) != HAL_OK)
 	{
    	Drv_HAL_Error(__FILE__, __LINE__);
  	}
#endif

	S_TIM_NVICConfig(_tTimer);
}

/**
 * @brief 定时器重置
 * @param _tTimer-Timer结构体指针
 * @retval Null
*/
void Drv_Timer_Reset(tagTIM_T *_tTimer)
{
	HAL_TIM_IRQHandler(&(_tTimer->tTimerHandle));
}

/**
 * @brief 定时器使能
 * @param _tTimer-Timer结构体指针
 * @retval Null
*/
void Drv_Timer_Enable(tagTIM_T *_tTimer)
{
	HAL_TIM_Base_Start_IT(&(_tTimer->tTimerHandle));
}

/**
 * @brief 定时器失能
 * @param _tTimer-Timer结构体指针
 * @retval Null
*/
void Drv_Timer_Disable(tagTIM_T *_tTimer)
{
	HAL_TIM_Base_Stop_IT(&_tTimer->tTimerHandle);
}

/**
 * @brief 定时器初始化
 * @param _tTimer-Timer结构体指针
 * @retval Null
*/
void Drv_Timer_Init(tagTIM_T *_tTimer)
{
	S_TIM_CLKEnable(_tTimer);		/* 使能TIM时钟 */	
	S_TIM_ParamMatch(_tTimer);	
	S_TIM_PramConfig(_tTimer);
}

/**
 * @brief 定时器中断
 * @param _tTimer-Timer结构体指针
 * @retval Null
*/
void Drv_Timer_IRQHandler(tagTIM_T *_tTimer)
{
	HAL_TIM_IRQHandler(&_tTimer->tTimerHandle);
}

#endif
