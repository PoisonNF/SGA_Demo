/****************************************************************************

* Sigma团队

* 文件名: ocd_rm3100.c

* 内容简述：rm3100模块文件

* 文件历史：

* 版本号	日期		作者		说明
* 1.1.8		2022-10-25	李豪	创建该文件

****************************************************************************/
#include "ocd_rm3100.h"

/* rm3100初始化数据结构体 */
struct config {
    unsigned int cycle_count;
    float sample_rate;
    float max_data_rate;
    float gain;
};
/*rm3100初始化数据*/
struct config rm = {
    .cycle_count   = 200,
    .sample_rate   = 37,
    .max_data_rate = 440,
    .gain          = 75
};

/**
 * @brief rm3100片选
 * @param _tSPI-软件spi句柄指针
 * @param level-电平
 * @retval NULL
*/
void OCD_ThreeD3100_CS(tagSPI_T *_tSPI,uint8_t level)
{
	HAL_GPIO_WritePin(_tSPI->tGPIO[3].tGPIOPort,_tSPI->tGPIO[3].tGPIOInit.Pin,level?GPIO_PIN_SET:GPIO_PIN_RESET);
}

/**
 * @brief RM3100的多字节写入
 * @param _tSPI-spi句柄指针
 * @param reg_addr-寄存器地址
 * @param *datain-写入数据的地址
 * @param lenth-写入数据的长度
 * @retval NULL
*/
void OCD_SPI_Write_M3D3100(tagSPI_T *_tSPI,uint8_t reg_addr,uint8_t *datain,uint8_t lenth)
{
	uint8_t bytecount=0;
	uint8_t temp=0;
	OCD_ThreeD3100_CS(_tSPI,0) ;	
	temp=0x00+(reg_addr&0x3f); //MSB=0  read reg
	Drv_SPI_TransmitReceive(_tSPI,temp);

	//send lest bytes
	for(bytecount=0;bytecount<lenth;bytecount++)
	{
		temp=*(datain+bytecount);
		Drv_SPI_TransmitReceive(_tSPI,temp);
	}
	OCD_ThreeD3100_CS(_tSPI,1);
}

/**
 * @brief RM3100的多字节写入
 * @param _tSPI-spi句柄指针
 * @param reg_addr-寄存器地址
 * @param *datain-写入数据的地址
 * @param lenth-写入数据的长度
 * @retval NULL
*/
void OCD_SPI_Read_M3D3100(tagSPI_T *_tSPI,uint8_t reg_addr,uint8_t *dataout,uint8_t lenth)
{
	uint8_t bytecount=0;
	uint8_t temp=0;	
	OCD_ThreeD3100_CS(_tSPI,0);
	temp=0x80+(reg_addr); //MSB=1 rw=1-->addr auto increase
	Drv_SPI_TransmitReceive(_tSPI,temp);
	
	for(bytecount=0;bytecount<lenth;bytecount++)
	{
		temp=Drv_SPI_TransmitReceive(_tSPI,0);
		*(dataout+bytecount) = temp;
	}
	OCD_ThreeD3100_CS(_tSPI,1);	
}

/**
 * @brief rm3100为连续测量模式时，设置数据测量速率
 * @param value-命令
 * @param _tSPI-spi句柄指针
 * @retval 成功-1  失败-0
*/
uint8_t OCD_setCycleCount(uint16_t value,tagSPI_T *_tSPI) 
{
    uint8_t to_reg[6];

    if (value > 65535 )
        return 0;
    else
	{
		if (value > 400)
        value=400;
		else if (value < 30)
        value=30;

		to_reg[0] = value>>8;
		to_reg[1] = value;
		to_reg[2] = to_reg[0];
		to_reg[3] = to_reg[1];
		to_reg[4] = to_reg[0];
		to_reg[5] = to_reg[1];

		OCD_SPI_Write_M3D3100(_tSPI, M3D_3100_CCX, to_reg,6);
    }
    return 1;
}

