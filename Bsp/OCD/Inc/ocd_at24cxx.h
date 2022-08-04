#ifndef __OCD_AT24CXX_H_
#define __OCD_AT24CXX_H_

#include "drv_hal_conf.h"

typedef struct{
	tagIICSoft_T tIIC;
}tagAT24CXX_T;

void OCD_AT24CXX_WriteLenByte(tagAT24CXX_T *_tAT24CXX, uint16_t _usAddr, uint8_t *_ulpData, uint8_t _ucLen);
void OCD_AT24CXX_ReadLenByte(tagAT24CXX_T *_tAT24CXX, uint16_t _usAddr, uint8_t *_ulpData, uint8_t _ucLen);
uint8_t OCD_AT24CXX_Check(tagAT24CXX_T *_tAT24CXX);
void OCD_AT24CXX_Read(tagAT24CXX_T *_tAT24CXX, uint16_t _usAddr, uint8_t *_ucpBuffer, uint16_t _usNum);
void OCD_AT24CXX_Write(tagAT24CXX_T *_tAT24CXX, uint16_t _usAddr, uint8_t *_ucpBuffer, uint16_t _usNum);

void OCD_AT24CXX_Init(tagAT24CXX_T *_tAT24CXX);

#endif
