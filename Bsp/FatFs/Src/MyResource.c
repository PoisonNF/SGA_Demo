#include "d_spi.h"

/* 以下可以修改，但不要删，新增资源放在上面 */
_SPI_InitType my_SDcard_spi_bus =
{
	.hspi.Instance = SPI3,
	.hspi.Init.Mode = SPI_MODE_MASTER,
	.hspi.Init.Direction = SPI_DIRECTION_2LINES,
	.hspi.Init.DataSize = SPI_DATASIZE_8BIT,
	.hspi.Init.CLKPolarity = SPI_POLARITY_HIGH,
	.hspi.Init.CLKPhase = SPI_PHASE_2EDGE,
	.hspi.Init.NSS = SPI_NSS_SOFT,
	.hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256,
	.hspi.Init.FirstBit = SPI_FIRSTBIT_MSB,
	.hspi.Init.TIMode = SPI_TIMODE_DISABLE,
	.hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE,
	.hspi.Init.CRCPolynomial = 7,	
#if defined (STM32L4_BSP_ENABLE)
	.hspi.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE,
	.hspi.Init.NSSPMode = SPI_NSS_PULSE_ENABLE,	
#endif
};
