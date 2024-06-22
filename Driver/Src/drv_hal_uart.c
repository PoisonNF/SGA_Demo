/****************************************************************************

* Sigma�Ŷ�

* �ļ���: drv_hal_uart.c

* ���ݼ��������������ļ�

* �ļ���ʷ��

* �汾��	  ����		  ����				˵��
*  3.3 	 	2024-06-03	 �����		�޸������жϽ��մ����Ӻ������մ���

*  3.2	 	2024-03-28	 �����		�޸�����DMA���պ�������Խ�������

*  3.0	 	2024-01-26	 �����		����STM32F4ϵ��

*  2.9 		2023-11-26   �����		���Ӵ���DMAͨ���жϴ����Ӻ���

*  2.8 		2023-10-15   �����		�޸�����DMA���͵�����

*  2.7		2023-07-06   �����		���ڳ�ʼ���ṹ������Ĭ��ֵ�������жϽ��մ����Ӻ������ӽ�β��������

*  2.4		2023-05-12   �����		���������жϽ�����غ�������ʽ�Ż�

* 2.2.1		2023-04-03   �����		������Ϣ�ṹ���Ա���ģ���������DMA���ݽ��պ���

*  1.2		2023-02-26   �����		��������DMA������غ������Ż�ע��

* 1.1.7 	2022-10-11   �����		��������DMA������غ���

* 1.1.4 	2022-09-03   �����		��������������ӳ�����

* 1.1.1 	2022-08-04   �����		�޸��˴����������ȷ������

* 1.0.0a 	2020-02-22	 ���		�������ļ�

****************************************************************************/
#include "drv_hal_conf.h"

#ifdef DRV_HAL_UART_ENABLE

/**
 * @brief �����ж����ú���
 * @param _tUART-����ʵ��ָ��
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
#ifdef STM32F4_SGA_ENABLE
	else if(_tUART->tUARTHandle.Instance == USART6)
	{
		HAL_NVIC_SetPriority(USART6_IRQn, _tUART->ucPriority, _tUART->ucSubPriority);
		HAL_NVIC_EnableIRQ(USART6_IRQn);
	}
#endif 
}

/**
 * @brief ����DMA�ж����ú���
 * @param _tUART-����ʵ��ָ��
 * @retval Null 
*/
static void S_Uart_DMA_NVICConfig(tagUART_T *_tUART)
{
#ifdef STM32F1_SGA_ENABLE
	/* DMA����ʹ�� */
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
	
	/* DMA����ʹ�� */
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

#endif

#ifdef STM32F4_SGA_ENABLE
	/* DMA����ʹ�� */
	if(_tUART->tUartDMA.bRxEnable == true)
	{
		if(_tUART->tUARTHandle.Instance == USART1)
		{
			/* DMA2_Stream5_IRQn interrupt configuration */
			HAL_NVIC_SetPriority(DMA2_Stream5_IRQn,_tUART->tUartDMA.ucDMARxPriority, _tUART->tUartDMA.ucDMARxSubPriority);
			HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);
		}
		else if(_tUART->tUARTHandle.Instance == USART2)
		{
			/* DMA1_Stream5_IRQn interrupt configuration */
			HAL_NVIC_SetPriority(DMA1_Stream5_IRQn,_tUART->tUartDMA.ucDMARxPriority, _tUART->tUartDMA.ucDMARxSubPriority);
			HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
		}
		else if(_tUART->tUARTHandle.Instance == USART3)
		{
			/* DMA1_Stream1_IRQn interrupt configuration */
			HAL_NVIC_SetPriority(DMA1_Stream1_IRQn,_tUART->tUartDMA.ucDMARxPriority, _tUART->tUartDMA.ucDMARxSubPriority);
			HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
		}
		else if(_tUART->tUARTHandle.Instance == UART4)
		{
			/* DMA1_Stream2_IRQn interrupt configuration */
			HAL_NVIC_SetPriority(DMA1_Stream2_IRQn,_tUART->tUartDMA.ucDMARxPriority, _tUART->tUartDMA.ucDMARxSubPriority);
			HAL_NVIC_EnableIRQ(DMA1_Stream2_IRQn);
		}
		else if(_tUART->tUARTHandle.Instance == UART5)
		{
			/* DMA1_Stream0_IRQn interrupt configuration */
			HAL_NVIC_SetPriority(DMA1_Stream0_IRQn,_tUART->tUartDMA.ucDMARxPriority, _tUART->tUartDMA.ucDMARxSubPriority);
			HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
		}
		else if(_tUART->tUARTHandle.Instance == USART6)
		{
			/* DMA2_Stream1_IRQn interrupt configuration */
			HAL_NVIC_SetPriority(DMA2_Stream1_IRQn,_tUART->tUartDMA.ucDMARxPriority, _tUART->tUartDMA.ucDMARxSubPriority);
			HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
		}
	}

	/* DMA����ʹ�� */
	if(_tUART->tUartDMA.bTxEnable == true)
	{
		if(_tUART->tUARTHandle.Instance == USART1)
		{
			/* DMA2_Stream7_IRQn interrupt configuration */
			HAL_NVIC_SetPriority(DMA2_Stream7_IRQn,_tUART->tUartDMA.ucDMARxPriority, _tUART->tUartDMA.ucDMARxSubPriority);
			HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
		}
		else if(_tUART->tUARTHandle.Instance == USART2)
		{
			/* DMA1_Stream6_IRQn interrupt configuration */
			HAL_NVIC_SetPriority(DMA1_Stream6_IRQn,_tUART->tUartDMA.ucDMARxPriority, _tUART->tUartDMA.ucDMARxSubPriority);
			HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
		}
		else if(_tUART->tUARTHandle.Instance == USART3)
		{
			/* DMA1_Stream3_IRQn interrupt configuration */
			HAL_NVIC_SetPriority(DMA1_Stream3_IRQn,_tUART->tUartDMA.ucDMARxPriority, _tUART->tUartDMA.ucDMARxSubPriority);
			HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
		}
		else if(_tUART->tUARTHandle.Instance == UART4)
		{
			/* DMA1_Stream4_IRQn interrupt configuration */
			HAL_NVIC_SetPriority(DMA1_Stream4_IRQn,_tUART->tUartDMA.ucDMARxPriority, _tUART->tUartDMA.ucDMARxSubPriority);
			HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
		}
		else if(_tUART->tUARTHandle.Instance == UART5)
		{
			/* DMA1_Stream7_IRQn interrupt configuration */
			HAL_NVIC_SetPriority(DMA1_Stream7_IRQn,_tUART->tUartDMA.ucDMARxPriority, _tUART->tUartDMA.ucDMARxSubPriority);
			HAL_NVIC_EnableIRQ(DMA1_Stream7_IRQn);
		}
		else if(_tUART->tUARTHandle.Instance == USART6)
		{
			/* DMA2_Stream6_IRQn interrupt configuration */
			HAL_NVIC_SetPriority(DMA2_Stream6_IRQn,_tUART->tUartDMA.ucDMARxPriority, _tUART->tUartDMA.ucDMARxSubPriority);
			HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
		}		
	}

