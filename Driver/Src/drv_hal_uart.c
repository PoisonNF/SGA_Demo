/****************************************************************************

* Sigma团队

* 文件名: drv_hal_uart.c

* 内容简述：串口驱动文件

* 文件历史：

* 版本号	  日期		   作者				说明
* 1.1.7 	2022-10-11   鲍程璐		新增串口DMA接收相关函数

* 1.1.4 	2022-09-03   鲍程璐		新增串口引脚重映射代码

* 1.1.1 	2022-08-04   鲍程璐		修复了串口输出不正确的问题

* 1.0.0a 	2020-02-22	 李环宇		创建该文件

****************************************************************************/
#include "drv_hal_conf.h"

#ifdef DRV_HAL_UART_ENABLE

#define TIME_OUT	0xff

/**
 * @brief 串口发送数据函数
 * @param _tUART-串口实例指针
 * @param _ucpTxData-发送数据地址指针
 * @param _uspSize-发送数据大小
 * @retval Null
*/
void Drv_Uart_Transmit(tagUART_T *_tUART, uint8_t *_ucpTxData, uint16_t _uspSize)
{	
	HAL_UART_Transmit(&_tUART->tUARTHandle, _ucpTxData, _uspSize, TIME_OUT);
}

/**
 * @brief 串口接收中断重置函数
 * @param _tUART-串口实例指针
 * @param _ucpBuffer-发送数据指针
 * @param _usSize-发送数据大小
 * @retval Null 
*/
void Drv_Uart_ReceIT_Enable(tagUART_T *_tUART, uint8_t *_ucpBuffer, uint16_t _usSize)
{
	HAL_UART_Receive_IT(&_tUART->tUARTHandle, _ucpBuffer, _usSize);
}

/**
 * @brief 串口中断配置函数
 * @param _tUART-串口实例指针
 * @retval Null 
*/
static void S_Uart_NVICConfig(tagUART_T *_tUART)
{	
	if(_tUART->tUARTHandle.Instance == USART1)
	{
		HAL_NVIC_SetPriority(USART1_IRQn, _tUART->ulPriority, _tUART->ulSubPriority);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
	}       
	else if(_tUART->tUARTHandle.Instance == USART2)
	{
		HAL_NVIC_SetPriority(USART2_IRQn, _tUART->ulPriority, _tUART->ulSubPriority);
		HAL_NVIC_EnableIRQ(USART2_IRQn);
	}
	else if(_tUART->tUARTHandle.Instance == USART3)
	{
		HAL_NVIC_SetPriority(USART3_IRQn, _tUART->ulPriority, _tUART->ulSubPriority);
		HAL_NVIC_EnableIRQ(USART3_IRQn);
	}
	else if(_tUART->tUARTHandle.Instance == UART4)
	{
		HAL_NVIC_SetPriority(UART4_IRQn, _tUART->ulPriority, _tUART->ulSubPriority);
		HAL_NVIC_EnableIRQ(UART4_IRQn);
	}
	else if(_tUART->tUARTHandle.Instance == UART5)
	{
		HAL_NVIC_SetPriority(UART5_IRQn, _tUART->ulPriority, _tUART->ulSubPriority);
		HAL_NVIC_EnableIRQ(UART5_IRQn);
	}
}

/**
 * @brief 串口DMA中断配置函数
 * @param _tUART-串口实例指针
 * @retval Null 
*/
static void S_Uart_DMA_NVICConfig(tagUART_T *_tUART)
{
	//目前只有接收
	if(_tUART->tUARTHandle.Instance == USART1)
	{
		/* DMA1_Channel5_IRQn interrupt configuration */
		HAL_NVIC_SetPriority(DMA1_Channel5_IRQn,_tUART->tUartDMA.ulDMAPriority, _tUART->tUartDMA.ulDMASubPriority);
  		HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
	}
	else if(_tUART->tUARTHandle.Instance == USART2)
	{
		/* DMA1_Channel6_IRQn interrupt configuration */
		HAL_NVIC_SetPriority(DMA1_Channel6_IRQn,_tUART->tUartDMA.ulDMAPriority, _tUART->tUartDMA.ulDMASubPriority);
		HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
	}
	else if(_tUART->tUARTHandle.Instance == USART3)
	{
		/* DMA1_Channel3_IRQn interrupt configuration */
		HAL_NVIC_SetPriority(DMA1_Channel3_IRQn,_tUART->tUartDMA.ulDMAPriority, _tUART->tUartDMA.ulDMASubPriority);
		HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
	}
	else if(_tUART->tUARTHandle.Instance == UART4)
	{
		/* DMA2_Channel3_IRQn interrupt configuration */
		HAL_NVIC_SetPriority(DMA2_Channel3_IRQn,_tUART->tUartDMA.ulDMAPriority, _tUART->tUartDMA.ulDMASubPriority);
		HAL_NVIC_EnableIRQ(DMA2_Channel3_IRQn);
	}

}


