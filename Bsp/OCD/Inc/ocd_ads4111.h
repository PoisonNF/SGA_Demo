#ifndef __OCD_ADS4111_H_
#define __OCD_ADS4111_H_

#include "drv_hal_conf.h"

#define AD411X_ID_VAL 0x30D0

/* AD411X Register Map */
#define AD411X_COMM_REG       0x00
#define AD411X_STATUS_REG     0x00
#define AD411X_ADCMODE_REG    0x01
#define AD411X_IFMODE_REG     0x02
#define AD411X_REGCHECK_REG   0x03
#define AD411X_DATA_REG       0x04
#define AD411X_GPIOCON_REG    0x06
#define AD411X_ID_REG         0x07
#define AD411X_CHMAP0_REG     0x10
#define AD411X_CHMAP1_REG     0x11
#define AD411X_CHMAP2_REG     0x12
#define AD411X_CHMAP3_REG     0x13
#define AD411X_CHMAP4_REG     0x14
#define AD411X_CHMAP5_REG     0x15
#define AD411X_CHMAP6_REG     0x16
#define AD411X_CHMAP7_REG     0x17
#define AD411X_CHMAP8_REG     0x18
#define AD411X_CHMAP9_REG     0x19
#define AD411X_CHMAP10_REG    0x1A
#define AD411X_CHMAP11_REG    0x1B
#define AD411X_CHMAP12_REG    0x1C
#define AD411X_CHMAP13_REG    0x1D
#define AD411X_CHMAP14_REG    0x1E
#define AD411X_CHMAP15_REG    0x1F
#define AD411X_SETUPCON0_REG  0x20
#define AD411X_SETUPCON1_REG  0x21
#define AD411X_SETUPCON2_REG  0x22
#define AD411X_SETUPCON3_REG  0x23
#define AD411X_SETUPCON4_REG  0x24
#define AD411X_SETUPCON5_REG  0x25
#define AD411X_SETUPCON6_REG  0x26
#define AD411X_SETUPCON7_REG  0x27
#define AD411X_FILTCON0_REG   0x28
#define AD411X_FILTCON1_REG   0x29
#define AD411X_FILTCON2_REG   0x2A
#define AD411X_FILTCON3_REG   0x2B
#define AD411X_FILTCON4_REG   0x2C
#define AD411X_FILTCON5_REG   0x2D
#define AD411X_FILTCON6_REG   0x2E
#define AD411X_FILTCON7_REG   0x2F
#define AD411X_OFFSET0_REG    0x30
#define AD411X_OFFSET1_REG    0x31
#define AD411X_OFFSET2_REG    0x32
#define AD411X_OFFSET3_REG    0x33
#define AD411X_OFFSET4_REG    0x34
#define AD411X_OFFSET5_REG    0x35
#define AD411X_OFFSET6_REG    0x36
#define AD411X_OFFSET7_REG    0x37
#define AD411X_GAIN0_REG      0x38
#define AD411X_GAIN1_REG      0x39
#define AD411X_GAIN2_REG      0x3A
#define AD411X_GAIN3_REG      0x3B
#define AD411X_GAIN4_REG      0x3C
#define AD411X_GAIN5_REG      0x3D
#define AD411X_GAIN6_REG      0x3E
#define AD411X_GAIN7_REG      0x3F

/* Communication Register bits */
#define AD411X_COMM_REG_WEN    (0 << 7)
#define AD411X_COMM_REG_WR     (0 << 6)
#define AD411X_COMM_REG_RD     (1 << 6)
#define AD411X_COMM_REG_RA(x)  ((x) & 0x3F)

/* Status Register bits */
#define AD411X_STATUS_REG_RDY      (1 << 7)
#define AD411X_STATUS_REG_ADC_ERR  (1 << 6)
#define AD411X_STATUS_REG_CRC_ERR  (1 << 5)
#define AD411X_STATUS_REG_REG_ERR  (1 << 4)
#define AD411X_STATUS_REG_CH(x)    ((x) & 0x0F)