#endif
}

/**
 * @brief ���ڲ������ú���
 * @param _tUART-����ʵ��ָ��
 * @retval Null
*/
static void S_Uart_ParamConfig(tagUART_T *_tUART)
{
	/* ���ô��ڳ�ʼ������ */
	if (HAL_UART_Init(&_tUART->tUARTHandle) != HAL_OK)
	{
		Drv_HAL_Error(__FILE__, __LINE__);
	}
}

/**
 * @brief ����DMA�������ú���
 * @param _tUART-����ʵ��ָ��
 * @retval Null
*/
static void S_Uart_DMAParamConfig(tagUART_T *_tUART)
{
	/* ���ʹ��DMA���� */
	if(_tUART->tUartDMA.bRxEnable == true)
	{
		if(HAL_DMA_Init(&_tUART->tUartDMA.tDMARx) != HAL_OK)	Drv_HAL_Error(__FILE__,__LINE__);
	}

	/* ���ʹ��DMA���� */
	if(_tUART->tUartDMA.bTxEnable == true)
	{
		if(HAL_DMA_Init(&_tUART->tUartDMA.tDMATx) != HAL_OK)	Drv_HAL_Error(__FILE__,__LINE__);
	}
}

/**
 * @brief ����ʱ�ӳ�ʼ������---L4ʹ��
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
 * @brief ����ʱ��ʹ�ܺ���
 * @param _tUART-���ڽṹ��ָ��
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
#ifdef STM32F4_SGA_ENABLE
	else if(_tUART->tUARTHandle.Instance == USART6)
	{
		__HAL_RCC_USART6_CLK_ENABLE();
	}
#endif
}

/**
 * @brief DMAʱ��ʹ�ܺ���
 * @param _tUART-���ڽṹ��ָ��
 * @retval Null
*/
static void S_Uart_DMA_CLKEnable(tagUART_T *_tUART)
{
#ifdef STM32F1_SGA_ENABLE
	if(_tUART->tUARTHandle.Instance == USART1)			__HAL_RCC_DMA1_CLK_ENABLE();
	else if(_tUART->tUARTHandle.Instance == USART2)		__HAL_RCC_DMA1_CLK_ENABLE();
	else if(_tUART->tUARTHandle.Instance == USART3)		__HAL_RCC_DMA1_CLK_ENABLE();
	else if(_tUART->tUARTHandle.Instance == UART4)		__HAL_RCC_DMA2_CLK_ENABLE();
	else if(_tUART->tUARTHandle.Instance == UART5)
	{
		/* Uart5����ʹ��DMA */
		Drv_HAL_Error(__FILE__,__LINE__);
		while(1);
	}
#endif

#ifdef STM32F4_SGA_ENABLE
	if(_tUART->tUARTHandle.Instance == USART1)			__HAL_RCC_DMA2_CLK_ENABLE();
	else if(_tUART->tUARTHandle.Instance == USART2)		__HAL_RCC_DMA1_CLK_ENABLE();
	else if(_tUART->tUARTHandle.Instance == USART3)		__HAL_RCC_DMA1_CLK_ENABLE();
	else if(_tUART->tUARTHandle.Instance == UART4)		__HAL_RCC_DMA1_CLK_ENABLE();
	else if(_tUART->tUARTHandle.Instance == UART5)		__HAL_RCC_DMA1_CLK_ENABLE();
	else if(_tUART->tUARTHandle.Instance == USART6)		__HAL_RCC_DMA2_CLK_ENABLE();
#endif
}

