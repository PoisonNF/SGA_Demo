/****************************************************************************

* Sigma�Ŷ�

* �ļ���: drv_hal_pwr.c

* ���ݼ�����PWR��Դ���������ļ�

* �ļ���ʷ��

* �汾��	����		����		˵��
*  3.3   2024-06-22	  �����	����PVD��Դ�������

*  2.5   2023-06-03	  �����	�������ļ�

****************************************************************************/
#include "drv_hal_conf.h"

#ifdef DRV_HAL_PWR_ENABLE

/**
 * @brief �ָ�HSEʱ�Ӻ���
 * @param Null
 * @note ����ֹͣģʽ������ʱ���ָ���HSE����ʱ��
 * @retval Null 
*/
static void S_PWR_RestoreHSEClk(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    uint32_t pFLatency = 0;

    /* ���õ�Դ����ʱ�� */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* �����ڲ�RCC�Ĵ�����ȡ�������� */
    HAL_RCC_GetOscConfig(&RCC_OscInitStruct);

    /* ��ֹͣģʽ���Ѻ���������ϵͳʱ��:
    ����HSE��PLL */
    RCC_OscInitStruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState        = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState    = RCC_PLL_ON;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Drv_HAL_Error(__FILE__, __LINE__);
    }

    /* �����ڲ�RCC�Ĵ�����ȡʱ������ */
    HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &pFLatency);

    /* ѡ�� PLL ��Ϊϵͳʱ��Դ, ������ HCLK��PCLK1 �� PCLK2ʱ�ӷ�Ƶϵ�� */
    RCC_ClkInitStruct.ClockType     = RCC_CLOCKTYPE_SYSCLK;
    RCC_ClkInitStruct.SYSCLKSource  = RCC_SYSCLKSOURCE_PLLCLK;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, pFLatency) != HAL_OK)
    {
        Drv_HAL_Error(__FILE__, __LINE__);
    }

    /* ����ʱ�ӼĴ�����ֵ����SystemCoreClock���� */
    SystemCoreClockUpdate();
}

/**
 * @brief ����˯��ģʽ����
 * @param Null
 * @note ˯��ʱ:
 *       �ر��ں�ʱ�ӣ��ں�ֹͣ���������������У�������ϱ���Ϊ����ִ���µĴ��롣
 *       ���Ѻ�:
 *       ���������жϻ��ѣ��Ƚ����жϣ��˳��жϷ������󣬽���ִ��WFIָ���ĳ���
 *       �����¼����ѣ�ֱ�ӽ���ִ��WFE��ĳ���
 * @retval Null 
*/
void Drv_PWR_EnterSleepMode(void)
{
    /* ��ͣ�δ�ʱ�ӣ���ֹͨ���δ�ʱ���жϻ��� */
    HAL_SuspendTick();

    /* ����˯��ģʽ�����뽫�ڴ˴���ͣ���ȴ����� */
    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON,PWR_SLEEPENTRY_WFI);

    /* �ӵ͹���ģʽ���Ѻ�������ѱ�־λ */
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

    /* �����Ѻ󣬻ָ��δ�ʱ�� */
    HAL_ResumeTick();
}

/**
 * @brief ����ֹͣģʽ����
 * @param Null
 * @note ֹͣʱ:
 *       �ر��ں�ʱ�ӣ��ں�ֹͣ��Ƭ������Ҳֹͣ��
 *       ���Ѻ�:
 *       STM32��ʹ��HSI��Ϊϵͳʱ�ӡ�
 *       ����EXTI�жϻ��ѣ��Ƚ����жϣ��˳��жϷ������󣬽���ִ��WFIָ���ĳ���
 *       �����¼����ѣ�ֱ�ӽ���ִ��WFE��ĳ���
 * @retval Null 
*/
void Drv_PWR_EnterStopMode(void)
{
    /* ��ͣ�δ�ʱ�ӣ���ֹͨ���δ�ʱ���жϻ��� */
    HAL_SuspendTick();

    /* ����ֹͣģʽ�����뽫�ڴ˴���ͣ���ȴ�����  */
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON,PWR_STOPENTRY_WFI);

    /* �ӵ͹���ģʽ���Ѻ�������ѱ�־λ */
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

    /* ����HSE����ʱ�� */
    S_PWR_RestoreHSEClk();

    /* �����Ѻ󣬻ָ��δ�ʱ�� */
    HAL_ResumeTick();
}

/**
 * @brief �������ģʽ����
 * @param Null
 * @note ����ʱ:
 *       ȫ���رա�����λ���š�RTC_AF1���ż�WKUP���ţ�����I/O�ھ������ڸ���̬��
 *       ���Ѻ�:
 *       �൱��оƬ��λ���ڳ������Ϊ��ͷ��ʼִ�д��롣
 * @retval Null 
*/
void Drv_PWR_EnterStandByMode(void)
{
    /* ʹ��WKUP���ŵĻ��ѹ��� ��ʹ��PA0�������ش������� */
    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);

    /* ��ͣ�δ�ʱ�ӣ���ֹͨ���δ�ʱ���жϻ��� */
    HAL_SuspendTick();

    /* �������ģʽ�����Ѻ�оƬ���Ḵλ */
    HAL_PWR_EnterSTANDBYMode();
}

/**
 * @brief ��ʼ��PVD��Դ��⹦��
 * @param _ulPVDLevel ���ĵ�ѹ�ȼ�
 *         �ò�������Ϊ���п�ѡֵ֮һ
 *           @arg PWR_PVDLEVEL_0    1.9V
 *           @arg PWR_PVDLEVEL_1    2.1V
 *           @arg PWR_PVDLEVEL_2    2.3V
 *           @arg PWR_PVDLEVEL_3    2.5V
 *           @arg PWR_PVDLEVEL_4    2.7V
 *           @arg PWR_PVDLEVEL_5    2.9V
 *           @arg PWR_PVDLEVEL_6    3.1V
 *           @arg PWR_PVDLEVEL_7    �ⲿģ���ѹ����(PB7)    
 * @note 1.��Ҫ��task_irq.c��void PVD_IRQHandler(void)�е���void Drv_PVD_IRQHandler(void)
         2.��Ҫ��task_irq.c�е�void HAL_PWR_PVDCallback(void)������д���Դ����ʱ������
 * @retval Null 
*/
void Drv_PWR_PVD_Init(uint32_t _ulPVDLevel)
{
    /* ����PWRʱ�� */
    __HAL_RCC_PWR_CLK_ENABLE();
 
    /* ����PVD�ж� */
    HAL_NVIC_SetPriority(PVD_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(PVD_IRQn);
 
    /* ����PVD����ѹ�ȼ��Լ��������� */
    PWR_PVDTypeDef tConfigPVD;
    tConfigPVD.PVDLevel = _ulPVDLevel;
    tConfigPVD.Mode = PWR_PVD_MODE_IT_RISING;
    HAL_PWR_ConfigPVD(&tConfigPVD);

    /* ����PVD��� */
    HAL_PWR_EnablePVD();
}

/**
 * @brief PVD�ж�ñ�Ӻ���(����PVD�жϴ�������)
 * @note ��void PVD_IRQHandler(void)�����е���
 * @param Null
 * @retval Null
*/
void Drv_PVD_IRQHandler(void)
{
    HAL_PWR_PVD_IRQHandler();
}

#endif
