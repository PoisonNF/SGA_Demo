/****************************************************************************

* Sigma团队

* 文件名: ocd_rm3100.c

* 内容简述：rm3100模块文件

* 文件历史：

* 版本号		日期	  作者		说明
* 2.2.2		2023-04-09	鲍程璐	代码重构，统一API

* 1.1.8		2022-10-25	李豪	创建该文件

****************************************************************************/
#include "ocd_rm3100.h"

/* RM3100初始化数据 */
struct config tRM3100 = 
{
    .ucCycle_Count  = 200,
    .fSample_Rate   = 37,
    .fMax_Data_Rate = 440,
    .fGain          = 75
};

/**
 * @brief RM3100片选(硬件SPI专用)
 * @param _tRM3100-RM3100句柄指针
 * @param _ucLevel-电平
 * @retval Null
*/
static void S_RM3100_CS(tagRM3100_T *_tRM3100,uint8_t _ucLevel)
{
	HAL_GPIO_WritePin(_tRM3100->tSPI.tGPIO[3].tGPIOPort,_tRM3100->tSPI.tGPIO[3].tGPIOInit.Pin,_ucLevel?GPIO_PIN_SET:GPIO_PIN_RESET);
}

/**
 * @brief RM3100的多字节写入
 * @param _tRM3100-RM3100句柄指针
 * @param _ucReg_addr-寄存器地址
 * @param _ucpDataIn-写入数据的地址
 * @param _ucLenth-写入数据的长度
 * @retval Null
*/
static void S_RM3100_Write(tagRM3100_T *_tRM3100,uint8_t _ucReg_addr,uint8_t *_ucpDataIn,uint8_t _ucLenth)
{
	uint8_t ucByteCnt = 0;
	uint8_t ucTemp = 0;

	if(_tRM3100->bSPIEnable == true)
	{
		S_RM3100_CS(_tRM3100,0);

		ucTemp = 0x00 + (_ucReg_addr&0x3f);
		Drv_SPI_TransmitReceive(&_tRM3100->tSPI,ucTemp);

		for(ucByteCnt = 0;ucByteCnt < _ucLenth;ucByteCnt++)
		{
			ucTemp = *(_ucpDataIn+ucByteCnt);
			Drv_SPI_TransmitReceive(&_tRM3100->tSPI,ucTemp);
		}

		S_RM3100_CS(_tRM3100,1);
	}
	else if(_tRM3100->bSPISoftEnable == true)
	{
		Drv_SPI_NSS(&_tRM3100->tSoftSPI,0);

		ucTemp = 0x00 + (_ucReg_addr&0x3f);
		Drv_SPI_WriteByte_Soft(&_tRM3100->tSoftSPI,ucTemp);

		for(ucByteCnt = 0;ucByteCnt < _ucLenth;ucByteCnt++)
		{
			ucTemp = *(_ucpDataIn+ucByteCnt);
			Drv_SPI_WriteByte_Soft(&_tRM3100->tSoftSPI,ucTemp);
		}

		Drv_SPI_NSS(&_tRM3100->tSoftSPI,1);
	}

}

/**
 * @brief RM3100的多字节读取
 * @param _tRM3100-RM3100句柄指针
 * @param _ucReg_addr-寄存器地址
 * @param _ucpDataOut-读出数据的地址
 * @param _ucLenth-读出数据的长度
 * @retval Null
*/
static void S_RM3100_Read(tagRM3100_T *_tRM3100,uint8_t _ucReg_addr,uint8_t *_ucpDataOut,uint8_t _ucLenth)
{
	uint8_t ucByteCnt = 0;
	uint8_t ucTemp = 0;
	
	if(_tRM3100->bSPIEnable == true)
	{
		S_RM3100_CS(_tRM3100,0);

		ucTemp = 0x80 + (_ucReg_addr);
		Drv_SPI_TransmitReceive(&_tRM3100->tSPI,ucTemp);
		
		for(ucByteCnt = 0;ucByteCnt < _ucLenth;ucByteCnt++)
		{
			ucTemp = Drv_SPI_TransmitReceive(&_tRM3100->tSPI,0);
			*(_ucpDataOut+ucByteCnt) = ucTemp;
		}

		S_RM3100_CS(_tRM3100,1);
	}
	else if(_tRM3100->bSPISoftEnable == true)
	{
		Drv_SPI_NSS(&_tRM3100->tSoftSPI,0);

		ucTemp = 0x80 + (_ucReg_addr);
		Drv_SPI_WriteByte_Soft(&_tRM3100->tSoftSPI,ucTemp);

		for(ucByteCnt = 0;ucByteCnt < _ucLenth;ucByteCnt++)
		{
			ucTemp = Drv_SPI_WriteByte_Soft(&_tRM3100->tSoftSPI,0);
			*(_ucpDataOut+ucByteCnt) = ucTemp;
		}

		Drv_SPI_NSS(&_tRM3100->tSoftSPI,1);
	}

}

