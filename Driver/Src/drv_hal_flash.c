/****************************************************************************

* Sigma团队

* 文件名: drv_hal_flash.c

* 内容简述：Flash驱动文件

* 文件历史：

* 版本号	   日期		  作者		  说明
*  2.8	 	2023-09-22	鲍程璐		创建该文件

****************************************************************************/
#include "drv_hal_conf.h"

#ifdef DRV_HAL_FLASH_ENABLE

#ifdef STM32F1_SGA_ENABLE

/**
 * @brief Flash擦除页（F1系列）
 * 
 * @param _ulPageAddr 起始地址
 * @param _ulNum 擦除的页数
 * @return int 0 成功 -1 失败
 */
int Drv_Flash_Erase_Page(uint32_t _ulPageAddr,uint32_t _ulNum)
{
    /* 解锁Flash */
	HAL_FLASH_Unlock();

    /* 擦除Flash*/
	FLASH_EraseInitTypeDef FlashSet;
	FlashSet.TypeErase = FLASH_TYPEERASE_PAGES;
	FlashSet.PageAddress = _ulPageAddr;
	FlashSet.NbPages = _ulNum;

    /*设置PageError，调用擦除函数*/
	uint32_t PageError = 0;
	if(HAL_FLASHEx_Erase(&FlashSet, &PageError) == HAL_ERROR)
    {
        /* 锁定flash */
	    HAL_FLASH_Lock();
        return -1;
    }

    /* 锁定flash */
	HAL_FLASH_Lock();

    return 0;
}
#endif

#ifdef STM32F4_SGA_ENABLE

/**
 * @brief 获取地址所在的sector
 * @param _ulAddress  起始地址	
 * @return uint32_t ulSector 扇区号
 */
static uint32_t S_Flash_Get_Sector(uint32_t _ulAddress)
{
    uint32_t ulSector = 0;

	(_ulAddress <= 0x080FFFFF && _ulAddress >= 0x080E0000)? ulSector = 11:
	(_ulAddress >= 0x080C0000)? ulSector = 10:
	(_ulAddress >= 0x080A0000)? ulSector = 9:
	(_ulAddress >= 0x08080000)? ulSector = 8:
	(_ulAddress >= 0x08060000)? ulSector = 7:
	(_ulAddress >= 0x08040000)? ulSector = 6:
	(_ulAddress >= 0x08020000)? ulSector = 5:
	(_ulAddress >= 0x08010000)? ulSector = 4:
	(_ulAddress >= 0x0800C000)? ulSector = 3:
	(_ulAddress >= 0x08008000)? ulSector = 2:
	(_ulAddress >= 0x08006000)? ulSector = 1:
	(_ulAddress >= 0x08004000)? ulSector = 1:0;
	
    return ulSector;
}

/**
 * @brief Flash擦除Sector（F4系列）
 * @param _ulStart_Addr  起始地址	
 * @param _ulEnd_Addr    结束地址
 * @return 0 成功 -1 失败
 */
int Drv_Flash_Erase_Sector(uint32_t _ulStart_Addr, uint32_t _ulEnd_Addr)
{
	uint32_t UserStartSector;
	uint32_t SectorError = 0;
	FLASH_EraseInitTypeDef FlashSet;

	/* 解锁flash */
	HAL_FLASH_Unlock();
	
	/* 获取起始地址的扇区，擦除FLASH*/
	UserStartSector = S_Flash_Get_Sector(_ulStart_Addr);

	FlashSet.TypeErase = TYPEERASE_SECTORS;
	FlashSet.Sector = UserStartSector;
	FlashSet.NbSectors = S_Flash_Get_Sector(_ulEnd_Addr) - UserStartSector;
	FlashSet.VoltageRange = VOLTAGE_RANGE_3;
    
	/*调用擦除函数*/
	if(HAL_FLASHEx_Erase(&FlashSet, &SectorError) == HAL_ERROR)
    {
        /* 锁定Flash */
	    HAL_FLASH_Lock();
        return -1;
    }
	
	/* 锁定Flash */
	HAL_FLASH_Lock();
	return 0;
}

#endif

/**
 * @brief Flash写若干个数据(word)
 * @param _ulAddr      写入的地址
 * @param _ulpBuf      写入数据的起始地址
 * @param _ulWordSize  长度
 * @return NULL
 */
void Drv_Flash_Write(uint32_t _ulAddr,uint32_t *_ulpBuf,uint32_t _ulWordSize)
{
	/* 解锁Flash */
	HAL_FLASH_Unlock();

	/* 烧写到指定地址 */
	for(uint32_t i = 0;i < _ulWordSize;i++)
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,_ulAddr + 4*i,_ulpBuf[i]);

	/* 锁定Flash */
	HAL_FLASH_Lock();
    
    return;
}

/**
 * @brief Flash读若干个数据(word)
 * @param _ulAddr      读数据的地址
 * @param _ulpBuf      读出数据的数组指针
 * @param _ulWordSize  长度
 * @return NULL
 */
void Drv_Flash_Read(uint32_t _ulAddr, uint32_t *_ulpBuf,uint32_t _ulWordSize)
{
	memcpy(_ulpBuf, (uint32_t*) _ulAddr, _ulWordSize * sizeof(uint32_t));
}

#endif
