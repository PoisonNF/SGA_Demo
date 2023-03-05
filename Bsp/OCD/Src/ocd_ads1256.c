/****************************************************************************

* Sigma团队

* 文件名: ocd_ads1256.c

* 内容简述：ads1256模块文件

* 文件历史：

* 版本号	日期		作者		说明

* 1.0.0a 	2020-02-22	李环宇		创建该文件

****************************************************************************/
#include "ocd_ads1256.h"

/**
 * @brief ADS1256I/O初始化函数
 * @param _tADS1256-ADS1256结构体指针
 * @retval Null
*/
static void S_ADS1256_DRDY_Init(tagADS1256_T *_tADS1256)
{
	Drv_GPIO_Init(&_tADS1256->tDrdyGPIO, 1);
}

/**
 * @brief ADS1256设置命令函数
 * @param _tADS1256-ADS1256结构体指针
 * @param _ucCmd-设置的命令
 * @retval Null
*/
static void S_ADS1256_SetCommand(tagADS1256_T *_tADS1256, uint8_t _ucCmd)
{
	Drv_SPI_TransmitReceive(&_tADS1256->tSPI, _ucCmd);
}

/**
 * @brief ADS1256延时函数
 * @param _ulUs 延时微秒
 * @retval Null
*/
static void S_ADS1256_DelayUs(uint32_t _ulUs)
{
	int i,j;

	for(i = 0; i < _ulUs; i++)
		for(j = 0; j < 12; j++);
}

/**
 * @brief ADS1256设置校准命令函数
 * @param _tADS1256-ADS1256结构体指针
 * @retval Null
*/
static void S_ADS1256_SetCalibrationCommand(tagADS1256_T *_tADS1256)
{
	S_ADS1256_SetCommand(_tADS1256, ADS1256_CMD_SELFCAL);
	
	/*DRDY goes high at the beginning of the calibration*/
	while(!Drv_GPIO_Read(&_tADS1256->tDrdyGPIO));
	
	/*DRDY goes low after the calibration completes and settled data is ready*/
	while(Drv_GPIO_Read(&_tADS1256->tDrdyGPIO));
}

/**
 * @brief ADS1256写寄存器函数
 * @param _tADS1256-ADS1256结构体指针
 * @param _ucRegAddr-寄存器地址
 * @param _ucRegValue-寄存器数值
 * @retval Null
*/
static void S_ADS1256_WriteReg(tagADS1256_T *_tADS1256, uint8_t _ucRegAddr, uint8_t _ucRegValue)
{
	while(Drv_GPIO_Read(&_tADS1256->tDrdyGPIO));	/* DRY为低时才能写寄存器 */
	
	/*1st Command Byte: 0101 rrrr where rrrr is the address to the first register to be written.*/
	Drv_SPI_Transmit(&_tADS1256->tSPI, ADS1256_CMD_WREG | _ucRegAddr );
	
	/*2nd Command Byte: 0000 nnnn where nnnn is the number of bytes to be written*/
	Drv_SPI_Transmit(&_tADS1256->tSPI,0x00);	/*here nnnn is  0 = 1-1 */
	
	/*Data Byte(s): data to be written to the registers.*/
	Drv_SPI_Transmit(&_tADS1256->tSPI, _ucRegValue);
	
	S_ADS1256_DelayUs(6);
}

/**
 * @brief ADS1256设置单通道函数
 * @param _tADS1256-ADS1256结构体指针
 * @param _ucChanel-通道号
 * @retval Null
*/
static void S_ADS1256_SetSingleChannel(tagADS1256_T *_tADS1256, uint8_t _ucChanel)
{
	if(_ucChanel > 7)	
		return;

	/*for single-end analog input mode,ch value set postive analog input
	**while negative input channel(AINN)is set with AINCOM in default(bit3 = 1)			
	*/

	S_ADS1256_WriteReg(_tADS1256, ADS1256_REG_MUX ,((_ucChanel<<4) | (1<<3)));
}

