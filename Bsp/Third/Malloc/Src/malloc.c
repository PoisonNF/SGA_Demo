#include "malloc.h"	 

/* 内存参数设定 */
#define MEM_BLOCK_SIZE			32  	  						/* 内存块大小为32字节 */
#define MEM_MAX_SIZE			35*1024  						/* 最大管理内存 35K */
#define MEM_ALLOC_TABLE_SIZE	MEM_MAX_SIZE/MEM_BLOCK_SIZE 	/* 内存表大小 */

static unsigned char UT_Mem_Perused(void);					/* 得内存使用率(外/内部调用) */

static __align(4) unsigned char s_mem_base[MEM_MAX_SIZE];				/* SRAM内存池，(4字节对齐) */
static unsigned short s_mem_mapBase[MEM_ALLOC_TABLE_SIZE];			/* SRAM内存池MAP */   
static const unsigned int s_mem_list_size = MEM_ALLOC_TABLE_SIZE;		/* 内存表大小 */
static const unsigned int s_mem_piece_size = MEM_BLOCK_SIZE;				/* 内存分块大小 */
static const unsigned int s_mem_size = MEM_MAX_SIZE;					/* 内存总大小 */

/* 内存管理控制器 */
struct _mem_allocation_dev
{
	void 			(*init)(void);		/* 初始化 */
	unsigned char	(*perused)(void);	/* 内存使用率 */
	unsigned char	*s_mem_base;				/* 内存池 */
	unsigned short	*memMap;				/* 内存管理状态表 */
	unsigned char	memRdy;					/* 内存管理是否就绪 */
};

/* 内存管理控制器 */
static struct _mem_allocation_dev s_mem_allocation_dev=
{
	UT_Mem_Init,		/* 内存初始化 */
	UT_Mem_Perused,		/* 内存使用率 */
	s_mem_base,			/* 内存池 */
	s_mem_mapBase,			/* 内存管理状态表 */
	0,  				/* 内存管理未就绪 */
};

/**
 * @brief 内存赋值
 * @param *_des-目的地址
 * @param *_src-源地址
 * @param _lenth-需要复制的内存长度(字节为单位)
 * @retval null
*/
static void UT_Mem_Copy(void *_des,void *_src,unsigned int _lenth)  
{  
    unsigned char *xdes=_des;
	unsigned char *xsrc=_src; 
    while(_lenth--)*xdes++=*xsrc++;  
}  

/**
 * @brief 设置内存
 * @param *_src-源地址
 * @param _val-要设置的值
 * @param _size-需要设置的内存大小(字节为单位)
 * @retval null
*/
static void UT_Mem_Set(void *_src,unsigned char _val,unsigned int _size)  
{  
    unsigned char *xs = _src;  
    while(_size--)*xs++=_val; 
}	  

/**
 * @brief 内存分配
 * @param _size-要分配的内存大小(字节)
 * @retval 0XFFFFFFFF,代表错误;其他,内存偏移地址 
*/
static unsigned int UT_Mem_MyAllocation(unsigned int _size)  
{  
    signed long offset=0;  
    unsigned short nmemb;						/* 需要的内存块数  */
	unsigned short cmemb=0;						/* 连续空内存块数 */
    unsigned int i;  
    if(!s_mem_allocation_dev.memRdy)
		s_mem_allocation_dev.init();						/* 未初始化,先执行初始化 */
    if(_size==0)
		return 0XFFFFFFFF;						/* 不需要分配 */
	
    nmemb=_size/s_mem_piece_size;  					/* 获取需要分配的连续内存块数 */
	
    if(_size%s_mem_piece_size)
		nmemb++;  
    for(offset=s_mem_list_size-1;offset>=0;offset--)	/* 搜索整个内存控制区  */ 
    {     
		if(!s_mem_allocation_dev.memMap[offset])
			cmemb++;	/* 连续空内存块数增加 */
		else 
			cmemb=0;							/* 连续内存块清零 */
		
		if(cmemb==nmemb)						/* 找到了连续nmemb个空内存块 */
		{
            for(i=0;i<nmemb;i++)  				/* 标注内存块非空  */
            {  
                s_mem_allocation_dev.memMap[offset+i]=nmemb;  
            }  
            return (offset*s_mem_piece_size);			/* 返回偏移地址   */
		}
    }  
    return 0XFFFFFFFF;/* 未找到符合分配条件的内存块  */ 
} 

