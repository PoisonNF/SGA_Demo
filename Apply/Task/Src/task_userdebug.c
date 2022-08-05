#include "task_conf.h"

#include "bsp_io.h"

void Task_UserDebug(void)
{
    
    printf("test\r\n");

    Drv_GPIO_Write(&demoGPIO[0],GPIO_PIN_SET);
    Drv_GPIO_Write(&demoGPIO[1],GPIO_PIN_RESET);
    Drv_GPIO_Write(&demoGPIO[2],GPIO_PIN_SET);
    
    while(1)
    {

    }
}
