/****************************************************************************

* Sigma�Ŷ�

* �ļ���: drv_hal_can.c

* ���ݼ�����CAN�����ļ�

* �ļ���ʷ��

* �汾��	����		����		˵��
*  3.2  2024-05-08	  �����	�Ż����ͺͽ����߼����������ö����������ʹ�ö��FIFO

*  3.1  2024-03-20	  �����	����CAN�ж�ģʽ��ʼ������

*    	2024-02-28	  �����	����CAN�ж���غ���

*  3.0 	2024-01-26	  �����	����STM32F4ϵ��

        2024-01-02	  �����	�޸��βη��Ϲ淶

*  2.9 	2023-12-27	  �����	�������ļ�

****************************************************************************/
#include "drv_hal_conf.h"

#ifdef DRV_HAL_CAN_ENABLE

/**
 * @brief CANʱ��ʹ�ܺ���
 * @param _tCAN-CAN�ṹ��ָ��
 * @retval Null
*/
static inline void S_CAN_CLKEnable(tagCAN_T *_tCAN)
{
    /* ʹ��CANʱ�� */
#ifdef STM32F1_SGA_ENABLE
    __HAL_RCC_CAN1_CLK_ENABLE();
#endif

#ifdef STM32F4_SGA_ENABLE
    if(_tCAN->tCANHandle.Instance == CAN1)
        __HAL_RCC_CAN1_CLK_ENABLE();
    else if(_tCAN->tCANHandle.Instance == CAN2)
        __HAL_RCC_CAN2_CLK_ENABLE();
#endif
}

/**
 * @brief CAN GPIO���ú���
 * @param _tCAN-CAN�ṹ��ָ��
 * @retval Null
*/
static void S_CAN_GPIOConfig(tagCAN_T *_tCAN)
{
#ifdef STM32F1_SGA_ENABLE
    /* ��������ģʽʱ�� */
	__HAL_RCC_AFIO_CLK_ENABLE();

    /* ���ݶ˿ںŽ��и������� */
    if(_tCAN->tGPIO[0].tGPIOPort == GPIOA && _tCAN->tGPIO[1].tGPIOPort == GPIOA)
        __HAL_AFIO_REMAP_CAN1_1();

    else if(_tCAN->tGPIO[0].tGPIOPort == GPIOB && _tCAN->tGPIO[1].tGPIOPort == GPIOB)
        __HAL_AFIO_REMAP_CAN1_2();

    else if(_tCAN->tGPIO[0].tGPIOPort == GPIOC && _tCAN->tGPIO[1].tGPIOPort == GPIOC)
        __HAL_AFIO_REMAP_CAN1_3();
#endif

#ifdef STM32F4_SGA_ENABLE
    /* ����CAN�Ŷ�GPIO���и������� */
    if(_tCAN->tCANHandle.Instance == CAN1)
    {
        _tCAN->tGPIO[0].tGPIOInit.Alternate = GPIO_AF9_CAN1;
        _tCAN->tGPIO[1].tGPIOInit.Alternate = GPIO_AF9_CAN1;
    }
    if(_tCAN->tCANHandle.Instance == CAN2)
    {
        _tCAN->tGPIO[0].tGPIOInit.Alternate = GPIO_AF9_CAN2;
        _tCAN->tGPIO[1].tGPIOInit.Alternate = GPIO_AF9_CAN2;
    }
#endif

	Drv_GPIO_Init(_tCAN->tGPIO, 2);
}

/**
 * @brief CAN�ж����ú���
 * @param _tCAN-CAN�ṹ��ָ��
 * @retval Null 
*/
static void S_CAN_NVICConfig(tagCAN_T *_tCAN)
{
    if(_tCAN->tCANHandle.Instance == CAN1)
    {
        /* CAN1�����ж����� */
        HAL_NVIC_SetPriority(CAN1_TX_IRQn,_tCAN->ucTxPriority,_tCAN->ucTxSubPriority);
        HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);

        /* CAN1����0�ж����� */
        HAL_NVIC_SetPriority(CAN1_RX0_IRQn,_tCAN->ucTxPriority,_tCAN->ucTxSubPriority);
        HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);

        /* CAN1����1�ж����� */
        HAL_NVIC_SetPriority(CAN1_RX1_IRQn,_tCAN->ucTxPriority,_tCAN->ucTxSubPriority);
        HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);

        /* CAN1����״̬�ı��ж����� */
        HAL_NVIC_SetPriority(CAN1_SCE_IRQn,0,0);
        HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);
    }