/**
 * @brief UART��GPIO�������ú���
 * @param _tUART-���ھ��ָ��
 * @retval Null
*/
static void S_Uart_GPIOConfig(tagUART_T *_tUART)
{
#ifdef STM32F1_SGA_ENABLE
	/* ��������ģʽʱ�� */
	__HAL_RCC_AFIO_CLK_ENABLE();

	/* ���ݲ�ͬ���ڵ�ucAFMode������Ӧ����ӳ�䣬��ӳ�����drv_hal_uart.h�� */
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
#endif

#ifdef STM32F4_SGA_ENABLE
	if(_tUART->tUARTHandle.Instance == USART1)
	{
		_tUART->tGPIO[0].tGPIOInit.Alternate = GPIO_AF7_USART1;
		_tUART->tGPIO[1].tGPIOInit.Alternate = GPIO_AF7_USART1;
	}
	else if (_tUART->tUARTHandle.Instance == USART2)
	{
		_tUART->tGPIO[0].tGPIOInit.Alternate = GPIO_AF7_USART2;
		_tUART->tGPIO[1].tGPIOInit.Alternate = GPIO_AF7_USART2;		
	}
	else if (_tUART->tUARTHandle.Instance == USART3)
	{
		_tUART->tGPIO[0].tGPIOInit.Alternate = GPIO_AF7_USART3;
		_tUART->tGPIO[1].tGPIOInit.Alternate = GPIO_AF7_USART3;		
	}
	else if (_tUART->tUARTHandle.Instance == UART4)
	{
		_tUART->tGPIO[0].tGPIOInit.Alternate = GPIO_AF8_UART4;
		_tUART->tGPIO[1].tGPIOInit.Alternate = GPIO_AF8_UART4;		
	}
	else if (_tUART->tUARTHandle.Instance == UART5)
	{
		_tUART->tGPIO[0].tGPIOInit.Alternate = GPIO_AF8_UART5;
		_tUART->tGPIO[1].tGPIOInit.Alternate = GPIO_AF8_UART5;		
	}
	else if (_tUART->tUARTHandle.Instance == USART6)
	{
		_tUART->tGPIO[0].tGPIOInit.Alternate = GPIO_AF8_USART6;
		_tUART->tGPIO[1].tGPIOInit.Alternate = GPIO_AF8_USART6;		
	}
#endif
	
	Drv_GPIO_Init(_tUART->tGPIO, 2); 	/* GPIO��ʼ�� */
}

