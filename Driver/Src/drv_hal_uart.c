/****************************************************************************

* Sigma团队

* 文件名: drv_hal_uart.c

* 内容简述：串口驱动文件

* 文件历史：

* 版本号	  日期		  作者				说明
*  2.9 		2023-11-26   鲍程璐		增加串口DMA通道中断处理子函数

*  2.8 		2023-10-15   鲍程璐		修复串口DMA发送的问题

*  2.7		2023-07-06   鲍程璐		串口初始化结构体增加默认值，串口中断接收处理子函数增加结尾符检测参数

*  2.4		2023-05-12   鲍程璐		新增串口中断接收相关函数，格式优化

* 2.2.1		2023-04-03   鲍程璐		串口信息结构体成员更改，新增串口DMA数据接收函数

*  1.2		2023-02-26   鲍程璐		新增串口DMA发送相关函数，优化注释

* 1.1.7 	2022-10-11   鲍程璐		新增串口DMA接收相关函数

* 1.1.4 	2022-09-03   鲍程璐		新增串口引脚重映射代码

* 1.1.1 	2022-08-04   鲍程璐		修复了串口输出不正确的问题

* 1.0.0a 	2020-02-22	 李环宇		创建该文件

****************************************************************************/
#include "drv_hal_conf.h"

#ifdef DRV_HAL_UART_ENABLE

/**
 * @brief 串口中断配置函数
 * @param _tUART-串口实例指针
 * @retval Null 
*/
static void S_Uart_NVICConfig(tagUART_T *_tUART)
{	
	if(_tUART->tUARTHandle.Instance == USART1)
	{
		HAL_NVIC_SetPriority(USART1_IRQn, _tUART->ucPriority, _tUART->ucSubPriority);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
	}       
	else if(_tUART->tUARTHandle.Instance == USART2)
	{
		HAL_NVIC_SetPriority(USART2_IRQn, _tUART->ucPriority, _tUART->ucSubPriority);
		HAL_NVIC_EnableIRQ(USART2_IRQn);
	}
	else if(_tUART->tUARTHandle.Instance == USART3)
	{
		HAL_NVIC_SetPriority(USART3_IRQn, _tUART->ucPriority, _tUART->ucSubPriority);
		HAL_NVIC_EnableIRQ(USART3_IRQn);
	}
	else if(_tUART->tUARTHandle.Instance == UART4)
	{
		HAL_NVIC_SetPriority(UART4_IRQn, _tUART->ucPriority, _tUART->ucSubPriority);
		HAL_NVIC_EnableIRQ(UART4_IRQn);
	}
	else if(_tUART->tUARTHandle.Instance == UART5)
	{
		HAL_NVIC_SetPriority(UART5_IRQn, _tUART->ucPriority, _tUART->ucSubPriority);
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
	/* DMA接收使能 */
	if(_tUART->tUartDMA.bRxEnable == true)
	{
		if(_tUART->tUARTHandle.Instance == USART1)
		{
			/* DMA1_Channel5_IRQn interrupt configuration */
			HAL_NVIC_SetPriority(DMA1_Channel5_IRQn,_tUART->tUartDMA.ucDMARxPriority, _tUART->tUartDMA.ucDMARxSubPriority);
			HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
		}
		else if(_tUART->tUARTHandle.Instance == USART2)
		{
			/* DMA1_Channel6_IRQn interrupt configuration */
			HAL_NVIC_SetPriority(DMA1_Channel6_IRQn,_tUART->tUartDMA.ucDMARxPriority, _tUART->tUartDMA.ucDMARxSubPriority);
			HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
		}
		else if(_tUART->tUARTHandle.Instance == USART3)
		{
			/* DMA1_Channel3_IRQn interrupt configuration */
			HAL_NVIC_SetPriority(DMA1_Channel3_IRQn,_tUART->tUartDMA.ucDMARxPriority, _tUART->tUartDMA.ucDMARxSubPriority);
			HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
		}
		else if(_tUART->tUARTHandle.Instance == UART4)
		{
			/* DMA2_Channel3_IRQn interrupt configuration */
			HAL_NVIC_SetPriority(DMA2_Channel3_IRQn,_tUART->tUartDMA.ucDMARxPriority, _tUART->tUartDMA.ucDMARxSubPriority);
			HAL_NVIC_EnableIRQ(DMA2_Channel3_IRQn);
		}
	}
	
	/* DMA发送使能 */
	if(_tUART->tUartDMA.bTxEnable == true)
	{
		if(_tUART->tUARTHandle.Instance == USART1)
		{
			/* DMA1_Channel4_IRQn interrupt configuration */
			HAL_NVIC_SetPriority(DMA1_Channel4_IRQn,_tUART->tUartDMA.ucDMATxPriority,_tUART->tUartDMA.ucDMATxSubPriority);
			HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
		}
		else if(_tUART->tUARTHandle.Instance == USART2)
		{
			/* DMA1_Channel7_IRQn interrupt configuration */
			HAL_NVIC_SetPriority(DMA1_Channel7_IRQn,_tUART->tUartDMA.ucDMATxPriority,_tUART->tUartDMA.ucDMATxSubPriority);
			HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);
		}
		else if(_tUART->tUARTHandle.Instance == USART3)
		{
			/* DMA1_Channel2_IRQn interrupt configuration */
			HAL_NVIC_SetPriority(DMA1_Channel2_IRQn,_tUART->tUartDMA.ucDMATxPriority,_tUART->tUartDMA.ucDMATxSubPriority);
			HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
		}
		else if(_tUART->tUARTHandle.Instance == UART4)
		{
			/* DMA2_Channel5_IRQn interrupt configuration */
			HAL_NVIC_SetPriority(DMA2_Channel4_5_IRQn,_tUART->tUartDMA.ucDMATxPriority,_tUART->tUartDMA.ucDMATxSubPriority);
			HAL_NVIC_EnableIRQ(DMA2_Channel4_5_IRQn);
		}
	}
}

