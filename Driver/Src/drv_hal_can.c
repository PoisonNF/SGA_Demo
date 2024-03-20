/****************************************************************************

* Sigma团队

* 文件名: drv_hal_can.c

* 内容简述：CAN驱动文件

* 文件历史：

* 版本号	日期		作者		说明
*    	2024-03-20	  鲍程璐	增加CAN中断模式初始化函数

*    	2024-02-28	  鲍程璐	增加CAN中断相关函数

*  3.0 	2024-01-26	  鲍程璐	适配STM32F4系列

        2024-01-02	  鲍程璐	修改形参符合规范

*  2.9 	2023-12-27	  鲍程璐	创建该文件

****************************************************************************/
#include "drv_hal_conf.h"

#ifdef DRV_HAL_CAN_ENABLE

/**
 * @brief CAN时钟使能函数
 * @param _tCAN-CAN结构体指针
 * @retval Null
*/
static inline void S_CAN_CLKEnable(tagCAN_T *_tCAN)
{
    /* 使能CAN时钟 */
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
 * @brief CAN GPIO配置函数
 * @param _tCAN-CAN结构体指针
 * @retval Null
*/
static void S_CAN_GPIOConfig(tagCAN_T *_tCAN)
{
#ifdef STM32F1_SGA_ENABLE
    /* 开启复用模式时钟 */
	__HAL_RCC_AFIO_CLK_ENABLE();

    /* 根据端口号进行复用配置 */
    if(_tCAN->tGPIO[0].tGPIOPort == GPIOA && _tCAN->tGPIO[1].tGPIOPort == GPIOA)
        __HAL_AFIO_REMAP_CAN1_1();

    else if(_tCAN->tGPIO[0].tGPIOPort == GPIOB && _tCAN->tGPIO[1].tGPIOPort == GPIOB)
        __HAL_AFIO_REMAP_CAN1_2();

    else if(_tCAN->tGPIO[0].tGPIOPort == GPIOC && _tCAN->tGPIO[1].tGPIOPort == GPIOC)
        __HAL_AFIO_REMAP_CAN1_3();
#endif

#ifdef STM32F4_SGA_ENABLE
    /* 根据CAN号对GPIO进行复用配置 */
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
 * @brief CAN中断配置函数
 * @param _tCAN-CAN结构体指针
 * @retval Null 
*/
static void S_CAN_NVICConfig(tagCAN_T *_tCAN)
{
    if(_tCAN->tCANHandle.Instance == CAN1)
    {
        /* CAN1发送中断配置 */
        HAL_NVIC_SetPriority(CAN1_TX_IRQn,_tCAN->ucTxPriority,_tCAN->ucTxSubPriority);
        HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);

        /* CAN1接收0中断配置 */
        HAL_NVIC_SetPriority(CAN1_RX0_IRQn,_tCAN->ucTxPriority,_tCAN->ucTxSubPriority);
        HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);

        /* CAN1接收1中断配置 */
        HAL_NVIC_SetPriority(CAN1_RX1_IRQn,_tCAN->ucTxPriority,_tCAN->ucTxSubPriority);
        HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);

        /* CAN1总线状态改变中断配置 */
        HAL_NVIC_SetPriority(CAN1_SCE_IRQn,0,0);
        HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);
    }

#ifdef STM32F4_SGA_ENABLE
    else if(_tCAN->tCANHandle.Instance == CAN2)
    {
        /* CAN2发送中断配置 */
        HAL_NVIC_SetPriority(CAN2_TX_IRQn,_tCAN->ucTxPriority,_tCAN->ucTxSubPriority);
        HAL_NVIC_EnableIRQ(CAN2_TX_IRQn);

        /* CAN2接收0中断配置 */
        HAL_NVIC_SetPriority(CAN2_RX0_IRQn,_tCAN->ucTxPriority,_tCAN->ucTxSubPriority);
        HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);

        /* CAN2接收1中断配置 */
        HAL_NVIC_SetPriority(CAN2_RX1_IRQn,_tCAN->ucTxPriority,_tCAN->ucTxSubPriority);
        HAL_NVIC_EnableIRQ(CAN2_RX1_IRQn);

        /* CAN2总线状态改变中断配置 */
        HAL_NVIC_SetPriority(CAN2_SCE_IRQn,0,0);
        HAL_NVIC_EnableIRQ(CAN2_SCE_IRQn);
    }
#endif

}

