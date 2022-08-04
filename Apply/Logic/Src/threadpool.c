#include "threadpool.h"

void (*ucNowThread)();
void (*ucNextThread)();

void ThreadPool(void)
{
//	ucNowThread = Thread_Sleep;	/* 默认线程 */
	
	while(1)
	{
		ucNowThread();	/* 执行当前线程 */
		ucNowThread = ucNextThread;
	}
}
