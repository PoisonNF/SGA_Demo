/****************************************************************************

* Sigma�Ŷ�

* �ļ���: drv_hal.c

* ���ݼ�����hal�������ļ�

* �ļ���ʷ��

* �汾��	  ����		  ����		   ˵��
*    	  2024-05-31	�����		����DWT�����ʼ������

*  3.0 	  2023-01-26	�����		����STM32F4ϵ��

*  2.2 	  2023-03-29	�����		����task_irq.c�µײ��жϺ���

* 1.0.0a  2020-02-22	���		�������ļ�

****************************************************************************/
#include "drv_hal_conf.h"

#ifdef DRV_HAL_ENABLE

static char s_cpOutput[256] = {0};
/**
 * @brief Debug���
 * @param _cpMsg
 * @param _cpFunc-��Ӧ����ָ��
 * @param _iLine-��Ӧ�к�
 * @param _cpFmt
 * @retval Null
*/
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
 * @brief Debug���
 * @param _cpMsg
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
 * @brief ������Ϣ���
 * @param Null
 * @retval Null
*/
void Drv_HAL_ProgramInfoOutput(void)
{
	printf("\r\nDriver Version   ->%s\r\n\r\n", DRIVER_VERSION);
	printf("\r\nProgram Build    ->%s %s\r\n\r\n", __DATE__, __TIME__);
}

/**
 * @brief ϵͳ������
 * @param _cpFile-�ļ���ָ��
 * @param _ulLine-�к�
 * @retval Null
*/
void Drv_HAL_Error(char *_cpFile, uint32_t _ulLine)
{
	/* 	�û���������Լ��Ĵ��뱨��Դ�����ļ����ʹ����кţ����罫�����ļ����кŴ�ӡ������*/
	printf("->Happen Error!!!\r\n"); 
	printf("->Error Parameters Value: file %s on line %d\r\n", _cpFile, _ulLine); 
}

/**
 * @brief ϵͳʱ�ӳ�ʼ������
 * @param Null
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

#if defined (STM32F4_SGA_ENABLE)

  	/** Configure the main internal regulator output voltage
  	*/
  	__HAL_RCC_PWR_CLK_ENABLE();
  	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  	/** Initializes the RCC Oscillators according to the specified parameters
  	* in the RCC_OscInitTypeDef structure.
  	*/
  	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  	RCC_OscInitStruct.PLL.PLLM = 4;
  	RCC_OscInitStruct.PLL.PLLN = 168;
  	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  	RCC_OscInitStruct.PLL.PLLQ = 7;
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
  	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  	{
  	  	while(1);
  	}

#endif

#if defined (STM32L4_SGA_ENABLE)	
	/* �ڲ����پ��� start */
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
	/* �ڲ����پ��� end */
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
 * @brief RTT����ʱ�ӳ�ʼ������
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
 * @brief �δ�ʱ��ʹ��
 * @param Null
 * @retval Null
*/
void Drv_HAL_SysTickEnable(void)
{
	HAL_ResumeTick();
}

/**
 * @brief �δ�ʱ��ʧ��
 * @param Null
 * @retval Null
*/
void Drv_HAL_SysTickDisable(void)
{
	HAL_SuspendTick();
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

#ifdef FREERTOS_ENABLE
/**
 * @brief DWT�����ʼ��
*/
static void S_HAL_DWT_Init()
{
    DEM_CR |= DEM_CR_TRCENA; 		/*��DEMCR�Ĵ�����λ24���ƣ�д1ʹ��DWT���衣*/
    DWT_CYCCNT = 0;					/*����DWT��CYCCNT�����Ĵ�����0��*/
    DWT_CR |= DWT_CR_CYCCNTENA;		/*��DWT���ƼĴ�����λ0���ƣ�д1ʹ��CYCCNT�Ĵ�����*/
}
#endif

/**
 * @brief SGA���ʼ������
 * @param Null
 * @retval Null
*/
void Drv_HAL_Init(void)
{
	HAL_Init();			/* HAL���ʼ�� */
	S_HAL_CLKConfig();	/* ϵͳʱ�ӳ�ʼ�� */
	
#ifdef RTT_ENABLE
	S_HAL_RTTCLKInit();
#endif

#ifdef FREERTOS_ENABLE
	S_HAL_DWT_Init();
	osKernelInitialize();
#endif
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

#ifndef FREERTOS_ENABLE
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
#endif

#if !defined(FREERTOS_ENABLE) && !defined(RTT_ENABLE)
/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
 * @brief ϵͳ�δ��ж�
 * @param Null
 * @retval Null
*/
void SysTick_Handler(void)
{
    Drv_HAL_IncTick();
}

/**
 * @brief HAL��ϵͳ�����ж�
 * @param Null
 * @retval Null
*/
void HardFault_Handler(void)
{
	while(1)
	{
		/* USER CODE BEGIN W1_HardFault_IRQn 0 */
		/* USER CODE END W1_HardFault_IRQn 0 */
	}
}
#endif

#ifdef FREERTOS_ENABLE
#include "task.h"
/**
 * @brief Ӧ����FreeRTOS��ϵͳ�δ��ж�
 * @param Null
 * @retval Null
*/
void SysTick_Handler(void)
{
	HAL_IncTick();
#if (INCLUDE_xTaskGetSchedulerState == 1 )
	if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
	{
#endif /* INCLUDE_xTaskGetSchedulerState */
	xPortSysTickHandler();
#if (INCLUDE_xTaskGetSchedulerState == 1 )
	}
#endif /* INCLUDE_xTaskGetSchedulerState */
}
#endif

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
 * @brief hal���û��Զ����ʼ������
 * @param Null
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
    /* ���ȼ����飬�����ȼ�4λ�������ȼ�0λ */
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    /* System interrupt init*/
    __HAL_RCC_AFIO_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();

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

#ifdef STM32F4_SGA_ENABLE
	__HAL_RCC_SYSCFG_CLK_ENABLE();
   	__HAL_RCC_PWR_CLK_ENABLE();

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
#endif
}

/**
 * @brief SGA��printf�ض���
 * @param Null
 * @retval Null
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

#if	defined (STM32F1_SGA_ENABLE) || defined (STM32F4_SGA_ENABLE)
	/* F1 �� F4ϵ�� */
	while((PRINTF_UART->SR&0X40)==0)	/* ѭ������,ֱ��������� */  
#endif

#if defined(STM32L4_SGA_ENABLE)
	/* L4ϵ�� */
	while((PRINTF_UART->ISR&0X40)==0)	/* ѭ������,ֱ��������� */
#endif
    {
		/* ��ֹ�쳣��ʱ�˳� */
        if((usCnt++)>60000)
        {
            break;
        }          
    }   
#if	defined (STM32F1_SGA_ENABLE) || defined (STM32F4_SGA_ENABLE)
	/* F1 �� F4ϵ�� */
    PRINTF_UART->DR = (uint8_t) ch; 
#endif

#if defined (STM32L4_SGA_ENABLE)
	/* L4ϵ�� */
	PRINTF_UART->TDR = (uint8_t) ch; 

#endif
        
	return ch;
}
#endif





