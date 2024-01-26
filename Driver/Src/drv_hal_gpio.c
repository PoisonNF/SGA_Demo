/****************************************************************************

* Sigma团队

* 文件名: drv_hal_gpio.c

* 内容简述：GPIO驱动文件

* 文件历史：

* 版本号	   日期		  作者		  说明
*  3.0	 	2023-01-26	鲍程璐		适配STM32F4系列

*  2.6	 	2023-06-15	鲍程璐		新增GPIO状态翻转函数

*  2.5	 	2023-05-29	鲍程璐		修复外部中断5-15无法使用的问题

*  2.0	 	2023-03-03	鲍程璐		IO初始化中增加对外部中断的支持，减少嵌套

* 1.0.0a 	2020-02-22	李环宇		创建该文件

* 1.0.1a 	2020-03-16	李环宇		新增Drv_GPIO_NVICDisable函数

****************************************************************************/
#include "drv_hal_conf.h"

#ifdef DRV_HAL_GPIO_ENABLE

/**
 * @brief GPIO时钟初始化函数
 * @param _tGPIO-GPIO句柄
 * @retval Null 
*/
static void S_GPIO_CLKEnable(tagGPIO_T *_tGPIO) 
{	
	if(_tGPIO->tGPIOPort == GPIOA)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
	}
	else if(_tGPIO->tGPIOPort == GPIOB)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();
	}
	else if(_tGPIO->tGPIOPort == GPIOC)
	{
		__HAL_RCC_GPIOC_CLK_ENABLE();
	}
	else if(_tGPIO->tGPIOPort == GPIOD)
	{
		__HAL_RCC_GPIOD_CLK_ENABLE();
	}
	else if(_tGPIO->tGPIOPort == GPIOE)
	{
		__HAL_RCC_GPIOE_CLK_ENABLE();
	}
#ifdef STM32F4_SGA_ENABLE
	else if(_tGPIO->tGPIOPort == GPIOF)
	{
		__HAL_RCC_GPIOF_CLK_ENABLE();
	}
	else if(_tGPIO->tGPIOPort == GPIOG)
	{
		__HAL_RCC_GPIOG_CLK_ENABLE();
	}
	else if(_tGPIO->tGPIOPort == GPIOH)
	{
		__HAL_RCC_GPIOH_CLK_ENABLE();
	}
	else if(_tGPIO->tGPIOPort == GPIOI)
	{
		__HAL_RCC_GPIOI_CLK_ENABLE();
	}
#endif
}

/**
 * @brief GPIO参数配置函数
 * @param _tGPIO-GPIO句柄
 * @retval Null
*/
static void S_GPIO_ParamConfig(tagGPIO_T *_tGPIO)
{
	HAL_GPIO_Init(_tGPIO->tGPIOPort, &_tGPIO->tGPIOInit);
}

/**
 * @brief GPIO中断线选择函数
 * @param _tGPIO-GPIO句柄
 * @retval uint8_t-IO对应的中断线
*/
static uint8_t S_GPIO_NVICIRQnSelect(tagGPIO_T *_tGPIO)
{
	if(_tGPIO->tGPIOInit.Pin == GPIO_PIN_0)
		return EXTI0_IRQn;

	else if(_tGPIO->tGPIOInit.Pin == GPIO_PIN_1)
		return EXTI1_IRQn;

	else if(_tGPIO->tGPIOInit.Pin == GPIO_PIN_2)
		return EXTI2_IRQn;

	else if(_tGPIO->tGPIOInit.Pin == GPIO_PIN_3)
		return EXTI3_IRQn;

	else if(_tGPIO->tGPIOInit.Pin == GPIO_PIN_4)
		return EXTI4_IRQn;

	else if((_tGPIO->tGPIOInit.Pin == GPIO_PIN_5)
		  ||(_tGPIO->tGPIOInit.Pin == GPIO_PIN_6)
		  ||(_tGPIO->tGPIOInit.Pin == GPIO_PIN_7)
		  ||(_tGPIO->tGPIOInit.Pin == GPIO_PIN_8)
		  ||(_tGPIO->tGPIOInit.Pin == GPIO_PIN_9))
		return EXTI9_5_IRQn;

	else if((_tGPIO->tGPIOInit.Pin == GPIO_PIN_10)
		  ||(_tGPIO->tGPIOInit.Pin == GPIO_PIN_11)
		  ||(_tGPIO->tGPIOInit.Pin == GPIO_PIN_12)
		  ||(_tGPIO->tGPIOInit.Pin == GPIO_PIN_13)
		  ||(_tGPIO->tGPIOInit.Pin == GPIO_PIN_14)
		  ||(_tGPIO->tGPIOInit.Pin == GPIO_PIN_15))
		return EXTI15_10_IRQn;
	
	return NULL;
}

