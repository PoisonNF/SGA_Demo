/****************************************************************************

* Sigma团队

* 文件名: drv_hal_pwm.c

* 内容简述：PWM驱动文件

* 文件历史：

* 版本号	   日期		  	作者		  说明
* 2.3.1 	2023-05-05   鲍程璐		格式优化

* 1.1.7 	2022-10-11   鲍程璐		优化执行顺序

* 1.1.2 	2022-08-08	 鲍程璐		新增定时器引脚重映射代码

* 1.0.0a 	2020-02-22	 李环宇		创建该文件

****************************************************************************/
#include "drv_hal_conf.h"

#ifdef DRV_HAL_PWM_ENABLE

/**
 * @brief PWM波时钟使能
 * @param _tPWM-PWM结构体指针
 * @retval Null
*/
static void S_PWM_CLKEnable(tagPWM_T *_tPWM)
{
	if(_tPWM->tPWMHandle.Instance == TIM1)
	{
		__HAL_RCC_TIM1_CLK_ENABLE();	/* 使能定时器1 */
	}
	else if(_tPWM->tPWMHandle.Instance == TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();	/* 使能定时器2 */
	}
	else if(_tPWM->tPWMHandle.Instance == TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();	/* 使能定时器3 */
	}
	else if(_tPWM->tPWMHandle.Instance == TIM4)
	{
		__HAL_RCC_TIM4_CLK_ENABLE();	/* 使能定时器4 */
	}
	else if(_tPWM->tPWMHandle.Instance == TIM5)
	{
		__HAL_RCC_TIM5_CLK_ENABLE();	/* 使能定时器5 */
	}
	else if(_tPWM->tPWMHandle.Instance == TIM8)
	{
		__HAL_RCC_TIM8_CLK_ENABLE();	/* 使能定时器8 */
	}
}

/**
 * @brief PWM波参数设置
 * @param _tPWM-PWM结构体指针
 * @retval Null
*/
static void S_PWM_PramConfig(tagPWM_T *_tPWM)
{	
	_tPWM->tPWMHandle.Init.Prescaler			= SYSTEM_CLOCK - 1;         		/* 定时器分频 */
	_tPWM->tPWMHandle.Init.CounterMode			= TIM_COUNTERMODE_UP;			    /* 向上计数模式 */
	_tPWM->tPWMHandle.Init.Period				= MAX_RELOAD /_tPWM->ulFreq - 1;	/* 自动重装载值 */
	_tPWM->tPWMHandle.Init.ClockDivision		= TIM_CLOCKDIVISION_DIV1;
	_tPWM->tPWMHandle.Init.AutoReloadPreload 	= TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_PWM_Init(&_tPWM->tPWMHandle) != HAL_OK)
	{
		Drv_HAL_Error(__FILE__, __LINE__);
	}
    
	/* 模式选择PWM1 */
	_tPWM->tPWMChannel.OCMode		= TIM_OCMODE_PWM1;
    
 	/* 设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50% */    
	_tPWM->tPWMChannel.Pulse		= (MAX_RELOAD / _tPWM->ulFreq -1) * _tPWM->fDuty / 100;
    
    /* 输出比较极性为高 */
	_tPWM->tPWMChannel.OCPolarity	= TIM_OCPOLARITY_HIGH;
	
	if(HAL_TIM_PWM_ConfigChannel(&_tPWM->tPWMHandle,&_tPWM->tPWMChannel,_tPWM->ucChannel))	/* 配置通道 */
	{
		Drv_HAL_Error(__FILE__, __LINE__);
	}				
	if(HAL_TIM_PWM_Start(&_tPWM->tPWMHandle,_tPWM->ucChannel))	/* 开启通道 */
	{
		Drv_HAL_Error(__FILE__, __LINE__);
	}
}

