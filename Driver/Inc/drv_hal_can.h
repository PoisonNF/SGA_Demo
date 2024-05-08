#ifndef __DRV_CAN_H_
#define __DRV_CAN_H_

#include "drv_hal_conf.h"

typedef struct
{
    CAN_HandleTypeDef   tCANHandle;             /* CAN句柄 */
    CAN_TxHeaderTypeDef tCANTxHeader;           /* CAN发送句柄 */
    CAN_RxHeaderTypeDef tCANRxHeader;           /* CAN接收句柄 */
    tagGPIO_T           tGPIO[2];               /* GPIO配置 */

    uint8_t             ucCANFilterNum;         /* 配置的过滤器个数，范围[1-14] */
    CAN_FilterTypeDef   tCANFilter[14];         /* CAN过滤器配置句柄，单CAN最多配置14个过滤器 */

    uint8_t				ucTxPriority;		    /* 发送中断优先级，0-15 */
	uint8_t 			ucTxSubPriority;	    /* 发送中断子优先级，0-15 */

    uint8_t				ucRxPriority;		    /* 接收中断优先级，0-15 */
	uint8_t 			ucRxSubPriority;	    /* 接收中断子优先级，0-15 */
}tagCAN_T;

/* 中断回调函数 */
/*
    //CAN通信-发送完成回调函数
    void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan);
    void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan);
    void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan);
    //CAN通信-发送取消回调函数
    void HAL_CAN_TxMailbox0AbortCallback(CAN_HandleTypeDef *hcan);
    void HAL_CAN_TxMailbox1AbortCallback(CAN_HandleTypeDef *hcan);
    void HAL_CAN_TxMailbox2AbortCallback(CAN_HandleTypeDef *hcan);

    //CAN通信-FIFO0接收新消息回调函数
    void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
    //CAN通信-FIFO0接收满回调函数
    void HAL_CAN_RxFifo0FullCallback(CAN_HandleTypeDef *hcan);
    //CAN通信-FIFO1接收新消息回调函数
    void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan);
    //CAN通信-FIFO1接收满回调函数
    void HAL_CAN_RxFifo1FullCallback(CAN_HandleTypeDef *hcan);

    //CAN通信-休眠回调函数
    void HAL_CAN_SleepCallback(CAN_HandleTypeDef *hcan);
    //CAN通信-唤醒回调函数
    void HAL_CAN_WakeUpFromRxMsgCallback(CAN_HandleTypeDef *hcan);
    //CAN通信-错误回调函数
    void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan);
*/

void Drv_CAN_ModeConfig(tagCAN_T *_tCAN,uint32_t _ulMode);
void Drv_CAN_TxIDConfig(tagCAN_T *_tCAN,uint32_t _ulID);
uint8_t Drv_CAN_SendMsg(tagCAN_T *_tCAN,uint8_t *_ucpMsg,uint8_t _ucLen);
uint8_t Drv_CAN_ReceMsg(tagCAN_T *_tCAN,uint8_t *_ucpMsg,uint32_t _ulRxFifo);
void Drv_CAN_Init(tagCAN_T *_tCAN);
void Drv_CAN_ITInit(tagCAN_T *_tCAN);
void Drv_CAN_IRQHandler(tagCAN_T *_tCAN);

#endif

