#ifndef __SGA_FUNC_H_
#define __SGA_FUNC_H_

#include "drv_hal_conf.h"

typedef struct
{
	uint8_t *ucpAddr;
	uint32_t ulIndex;
}tag_StringInfo;

tag_StringInfo SGA_Function_Query_Sub(uint8_t *_ucpStrA, uint8_t *_ucpStrB);
void SGA_Function_Insert_Sub(uint8_t *_ucpStrA, uint8_t *_ucpStrB, uint16_t usOffset);
uint16_t SGA_Function_Crc16_Modbus(uint8_t *_puchMsg, uint16_t _usDataLen);
uint32_t SGA_StringToHex(uint8_t *_ucpSrc, uint8_t _ucLen );
uint8_t SGA_DecToHex(uint8_t _ucDec);

#endif