/**
 * @brief 串口参数配置函数
 * @param _tUART-串口实例指针
 * @retval Null
*/
static void S_Uart_ParamConfig(tagUART_T *_tUART)
{	
	if (HAL_UART_Init(&_tUART->tUARTHandle) != HAL_OK)
	{
		Drv_HAL_Error(__FILE__, __LINE__);
	}
}

/**
 * @brief 串口DMA参数配置函数
 * @param _tUART-串口实例指针
 * @retval Null
*/
static void S_Uart_DMAParamConfig(tagUART_T *_tUART)
{
	//目前只有接收
	if(HAL_DMA_Init(&_tUART->tUartDMA.tDMARx) != HAL_OK)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
	}
}

/**
 * @brief 串口时钟初始化函数---L4使用
 * @param null
 * @retval Null
*/
static void S_Uart_CLKConfig(void)
{
#if defined (STM32L4_SGA_ENABLE)
/* Select UART function start */
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
	
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2
                              |RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_UART4
                              |RCC_PERIPHCLK_UART5;
	PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
	PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
	PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
	PeriphClkInit.Uart4ClockSelection = RCC_UART4CLKSOURCE_PCLK1;
	PeriphClkInit.Uart5ClockSelection = RCC_UART5CLKSOURCE_PCLK1;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Drv_HAL_Error(__FILE__, __LINE__);
	}
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
	{
		Drv_HAL_Error(__FILE__, __LINE__);
	}
/* Select UART function end */	
#endif
}

/**
 * @brief 串口时钟使能函数
 * @param *_tUART-串口结构体指针
 * @retval Null
*/
static void S_UART_CLKEnable(tagUART_T *_tUART)
{
	if(_tUART->tUARTHandle.Instance == USART1)
	{
		__HAL_RCC_USART1_CLK_ENABLE();
	}
	else if(_tUART->tUARTHandle.Instance == USART2)
	{
		__HAL_RCC_USART2_CLK_ENABLE();
	}
	else if(_tUART->tUARTHandle.Instance == USART3)
	{
		__HAL_RCC_USART3_CLK_ENABLE();
	}
	else if(_tUART->tUARTHandle.Instance == UART4)
	{
		__HAL_RCC_UART4_CLK_ENABLE();
	}
	else if(_tUART->tUARTHandle.Instance == UART5)
	{
		__HAL_RCC_UART5_CLK_ENABLE();
	}
}

/**
 * @brief DMA时钟使能函数
 * @param *_tUART-串口结构体指针
 * @retval Null
*/
static void S_UART_DMA_CLKEnable(tagUART_T *_tUART)
{
	if(_tUART->tUARTHandle.Instance == USART1)			__HAL_RCC_DMA1_CLK_ENABLE();
	else if(_tUART->tUARTHandle.Instance == USART2)		__HAL_RCC_DMA1_CLK_ENABLE();
	else if(_tUART->tUARTHandle.Instance == USART3)		__HAL_RCC_DMA1_CLK_ENABLE();
	else if(_tUART->tUARTHandle.Instance == UART4)		__HAL_RCC_DMA2_CLK_ENABLE();
	else if(_tUART->tUARTHandle.Instance == UART5)		while(1);	//uart5不能使用DMA
}