/**
 * @brief UART����ƥ�亯��
 * @param _tUART-���ھ��ָ��
 * @note ���û��ָ������������ΪĬ�ϣ�����1,115200,8,1,N��
 * @retval Null
*/
static void S_Uart_ParamMatch(tagUART_T *_tUART)
{
	/* ���ڲ��� */
	DEFAULT(_tUART->tUARTHandle.Instance,USART1);
	DEFAULT(_tUART->tUARTHandle.Init.BaudRate,115200);
	DEFAULT(_tUART->tUARTHandle.Init.Mode,UART_MODE_TX_RX);
	DEFAULT(_tUART->tUARTHandle.Init.WordLength,UART_WORDLENGTH_8B);
	DEFAULT(_tUART->tUARTHandle.Init.StopBits,UART_STOPBITS_1);
	DEFAULT(_tUART->tUARTHandle.Init.Parity,UART_PARITY_NONE);
	DEFAULT(_tUART->ucPriority,1);
	DEFAULT(_tUART->ucSubPriority,3);

#ifdef STM32F1_SGA_ENABLE
	/* ����DMA���� */
	if(_tUART->tUartDMA.bTxEnable == true)
	{
		/* ���ݴ��ں�ѡ��DMAͨ�� */
		if(_tUART->tUARTHandle.Instance == USART1)
			_tUART->tUartDMA.tDMATx.Instance = DMA1_Channel4;
		else if(_tUART->tUARTHandle.Instance == USART2)
			_tUART->tUartDMA.tDMATx.Instance = DMA1_Channel7;
		else if(_tUART->tUARTHandle.Instance == USART3)
			_tUART->tUartDMA.tDMATx.Instance = DMA1_Channel2;
		else if(_tUART->tUARTHandle.Instance == UART4)
			_tUART->tUartDMA.tDMATx.Instance = DMA2_Channel5;

		/* DMA����ģʽ */
		DEFAULT(_tUART->tUartDMA.tDMATx.Init.Direction,DMA_MEMORY_TO_PERIPH);
		DEFAULT(_tUART->tUartDMA.tDMATx.Init.PeriphInc,DMA_PINC_DISABLE);
		DEFAULT(_tUART->tUartDMA.tDMATx.Init.MemInc,DMA_MINC_ENABLE);
		DEFAULT(_tUART->tUartDMA.tDMATx.Init.PeriphDataAlignment,DMA_PDATAALIGN_BYTE);
		DEFAULT(_tUART->tUartDMA.tDMATx.Init.MemDataAlignment,DMA_MDATAALIGN_BYTE);
		DEFAULT(_tUART->tUartDMA.tDMATx.Init.Mode,DMA_NORMAL);
		DEFAULT(_tUART->tUartDMA.tDMATx.Init.Priority,DMA_PRIORITY_LOW);

		/* DMA�ж����ȼ� */
		DEFAULT(_tUART->tUartDMA.ucDMATxPriority,1);
		DEFAULT(_tUART->tUartDMA.ucDMATxSubPriority,1);

		/* DMA���ͻ�������С */
		DEFAULT(_tUART->tTxInfo.usDMATxMAXSize, 100);
	}

	if(_tUART->tUartDMA.bRxEnable == true)
	{
		/* ���ݴ��ں�ѡ��DMAͨ�� */
		if(_tUART->tUARTHandle.Instance == USART1)
			_tUART->tUartDMA.tDMARx.Instance = DMA1_Channel5;
		else if(_tUART->tUARTHandle.Instance == USART2)
			_tUART->tUartDMA.tDMARx.Instance = DMA1_Channel6;
		else if(_tUART->tUARTHandle.Instance == USART3)
			_tUART->tUartDMA.tDMARx.Instance = DMA1_Channel3;
		else if(_tUART->tUARTHandle.Instance == UART4)
			_tUART->tUartDMA.tDMARx.Instance = DMA2_Channel3;

		/* DMA����ģʽ */
		DEFAULT(_tUART->tUartDMA.tDMARx.Init.Direction,DMA_PERIPH_TO_MEMORY);
		DEFAULT(_tUART->tUartDMA.tDMARx.Init.PeriphInc,DMA_PINC_DISABLE);
		DEFAULT(_tUART->tUartDMA.tDMARx.Init.MemInc,DMA_MINC_ENABLE);
		DEFAULT(_tUART->tUartDMA.tDMARx.Init.PeriphDataAlignment,DMA_PDATAALIGN_BYTE);
		DEFAULT(_tUART->tUartDMA.tDMARx.Init.MemDataAlignment,DMA_MDATAALIGN_BYTE);
		DEFAULT(_tUART->tUartDMA.tDMARx.Init.Mode,DMA_CIRCULAR);
		DEFAULT(_tUART->tUartDMA.tDMARx.Init.Priority,DMA_PRIORITY_LOW);
		
		/* DMA�ж����ȼ� */
		DEFAULT(_tUART->tUartDMA.ucDMARxPriority,1);
		DEFAULT(_tUART->tUartDMA.ucDMARxSubPriority,1);

		/* DMA���ջ�������С */
		DEFAULT(_tUART->tRxInfo.usDMARxMAXSize, 100);
	}

	/* ����I/O���� [0]TX [1]RX */
	/* �������I/O���˳����� */
	if(_tUART->tGPIO[0].tGPIOInit.Pin != 0 && _tUART->tGPIO[1].tGPIOInit.Pin != 0)
		return;

	/* ����ʹ�õĴ���ѡ�����Ӧ��Ĭ��I/O */
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
#endif

#ifdef STM32F4_SGA_ENABLE
	/* ����DMA���� */
	if(_tUART->tUartDMA.bTxEnable == true)
	{
		/* ���ݴ��ں�ѡ��DMAͨ�� */
		if(_tUART->tUARTHandle.Instance == USART1)
		{
			_tUART->tUartDMA.tDMATx.Instance = DMA2_Stream7;
		}
		else if(_tUART->tUARTHandle.Instance == USART2)
		{
			_tUART->tUartDMA.tDMATx.Instance = DMA1_Stream6;
		}
		else if(_tUART->tUARTHandle.Instance == USART3)
		{
			_tUART->tUartDMA.tDMATx.Instance = DMA1_Stream3;
		}
		else if(_tUART->tUARTHandle.Instance == UART4)
		{
			_tUART->tUartDMA.tDMATx.Instance = DMA1_Stream4;
		}
		else if(_tUART->tUARTHandle.Instance == UART5)
		{
			_tUART->tUartDMA.tDMATx.Instance = DMA1_Stream7;
		}
		else if(_tUART->tUARTHandle.Instance == USART6)
		{
			_tUART->tUartDMA.tDMATx.Instance = DMA2_Stream6;
		}

		/* DMA����ģʽ */
		DEFAULT(_tUART->tUartDMA.tDMATx.Init.Channel,DMA_CHANNEL_4);
		DEFAULT(_tUART->tUartDMA.tDMATx.Init.Direction,DMA_MEMORY_TO_PERIPH);
		DEFAULT(_tUART->tUartDMA.tDMATx.Init.PeriphInc,DMA_PINC_DISABLE);
		DEFAULT(_tUART->tUartDMA.tDMATx.Init.MemInc,DMA_MINC_ENABLE);
		DEFAULT(_tUART->tUartDMA.tDMATx.Init.PeriphDataAlignment,DMA_PDATAALIGN_BYTE);
		DEFAULT(_tUART->tUartDMA.tDMATx.Init.MemDataAlignment,DMA_MDATAALIGN_BYTE);
		DEFAULT(_tUART->tUartDMA.tDMATx.Init.Mode,DMA_NORMAL);
		DEFAULT(_tUART->tUartDMA.tDMATx.Init.Priority,DMA_PRIORITY_LOW);

		/* DMA�ж����ȼ� */
		DEFAULT(_tUART->tUartDMA.ucDMATxPriority,1);
		DEFAULT(_tUART->tUartDMA.ucDMATxSubPriority,1);

		/* DMA���ͻ�������С */
		DEFAULT(_tUART->tTxInfo.usDMATxMAXSize, 100);
	}

	if(_tUART->tUartDMA.bRxEnable == true)
	{
		/* ���ݴ��ں�ѡ��DMAͨ�� */
		if(_tUART->tUARTHandle.Instance == USART1)
		{
			_tUART->tUartDMA.tDMARx.Instance = DMA2_Stream5;
		}
		else if(_tUART->tUARTHandle.Instance == USART2)
		{
			_tUART->tUartDMA.tDMARx.Instance = DMA1_Stream5;
		}
		else if(_tUART->tUARTHandle.Instance == USART3)
		{
			_tUART->tUartDMA.tDMARx.Instance = DMA1_Stream1;
		}
		else if(_tUART->tUARTHandle.Instance == UART4)
		{
			_tUART->tUartDMA.tDMARx.Instance = DMA1_Stream2;
		}
		else if(_tUART->tUARTHandle.Instance == UART5)
		{
			_tUART->tUartDMA.tDMARx.Instance = DMA1_Stream0;
		}
		else if(_tUART->tUARTHandle.Instance == USART6)
		{
			_tUART->tUartDMA.tDMARx.Instance = DMA2_Stream1;
		}

		/* DMA����ģʽ */
		DEFAULT(_tUART->tUartDMA.tDMARx.Init.Channel,DMA_CHANNEL_4);
		DEFAULT(_tUART->tUartDMA.tDMARx.Init.Direction,DMA_PERIPH_TO_MEMORY);
		DEFAULT(_tUART->tUartDMA.tDMARx.Init.PeriphInc,DMA_PINC_DISABLE);
		DEFAULT(_tUART->tUartDMA.tDMARx.Init.MemInc,DMA_MINC_ENABLE);
		DEFAULT(_tUART->tUartDMA.tDMARx.Init.PeriphDataAlignment,DMA_PDATAALIGN_BYTE);
		DEFAULT(_tUART->tUartDMA.tDMARx.Init.MemDataAlignment,DMA_MDATAALIGN_BYTE);
		DEFAULT(_tUART->tUartDMA.tDMARx.Init.Mode,DMA_CIRCULAR);
		DEFAULT(_tUART->tUartDMA.tDMARx.Init.Priority,DMA_PRIORITY_LOW);
		
		/* DMA�ж����ȼ� */
		DEFAULT(_tUART->tUartDMA.ucDMARxPriority,1);
		DEFAULT(_tUART->tUartDMA.ucDMARxSubPriority,1);

		/* DMA���ջ�������С */
		DEFAULT(_tUART->tRxInfo.usDMARxMAXSize, 100);
	}

	/* ����I/O���� [0]TX [1]RX */
	/* �������I/O���˳����� */
	if(_tUART->tGPIO[0].tGPIOInit.Pin != 0 && _tUART->tGPIO[1].tGPIOInit.Pin != 0)
		return;

	/* ����ʹ�õĴ���ѡ�����Ӧ��Ĭ��I/O */
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
		_tUART->tGPIO[0].tGPIOInit.Pin 	= GPIO_PIN_0;
		_tUART->tGPIO[0].tGPIOPort 		= GPIOA;

		_tUART->tGPIO[1].tGPIOInit.Pin 	= GPIO_PIN_1;
		_tUART->tGPIO[1].tGPIOPort 		= GPIOA;
	}
	else if(_tUART->tUARTHandle.Instance == UART5)
	{
		_tUART->tGPIO[0].tGPIOInit.Pin 	= GPIO_PIN_12;
		_tUART->tGPIO[0].tGPIOPort 		= GPIOC;

		_tUART->tGPIO[1].tGPIOInit.Pin 	= GPIO_PIN_2;
		_tUART->tGPIO[1].tGPIOPort 		= GPIOD;
	}
	else if(_tUART->tUARTHandle.Instance == USART6)
	{
		_tUART->tGPIO[0].tGPIOInit.Pin 	= GPIO_PIN_6;
		_tUART->tGPIO[0].tGPIOPort 		= GPIOC;

		_tUART->tGPIO[1].tGPIOInit.Pin 	= GPIO_PIN_7;
		_tUART->tGPIO[1].tGPIOPort 		= GPIOC;
	}

	_tUART->tGPIO[0].tGPIOInit.Mode = GPIO_MODE_AF_PP;
	_tUART->tGPIO[1].tGPIOInit.Mode = GPIO_MODE_AF_PP;

	_tUART->tGPIO->tGPIOInit.Pull 	= GPIO_NOPULL;
	_tUART->tGPIO->tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH;