/**
 * @brief 串口参数配置函数
 * @param _tUART-串口实例指针
 * @retval Null
*/
static void S_Uart_ParamConfig(tagUART_T *_tUART)
{
	/* 调用串口初始化函数 */
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
	/* 如果使能DMA接收 */
	if(_tUART->tUartDMA.bRxEnable == true)
	{
		if(HAL_DMA_Init(&_tUART->tUartDMA.tDMARx) != HAL_OK)	Drv_HAL_Error(__FILE__,__LINE__);
	}

	/* 如果使能DMA发送 */
	if(_tUART->tUartDMA.bTxEnable == true)
	{
		if(HAL_DMA_Init(&_tUART->tUartDMA.tDMATx) != HAL_OK)	Drv_HAL_Error(__FILE__,__LINE__);
	}
}

/**
 * @brief 串口时钟初始化函数---L4使用
 * @param Null
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
 * @param _tUART-串口结构体指针
 * @retval Null
*/
static void S_Uart_CLKEnable(tagUART_T *_tUART)
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
 * @param _tUART-串口结构体指针
 * @retval Null
*/
static void S_Uart_DMA_CLKEnable(tagUART_T *_tUART)
{
	if(_tUART->tUARTHandle.Instance == USART1)			__HAL_RCC_DMA1_CLK_ENABLE();
	else if(_tUART->tUARTHandle.Instance == USART2)		__HAL_RCC_DMA1_CLK_ENABLE();
	else if(_tUART->tUARTHandle.Instance == USART3)		__HAL_RCC_DMA1_CLK_ENABLE();
	else if(_tUART->tUARTHandle.Instance == UART4)		__HAL_RCC_DMA2_CLK_ENABLE();
	else if(_tUART->tUARTHandle.Instance == UART5)
	{
		/* Uart5不能使用DMA */
		Drv_HAL_Error(__FILE__,__LINE__);
		while(1);
	}
}

