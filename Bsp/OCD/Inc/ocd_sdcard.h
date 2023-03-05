#ifndef __UT_SDCARD_H
#define __UT_SDCARD_H

#include "drv_hal_conf.h"

/* SD卡相关寄存器宏定义 */  
#define CMD0    0       /* 卡复位 */
#define CMD1    1
#define CMD8    8       /* 命令8 ，SEND_IF_COND */
#define CMD9    9       /* 命令9 ，读CSD数据 */
#define CMD10   10      /* 命令10，读CID数据 */
#define CMD12   12      /* 命令12，停止数据传输 */
#define CMD16   16      /* 命令16，设置SectorSize 应返回0x00 */
#define CMD17   17      /* 命令17，读sector */
#define CMD18   18      /* 命令18，读Multi sector */
#define CMD23   23      /* 命令23，设置多sector写入前预先擦除N个block */
#define CMD24   24      /* 命令24，写sector */
#define CMD25   25      /* 命令25，写Multi sector */
#define CMD41   41      /* 命令41，应返回0x00 */
#define CMD55   55      /* 命令55，应返回0x01 */
#define CMD58   58      /* 命令58，读OCR信息 */
#define CMD59   59      /* 命令59，使能/禁止CRC，应返回0x00 */

/* SD卡类型宏定义 */
#define SD_TYPE_ERR     0X00
#define SD_TYPE_MMC     0X01
#define SD_TYPE_V1      0X02
#define SD_TYPE_V2      0X04
#define SD_TYPE_V2HC    0X06 

static uint8_t s_ucSdType = 0;
static tagSPI_T s_tSPI;  	/* SPI结构体 */

uint8_t OCD_SDCard_SPIInit(tagSPI_T _sdcard);

#endif
