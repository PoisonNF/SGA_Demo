#include "main.h"
#include "drv_hal_conf.h"	/* 裸机相关定义 */ 
#include "task_userinit.h"
#include "usercode.h"

#ifdef RTT_ENABLE
#include <rtthread.h>		/* RTOS参数修改在rtconfig.h中*/
#else
#include "task_sysinit.h"
#endif

int main()
{
#ifdef	RTT_ENABLE
	rt_kprintf("RTT 启动\r\n");
	/*
     * 开发板硬件初始化， RT-Thread 系统初始化已经在 main 函数之前完成，
     * 即在 component.c 文件中的 rtthread_startup()函数中完成了。 
     * 所以在 main 函数中，只需要创建线程和启动线程即可。
    */
#else	/* 裸机代码 */
	Task_Sys_Init();		/* 系统初始化，必需 */
#endif
    Task_UserInit();		/* 用户外设初始化 */	
    UserLogic_Code();		/* 用户逻辑执行 */
}

