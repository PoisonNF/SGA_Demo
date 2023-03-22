#ifndef __DEV_PS2_H_
#define __DEV_PS2_H_

#include "drv_hal_conf.h"

#define DI  Drv_GPIO_Read(&_tPS2->tGPIO[0])     /* DI输入 */

#define DO_H Drv_GPIO_Set(&_tPS2->tGPIO[1])     /* 命令位高 */
#define DO_L Drv_GPIO_Reset(&_tPS2->tGPIO[1])   /* 命令位低 */

#define CS_H Drv_GPIO_Set(&_tPS2->tGPIO[2])     /* CS拉高 */
#define CS_L Drv_GPIO_Reset(&_tPS2->tGPIO[2])   /* CS拉低 */

#define CLK_H Drv_GPIO_Set(&_tPS2->tGPIO[3])    /* 时钟拉高 */
#define CLK_L Drv_GPIO_Reset(&_tPS2->tGPIO[3])  /* 时钟拉低 */

/* 按键对应 */
#define PSB_SELECT      1
#define PSB_L3          2
#define PSB_R3          3
#define PSB_START       4
#define PSB_PAD_UP      5
#define PSB_PAD_RIGHT   6
#define PSB_PAD_DOWN    7
#define PSB_PAD_LEFT    8
#define PSB_L2          9
#define PSB_R2          10
#define PSB_L1          11
#define PSB_R1          12
/* 颜色分辨 */
#define PSB_GREEN       13
#define PSB_RED         14
#define PSB_BLUE        15
#define PSB_PINK        16
/* 图形分辨 */
#define PSB_TRIANGLE    13
#define PSB_CIRCLE      14
#define PSB_CROSS       15
#define PSB_SQUARE      16

/* 摇杆存储位置 */
#define PSS_RX 5                /* 右摇杆X轴数据 */
#define PSS_RY 6                /* 右摇杆Y轴数据 */
#define PSS_LX 7                /* 左摇杆X轴数据 */
#define PSS_LY 8                /* 左摇杆Y轴数据 */

#define DELAY_TIME  Drv_Delay_Us(5); 

typedef struct 
{
	tagGPIO_T tGPIO[4];     /* 按照[0]DI/DAT [1]DO/CMD [2]CS [3]CLK顺序 */
}tagPS2_T;

void Dev_PS2_ReadData(tagPS2_T *_tPS2);
void Dev_PS2_ClearData(void);
uint8_t Dev_PS2_RedLight(tagPS2_T *_tPS2);
uint8_t Dev_PS2_AnologData(uint8_t button);
uint8_t Dev_PS2_DataKey(tagPS2_T *_tPS2);
void Dev_PS2_Vibration(tagPS2_T *_tPS2,uint8_t _ucMotor1, uint8_t _ucMotor2);
void Dev_PS2_VibrationMode(tagPS2_T *_tPS2);
void Dev_PS2_Init(tagPS2_T *_tPS2);

#endif