/**
 * @brief ADS1256设置同步命令函数
 * @param _tADS1256-ADS1256结构体指针
 * @retval Null
*/
static void S_ADS1256_SetSynCommand(tagADS1256_T *_tADS1256)
{
	Drv_SPI_Transmit(&_tADS1256->tSPI, ADS1256_CMD_SYNC);
	
	Drv_SPI_Transmit(&_tADS1256->tSPI, ADS1256_CMD_WAKEUP);
}

/**
 * @brief ADS1256检查函数
 * @param _tADS1256-ADS1256结构体指针
 * @retval uint8_t
*/
uint8_t OCD_ADS1256_Check(tagADS1256_T *_tADS1256)
{
	uint8_t ucRxData[4] ;
	uint8_t index;
	
	while(Drv_GPIO_Read(&_tADS1256->tDrdyGPIO));	
	/* 1st Command Byte: 0001 rrrr where rrrr is the address of the first register to ucRxData */
	Drv_SPI_TransmitReceive(&_tADS1256->tSPI, ADS1256_CMD_RREG | FIRST_REG_ADDR);
	
	/* 2nd Command Byte: 0000 nnnn where nnnn is the number of bytes to ucRxData – 1 */
	Drv_SPI_TransmitReceive(&_tADS1256->tSPI,0x03);	/* here nnnn is  3 = 4-1 ,ucRxData 4 register */
	
	/* notice the timing definition of T6 */
	S_ADS1256_DelayUs(10);
			
	for(index = 0;index < 4; index++)
		ucRxData[index] = Drv_SPI_TransmitReceive(&_tADS1256->tSPI,0xFF);
			
	/* Judge if ads1256 was configed correct */
	if((ucRxData[0] & (1<<2) & (1<<2)))//(ptr->ucBufferStatus&(0<<1)) &&
		return 1; /* config the ads1256 success */
	else 
		return 0; /* false */	
}

/**
 * @brief ADS1256读取状态函数
 * @param _tADS1256-ADS1256结构体指针
 * @param _pAddr-读取的地址
 * @retval uint8_t
*/
uint8_t OCD_ADS1256_ReadStatus(tagADS1256_T *_tADS1256, tagADS1256Status_T *_pAddr)
{
	uint8_t ucRxData[4] ;
	uint8_t index;
	
	while(Drv_GPIO_Read(&_tADS1256->tDrdyGPIO));	
	/* 1st Command Byte: 0001 rrrr where rrrr is the address of the first register to ucRxData */
	Drv_SPI_TransmitReceive(&_tADS1256->tSPI, ADS1256_CMD_RREG | FIRST_REG_ADDR);
	
	/* 2nd Command Byte: 0000 nnnn where nnnn is the number of bytes to ucRxData – 1 */
	Drv_SPI_TransmitReceive(&_tADS1256->tSPI,0x03);	/* here nnnn is  3 = 4-1 ,ucRxData 4 register */
	
	/* notice the timing definition of T6 */
	S_ADS1256_DelayUs(10);
			
	for(index = 0;index < 4; index++)
		ucRxData[index] = Drv_SPI_TransmitReceive(&_tADS1256->tSPI,0xFF);
	
	/* Analyse the register value */
	_pAddr->ucID = (ucRxData[0]>>4) ;
	_pAddr->ucDataOrder = (ucRxData[0] & (1<<3));
	_pAddr->ucAutoCalibration = (ucRxData[0] & (1<<2));
	_pAddr->ucBufferStatus = (ucRxData[0] & (1<<1));
			
	_pAddr->ucMuxValue = ucRxData[1];
	_pAddr->ucPgaValue = (ucRxData[2] & 0x07);
	_pAddr->ucDataRate = ucRxData[3];
			
	/* Judge if ads1256 was configed correct */
	if((_pAddr->ucAutoCalibration & (1<<2))) //(ptr->ucBufferStatus&(0<<1)) &&
		return 1; /* config the ads1256 success */
	else 
		return 0; /* false */		
}