/**
 * @brief UART的GPIO参数配置函数
 * @param _tUART-串口句柄指针
 * @retval Null
*/
static void S_Uart_GPIOConfig(tagUART_T *_tUART)
{
	/* 开启复用模式时钟 */
	__HAL_RCC_AFIO_CLK_ENABLE();

	/* 根据不同串口的ucAFMode开启对应的重映射，重映射表在drv_hal_uart.h中 */
	if(_tUART->tUARTHandle.Instance == USART1)
	{
		if(_tUART->tGPIO->ucAFMode == NO_REMAP)		    	__HAL_AFIO_REMAP_USART1_DISABLE();
		else if(_tUART->tGPIO->ucAFMode == FULL_REMAP) 	    __HAL_AFIO_REMAP_USART1_ENABLE();
		else if(_tUART->tGPIO->ucAFMode == PARTIAL_REMAP)	while(1);
		else if(_tUART->tGPIO->ucAFMode == PARTIAL_REMAP2)	while(1);
	}
	else if(_tUART->tUARTHandle.Instance == USART2)
	{
		if(_tUART->tGPIO->ucAFMode == NO_REMAP)		    	__HAL_AFIO_REMAP_USART2_DISABLE();
		else if(_tUART->tGPIO->ucAFMode == PARTIAL_REMAP)	while(1);
		else if(_tUART->tGPIO->ucAFMode == PARTIAL_REMAP2)	while(1);
		else if(_tUART->tGPIO->ucAFMode == FULL_REMAP)		__HAL_AFIO_REMAP_USART2_ENABLE();
	}	
	else if(_tUART->tUARTHandle.Instance == USART3)
	{
		if(_tUART->tGPIO->ucAFMode == NO_REMAP)		    	__HAL_AFIO_REMAP_USART3_DISABLE();
		else if(_tUART->tGPIO->ucAFMode == PARTIAL_REMAP)	__HAL_AFIO_REMAP_USART3_PARTIAL();
		else if(_tUART->tGPIO->ucAFMode == PARTIAL_REMAP2)	while(1);
		else if(_tUART->tGPIO->ucAFMode == FULL_REMAP)		__HAL_AFIO_REMAP_USART3_ENABLE();
	}
	
	Drv_GPIO_Init(_tUART->tGPIO, 2); 	/* GPIO初始化 */
}

