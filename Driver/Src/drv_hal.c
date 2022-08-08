/****************************************************************************

* Sigma团队

* 文件名: drv_hal.c

* 内容简述：hal库驱动文件

* 文件历史：

* 版本号	日期		作者		说明

* 1.0.0a 	2020-02-22	李环宇		创建该文件

****************************************************************************/
#include "drv_hal_conf.h"

#ifdef DRV_HAL_ENABLE
/**
 * @brief Debug输出
 * @param *_cpMsg
 * @param *_cpFunc-对应函数指针
 * @param _iLine-对应行号
 * @param *_cpFmt
 * @retval Null
*/
static char s_cpOutput[256]={0};
void Drv_HAL_PrintLog(const char *_cpMsg,
            const char *_cpFunc,   
            const int   _iLine,
            const char *_cpFmt,...)
{
        va_list vaArglst;

        va_start(vaArglst,_cpFmt);
        vsnprintf(s_cpOutput,sizeof(s_cpOutput),_cpFmt,vaArglst);
        printf("[%s][%s():%04d] %s\r\n",_cpMsg, _cpFunc, _iLine, s_cpOutput);
        va_end(vaArglst);
}

/**
 * @brief Debug输出
 * @param *_cpMsg
 * @retval Null
*/
void Drv_HAL_PrintBuf(const char *_cpMsg, ...)
{
        uint16_t i;
        uint8_t* ucpBuf;
        uint32_t ulCnt;
        va_list vaArglst;
	
        va_start(vaArglst, _cpMsg);
        ucpBuf = va_arg( vaArglst, uint8_t*);        
        ulCnt = va_arg( vaArglst, uint32_t);

        printf("[%s][L:%d] ", _cpMsg,ulCnt);
        for(i = 0; i < ulCnt; i++)
        {
                printf("0x%02X,",ucpBuf[i]);
        }
        printf("\n");
        va_end(vaArglst);
}

/**
 * @brief 程序信息输出
 * @param NULL
 * @retval Null
*/
void Drv_HAL_ProgramInfoOutput(void)
{
	printf("\r\nDriver Version   ->%s\r\n\r\n", DRIVER_VERSION);
	printf("\r\nProgram Build    ->%s %s\r\n\r\n", __DATE__, __TIME__);
}

/**
 * @brief 系统报错函数
 * @param *file-文件名指针
 * @param *_line-行号
 * @retval Null
*/
void Drv_HAL_Error(char *_cpFile, uint32_t _ulLine)
{
	/* 	用户可以添加自己的代码报告源代码文件名和代码行号，比如将错误文件和行号打印到串口*/
	printf("->Happen Error!!!\r\n"); 
	printf("->Error Parameters Value: file %s on line %d\r\n", _cpFile, _ulLine); 
}

/**
 * @brief 系统时钟初始化函数
 * @param null
 * @retval Null
*/
static void S_HAL_CLKConfig(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  
#if defined (STM32F1_SGA_ENABLE)
	/** Initializes the CPU, AHB and APB busses clocks 
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		while(1);
	}
	/** Initializes the CPU, AHB and APB busses clocks 
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
								  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		while(1);
	}
#endif

#if defined (STM32L4_SGA_ENABLE)	
	/* 内部低速晶振 start */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 10;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	/* 内部低速晶振 end */
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		while(1);
	}
	/** Initializes the CPU, AHB and APB buses clocks
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
								  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
	{
		while(1);
	}
#endif
}

/**
 * @brief RTT调度时钟初始化函数
 * @param Null
 * @retval Null
*/
#ifdef RTT_ENABLE
static void S_HAL_RTTCLKInit(void)
{	
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);	
	HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}
#endif

/**
 * @brief SGA库初始化函数
 * @param null
 * @retval null
*/
void Drv_HAL_Init(void)
{
	HAL_Init();			/* HAL库初始化 */
	S_HAL_CLKConfig();	/* 系统时钟初始化 */
#ifdef RTT_ENABLE
	S_HAL_RTTCLKInit();
#endif
}

/**
 * @brief 滴答时钟使能
 * @param null
 * @retval null
*/
void Drv_HAL_SysTickEnable(void)
{
	HAL_ResumeTick();
}

/**
 * @brief 滴答时钟失能
 * @param null
 * @retval null
*/
void Drv_HAL_SysTickDisable(void)
{
	HAL_SuspendTick();
}

/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}



/**
  * @brief This function handles System tick timer.
  */
void Drv_HAL_IncTick(void)
{
	/* USER CODE BEGIN SysTick_IRQn 0 */

	/* USER CODE END SysTick_IRQn 0 */
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
	/* USER CODE BEGIN SysTick_IRQn 1 */

	/* USER CODE END SysTick_IRQn 1 */
}

/**
 * @brief hal库用户自定义初始化函数
 * @param null
 * @retval Null
*/
void HAL_MspInit(void)
{
#ifdef STM32L4_SGA_ENABLE
	__HAL_RCC_SYSCFG_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();

	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);
#endif
	
#ifdef STM32F1_SGA_ENABLE
    /* 优先级分组，主优先级4位，副优先级4位 */
	  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
    /* System interrupt init*/
    __HAL_RCC_AFIO_CLK_ENABLE();
    /* MemoryManagement_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
    /* BusFault_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
    /* UsageFault_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
    /* SVCall_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SVCall_IRQn, 0, 0);
    /* DebugMonitor_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);
    /* PendSV_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);
    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
    /** JTAG-DP Disabled and SW-DP Enabled**/
    __HAL_AFIO_REMAP_SWJ_NOJTAG();
#endif
}

/**
 * @brief SGA库printf重定义
 * @param null
 * @retval null
*/
#pragma import(__use_no_semihosting)                   
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
	   
void _sys_exit(int x) 
{ 
	x = x; 
} 
	
int fputc(int ch, FILE *f)
{      
    uint16_t usCnt=0;

#if	defined (STM32F1_SGA_ENABLE)
	/* F1系列 */
	while((PRINTF_UART->SR&0X40)==0)//循环发送,直到发送完毕  
#else
	/* L4系列 */
	while((PRINTF_UART->ISR&0X40)==0)//循环发送,直到发送完毕  
#endif
    {
        if((usCnt++)>60000)//防止异常超时退出
        {
            break;
        }          
    }   
#if	defined (STM32F1_SGA_ENABLE)	
	/* F1系列 */
    PRINTF_UART->DR = (uint8_t) ch; 
#else
	/* L4系列 */
	PRINTF_UART->TDR = (uint8_t) ch; 
#endif
        
	return ch;
}
#endif





