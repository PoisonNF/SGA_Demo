#include "ocd_ads4111.h"

/****************************************************************************

* Sigma团队

* 文件名: ocd_ads4111.c

* 内容简述：ads4111模块文件

* 文件历史：

* 版本号	日期		作者		说明

* 1.0.0a 	2020-02-22	李环宇		创建该文件

****************************************************************************/

/**
 * @brief ADS4111延时函数
 * @retval Null
*/
static void S_ADS4111_Delay(void)
{
	Drv_Delay_Ms(10);
}

const uint8_t s_ucByteSize = 16;
/**
 * @brief ADS4111通信读写函数
 * @param _tADS4111-ADS4111结构体指针
 * @param _ucpTx-发送数组
 * @param _ucSize-发送长度
 * @retval uint8_t
*/
static uint8_t S_ADS4111WriteAndRead(tagAD4111_T *_tADS4111, uint8_t *_ucpTx, uint8_t _ucSize)
{
	uint8_t index;
	
	/* 如果发送长度大于16 */
	if(_ucSize > s_ucByteSize)
		return ERROR;
	
	/* 通过SPI发送 */
	for(index = 0; index < _ucSize; index++)
	{
		_ucpTx[index] = Drv_SPI_TransmitReceive(&_tADS4111->tSPI, _ucpTx[index]);
	}
	
	return OK;
}

/**
 * @brief Computes the CRC checksum for a data buffer.
 * @param _ucpBuf-Data buffer
 * @param _ucSize-Data buffer size in bytes
 * @return uint8_t-Returns the computed CRC checksum.
*/
uint8_t S_ADS4111_CRC8(uint8_t *_ucpBuf, uint8_t _ucSize)
{
	uint8_t i   = 0;
	uint8_t crc = 0;

	while(_ucSize) 
	{
		for(i = 0x80; i != 0; i >>= 1) 
		{
			if(((crc & 0x80) != 0) != ((*_ucpBuf & i) != 0)) 
			{
				/* MSB of CRC register XOR input Bit from Data */
				crc <<= 1;
				crc ^= AD411X_CRC8_POLYNOMIAL_REPRESENTATION;
			} 
			else 
			{
				crc <<= 1;
			}
		}
		_ucpBuf++;
		_ucSize--;
	}
	return crc;
}

/**
 * @brief Computes the XOR checksum for a data buffer.
 * @param _ucpBuf-Data buffer
 * @param _ucSize-Data buffer size in bytes
 * @return uint8_t-Returns the computed XOR checksum.
*/
static uint8_t S_ADS4111_XOR8(uint8_t *_ucpBuf, uint8_t _ucSize)
{
	uint8_t ucRes = 0;

	while(_ucSize) 
	{
		ucRes ^= *_ucpBuf;
		_ucpBuf++;
		_ucSize--;
	}
	return ucRes;
}

/**
 * @brief  Searches through the list of registers of the driver instance and
 *         retrieves a pointer to the register that matches the given address.
 * @param _tAD4111-The handler of the instance of the driver.
 * @param _ucAddr-The address to be used to find the register.
 * @return tagAD4111Reg_T-A pointer to the register if found or 0.
*/
static tagAD4111Reg_T *S_ADS4111_GetReg(tagAD4111_T *_tAD4111,uint8_t _ucAddr)
{
	uint8_t index;
	tagAD4111Reg_T *tReg = 0;

	if (!_tAD4111 || !_tAD4111->tADS4111Reg)
		return 0;

	for (index = 0; index < _tAD4111->ucRegNum; index++) 
	{
		if(_tAD4111->tADS4111Reg[index].ulAddr == _ucAddr) 
		{
			tReg = &_tAD4111->tADS4111Reg[index];
			break;
		}
	}

	return tReg;
}