#endif

}

/**
 * @brief ���ڷ������ݺ���
 * @param _tUART-����ʵ��ָ��
 * @param _ucpTxData-�������ݵ�ַָ��
 * @param _usSize-�������ݴ�С
 * @retval Null
*/
void Drv_Uart_Transmit(tagUART_T *_tUART, uint8_t *_ucpTxData, uint16_t _usSize)
{
	HAL_UART_Transmit(&_tUART->tUARTHandle, _ucpTxData, _usSize, UART_TIME_OUT);
}

/**
 * @brief �����жϷ������ݺ���
 * @param _tUART-����ʵ��ָ��
 * @param _ucpTxData-�������ݵ�ַָ��
 * @param _usSize-�������ݴ�С
 * @retval Null
*/
void Drv_Uart_Transmit_IT(tagUART_T *_tUART, uint8_t *_ucpTxData, uint16_t _usSize)
{
	HAL_UART_Transmit_IT(&_tUART->tUARTHandle, _ucpTxData, _usSize);
}

/**
 * @brief ����DMA�������ݺ���
 * @param _tUART-����ʵ��ָ��
 * @param _ucpTxData-�������ݵ�ַָ��
 * @param _usSize-�������ݴ�С
 * @retval Null
*/
void Drv_Uart_Transmit_DMA(tagUART_T *_tUART, uint8_t *_ucpTxData, uint16_t _usSize)
{
	/* ��ȡDMA״̬ */
	while(HAL_DMA_GetState(&_tUART->tUartDMA.tDMATx) != HAL_DMA_STATE_READY);

	_tUART->tTxInfo.usDMATxLength = _usSize;

	/* ׼��״̬���ɷ��� */
	HAL_UART_Transmit_DMA(&_tUART->tUARTHandle, _ucpTxData, _tUART->tTxInfo.usDMATxLength);
}

