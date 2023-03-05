#ifndef __OCD_ADS1256_H_
#define __OCD_ADS1256_H_

#include "drv_hal_conf.h"

/* register address,follow with reset value */
#define ADS1256_REG_STATUS  0  /* x1H */
#define ADS1256_REG_MUX     1  /* 01H */
#define ADS1256_REG_ADCON   2  /* 20H */
#define ADS1256_REG_DRATE   3  /* F0H */
#define ADS1256_REG_IO      4  /* E0H */
#define ADS1256_REG_OFC0    5  /* xxH */
#define ADS1256_REG_OFC1    6  /* xxH */
#define ADS1256_REG_OFC2    7  /* xxH */
#define ADS1256_REG_FSC0    8  /* xxH */
#define ADS1256_REG_FSC1    9  /* xxH */
#define ADS1256_REG_FSC2    10 /* xxH */

/* ads1256 command definition */
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
#define ADS1256_RATE_30000SPS	0xF0 /* default value */
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

typedef struct
{
	tagSPI_T 	tSPI;
	tagGPIO_T	tDrdyGPIO;
	tagGPIO_T	tCsGPIO;
}tagADS1256_T;

typedef struct
{
	uint8_t ucID;
	uint8_t ucDataOrder ;
	uint8_t ucAutoCalibration;
	uint8_t ucBufferStatus ;	
	uint8_t ucMuxValue;
	uint8_t ucPgaValue;
	uint8_t ucDataRate;
}tagADS1256Status_T;

uint8_t OCD_ADS1256_Check(tagADS1256_T *_tADS1256);
uint8_t OCD_ADS1256_ReadStatus(tagADS1256_T *_tADS1256, tagADS1256Status_T *_pAddr);
float OCD_ADS1256_ReadOneChannelVoltage(tagADS1256_T *_tADS1256, uint8_t _ucChannel);
void OCD_ADS1256_Init(tagADS1256_T *_tADS1256);

#endif
