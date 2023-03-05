/****************************************************************************

* Sigma团队

* 文件名: ocd_rm3100.c

* 内容简述：rm3100模块文件

* 文件历史：

* 版本号	日期		作者		说明
* 1.1.8		2022-10-25	李豪	创建该文件

****************************************************************************/
#include "ocd_rm3100.h"

//*********************************RM3100--spi***************************************************//

/**
 * @brief RM3100片选
 * @param _tSPI-spi句柄指针
 * @param _ucLevel-电平
 * @retval Null
*/
static void S_ThreeD3100_CS(tagSPI_T *_tSPI,uint8_t _ucLevel)
{
	HAL_GPIO_WritePin(_tSPI->tGPIO[3].tGPIOPort,_tSPI->tGPIO[3].tGPIOInit.Pin,_ucLevel?GPIO_PIN_SET:GPIO_PIN_RESET);
}

/**
 * @brief RM3100的多字节写入
 * @param _tSPI-spi句柄指针
 * @param _ucReg_addr-寄存器地址
 * @param _ucpDataIn-写入数据的地址
 * @param _ucLenth-写入数据的长度
 * @retval Null
*/
static void S_SPI_Write_M3D3100(tagSPI_T *_tSPI,uint8_t _ucReg_addr,uint8_t *_ucpDataIn,uint8_t _ucLenth)
{
	uint8_t bytecount=0;
	uint8_t temp=0;

	S_ThreeD3100_CS(_tSPI,0) ;	
	temp = 0x00+(_ucReg_addr&0x3f);
	Drv_SPI_TransmitReceive(_tSPI,temp);

	for(bytecount=0;bytecount<_ucLenth;bytecount++)
	{
		temp=*(_ucpDataIn+bytecount);
		Drv_SPI_TransmitReceive(_tSPI,temp);
	}
	S_ThreeD3100_CS(_tSPI,1);
}

/**
 * @brief RM3100的多字节读取
 * @param _tSPI-spi句柄指针
 * @param _ucReg_addr-寄存器地址
 * @param _ucpDataOut-读出数据的地址
 * @param _ucLenth-读出数据的长度
 * @retval Null
*/
static void S_SPI_Read_M3D3100(tagSPI_T *_tSPI,uint8_t _ucReg_addr,uint8_t *_ucpDataOut,uint8_t _ucLenth)
{
	uint8_t bytecount=0;
	uint8_t temp=0;
	
	S_ThreeD3100_CS(_tSPI,0);
	temp = 0x80+(_ucReg_addr);
	Drv_SPI_TransmitReceive(_tSPI,temp);
	
	for(bytecount=0;bytecount<_ucLenth;bytecount++)
	{
		temp=Drv_SPI_TransmitReceive(_tSPI,0);
		*(_ucpDataOut+bytecount) = temp;
	}
	S_ThreeD3100_CS(_tSPI,1);	
}

/**
 * @brief RM3100为连续测量模式时，设置数据测量速率
 * @param _tSPI-spi句柄指针
 * @param _usValue-命令
 * @retval uint8_t-成功-1  失败-0
*/
uint8_t OCD_SetCycleCount(tagSPI_T *_tSPI,uint16_t _usValue) 
{
    uint8_t to_reg[6];

    if (_usValue > 65535)
        return 0;
    else
	{
		if (_usValue > 400)
        _usValue = 400;
		else if (_usValue < 30)
        _usValue = 30;

		to_reg[0] = _usValue>>8;
		to_reg[1] = _usValue;
		to_reg[2] = to_reg[0];
		to_reg[3] = to_reg[1];
		to_reg[4] = to_reg[0];
		to_reg[5] = to_reg[1];

		S_SPI_Write_M3D3100(_tSPI, M3D_3100_CCX, to_reg,6);
    }
    return 1;
}

