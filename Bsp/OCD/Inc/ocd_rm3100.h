#ifndef __OCD_RM3100_H_
#define __OCD_RM3100_H_
#include "drv_hal_conf.h"

#define RM3100_SINGLE

/* 寄存器地址*/
#define RM3100_ADDRESS      0x20
#define M3D_3100_POLL 	    0			  //0X00
#define M3D_3100_CMM		    1			  //0X00
#define M3D_3100_CCX		    4			  //0X00C8
#define M3D_3100_CCY		    6			  //0X00C8
#define M3D_3100_CCZ		    8			  //0X00C8
#define M3D_3100_TMRC	      0x0B		//0X96
#define M3D_3100_MX		      0x24
#define M3D_3100_MY		      0x27
#define M3D_3100_MZ		      0x2A
#define M3D_3100_BIST	      0x33
#define M3D_3100_STATUS	    0x34
#define M3D_3100_HSHAKE	    0x35		//0X1B
#define M3D_3100_REVID      0x36


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

typedef struct {
  int32_t MAG_X;
  int32_t MAG_Y;
  int32_t MAG_Z;
} MagData_t;



void OCD_singlemeasure_soft(uint8_t conf ,tagSPISoft_T *_tSPI);
void OCD_singlemeasure(uint8_t conf,tagSPI_T *_tSPI);

void OCD_ThreeD3100_magic_init(tagSPI_T *_tSPI);
void OCD_ThreeD3100_magic_init_soft(tagSPISoft_T *_tSPI);


void OCD_ThreeD3100_magic_GetData(MagData_t* buff,tagSPI_T *_tSPI);
void OCD_ThreeD3100_magic_GetData_soft(MagData_t* buff,tagSPISoft_T *_tSPI);


void OCD_SPI_Write_M3D3100(tagSPI_T *_tSPI,uint8_t reg_addr,uint8_t *datain,uint8_t lenth);
void OCD_SPI_Read_M3D3100(tagSPI_T *_tSPI,uint8_t reg_addr,uint8_t *dataout,uint8_t lenth);
#endif
