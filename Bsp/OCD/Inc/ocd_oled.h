#ifndef __OCD_OLED_H_
#define __OCD_OLED_H_

#include "drv_hal_conf.h"

typedef struct
{
    tagIICSoft_T tIIC;
}tagOLED_T;

#define OLED_CMD    0
#define OLED_DATA   1

#define OLED_ADDR                       0x78    /* OLED设备地址 */
#define DEACTIVATE_SCROLL			    0x2E	/* 关闭滚动显示功能 */
#define SET_VERTICAL_SCROLL_AREA		0xA3	/* 设置垂直滚动范围命令 */
#define MAX_COLUMN                      128

void OCD_OLED_DisplayON(tagOLED_T *_tOLED);
void OCD_OLED_DisplayOFF(tagOLED_T *_tOLED);
void OCD_OLED_Clear(tagOLED_T *_tOLED);
void OCD_OLED_SetCursorAddress(tagOLED_T *_tOLED,uint8_t x,uint8_t _ucPage);
void OCD_OLED_PageClear(tagOLED_T *_tOLED,uint8_t _ucPage);
void OCD_OLED_ShowChar(tagOLED_T *_tOLED,uint8_t x,uint8_t _ucPage,uint8_t _ucStr,uint8_t _ucChar_Size);
void OCD_OLED_ShowString(tagOLED_T *_tOLED,uint8_t x,uint8_t _ucPage,char *_cpStr,uint8_t _ucChar_Size);
void OCD_OLED_ShowNum(tagOLED_T *_tOLED,uint8_t x,uint8_t _ucPage,uint32_t _ulNum,uint8_t _ucLen,uint8_t _ucChar_Size);
void OCD_OLED_ShowFloatNum(tagOLED_T *_tOLED,uint8_t x,uint8_t _ucPage,float _fNum,uint8_t _ucChar_Size);
void OCD_OLED_ShowBMP(tagOLED_T *_tOLED,uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1,uint8_t _ucaBMP[]);
void OCD_OLED_Init(tagOLED_T *_tOLED);

#endif