/**
 * @brief UART的GPIO参数配置函数
 * @param *_tUART-串口句柄指针
 * @retval Null
*/
static void S_UART_GPIOConfig(tagUART_T *_tUART)
{
	/* 开启复用模式时钟 */
	__HAL_RCC_AFIO_CLK_ENABLE();

	/* 根据不同串口的AFMode开启对应的重映射，重映射表在drv_hal_uart.h中 */
	if(_tUART->tUARTHandle.Instance == USART1)
	{
		if(_tUART->tGPIO->AFMode == NO_REMAP)		    	__HAL_AFIO_REMAP_USART1_DISABLE();
		else if(_tUART->tGPIO->AFMode == FULL_REMAP) 	    __HAL_AFIO_REMAP_USART1_ENABLE();
		else if(_tUART->tGPIO->AFMode == PARTIAL_REMAP)	    while(1);
		else if(_tUART->tGPIO->AFMode == PARTIAL_REMAP2)	while(1);
	}
	else if(_tUART->tUARTHandle.Instance == USART2)
	{
		if(_tUART->tGPIO->AFMode == NO_REMAP)		    	__HAL_AFIO_REMAP_USART2_DISABLE();
		else if(_tUART->tGPIO->AFMode == PARTIAL_REMAP)	    while(1);
		else if(_tUART->tGPIO->AFMode == PARTIAL_REMAP2)	while(1);
		else if(_tUART->tGPIO->AFMode == FULL_REMAP)		__HAL_AFIO_REMAP_USART2_ENABLE();
	}	
	else if(_tUART->tUARTHandle.Instance == USART3)
	{
		if(_tUART->tGPIO->AFMode == NO_REMAP)		    	__HAL_AFIO_REMAP_USART3_DISABLE();
		else if(_tUART->tGPIO->AFMode == PARTIAL_REMAP)	    __HAL_AFIO_REMAP_USART3_PARTIAL();
		else if(_tUART->tGPIO->AFMode == PARTIAL_REMAP2)	while(1);
		else if(_tUART->tGPIO->AFMode == FULL_REMAP)		__HAL_AFIO_REMAP_USART3_ENABLE();
	}
	
	Drv_GPIO_Init(_tUART->tGPIO, 2); 	/* GPIO初始化 */
}

/**
 * @brief 串口普通中断模式初始化函数
 * @param _tUART-串口结构体指针
 * @retval Null 
*/
void Drv_Uart_ITInit(tagUART_T *_tUART)
{
	S_Uart_CLKConfig();
	S_UART_CLKEnable(_tUART);
	S_UART_GPIOConfig(_tUART);
	S_Uart_ParamConfig(_tUART);		/* 设置串口参数 */
	S_Uart_NVICConfig(_tUART);		/* 设置中断优先级 */

	HAL_UART_Receive_IT(&_tUART->tUARTHandle, _tUART->tRxInfo.ucpRxBuffer, 1);
}

/**
 * @brief 串口DMA模式初始化函数
 * @param _tUART-串口结构体指针
 * @retval Null 
*/
void Drv_Uart_DMAInit(tagUART_T *_tUART)
{
	S_Uart_CLKConfig();				/* L4所需 */

	S_UART_DMA_CLKEnable(_tUART);
	S_Uart_DMA_NVICConfig(_tUART);
	S_Uart_DMAParamConfig(_tUART);

	S_UART_CLKEnable(_tUART);
	S_UART_GPIOConfig(_tUART);
	S_Uart_NVICConfig(_tUART);		/* 设置中断优先级 */
	S_Uart_ParamConfig(_tUART);		/* 设置串口参数 */

	__HAL_LINKDMA(&_tUART->tUARTHandle,hdmarx,_tUART->tUartDMA.tDMARx);
	_tUART->tRxInfo.ucpRxCache = (uint8_t *)malloc(_tUART->tRxInfo.usRxMAXLenth);	//为cache申请一段长度的动态内存
	__HAL_UART_ENABLE_IT(&_tUART->tUARTHandle,UART_IT_IDLE); //打开空闲中断
	HAL_UART_Receive_DMA(&_tUART->tUARTHandle,_tUART->tRxInfo.ucpRxCache,_tUART->tRxInfo.usRxMAXLenth);//开启DMA接收

}

/**
 * @brief 串口1中断帽子函数
 * @param _tUART-串口结构体指针
 * @retval Null
*/
void Drv_Uart_IRQHandler(tagUART_T *_tUART)
{
	HAL_UART_IRQHandler(&_tUART->tUARTHandle);
}

/**
 * @brief 串口DMA中断接收处理子函数(用于串口中断处理函数中)
 * @param _tUART-串口结构体指针
 * @retval Null
*/
void Drv_Uart_DMA_Handler(tagUART_T *_tUART)
{
	if(__HAL_UART_GET_FLAG(&_tUART->tUARTHandle,UART_FLAG_IDLE) != RESET) // 空闲中断标记被置位
	{
		__HAL_UART_CLEAR_IDLEFLAG(&_tUART->tUARTHandle);// 清除中断标记
		HAL_UART_DMAStop(&_tUART->tUARTHandle);// 停止DMA接收
		_tUART->tRxInfo.usRxLenth = _tUART->tRxInfo.usRxMAXLenth - __HAL_DMA_GET_COUNTER(_tUART->tUARTHandle.hdmarx);// 总数据量减去未接收到的数据量为已经接收到的数据量
        _tUART->tUartDMA.DMARxCplt = 1;// 标记接收结束
        HAL_UART_Receive_DMA(&_tUART->tUARTHandle,_tUART->tRxInfo.ucpRxCache,_tUART->tRxInfo.usRxMAXLenth);// 重新启动DMA接收
	}
}


#endif
