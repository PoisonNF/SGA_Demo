/****************************************************************************

* Sigma团队

* 文件名: ocd_fatfs.c

* 内容简述：FATFS模块文件

* 文件历史：

* 版本号	日期		作者		说明

* 1.0.0a 	2020-02-22	李环宇		创建该文件
* 1.0.1a 	2020-03-29	李环宇		修改OCD_FATFS_ReadDir的接口参数以及内部逻辑，修改OCD_FATFS_ReadData_SpecifyIndex执行逻辑
									新增OCD_FATFS_GetFileInfo函数

****************************************************************************/
#include "ocd_fatfs.h"

/**
 * @brief FATFS文件系统创建文件夹
 * @param _tFATFS-FATFS结构体指针
 * @param _cpPath-文件夹路径指针
 * @retval uint8_t
 */
uint8_t OCD_FATFS_CreateDir(tagFATFS_T *_tFATFS, const char *_cpPath)
{
	f_mount(&_tFATFS->tFATFSInfo.tFATFS, "0:", 1); /* 为SD卡开辟一个工作区 */
	if(f_mkdir(_cpPath)==0) 
	{
	   
	}
	else
	{
		return 0; /* 创建文件夹失败或文件夹已存在 */
	}
	f_mount(NULL, 0, 1); /* 注销工作区 */
	return 1;
}

/**
 * @brief FATFS文件系统读取对应文件夹目录下的文件名
 * @param _tFATFS-FATFS结构体指针
 * @param _tpINFO-文件相关信息结构体指针
 * @param _cpPath-文件夹路径指针
 * @param _ucNameLen-文件名长度
 * @retval uint8_t-文件个数
 */
uint8_t OCD_FATFS_ReadDir(tagFATFS_T *_tFATFS, tagFileInfo_T *_tpINFO, const char *_cpPath, uint8_t _ucNameLen)
{  
	uint8_t index = 0;
	uint8_t j;
	
	f_mount(&_tFATFS->tFATFSInfo.tFATFS, "0:", 1);
	
	if (f_opendir(&_tFATFS->tFATFSInfo.tDIR, _cpPath) == FR_OK)    
	{   
		while (f_readdir(&_tFATFS->tFATFSInfo.tDIR, &_tFATFS->tFATFSInfo.tFILINFO) == FR_OK)    
		{  
			if(!_tFATFS->tFATFSInfo.tFILINFO.fname[0])
				break;
				
			for(j = 0; j < _ucNameLen; j++)
			{
				_tpINFO[index].file_name[j] = _tFATFS->tFATFSInfo.tFILINFO.fname[j];
			}
			index++;
		}
	}
	else 
	{
		return 0;
	}
	   
	f_mount(NULL, 0, 1);
	return index;
}

/**
 * @brief FATFS文件系统读取对应路径下文件指定长度的数据
 * @param _tFATFS-FATFS文件系统结构体
 * @param _cpFileName-文件路径指针
 * @param _ucpData-存放数据的缓冲数组指针
 * @param _usLen-数据长度
 * @param _ulByteVal-读取到的有效字节数
 * @retval uint8_t
 */
uint8_t OCD_FATFS_ReadData_SpecifyFiles(tagFATFS_T *_tFATFS, char *_cpFileName, uint8_t *_ucpData, uint16_t _usLen, uint32_t _ulByteVal)
{
	FRESULT res;
	
	f_mount(&_tFATFS->tFATFSInfo.tFATFS, "0:", 1);
	res = f_open(&_tFATFS->tFATFSInfo.tFIL, _cpFileName, FA_OPEN_EXISTING|FA_READ);
	if(res)
		return 0;
	
	res = f_read(&_tFATFS->tFATFSInfo.tFIL, _ucpData, _usLen, &_ulByteVal);
	if(res)
		return 0;
	
	res = f_close(&_tFATFS->tFATFSInfo.tFIL);
	if(res)
		return 0;
	
	return 1;
}

/**
 * @brief FATFS文件系统读取对应路径下文件的指定位置的数据
 * @param _tFATFS-文件系统结构体
 * @param _cpFileName-文件路径指针
 * @param _ucpData-存放数据的缓冲数组指针
 * @param _usLength-读取数据的长度
 * @param _ulOffset-读取位置的偏移量
 * @param _ulByteVal-读取到的有效字节数
 * @retval uint8_t
 */
uint8_t OCD_FATFS_ReadData_SpecifyIndex(tagFATFS_T *_tFATFS, char *_cpFileName, uint8_t *_ucpData, uint16_t _usLength, uint32_t _ulOffset, uint32_t _ulByteVal)
{
	FRESULT res;
	
	f_mount(&_tFATFS->tFATFSInfo.tFATFS, "0:", 1);
	res = f_open(&_tFATFS->tFATFSInfo.tFIL, _cpFileName, FA_OPEN_EXISTING|FA_READ);	
	if(res)
		return 0;
	
	res = f_lseek(&_tFATFS->tFATFSInfo.tFIL,_ulOffset);	
	if(res)
		return 0;
	
	res = f_read(&_tFATFS->tFATFSInfo.tFIL, _ucpData, _usLength, &_ulByteVal);
	if(res)
		return 0;
	
	res = f_close(&_tFATFS->tFATFSInfo.tFIL);
	if(res)
		return 2;
	
	f_mount(NULL, 0, 1);

	return 1;
}