/* ADC Mode Register bits */
#define AD411X_ADCMODE_REG_REF_EN     (1 << 15)
#define AD411X_ADCMODE_SING_CYC       (1 << 13)
#define AD411X_ADCMODE_REG_DELAY(x)   (((x) & 0x7) << 8)
#define AD411X_ADCMODE_REG_MODE(x)    (((x) & 0x7) << 4)
#define AD411X_ADCMODE_REG_CLKSEL(x)  (((x) & 0x3) << 2)

/* ADC Mode Register additional bits for AD7172-2, AD7172-4, AD4111 and AD411X */
#define AD411X_ADCMODE_REG_HIDE_DELAY   (1 << 14)

/* Interface Mode Register bits */
#define AD411X_IFMODE_REG_ALT_SYNC      (1 << 12)
#define AD411X_IFMODE_REG_IOSTRENGTH    (1 << 11)
#define AD411X_IFMODE_REG_HIDE_DELAY    (1 << 10)
#define AD411X_IFMODE_REG_DOUT_RESET    (1 << 8)
#define AD411X_IFMODE_REG_CONT_READ     (1 << 7)
#define AD411X_IFMODE_REG_DATA_STAT     (1 << 6)
#define AD411X_IFMODE_REG_REG_CHECK     (1 << 5)
#define AD411X_IFMODE_REG_XOR_EN        (0x01 << 2)
#define AD411X_IFMODE_REG_CRC_EN        (0x02 << 2)
#define AD411X_IFMODE_REG_XOR_STAT(x)   (((x) & AD411X_IFMODE_REG_XOR_EN) == AD411X_IFMODE_REG_XOR_EN)
#define AD411X_IFMODE_REG_CRC_STAT(x)   (((x) & AD411X_IFMODE_REG_CRC_EN) == AD411X_IFMODE_REG_CRC_EN)
#define AD411X_IFMODE_REG_DATA_WL16     (1 << 0)

/* GPIO Configuration Register bits */
#define AD411X_GPIOCON_REG_MUX_IO      (1 << 12)
#define AD411X_GPIOCON_REG_SYNC_EN     (1 << 11)
#define AD411X_GPIOCON_REG_ERR_EN(x)   (((x) & 0x3) << 9)
#define AD411X_GPIOCON_REG_ERR_DAT     (1 << 8)
#define AD411X_GPIOCON_REG_IP_EN1      (1 << 5)
#define AD411X_GPIOCON_REG_IP_EN0      (1 << 4)
#define AD411X_GPIOCON_REG_OP_EN1      (1 << 3)
#define AD411X_GPIOCON_REG_OP_EN0      (1 << 2)

/* GPIO Configuration Register additional bits for AD4111, AD411X */
#define AD411X_GPIOCON_REG_OP_EN0_1    (1 << 13)
#define AD411X_GPIOCON_REG_DATA1       (1 << 7)
#define AD411X_GPIOCON_REG_DATA0       (1 << 6)

/* GPIO Configuration Register additional bits for AD4111 */
#define AD4111_GPIOCON_REG_OW_EN       (1 << 12)

/* Channel Map Register 0-3 bits */
#define AD411X_CHMAP_REG_CH_EN         (1 << 15)
#define AD411X_CHMAP_REG_SETUP_SEL(x)  (((x) & 0x7) << 12)
#define AD411X_CHMAP_REG_AINPOS(x)     (((x) & 0x1F) << 5)
#define AD411X_CHMAP_REG_AINNEG(x)     (((x) & 0x1F) << 0)

/* Channel Map Register additional bits for AD4111, AD411X */
#define AD411X_CHMAP_REG_INPUT(x)      (((x) & 0x3FF) << 0)

/* Setup Configuration Register 0-3 bits */
#define AD411X_SETUP_CONF_REG_BI_UNIPOLAR  (1 << 12)
#define AD411X_SETUP_CONF_REG_REF_SEL(x)   (((x) & 0x3) << 4)



