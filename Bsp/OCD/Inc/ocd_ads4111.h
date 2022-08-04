#ifndef __OCD_ADS4111_H_
#define __OCD_ADS4111_H_

#include "drv_hal_conf.h"

typedef enum {
	AD411X_DISABLE,
	AD411X_USE_CRC,
	AD411X_USE_XOR,
}temAD4111CrcMode;

typedef struct 
{
	int32_t		ulAddr;
	int32_t		ulValue;
	int32_t		ulSize;
}tagAD4111Reg_T;	/* ad411x_st_reg */

typedef struct 
{
	tagSPI_T			tSPI;
	tagAD4111Reg_T		*tADS4111Reg;
	uint8_t				ucRegNum;
	temAD4111CrcMode	tCrcMode;
}tagAD4111_T;

#endif