/**
 * @brief 释放内存
 * @param _offset-内存地址偏移
 * @retval 0,释放成功;1,释放失败; 
*/
static unsigned char UT_Mem_Clear(unsigned int _offset)  
{  
    int i;  
    if(!s_mem_allocation_dev.memRdy)					/* 未初始化,先执行初始化 */
	{
		s_mem_allocation_dev.init();    
        return 1;							/* 未初始化 */ 
    }  
    if(_offset<s_mem_size)						/* 偏移在内存池内 */
    {  
        int index=_offset/s_mem_piece_size;		/* 偏移所在内存块号码 */
        int nmemb=s_mem_allocation_dev.memMap[index];	/* 内存块数量 */
        for(i=0;i<nmemb;i++)  				/* 内存块清零 */
        {  
            s_mem_allocation_dev.memMap[index+i]=0;  
        }
        return 0;  
    }
	else 
		return 2;							/* 偏移超区了 */
}  

/**
 * @brief 内存管理初始化
 * @retval null
*/
void UT_Mem_Init(void)  
{  
    UT_Mem_Set(s_mem_allocation_dev.memMap, 0,s_mem_list_size*2);//内存状态表数据清零  
	UT_Mem_Set(s_mem_allocation_dev.s_mem_base, 0,s_mem_size);	//内存池所有数据清零  
	s_mem_allocation_dev.memRdy=1;						//内存管理初始化OK  
}  

/**
 * @brief 获取内存使用率
 * @retval 返回值:使用率(0~100)
*/
unsigned char UT_Mem_Perused(void)  
{  
    unsigned int used=0;  
    unsigned int i;  
	
    for(i=0;i<s_mem_list_size;i++)  
    {  
        if(s_mem_allocation_dev.memMap[i])
			used++; 
    }
	
    return (used*100)/(s_mem_list_size);  
}  

/**
 * @brief 释放内存(外部调用) 
 * @param *_ptr-内存首地址
 * @retval null
*/
void UT_Mem_Free(void *_ptr)  
{  
	unsigned int offset;  
    if(_ptr==0)
		return;				/* 地址为0 */  
 	offset=(unsigned int)_ptr-(unsigned int)s_mem_allocation_dev.s_mem_base;  
    UT_Mem_Clear(offset);	/* 释放内存 */     
}  

/**
 * @brief 分配内存(外部调用) 
 * @param _size-内存大小(字节)
 * @retval 分配到的内存首地址
*/
void *UT_Mem_Allocation(unsigned int _size)  
{  
    unsigned int offset;  									      
	offset=UT_Mem_MyAllocation(_size);  	   				   
    if(offset==0XFFFFFFFF)
		return 0;  
    else 
		return (void*)((unsigned int)s_mem_allocation_dev.s_mem_base+offset);  
}  

/**
 * @brief 重新分配内存(外部调用) 
 * @param *_ptr-旧内存首地址
 * @param _size-内存大小(字节)
 * @retval 新分配到的内存首地址
*/
void *UT_Mem_ReAllocation(void *_ptr,unsigned int _size)  
{  
    unsigned int offset;  
    offset=UT_Mem_MyAllocation(_size);  
    if(offset==0XFFFFFFFF)
		return 0;     
    else  
    {  									   
	    UT_Mem_Copy((void*)((unsigned int)s_mem_allocation_dev.s_mem_base+offset),_ptr,_size);	//拷贝旧内存内容到新内存   
        UT_Mem_Free(_ptr);  											  	//释放旧内存
        return (void*)((unsigned int)s_mem_allocation_dev.s_mem_base+offset);  			//返回新内存首地址
    }  
}