/* Setup Configuration Register additional bits for AD4111, AD411X */
#define AD411X_SETUP_CONF_REG_REFPOS_BUF   (1 << 11)
#define AD411X_SETUP_CONF_REG_REFNEG_BUF   (1 << 10)
#define AD411X_SETUP_CONF_REG_AIN_BUF(x)   (((x) & 0x3) << 8)
#define AD411X_SETUP_CONF_REG_BUFCHOPMAX   (1 << 6)

/* Filter Configuration Register 0-3 bits */
#define AD411X_FILT_CONF_REG_SINC3_MAP    (1 << 15)
#define AD411X_FILT_CONF_REG_ENHFILTEN    (1 << 11)
#define AD411X_FILT_CONF_REG_ENHFILT(x)   (((x) & 0x7) << 8)
#define AD411X_FILT_CONF_REG_ORDER(x)     (((x) & 0x3) << 5)
#define AD411X_FILT_CONF_REG_ODR(x)       (((x) & 0x1F) << 0)

/* ID register mask for relevant bits */
#define AD411X_ID_REG_MASK	  0xFFF0
/* AD7172-2 ID */
#define AD7172_2_ID_REG_VALUE 0x00D0
/* AD7172-4 ID */
#define AD7172_4_ID_REG_VALUE 0x2050
/* AD7173-8 ID */
#define AD7173_8_ID_REG_VALUE 0x30D0
/* AD7175-2 ID */
#define AD7175_2_ID_REG_VALUE 0x0CD0
/* AD7175-8 ID */
#define AD7175_8_ID_REG_VALUE 0x3CD0
/* AD7176-2 ID */
#define AD7176_2_ID_REG_VALUE 0x0C90
/* AD7177-2 ID */
#define AD7177_2_ID_REG_VALUE 0x4FD0
/* AD411x ID */
#define AD411X_ID_REG_VALUE   0x30D0

#define AD411X_CRC8_POLYNOMIAL_REPRESENTATION 0x07

/* Error codes */
#define OK			0x00
#define ERROR		0x01
#define INVALID_VAL 0x02 /* Invalid argument */
#define COMM_ERR    0x03 /* Communication error on receive */
#define TIMEOUT     0x04 /* A timeout has occured */

typedef enum {
	AD411X_DISABLE,
	AD411X_USE_CRC,
	AD411X_USE_XOR,
}temAD4111CrcMode;

typedef struct 
{
	int32_t		ulAddr;
	int32_t		ulValue;
	int32_t		ulSize;
}tagAD4111Reg_T;	/* ad411x_st_reg */

typedef struct 
{
	tagSPI_T			tSPI;
	tagAD4111Reg_T		*tADS4111Reg;
	uint8_t				ucRegNum;
	temAD4111CrcMode	tCrcMode;
}tagAD4111_T;

