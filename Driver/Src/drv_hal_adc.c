/****************************************************************************

* Sigma团队

* 文件名: drv_hal_adc.c

* 内容简述：ADC驱动文件

* 文件历史：

* 版本号	日期		作者		说明
*  3.0 	   2024-01-26	鲍程璐		适配STM32F4系列

* 1.0.0a   2020-02-22	李环宇		创建该文件

****************************************************************************/
#include "drv_hal_conf.h"

#ifdef DRV_HAL_ADC_ENABLE

/**
 * @brief ADC时钟配置函数
 * @param Null
 * @retval Null
*/
static void S_ADC_CLKConfig(void)
{
#ifdef STM32F1_SGA_ENABLE
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
	
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Drv_HAL_Error(__FILE__, __LINE__);
	}
#endif
}

/**
 * @brief ADC时钟使能函数
 * @param _tADC-ADC结构体指针
 * @retval Null
*/
static void S_ADC_CLKEnable(tagADC_T *_tADC)
{
	if(_tADC->tADCHandle.Instance == ADC1)
	{
		__HAL_RCC_ADC1_CLK_ENABLE();
	}
	else if(_tADC->tADCHandle.Instance == ADC2)
	{
		__HAL_RCC_ADC2_CLK_ENABLE();
	}
	else if(_tADC->tADCHandle.Instance == ADC3)
	{
		__HAL_RCC_ADC3_CLK_ENABLE();
	}
}

/**
 * @brief ADC参数配置函数
 * @param _tADC-ADC结构体指针
 * @retval Null
*/
static void S_ADC_GPIOConfig(tagADC_T *_tADC)
{
	Drv_GPIO_Init(&_tADC->tGPIO, 1);
}

/**
 * @brief ADC参数配置函数
 * @param _tADC-ADC结构体指针
 * @retval Null
*/
static void S_ADC_ParamConfig(tagADC_T *_tADC)
{
	if (HAL_ADC_Init(&_tADC->tADCHandle) != HAL_OK)
	{
		Drv_HAL_Error(__FILE__, __LINE__);
	}

#ifdef STM32F1_SGA_ENABLE	
	HAL_ADCEx_Calibration_Start(&_tADC->tADCHandle);				  /* 校准ADC */
#endif
	
	if (HAL_ADC_ConfigChannel(&_tADC->tADCHandle, &_tADC->tADCChannel) != HAL_OK)
	{
		Drv_HAL_Error(__FILE__, __LINE__);
	}
}

/**
 * @brief ADC初始化函数
 * @param _tADC-ADC结构体指针
 * @param _ucNum-初始化ADC个数
 * @retval Null
*/
void Drv_ADC_Init(tagADC_T *_tADC, uint8_t _ucNum)
{
	uint8_t index;
	
	for(index = 0; index < _ucNum; index++)
	{
		S_ADC_CLKConfig();					/* ADC时钟树配置 */
		S_ADC_CLKEnable(&_tADC[index]);		/* ADC时钟使能 */
		S_ADC_GPIOConfig(&_tADC[index]);	/* ADC的GPIO配置 */
		S_ADC_ParamConfig(&_tADC[index]);	/* ADC参数配置 */
	}
}

/**
 * @brief ADC轮询读取函数
 * @param _tADC-ADC结构体指针
 * @retval uint16_t-读取到的ADC直接值
*/
uint16_t Drv_ADC_PollGetData(tagADC_T *_tADC)
{
	HAL_ADC_ConfigChannel(&_tADC->tADCHandle, &_tADC->tADCChannel);	/* 通道配置 */
		
	HAL_ADC_Start(&_tADC->tADCHandle);								/* 开启ADC */
	HAL_ADC_PollForConversion(&_tADC->tADCHandle, POLL_NUM);		/* 轮询转换 */
	
	if(HAL_IS_BIT_SET(HAL_ADC_GetState(&_tADC->tADCHandle),HAL_ADC_STATE_REG_EOC))
		return (uint16_t)HAL_ADC_GetValue(&_tADC->tADCHandle);
	else	
		return (uint16_t)HAL_ADC_GetState(&_tADC->tADCHandle);
}

/**
 * @brief ADC轮询读取函数
 * @param _tADC-ADC结构体指针
 * @retval float-读取到的ADC转换后的实际值
*/
float Drv_ADC_PollGetValue(tagADC_T *_tADC)
{
	uint32_t ulADCData = 0;
	uint8_t ucNum;
	float fADCVal;
	
	/* 循环读取，取平均值 */
	for(ucNum = 0;ucNum < _tADC->ucAverageNum;ucNum++)
	{
		ulADCData += Drv_ADC_PollGetData(_tADC);		/* 返回最近一次ADC规则组的转换结果 */
		Drv_Delay_Ms(5);
	}
	
	ulADCData /= _tADC->ucAverageNum;			/* 取平均值 */
	fADCVal = (float)ulADCData * (3.3 / 4096);	/* 算式换算 */
	
	return fADCVal;
}

#endif
