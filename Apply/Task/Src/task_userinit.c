#include "task_conf.h"

#include "bsp_io.h"

void Task_UserInit(void)
{
	Drv_GPIO_Init(demoGPIO, 3);
    
    Drv_Uart_ITInit(&demoUart);
}
