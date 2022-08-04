/****************************************************************************

* Sigma团队

* 文件名: ut_ads1256.c

* 内容简述：ads1256模块文件

* 文件历史：

* 版本号	日期		作者		说明

* 1.0.0a 	2020-02-22	李环宇		创建该文件

****************************************************************************/
#include "ocd_ads1256.h"

/* register address,follow with reset value */
#define ADS1256_REG_STATUS  0 /* x1H */
#define ADS1256_REG_MUX     1 /* 01H */
#define ADS1256_REG_ADCON   2 /* 20H */
#define ADS1256_REG_DRATE   3 /* F0H */
#define ADS1256_REG_IO      4 /* E0H */
#define ADS1256_REG_OFC0    5 /* xxH */
#define ADS1256_REG_OFC1    6 /* xxH */
#define ADS1256_REG_OFC2    7 /* xxH */
#define ADS1256_REG_FSC0    8 /* xxH */
#define ADS1256_REG_FSC1    9 /* xxH */
#define ADS1256_REG_FSC2    10 /* xxH */

/*ads1256 command definition */
#define ADS1256_CMD_WAKEUP   0x00 /* Completes SYNC and Exits Standby Mode 0000  0000 (00h) */
#define ADS1256_CMD_RDATA    0x01 /* Read Data 0000  0001 (01h) */
#define ADS1256_CMD_RDATAC   0x03 /* Read Data Continuously 0000   0011 (03h) */
#define ADS1256_CMD_SDATAC   0x0F /* Stop Read Data Continuously 0000   1111 (0Fh) */
#define ADS1256_CMD_RREG     0x10 /* Read from REG rrr 0001 rrrr (1xh) */
#define ADS1256_CMD_WREG     0x50 /* Write to REG rrr 0101 rrrr (5xh) */
#define ADS1256_CMD_SELFCAL  0xF0 /* Offset and Gain Self-Calibration 1111    0000 (F0h) */
#define ADS1256_CMD_SELFOCAL 0xF1 /* Offset Self-Calibration 1111    0001 (F1h) */
#define ADS1256_CMD_SELFGCAL 0xF2 /* Gain Self-Calibration 1111    0010 (F2h) */
#define ADS1256_CMD_SYSOCAL  0xF3 /* System Offset Calibration 1111   0011 (F3h) */
#define ADS1256_CMD_SYSGCAL  0xF4 /* System Gain Calibration 1111    0100 (F4h) */
#define ADS1256_CMD_SYNC     0xFC /* Synchronize the A/D Conversion 1111   1100 (FCh) */
#define ADS1256_CMD_STANDBY  0xFD /* Begin Standby Mode 1111   1101 (FDh) */
#define ADS1256_CMD_RESET    0xFE /* Reset to Power-Up Values 1111   1110 (FEh) */

/* ads1256 sps enum ,range from 2.5 to 30,000 sps */
#define ADS1256_RATE_30000SPS	0xF0	/* default value */
#define ADS1256_RATE_15000SPS	0xE0
#define ADS1256_RATE_7500SPS	0xD0
#define ADS1256_RATE_3750SPS	0xC0
#define ADS1256_RATE_2000SPS	0xB0
#define ADS1256_RATE_1000SPS	0xA1
#define ADS1256_RATE_500SPS		0x92
#define ADS1256_RATE_100SPS		0x82
#define ADS1256_RATE_60SPS		0x72
#define ADS1256_RATE_50SPS		0x63
#define ADS1256_RATE_30SPS		0x53
#define ADS1256_RATE_25SPS		0x43
#define ADS1256_RATE_15SPS		0x33
#define ADS1256_RATE_10SPS		0x23
#define ADS1256_RATE_5SPS		0x13
#define ADS1256_RATE_2O5SPS		0x03