/**
 * @brief RM3100为连续测量模式时，设置数据转换速率
 * @param _tSPI-spi句柄指针
 * @param _ucConf-命令
 * @retval uint8_t-成功-1  失败-0
*/
uint8_t OCD_SetCMMdatarate(tagSPI_T *_tSPI,uint8_t _ucConf) 
{
    if (_ucConf < CMM_UPDATERATE_600 || _ucConf > CMM_UPDATERATE_0_075)
        return 0;
    else{
        uint8_t *ptr;
        float temp = 1000;

        switch (_ucConf){
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

        ptr = &_ucConf;
        S_SPI_Write_M3D3100(_tSPI, M3D_3100_TMRC, ptr,1);
    }
    return 1;
}

/**
 * @brief RM3100设置连续测量模式
 * @param _tSPI-spi句柄指针
 * @param _ucConf-命令
 * @retval Null
*/
void OCD_ContinuousModeConfig(tagSPI_T *_tSPI,uint8_t _ucConf) 
{
    uint8_t *ptr;
    ptr = &_ucConf;
    S_SPI_Write_M3D3100(_tSPI, M3D_3100_CMM, ptr,1);
}

/**
 * @brief RM3100设置单次测量模式
 * @param _tSPI-spi句柄指针
 * @param _ucConf-命令
 * @retval Null
*/
void OCD_SingleModeConfig(tagSPI_T *_tSPI,uint8_t _ucConf) 
{
    uint8_t *ptr;
    ptr = &_ucConf;
    S_SPI_Write_M3D3100(_tSPI, M3D_3100_POLL, ptr,1);
}

/**
 * @brief RM3100数据是否转换完毕
 * @param _tSPI-spi句柄指针
 * @retval uint8_t-转换完毕-1  未转换完-0
*/
uint8_t OCD_GetDataReadyStatus(tagSPI_T *_tSPI) 
{
    char data[1];
    S_SPI_Read_M3D3100(_tSPI, M3D_3100_STATUS, (uint8_t*) data,1);
    return (data[0] & STATUS_MASK);
}

/**
 * @brief 设定RM3100测量模式
 * @param _tSPI-spi句柄指针
 * @retval Null
*/
void OCD_ThreeD3100_Magic_Init(tagSPI_T *_tSPI)
{
    #ifdef  RM3100_SINGLE
	OCD_SingleModeConfig(_tSPI,CMM_ALL_AXIS_ON);     /* 单测模式 */
	#endif

	#ifndef RM3100_SINGLE
  	OCD_ContinuousModeConfig(_tSPI,CMM_ALL_AXIS_ON|DRDY_WHEN_ALL_AXIS_MEASURED|CM_START); 			/* 设置测试轴,drdy 开启连续模式 */
  	OCD_SetCycleCount(_tSPI,200);												
  	OCD_SetCMMdatarate(_tSPI,12);
	#endif
}

/**
 * @brief spi驱动RM3100获取数据
 * @param _tSPI-spi句柄指针
 * @param _tBuff-存放获得数据
 * @retval Null
*/
void OCD_ThreeD3100_Magic_GetData(tagSPI_T *_tSPI,MagData_t *_tBuff)
{
	uint8_t i;
	uint8_t temp[9]={0};
	int32_t Mag_Data[3]={0};

	while(OCD_GetDataReadyStatus(_tSPI)==0);
	S_SPI_Read_M3D3100(_tSPI,0x24,temp,9);
	Mag_Data[0]=temp[0]<<16 | temp[1]<<8 | temp[2];			
	Mag_Data[1]=temp[3]<<16 | temp[4]<<8 | temp[5];
	Mag_Data[2]=temp[6]<<16 | temp[7]<<8 | temp[8];

	for(i=0;i<3;i++)
	{
		if(Mag_Data[i]&0x00800000)
		Mag_Data[i]|=0xff000000;
	}
	
	_tBuff->MAG_X=Mag_Data[1]*13;
	_tBuff->MAG_Y=-Mag_Data[0]*13;
	_tBuff->MAG_Z=-Mag_Data[2]*13; 
}


//*********************************RM3100--soft_spi***************************************************//

/**
 * @brief RM3100的多字节写入
* @param _tSPI-软件spi句柄指针
 * @param _ucReg_addr-寄存器地址
 * @param _ucpDataIn-写入数据的地址
 * @param _ucLenth-写入数据的长度
 * @retval Null
*/
static void S_SPI_Write_M3D3100_Soft(tagSPISoft_T *_tSPI,uint8_t _ucReg_addr,uint8_t *_ucpDataIn,uint8_t _ucLenth)
{
	uint8_t bytecount=0;
	uint8_t temp=0;
 
	Drv_SPI_NSS(_tSPI,0);

	temp=0x00+(_ucReg_addr&0x3f);
	Drv_SPI_WriteByte_Soft(_tSPI,temp);

	for(bytecount=0;bytecount<_ucLenth;bytecount++)
	{
		temp=*(_ucpDataIn+bytecount);
		Drv_SPI_WriteByte_Soft(_tSPI,temp);
	}
	Drv_SPI_NSS(_tSPI,1);
}

/**
 * @brief RM3100的读多字节数据
 * @param _tSPI-软件spi句柄指针
 * @param _ucReg_addr-寄存器地址
 * @param _ucpDataOut-写入数据的地址
 * @param _ucLenth-写入数据的长度
 * @retval Null
*/
static void S_SPI_Read_M3D3100_Soft(tagSPISoft_T *_tSPI,uint8_t _ucReg_addr,uint8_t *_ucpDataOut,uint8_t _ucLenth)
{
	uint8_t bytecount = 0;
	uint8_t temp = 0;	

	Drv_SPI_NSS(_tSPI,0);

	temp=0x80+(_ucReg_addr);
	Drv_SPI_WriteByte_Soft(_tSPI,temp);

	for(bytecount=0;bytecount<_ucLenth;bytecount++)
	{
		temp=Drv_SPI_WriteByte_Soft(_tSPI,0);
		*(_ucpDataOut+bytecount) = temp;
	}
	Drv_SPI_NSS(_tSPI,1);
}

/**
 * @brief RM3100设置单次测量模式
 * @param _tSPI-软件spi句柄指针
 * @param _ucConf-命令
 * @retval Null
*/
void OCD_SingleModeConfig_Soft(tagSPISoft_T *_tSPI,uint8_t _ucConf) 
{
    uint8_t *ptr;

    ptr = &_ucConf ;
    S_SPI_Write_M3D3100_Soft(_tSPI,M3D_3100_POLL, ptr,1);
}

/**
 * @brief RM3100数据是否转换完毕
 * @param _tSPI-软件spi句柄指针
 * @retval uint8_t-转换完毕-1  未转换完-0
*/
uint8_t OCD_GetDataReadyStatus_Soft(tagSPISoft_T *_tSPI)
{
    char data[1];

    S_SPI_Read_M3D3100_Soft(_tSPI,M3D_3100_STATUS, (uint8_t*) data,1);

    return (data[0] & STATUS_MASK);
}

/**
 * @brief RM3100设置连续测量模式
 * @param _tSPI-软件spi句柄指针
 * @param _ucConf-命令
 * @retval Null
*/
void OCD_ContinuousModeConfig_Soft(tagSPISoft_T *_tSPI,uint8_t _ucConf) 
{
    uint8_t *ptr;

    ptr = &_ucConf ;
    S_SPI_Write_M3D3100_Soft(_tSPI,M3D_3100_CMM, ptr,1);
}

/**
 * @brief RM3100为连续测量模式时，设置数据测量速率
 * @param _tSPI-软件spi句柄指针
 * @param _usValue-命令
 * @retval uint8_t-成功-1  失败-0
*/
uint8_t OCD_SetCycleCount_Soft(tagSPISoft_T *_tSPI,uint16_t _usValue)
{
    uint8_t to_reg[6];

    if (_usValue > 65535 )
        return 0;
    else
	{
		if (_usValue > 400) _usValue=400;
		else if (_usValue < 30) _usValue=30;

		to_reg[0] = _usValue>>8;
		to_reg[1] = _usValue;
		to_reg[2] = to_reg[0];
		to_reg[3] = to_reg[1];
		to_reg[4] = to_reg[0];
		to_reg[5] = to_reg[1];

		S_SPI_Write_M3D3100_Soft(_tSPI,M3D_3100_CCX, to_reg,6);
    }
    return 1;
}

/**
 * @brief RM3100为连续测量模式时，设置数据转换速率
 * @param _tSPI-软件spi句柄指针
 * @param _ucConf-命令
 * @retval uint8_t-成功-1  失败-0
*/
uint8_t OCD_SetCMMdatarate_Soft(tagSPISoft_T *_tSPI,uint8_t _ucConf)
{
    if (_ucConf < CMM_UPDATERATE_600 || _ucConf > CMM_UPDATERATE_0_075)
        return 0;
    else{
		uint8_t *ptr;
		float temp = 1000;

		switch (_ucConf)
		{
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

      	ptr = &_ucConf;
      	S_SPI_Write_M3D3100_Soft(_tSPI,M3D_3100_TMRC, ptr,1);
    }
   	return 1;
}

/**
 * @brief 设定RM3100测量模式
 * @param _tSPI-软件spi句柄指针
 * @retval Null
*/
void OCD_ThreeD3100_Magic_Init_Soft(tagSPISoft_T *_tSPI)
{
	#ifdef  RM3100_SINGLE
	OCD_SingleModeConfig_Soft(_tSPI,CMM_ALL_AXIS_ON);     /* 单测模式 */
	#endif
	#ifndef RM3100_SINGLE
  	OCD_ContinuousModeConfig_Soft(_tSPI,CMM_ALL_AXIS_ON|DRDY_WHEN_ALL_AXIS_MEASURED|CM_START); 			/* 设置测试轴,drdy 开启连续模式 */

  	OCD_SetCycleCount_Soft(_tSPI,200);												
  	OCD_SetCMMdatarate_Soft(_tSPI,12);
	#endif
}

/**
 * @brief 软件spi驱动RM3100获取数据
 * @param _tSPI-软件spi句柄指针
 * @param _tBuff-存放获得数据
 * @retval Null
*/
void OCD_ThreeD3100_Magic_GetData_Soft(tagSPISoft_T *_tSPI,MagData_t *_tBuff)
{
	uint8_t i;
	uint8_t temp[9]={0};
	int32_t Mag_Data[3]={0};

	while(OCD_GetDataReadyStatus_Soft(_tSPI)==0);
	Drv_SPI_NSS(_tSPI,1); 
	S_SPI_Read_M3D3100_Soft(_tSPI,0x24,temp,9);
	Mag_Data[0]=temp[0]<<16 | temp[1]<<8 | temp[2];			
	Mag_Data[1]=temp[3]<<16 | temp[4]<<8 | temp[5];
	Mag_Data[2]=temp[6]<<16 | temp[7]<<8 | temp[8];

	for(i=0;i<3;i++)
	{
		if(Mag_Data[i]&0x00800000)
		Mag_Data[i]|=0xff000000;
	}
	
	_tBuff->MAG_X = Mag_Data[1]*13;
	_tBuff->MAG_Y = -Mag_Data[0]*13;
	_tBuff->MAG_Z = -Mag_Data[2]*13; 
}



