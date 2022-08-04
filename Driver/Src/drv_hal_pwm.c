/****************************************************************************

* Sigma团队

* 文件名: drv_hal_pwm.c

* 内容简述：PWM驱动文件

* 文件历史：

* 版本号	日期		作者		说明

* 1.0.0a 	2020-02-22	李环宇		创建该文件

****************************************************************************/
#include "drv_hal_conf.h"

#ifdef DRV_HAL_PWM_ENABLE

#define MAX_RELOAD		SYSTEM_CLOCK * 1000000
#define INIT_CRR		SYSTEM_CLOCK * 500000

/**
 * @brief PWM波占空比设置
 * @param _tPWM-PWM结构体指针
 * @param _fDuty-占空比(单位：%)
 * @retval null
*/
void Drv_PWM_DutyfactorSet(tagPWM_T *_tPWM, float _fDuty)
{
	switch(_tPWM->ulChannel)
	{
		case TIM_CHANNEL_1:
			_tPWM->tPWMHandle.Instance->CCR1 = (MAX_RELOAD /_tPWM->ulFreq - 1) * _fDuty / 100;
		break;
		
		case TIM_CHANNEL_2:
			_tPWM->tPWMHandle.Instance->CCR2 = (MAX_RELOAD /_tPWM->ulFreq - 1) * _fDuty / 100;
		break;
		
		case TIM_CHANNEL_3:
			_tPWM->tPWMHandle.Instance->CCR3 = (MAX_RELOAD /_tPWM->ulFreq - 1) * _fDuty / 100;
		break;
		
		case TIM_CHANNEL_4:
			_tPWM->tPWMHandle.Instance->CCR4 = (MAX_RELOAD /_tPWM->ulFreq - 1) * _fDuty / 100;
		break;
		
		default:
			Drv_HAL_Error(__FILE__, __LINE__);
		break;
	}
}

/**
 * @brief PWM波时钟使能
 * @param _tPWM-PWM结构体指针
 * @retval null
*/
static void S_PWM_CLKEnable(tagPWM_T *_tPWM)
{
	if(_tPWM->tPWMHandle.Instance==TIM1)
	{
		__HAL_RCC_TIM1_CLK_ENABLE();	/* 使能定时器1 */
	}
	else if(_tPWM->tPWMHandle.Instance==TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();	/* 使能定时器2 */
	}
	else if(_tPWM->tPWMHandle.Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();	/* 使能定时器3 */
	}
	else if(_tPWM->tPWMHandle.Instance==TIM4)
	{
		__HAL_RCC_TIM4_CLK_ENABLE();	/* 使能定时器4 */
	}
	else if(_tPWM->tPWMHandle.Instance==TIM5)
	{
		__HAL_RCC_TIM5_CLK_ENABLE();	/* 使能定时器5 */
	}
	else if(_tPWM->tPWMHandle.Instance==TIM8)
	{
		__HAL_RCC_TIM8_CLK_ENABLE();	/* 使能定时器8 */
	}
}

/**
 * @brief PWM波参数设置
 * @param _tPWM-PWM结构体指针
 * @retval null
*/
static void S_PWM_PramConfig(tagPWM_T *_tPWM)
{	
	_tPWM->tPWMHandle.Init.Prescaler		=SYSTEM_CLOCK - 1;          /* 定时器分频 */
	_tPWM->tPWMHandle.Init.CounterMode		=TIM_COUNTERMODE_UP;		/* 向上计数模式 */
	_tPWM->tPWMHandle.Init.Period			=MAX_RELOAD /_tPWM->ulFreq - 1;	/* 自动重装载值 */
	_tPWM->tPWMHandle.Init.ClockDivision	=TIM_CLOCKDIVISION_DIV1;
		
	if (HAL_TIM_PWM_Init(&_tPWM->tPWMHandle) != HAL_OK)
	{
		Drv_HAL_Error(__FILE__, __LINE__);
	}
	
	_tPWM->tPWMChannel.OCMode		=TIM_OCMODE_PWM1; 			/* 模式选择PWM1 */
	_tPWM->tPWMChannel.Pulse		=INIT_CRR /_tPWM->ulFreq - 1;		/* 设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50% */
	_tPWM->tPWMChannel.OCPolarity	=TIM_OCPOLARITY_HIGH; 		/* 输出比较极性为低 */
	
	if(HAL_TIM_PWM_ConfigChannel(&_tPWM->tPWMHandle,&_tPWM->tPWMChannel,_tPWM->ulChannel))	/* 配置通道 */
	{
		Drv_HAL_Error(__FILE__, __LINE__);
	}				
	if(HAL_TIM_PWM_Start(&_tPWM->tPWMHandle,_tPWM->ulChannel))	/* 开启通道 */
	{
		Drv_HAL_Error(__FILE__, __LINE__);
	}
}

/**
 * @brief PWM对应GPIO配置
 * @param _tPWM-PWM结构体指针
 * @retval uint8_t- 0-失败，1-成功
*/
static void S_PWM_GPIOConfig(tagPWM_T *_tPWM)
{
	Drv_GPIO_Init(&_tPWM->tGPIO, 1);
}

/**
 * @brief PWM初始化
 * @param _tPWM-PWM结构体指针
 * @param _num-PWM数量
 * @retval null
*/
void Drv_PWM_Init(tagPWM_T *_tPWM, uint8_t _ucNum)
{
	uint8_t index;
	
	for(index = 0;index < _ucNum;index++)
	{
		S_PWM_CLKEnable(&_tPWM[index]);
		S_PWM_GPIOConfig(&_tPWM[index]);
		S_PWM_PramConfig(&_tPWM[index]);
	}
}
#endif
