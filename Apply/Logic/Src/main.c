#include "main.h"
#include "drv_hal_conf.h"	/* 裸机相关定义以及RTOS开关 */ 
#include "task_userinit.h"
#include "usercode.h"       

#if !defined(RTT_ENABLE)
#include "task_sysinit.h"
#endif

int main()
{
#if !defined(RTT_ENABLE)
    Task_Sys_Init();		/* 系统初始化，必需 */
#endif
    
    Task_UserInit();		/* 用户外设初始化 */	
    
#ifdef RTT_ENABLE
	rt_kprintf("RTT Start!\r\n");
	/*
     * 开发板硬件初始化， RT-Thread 系统初始化已经在 main 函数之前完成
     * 即在 component.c 文件中的 rtthread_startup()函数中完成了。 
     * 1.在 usercode 中，创建线程和启动线程。
     * 2.在 threadpool 中，写入线程入口函数。
    */
#endif

#ifdef FREERTOS_ENABLE
    printf("FreeRTOS Start!\r\n");
	/*
     * 1.在 usercode 中，创建线程和启动线程。
     * 2.在 threadpool 中，写入线程入口函数。
     * 3.手动调用osKernelStart()函数开启调度。
    */
#endif

    UserLogic_Code();		/* 用户逻辑执行 */
}

