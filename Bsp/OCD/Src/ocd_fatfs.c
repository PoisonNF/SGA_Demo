/****************************************************************************

* Sigma团队

* 文件名: ocd_fatfs.c

* 内容简述：FATFS模块文件
            需配合ocd_sdcard.c使用
			使用前需要在工程中添加Bsp/Fatfs文件夹，加入ff.c和diskio.c

* 文件历史：

* 版本号		日期		作者		    说明
*  	 		2023-05-14	  鲍程璐		修复读写函数无返回有效字节数的问题，格式优化

* 1.0.1a 	2020-03-29	  李环宇		修改OCD_FATFS_ReadDir的接口参数以及内部逻辑，修改OCD_FATFS_ReadData_SpecifyIndex执行逻辑
									    新增OCD_FATFS_GetFileInfo函数

* 1.0.0a 	2020-02-22	  李环宇		创建该文件

****************************************************************************/
#include "ocd_fatfs.h"

/**
 * @brief FATFS文件系统创建文件夹
 * @param _tFATFS-FATFS结构体指针
 * @param _cpPath-文件夹路径指针
 * @retval uint8_t FR_OK：成功 FR_EXIST:文件夹已经存在 -1：失败
 */
int8_t OCD_FATFS_CreateDir(tagFATFS_T *_tFATFS, const char *_cpPath)
{
	FRESULT res;

	f_mount(&_tFATFS->tFATFSInfo.tFATFS, "0:", 1); /* 为SD卡开辟一个工作区 */

	res = f_mkdir(_cpPath);
    
    /* 文件夹已经存在 */
    if(res == FR_EXIST)
    {
        f_mount(NULL, 0, 1);	/* 注销工作区 */
        return FR_EXIST;
    }
        
	/* 创建文件夹失败 */
	if(res != FR_OK)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}
	
	f_mount(NULL, 0, 1);	/* 注销工作区 */
	return FR_OK;
}

/**
 * @brief FATFS文件系统读取对应文件夹目录下的文件名
 * @param _tFATFS-FATFS结构体指针
 * @param _tpINFO-文件相关信息结构体指针
 * @param _cpPath-文件夹路径指针
 * @param _ucNameLen-文件名长度
 * @retval uint8_t-文件个数
 */
int8_t OCD_FATFS_ReadDir(tagFATFS_T *_tFATFS, tagFileInfo_T *_tpINFO, const char *_cpPath, uint8_t _ucNameLen)
{  
	uint8_t index = 0;
	uint8_t j;
	
	f_mount(&_tFATFS->tFATFSInfo.tFATFS, "0:", 1);
	
	if(f_opendir(&_tFATFS->tFATFSInfo.tDIR, _cpPath) == FR_OK)    
	{   
		while (f_readdir(&_tFATFS->tFATFSInfo.tDIR, &_tFATFS->tFATFSInfo.tFILINFO) == FR_OK)    
		{  
			if(!_tFATFS->tFATFSInfo.tFILINFO.fname[0])
				break;
				
			for(j = 0; j < _ucNameLen; j++)
			{
				_tpINFO[index].ucFile_Name[j] = _tFATFS->tFATFSInfo.tFILINFO.fname[j];
			}
			index++;
		}
	}
	else 
	{
		return -1;
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
 * @param _ulpByteVal-返回读取到的有效字节数的地址
 * @retval uint8_t FR_OK：成功 -1：失败
 */
int8_t OCD_FATFS_Read_SpecifyFiles(tagFATFS_T *_tFATFS, char *_cpFileName, uint8_t *_ucpData, uint32_t _ulLength, uint32_t *_ulpByteVal)
{
	FRESULT res;
	
	f_mount(&_tFATFS->tFATFSInfo.tFATFS, "0:", 1);
	res = f_open(&_tFATFS->tFATFSInfo.tFIL, _cpFileName, FA_OPEN_EXISTING|FA_READ);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}
		
	res = f_read(&_tFATFS->tFATFSInfo.tFIL, _ucpData, _ulLength, _ulpByteVal);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}
	
	res = f_close(&_tFATFS->tFATFSInfo.tFIL);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}

	f_mount(NULL, 0, 1);
	return FR_OK;
}

/**
 * @brief FATFS文件系统读取对应路径下文件的指定位置的数据
 * @param _tFATFS-文件系统结构体
 * @param _cpFileName-文件路径指针
 * @param _ucpData-存放数据的缓冲数组指针
 * @param _ulLength-读取数据的长度
 * @param _ulOffset-读取位置的偏移量
 * @param _ulpByteVal-返回读取到的有效字节数的地址
 * @retval uint8_t FR_OK：成功 -1：失败
 */
int8_t OCD_FATFS_Read_SpecifyIndex(tagFATFS_T *_tFATFS, char *_cpFileName, uint8_t *_ucpData, uint32_t _ulLength, uint32_t _ulOffset, uint32_t *_ulpByteVal)
{
	FRESULT res;
	
	f_mount(&_tFATFS->tFATFSInfo.tFATFS, "0:", 1);
	res = f_open(&_tFATFS->tFATFSInfo.tFIL, _cpFileName, FA_OPEN_EXISTING|FA_READ);	
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}
	
	res = f_lseek(&_tFATFS->tFATFSInfo.tFIL,_ulOffset);	
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}
	
	res = f_read(&_tFATFS->tFATFSInfo.tFIL, _ucpData, _ulLength, _ulpByteVal);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}
	
	res = f_close(&_tFATFS->tFATFSInfo.tFIL);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}
	
	f_mount(NULL, 0, 1);
	return FR_OK;
}