/**
 * @brief UART参数匹配函数
 * @param _tUART-串口句柄指针
 * @note 如果没有指定参数就设置为默认（串口1,115200,8,1,N）
 * @retval Null
*/
static void S_Uart_ParamMatch(tagUART_T *_tUART)
{
	/* 串口参数 */
	DEFAULT(_tUART->tUARTHandle.Instance,USART1);
	DEFAULT(_tUART->tUARTHandle.Init.BaudRate,115200);
	DEFAULT(_tUART->tUARTHandle.Init.Mode,UART_MODE_TX_RX);
	DEFAULT(_tUART->tUARTHandle.Init.WordLength,UART_WORDLENGTH_8B);
	DEFAULT(_tUART->tUARTHandle.Init.StopBits,UART_STOPBITS_1);
	DEFAULT(_tUART->tUARTHandle.Init.Parity,UART_PARITY_NONE);
	DEFAULT(_tUART->ucPriority,1);
	DEFAULT(_tUART->ucSubPriority,3);

	/* 串口DMA参数 */
	if(_tUART->tUartDMA.bTxEnable == true)
	{
		/* 根据串口号选择DMA通道 */
		if(_tUART->tUARTHandle.Instance == USART1)
			_tUART->tUartDMA.tDMATx.Instance = DMA1_Channel4;
		else if(_tUART->tUARTHandle.Instance == USART2)
			_tUART->tUartDMA.tDMATx.Instance = DMA1_Channel7;
		else if(_tUART->tUARTHandle.Instance == USART3)
			_tUART->tUartDMA.tDMATx.Instance = DMA1_Channel2;
		else if(_tUART->tUARTHandle.Instance == UART4)
			_tUART->tUartDMA.tDMATx.Instance = DMA2_Channel5;

		/* DMA工作模式 */
		DEFAULT(_tUART->tUartDMA.tDMATx.Init.Direction,DMA_MEMORY_TO_PERIPH);
		DEFAULT(_tUART->tUartDMA.tDMATx.Init.PeriphInc,DMA_PINC_DISABLE);
		DEFAULT(_tUART->tUartDMA.tDMATx.Init.MemInc,DMA_MINC_ENABLE);
		DEFAULT(_tUART->tUartDMA.tDMATx.Init.PeriphDataAlignment,DMA_PDATAALIGN_BYTE);
		DEFAULT(_tUART->tUartDMA.tDMATx.Init.MemDataAlignment,DMA_MDATAALIGN_BYTE);
		DEFAULT(_tUART->tUartDMA.tDMATx.Init.Mode,DMA_NORMAL);
		DEFAULT(_tUART->tUartDMA.tDMATx.Init.Priority,DMA_PRIORITY_LOW);

		/* DMA中断优先级 */
		DEFAULT(_tUART->tUartDMA.ucDMATxPriority,1);
		DEFAULT(_tUART->tUartDMA.ucDMATxSubPriority,1);

		/* DMA发送缓冲区大小 */
		DEFAULT(_tUART->tTxInfo.usDMATxMAXSize, 100);
	}

	if(_tUART->tUartDMA.bRxEnable == true)
	{
		/* 根据串口号选择DMA通道 */
		if(_tUART->tUARTHandle.Instance == USART1)
			_tUART->tUartDMA.tDMARx.Instance = DMA1_Channel5;
		else if(_tUART->tUARTHandle.Instance == USART2)
			_tUART->tUartDMA.tDMARx.Instance = DMA1_Channel6;
		else if(_tUART->tUARTHandle.Instance == USART3)
			_tUART->tUartDMA.tDMARx.Instance = DMA1_Channel3;
		else if(_tUART->tUARTHandle.Instance == UART4)
			_tUART->tUartDMA.tDMARx.Instance = DMA2_Channel3;

		/* DMA工作模式 */
		DEFAULT(_tUART->tUartDMA.tDMARx.Init.Direction,DMA_PERIPH_TO_MEMORY);
		DEFAULT(_tUART->tUartDMA.tDMARx.Init.PeriphInc,DMA_PINC_DISABLE);
		DEFAULT(_tUART->tUartDMA.tDMARx.Init.MemInc,DMA_MINC_ENABLE);
		DEFAULT(_tUART->tUartDMA.tDMARx.Init.PeriphDataAlignment,DMA_PDATAALIGN_BYTE);
		DEFAULT(_tUART->tUartDMA.tDMARx.Init.MemDataAlignment,DMA_MDATAALIGN_BYTE);
		DEFAULT(_tUART->tUartDMA.tDMARx.Init.Mode,DMA_CIRCULAR);
		DEFAULT(_tUART->tUartDMA.tDMARx.Init.Priority,DMA_PRIORITY_LOW);
		
		/* DMA中断优先级 */
		DEFAULT(_tUART->tUartDMA.ucDMARxPriority,1);
		DEFAULT(_tUART->tUartDMA.ucDMARxSubPriority,1);

		/* DMA接收缓冲区大小 */
		DEFAULT(_tUART->tRxInfo.usDMARxMAXSize, 100);
	}

	/* 串口I/O设置 [0]TX [1]RX */
	/* 如果复用I/O则退出函数 */
	if(_tUART->tGPIO[0].tGPIOInit.Pin != 0 && _tUART->tGPIO[1].tGPIOInit.Pin != 0)
		return;

	/* 根据使用的串口选择相对应的默认I/O */
	if(_tUART->tUARTHandle.Instance == USART1)
	{
		_tUART->tGPIO[0].tGPIOInit.Pin 	= GPIO_PIN_9;
		_tUART->tGPIO[0].tGPIOPort 		= GPIOA;

		_tUART->tGPIO[1].tGPIOInit.Pin 	= GPIO_PIN_10;
		_tUART->tGPIO[1].tGPIOPort 		= GPIOA;
	}
	else if(_tUART->tUARTHandle.Instance == USART2)
	{
		_tUART->tGPIO[0].tGPIOInit.Pin 	= GPIO_PIN_2;
		_tUART->tGPIO[0].tGPIOPort 		= GPIOA;

		_tUART->tGPIO[1].tGPIOInit.Pin 	= GPIO_PIN_3;
		_tUART->tGPIO[1].tGPIOPort 		= GPIOA;
	}
	else if(_tUART->tUARTHandle.Instance == USART3)
	{
		_tUART->tGPIO[0].tGPIOInit.Pin 	= GPIO_PIN_10;
		_tUART->tGPIO[0].tGPIOPort 		= GPIOB;

		_tUART->tGPIO[1].tGPIOInit.Pin 	= GPIO_PIN_11;
		_tUART->tGPIO[1].tGPIOPort 		= GPIOB;
	}
	else if(_tUART->tUARTHandle.Instance == UART4)
	{
		_tUART->tGPIO[0].tGPIOInit.Pin 	= GPIO_PIN_10;
		_tUART->tGPIO[0].tGPIOPort 		= GPIOC;

		_tUART->tGPIO[1].tGPIOInit.Pin 	= GPIO_PIN_11;
		_tUART->tGPIO[1].tGPIOPort 		= GPIOC;
	}
	else if(_tUART->tUARTHandle.Instance == UART5)
	{
		_tUART->tGPIO[0].tGPIOInit.Pin 	= GPIO_PIN_12;
		_tUART->tGPIO[0].tGPIOPort 		= GPIOC;

		_tUART->tGPIO[1].tGPIOInit.Pin 	= GPIO_PIN_2;
		_tUART->tGPIO[1].tGPIOPort 		= GPIOD;
	}

	_tUART->tGPIO[0].tGPIOInit.Mode = GPIO_MODE_AF_PP;
	_tUART->tGPIO[1].tGPIOInit.Mode = GPIO_MODE_INPUT;

	_tUART->tGPIO->tGPIOInit.Pull 	= GPIO_NOPULL;
	_tUART->tGPIO->tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH;
	_tUART->tGPIO->ucAFMode 		= NO_REMAP;
}