tagAD4111Reg_T tAD4111Reg[] = {
	{ AD411X_STATUS_REG, 0x00, 1 },
	{
		AD411X_ADCMODE_REG,
		0x800C,
		2
	},
	{ AD411X_IFMODE_REG, 0x0040, 2 },
	{ AD411X_REGCHECK_REG, 0x0000, 3 },
	{ AD411X_DATA_REG, 0x0000, 3 },
	{
		AD411X_GPIOCON_REG,
		0x0400,
		2
	},
	{ AD411X_ID_REG, 0x0000, 2 },
	{ AD411X_CHMAP0_REG, 0x0000, 2 },
	{ AD411X_CHMAP1_REG, 0x8050, 2 },
	{ AD411X_CHMAP2_REG, 0x8030, 2 },
	{ AD411X_CHMAP3_REG, 0x8070, 2 },
	{ AD411X_CHMAP4_REG, 0x0000, 2 },
	{ AD411X_CHMAP5_REG, 0x0000, 2 },
	{ AD411X_CHMAP6_REG, 0x0000, 2 },
	{ AD411X_CHMAP7_REG, 0x0000, 2 },
	{ AD411X_CHMAP8_REG, 0x0000, 2 },
	{ AD411X_CHMAP9_REG, 0x0000, 2 },
	{ AD411X_CHMAP10_REG, 0x0000, 2 },
	{ AD411X_CHMAP11_REG, 0x0000, 2 },
	{ AD411X_CHMAP12_REG, 0x0000, 2 },
	{ AD411X_CHMAP13_REG, 0x0000, 2 },
	{ AD411X_CHMAP14_REG, 0x0000, 2 },
	{ AD411X_CHMAP15_REG, 0x0000, 2 },
	{ AD411X_SETUPCON0_REG, 0x0300 | AD411X_SETUP_CONF_REG_REF_SEL(2), 2 },
	{ AD411X_SETUPCON1_REG, 0x0000 | AD411X_SETUP_CONF_REG_REF_SEL(2), 2 },
	{ AD411X_SETUPCON2_REG, 0x0000 | AD411X_SETUP_CONF_REG_REF_SEL(2), 2 },
	{ AD411X_SETUPCON3_REG, 0x0000 | AD411X_SETUP_CONF_REG_REF_SEL(2), 2 },
	{ AD411X_SETUPCON4_REG, 0x0000 | AD411X_SETUP_CONF_REG_REF_SEL(2), 2 },
	{ AD411X_SETUPCON5_REG, 0x0000 | AD411X_SETUP_CONF_REG_REF_SEL(2), 2 },
	{ AD411X_SETUPCON6_REG, 0x0000 | AD411X_SETUP_CONF_REG_REF_SEL(2), 2 },
	{ AD411X_SETUPCON7_REG, 0x0000 | AD411X_SETUP_CONF_REG_REF_SEL(2), 2 },
	{
		AD411X_FILTCON0_REG,0x0208, 2
	},
	{
		AD411X_FILTCON1_REG, AD411X_FILT_CONF_REG_ENHFILT(2)|0x08, 2
	},
	{
		AD411X_FILTCON2_REG, AD411X_FILT_CONF_REG_ENHFILT(2)|0x07, 2
	},
	{
		AD411X_FILTCON3_REG, AD411X_FILT_CONF_REG_ENHFILT(2)|0x07, 2
	},
	{
		AD411X_FILTCON4_REG, AD411X_FILT_CONF_REG_ENHFILT(2)|0x07, 2
	},
	{
		AD411X_FILTCON5_REG, AD411X_FILT_CONF_REG_ENHFILT(2)|0x07, 2
	},
	{
		AD411X_FILTCON6_REG, AD411X_FILT_CONF_REG_ENHFILT(2)|0x07, 2
	},
	{
		AD411X_FILTCON7_REG, AD411X_FILT_CONF_REG_ENHFILT(2)|0x07, 2
	},
	{AD411X_OFFSET0_REG, 0, 3 },
	{AD411X_OFFSET1_REG, 0, 3 },
	{AD411X_OFFSET2_REG, 0, 3 },
	{AD411X_OFFSET3_REG, 0, 3 },
	{AD411X_OFFSET4_REG, 0, 3 },
	{AD411X_OFFSET5_REG, 0, 3 },
	{AD411X_OFFSET6_REG, 0, 3 },
	{AD411X_OFFSET7_REG, 0, 3 },
	{AD411X_GAIN0_REG, 0x55567C, 3 },
	{AD411X_GAIN1_REG, 0, 3 },
	{AD411X_GAIN2_REG, 0, 3 },
	{AD411X_GAIN3_REG, 0, 3 },
	{AD411X_GAIN4_REG, 0, 3 },
	{AD411X_GAIN5_REG, 0, 3 },
	{AD411X_GAIN6_REG, 0, 3 },
	{AD411X_GAIN7_REG, 0, 3 },
};

int8_t OCD_ADS4111Init(tagAD4111_T *_tAS4111);

#endif