/**
 * @brief rm3100为连续测量模式时，设置数据转换速率
 * @param conf-命令
 * @param _tSPI-spi句柄指针
 * @retval 成功-1  失败-0
*/
uint8_t OCD_setCMMdatarate(uint8_t conf,tagSPI_T *_tSPI) 
{
    if (conf < CMM_UPDATERATE_600 || conf > CMM_UPDATERATE_0_075)
        return 0;
    else{
        uint8_t *ptr;
        float temp = 1000;

        switch (conf){
            case CMM_UPDATERATE_600: temp=600;
                                    break;
            case CMM_UPDATERATE_300: temp=300;
                                    break;
            case CMM_UPDATERATE_150: temp=150;
                                    break;
            case CMM_UPDATERATE_75 : temp=75;
                                    break;
            case CMM_UPDATERATE_37 : temp=37;
                                    break;
            case CMM_UPDATERATE_18 : temp=18;
                                    break;
            case CMM_UPDATERATE_9  : temp=9;
                                    break;
            case CMM_UPDATERATE_4_5: temp=4.5;
                                    break;
            case CMM_UPDATERATE_2_3: temp=2.3;
                                    break;
            case CMM_UPDATERATE_1_2: temp=1.2;
                                    break;
            case CMM_UPDATERATE_0_6: temp=0.6;
                                    break;
            case CMM_UPDATERATE_0_3: temp=0.3;
                                    break;
            case CMM_UPDATERATE_0_15: temp=0.15;
                                    break;
            case CMM_UPDATERATE_0_075: temp=0.075;
                                    break;
        }
        if (temp > rm.max_data_rate)				
            return 0;

        ptr = &conf ;
        OCD_SPI_Write_M3D3100(_tSPI, M3D_3100_TMRC, ptr,1);
    }
    return 1;
}

/**
 * @brief spi设置rm3100为连续测量模式
 * @param conf-命令
 * @param _tSPI-spi句柄指针
 * @retval NULL
*/
void OCD_continuousModeConfig (uint8_t conf ,tagSPI_T *_tSPI) 
{
    uint8_t *ptr;
    ptr = &conf ;
    OCD_SPI_Write_M3D3100(_tSPI, M3D_3100_CMM, ptr,1);
}

/**
 * @brief RM3100设置单次测量 
 * @param conf-命令
 * @param _tSPI-spi句柄指针
 * @retval NULL
*/
void  OCD_singlemeasure(uint8_t conf,tagSPI_T *_tSPI) 
{
    uint8_t *ptr;
    ptr = &conf ;
    OCD_SPI_Write_M3D3100(_tSPI,M3D_3100_POLL, ptr,1);
}

/**
 * @brief RM3100数据是否转换完毕
 * @param _tSPI-spi句柄指针
 * @retval 转换完毕-1  未转换完-0
*/
uint8_t OCD_getDataReadyStatus(tagSPI_T *_tSPI) 
{
    char data[1];
    OCD_SPI_Read_M3D3100(_tSPI, M3D_3100_STATUS, (uint8_t*) data,1);
    return (data[0] & STATUS_MASK);
}

/**
 * @brief 设定rm3100测量模式
 * @param _tSPI-spi句柄指针
 * @retval NULL
*/
void OCD_ThreeD3100_magic_init(tagSPI_T *_tSPI)
{
    #ifdef  RM3100_SINGLE
	OCD_singlemeasure(CMM_ALL_AXIS_ON,_tSPI);     //单测
	#endif

	#ifndef RM3100_SINGLE
  	OCD_continuousModeConfig(CMM_ALL_AXIS_ON|DRDY_WHEN_ALL_AXIS_MEASURED|CM_START,_tSPI); 			//设置测试轴,drdy 开启连续模式
  	OCD_setCycleCount(200,_tSPI);												
  	OCD_setCMMdatarate(12,_tSPI);
	#endif
}

/**
 * @brief spi驱动rm3100获取数据
 * @param buff-存放获得数据
 * @param _tSPI-spi句柄指针
 * @retval Null
*/
void OCD_ThreeD3100_magic_GetData(MagData_t* buff,tagSPI_T *_tSPI)
{
	uint8_t i;
	uint8_t temp[9]={0};

	int32_t Mag_Data[3]={0};

	while(OCD_getDataReadyStatus(_tSPI)==0);
	OCD_SPI_Read_M3D3100(_tSPI,0x24,temp,9);
	Mag_Data[0]=temp[0]<<16 | temp[1]<<8 | temp[2];			
	Mag_Data[1]=temp[3]<<16 | temp[4]<<8 | temp[5];
	Mag_Data[2]=temp[6]<<16 | temp[7]<<8 | temp[8];

	for(i=0;i<3;i++)
	{
		if(Mag_Data[i]&0x00800000)
		Mag_Data[i]|=0xff000000;
	}
 
	buff->MAG_X=Mag_Data[1]*13;
	buff->MAG_Y=-Mag_Data[0]*13;
	buff->MAG_Z=-Mag_Data[2]*13; 
}


