#ifndef __DRV_FLASH_H_
#define __DRV_FLASH_H_

#include "drv_hal_conf.h"

int Drv_Flash_Erase_Page(uint32_t ulPageAddr,uint32_t ulNum);
int Drv_Flash_Erase_Sector(uint32_t ulStart_Addr, uint32_t ulEnd_Addr);
void Drv_Flash_Write(uint32_t ulAddr,uint32_t *ulpBuf,uint32_t ulWordSize);
void Drv_Flash_Read(uint32_t ulAddr, uint32_t *ulpBuf,uint32_t ulWordSize);


#endif // !__DRV_FLASH_H_