/**
 * @brief GPIO中断参数设置
 * @param _tGPIO-GPIO句柄
 * @retval Null
*/
static void S_GPIO_NVICParamConfig(tagGPIO_T *_tGPIO)
{
	if(_tGPIO->tGPIOInit.Pin == GPIO_PIN_0)
	{
		HAL_NVIC_SetPriority(EXTI0_IRQn,_tGPIO->ucPriority,_tGPIO->ucSubPriority);
		HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	}
	else if(_tGPIO->tGPIOInit.Pin == GPIO_PIN_1)
	{
		HAL_NVIC_SetPriority(EXTI1_IRQn,_tGPIO->ucPriority,_tGPIO->ucSubPriority);
		HAL_NVIC_EnableIRQ(EXTI1_IRQn);
	}
	else if(_tGPIO->tGPIOInit.Pin == GPIO_PIN_2)
	{
		HAL_NVIC_SetPriority(EXTI2_IRQn,_tGPIO->ucPriority,_tGPIO->ucSubPriority);
		HAL_NVIC_EnableIRQ(EXTI2_IRQn);
	}
	else if(_tGPIO->tGPIOInit.Pin == GPIO_PIN_3)
	{
		HAL_NVIC_SetPriority(EXTI3_IRQn,_tGPIO->ucPriority,_tGPIO->ucSubPriority);
		HAL_NVIC_EnableIRQ(EXTI3_IRQn);
	}
	else if(_tGPIO->tGPIOInit.Pin == GPIO_PIN_4)
	{
		HAL_NVIC_SetPriority(EXTI4_IRQn,_tGPIO->ucPriority,_tGPIO->ucSubPriority);
		HAL_NVIC_EnableIRQ(EXTI4_IRQn);	
	}
	else if((_tGPIO->tGPIOInit.Pin == GPIO_PIN_5)
		  ||(_tGPIO->tGPIOInit.Pin == GPIO_PIN_6)
		  ||(_tGPIO->tGPIOInit.Pin == GPIO_PIN_7)
		  ||(_tGPIO->tGPIOInit.Pin == GPIO_PIN_8)
		  ||(_tGPIO->tGPIOInit.Pin == GPIO_PIN_9))
	{
		HAL_NVIC_SetPriority(EXTI9_5_IRQn,_tGPIO->ucPriority,_tGPIO->ucSubPriority);
		HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);	
	}
	else if((_tGPIO->tGPIOInit.Pin == GPIO_PIN_10)
		  ||(_tGPIO->tGPIOInit.Pin == GPIO_PIN_11)
		  ||(_tGPIO->tGPIOInit.Pin == GPIO_PIN_12)
		  ||(_tGPIO->tGPIOInit.Pin == GPIO_PIN_13)
		  ||(_tGPIO->tGPIOInit.Pin == GPIO_PIN_14)
		  ||(_tGPIO->tGPIOInit.Pin == GPIO_PIN_15))
	{
		HAL_NVIC_SetPriority(EXTI15_10_IRQn,_tGPIO->ucPriority,_tGPIO->ucSubPriority);
		HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);	
	}
}