/**
 * @brief FATFS文件系统对应路径下文件的指定位置写入数据
 * @param _tFATFS-文件系统结构体
 * @param _cpFileName-文件路径指针
 * @param _cpData-需要写入数据的缓冲数组指针
 * @param _lLen-写入数据的长度
 * @param _ulByteVal-写入的有效字节数
 * @retval uint8_t-1-成功；其它-失败
 */
uint8_t OCD_FATFS_Write_SpecifyIndex(tagFATFS_T *_tFATFS, char *_cpFileName, char *_cpData, int _lLen, unsigned int _ulIndex, uint32_t _ulByteVal)
{
	FRESULT res;
	
	res = f_mount(&_tFATFS->tFATFSInfo.tFATFS, "0:", 1);
	
	if(res)
	{
		return 0;
	}

	res = f_open(&_tFATFS->tFATFSInfo.tFIL, _cpFileName, FA_WRITE | FA_CREATE_ALWAYS);		
	if(res)
	{
		return 0;
	}
	
	res = f_lseek(&_tFATFS->tFATFSInfo.tFIL, _ulIndex);
	if(res)
	{
//		f_mount(0,0);
		return 3;
	}
	
	res = f_write(&_tFATFS->tFATFSInfo.tFIL,_cpData, _lLen, &_ulByteVal);
	if(res)
	{
//		f_mount(0,0);
		return 4;
	}
	
	res = f_close(&_tFATFS->tFATFSInfo.tFIL);
		
//	f_mount(&_tFATFS->tFATFSInfo.tFATFS, 0, 0);
		
	if(res)
	{
		return 2;
	}
	
	return 1;
}

/**
 * @brief FATFS文件系统对应路径下文件的尾部写入数据
 * @param _tFATFS-文件系统结构体
 * @param _cpFileName-文件路径指针
 * @param _cpData-需要写入数据的缓冲数组指针
 * @param _lLen-写入数据的长度
 * @param _ulByteVal-写入的有效字节数
 * @retval uint8_t-1-成功；其它-失败
 */
uint8_t OCD_FATFS_Write_End(tagFATFS_T *_tFATFS, char *_cpFileName, char *_cpData, int _lLen, uint32_t _ulByteVal)
{
	FRESULT res = FR_OK;
	
	res = f_mount(&_tFATFS->tFATFSInfo.tFATFS, "0:", 1);
	
	if(res)
	{
		return 0;
	}

	res = f_open(&_tFATFS->tFATFSInfo.tFIL, _cpFileName, FA_WRITE | FA_OPEN_ALWAYS);
		
	if(res)
	{
//		f_mount(0,0);
		return 0;
	}
	
	res = f_lseek(&_tFATFS->tFATFSInfo.tFIL, f_size(&_tFATFS->tFATFSInfo.tFIL));
	if(res)
	{
//		f_mount(0,0);
		return 3;
	}

	res = f_write(&_tFATFS->tFATFSInfo.tFIL,_cpData, _lLen, &_ulByteVal);
	if(res)
	{
//		f_mount(0,0);
		return 4;
	}
	res = f_close(&_tFATFS->tFATFSInfo.tFIL);
		
//	f_mount(&_tFATFS->tFATFSInfo.tFATFS, 0, 0);
		
	if(res)
	{
		return 2;
	}
	
	return 1;
}

/**
 * @brief FATFS文件系统对应路径下文件写入数据
 * @param _tFATFS-文件系统结构体
 * @param _cpFileName-文件路径指针
 * @param _cpData-需要写入数据的缓冲数组指针
 * @param _lLen-写入数据的长度
 * @param _ulByteVal-写入的有效字节数
 * @retval uint8_t-1-成功；其它-失败
 */
uint8_t OCD_FATFS_Write(tagFATFS_T *_tFATFS, char *_cpFileName,char *_cpData,int _lLen, uint32_t _ulByteVal)
{
	FRESULT res;
	
	res = f_mount(&_tFATFS->tFATFSInfo.tFATFS, "0:", 1);
	
	if(res)
		return 0;

	res = f_open(&_tFATFS->tFATFSInfo.tFIL, _cpFileName, FA_WRITE | FA_OPEN_ALWAYS);
		
	if(res)
		return 0;

	res = f_write(&_tFATFS->tFATFSInfo.tFIL,_cpData, _lLen, &_ulByteVal);
	if(res)
		return 4;

	res = f_close(&_tFATFS->tFATFSInfo.tFIL);
		
//	f_mount(&_tFATFS->tFATFSInfo.tFATFS, 0, 0);
		
	if(res)
	{
		return 2;
	}
	
	return 1;
}

/**
 * @brief FATFS文件系统读取文件信息
 * @param _tFATFS-文件系统结构体
 * @param _cpFileName-文件路径指针
 * @retval uint8_t-1-成功；其它-失败
 */
uint8_t OCD_FATFS_GetFileInfo(tagFATFS_T *_tFATFS, char *_cpFileName)
{
	FRESULT res;
	
	f_mount(&_tFATFS->tFATFSInfo.tFATFS, "0:", 1);
	res = f_open(&_tFATFS->tFATFSInfo.tFIL, _cpFileName, FA_OPEN_EXISTING|FA_READ);	
	if(res)
		return 0;
	
	res = f_stat(_cpFileName, &_tFATFS->tFATFSInfo.tFILINFO);
	if(res)
		return 2;
	
	return 1;
}

/**
 * @brief FATFS文件系统初始化
 * @param _tFATFS-FATFS结构体指针
 * @retval uint8_t
 */
uint8_t OCD_FATFS_Init(tagFATFS_T *_tFATFS)
{
	return OCD_SDCard_SPIInit(_tFATFS->tSPI);
}

