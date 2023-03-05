#ifndef __OCD_DS3231_H_
#define __OCD_DS3231_H_

#include "drv_hal_conf.h"

/* ds3231和DS1337用的同一套驱动 */
#define DS3231_TYPE_ENABLE
//#define DS1337_TYPE _ENABLE

#define DS3231_IIC_ADDR 	0XD0

#define DS3231_WriteAddress 0xD0    /* 器件写地址 */
#define DS3231_ReadAddress  0xD1    /* 器件读地址 */

#define DS3231_SECOND       0x00    /* 秒 */
#define DS3231_MINUTE       0x01    /* 分 */
#define DS3231_HOUR         0x02    /* 时 */
#define DS3231_WEEK         0x03    /* 星期 */
#define DS3231_DAY          0x04    /* 日 */
#define DS3231_MONTH        0x05    /* 月 */
#define DS3231_YEAR         0x06    /* 年 */

/* 闹铃1寄存器 */            
#define DS3231_ALARM1SECOND 0x07    /* 秒 */
#define DS3231_ALARM1MINUTE 0x08    /* 分 */
#define DS3231_ALARM1HOUR   0x09    /* 时 */
#define DS3231_ALARM1WEEK   0x0A    /* 星期/日 */

/* 闹铃2寄存器 */
#define DS3231_ALARM2MINUTE 0x0b    /* 分 */
#define DS3231_ALARM2HOUR   0x0c    /* 时 */
#define DS3231_ALARM2WEEK   0x0d    /* 星期/日 */

#define DS3231_CONTROL      0x0e    /* 控制寄存器 */
#define DS3231_STATUS       0x0f    /* 状态寄存器 */
#define BSY                 2       /* 忙 */
#define OSF                 7       /* 振荡器停止标志 */
#define DS3231_XTAL         0x10    /* 晶体老化寄存器 */
#define DS3231_TEMPERATUREH 0x11    /* 温度寄存器高字节(8位) */
#define DS3231_TEMPERATUREL 0x12    /* 温度寄存器低字节(高2位) */

typedef struct 
{
	uint8_t  ucYear ;  
	uint8_t  ucMonth ; 
	uint8_t  ucDate ; 
	uint8_t  ucHour ; 
	uint8_t  ucMinute ; 
	uint8_t  ucSecond ; 
	uint8_t  ucWeek ; 
}tagDS3231Time_T;

typedef struct 
{
	uint8_t  ucYear[2] ;  
	uint8_t  ucMonth[2] ; 
	uint8_t  ucDate[2] ; 
	uint8_t  ucHour[2] ; 
	uint8_t  ucMinute[2] ; 
	uint8_t  ucSecond[2] ;
	uint8_t  ucWeek[2] ;
}tagDS3231TimeASCII_T;

typedef struct 
{
	tagIICSoft_T	tIICSoft;
}tagDS3231_T;

uint8_t OCD_DS3231_TimeSetASCII(tagDS3231_T *_tIIC, tagDS3231TimeASCII_T *_tTime);
uint8_t OCD_DS3231_TimeSetHex(tagDS3231_T *_tIIC, tagDS3231Time_T *_tTime);
uint8_t OCD_DS3231_TimeGetASCII(tagDS3231_T *_tIIC, tagDS3231TimeASCII_T *_tTime);
uint8_t OCD_DS3231_TimeGetHex(tagDS3231_T *_tIIC, tagDS3231Time_T *_tTime);
void OCD_DS3231_Alarm1ConfigASCII(tagDS3231_T *_tIIC, tagDS3231TimeASCII_T *_tTime);
void OCD_DS3231_Alarm1ConfigHex(tagDS3231_T *_tIIC, tagDS3231Time_T *_tTime);
void OCD_DS3231_Init(tagDS3231_T *_tIIC);

#endif