/**
 * @brief 写入GPIO状态
 * @param _tGPIO-GPIO句柄
 * @param _PinStatus-写入状态
 * @retval Null
*/
void Drv_GPIO_Write(tagGPIO_T *_tGPIO, GPIO_PinState _PinStatus)
{
	HAL_GPIO_WritePin(_tGPIO->tGPIOPort, _tGPIO->tGPIOInit.Pin, _PinStatus);
}

/**
 * @brief GPIO拉高
 * @param _tGPIO-GPIO句柄
 * @retval Null
*/
void Drv_GPIO_Set(tagGPIO_T *_tGPIO)
{
	HAL_GPIO_WritePin(_tGPIO->tGPIOPort, _tGPIO->tGPIOInit.Pin, GPIO_PIN_SET);
}

/**
 * @brief GPIO拉低
 * @param _tGPIO-GPIO句柄
 * @retval Null
*/
void Drv_GPIO_Reset(tagGPIO_T *_tGPIO)
{
	HAL_GPIO_WritePin(_tGPIO->tGPIOPort, _tGPIO->tGPIOInit.Pin, GPIO_PIN_RESET);
}

/**
 * @brief 读取GPIO状态
 * @param _tGPIO-GPIO句柄
 * @retval Null
*/
GPIO_PinState Drv_GPIO_Read(tagGPIO_T *_tGPIO)
{
	return HAL_GPIO_ReadPin(_tGPIO->tGPIOPort, _tGPIO->tGPIOInit.Pin);
}

/**
 * @brief GPIO状态翻转
 * @param _tGPIO-GPIO句柄
 * @retval Null
*/
void Drv_GPIO_Toggle(tagGPIO_T *_tGPIO)
{
	HAL_GPIO_TogglePin(_tGPIO->tGPIOPort,_tGPIO->tGPIOInit.Pin);
}

/**
 * @brief GPIO中断失能函数
 * @param _tGPIO-GPIO句柄
 * @retval Null
*/
void Drv_GPIO_NVICDisable(tagGPIO_T *_tGPIO)
{
	__HAL_GPIO_EXTI_CLEAR_IT(_tGPIO->tGPIOInit.Pin);
	HAL_NVIC_DisableIRQ((IRQn_Type)S_GPIO_NVICIRQnSelect(_tGPIO));
}

/**
 * @brief GPIO中断初始化函数
 * @param _tGPIO-GPIO句柄
 * @retval Null
*/
void Drv_GPIO_NVICConfig(tagGPIO_T *_tGPIO)
{	
	S_GPIO_NVICParamConfig(_tGPIO);
}

/**
 * @brief GPIO初始化函数
 * @param _tGPIO-GPIO句柄
 * @param _ucNum-初始化IO个数
 * @retval Null
*/
void Drv_GPIO_Init(tagGPIO_T *_tGPIO, uint8_t _ucNum)
{
	uint8_t index;
	
	for(index = 0;index < _ucNum;index++)
	{
		S_GPIO_CLKEnable(&_tGPIO[index]);
		S_GPIO_ParamConfig(&_tGPIO[index]);
		
		/* 如果IO配置为中断模式 */
		if((_tGPIO[index].tGPIOInit.Mode == GPIO_MODE_IT_FALLING)
		 ||(_tGPIO[index].tGPIOInit.Mode == GPIO_MODE_IT_RISING)
		 ||(_tGPIO[index].tGPIOInit.Mode == GPIO_MODE_IT_RISING_FALLING))
			S_GPIO_NVICParamConfig(&_tGPIO[index]);
	}
}

/**
 * @brief GPIO默认重置函数
 * @param _tGPIO-GPIO句柄
 * @retval Null
*/
void Drv_GPIO_DeInit(tagGPIO_T *_tGPIO)
{
	HAL_GPIO_DeInit(_tGPIO->tGPIOPort, _tGPIO->tGPIOInit.Pin);
}

/**
 * @brief GPIO中断函数
 * @param _tGPIO-GPIO句柄
 * @retval Null
*/
void Drv_GPIO_EXTI_IRQHandler(tagGPIO_T *_tGPIO)
{
	HAL_GPIO_EXTI_IRQHandler(_tGPIO->tGPIOInit.Pin);
}

#endif

