#ifndef __OCD_SDCARD_FUNC_H_
#define __OCD_SDCARD_FUNC_H_

#include "ocd_sdcard.h"

#include <ff.h>

#define FILE_NAME_LENTH		20		/* 存储文件名长度支持20字节字符，该架构下的fatfs文件系统暂不支持中文文件名 */

/* FATFS文件系统相关信息结构体 */
typedef struct 
{
	FATFS tFATFS;
	FIL tFIL;

	FILINFO tFILINFO;
	DIR   tDIR;
}tagFATFSInfo_T;

/* FATFS文件相关信息结构体 */
typedef struct 
{
	unsigned char file_name[FILE_NAME_LENTH];
}tagFileInfo_T;		/* FATFS文件相关信息结构体 */

/* FATFS文件系统结构体 */
typedef struct
{
	tagSPI_T 		tSPI;		/* STM32内部SPI设备指针 */
	tagFATFSInfo_T	tFATFSInfo;	/* FATFS文件系统相关信息结构体 */
}tagFATFS_T;

uint8_t OCD_FATFS_Init(tagFATFS_T *_tFATFS);
uint8_t OCD_FATFS_CreateDir(tagFATFS_T *_tFATFS, const char *_cpPath);
uint8_t OCD_FATFS_ReadDir(tagFATFS_T *_tFATFS, tagFileInfo_T *_tpINFO, const char *_cpPath, uint8_t _ucNameLen);
uint8_t OCD_FATFS_ReadData_SpecifyFiles(tagFATFS_T *_tFATFS, char *_cpFileName, uint8_t *_ucpData, uint16_t _usLen, uint32_t _ulByteVal);
uint8_t OCD_FATFS_ReadData_SpecifyIndex(tagFATFS_T *_fatfsInfo, char *_cpFileName, uint8_t *_ucpData, uint16_t _usLength, uint32_t _ulOffset, uint32_t _ulByteVal);
uint8_t OCD_FATFS_Write_SpecifyIndex(tagFATFS_T *_tFATFS, char *_cpFileName, char *_cpData, int _lLen, unsigned int _ulIndex, uint32_t _ulByteVal);
uint8_t OCD_FATFS_Write_End(tagFATFS_T *_tFATFS, char *_cpFileName, char *_cpData, int _lLen, uint32_t _ulByteVal);
uint8_t OCD_FATFS_Write(tagFATFS_T *_tFATFS, char *_cpFileName,char *_cpData,int _lIndex, uint32_t _ulByteVal);
uint8_t OCD_FATFS_GetFileInfo(tagFATFS_T *_tFATFS, char *_cpFileName);

#endif