/**
 * @brief RM3100数据是否转换完毕
 * @param _tRM3100-RM3100句柄指针
 * @retval uint8_t-转换完毕-1  未转换完-0
*/
static uint8_t S_RM3100_GetDataReadyStatus(tagRM3100_T *_tRM3100) 
{
    char cData[1];

    S_RM3100_Read(_tRM3100, M3D_3100_STATUS, (uint8_t*) cData,1);
    return (cData[0] & STATUS_MASK);
}

/**
 * @brief RM3100为连续测量模式时，设置数据循环次数
 * @param _tRM3100-RM3100句柄指针
 * @param _usValue-数量
 * @retval uint8_t-成功-1  失败-0
*/
uint8_t OCD_RM3100_SetCycleCount(tagRM3100_T *_tRM3100,uint16_t _usValue) 
{
    uint8_t ucReg[6];

    if (_usValue > 65535)
        return 0;
    else
	{
		if (_usValue > 400)
        	_usValue = 400;
		else if (_usValue < 30)
        	_usValue = 30;

		ucReg[0] = _usValue>>8;
		ucReg[1] = _usValue;
		ucReg[2] = ucReg[0];
		ucReg[3] = ucReg[1];
		ucReg[4] = ucReg[0];
		ucReg[5] = ucReg[1];

		S_RM3100_Write(_tRM3100, M3D_3100_CCX, ucReg,6);
    }
    return 1;
}

/**
 * @brief RM3100为连续测量模式时，设置数据转换速率
 * @param _tRM3100-RM3100句柄指针
 * @param _ucConf-命令
 * @retval uint8_t-成功-1  失败-0
*/
uint8_t OCD_RM3100_SetCMM_DataRate(tagRM3100_T *_tRM3100,uint8_t _ucConf) 
{
    if (_ucConf < CMM_UPDATERATE_600 || _ucConf > CMM_UPDATERATE_0_075)
        return 0;
    else{
        uint8_t *ptr;
        float fTemp = 1000;

        switch (_ucConf)
		{
            case CMM_UPDATERATE_600:   fTemp=600;
                                    break;
            case CMM_UPDATERATE_300:   fTemp=300;
                                    break;
            case CMM_UPDATERATE_150:   fTemp=150;
                                    break;
            case CMM_UPDATERATE_75 :   fTemp=75;
                                    break;
            case CMM_UPDATERATE_37 :   fTemp=37;
                                    break;
            case CMM_UPDATERATE_18 :   fTemp=18;
                                    break;
            case CMM_UPDATERATE_9  :   fTemp=9;
                                    break;
            case CMM_UPDATERATE_4_5:   fTemp=4.5;
                                    break;
            case CMM_UPDATERATE_2_3:   fTemp=2.3;
                                    break;
            case CMM_UPDATERATE_1_2:   fTemp=1.2;
                                    break;
            case CMM_UPDATERATE_0_6:   fTemp=0.6;
                                    break;
            case CMM_UPDATERATE_0_3:   fTemp=0.3;
                                    break;
            case CMM_UPDATERATE_0_15:  fTemp=0.15;
                                    break;
            case CMM_UPDATERATE_0_075: fTemp=0.075;
                                    break;
        }
        if (fTemp > tRM3100.fMax_Data_Rate)				
            return 0;

        ptr = &_ucConf;
        S_RM3100_Write(_tRM3100, M3D_3100_TMRC, ptr,1);
    }
    return 1;
}

/**
 * @brief RM3100设置连续测量模式
 * @param _tRM3100-RM3100句柄指针
 * @param _ucConf-命令
 * @retval Null
*/
void OCD_RM3100_ContinuousModeConfig(tagRM3100_T *_tRM3100,uint8_t _ucConf) 
{
    uint8_t *ptr;

    ptr = &_ucConf;
    S_RM3100_Write(_tRM3100, M3D_3100_CMM, ptr,1);
}

