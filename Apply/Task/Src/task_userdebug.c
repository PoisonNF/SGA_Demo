#include "task_conf.h"

#include "bsp_io.h"

void Task_UserDebug(void)
{
	Drv_GPIO_Write(&demoGPIO[0],GPIO_PIN_RESET);
    Drv_GPIO_Write(&demoGPIO[1],GPIO_PIN_RESET);
    Drv_GPIO_Write(&demoGPIO[2],GPIO_PIN_SET);
    
    //HAL_UART_Transmit(&demoUart.tUARTHandle,"aaaa",4,1000);
    //Drv_Uart_Transmit(&demoUart,"aaaa",4);
    //printf("abcd\r\n");
    //printf("test\r\n");
}