/**
 * @brief CAN参数设置
 * @param _tCAN-CAN结构体指针
 * @retval Null
*/
static void S_CAN_ParamConfig(tagCAN_T *_tCAN)
{
    /* CAN初始化 */
    if(HAL_CAN_Init(&_tCAN->tCANHandle) != HAL_OK)
        Drv_HAL_Error(__FILE__, __LINE__);

    /* CAN过滤器配置 */
    if(HAL_CAN_ConfigFilter(&_tCAN->tCANHandle,&_tCAN->tCANFilter) != HAL_OK)
        Drv_HAL_Error(__FILE__, __LINE__);
    
    /* 启动CAN外围设备 */
    if(HAL_CAN_Start(&_tCAN->tCANHandle) != HAL_OK)
        Drv_HAL_Error(__FILE__,__LINE__);

    /* 激活CAN发送中断通知 */
    if(HAL_CAN_ActivateNotification(&_tCAN->tCANHandle,CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK)
        Drv_HAL_Error(__FILE__,__LINE__);

    /* 根据FIFO选择 */
    if(_tCAN->tCANFilter.FilterFIFOAssignment == CAN_RX_FIFO0)
    {
        /* 激活FIFO0接收新消息通知 */
        if(HAL_CAN_ActivateNotification(&_tCAN->tCANHandle, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
            Drv_HAL_Error(__FILE__,__LINE__);
    }
    else
    {
        /* 激活FIFO1接收新消息通知 */
        if(HAL_CAN_ActivateNotification(&_tCAN->tCANHandle, CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK)
            Drv_HAL_Error(__FILE__,__LINE__);
    }

    /* 激活CAN错误中断通知 */
    if(HAL_CAN_ActivateNotification(&_tCAN->tCANHandle, CAN_IT_ERROR) != HAL_OK)
        Drv_HAL_Error(__FILE__,__LINE__);
}

/**
 * @brief CAN设置模式
 * @param _tCAN-CAN结构体指针
 * @param _ulMode-模式选择
 *        这个参数可以为以下值: 
 *        CAN_MODE_NORMAL           正常模式
 *        CAN_MODE_LOOPBACK         回环模式
 *        CAN_MODE_SILENT           静默模式
 *        CAN_MODE_SILENT_LOOPBACK  静默回环模式
 * @retval Null
*/
void Drv_CAN_ModeConfig(tagCAN_T *_tCAN,uint32_t _ulMode)
{
    _tCAN->tCANHandle.Init.Mode = _ulMode;

    /* 重新对CAN初始化 */
    Drv_CAN_Init(_tCAN);
}

/**
 * @brief CAN设置TX标识符ID函数
 * @param _tCAN-CAN结构体指针
 * @param _ulID-指定的ID号
 * @retval Null
*/
void Drv_CAN_TxIDConfig(tagCAN_T *_tCAN,uint32_t _ulID)
{
    /* 判断是不是标准标识符 */
    if(_tCAN->tCANTxHeader.IDE == CAN_ID_STD)
        _tCAN->tCANTxHeader.StdId = _ulID;
    else
        _tCAN->tCANTxHeader.ExtId = _ulID;
}

/**
 * @brief CAN发送数据
 * @param _tCAN-CAN结构体指针
 * @param _ucpMsg-发送数据指针，最大为8字节
 * @param _ucLen-发送数据长度，最大为8
 * @retval uint8_t 0成功 1失败
*/
uint8_t Drv_CAN_SendMsg(tagCAN_T *_tCAN,uint8_t *ucpMsg,uint8_t ucLen)
{
    uint8_t index = 0;
    uint8_t ucTemp[8];
    uint32_t ulTXMailBox;

    _tCAN->tCANTxHeader.DLC = ucLen;    /* 发送长度设置 */
    
    ulTXMailBox = CAN_TX_MAILBOX0;      /* 发送邮箱设置 */

    /* 数据拷贝 */
    for(index = 0;index < ucLen;index ++)
    {
        ucTemp[index] = ucpMsg[index];
    }

    /* 将数据存储到发送邮箱中 */
    if(HAL_CAN_AddTxMessage(&_tCAN->tCANHandle,&_tCAN->tCANTxHeader,ucTemp,&ulTXMailBox))
    {
        return 1;
    }

    return 0;
}

/**
 * @brief CAN接收数据
 * @param _tCAN-CAN结构体指针
 * @param _ucpMsg-接收数据指针
 * @retval uint8_t 接收数据长度
*/
uint8_t Drv_CAN_ReceMsg(tagCAN_T *_tCAN,uint8_t *_ucpMsg)
{
    uint8_t index = 0;
    uint8_t ucTemp[8];

    if(HAL_CAN_GetRxMessage(&_tCAN->tCANHandle,_tCAN->tCANFilter.FilterFIFOAssignment,&_tCAN->tCANRxHeader,ucTemp) != HAL_OK)
    {
        return 0;
    }

    for(index = 0;index < _tCAN->tCANRxHeader.DLC; index++)
    {
        _ucpMsg[index] = ucTemp[index];
    }

    return _tCAN->tCANRxHeader.DLC;
}

/**
 * @brief CAN初始化
 * @param _tCAN-CAN结构体指针
 * @retval Null
*/
void Drv_CAN_Init(tagCAN_T *_tCAN)
{
	S_CAN_CLKEnable(_tCAN);
	S_CAN_GPIOConfig(_tCAN);
	S_CAN_ParamConfig(_tCAN);
}

/**
 * @brief CAN中断模式初始化
 * @param _tCAN-CAN结构体指针
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
 * @brief CAN中断帽子函数(用于CAN中断处理函数中)
 * @note 在类似void CAN1_TX_IRQHandler(void)函数中调用，根据实际情况进行更改
 * @param _tCAN-CAN结构体指针
 * @retval Null
*/
void Drv_CAN_IRQHandler(tagCAN_T *_tCAN)
{
	HAL_CAN_IRQHandler(&_tCAN->tCANHandle);
}

#endif

