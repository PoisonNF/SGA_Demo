#include "task_conf.h"

#include "config.h"

/**
 * @brief 外部中断5-9任务函数
 * @param null
 * @retval Null
*/
__weak void Task_EXTI9_5_IRQHandler(void)
{
	
}

/**
 * @brief 外部中断5-9服务函数
 * @param null
 * @retval Null
*/
void EXTI9_5_IRQHandler(void)
{
//	Drv_GPIO_EXTI_IRQHandler(&DS3231_ALARM_IRQ);	/* 必须加，参数需修改 */
	
	Task_EXTI9_5_IRQHandler();
}

/**
 * @brief 外部中断10-15任务函数
 * @param null
 * @retval Null
*/
__weak void Task_EXTI15_10_IRQHandler(void)
{
	
}

/**
 * @brief 外部中断10-15服务函数
 * @param null
 * @retval Null
*/
void EXTI15_10_IRQHandler(void)
{
//	Drv_GPIO_EXTI_IRQHandler(&tPCUart.tGPIO[1]);	/* 必须加，参数需修改 */
	
	Task_EXTI15_10_IRQHandler();
}

/**
 * @brief 串口1中断任务函数
 * @param null
 * @retval Null
*/
__weak void Task_USART1_IRQHandler(void)
{
	
}

/**
 * @brief 串口1中断服务函数
 * @param null
 * @retval Null
*/
void USART1_IRQHandler(void)
{
	/* 示例 */
//	Drv_Uart_IRQHandler(&tPCUart);		/* 必需部分 */
	
	Task_USART1_IRQHandler();
}

/**
 * @brief 串口2中断任务函数
 * @param null
 * @retval Null
*/
__weak void Task_USART2_IRQHandler(void)
{
	
}

/**
 * @brief 串口1中断服务函数
 * @param null
 * @retval Null
*/
void USART2_IRQHandler(void)
{
		/* 示例 */
//	Drv_Uart_IRQHandler(&tJY901B.tUART);		/* 必需部分 */
//	
//	Task_USART2_IRQHandler();
}

/**
 * @brief 串口3中断服务函数
 * @param null
 * @retval Null
*/
void USART3_IRQHandler(void)
{

}

/**
 * @brief 串口4中断服务函数
 * @param null
 * @retval Null
*/
void UART4_IRQHandler(void)
{

}

/**
 * @brief 串口5中断服务函数
 * @param null
 * @retval Null
*/
void UART5_IRQHandler(void)
{

}

/**
 * @brief 定时器2中断服务函数
 * @param null
 * @retval Null
*/
void TIM2_IRQHandler(void)
{
	/* 示例 */
//    Drv_Timer_IRQHandler(&demoTIM);
}

/**
 * @brief 定时器3中断服务函数
 * @param null
 * @retval Null
*/
void TIM3_IRQHandler(void)
{
	/* 示例 */
//    Drv_Timer_IRQHandler(&demoTIM);
}

/**
 * @brief 定时器4中断服务函数
 * @param null
 * @retval Null
*/
void TIM4_IRQHandler(void)
{
    /* 示例 */
//    Drv_Timer_IRQHandler(&demoTIM);
}

/**
 * @brief 定时器5中断服务函数
 * @param null
 * @retval Null
*/
void TIM5_IRQHandler(void)
{
    /* 示例 */
//    Drv_Timer_IRQHandler(&demoTIM);
}

/**
 * @brief 定时器6中断服务函数
 * @param null
 * @retval Null
*/
void TIM6_IRQHandler(void)
{
    /* 示例 */
//    Drv_Timer_IRQHandler(&demoTIM);
}

/**
 * @brief 定时器7中断服务函数
 * @param null
 * @retval Null
*/
void TIM7_IRQHandler(void)
{
	/* 示例 */
//    Drv_Timer_IRQHandler(&demoTIM);
}



