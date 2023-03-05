#ifndef __DRV_SPI_SOFT_H_
#define	__DRV_SPI_SOFT_H_

#include "drv_hal_conf.h"

#define SPI_Delay  {__nop();__nop();}

typedef struct{
	tagGPIO_T tSPISoft[4];
} tagSPISoft_T;

/**SPI GPIO Configuration  按照这个顺序来定义
PXxx     ------> SPI_SCK
PXxx      ------> SPI_NSS 
PXxx      ------> SPI_MOSI
PXxx      ------> SPI_MISO
*/

void Drv_SPISoft_Init(tagSPISoft_T *_tSPI);
void Drv_SPI_NSS(tagSPISoft_T *_tSPI,uint8_t _ucLevel);
uint8_t Drv_SPI_WriteByte_Soft(tagSPISoft_T *_tSPI,uint8_t _ucData);


#endif

