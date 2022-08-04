/****************************************************************************

* Sigma团队

* 文件名: ut_sdcard.c

* 内容简述：SD卡模块文件

* 文件历史：

* 版本号	日期		作者		说明

* 1.0.0a 	2020-02-22	李环宇		创建该文件

****************************************************************************/
#include "ocd_sdcard.h"

#include "diskio.h"			/* 不使用FATFS时注释掉 */

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

/**
 * @brief SD卡发送等待终止信号
 * @param NULL
 * @retval Null
*/
static void S_SDCard_WaitStop(void)
{
	Drv_SPI_TransmitReceive(&s_tSPI, 0xFF);
}

/**
 * @brief SD卡发送等待就绪信号
 * @param NULL
 * @retval uint8_t-1,就绪；0-错误
*/
static uint8_t S_SDCard_WaitReady(void)
{
	uint8_t res = 0;
	uint32_t ulTxNum = 0;	/* 发送次数 */
	
	do
	{		
		res = Drv_SPI_TransmitReceive(&s_tSPI, 0xFF);
		if(res ==0xFF)
			return 0;
		
		ulTxNum++;		  	
	}while(ulTxNum < 0xFFFFFF);
	
	return 1;
}

/**
 * @brief SD卡等待
 * @param NULL
 * @retval uint8_t-0,就绪；0-错误
*/
static uint8_t S_SDCard_Wait(void)
{
	if(S_SDCard_WaitReady()==0)
	{
		return 0;
	}
	else
	{
		S_SDCard_WaitStop();
		return 1;
	}
}

/**
 * @brief SD卡发送命令
 * @param _ucCmd-命令
 * @param _ulArg-
 * @param _ucCrc-CRC校验
 * @retval uint8_t-0xff,错误；其它-返回的一个字节数据
*/
static uint8_t S_SDCard_SendCMD(uint8_t _ucCmd, uint32_t _ulArg, uint8_t _ucCrc)
{
	uint8_t ucRx;	
	uint8_t ucReTryNum = 0; 
	
	S_SDCard_WaitStop();
	
	if(S_SDCard_Wait()==1)
		return 0xFF; 

    Drv_SPI_TransmitReceive( &s_tSPI, _ucCmd | 0x40);
    Drv_SPI_TransmitReceive( &s_tSPI, _ulArg >> 24);
    Drv_SPI_TransmitReceive( &s_tSPI, _ulArg >> 16);
    Drv_SPI_TransmitReceive( &s_tSPI, _ulArg >>  8);
    Drv_SPI_TransmitReceive( &s_tSPI, _ulArg);	  
    Drv_SPI_TransmitReceive( &s_tSPI, _ucCrc); 
	if(_ucCmd == CMD12)
		Drv_SPI_TransmitReceive( &s_tSPI, 0xFF);
	
	ucReTryNum = 0x1F;	/* 重发次数 */
	do
	{
		ucRx = Drv_SPI_TransmitReceive(&s_tSPI, 0xFF);
	}while((ucRx & 0x80) && ucReTryNum--);
	
	return ucRx;
}

/**
 * @brief SD卡的初始化
 * @param NULL
 * @retval Null
*/
static uint8_t S_SDCard_Init(void)
{
	uint8_t ucRx;      
	uint8_t index;
	uint16_t usReTryNum;  
	uint8_t ucRxBuffer[4];
	
	Drv_SPI_Init(&s_tSPI);
	Drv_SPI_SpeedConfig(&s_tSPI, 0);
		
	for(index = 0;index < 10;index++)
		Drv_SPI_TransmitReceive(&s_tSPI, 0xFF);
		
	usReTryNum = 20;
	do
	{
		ucRx = S_SDCard_SendCMD( CMD0, 0, 0x95);
	}while((ucRx != 0X01) && usReTryNum--);
			
	if(ucRx == 0X01)
	{
		if(S_SDCard_SendCMD( CMD8, 0x1AA, 0x87) == 1)
		{
			for(index = 0; index < 4; index++)
				ucRxBuffer[index] = Drv_SPI_TransmitReceive(&s_tSPI, 0xFF);	
						
			/* 卡是否支持2.7~3.6V */
			if(ucRxBuffer[2]==0x01 && ucRxBuffer[3]==0xAA)
			{
				usReTryNum = 0XFFFE;
				do
				{
					ucRx = S_SDCard_SendCMD(CMD55, 0, 0x01);
					ucRx = S_SDCard_SendCMD(CMD41, 0x40000000, 0x01);
				}
				while(ucRx && usReTryNum--);
				if(usReTryNum&&S_SDCard_SendCMD(CMD58, 0, 0x01)==0)
				{
					for(index = 0; index < 4; index++)
						ucRxBuffer[index] = Drv_SPI_TransmitReceive(&s_tSPI, 0xFF);
					
					if(ucRxBuffer[0] & 0x40)
						s_ucSdType=SD_TYPE_V2HC;    
					else 
						s_ucSdType=SD_TYPE_V2;   
				}
			}
		}
		
		else
		{
			S_SDCard_SendCMD(CMD55,0,0x01);		
			ucRx = S_SDCard_SendCMD(CMD41, 0, 0x01);	
			if(ucRx <= 1)
			{		
				s_ucSdType = SD_TYPE_V1;
				usReTryNum = 0xFFFE;
				do 
				{
					S_SDCard_SendCMD(CMD55, 0, 0x01);	
					ucRx = S_SDCard_SendCMD(CMD41, 0, 0x01);
				}while(ucRx && usReTryNum--);
			}
			else
			{
				s_ucSdType = SD_TYPE_MMC;
				usReTryNum = 0xFFFE;
				do 
				{											    
					ucRx = S_SDCard_SendCMD(CMD1,0,0x01);
				}while(ucRx && usReTryNum--);
			}
			
			if(usReTryNum==0 || S_SDCard_SendCMD(CMD16, 512, 0x01) != 0)
				s_ucSdType = SD_TYPE_ERR;
		}
	}
	
	S_SDCard_WaitStop();
	Drv_SPI_SpeedConfig(&s_tSPI, 1);
	if(s_ucSdType)
	{
		return 0;
	}
	else 
		if(ucRx) 
			return ucRx; 	
	
	return 0XAA;		
}