//*********************************rm3100--soft_spi***************************************************//

/**
 * @brief RM3100的多字节写入
 * @param reg_addr-寄存器地址
 * @param *datain-写入数据的地址
 * @param lenth-写入数据的长度
 * @param _tSPI-软件spi句柄指针
 * @retval NULL
*/
void OCD_SPI_Write_M3D3100_soft(uint8_t reg_addr,uint8_t *datain,uint8_t lenth,tagSPISoft_T *_tSPI)
{
	uint8_t bytecount=0;
	uint8_t temp=0;
 
	Drv_SPI_NSS(_tSPI,0);
//	HAL_Delay(10);	
	temp=0x00+(reg_addr&0x3f); //MSB=0  read reg
	Drv_SPI_WriteByte_soft(_tSPI,temp);

	//send lest bytes
	for(bytecount=0;bytecount<lenth;bytecount++)
	{
		temp=*(datain+bytecount);
		Drv_SPI_WriteByte_soft(_tSPI,temp);
	}
	Drv_SPI_NSS(_tSPI,1);
}

/**
 * @brief RM3100的读多字节数据 
 * @param reg_addr-寄存器地址
 * @param *dataout-写入数据的地址
 * @param lenth-写入数据的长度
 * @param _tSPI-软件spi句柄指针
 * @retval NULL
*/
void  OCD_SPI_Read_M3D3100_soft(uint8_t reg_addr,uint8_t *dataout,uint8_t lenth,tagSPISoft_T *_tSPI)
{
	uint8_t bytecount=0;
	uint8_t temp=0;	

	Drv_SPI_NSS(_tSPI,0);
//	HAL_Delay(10);
	temp=0x80+(reg_addr); //MSB=1 rw=1-->addr auto increase
	Drv_SPI_WriteByte_soft(_tSPI,temp);

	for(bytecount=0;bytecount<lenth;bytecount++)
	{
		temp=Drv_SPI_WriteByte_soft(_tSPI,0);
		*(dataout+bytecount) = temp;
	}
	Drv_SPI_NSS(_tSPI,1);
}

/**
 * @brief RM3100设置单次测量 
 * @param conf-命令
 * @param _tSPI-软件spi句柄指针
 * @retval NULL
*/
void OCD_singlemeasure_soft(uint8_t conf,tagSPISoft_T *_tSPI) 
{
    uint8_t *ptr;

    ptr = &conf ;
    OCD_SPI_Write_M3D3100_soft(M3D_3100_POLL, ptr,1,_tSPI);
}

/**
 * @brief RM3100数据是否转换完毕
 * @param _tSPI-软件spi句柄指针
 * @retval 转换完毕-1  未转换完-0
*/
uint8_t OCD_getDataReadyStatus_soft(tagSPISoft_T *_tSPI)
{
    char data[1];

    OCD_SPI_Read_M3D3100_soft(M3D_3100_STATUS, (uint8_t*) data,1,_tSPI);

    return (data[0] & STATUS_MASK);
}

/**
 * @brief 软件spi设置rm3100为连续测量模式
 * @param conf-命令
 * @param _tSPI-软件spi句柄指针
 * @retval 成功-0  失败-1
*/
void OCD_continuousModeConfig_soft(uint8_t conf,tagSPISoft_T *_tSPI) 
{
    uint8_t *ptr;

    ptr = &conf ;
    OCD_SPI_Write_M3D3100_soft(M3D_3100_CMM, ptr,1,_tSPI);
}

