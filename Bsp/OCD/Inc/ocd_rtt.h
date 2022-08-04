#ifndef __UT_SIGMA_RTT_H_
#define __UT_SIGMA_RTT_H_

#include "drv_hal_conf.h"

#include <rthw.h>
#include <rtthread.h>

#define SGA_RTT_ENABLE

/* whether use board external SRAM memory */
// <e>Use external SRAM memory on the board
// 	<i>Enable External SRAM memory
#define STM32_EXT_SRAM 0
//	<o>Begin Address of External SRAM
//		<i>Default: 0x68000000
#define STM32_EXT_SRAM_BEGIN 0x68000000 /* the begining address of external SRAM */
//	<o>End Address of External SRAM
//		<i>Default: 0x68080000
#define STM32_EXT_SRAM_END 0x68080000 /* the end address of external SRAM */
// </e>
// <o> Internal SRAM memory size[Kbytes] <8-64>
//	<i>Default: 64
#define STM32_SRAM_END (0x20000000 + STM32_SRAM_SIZE * 1024)
#ifdef __CC_ARM
extern int Image$$RW_IRAM1$$ZI$$Limit;
#define HEAP_BEGIN  ((void *)&Image$$RW_IRAM1$$ZI$$Limit)
#elif __ICCARM__
#pragma section="HEAP"
#define HEAP_BEGIN  (__segment_end("HEAP"))
#else
extern int __bss_end;
#define HEAP_BEGIN  ((void *)&__bss_end)
#endif
#define HEAP_END    STM32_SRAM_END

typedef struct 
{
	const char		ucpName[RT_NAME_MAX];
	void (*fun)();					/* 入口函数 */
	void       		*parameter;
	uint32_t		ulStackSize;	/* 栈大小 */
	uint8_t			ucPriority;			/* 优先级 */
	uint32_t		ucTick;			/* 时间片 */
	
	rt_thread_t		pThread;
}tagRTTThreadInfo_T;	/* ad411x_st_reg */

void rt_hw_board_init(void);

#endif
