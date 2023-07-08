#include "task_conf.h"

#include "config.h"

/* SGA库初始化函数 */
void Task_Sys_Init(void)
{
	Drv_HAL_Init();		/* SGA库启动必需 */
}