/**
 * @brief �����жϽ������ݺ���
 * @param _tUART-����ʵ��ָ��
 * @param _ucpRxData-�������ݵ�ַָ��
 * @retval uint16_t ���յ��ĳ���
*/
uint16_t Drv_Uart_Receive_IT(tagUART_T *_tUART, uint8_t *_ucpRxData)
{
	if(_tUART->tRxInfo.ucRxCplt)
	{
		/* ���ݿ��� */
		memcpy(_ucpRxData,_tUART->tRxInfo.ucpITRxCache,_tUART->tRxInfo.usRxLength);

		/* ��־λ���� */
		_tUART->tRxInfo.ucRxCplt = 0;

		return _tUART->tRxInfo.usRxLength;
	}
	
	return 0;
}

/**
 * @brief ����DMA�������ݺ���
 * @param _tUART-����ʵ��ָ��
 * @param _ucpRxData-�������ݵ�ַָ��
 * @retval uint16_t usRxNum ���յ��ĳ���
*/
uint16_t Drv_Uart_Receive_DMA(tagUART_T *_tUART, uint8_t *_ucpRxData)
{
	uint16_t usRxNum;
    
	/* �ж��Ƿ������� */
	if(_tUART->tRxInfo.ucDMARxCplt != 1)
	{
		return 0;
	}
	else
	{
		/* ���������ݵĳ��ȱ��� */
		usRxNum = _tUART->tRxInfo.usDMARxLength;

		/* ��ս������ݵ�ַָ�� */
        memset(_ucpRxData,0,usRxNum);

		/* ���ݿ��� */
		memcpy(_ucpRxData,_tUART->tRxInfo.ucpDMARxCache,usRxNum);

		/* �����־λ */
		_tUART->tRxInfo.ucDMARxCplt = 0;

		return usRxNum;
	}
}

/**
 * @brief ���ڽ����ж����ú���
 * @param _tUART-����ʵ��ָ��
 * @param _ucpRxData-��������ָ��
 * @param _usSize-�������ݴ�С
 * @retval Null 
*/
void Drv_Uart_ReceIT_Enable(tagUART_T *_tUART, uint8_t *_ucpRxData, uint16_t _usSize)
{
	HAL_UART_Receive_IT(&_tUART->tUARTHandle, _ucpRxData, _usSize);
}

/**
 * @brief ������ͨ�ж�ģʽ��ʼ������
 * @param _tUART-���ڽṹ��ָ��
 * @retval Null 
*/
void Drv_Uart_ITInit(tagUART_T *_tUART)
{
	S_Uart_CLKConfig();
	S_Uart_ParamMatch(_tUART);
	S_Uart_CLKEnable(_tUART);
	S_Uart_GPIOConfig(_tUART);
	S_Uart_ParamConfig(_tUART);		/* ���ô��ڲ��� */
	S_Uart_NVICConfig(_tUART);		/* �����ж����ȼ� */

	/* Ϊcache����һ�γ��ȵĶ�̬�ڴ� */
	_tUART->tRxInfo.ucpITRxCache = (uint8_t *)malloc(UART_IT_RX_CACHE_SIZE);

	/* ��һ���������н���һ���ַ� */
	HAL_UART_Receive_IT(&_tUART->tUARTHandle, _tUART->tRxInfo.ucpRxBuffer, 1);
}