#define ADS1256_MUXP_AIN0   0x00 
#define ADS1256_MUXP_AIN1   0x10 
#define ADS1256_MUXP_AIN2   0x20 
#define ADS1256_MUXP_AIN3   0x30 
#define ADS1256_MUXP_AIN4   0x40 
#define ADS1256_MUXP_AIN5   0x50 
#define ADS1256_MUXP_AIN6   0x60 
#define ADS1256_MUXP_AIN7   0x70 
#define ADS1256_MUXP_AINCOM 0x80

#define ADS1256_MUXN_AIN0   0x00 
#define ADS1256_MUXN_AIN1   0x01 
#define ADS1256_MUXN_AIN2   0x02 
#define ADS1256_MUXN_AIN3   0x03 
#define ADS1256_MUXN_AIN4   0x04 
#define ADS1256_MUXN_AIN5   0x05 
#define ADS1256_MUXN_AIN6   0x06 
#define ADS1256_MUXN_AIN7   0x07 
#define ADS1256_MUXN_AINCOM 0x08 

#define FIRST_REG_ADDR		0x00
			
static void S_ADS1256_DRDY_Init(tagADS1256_T *_tADS1256)
{
	Drv_GPIO_Init(&_tADS1256->tDrdyGPIO, 1);
}

static void S_ADS1256_SetCommand(tagADS1256_T *_tADS1256, uint8_t _ucCmd)
{
	Drv_SPI_TransmitReceive(&_tADS1256->tSPI, _ucCmd);
}

static void S_ADS1256_DelayUs(uint32_t _ulUs)
{
	int i,j;
	for(i = 0; i < _ulUs; i++)
		for(j = 0; j < 12; j++);
	
//	Dr_Delay_Us(_nus);
}

static void S_ADS1256_SetCalibrationCommand(tagADS1256_T *_tADS1256)
{
	S_ADS1256_SetCommand(_tADS1256, ADS1256_CMD_SELFCAL);
	
	/*DRDY goes high at the beginning of the calibration*/
	while(!Drv_GPIO_Read(&_tADS1256->tDrdyGPIO));
	
	/*DRDY goes low after the calibration completes and settled data is ready*/
	while(Drv_GPIO_Read(&_tADS1256->tDrdyGPIO));
}

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

static void S_ADS1256_SetSingleChannel(tagADS1256_T *_tADS1256, uint8_t _ucChanel)
{
	if(_ucChanel > 7)	
		return	;

	/*for single-end analog input mode,ch value set postive analog input
	**while negative input channel(AINN)is set with AINCOM in default(bit3 = 1)			
	*/

	S_ADS1256_WriteReg(_tADS1256, ADS1256_REG_MUX ,((_ucChanel<<4) | (1<<3)));
}

static void S_ADS1256_SetSynCommand(tagADS1256_T *_tADS1256)
{
	Drv_SPI_Transmit(&_tADS1256->tSPI, ADS1256_CMD_SYNC);
	
	Drv_SPI_Transmit(&_tADS1256->tSPI, ADS1256_CMD_WAKEUP);//wakeup command
}