/**
 * @brief 串口发送数据函数
 * @param _tUART-串口实例指针
 * @param _ucpTxData-发送数据地址指针
 * @param _usSize-发送数据大小
 * @retval Null
*/
void Drv_Uart_Transmit(tagUART_T *_tUART, uint8_t *_ucpTxData, uint16_t _usSize)
{
	HAL_UART_Transmit(&_tUART->tUARTHandle, _ucpTxData, _usSize, UART_TIME_OUT);
}

/**
 * @brief 串口中断发送数据函数
 * @param _tUART-串口实例指针
 * @param _ucpTxData-发送数据地址指针
 * @param _usSize-发送数据大小
 * @retval Null
*/
void Drv_Uart_Transmit_IT(tagUART_T *_tUART, uint8_t *_ucpTxData, uint16_t _usSize)
{
	HAL_UART_Transmit_IT(&_tUART->tUARTHandle, _ucpTxData, _usSize);
}

/**
 * @brief 串口DMA发送数据函数
 * @param _tUART-串口实例指针
 * @param _ucpTxData-发送数据地址指针
 * @param _usSize-发送数据大小
 * @retval Null
*/
void Drv_Uart_Transmit_DMA(tagUART_T *_tUART, uint8_t *_ucpTxData, uint16_t _usSize)
{
	/* 获取DMA状态 */
	while(HAL_DMA_GetState(&_tUART->tUartDMA.tDMATx) != HAL_DMA_STATE_READY);

	_tUART->tTxInfo.usDMATxLength = _usSize;

	/* 准备状态即可发送 */
	HAL_UART_Transmit_DMA(&_tUART->tUARTHandle, _ucpTxData, _tUART->tTxInfo.usDMATxLength);
}