/**
 * @brief ����DMAģʽ��ʼ������
 * @param _tUART-���ڽṹ��ָ��
 * @retval Null 
*/
void Drv_Uart_DMAInit(tagUART_T *_tUART)
{
	S_Uart_CLKConfig();				/* L4���� */
	
	/* ����ƥ�� */
	S_Uart_ParamMatch(_tUART);

	/* DMA���� */
	S_Uart_DMA_CLKEnable(_tUART);
	S_Uart_DMA_NVICConfig(_tUART);	/* DMA�ж����� */
	S_Uart_DMAParamConfig(_tUART);	/* ����DMA���� */

	/* �������� */
	S_Uart_CLKEnable(_tUART);
	S_Uart_GPIOConfig(_tUART);
	S_Uart_NVICConfig(_tUART);		/* �����ж����� */
	S_Uart_ParamConfig(_tUART);		/* ���ô��ڲ��� */

	/* ���ʹ��DMA���� */
	if(_tUART->tUartDMA.bRxEnable == true)
	{
		/* �������ں�DMA���� */
		__HAL_LINKDMA(&_tUART->tUARTHandle,hdmarx,_tUART->tUartDMA.tDMARx);

		/* Ϊcache����һ�γ��ȵĶ�̬�ڴ� */
		_tUART->tRxInfo.ucpDMARxCache = (uint8_t *)malloc(_tUART->tRxInfo.usDMARxMAXSize);

		/*	�򿪿����ж� */
		__HAL_UART_ENABLE_IT(&_tUART->tUARTHandle,UART_IT_IDLE);

		/* ����DMA���� */
		HAL_UART_Receive_DMA(&_tUART->tUARTHandle,_tUART->tRxInfo.ucpDMARxCache,_tUART->tRxInfo.usDMARxMAXSize);
	}

	/* ���ʹ��DMA���� */
	if(_tUART->tUartDMA.bTxEnable == true)
	{
		/* �������ں�DMA���� */
		__HAL_LINKDMA(&_tUART->tUARTHandle,hdmatx,_tUART->tUartDMA.tDMATx);

		/* Ϊcache����һ�γ��ȵĶ�̬�ڴ� */
		_tUART->tTxInfo.ucpDMATxCache = (uint8_t *)malloc(_tUART->tTxInfo.usDMATxMAXSize);
	}
}

/**
 * @brief �����ж�ñ�Ӻ���(���ڴ����жϴ�������)
 * @note ������void USART1_IRQHandler(void)�����е��ã�����ʹ�õĴ��ںŽ��и���
 * @param _tUART-���ڽṹ��ָ��
 * @retval Null
*/
void Drv_Uart_IRQHandler(tagUART_T *_tUART)
{
	HAL_UART_IRQHandler(&_tUART->tUARTHandle);
}

/**
 * @brief �����жϽ��մ����Ӻ���(���ڴ��ڽ�����ɻص�������)
 * @note �ں�����Ϊvoid HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)�е���
 * @param _tUART-���ڽṹ��ָ��
 * @param _ucEndChar ��Ҫ���Ľ�β��
 * @note ���ݱ������ʼ��ַΪucpITRxCache,���յ��ĳ���ΪusRxLength
 * @retval Null
*/
void Drv_Uart_IT_RxHandler(tagUART_T *_tUART, uint8_t _ucEndChar)
{
    /* �洢1�ֽ����ݣ�ͬʱ���ռ�������һ */
	_tUART->tRxInfo.ucpITRxCache[_tUART->tRxInfo.usRxCnt] = _tUART->tRxInfo.ucpRxBuffer[0];
    _tUART->tRxInfo.usRxCnt++;
    
	/* ��⵽��β��ucEndChar������ս��� */
	if(_tUART->tRxInfo.ucpRxBuffer[0] == _ucEndChar)
	{
		/* ������յ��ĳ��� */
		_tUART->tRxInfo.usRxLength = _tUART->tRxInfo.usRxCnt;

		/* ���ռ��������� */
		_tUART->tRxInfo.usRxCnt = 0;

		/* ������ɱ�־λ��1 */
		_tUART->tRxInfo.ucRxCplt = 1;
	}

	/* ��ǰ���ճ��ȳ�������ֵ������ǽ������ */
	else if(_tUART->tRxInfo.usRxCnt >= UART_IT_RX_CACHE_SIZE)
	{
		/* ������յ��ĳ��� */
		_tUART->tRxInfo.usRxLength = UART_IT_RX_CACHE_SIZE;

		/* ���ռ��������� */
		_tUART->tRxInfo.usRxCnt = 0;

		/* ������ɱ�־λ��1 */
		_tUART->tRxInfo.ucRxCplt = 1;
	}

	/* �ȴ���һ���ֽڽ������ */
	while(HAL_UART_Receive_IT(&_tUART->tUARTHandle,_tUART->tRxInfo.ucpRxBuffer,1) != HAL_OK);
}

/**
 * @brief ����DMA�жϽ��մ����Ӻ���(���ڴ����жϴ�������)
 * @note ������void USART1_IRQHandler(void)�����е��ã�����ʹ�õĴ��ںŽ��и���
 * @param _tUART-���ڽṹ��ָ��
 * @note ���ݱ������ʼ��ַΪucpDMARxCache�����յ��ĳ���ΪusDMARxLength
 * @retval Null
*/
void Drv_Uart_DMA_RxHandler(tagUART_T *_tUART)
{
	/* �����жϱ�Ǳ���λ */
	if(__HAL_UART_GET_FLAG(&_tUART->tUARTHandle,UART_FLAG_IDLE) != RESET) 
	{
		/* ����жϱ�� */
		__HAL_UART_CLEAR_IDLEFLAG(&_tUART->tUARTHandle);

		/* ֹͣDMA���� */
		HAL_UART_DMAStop(&_tUART->tUARTHandle);

		/* ����������ȥδ���յ���������Ϊ�Ѿ����յ��������� */
		_tUART->tRxInfo.usDMARxLength = _tUART->tRxInfo.usDMARxMAXSize - __HAL_DMA_GET_COUNTER(_tUART->tUARTHandle.hdmarx);

		/* ���ձ�־λ��1 */
        _tUART->tRxInfo.ucDMARxCplt = 1;
	}

	/* ��������DMA���� */
    HAL_UART_Receive_DMA(&_tUART->tUARTHandle,_tUART->tRxInfo.ucpDMARxCache,_tUART->tRxInfo.usDMARxMAXSize);
}

