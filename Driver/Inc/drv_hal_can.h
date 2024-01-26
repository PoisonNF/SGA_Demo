#ifndef __DRV_CAN_H_
#define __DRV_CAN_H_

#include "drv_hal_conf.h"

typedef struct
{
    CAN_HandleTypeDef tCANHandle;           /* CAN¾ä±ú */
    CAN_TxHeaderTypeDef tCANTxHeader;       /* CAN·¢ËÍ¾ä±ú */
    CAN_RxHeaderTypeDef tCANRxHeader;       /* CAN½ÓÊÕ¾ä±ú */
    CAN_FilterTypeDef tCANFilter;           /* CAN¹ýÂËÆ÷ÅäÖÃ¾ä±ú */
    tagGPIO_T tGPIO[2];                     /* GPIOÅäÖÃ */

}tagCAN_T;

void Drv_CAN_ModeConfig(tagCAN_T *_tCAN,uint32_t _ulMode);
void Drv_CAN_TxIDConfig(tagCAN_T *_tCAN,uint32_t _ulID);
uint8_t Drv_CAN_SendMsg(tagCAN_T *_tCAN,uint8_t *_ucpMsg,uint8_t _ucLen);
uint8_t Drv_CAN_ReceMsg(tagCAN_T *_tCAN,uint8_t *_ucpMsg);
void Drv_CAN_Init(tagCAN_T *_tCAN);

#endif
