#ifndef __DRV_SPI_SOFT_H_
#define	__DRV_SPI_SOFT_H_

#include "drv_hal_conf.h"

typedef struct{
	tagGPIO_T tSPISoft[4];
} tagSPISoft_T;
	/**SPI GPIO Configuration  按照这个顺序来定义
    PXxx     ------> SPI_SCK
    PXxx      ------> SPI_NSS 
    PXxx      ------> SPI_MOSI
    PXxx      ------> SPI_MISO
    */

#define SPI_Delay  {__nop();__nop();}

void Drv_SPISoft_Init(tagSPISoft_T *_tSPI);
void Drv_SPI_NSS(tagSPISoft_T *_tSPI,uint8_t level);
uint8_t Drv_SPI_WriteByte_soft(tagSPISoft_T *_tSPI,uint8_t data);


#endif

