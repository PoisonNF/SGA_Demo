#ifndef __DRV_CAN_H_
#define __DRV_CAN_H_

#include "drv_hal_conf.h"

typedef struct
{
    CAN_HandleTypeDef tCANHandle;           /* CAN句柄 */
    CAN_TxHeaderTypeDef tCANTxHeader;       /* CAN发送句柄 */
    CAN_RxHeaderTypeDef tCANRxHeader;       /* CAN接收句柄 */
    CAN_FilterTypeDef tCANFilter;           /* CAN过滤器配置句柄 */
    tagGPIO_T tGPIO[2];                     /* GPIO配置 */

}tagCAN_T;

void Drv_CAN_ModeConfig(tagCAN_T *_tCAN,uint32_t ulMode);
void Drv_CAN_TxIDConfig(tagCAN_T *_tCAN,uint32_t ulID);
uint8_t Drv_CAN_SendMsg(tagCAN_T *_tCAN,uint8_t *ucpMsg,uint8_t ucLen);
uint8_t Drv_CAN_ReceMsg(tagCAN_T *_tCAN,uint8_t *ucpMsg);
void Drv_CAN_Init(tagCAN_T *_tCAN);

#endif
