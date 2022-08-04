#ifndef __OCD_DS3231_H_
#define __OCD_DS3231_H_

#include "drv_hal_conf.h"

/* ds3231和DS1337用的同一套驱动 */
#define DS3231_TYPE_ENABLE
//#define DS1337_TYPE _ENABLE

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
