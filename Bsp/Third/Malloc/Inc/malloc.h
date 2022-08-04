#ifndef __MALLOC_H
#define __MALLOC_H
   
void UT_Mem_Init(void);					 //内存管理初始化函数(外/内部调用)
void UT_Mem_Free(void *ptr);  				//内存释放(外部调用)
void *UT_Mem_Allocation(unsigned int size);				//内存分配(外部调用)
void *UT_Mem_ReAllocation(void *ptr,unsigned int size);	//重新分配内存(外部调用)
#endif