/**
 * @brief ADS1256读取一个通道电压函数
 * @param _tADS1256-ADS1256结构体指针
 * @param _ucChannel-读取的通道
 * @retval float
*/
float OCD_ADS1256_ReadOneChannelVoltage(tagADS1256_T *_tADS1256, uint8_t _ucChannel)
{
	uint32_t ulDigitValue = 0 ; 
	float fResVoltage = 0.0;
	
	/* select channel */
	while(Drv_GPIO_Read(&_tADS1256->tDrdyGPIO));	
	S_ADS1256_SetSingleChannel(_tADS1256, _ucChannel);
	
	while(Drv_GPIO_Read(&_tADS1256->tDrdyGPIO));	
	S_ADS1256_SetSynCommand(_tADS1256);	
		
	/* After a synchronization operation,DRDY stays high until valid data is ready */
	while(!Drv_GPIO_Read(&_tADS1256->tDrdyGPIO));	
	
	/* Issue command:RDATA after DRDY goes low to ucRxData a single conversion result */
	while(Drv_GPIO_Read(&_tADS1256->tDrdyGPIO));		
	S_ADS1256_SetCommand(_tADS1256, ADS1256_CMD_RDATA);
	
	/* Timing sequence:T6 */
	S_ADS1256_DelayUs(10);

	ulDigitValue = (Drv_SPI_TransmitReceive(&_tADS1256->tSPI, 0xFF)<<16);
	ulDigitValue += (Drv_SPI_TransmitReceive(&_tADS1256->tSPI, 0xFF)<<8);
	ulDigitValue += Drv_SPI_TransmitReceive(&_tADS1256->tSPI, 0xFF);
		
	if((ulDigitValue&0x800000)==0x800000)
	{
		/* 负数情况 */
		fResVoltage = (0x7fffff-(ulDigitValue-0x800000)+1)*0.00000059032;	
	}
	else
	{
		/* 正数情况 */
		fResVoltage = (ulDigitValue)*0.00000059032;
	}
		
	return fResVoltage;				
}

/**
 * @brief ADS1256初始化函数
 * @param _tADS1256-ADS1256结构体指针
 * @retval Null
*/
void OCD_ADS1256_Init(tagADS1256_T *_tADS1256)
{
	S_ADS1256_DRDY_Init(_tADS1256);
	
	Drv_SPI_Init(&_tADS1256->tSPI); 
	
	S_ADS1256_SetCommand(_tADS1256, ADS1256_CMD_RESET);
	S_ADS1256_DelayUs(100000);	/* 等待重启 */
	
	/* it is strongly recommended to perform an additional self-calibration by issuing 
	the SELFCAL command after the power supplies and voltage reference have had time
	to settle to their final values in pdf.27 */
	S_ADS1256_SetCalibrationCommand(_tADS1256);
	while(Drv_GPIO_Read(&_tADS1256->tDrdyGPIO));
	
	/* Most Significant Bit First,Auto-Calibration Enabled,Buffer disabled:11110101b */
	S_ADS1256_WriteReg(_tADS1256, ADS1256_REG_STATUS ,0xF5);
	while(Drv_GPIO_Read(&_tADS1256->tDrdyGPIO));

	/* PGA=1 */
	S_ADS1256_WriteReg(_tADS1256, ADS1256_REG_ADCON, 0x00);
	while(Drv_GPIO_Read(&_tADS1256->tDrdyGPIO));

	/* select channle AIN0 as initial analog input */
	S_ADS1256_SetSingleChannel(_tADS1256, 0);
	while(Drv_GPIO_Read(&_tADS1256->tDrdyGPIO));

	/* set data rate as sps as initial rate */
	//ADS1256_WriteReg(REG_DRATE,RATE_10SPS);		
	S_ADS1256_WriteReg(_tADS1256, ADS1256_REG_DRATE, ADS1256_RATE_100SPS);	
		
	S_ADS1256_SetCalibrationCommand(_tADS1256);
			
	/* After changing the PGA, data rate, buffer status, writing to the OFC or FSC registers, 
	and enabling or disabling the	sensor detect circuitry,perform a synchronization operation 
	to force DRDY high. It will stay high until valid data is ready */
	S_ADS1256_SetSynCommand(_tADS1256);	
}

