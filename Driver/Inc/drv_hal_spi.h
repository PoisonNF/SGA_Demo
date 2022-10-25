#ifndef __DRV_SPI_H_
#define __DRV_SPI_H_

#include "drv_hal_conf.h"

typedef struct
{
	SPI_HandleTypeDef	tSPIHandle;
	tagGPIO_T			tGPIO[4];
}tagSPI_T;

/* SPI÷ÿ”≥…‰±Ì
	SPI1 full remap (NSS/PA15, SCK/PB3, MISO/PB4, MOSI/PB5)
		 no remap (NSS/PA4,  SCK/PA5, MISO/PA6, MOSI/PA7)
*/

#define SPI_SPEED_HIGH	1
#define SPI_SPEED_LOW	0

void Drv_SPI_SpeedConfig(tagSPI_T *_tSPI, uint8_t _ucSpeed);
uint8_t Drv_SPI_TransmitReceive(tagSPI_T *_tSPI, uint8_t _ucTxData);
void Drv_SPI_Transmit(tagSPI_T *_tSPI, uint8_t _ucTxData);

void Drv_SPI_Init(tagSPI_T *_tSPI);

#endif