/**
 * @brief 串口中断接收数据函数
 * @param _tUART-串口实例指针
 * @param _ucpRxData-接收数据地址指针
 * @retval uint16_t 接收到的长度
*/
uint16_t Drv_Uart_Receive_IT(tagUART_T *_tUART, uint8_t *_ucpRxData)
{
	if(_tUART->tRxInfo.ucRxCplt)
	{
		/* 数据拷贝 */
		memcpy(_ucpRxData,_tUART->tRxInfo.ucpITRxCache,_tUART->tRxInfo.usRxLength);

		/* 标志位清零 */
		_tUART->tRxInfo.ucRxCplt = 0;

		return _tUART->tRxInfo.usRxLength;
	}
	
	return 0;
}

/**
 * @brief 串口DMA接收数据函数
 * @param _tUART-串口实例指针
 * @param _ucpRxData-接收数据地址指针
 * @retval uint16_t usRxNum 接收到的长度
*/
uint16_t Drv_Uart_Receive_DMA(tagUART_T *_tUART, uint8_t *_ucpRxData)
{
	uint16_t usRxNum;
    
	/* 判断是否接收完成 */
	if(_tUART->tRxInfo.ucDMARxCplt != 1)
	{
		return 0;
	}
	else
	{
		/* 将接收数据的长度保存 */
		usRxNum = _tUART->tRxInfo.usDMARxLength;

		/* 清空接收数据地址指针 */
        memset(_ucpRxData,0,usRxNum+1);

		/* 数据拷贝 */
		memcpy(_ucpRxData,_tUART->tRxInfo.ucpDMARxCache,usRxNum);

		/* 清理标志位 */
		_tUART->tRxInfo.ucDMARxCplt = 0;

		return usRxNum;
	}
}

/**
 * @brief 串口接收中断重置函数
 * @param _tUART-串口实例指针
 * @param _ucpRxData-接收数据指针
 * @param _usSize-接收数据大小
 * @retval Null 
*/
void Drv_Uart_ReceIT_Enable(tagUART_T *_tUART, uint8_t *_ucpRxData, uint16_t _usSize)
{
	HAL_UART_Receive_IT(&_tUART->tUARTHandle, _ucpRxData, _usSize);
}

/**
 * @brief 串口普通中断模式初始化函数
 * @param _tUART-串口结构体指针
 * @retval Null 
*/
void Drv_Uart_ITInit(tagUART_T *_tUART)
{
	S_Uart_CLKConfig();
	S_Uart_ParamMatch(_tUART);
	S_Uart_CLKEnable(_tUART);
	S_Uart_GPIOConfig(_tUART);
	S_Uart_ParamConfig(_tUART);		/* 设置串口参数 */
	S_Uart_NVICConfig(_tUART);		/* 设置中断优先级 */

	/* 为cache申请一段长度的动态内存 */
	_tUART->tRxInfo.ucpITRxCache = (uint8_t *)malloc(UART_IT_RX_CACHE_SIZE);

	/* 往一级缓冲区中接收一个字符 */
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
	
	/* 参数匹配 */
	S_Uart_ParamMatch(_tUART);

	/* DMA配置 */
	S_Uart_DMA_CLKEnable(_tUART);
	S_Uart_DMA_NVICConfig(_tUART);	/* DMA中断配置 */
	S_Uart_DMAParamConfig(_tUART);	/* 设置DMA参数 */

	/* 串口配置 */
	S_Uart_CLKEnable(_tUART);
	S_Uart_GPIOConfig(_tUART);
	S_Uart_NVICConfig(_tUART);		/* 串口中断配置 */
	S_Uart_ParamConfig(_tUART);		/* 设置串口参数 */

	/* 如果使能DMA接收 */
	if(_tUART->tUartDMA.bRxEnable == true)
	{
		/* 关联串口和DMA接收 */
		__HAL_LINKDMA(&_tUART->tUARTHandle,hdmarx,_tUART->tUartDMA.tDMARx);

		/* 为cache申请一段长度的动态内存 */
		_tUART->tRxInfo.ucpDMARxCache = (uint8_t *)malloc(_tUART->tRxInfo.usDMARxMAXSize);

		/*	打开空闲中断 */
		__HAL_UART_ENABLE_IT(&_tUART->tUARTHandle,UART_IT_IDLE);

		/* 开启DMA接收 */
		HAL_UART_Receive_DMA(&_tUART->tUARTHandle,_tUART->tRxInfo.ucpDMARxCache,_tUART->tRxInfo.usDMARxMAXSize);
	}

	/* 如果使能DMA发送 */
	if(_tUART->tUartDMA.bTxEnable == true)
	{
		/* 关联串口和DMA发送 */
		__HAL_LINKDMA(&_tUART->tUARTHandle,hdmatx,_tUART->tUartDMA.tDMATx);

		/* 为cache申请一段长度的动态内存 */
		_tUART->tTxInfo.ucpDMATxCache = (uint8_t *)malloc(_tUART->tTxInfo.usDMATxMAXSize);
	}
}