void OCD_ADS1256_Init(tagADS1256_T *_tADS1256)
{
	S_ADS1256_DRDY_Init(_tADS1256);
	
	Drv_SPI_Init(&_tADS1256->tSPI); 
	
	S_ADS1256_SetCommand(_tADS1256, ADS1256_CMD_RESET);
	S_ADS1256_DelayUs(100000);	/* 等待重启 */
	
	/*it is strongly recommended to perform an additional self-calibration by issuing 
	the SELFCAL command after the power supplies and voltage reference have had time
	to settle to their final values in pdf.27*/
	S_ADS1256_SetCalibrationCommand(_tADS1256);
	while(Drv_GPIO_Read(&_tADS1256->tDrdyGPIO));
	
	/*Most Significant Bit First,Auto-Calibration Enabled,Buffer disabled:11110101b */
	S_ADS1256_WriteReg(_tADS1256, ADS1256_REG_STATUS ,0xF5);
	while(Drv_GPIO_Read(&_tADS1256->tDrdyGPIO));

	/*PGA=1 */
	S_ADS1256_WriteReg(_tADS1256, ADS1256_REG_ADCON, 0x00);
	while(Drv_GPIO_Read(&_tADS1256->tDrdyGPIO));

	/*select channle AIN0 as initial analog input*/
	S_ADS1256_SetSingleChannel(_tADS1256, 0);
	while(Drv_GPIO_Read(&_tADS1256->tDrdyGPIO));

	/*set data rate as sps as initial rate*/
	//ADS1256_WriteReg(REG_DRATE,RATE_10SPS);		
	S_ADS1256_WriteReg(_tADS1256, ADS1256_REG_DRATE, ADS1256_RATE_100SPS);	
		
	S_ADS1256_SetCalibrationCommand(_tADS1256);
			
	/*After changing the PGA, data rate, buffer status, writing to the OFC or FSC registers, 
	and enabling or disabling the	sensor detect circuitry,perform a synchronization operation 
	to force DRDY high. It will stay high until valid data is ready*/
	S_ADS1256_SetSynCommand(_tADS1256);	
}

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
			
	/*Judge if ads1256 was configed correct*/
	if(  (ucRxData[0] & (1<<2) & (1<<2)) ) //(ptr->ucBufferStatus&(0<<1)) &&
		return 1; //config the ads1256 success
	else 
		return 0;	//false	
}

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
	
	/*Analyse the register value*/
	_pAddr->ucID = (ucRxData[0]>>4) ;
	_pAddr->ucDataOrder = (ucRxData[0] & (1<<3));
	_pAddr->ucAutoCalibration = (ucRxData[0] & (1<<2));
	_pAddr->ucBufferStatus = (ucRxData[0] & (1<<1));
			
	_pAddr->ucMuxValue = ucRxData[1];
	_pAddr->ucPgaValue = (ucRxData[2] & 0x07);
	_pAddr->ucDataRate = ucRxData[3];
			
	/*Judge if ads1256 was configed correct*/
	if(  (_pAddr->ucAutoCalibration & (1<<2)) ) //(ptr->ucBufferStatus&(0<<1)) &&
		return 1; //config the ads1256 success
	else 
		return 0;	//false			
}

float OCD_ADS1256_ReadOneChannelVoltage(tagADS1256_T *_tADS1256, unsigned char _ch)
{
	uint32_t ulDigitValue = 0 ; 
	float fResVoltage=0.0;
	
	/*select channel*/
	while(Drv_GPIO_Read(&_tADS1256->tDrdyGPIO));	
	S_ADS1256_SetSingleChannel(_tADS1256, _ch);
	
	while(Drv_GPIO_Read(&_tADS1256->tDrdyGPIO));	
	S_ADS1256_SetSynCommand(_tADS1256);	
		
	/*After a synchronization operation,DRDY stays high until valid data is ready*/
	while(!Drv_GPIO_Read(&_tADS1256->tDrdyGPIO));	
	
	/*Issue command:RDATA after DRDY goes low to ucRxData a single conversion result*/
	while(Drv_GPIO_Read(&_tADS1256->tDrdyGPIO));		
	S_ADS1256_SetCommand(_tADS1256, ADS1256_CMD_RDATA);
	
	/*	Timing sequence:T6	*/
	S_ADS1256_DelayUs(10);

	ulDigitValue = (Drv_SPI_TransmitReceive(&_tADS1256->tSPI, 0xFF)<<16);
	ulDigitValue += (Drv_SPI_TransmitReceive(&_tADS1256->tSPI, 0xFF)<<8);
	ulDigitValue += Drv_SPI_TransmitReceive(&_tADS1256->tSPI, 0xFF);
		
	if((ulDigitValue&0x800000)==0x800000)
	{
		//负数情况
		fResVoltage=(0x7fffff-(ulDigitValue-0x800000)+1)*0.00000059032;	
	}
	else
	{
		//正数情况
		fResVoltage=(ulDigitValue)*0.00000059032;
	}
		
	return fResVoltage ;				
}
