#include "main.h"

#ifndef RTT_ENABLE
#include "task_sysinit.h"
#include "task_userinit.h"
#include "threadpool.h"
#include "task_userdebug.h"
#else
#include "task_rttinit.h"
#endif

int main()
{
#ifndef	RTT_ENABLE
	Task_Sys_Init();		/* 系统初始化，必需 */
	Task_UserInit();
	
	Task_UserDebug();

	ThreadPool();
#else
	RTT_ThreadCreate();		/* RTT线程创建 */
#endif
}
