/****************************************************************************

* Sigma团队

* 文件名: drv_hal_can.c

* 内容简述：CAN驱动文件

* 文件历史：

* 版本号	日期		作者		说明

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
    __HAL_RCC_CAN1_CLK_ENABLE();
}

/**
 * @brief CAN GPIO配置函数
 * @param _tCAN-CAN结构体指针
 * @retval Null
*/
static void S_CAN_GPIOConfig(tagCAN_T *_tCAN)
{
    /* 开启复用模式时钟 */
	__HAL_RCC_AFIO_CLK_ENABLE();

    /* 根据端口号进行复用配置 */
    if(_tCAN->tGPIO[0].tGPIOPort == GPIOA && _tCAN->tGPIO[1].tGPIOPort == GPIOA)
        __HAL_AFIO_REMAP_CAN1_1();

    else if(_tCAN->tGPIO[0].tGPIOPort == GPIOB && _tCAN->tGPIO[1].tGPIOPort == GPIOB)
        __HAL_AFIO_REMAP_CAN1_2();

    else if(_tCAN->tGPIO[0].tGPIOPort == GPIOC && _tCAN->tGPIO[1].tGPIOPort == GPIOC)
        __HAL_AFIO_REMAP_CAN1_3();

	Drv_GPIO_Init(_tCAN->tGPIO, 2);
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

    /* 激活RX通知 */
    if(HAL_CAN_ActivateNotification(&_tCAN->tCANHandle, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
        Drv_HAL_Error(__FILE__,__LINE__);
}

/**
 * @brief CAN设置模式
 * @param _tCAN-CAN结构体指针
 * @param ulMode-模式选择
 *        这个参数可以为以下值: 
 *        CAN_MODE_NORMAL           正常模式
 *        CAN_MODE_LOOPBACK         回环模式
 *        CAN_MODE_SILENT           静默模式
 *        CAN_MODE_SILENT_LOOPBACK  静默回环模式
 * @retval Null
*/
void Drv_CAN_ModeConfig(tagCAN_T *_tCAN,uint32_t ulMode)
{
    _tCAN->tCANHandle.Init.Mode = ulMode;

    /* 重新对CAN初始化 */
    Drv_CAN_Init(_tCAN);
}

/**
 * @brief CAN设置TX标识符ID函数
 * @param _tCAN-CAN结构体指针
 * @param ulMode-模式选择
 * @retval Null
*/
void Drv_CAN_TxIDConfig(tagCAN_T *_tCAN,uint32_t ulID)
{
    /* 判断是不是标准标识符 */
    if(_tCAN->tCANTxHeader.IDE == CAN_ID_STD)
        _tCAN->tCANTxHeader.StdId = ulID;
    else
        _tCAN->tCANTxHeader.ExtId = ulID;
}

/**
 * @brief CAN发送数据
 * @param _tCAN-CAN结构体指针
 * @param ucpMsg-发送数据指针，最大为8字节
 * @param ucLen-发送数据长度，最大为8
 * @retval uint8_t 0成功 1失败
*/
uint8_t Drv_CAN_SendMsg(tagCAN_T *_tCAN,uint8_t *ucpMsg,uint8_t ucLen)
{
    uint8_t index = 0;
    uint8_t ucTemp[8];

    _tCAN->tCANTxHeader.DLC = ucLen;    /* 发送长度设置 */

    /* 数据拷贝 */
    for(index = 0;index < ucLen;index ++)
    {
        ucTemp[index] = ucpMsg[index];
    }

    /* 将数据存储到发送邮箱中 */
    if(HAL_CAN_AddTxMessage(&_tCAN->tCANHandle,&_tCAN->tCANTxHeader,ucTemp,(uint32_t *)CAN_TX_MAILBOX1))
    {
        return 1;
    }

    return 0;
}

/**
 * @brief CAN接收数据
 * @param _tCAN-CAN结构体指针
 * @param ucpMsg-接收数据指针
 * @retval uint8_t 接收数据长度
*/
uint8_t Drv_CAN_ReceMsg(tagCAN_T *_tCAN,uint8_t *ucpMsg)
{
    uint8_t index = 0;
    uint8_t ucTemp[8];

    if(HAL_CAN_GetRxFifoFillLevel(&_tCAN->tCANHandle,_tCAN->tCANFilter.FilterFIFOAssignment) != 1)
    {
        return 0xf1;
    }

    if(HAL_CAN_GetRxMessage(&_tCAN->tCANHandle,_tCAN->tCANFilter.FilterFIFOAssignment,&_tCAN->tCANRxHeader,ucTemp) != HAL_OK)
    {
        return 0xf2;
    }

    for(index = 0;index < _tCAN->tCANRxHeader.DLC; index++)
    {
        ucpMsg[index] = ucTemp[index];
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

#endif