/**
 * @brief PWM对应GPIO配置
 * @param _tPWM-PWM结构体指针
 * @retval Null
*/
static void S_PWM_GPIOConfig(tagPWM_T *_tPWM)
{
	/*PWM生成默认I/O配置*/
	_tPWM->tGPIO.tGPIOInit.Mode	 = GPIO_MODE_AF_PP;			/* 复用推挽输出 */
	_tPWM->tGPIO.tGPIOInit.Pull	 = GPIO_PULLUP;				/* 上拉 */
	_tPWM->tGPIO.tGPIOInit.Speed = GPIO_SPEED_FREQ_HIGH;	/* 高速模式 */

	/* 开启复用模式时钟 */
	__HAL_RCC_AFIO_CLK_ENABLE();

	/* 根据不同定时器的ucAFMode开启对应的重映射，重映射表在drv_hal_pwm.h中 */
	if(_tPWM->tPWMHandle.Instance == TIM1)
	{
		if(_tPWM->tGPIO.ucAFMode == NO_REMAP)				__HAL_AFIO_REMAP_TIM1_DISABLE();
		else if(_tPWM->tGPIO.ucAFMode == PARTIAL_REMAP)	    __HAL_AFIO_REMAP_TIM1_PARTIAL();
		else if(_tPWM->tGPIO.ucAFMode == PARTIAL_REMAP2)	while(1);
		else if(_tPWM->tGPIO.ucAFMode == FULL_REMAP)		__HAL_AFIO_REMAP_TIM1_ENABLE();
	}
	else if(_tPWM->tPWMHandle.Instance == TIM2)
	{
		if(_tPWM->tGPIO.ucAFMode == NO_REMAP)				__HAL_AFIO_REMAP_TIM2_DISABLE();
		else if(_tPWM->tGPIO.ucAFMode == PARTIAL_REMAP)	    __HAL_AFIO_REMAP_TIM2_PARTIAL_1();
		else if(_tPWM->tGPIO.ucAFMode == PARTIAL_REMAP2)	__HAL_AFIO_REMAP_TIM2_PARTIAL_2();
		else if(_tPWM->tGPIO.ucAFMode == FULL_REMAP)		__HAL_AFIO_REMAP_TIM2_ENABLE();
	}
	else if(_tPWM->tPWMHandle.Instance == TIM3)
	{
		if(_tPWM->tGPIO.ucAFMode == NO_REMAP)				__HAL_AFIO_REMAP_TIM3_DISABLE();
		else if(_tPWM->tGPIO.ucAFMode == PARTIAL_REMAP)	    __HAL_AFIO_REMAP_TIM3_PARTIAL();
		else if(_tPWM->tGPIO.ucAFMode == PARTIAL_REMAP2)	while(1);
		else if(_tPWM->tGPIO.ucAFMode == FULL_REMAP)		__HAL_AFIO_REMAP_TIM3_ENABLE();
	}
	else if(_tPWM->tPWMHandle.Instance == TIM4)
	{
		if(_tPWM->tGPIO.ucAFMode == NO_REMAP)				__HAL_AFIO_REMAP_TIM4_DISABLE();
		else if(_tPWM->tGPIO.ucAFMode == PARTIAL_REMAP)	    while(1);
		else if(_tPWM->tGPIO.ucAFMode == PARTIAL_REMAP2)	while(1);
		else if(_tPWM->tGPIO.ucAFMode == FULL_REMAP)		__HAL_AFIO_REMAP_TIM4_ENABLE();
	}
	else if(_tPWM->tPWMHandle.Instance == TIM5 && _tPWM->ucChannel == TIM_CHANNEL_4)
	{
		if(_tPWM->tGPIO.ucAFMode == NO_REMAP)				__HAL_AFIO_REMAP_TIM5CH4_DISABLE();
		else if(_tPWM->tGPIO.ucAFMode == FULL_REMAP) 		__HAL_AFIO_REMAP_TIM5CH4_ENABLE();
	}


	Drv_GPIO_Init(&_tPWM->tGPIO, 1);		/* GPIO初始化 */
}

/**
 * @brief PWM波占空比设置
 * @param _tPWM-PWM结构体指针
 * @param _fDuty-占空比(单位：%)
 * @retval Null
*/
void Drv_PWM_DutyfactorSet(tagPWM_T *_tPWM, float _fDuty)
{
	switch(_tPWM->ucChannel)
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
 * @brief PWM波频率设置
 * @param _tPWM-PWM结构体指针
 * @param _ulFreq -频率(单位：Hz)
 * @retval Null
*/
void Drv_PWM_FreqSet(tagPWM_T *_tPWM, uint32_t _ulFreq)
{
	if(_tPWM->ulFreq == 50)
	{
		switch(_tPWM->ucChannel)
		{
			case TIM_CHANNEL_1:
				_tPWM->tPWMHandle.Instance->CCR1 = _ulFreq;
			break;
			
			case TIM_CHANNEL_2:
				_tPWM->tPWMHandle.Instance->CCR2 = _ulFreq;
			break;
			
			case TIM_CHANNEL_3:
				_tPWM->tPWMHandle.Instance->CCR3 = _ulFreq;
			break;
			
			case TIM_CHANNEL_4:
				_tPWM->tPWMHandle.Instance->CCR4 = _ulFreq;
			break;
			
			default:
				Drv_HAL_Error(__FILE__, __LINE__);
			break;
		}
	}
}

/**
 * @brief PWM初始化
 * @param _tPWM-PWM结构体指针
 * @param _ucNum-PWM数量
 * @retval Null
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