/**
 * @brief SD卡的SPI初始化
 * @param _tSPI-spi结构体
 * @retval Null
*/
uint8_t OCD_SDCard_SPIInit(tagSPI_T _tSPI)
{
	s_tSPI = _tSPI;
	
	return S_SDCard_Init();
}

static uint8_t S_SDCard_GetResponse(uint8_t _ucAck)
{
	int lCnt = 0XFFFF; 	
	
	while ((Drv_SPI_TransmitReceive(&s_tSPI, 0XFF) != _ucAck) && lCnt)
		lCnt--;  
		
	if (lCnt==0)
		return 0xff;
	else 
		return  0;
}

static uint8_t S_SDCard_ReceiveData(uint8_t *_ucpBuffer, uint16_t _usLength)
{
	if(S_SDCard_GetResponse(0xFE) == 1)
		return 1;
	
    while(_usLength--)
    {
        *_ucpBuffer = Drv_SPI_TransmitReceive(&s_tSPI, 0xFF);
        _ucpBuffer++;
    }
	
    Drv_SPI_TransmitReceive(&s_tSPI, 0xFF);
    Drv_SPI_TransmitReceive(&s_tSPI, 0xFF);	
	
    return 0;
}

static uint8_t S_SDCard_ReadDisk(uint8_t *_ucpBuffer, uint32_t _ulSec, uint8_t _ucCnt)
{
	uint8_t ucRx;
	
	if(s_ucSdType!=SD_TYPE_V2HC)
		_ulSec <<= 9;
	
	if(_ucCnt==1)
	{
		ucRx = S_SDCard_SendCMD( CMD17, _ulSec, 0x01);
		if(ucRx == 0)
		{
			ucRx = S_SDCard_ReceiveData( _ucpBuffer, 512);
		}
	}
	else
	{
		ucRx = S_SDCard_SendCMD( CMD18, _ulSec, 0x01);
		do
		{
			ucRx = S_SDCard_ReceiveData( _ucpBuffer, 512);	 
			_ucpBuffer += 512;  
		}
		while(--_ucCnt && ucRx==0);
		
		S_SDCard_SendCMD( CMD12, 0, 0x01);	
	}   
	
	S_SDCard_WaitStop();
	return ucRx;
}

static uint8_t S_SDCard_SendBlock(uint8_t *_ucpBuffer, uint8_t _ucCmd)
{
	uint16_t t;
	
	if(S_SDCard_Wait()==1)
		return 1;
		
	Drv_SPI_TransmitReceive(&s_tSPI, _ucCmd);
		
	if(_ucCmd != 0xFD)
	{
		for(t = 0; t < 512; t++)
			Drv_SPI_TransmitReceive(&s_tSPI, _ucpBuffer[t]);
			
		Drv_SPI_TransmitReceive(&s_tSPI, 0xFF);
		Drv_SPI_TransmitReceive(&s_tSPI, 0xFF);
		t = Drv_SPI_TransmitReceive(&s_tSPI, 0xFF);
			
		if((t & 0x1F) != 0x05)
			return 2;							  					    
	}		
		
	return 0;
}

