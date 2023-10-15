#ifndef __DRV_FLASH_H_
#define __DRV_FLASH_H_

#include "drv_hal_conf.h"

int Drv_Flash_Erase_Page(uint32_t _ulPageAddr,uint32_t _ulNum);
int Drv_Flash_Erase_Sector(uint32_t _ulStart_Addr, uint32_t _ulEnd_Addr);
void Drv_Flash_Write(uint32_t _ulAddr,uint32_t *_ulpBuf,uint32_t _ulWordSize);
void Drv_Flash_Read(uint32_t _ulAddr, uint32_t *_ulpBuf,uint32_t _ulWordSize);


#endif // !__DRV_FLASH_H_
