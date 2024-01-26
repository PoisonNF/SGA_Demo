#include "config.h"


/* GPIO句柄示例 */
tagGPIO_T demoGPIO[] =
{
	
	[0]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_13,				/* GPIO引脚 */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
		.tGPIOPort 			= GPIOD,					/* GPIO分组 */
	},
    [1]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_14,				/* GPIO引脚 */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
		.tGPIOPort 			= GPIOD,					/* GPIO分组 */
	},
    [2]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_15,				/* GPIO引脚 */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO模式 */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
		.tGPIOPort 			= GPIOD,					/* GPIO分组 */
	},
    
};

/* 串口句柄示例 */
tagUART_T demoUart = 
{
	//串口工作模式配置
	.tUARTHandle.Instance 						= USART1,					/* STM32 串口设备 */
	.tUARTHandle.Init.BaudRate   				= 115200,					/* 串口波特率 */
	.tUARTHandle.Init.WordLength 				= UART_WORDLENGTH_8B,		/* 数据位长度 */
	.tUARTHandle.Init.StopBits   				= UART_STOPBITS_1,			/* 停止位长度 */
	.tUARTHandle.Init.Parity     				= UART_PARITY_NONE,			/* 校验位设置 */
	.tUARTHandle.Init.HwFlowCtl  				= UART_HWCONTROL_NONE,		/* 硬件流控设置 */
	.tUARTHandle.Init.Mode       				= UART_MODE_TX_RX,			/* 串口模式 */
	.tUARTHandle.Init.OverSampling 				= UART_OVERSAMPLING_16,		/* 过采样 */

#if defined (STM32L4_SGA_ENABLE)
	.tUARTHandle.Init.OneBitSampling 			= UART_ONE_BIT_SAMPLE_DISABLE,
	.tUARTHandle.AdvancedInit.AdvFeatureInit 	= UART_ADVFEATURE_NO_INIT,
#endif
	
	.ucPriority									= 1,						/* 中断优先级 */
	.ucSubPriority								= 3,						/* 中断子优先级 */
	
	//串口DMA接收参数配置
	.tUartDMA.bRxEnable							= true,						/* DMA接收使能 */
#ifdef STM32F1_SGA_ENABLE
	.tUartDMA.tDMARx.Instance					= DMA1_Channel5,
#endif
#ifdef STM32F4_SGA_ENABLE
	.tUartDMA.tDMARx.Instance					= DMA2_Stream2,
	.tUartDMA.tDMARx.Init.Channel				= DMA_CHANNEL_4,
#endif
	.tUartDMA.tDMARx.Init.Direction				= DMA_PERIPH_TO_MEMORY,
	.tUartDMA.tDMARx.Init.PeriphInc				= DMA_PINC_DISABLE,
	.tUartDMA.tDMARx.Init.MemInc				= DMA_MINC_ENABLE,
	.tUartDMA.tDMARx.Init.PeriphDataAlignment 	= DMA_PDATAALIGN_BYTE,
	.tUartDMA.tDMARx.Init.MemDataAlignment	  	= DMA_MDATAALIGN_BYTE,
	.tUartDMA.tDMARx.Init.Mode					= DMA_NORMAL,
	.tUartDMA.tDMARx.Init.Priority				= DMA_PRIORITY_LOW,

	.tRxInfo.usDMARxMAXSize             		= 100,              		/* DMA接收缓冲区大小 大小保持在协议最长字节*2以上，确保缓存池一定能够稳定接收一个完整的数据帧*/

	.tUartDMA.ucDMARxPriority					= 1,						/* DMA接收中断优先级 */
	.tUartDMA.ucDMARxSubPriority				= 1,						/* DMA接收中断子优先级 */
	
	//串口DMA发送参数配置
	.tUartDMA.bTxEnable							= true,						/* DMA发送使能 */
#ifdef STM32F1_SGA_ENABLE
	.tUartDMA.tDMATx.Instance					= DMA1_Channel4,
#endif
#ifdef STM32F4_SGA_ENABLE
	.tUartDMA.tDMATx.Instance					= DMA2_Stream7,
	.tUartDMA.tDMATx.Init.Channel				= DMA_CHANNEL_4,
	.tUartDMA.tDMATx.Init.FIFOMode				= DMA_FIFOMODE_ENABLE,
#endif
	.tUartDMA.tDMATx.Init.Direction				= DMA_MEMORY_TO_PERIPH,
	.tUartDMA.tDMATx.Init.PeriphInc				= DMA_PINC_DISABLE,
	.tUartDMA.tDMATx.Init.MemInc				= DMA_MINC_ENABLE,
	.tUartDMA.tDMATx.Init.PeriphDataAlignment	= DMA_PDATAALIGN_BYTE,
	.tUartDMA.tDMATx.Init.MemDataAlignment		= DMA_MDATAALIGN_BYTE,
	.tUartDMA.tDMATx.Init.Mode					= DMA_NORMAL,
	.tUartDMA.tDMATx.Init.Priority				= DMA_PRIORITY_HIGH,

	.tTxInfo.usDMATxMAXSize						= 50,						/* DMA发送缓冲区大小 */
	
	.tUartDMA.ucDMATxPriority					= 1,						/* DMA发送中断优先级 */
	.tUartDMA.ucDMATxSubPriority				= 1,						/* DMA发送中断子优先级 */

	//串口GPIO配置
	.tGPIO[0].tGPIOInit.Pin 					= GPIO_PIN_9,				/* GPIO引脚 */
	.tGPIO[0].tGPIOInit.Mode 					= GPIO_MODE_AF_PP,			/* GPIO模式 */
	.tGPIO[0].tGPIOInit.Pull 					= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
	.tGPIO[0].tGPIOInit.Speed 					= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
	.tGPIO[0].tGPIOPort 						= GPIOA,					/* GPIO分组 */
#ifdef STM32F1_SGA_ENABLE
	.tGPIO[0].ucAFMode							= NO_REMAP,					/* GPIO重映射 */
#endif
#ifdef STM32F4_SGA_ENABLE
	.tGPIO[0].tGPIOInit.Alternate				= GPIO_AF7_USART1,
#endif

	.tGPIO[1].tGPIOInit.Pin 					= GPIO_PIN_10,				/* GPIO引脚 */
	.tGPIO[1].tGPIOInit.Pull 					= GPIO_NOPULL,				/* GPIO上下拉设置，是否需要上下拉看硬件 */
	.tGPIO[1].tGPIOInit.Speed 					= GPIO_SPEED_FREQ_HIGH,		/* GPIO速度 */	
	.tGPIO[1].tGPIOPort 						= GPIOA,					/* GPIO分组 */
#ifdef STM32F1_SGA_ENABLE	
	.tGPIO[1].tGPIOInit.Mode 					= GPIO_MODE_INPUT,			/* F4系列需要设置为输入模式 */
	.tGPIO[1].ucAFMode							= NO_REMAP,					/* GPIO重映射 */
#endif
#ifdef STM32F4_SGA_ENABLE
	.tGPIO[1].tGPIOInit.Mode 					= GPIO_MODE_AF_PP,			/* F4系列需要设置为复用推挽 */
	.tGPIO[1].tGPIOInit.Alternate				= GPIO_AF7_USART1,
#endif
};

