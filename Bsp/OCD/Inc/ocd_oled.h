#ifndef __OCD_OLED_H_
#define __OCD_OLED_H_

#include "drv_hal_conf.h"

typedef struct
{
    tagIICSoft_T tIIC;
}tagOLED_T;

#define OLED_CMD    0
#define OLED_DATA   1

#define DEACTIVATE_SCROLL			    0x2E	//关闭滚动显示功能
#define SET_VERTICAL_SCROLL_AREA		0xA3	//设置垂直滚动范围命令
#define Max_Column                      128

static void OLED_WR_Byte(tagOLED_T *_tOLED,uint8_t dat,uint8_t cmd);
void OCD_OLED_DisplayON(tagOLED_T *_tOLED);
void OCD_OLED_DisplayOFF(tagOLED_T *_tOLED);
void OCD_OLED_Clear(tagOLED_T *_tOLED);
void OLED_SetCursorAddrese(tagOLED_T *_tOLED,uint8_t x,uint8_t y);
void OCD_OLED_PageClear(tagOLED_T *_tOLED,uint8_t page);
void OCD_OLED_ShowString(tagOLED_T *_tOLED,uint8_t x,uint8_t y,uint8_t *str,uint8_t Char_Size);
uint32_t OLED_Pow(uint8_t m,uint8_t n);
void OCD_OLED_ShowNum(tagOLED_T *_tOLED,uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t Char_Size);
void OCD_OLED_ShowFNum(tagOLED_T *_tOLED,uint8_t x,uint8_t y,float Fnum,uint8_t Char_Size);
void OLED_DrawBMP(tagOLED_T *_tOLED,uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1,uint8_t BMP[]);
void OCD_OLED_Init(tagOLED_T *_tOLED);

#endif