/**
 * @brief ����DMA�жϷ��ʹ����Ӻ���(���ڴ��ڷ�����ɻص�������)
 * @note �ں�����Ϊvoid HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)�е���
 * @param _tUART-���ڽṹ��ָ��
 * @retval Null
*/
void Drv_Uart_DMA_TxHandler(tagUART_T *_tUART)
{
	uint32_t ulDMAFlag;

#ifdef STM32F1_SGA_ENABLE
	/* USART1����ͨ�� */
	if(_tUART->tUartDMA.tDMATx.Instance == DMA1_Channel4)		ulDMAFlag = DMA_FLAG_TC4;

	/* USART2����ͨ�� */
	else if(_tUART->tUartDMA.tDMATx.Instance == DMA1_Channel7)	ulDMAFlag = DMA_FLAG_TC7;

	/* USART3����ͨ�� */
	else if(_tUART->tUartDMA.tDMATx.Instance == DMA1_Channel2) 	ulDMAFlag = DMA_FLAG_TC2;

	/* UART4����ͨ�� */
	else if(_tUART->tUartDMA.tDMATx.Instance == DMA2_Channel5)	ulDMAFlag = DMA_FLAG_TC5;

	/* �ж϶�Ӧͨ���Ƿ���ɷ��� */
	if(__HAL_DMA_GET_TC_FLAG_INDEX(&_tUART->tUartDMA.tDMATx) == ulDMAFlag)
	{
		/* �رմ���DMA */
		HAL_UART_DMAStop(&_tUART->tUARTHandle);

		/* ���������ɱ�־λ */
		__HAL_DMA_CLEAR_FLAG(&_tUART->tUartDMA.tDMATx,ulDMAFlag);

		/* ����CNDTR�Ĵ���������Ϊ���������С */
		_tUART->tUartDMA.tDMATx.Instance->CNDTR = _tUART->tTxInfo.usDMATxLength;
		_tUART->tTxInfo.usDMATxLength = 0;
        
		/* ������ɱ�־λ��1 */
        _tUART->tTxInfo.ucDMATxCplt = 1;

		/* ����DMA���� */
		__HAL_DMA_ENABLE(&_tUART->tUartDMA.tDMATx);
	}
#endif

#ifdef STM32F4_SGA_ENABLE
	/* USART1����ͨ�� */
	if(_tUART->tUARTHandle.Instance == USART1)		ulDMAFlag = DMA_FLAG_TCIF3_7;

	/* USART2����ͨ�� */
	else if(_tUART->tUARTHandle.Instance == USART2)	ulDMAFlag = DMA_FLAG_TCIF3_7;

	/* USART3����ͨ�� */
	else if(_tUART->tUARTHandle.Instance == USART3) ulDMAFlag = DMA_FLAG_TCIF0_4;

	/* UART4����ͨ�� */
	else if(_tUART->tUARTHandle.Instance == UART4)	ulDMAFlag = DMA_FLAG_TCIF2_6;

	/* UART5����ͨ�� */
	else if(_tUART->tUARTHandle.Instance == UART5)	ulDMAFlag = DMA_FLAG_TCIF3_7;

	/* USART6����ͨ�� */
	else if(_tUART->tUARTHandle.Instance == USART6) ulDMAFlag = DMA_FLAG_TCIF3_7;

	/* �ж϶�Ӧ�������Ƿ���ɷ��� */
	if(__HAL_DMA_GET_FLAG(&_tUART->tUartDMA.tDMATx,ulDMAFlag))
	{
		
		/* �رմ���DMA */
		HAL_UART_DMAStop(&_tUART->tUARTHandle);

		/* ���������ɱ�־λ */
		__HAL_DMA_CLEAR_FLAG(&_tUART->tUartDMA.tDMATx,ulDMAFlag);

		/* ����NDTR�Ĵ���������Ϊ���������С */
		_tUART->tUartDMA.tDMATx.Instance->NDTR = _tUART->tTxInfo.usDMATxLength;
		_tUART->tTxInfo.usDMATxLength = 0;
        
		/* ������ɱ�־λ��1 */
        _tUART->tTxInfo.ucDMATxCplt = 1;

		/* ����DMA���� */
		__HAL_DMA_ENABLE(&_tUART->tUartDMA.tDMATx);
	}
#endif

}

/**
 * @brief ����DMAͨ���жϴ����Ӻ���(����DMAͨ���жϺ�����)
 * @note �ں�����Ϊvoid DMAX_ChannelX_IRQHandler(void)�е���
 * @param _tUART-���ڽṹ��ָ��
 * @retval Null
*/
void Drv_Uart_DMA_IRQHandler(tagUART_T *_tUART)
{
	/* ���ʹ��DMA���� */
	if(_tUART->tUartDMA.bTxEnable)
		HAL_DMA_IRQHandler(&_tUART->tUartDMA.tDMATx);

	/* ���ʹ��DMA���� */
	if(_tUART->tUartDMA.bRxEnable)
		HAL_DMA_IRQHandler(&_tUART->tUartDMA.tDMARx);
}

#endif
