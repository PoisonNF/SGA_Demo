#include "task_conf.h"

#include <rthw.h>
#include <rtthread.h>

static void Thread_Main(void *parameter);
static void Thread_UserInit(void *parameter);
static void Thread_PCListner(void *parameter);
static void Thread_FATFSDataSave(void *parameter);
static void Thread_FATFSDataRead(void *parameter);
static void Thread_Sleep(void *parameter);
static void Thread_DataTx(void *parameter);
static void Thread_PCTx(void *parameter);

#define ucThreadNum	8
static tagRTTThreadInfo_T s_tRTTInfo[ucThreadNum] =
{
	/* 线程名		入口函数					参数 		栈大小	优先级	时间片	线程指针	*/
	{"Main",		Thread_Main,				RT_NULL,	512,	2,		5, 		RT_NULL},
	{"UserInit",	Thread_UserInit,			RT_NULL,	1024,	1,		5, 		RT_NULL},
	{"PCLis",		Thread_PCListner, 			RT_NULL,	512,	3,		5, 		RT_NULL},
	{"FATFSSa",		Thread_FATFSDataSave,		RT_NULL,	512,	5,		10, 	RT_NULL},
	{"FATFSRe",		Thread_FATFSDataRead,		RT_NULL,	512,	5,		10, 	RT_NULL},
	{"Sleep",		Thread_Sleep,				RT_NULL,	512,	4,		10, 	RT_NULL},
	{"DataTx",		Thread_DataTx,				RT_NULL,	512,	4,		10, 	RT_NULL},
	{"PCTx",		Thread_PCTx,				RT_NULL,	512,	3,		10, 	RT_NULL}
};

static void Thread_PCTx(void *parameter)
{
	while(1)
	{		
		Task_PCTx();
		
		rt_thread_resume(s_tRTTInfo[5].pThread);	/* 线程唤醒 */
		
		rt_thread_suspend(s_tRTTInfo[7].pThread);	/* 线程挂起 */
		rt_schedule();								/* 线程切换 */
	}
}

static void Thread_DataTx(void *parameter)
{
	while(1)
	{		
		rt_thread_suspend(s_tRTTInfo[6].pThread);	/* 线程挂起 */
		rt_schedule();								/* 线程切换 */
		
		Task_DataTx();
		
		rt_thread_resume(s_tRTTInfo[5].pThread);	/* 休眠线程唤醒 */
	}
}

static void Thread_Sleep(void *parameter)
{
	while(1)
	{		
		rt_thread_suspend(s_tRTTInfo[5].pThread);	/* 线程挂起 */
		rt_schedule();								/* 线程切换 */
		
		printf("Sleep Start!!!\r\n");
		Task_Sleep();
		printf("Sleep End!!!\r\n");
		
		rt_thread_resume(s_tRTTInfo[7].pThread);	/* 线程唤醒 */
	}
}

static void Thread_FATFSDataRead(void *parameter)
{
	while(1)
	{
		rt_thread_suspend(s_tRTTInfo[4].pThread);	/* 线程挂起 */
		rt_schedule();							/* 线程切换 */
		
		Task_FATFSDataRead();					/* 数据存储具体实现 */
	}
}

static void Thread_FATFSDataSave(void *parameter)
{
	while(1)
	{
		rt_thread_suspend(s_tRTTInfo[3].pThread);	/* 线程挂起 */
		rt_schedule();							/* 线程切换 */
		
		Task_FATFSDataSave();					/* 数据存储具体实现 */
	}
}

static void Thread_PCListner(void *parameter)
{
	while (1)
    {
		/* 方法一，禁止调度 */
		/* 临界区保护 */
		rt_enter_critical();	/* 进入临界区，禁止调度 */
        
		rt_exit_critical();		/* 退出临界区，调度器解锁 */
		
		/* 临界区保护 */
        rt_thread_mdelay(500);
    }
}

static void Thread_UserInit(void *parameter)
{	
	Task_UserInit();
	
	printf("执行线程->用户初始化\r\n");
}

static void Thread_Main(void *parameter)
{
	while(1)
	{
		rt_thread_mdelay(1000);
	}
}

void RTT_ThreadCreate(void)
{
	uint8_t index;
	
	for(index = 0; index < ucThreadNum; index++)
	{
		s_tRTTInfo[index].pThread = rt_thread_create(
										s_tRTTInfo[index].ucpName,
										s_tRTTInfo[index].fun,
										s_tRTTInfo[index].parameter,
										s_tRTTInfo[index].ulStackSize,
										s_tRTTInfo[index].ucPriority,
										s_tRTTInfo[index].ucTick
										);
		
		if(s_tRTTInfo[index].pThread != RT_NULL)
			rt_thread_startup(s_tRTTInfo[index].pThread);
	}
}

void RTT_ThreadDispatch(uint8_t _ucIndex)
{
	rt_thread_resume(s_tRTTInfo[_ucIndex].pThread);	/* 线程唤醒 */
}
