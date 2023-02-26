#ifndef __OCD_PS2_H_
#define __OCD_PS2_H_

#include "drv_hal_conf.h"

#define DI  Drv_GPIO_Read(&_tPS2->tGPIO[0])     //DI输入

#define DO_H Drv_GPIO_Set(&_tPS2->tGPIO[1])     //命令位高
#define DO_L Drv_GPIO_Reset(&_tPS2->tGPIO[1])   //命令位低

#define CS_H Drv_GPIO_Set(&_tPS2->tGPIO[2])     //CS拉高
#define CS_L Drv_GPIO_Reset(&_tPS2->tGPIO[2])   //CS拉低

#define CLK_H Drv_GPIO_Set(&_tPS2->tGPIO[3])    //时钟拉高
#define CLK_L Drv_GPIO_Reset(&_tPS2->tGPIO[3])  //时钟拉低

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
#define PSS_RX 5                //右摇杆X轴数据
#define PSS_RY 6                //右摇杆Y轴数据
#define PSS_LX 7                //左摇杆X轴数据
#define PSS_LY 8                //左摇杆Y轴数据

#define DELAY_TIME  Drv_Delay_Us(5); 

typedef struct 
{
    tagGPIO_T tGPIO[4];     //按照[0]DI/DAT [1]DO/CMD [2]CS [3]CLK顺序
}tagPS2_T;

uint16_t Handkey;	// 按键值读取，零时存储。
uint8_t Comd[2]={0x01,0x42};	//开始命令。请求数据
uint8_t PS2Data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //数据存储数组
                    /*PS2Data[0]    空 
                      PS2Data[1]    0x73 红灯模式 0x41 灭灯模式
                      PS2Data[2]    0x5A 返回数据预告
                      PS2Data[3]    低8位按键状态 0按下
                      PS2Data[4]    高8位按键状态 0按下
                      PS2Data[5]    PSS_RX
                      PS2Data[6]    PSS_RY
                      PS2Data[7]    PSS_LX
                      PS2Data[8]    PSS_LY
                    */
uint16_t MASK[]={
    PSB_SELECT,
    PSB_L3,
    PSB_R3 ,
    PSB_START,
    PSB_PAD_UP,
    PSB_PAD_RIGHT,
    PSB_PAD_DOWN,
    PSB_PAD_LEFT,
    PSB_L2,
    PSB_R2,
    PSB_L1,
    PSB_R1 ,
    PSB_GREEN,
    PSB_RED,
    PSB_BLUE,
    PSB_PINK
};

void OCD_PS2_Init(tagPS2_T *_tPS2);
uint8_t OCD_PS2_RedLight(tagPS2_T *_tPS2);
uint8_t OCD_PS2_AnologData(uint8_t button);
void OCD_PS2_ReadData(tagPS2_T *_tPS2);
uint8_t OCD_PS2_DataKey(tagPS2_T *_tPS2);

#endif

