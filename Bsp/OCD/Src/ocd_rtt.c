#include "ocd_rtt.h"

#ifdef RTT_ENABLE
void rt_hw_board_init(void)
{
    Drv_HAL_Init();		/* 框架初始化 */
	SystemCoreClockUpdate();	/* 时钟更新 */
	
#ifdef RT_USING_HEAP
    rt_system_heap_init((void *)HEAP_BEGIN, (void *)HEAP_END);
#endif
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif
}

void SysTick_Handler(void)
{
	/* enter interrupt */
    rt_interrupt_enter();
    Drv_HAL_IncTick();
    rt_tick_increase();
    /* leave interrupt */
    rt_interrupt_leave();
}
#endif
