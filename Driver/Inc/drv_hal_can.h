#ifndef __DRV_CAN_H_
#define __DRV_CAN_H_

#include "drv_hal_conf.h"

typedef struct
{
    CAN_HandleTypeDef   tCANHandle;             /* CAN��� */
    CAN_TxHeaderTypeDef tCANTxHeader;           /* CAN���;�� */
    CAN_RxHeaderTypeDef tCANRxHeader;           /* CAN���վ�� */
    tagGPIO_T           tGPIO[2];               /* GPIO���� */

    uint8_t             ucCANFilterNum;         /* ���õĹ�������������Χ[1-14] */
    CAN_FilterTypeDef   tCANFilter[14];         /* CAN���������þ������CAN�������14�������� */

    uint8_t				ucTxPriority;		    /* �����ж����ȼ���0-15 */
	uint8_t 			ucTxSubPriority;	    /* �����ж������ȼ���0-15 */

    uint8_t				ucRxPriority;		    /* �����ж����ȼ���0-15 */
	uint8_t 			ucRxSubPriority;	    /* �����ж������ȼ���0-15 */
}tagCAN_T;

/* �жϻص����� */
/*
    //CANͨ��-������ɻص�����
    void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan);
    void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan);
    void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan);
    //CANͨ��-����ȡ���ص�����
    void HAL_CAN_TxMailbox0AbortCallback(CAN_HandleTypeDef *hcan);
    void HAL_CAN_TxMailbox1AbortCallback(CAN_HandleTypeDef *hcan);
    void HAL_CAN_TxMailbox2AbortCallback(CAN_HandleTypeDef *hcan);

    //CANͨ��-FIFO0��������Ϣ�ص�����
    void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
    //CANͨ��-FIFO0�������ص�����
    void HAL_CAN_RxFifo0FullCallback(CAN_HandleTypeDef *hcan);
    //CANͨ��-FIFO1��������Ϣ�ص�����
    void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan);
    //CANͨ��-FIFO1�������ص�����
    void HAL_CAN_RxFifo1FullCallback(CAN_HandleTypeDef *hcan);

    //CANͨ��-���߻ص�����
    void HAL_CAN_SleepCallback(CAN_HandleTypeDef *hcan);
    //CANͨ��-���ѻص�����
    void HAL_CAN_WakeUpFromRxMsgCallback(CAN_HandleTypeDef *hcan);
    //CANͨ��-����ص�����
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