#ifdef STM32F4_SGA_ENABLE
    else if(_tCAN->tCANHandle.Instance == CAN2)
    {
        /* CAN2�����ж����� */
        HAL_NVIC_SetPriority(CAN2_TX_IRQn,_tCAN->ucTxPriority,_tCAN->ucTxSubPriority);
        HAL_NVIC_EnableIRQ(CAN2_TX_IRQn);

        /* CAN2����0�ж����� */
        HAL_NVIC_SetPriority(CAN2_RX0_IRQn,_tCAN->ucTxPriority,_tCAN->ucTxSubPriority);
        HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);

        /* CAN2����1�ж����� */
        HAL_NVIC_SetPriority(CAN2_RX1_IRQn,_tCAN->ucTxPriority,_tCAN->ucTxSubPriority);
        HAL_NVIC_EnableIRQ(CAN2_RX1_IRQn);

        /* CAN2����״̬�ı��ж����� */
        HAL_NVIC_SetPriority(CAN2_SCE_IRQn,0,0);
        HAL_NVIC_EnableIRQ(CAN2_SCE_IRQn);
    }
#endif

}

/**
 * @brief CAN��������
 * @param _tCAN-CAN�ṹ��ָ��
 * @retval Null
*/
static void S_CAN_ParamConfig(tagCAN_T *_tCAN)
{
    /* CAN��ʼ�� */
    if(HAL_CAN_Init(&_tCAN->tCANHandle) != HAL_OK)
        Drv_HAL_Error(__FILE__, __LINE__);

    /* CAN���������� */
    for(uint8_t index = 0; index < _tCAN->ucCANFilterNum; index++)
    {
        /* ���ù����� */
        HAL_CAN_ConfigFilter(&_tCAN->tCANHandle,&_tCAN->tCANFilter[index]);

        /* ����ѡ����FIFO�ſ�ʼ��Ӧ���ж�֪ͨ */
        if(_tCAN->tCANFilter[index].FilterFIFOAssignment == CAN_RX_FIFO0)
        {
            if(HAL_CAN_ActivateNotification(&_tCAN->tCANHandle, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
                Drv_HAL_Error(__FILE__,__LINE__);
            if(HAL_CAN_ActivateNotification(&_tCAN->tCANHandle, CAN_IT_RX_FIFO0_FULL) != HAL_OK)
                Drv_HAL_Error(__FILE__,__LINE__);
        }
        else
        {
            if(HAL_CAN_ActivateNotification(&_tCAN->tCANHandle, CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK)
                Drv_HAL_Error(__FILE__,__LINE__);
            if(HAL_CAN_ActivateNotification(&_tCAN->tCANHandle, CAN_IT_RX_FIFO1_FULL) != HAL_OK)
                Drv_HAL_Error(__FILE__,__LINE__);
        }
    }
    
    /* ����CAN��Χ�豸 */
    if(HAL_CAN_Start(&_tCAN->tCANHandle) != HAL_OK)
        Drv_HAL_Error(__FILE__,__LINE__);

    /* ����CAN�����ж�֪ͨ */
    if(HAL_CAN_ActivateNotification(&_tCAN->tCANHandle,CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK)
        Drv_HAL_Error(__FILE__,__LINE__);

    /* ����CAN�����ж�֪ͨ */
    if(HAL_CAN_ActivateNotification(&_tCAN->tCANHandle, CAN_IT_ERROR) != HAL_OK)
        Drv_HAL_Error(__FILE__,__LINE__);
}

/**
 * @brief CAN����ģʽ
 * @param _tCAN-CAN�ṹ��ָ��
 * @param _ulMode-ģʽѡ��
 *        �����������Ϊ����ֵ: 
 *        CAN_MODE_NORMAL           ����ģʽ
 *        CAN_MODE_LOOPBACK         �ػ�ģʽ
 *        CAN_MODE_SILENT           ��Ĭģʽ
 *        CAN_MODE_SILENT_LOOPBACK  ��Ĭ�ػ�ģʽ
 * @retval Null
*/
void Drv_CAN_ModeConfig(tagCAN_T *_tCAN,uint32_t _ulMode)
{
    _tCAN->tCANHandle.Init.Mode = _ulMode;

    /* ���¶�CAN��ʼ�� */
    Drv_CAN_Init(_tCAN);
}

/**
 * @brief CAN����TX��ʶ��ID����
 * @param _tCAN-CAN�ṹ��ָ��
 * @param _ulID-ָ����ID��
 * @retval Null
*/
void Drv_CAN_TxIDConfig(tagCAN_T *_tCAN,uint32_t _ulID)
{
    /* �ж��ǲ��Ǳ�׼��ʶ�� */
    if(_tCAN->tCANTxHeader.IDE == CAN_ID_STD)
        _tCAN->tCANTxHeader.StdId = _ulID;
    else
        _tCAN->tCANTxHeader.ExtId = _ulID;
}

/**
 * @brief CAN��������
 * @param _tCAN-CAN�ṹ��ָ��
 * @param _ucpMsg-��������ָ�룬���Ϊ8�ֽ�
 * @param _ucLen-�������ݳ��ȣ����Ϊ8
 * @retval uint8_t 0�ɹ� 1ʧ��
*/
uint8_t Drv_CAN_SendMsg(tagCAN_T *_tCAN,uint8_t *_ucpMsg,uint8_t _ucLen)
{
    uint32_t ulTXMailBox = CAN_TX_MAILBOX0;

    _tCAN->tCANTxHeader.DLC = _ucLen;    /* ���ͳ������� */
	
	/* Check Tx Mailbox 0 status */
    if((_tCAN->tCANHandle.Instance->TSR & CAN_TSR_TME0) != 0U)
    {
        ulTXMailBox = CAN_TX_MAILBOX0;
    }

    /* Check Tx Mailbox 1 status */
    else if((_tCAN->tCANHandle.Instance->TSR & CAN_TSR_TME1) != 0U)
    {
        ulTXMailBox = CAN_TX_MAILBOX1;
    }

    /* Check Tx Mailbox 2 status */
    else if((_tCAN->tCANHandle.Instance->TSR & CAN_TSR_TME2) != 0U)
    {
        ulTXMailBox = CAN_TX_MAILBOX2;
    }

	/* �����ݴ洢������������ */
    if(HAL_CAN_AddTxMessage(&_tCAN->tCANHandle,&_tCAN->tCANTxHeader,_ucpMsg,&ulTXMailBox))
    {
        return 1;
    }

    return 0;
}

/**
 * @brief CAN��������
 * @param _tCAN-CAN�ṹ��ָ��
 * @param _ucpMsg-��������ָ��
 * @param _ulRxFifo-�������ݵ�FIFO�ţ���ѡCAN_FILTER_FIFO0��CAN_FILTER_FIFO1
 * @retval uint8_t �������ݳ���
*/
uint8_t Drv_CAN_ReceMsg(tagCAN_T *_tCAN,uint8_t *_ucpMsg,uint32_t _ulRxFifo)
{
    if(HAL_CAN_GetRxMessage(&_tCAN->tCANHandle,_ulRxFifo,&_tCAN->tCANRxHeader,_ucpMsg) != HAL_OK)
    {
        return 0;
    }

    return _tCAN->tCANRxHeader.DLC;
}

/**
 * @brief CAN��ʼ��
 * @param _tCAN-CAN�ṹ��ָ��
 * @retval Null
*/
void Drv_CAN_Init(tagCAN_T *_tCAN)
{
	S_CAN_CLKEnable(_tCAN);
	S_CAN_GPIOConfig(_tCAN);
	S_CAN_ParamConfig(_tCAN);
}

/**
 * @brief CAN�ж�ģʽ��ʼ��
 * @param _tCAN-CAN�ṹ��ָ��
 * @retval Null
*/
void Drv_CAN_ITInit(tagCAN_T *_tCAN)
{
	S_CAN_CLKEnable(_tCAN);
	S_CAN_GPIOConfig(_tCAN);
    S_CAN_NVICConfig(_tCAN);
	S_CAN_ParamConfig(_tCAN);
}

/**
 * @brief CAN�ж�ñ�Ӻ���(����CAN�жϴ�������)
 * @note ������void CAN1_TX_IRQHandler(void)�����е��ã�����ʵ��������и���
 * @param _tCAN-CAN�ṹ��ָ��
 * @retval Null
*/
void Drv_CAN_IRQHandler(tagCAN_T *_tCAN)
{
	HAL_CAN_IRQHandler(&_tCAN->tCANHandle);
}

#endif

