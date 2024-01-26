#ifndef __OCD_MS5837_H_
#define __OCD_MS5837_H_

#include "drv_hal_conf.h"

/* 校准数据 
    C1  压力灵敏度 SENS|T1
    C2  压力补偿  OFF|T1
    C3	温度压力灵敏度系数 TCS
    C4	温度系数的压力补偿 TCO
    C5	参考温度 T|REF
    C6 	温度系数的温度 TEMPSENS
*/
#define C1 _tMS5837->usaCaliData[1]
#define C2 _tMS5837->usaCaliData[2]
#define C3 _tMS5837->usaCaliData[3]
#define C4 _tMS5837->usaCaliData[4]
#define C5 _tMS5837->usaCaliData[5]
#define C6 _tMS5837->usaCaliData[6]

/* 重力加速度(m/s^2) */
#define G   9.8f
/* 标准大气压值（mBar） */
#define ATM 1013

/* MS5837读写地址 */
#define MS5837_WRITEADDR            0xEC
#define MS5837_READADDR             0xED

#define MS5837_RESET                0X1E
#define MS5837_ADC_READ             0X00
#define MS5837_PROM_READ            0XA0

/* 分辨率寄存器 */
#define MS5837_D1_OSR_256			0x40
#define MS5837_D2_OSR_256			0x50
#define MS5837_D1_OSR_512			0x42
#define MS5837_D2_OSR_512			0x52
#define MS5837_D1_OSR_1024			0x44
#define MS5837_D2_OSR_1024			0x54
#define MS5837_D1_OSR_2048			0x46
#define MS5837_D2_OSR_2048			0x56
#define MS5837_D1_OSR_4096			0x48
#define MS5837_D2_OSR_4096			0x58
#define	MS5837_D1_OSR_8192          0x4A
#define	MS5837_D2_OSR_8192          0x5A

/* 过采样比率可选值 */
typedef enum{
    MS5837_OSR256,
    MS5837_OSR512,
    MS5837_OSR1024,
    MS5837_OSR2048,
    MS5837_OSR4096,
    MS5837_OSR8192
}MS5837OSR_E;

typedef struct
{
    /* 一阶补偿值 */
    float fTemp1;
    float fPress1;

    /* 二阶补偿值 */
    float fTemp2;
    float fPress2;

    /* 实际深度(cm) */
    float fDepth;

    /* 实际温度(C) */
    float fTemperature;

    /* 过采样比率 */
    MS5837OSR_E setOSR;

    /* 校准数据 */
    uint32_t usaCaliData[7];    

    tagIICSoft_T tIIC;
}tagMS5837_T;

void OCD_MS5837_GetData(tagMS5837_T *_tMS5837);
uint8_t OCD_MS5837_Init(tagMS5837_T *_tMS5837);

#endif
