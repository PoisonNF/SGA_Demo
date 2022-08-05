#include "main.h"
#include "drv_hal_conf.h"	/* 裸机相关定义 */ 


#ifdef RTT_ENABLE
#include <rtthread.h>		/* RTOS参数修改在rtconfig.h中*/
#else
#include "task_sysinit.h"
#include "task_userinit.h"
#include "task_userdebug.h"
#endif

int main()
{
#ifdef	RTT_ENABLE
	rt_kprintf("RTT 启动\r\n");
#else
	Task_Sys_Init();		/* 系统初始化，必需 */
	Task_UserInit();		/* 用户外设初始化 */
	
	Task_UserDebug();		/* 用户逻辑执行 */
#endif
}