/**
 * @brief Updates the CRC settings.
 * @param _tAD4111-The handler of the instance of the driver.
 * @return int32_t-Returns 0 for success or negative error code.
*/
static int32_t S_ADS4111_UpdateCRCSetting(tagAD4111_T *_tAD4111)
{
	tagAD4111Reg_T *tReg;

	if(!_tAD4111 || !_tAD4111->tADS4111Reg)
		return INVALID_VAL;

	tReg = S_ADS4111_GetReg(_tAD4111, AD411X_IFMODE_REG);
	if (!tReg)
		return INVALID_VAL;

	/* Get CRC State */
	if(AD411X_IFMODE_REG_CRC_STAT(tReg->ulValue))
	{
		_tAD4111->tCrcMode = AD411X_USE_CRC;
	} 
	else if(AD411X_IFMODE_REG_XOR_STAT(tReg->ulValue))
	{
		_tAD4111->tCrcMode = AD411X_USE_XOR;
	} 
	else
	{
		_tAD4111->tCrcMode = AD411X_DISABLE;
	}

	return 0;
}

const uint8_t s_ucResetBuf[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
/**
 * @brief ADS4111复位函数
 * @param _tADS4111-ADS4111结构体指针
 * @retval uint8_t
*/
static uint8_t S_ADS4111Reset(tagAD4111_T *_tADS4111)
{
	uint8_t ucRes;
	
	if(!_tADS4111)
		return INVALID_VAL;
	
	ucRes = S_ADS4111WriteAndRead(_tADS4111, (uint8_t*)s_ucResetBuf, sizeof(s_ucResetBuf));
	
	return ucRes;
}

const uint8_t s_ucRegSize = 8;
/**
 * @brief ADS4111写寄存器
 * @param _tADS4111-ADS4111结构体指针
 * @param _ucAddr-写寄存器的地址
 * @retval uint8_t
*/
static uint8_t S_ADS4111WriteRegister(tagAD4111_T *_tADS4111, uint8_t _ucAddr)
{
	uint8_t ucRes;
	uint8_t index;
	int32_t ulRegValue = 0;
	uint8_t ucWrRegBuf[s_ucRegSize] = {0};
	tagAD4111Reg_T *tReg;
	uint8_t ucCrc8 = 0;
	
	if(!_tADS4111)
		return INVALID_VAL;
	
	tReg = S_ADS4111_GetReg(_tADS4111, _ucAddr);
	
	if (!tReg)
		return INVALID_VAL;
	
	ucWrRegBuf[0] = AD411X_COMM_REG_WEN | AD411X_COMM_REG_WR | AD411X_COMM_REG_RA(tReg->ulAddr);
	
	/* Fill the write buffer */
	ulRegValue = tReg->ulValue;
	for(index = 0; index < tReg->ulSize; index++)
	{
		ucWrRegBuf[tReg->ulSize - index] = ulRegValue & 0xFF;
		ulRegValue >>= 8;
	}
	
	/* Compute the CRC */
	if(_tADS4111->tCrcMode != AD411X_DISABLE) 
	{
		ucCrc8 = S_ADS4111_CRC8(ucWrRegBuf, tReg->ulSize + 1);
		ucWrRegBuf[tReg->ulSize + 1] = ucCrc8;
	}
	
	/* Write data to the _tAD4111 */
	ucRes = S_ADS4111WriteAndRead(_tADS4111,
			ucWrRegBuf,
			(_tADS4111->tCrcMode != AD411X_DISABLE) ?
			tReg->ulSize + 2 : tReg->ulSize + 1);

	return ucRes;
}

/**
 * @brief Reads the value of the specified register.
 * @param _tAD4111-The handler of the instance of the driver.
 * @param _ucAddr-The address of the register to be read. The value will be stored
 *         inside the register structure that holds info about this register.
 * @return Returns 0 for success or negative error code.
*/
static int32_t S_ADS4111_ReadRegister(tagAD4111_T *_tAD4111, uint8_t _ucAddr)
{
	uint8_t ucRes			= 0;
	uint8_t ucpBuffer[8]	= {0, 0, 0, 0, 0, 0, 0, 0};
	uint8_t index			= 0;
	uint8_t ucCheck8			= 0;
	uint8_t ucpMsgBuf[8]		= {0, 0, 0, 0, 0, 0, 0, 0};
	tagAD4111Reg_T *tReg;

	if(!_tAD4111)
		return INVALID_VAL;

	tReg = S_ADS4111_GetReg(_tAD4111, _ucAddr);
	if (!tReg)
		return INVALID_VAL;

	/* Build the Command word */
	ucpBuffer[0] = AD411X_COMM_REG_WEN | AD411X_COMM_REG_RD | AD411X_COMM_REG_RA(tReg->ulAddr);

	/* Read data from the _tAD4111 */
	ucRes = S_ADS4111WriteAndRead(_tAD4111,
				 ucpBuffer,
				 ((_tAD4111->tCrcMode != AD411X_DISABLE) ? tReg->ulSize + 1
				  : tReg->ulSize) + 1);
	
	/* Check the CRC */
	if(_tAD4111->tCrcMode == AD411X_USE_CRC) 
	{
		ucpMsgBuf[0] = AD411X_COMM_REG_WEN | AD411X_COMM_REG_RD | AD411X_COMM_REG_RA(tReg->ulAddr);
		for(index = 1; index < tReg->ulSize + 2; ++index) 
		{
			ucpMsgBuf[index] = ucpBuffer[index];
		}
		ucCheck8 = S_ADS4111_CRC8(ucpMsgBuf, tReg->ulSize + 2);
	}
	if(_tAD4111->tCrcMode == AD411X_USE_XOR) 
	{
		ucpMsgBuf[0] = AD411X_COMM_REG_WEN | AD411X_COMM_REG_RD | AD411X_COMM_REG_RA(tReg->ulAddr);
		for(index = 1; index < tReg->ulSize + 2; ++index) 
		{
			ucpMsgBuf[index] = ucpBuffer[index];
		}
		ucCheck8 = S_ADS4111_XOR8(ucpMsgBuf, tReg->ulSize + 2);
	}

	if(ucCheck8 != 0) 
	{
		/* ReadRegister checksum failed. */
		return COMM_ERR;
	}

	/* Build the result */
	tReg->ulValue = 0;
	for(index = 1; index < tReg->ulSize + 1; index++) 
	{
		tReg->ulValue <<= 8;	
		tReg->ulValue += ucpBuffer[index];
	}

	return ucRes;
}

/**
 * @brief ADS4111自检函数
 * @param _tADS4111-ADS4111结构体指针
 * @retval Null
*/
static void S_ADS4111_Check(tagAD4111_T *_tAS4111)
{
	uint8_t ucRes;
	tagAD4111Reg_T *tReg;
	
	tReg = S_ADS4111_GetReg(_tAS4111, AD411X_GPIOCON_REG);
	while (tReg && tReg->ulAddr != AD411X_OFFSET0_REG) 
	{
		if (tReg->ulAddr == AD411X_ID_REG) 
		{
			tReg ++;
			continue;
		}
		
		S_ADS4111_Delay();
		
		ucRes = S_ADS4111WriteRegister(_tAS4111, tReg->ulAddr);
		if (ucRes > 0)
			break;
		tReg ++;
	}
}

/**
 * @brief ADS4111初始化函数
 * @param _tADS4111-ADS4111结构体指针
 * @retval int8_t
*/
int8_t OCD_ADS4111Init(tagAD4111_T *_tAS4111)
{
	uint8_t ucRes;
	
	Drv_SPI_Init(&_tAS4111->tSPI);
	S_ADS4111_Delay();
	
	_tAS4111->tADS4111Reg = tAD4111Reg;
	_tAS4111->ucRegNum = sizeof(tAD4111Reg)/sizeof(tagAD4111_T);
	_tAS4111->tCrcMode = AD411X_DISABLE;
	
	S_ADS4111Reset(_tAS4111);		/* 复位 */
	S_ADS4111_Delay();
	
	ucRes = S_ADS4111WriteRegister(_tAS4111, AD411X_ADCMODE_REG);		/* Initialize ADC mode register. */
	S_ADS4111_Delay();
	ucRes = S_ADS4111WriteRegister(_tAS4111, AD411X_IFMODE_REG);		/* Initialize Interface mode register. */
	S_ADS4111_Delay();
	ucRes = S_ADS4111_UpdateCRCSetting(_tAS4111);						/* Get CRC State */
	
	S_ADS4111_Check(_tAS4111);
	S_ADS4111_Delay();
	
	ucRes = S_ADS4111WriteRegister(_tAS4111, AD411X_GAIN0_REG);
	S_ADS4111_Delay();
	ucRes = S_ADS4111_ReadRegister(_tAS4111, AD411X_ID_REG);
	S_ADS4111_Delay();
	
	return ucRes;
}
