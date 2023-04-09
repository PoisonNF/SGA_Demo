#ifndef __OCD_RM3100_H_
#define __OCD_RM3100_H_
#include "drv_hal_conf.h"

#define RM3100_SINGLE

/* 寄存器地址*/
#define RM3100_ADDRESS          0x20
#define M3D_3100_POLL 	        0			  /* 0X00 */
#define M3D_3100_CMM		    1			  /* 0X00 */
#define M3D_3100_CCX		    4			  /* 0X00C8 */
#define M3D_3100_CCY		    6			  /* 0X00C8 */
#define M3D_3100_CCZ		    8			  /* 0X00C8 */
#define M3D_3100_TMRC	        0x0B		  /* 0X96 */
#define M3D_3100_MX		        0x24
#define M3D_3100_MY		        0x27
#define M3D_3100_MZ		        0x2A
#define M3D_3100_BIST	        0x33
#define M3D_3100_STATUS	        0x34
#define M3D_3100_HSHAKE	        0x35		  /* 0X1B */
#define M3D_3100_REVID          0x36


/*  初始化时测量 */
#define CMM_OFF                       0x00
#define CM_START                      0x01
#define ALARM_BIT                     0x02
#define DRDY_WHEN_ALARM_AND_ALL_AXIS  0x00
#define DRDY_WHEN_ANY_AXIS_MEASURED   0x04
#define DRDY_WHEN_ALL_AXIS_MEASURED   0x08
#define DRDY_WHEN_ALARM               0x0C
#define CMM_ALL_AXIS_ON               0x70
#define CMM_X_AXIS                    0x10
#define CMM_Y_AXIS                    0x20
#define CMM_Z_AXIS                    0x40
#define LDM_BIT                       0x80

/* 可能的数据转换选择 */
#define CMM_UPDATERATE_600            0x92
#define CMM_UPDATERATE_300            0x93
#define CMM_UPDATERATE_150            0x94
#define CMM_UPDATERATE_75             0x95
#define CMM_UPDATERATE_37             0x96
#define CMM_UPDATERATE_18             0x97
#define CMM_UPDATERATE_9              0x98
#define CMM_UPDATERATE_4_5            0x99
#define CMM_UPDATERATE_2_3            0x9A
#define CMM_UPDATERATE_1_2            0x9B
#define CMM_UPDATERATE_0_6            0x9C
#define CMM_UPDATERATE_0_3            0x9D
#define CMM_UPDATERATE_0_15           0x9E
#define CMM_UPDATERATE_0_075          0x9F

/* 状态寄存器中第7位为数据是否可以读取标志*/
#define STATUS_MASK    0x80 

typedef struct 
{
	int32_t MAG_X;
	int32_t MAG_Y;
	int32_t MAG_Z;
}MagData_t;

/* RM3100初始化数据结构体 */
struct config
{
    unsigned int ucCycle_Count;
    float fSample_Rate;
    float fMax_Data_Rate;
    float fGain;
};

/* RM3100结构体 */
typedef struct
{
    /* 使用硬件SPI */
    bool            bSPIEnable;
    tagSPI_T        tSPI;

    /* 使用软件SPI */
    bool            bSPISoftEnable;
    tagSPISoft_T    tSoftSPI;

    /* 数据储存 */
    MagData_t       tMagData;
}tagRM3100_T;

uint8_t OCD_RM3100_SetCycleCount(tagRM3100_T *_tRM3100, uint16_t _usValue);
uint8_t OCD_RM3100_SetCMM_DataRate(tagRM3100_T *_tRM3100, uint8_t _ucConf);
void OCD_RM3100_ContinuousModeConfig(tagRM3100_T *_tRM3100, uint8_t _ucConf);
void OCD_RM3100_SingleModeConfig(tagRM3100_T *_tRM3100, uint8_t _ucConf);
void OCD_RM3100_ModeConfig(tagRM3100_T *_tRM3100);
void OCD_RM3100_GetData(tagRM3100_T *_tRM3100);
void OCD_RM3100_Init(tagRM3100_T *_tRM3100, uint8_t _ucNum);

#endif