/**
 * @brief 串口中断帽子函数(用于串口中断处理函数中)
 * @note 在类似void USART1_IRQHandler(void)函数中调用，根据使用的串口号进行更改
 * @param _tUART-串口结构体指针
 * @retval Null
*/
void Drv_Uart_IRQHandler(tagUART_T *_tUART)
{
	HAL_UART_IRQHandler(&_tUART->tUARTHandle);
}

/**
 * @brief 串口中断接收处理子函数(用于串口接收完成回调函数中)
 * @note 在函数名为void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)中调用
 * @param _tUART-串口结构体指针
 * @param _ucEndChar 需要检测的结尾符
 * @note 数据保存的起始地址为ucpITRxCache,接收到的长度为usRxLength
 * @retval Null
*/
void Drv_Uart_IT_RxHandler(tagUART_T *_tUART, uint8_t _ucEndChar)
{
	/* 检测到结尾符ucEndChar代表接收结束 */
	if(_tUART->tRxInfo.ucpRxBuffer[0] == _ucEndChar)
	{
		/* 替换为'\0'表示字符串结尾 */
		_tUART->tRxInfo.ucpITRxCache[_tUART->tRxInfo.usRxCnt] = '\0';

		/* 保存接收到的长度 */
		_tUART->tRxInfo.usRxLength = _tUART->tRxInfo.usRxCnt;

		/* 接收计数器清零 */
		_tUART->tRxInfo.usRxCnt = 0;

		/* 接收完成标志位置1 */
		_tUART->tRxInfo.ucRxCplt = 1;
	}

	/* 未到结尾继续接收 */
	else
	{
		/* 未到接收数量上限继续接收 */
		if(_tUART->tRxInfo.usRxCnt < UART_IT_RX_CACHE_SIZE)
		{
			/* 存储1字节数据，同时接收计数器加一 */
			_tUART->tRxInfo.ucpITRxCache[_tUART->tRxInfo.usRxCnt] = _tUART->tRxInfo.ucpRxBuffer[0];
			_tUART->tRxInfo.usRxCnt++;
		}
		else	/* 若超过cache大小立即告知接收完成，计数器归零接收多余字节 */
		{
			/* 保存接收到的长度 */
			_tUART->tRxInfo.usRxLength = UART_IT_RX_CACHE_SIZE;

			/* 接收计数器清零 */
			_tUART->tRxInfo.usRxCnt = 0;

			/* 接收完成标志位置1 */
			_tUART->tRxInfo.ucRxCplt = 1;			
		}
	}

	/* 等待下一个字节接收完成 */
	while(HAL_UART_Receive_IT(&_tUART->tUARTHandle,_tUART->tRxInfo.ucpRxBuffer,1) != HAL_OK);
}