/**
 * @brief rm3100为连续测量模式时，设置数据测量速率
 * @param value-命令
 * @param _tSPI-软件spi句柄指针
 * @retval 成功-1  失败-0
*/
uint8_t OCD_setCycleCount_soft(uint16_t value ,tagSPISoft_T *_tSPI)
{
    uint8_t to_reg[6];

    if (value > 65535 )
        return 0;
    else{

		if (value > 400) value=400;
		else if (value < 30) value=30;

		to_reg[0] = value>>8;
		to_reg[1] = value;
		to_reg[2] = to_reg[0];
		to_reg[3] = to_reg[1];
		to_reg[4] = to_reg[0];
		to_reg[5] = to_reg[1];

		OCD_SPI_Write_M3D3100_soft(M3D_3100_CCX, to_reg,6,_tSPI);
    }
    return 1;
}

/**
 * @brief rm3100为连续测量模式时，设置数据转换速率
 * @param conf-命令
 * @param _tSPI-软件spi句柄指针
 * @retval 成功-1  失败-0
*/
uint8_t OCD_setCMMdatarate_soft(uint8_t conf ,tagSPISoft_T *_tSPI)
{
    if (conf < CMM_UPDATERATE_600 || conf > CMM_UPDATERATE_0_075)
        return 0;
    else{
		uint8_t *ptr;
		float temp = 1000;

		switch (conf){
			case CMM_UPDATERATE_600: temp=600;
									break;
			case CMM_UPDATERATE_300: temp=300;
									break;
			case CMM_UPDATERATE_150: temp=150;
									break;
			case CMM_UPDATERATE_75 : temp=75;
									break;
			case CMM_UPDATERATE_37 : temp=37;
									break;
			case CMM_UPDATERATE_18 : temp=18;
									break;
			case CMM_UPDATERATE_9  : temp=9;
									break;
			case CMM_UPDATERATE_4_5: temp=4.5;
									break;
			case CMM_UPDATERATE_2_3: temp=2.3;
									break;
			case CMM_UPDATERATE_1_2: temp=1.2;
									break;
			case CMM_UPDATERATE_0_6: temp=0.6;
									break;
			case CMM_UPDATERATE_0_3: temp=0.3;
									break;
			case CMM_UPDATERATE_0_15: temp=0.15;
                                    break;
			case CMM_UPDATERATE_0_075: temp=0.075;
									break;
        }
		if (temp > rm.max_data_rate)				
        return 0;

      	ptr = &conf ;
      	OCD_SPI_Write_M3D3100_soft(M3D_3100_TMRC, ptr,1,_tSPI);
    }
   	return 1;
}

/**
 * @brief 设定rm3100测量模式
 * @param _tSPI-软件spi句柄指针
 * @retval NULL
*/
void OCD_ThreeD3100_magic_init_soft(tagSPISoft_T *_tSPI)
{
	#ifdef  RM3100_SINGLE
	OCD_singlemeasure_soft(CMM_ALL_AXIS_ON,_tSPI);     //单测
	#endif
	#ifndef RM3100_SINGLE
  	OCD_continuousModeConfig_soft(CMM_ALL_AXIS_ON|DRDY_WHEN_ALL_AXIS_MEASURED|CM_START,_tSPI); 			//设置测试轴,drdy 开启连续模式

  	OCD_setCycleCount_soft(200,_tSPI);												
  	OCD_setCMMdatarate_soft(12,_tSPI);
	#endif
}

/**
 * @brief 软件spi驱动rm3100获取数据
 * @param buff-存放获得数据
 * @param _tSPI-软件spi句柄指针
 * @retval Null
*/
void OCD_ThreeD3100_magic_GetData_soft(MagData_t* buff,tagSPISoft_T *_tSPI)
{
	uint8_t i;
	uint8_t temp[9]={0};

	int32_t Mag_Data[3]={0};

	while(OCD_getDataReadyStatus_soft(_tSPI)==0);
	Drv_SPI_NSS(_tSPI,1); 
	OCD_SPI_Read_M3D3100_soft(0x24,temp,9,_tSPI);
	Mag_Data[0]=temp[0]<<16 | temp[1]<<8 | temp[2];			
	Mag_Data[1]=temp[3]<<16 | temp[4]<<8 | temp[5];
	Mag_Data[2]=temp[6]<<16 | temp[7]<<8 | temp[8];

	for(i=0;i<3;i++)
	{
		if(Mag_Data[i]&0x00800000)
		Mag_Data[i]|=0xff000000;
	}
 
	buff->MAG_X=Mag_Data[1]*13;
	buff->MAG_Y=-Mag_Data[0]*13;
	buff->MAG_Z=-Mag_Data[2]*13; 
}