/**
 * @brief FATFS文件系统对应路径下文件的指定位置写入数据
 * @param _tFATFS-文件系统结构体
 * @param _cpFileName-文件路径指针
 * @param _ucpData-需要写入数据的缓冲数组指针
 * @param _ulLength-写入数据的长度
 * @param _ulOffset-写入位置的偏移量
 * @param _ulpByteVal-返回写入到的有效字节数的地址
 * @retval uint8_t FR_OK：成功 -1：失败
 */
int8_t OCD_FATFS_Write_SpecifyIndex(tagFATFS_T *_tFATFS, char *_cpFileName, uint8_t *_ucpData, uint32_t _ulLength, uint32_t _ulOffset, uint32_t *_ulpByteVal)
{
	FRESULT res;
	
	res = f_mount(&_tFATFS->tFATFSInfo.tFATFS, "0:", 1);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}

	res = f_open(&_tFATFS->tFATFSInfo.tFIL, _cpFileName, FA_WRITE | FA_CREATE_ALWAYS);		
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}
	
	res = f_lseek(&_tFATFS->tFATFSInfo.tFIL, _ulOffset);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}
	
	res = f_write(&_tFATFS->tFATFSInfo.tFIL,_ucpData, _ulLength, _ulpByteVal);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}
	
	res = f_close(&_tFATFS->tFATFSInfo.tFIL);	
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}

	f_mount(NULL, 0, 1);
	return FR_OK;
}

/**
 * @brief FATFS文件系统对应路径下文件的尾部写入数据
 * @param _tFATFS-文件系统结构体
 * @param _cpFileName-文件路径指针
 * @param _ucpData-需要写入数据的缓冲数组指针
 * @param _ulLength-写入数据的长度
 * @param _ulpByteVal-返回写入到的有效字节数的地址
 * @retval uint8_t FR_OK：成功 -1：失败
 */
int8_t OCD_FATFS_Write_End(tagFATFS_T *_tFATFS, char *_cpFileName, uint8_t *_ucpData, uint32_t _ulLength , uint32_t *_ulpByteVal)
{
	FRESULT res = FR_OK;
	
	res = f_mount(&_tFATFS->tFATFSInfo.tFATFS, "0:", 1);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}

	res = f_open(&_tFATFS->tFATFSInfo.tFIL, _cpFileName, FA_WRITE | FA_OPEN_ALWAYS);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}
	
	res = f_lseek(&_tFATFS->tFATFSInfo.tFIL, f_size(&_tFATFS->tFATFSInfo.tFIL));
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}

	res = f_write(&_tFATFS->tFATFSInfo.tFIL,_ucpData, _ulLength, _ulpByteVal);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}

	res = f_close(&_tFATFS->tFATFSInfo.tFIL);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}
	
	f_mount(NULL, 0, 1);
	return FR_OK;
}

/**
 * @brief FATFS文件系统对应路径下文件写入数据
 * @param _tFATFS-文件系统结构体
 * @param _cpFileName-文件路径指针
 * @param _ucpData-需要写入数据的缓冲数组指针
 * @param _ulLength-写入数据的长度
 * @param _ulpByteVal-返回写入到的有效字节数的地址
 * @retval uint8_t FR_OK：成功 -1：失败
 */
int8_t OCD_FATFS_Write(tagFATFS_T *_tFATFS, char *_cpFileName,uint8_t *_ucpData, uint32_t _ulLength, uint32_t *_ulpByteVal)
{
	FRESULT res;
	
	res = f_mount(&_tFATFS->tFATFSInfo.tFATFS, "0:", 1);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}

	res = f_open(&_tFATFS->tFATFSInfo.tFIL, _cpFileName, FA_WRITE | FA_OPEN_ALWAYS);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}

	res = f_write(&_tFATFS->tFATFSInfo.tFIL,_ucpData, _ulLength, _ulpByteVal);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}

	res = f_close(&_tFATFS->tFATFSInfo.tFIL);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}

	f_mount(NULL, 0, 1);
	return FR_OK;
}

/**
 * @brief FATFS文件系统读取文件信息
 * @param _tFATFS-文件系统结构体
 * @param _cpFileName-文件路径指针
 * @retval uint8_t FR_OK：成功 -1：失败
 */
int8_t OCD_FATFS_GetFileInfo(tagFATFS_T *_tFATFS, char *_cpFileName)
{
	FRESULT res;
	
	f_mount(&_tFATFS->tFATFSInfo.tFATFS, "0:", 1);
	res = f_open(&_tFATFS->tFATFSInfo.tFIL, _cpFileName, FA_OPEN_EXISTING|FA_READ);	
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}
	
	res = f_stat(_cpFileName, &_tFATFS->tFATFSInfo.tFILINFO);
	if(res)
	{
		Drv_HAL_Error(__FILE__,__LINE__);
		return -1;
	}
	
	f_mount(NULL, 0, 1);
	return FR_OK;
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