/**
 * @brief 串口DMA中断接收处理子函数(用于串口中断处理函数中)
 * @note 在类似void USART1_IRQHandler(void)函数中调用，根据使用的串口号进行更改
 * @param _tUART-串口结构体指针
 * @note 数据保存的起始地址为ucpDMARxCache，接收到的长度为usDMARxLength
 * @retval Null
*/
void Drv_Uart_DMA_RxHandler(tagUART_T *_tUART)
{
	/* 空闲中断标记被置位 */
	if(__HAL_UART_GET_FLAG(&_tUART->tUARTHandle,UART_FLAG_IDLE) != RESET) 
	{
		/* 清除中断标记 */
		__HAL_UART_CLEAR_IDLEFLAG(&_tUART->tUARTHandle);

		/* 停止DMA接收 */
		HAL_UART_DMAStop(&_tUART->tUARTHandle);

		/* 总数据量减去未接收到的数据量为已经接收到的数据量 */
		_tUART->tRxInfo.usDMARxLength = _tUART->tRxInfo.usDMARxMAXSize - __HAL_DMA_GET_COUNTER(_tUART->tUARTHandle.hdmarx);

		/* 接收标志位置1 */
        _tUART->tRxInfo.ucDMARxCplt = 1;
	}

	/* 重新启动DMA接收 */
    HAL_UART_Receive_DMA(&_tUART->tUARTHandle,_tUART->tRxInfo.ucpDMARxCache,_tUART->tRxInfo.usDMARxMAXSize);
}

/**
 * @brief 串口DMA中断发送处理子函数(用于串口发送完成回调函数中)
 * @note 在函数名为void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)中调用
 * @param _tUART-串口结构体指针
 * @retval Null
*/
void Drv_Uart_DMA_TxHandler(tagUART_T *_tUART)
{
	uint32_t ulDMAFlag;

	/* UART1发送通道 */
	if(_tUART->tUartDMA.tDMATx.Instance == DMA1_Channel4)		ulDMAFlag = DMA_FLAG_TC4;

	/* UART2发送通道 */
	else if(_tUART->tUartDMA.tDMATx.Instance == DMA1_Channel7)	ulDMAFlag = DMA_FLAG_TC7;

	/* UART3发送通道 */
	else if(_tUART->tUartDMA.tDMATx.Instance == DMA1_Channel2) 	ulDMAFlag = DMA_FLAG_TC2;

	/* UART4发送通道 */
	else if(_tUART->tUartDMA.tDMATx.Instance == DMA2_Channel5)	ulDMAFlag = DMA_FLAG_TC5;

	/* 判断对应通道是否完成发送 */
	if(__HAL_DMA_GET_TC_FLAG_INDEX(&_tUART->tUartDMA.tDMATx) == ulDMAFlag)
	{
		/* 关闭串口DMA */
		HAL_UART_DMAStop(&_tUART->tUARTHandle);

		/* 清除发送完成标志位 */
		__HAL_DMA_CLEAR_FLAG(&_tUART->tUartDMA.tDMATx,ulDMAFlag);

		/* 重载CNDTR寄存器，数量为传递数组大小 */
		_tUART->tUartDMA.tDMATx.Instance->CNDTR = _tUART->tTxInfo.usDMATxLength;
		_tUART->tTxInfo.usDMATxLength = 0;
        
		/* 发送完成标志位置1 */
        _tUART->tTxInfo.ucDMATxCplt = 1;

		/* 开启DMA发送 */
		__HAL_DMA_ENABLE(&_tUART->tUartDMA.tDMATx);
	}
}

/**
 * @brief 串口DMA通道中断处理子函数(用于DMA通道中断函数中)
 * @note 在函数名为void DMAX_ChannelX_IRQHandler(void)中调用
 * @param _tUART-串口结构体指针
 * @retval Null
*/
void Drv_Uart_DMA_IRQHandler(tagUART_T *_tUART)
{
	/* 如果使能DMA发送 */
	if(_tUART->tUartDMA.bTxEnable)
		HAL_DMA_IRQHandler(&_tUART->tUartDMA.tDMATx);

	/* 如果使能DMA接收 */
	if(_tUART->tUartDMA.bRxEnable)
		HAL_DMA_IRQHandler(&_tUART->tUartDMA.tDMARx);
}

#endif