static uint8_t S_SDCard_WriteDisk(uint8_t *_ucpBuffer, uint32_t _ulSec, uint8_t _ucCnt)
{
	uint8_t ucRx=0;
	
	if(s_ucSdType != SD_TYPE_V2HC)
		_ulSec <<= 9;
	
	if(_ucCnt == 1)
	{
		ucRx=S_SDCard_SendCMD( CMD24, _ulSec, 0x01);
		if(!ucRx)
		{
			ucRx=S_SDCard_SendBlock( _ucpBuffer,0xFE);  
		}
	}
	else
	{
		S_SDCard_SendCMD( CMD55, 0, 0x01);	
		S_SDCard_SendCMD( CMD23, _ucCnt, 0x01);
		ucRx=S_SDCard_SendCMD( CMD25, _ulSec, 0x01);
		
		if(!ucRx)
		{
			do
			{
				ucRx = S_SDCard_SendBlock( _ucpBuffer,0xFC);
				_ucpBuffer += 512;  
			}while(--_ucCnt && ucRx==0);
						
			ucRx = S_SDCard_SendBlock( 0,0xFD);
		}
	}   
	
	S_SDCard_WaitStop();
	return ucRx;
}

/* FATFS文件系统 start */
#ifdef FRAMEWORK_FATFS_ENABLE

#define SD_CARD	 0  /* Example: Map MMC/SD card to physical drive 1 */

#define FLASH_SECTOR_SIZE 	512		/* 单个扇区大小 */
#define FLASH_BLOCK_SIZE	8		/* 每片Block有8个扇区 */
#define FLASH_SECTOR_COUNT	2048*10	/* 指定扇区数量-->FATFS可用大小为10M */

DSTATUS disk_initialize (
	BYTE _drv				/* Physical drive nmuber (0..) */
)
{
	return RES_OK; //初始化成功
}

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	return RES_OK;
}

DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *ucRxBuffer,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..255) */
)
{	
	uint8_t res=0; 
    if (!count)
		return RES_PARERR;	/* count不能等于0，否则返回参数错误 */	
	
	switch(drv)
	{
		case SD_CARD:		/* SD卡 */
			res = S_SDCard_ReadDisk( ucRxBuffer, sector, count);	 

		 	if(res)			/* STM32 SPI的bug,在sd卡操作失败的时候如果不执行下面的语句,可能导致SPI读写异常 */
			{
				Drv_SPI_SpeedConfig(&s_tSPI, 0);
				Drv_SPI_TransmitReceive(&s_tSPI, 0XFF);		/* 提供额外的8个时钟 */
				Drv_SPI_SpeedConfig(&s_tSPI, 1);
			}
		break;
		
		default:
			res=1; 
	}
	
	/* 处理返回值，将SPI_SD_driver.c的返回值转成ff.c的返回值 */
    if(res==0x00)
		return RES_OK;	 
    else 
		return RES_ERROR;
}

DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *ucRxBuffer,	        /* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..255) */
)
{
	unsigned char res=0;
    if (!count)
		return RES_PARERR;	/* count不能等于0，否则返回参数错误 */
	
	switch(drv)
	{
		case SD_CARD:		/* SD卡 */
			res=S_SDCard_WriteDisk( (unsigned char*)ucRxBuffer,sector,count);
			while(res)
			{
				S_SDCard_Init();
				res=S_SDCard_WriteDisk( (unsigned char*)ucRxBuffer,sector,count);
			}
			break;
			
		default:
			res=1; 
	}
    //处理返回值，将SPI_SD_driver.c的返回值转成ff.c的返回值
    if(res == 0x00)
		return RES_OK;	 
    else 
		return RES_ERROR;		 
}

DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *ucRxBuffer		/* Buffer to send/receive control data */
)
{	
	DRESULT res;						  			     
	if(drv==SD_CARD)		/* SD卡 */
	{
	    switch(ctrl)
	    {
		    case CTRL_SYNC:
				return RES_OK; 
		    case GET_SECTOR_COUNT:
		        *(unsigned int*)ucRxBuffer = FLASH_SECTOR_COUNT;		/* 获取扇区数 */
		        res = RES_OK;
		        break;	 
		    case GET_SECTOR_SIZE:
		        *(unsigned short*)ucRxBuffer = FLASH_SECTOR_SIZE;		/* 获取扇区大小 */
		        res = RES_OK;
		        break;	 
		    case GET_BLOCK_SIZE:
				*(unsigned short*)ucRxBuffer = FLASH_BLOCK_SIZE;		/* 获取块大小 */
		        res = RES_OK;
		        break;
		    default:
		        res = RES_PARERR;
		        break;
	    }
	}
	else 
		res=RES_ERROR;//其他的不支持
	return 
	  res;
}

DWORD get_fattime (void)
{				 
	return 0;
}
#endif
/* FATFS文件系统 end */