/**
 * @brief RM3100设置单次测量模式
 * @param _tRM3100-RM3100句柄指针
 * @param _ucConf-命令
 * @retval Null
*/
void OCD_RM3100_SingleModeConfig(tagRM3100_T *_tRM3100,uint8_t _ucConf) 
{
    uint8_t *ptr;

    ptr = &_ucConf;
    S_RM3100_Write(_tRM3100, M3D_3100_POLL, ptr,1);
}

/**
 * @brief 设定RM3100测量模式，每次读完数据后需再调用此函数
 * @param _tRM3100-RM3100句柄指针
 * @retval Null
*/
void OCD_RM3100_ModeConfig(tagRM3100_T *_tRM3100)
{
    #ifdef RM3100_SINGLE
	OCD_RM3100_SingleModeConfig(_tRM3100,CMM_ALL_AXIS_ON);     /* 单测模式 */
	#else
  	OCD_RM3100_ContinuousModeConfig(_tRM3100,CMM_ALL_AXIS_ON|DRDY_WHEN_ALL_AXIS_MEASURED|CM_START); 			/* 设置测试轴,drdy 开启连续模式 */
  	OCD_RM3100_SetCycleCount(_tRM3100,200);												
  	OCD_RM3100_SetCMM_DataRate(_tRM3100,12);
	#endif
}

/**
 * @brief RM3100获取数据，存入tMagData成员中
 * @param _tRM3100-RM3100句柄指针
 * @retval Null
*/
void OCD_RM3100_GetData(tagRM3100_T *_tRM3100)
{
	uint8_t i;
	uint8_t ucTemp[9] = {0};
	int32_t ulMag_Data[3] = {0};

	while(S_RM3100_GetDataReadyStatus(_tRM3100) == 0);

	if(_tRM3100->bSPISoftEnable == true)
		Drv_SPI_NSS(&_tRM3100->tSoftSPI,1); 
	
	S_RM3100_Read(_tRM3100,0x24,ucTemp,9);
	ulMag_Data[0] = ucTemp[0]<<16 | ucTemp[1]<<8 | ucTemp[2];			
	ulMag_Data[1] = ucTemp[3]<<16 | ucTemp[4]<<8 | ucTemp[5];
	ulMag_Data[2] = ucTemp[6]<<16 | ucTemp[7]<<8 | ucTemp[8];

	for(i = 0;i < 3;i++)
	{
		if(ulMag_Data[i]&0x00800000)
			ulMag_Data[i] |= 0xff000000;
	}
	
	_tRM3100->tMagData.MAG_X =  ulMag_Data[1]*13;
	_tRM3100->tMagData.MAG_Y = -ulMag_Data[0]*13;
	_tRM3100->tMagData.MAG_Z = -ulMag_Data[2]*13; 
}

/**
 * @brief RM3100初始化函数
 * @param _tRM3100-RM3100句柄指针
 * @param _ucNum-初始化个数
 * @retval Null
*/
void OCD_RM3100_Init(tagRM3100_T *_tRM3100,uint8_t _ucNum)
{
	uint8_t index;

	/* 根据使能标志依次进行初始化 */
	for(index = 0;index < _ucNum;index++)
	{
		/* 如果是硬件SPI驱动 */
		if(_tRM3100[index].bSPIEnable == true)
		{
			/* 如果传入空指针，报错 */
			if(&_tRM3100[index].tSPI == NULL)
			{
				Drv_HAL_Error(__FILE__, __LINE__);
				while(1);
			}
			Drv_SPI_Init(&_tRM3100[index].tSPI);
		}

		/* 如果是软件SPI驱动 */
		else if(_tRM3100[index].bSPISoftEnable == true)
		{
			/* 如果传入空指针，报错 */
			if(&_tRM3100[index].tSoftSPI == NULL)
			{
				Drv_HAL_Error(__FILE__, __LINE__);
				while(1);
			}
			Drv_SPISoft_Init(&_tRM3100[index].tSoftSPI);
		}

		/* 两者皆不是，报错 */
		else
		{
			Drv_HAL_Error(__FILE__, __LINE__);
			while(1);
		}

		/* RM3100模式配置 */
		OCD_RM3100_ModeConfig(&_tRM3100[index]);
	}
}

