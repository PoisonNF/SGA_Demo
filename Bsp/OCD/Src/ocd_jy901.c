/****************************************************************************

* Sigma团队

* 文件名: ocd_jy901.c

* 内容简述：jy901模块文件

* 文件历史：

* 版本号		日期	  作者			说明
*  2.5 		2023-05-17  鲍程璐		数据处理函数增加返回值

* 2.2.1		2023-04-03  鲍程璐		跟随drv_hal_uart进行修改

* 1.1.8		2022-10-22	鲍程璐		对结构体进行了整合，提供IT模式和DMA模式
									提供jy901数据的处理、转换、打印函数

* 1.0.0a 	2020-03-14	李环宇		创建该文件

* 1.1.0a 	2020-03-29	李环宇		修改部分文本

****************************************************************************/
#include "ocd_jy901.h"

const uint8_t ucpUnlockCmd[] = {0xff, 0xaa, 0x69, 0x88, 0xb5}; 
/**
 * @brief JY901发送解锁指令函数
 * @param _tJY901-JY901句柄指针
 * @retval Null
*/
static void S_JY901_UnLock(tagJY901_T *_tJY901)
{
	Drv_Uart_Transmit(&_tJY901->tUART, (uint8_t*)ucpUnlockCmd, sizeof(ucpUnlockCmd));
}

/**
 * @brief JY901延时函数
 * @param Null
 * @retval Null
*/
static void S_JY901_Delay(void)
{
	Drv_Delay_Ms(20);
}

/**
 * @brief JY901保存配置函数
 * @param _tJY901-JY901句柄指针
 * @param _ucSet-SAVE_NOW 保存当前;SAVE_RESET 重新启动;SAVE_DEFAULT 恢复出厂设置;
 * @retval Null
*/
static void S_JY901_SaveConfig(tagJY901_T *_tJY901, uint8_t _ucSet)
{
	uint8_t ucpWrite[] = {0xff, 0xaa, 0x00, 0x00, 0x00};
	
	switch(_ucSet)
	{
		case SAVE_NOW:
			ucpWrite[3] = SAVE_NOW;
		break;
		
		case SAVE_DEFAULT:
			ucpWrite[3] = SAVE_DEFAULT;
		break;

		case SAVE_RESET:
			ucpWrite[3] = SAVE_RESET;
		
		default:
			
		break;
	}
	Drv_Uart_Transmit(&_tJY901->tUART, ucpWrite, sizeof(ucpWrite));
}

/**
 * @brief JY901回传内容配置
 * @param _tJY901-JY901句柄指针
 * @retval Null
*/
void OCD_JY901_RxTypeConfig(tagJY901_T *_tJY901)
{
	uint8_t ucpWrite[] = {0xff, 0xaa, 0x02, 0x00, 0x00};
	
	ucpWrite[3] = _tJY901->tConfig.usType >> 8;
	ucpWrite[4] = _tJY901->tConfig.usType;
	
	S_JY901_UnLock(_tJY901);
	S_JY901_Delay();
	
	Drv_Uart_Transmit(&_tJY901->tUART, ucpWrite, sizeof(ucpWrite));
	S_JY901_Delay();
	
	S_JY901_SaveConfig(_tJY901, SAVE_NOW);		
}

/**
 * @brief JY901校准函数
 * @param _tJY901-JY901句柄指针
 * @param _ucMode-校准模式		
 * @retval Null
*/
void OCD_JY901_Correct(tagJY901_T *_tJY901, uint8_t _ucMode)
{
	uint8_t ucpWrite[] = {0xff, 0xaa, 0x01, 0x00, 0x00};
	
	ucpWrite[3] = _ucMode;
	
	S_JY901_UnLock(_tJY901);
	S_JY901_Delay();
	
	Drv_Uart_Transmit(&_tJY901->tUART, ucpWrite, sizeof(ucpWrite));
	S_JY901_Delay();
	
	S_JY901_SaveConfig(_tJY901, SAVE_NOW);
}

const uint8_t ucpSleepCmd[] = {0xff, 0xaa, 0x22, 0x01, 0x00}; 
/**
 * @brief JY901休眠函数（休眠后再次调用为解除休眠）
 * @param _tJY901-JY901句柄指针
 * @retval Null
*/
void OCD_JY901_Sleep(tagJY901_T *_tJY901)
{
	S_JY901_UnLock(_tJY901);
	S_JY901_Delay();
	
	Drv_Uart_Transmit(&_tJY901->tUART, (uint8_t*)ucpSleepCmd, sizeof(ucpSleepCmd));
	S_JY901_Delay();
	
	S_JY901_SaveConfig(_tJY901, SAVE_NOW);
}

/**
 * @brief JY901回传速率配置
 * @param _tJY901-JY901句柄指针
 * @retval Null
*/
void OCD_JY901_RxSpeedConfig(tagJY901_T *_tJY901)
{
	uint8_t ucpWrite[] = {0xff, 0xaa, 0x03, 0x00, 0x00};
	
	ucpWrite[3] = _tJY901->tConfig.ucRate;
	
	S_JY901_UnLock(_tJY901);
	S_JY901_Delay();
	
	Drv_Uart_Transmit(&_tJY901->tUART, ucpWrite, sizeof(ucpWrite));
	S_JY901_Delay();
	
	S_JY901_SaveConfig(_tJY901, SAVE_NOW);		
}

/**
 * @brief JY901回传波特率配置
 * @param _tJY901-JY901句柄指针
 * @retval Null
*/
void OCD_JY901_RxBaudConfig(tagJY901_T *_tJY901)
{
	uint8_t ucpWrite[] = {0xff, 0xaa, 0x04, 0x00, 0x00};
	
	ucpWrite[3] = _tJY901->tConfig.ucBaud;
	
	S_JY901_UnLock(_tJY901);
	S_JY901_Delay();
	
	Drv_Uart_Transmit(&_tJY901->tUART, ucpWrite, sizeof(ucpWrite));
	S_JY901_Delay();
	
	S_JY901_SaveConfig(_tJY901, SAVE_NOW);		
}

/**
 * @brief JY901陀螺仪自动校准函数
 * @param _tJY901-JY901句柄指针
 * @param _ucMode-1-禁用自动校准;0-启用自动校准
 * @retval Null
*/
void OCD_JY901_GyroAutoCorrect(tagJY901_T *_tJY901, uint8_t _ucMode)
{
	uint8_t ucpWrite[] = {0xff, 0xaa, 0x63, 0x00, 0x00};
	
	ucpWrite[3] = _ucMode;
	
	S_JY901_UnLock(_tJY901);
	S_JY901_Delay();
	
	Drv_Uart_Transmit(&_tJY901->tUART, ucpWrite, sizeof(ucpWrite));
	S_JY901_Delay();
	
	S_JY901_SaveConfig(_tJY901, SAVE_NOW);
}

const uint8_t ucpOutputOnceCmd[] = {0xff, 0xaa, 0x03, 0x0c, 0x00}; 
/**
 * @brief JY901单次输出（需设置回传速率为单次输出）
 * @param _tJY901-JY901句柄指针
 * @retval Null
*/
void OCD_JY901_OutputOnce(tagJY901_T *_tJY901)
{
	S_JY901_UnLock(_tJY901);
	S_JY901_Delay();
	
	Drv_Uart_Transmit(&_tJY901->tUART, (uint8_t*)ucpOutputOnceCmd, sizeof(ucpOutputOnceCmd));
	S_JY901_Delay();
	
	S_JY901_SaveConfig(_tJY901, SAVE_NOW);
}

/**
 * @brief JY901中断初始化函数
 * @param _tJY901-JY901句柄指针
 * @retval Null
*/
void OCD_JY901_ITInit(tagJY901_T *_tJY901)
{
	Drv_Uart_ITInit(&_tJY901->tUART);

	OCD_JY901_RxBaudConfig(_tJY901);
	OCD_JY901_RxSpeedConfig(_tJY901);
	OCD_JY901_RxTypeConfig(_tJY901);
}

/**
 * @brief JY901DMA初始化函数
 * @param _tJY901-JY901句柄指针
 * @retval Null
*/
void OCD_JY901_DMAInit(tagJY901_T *_tJY901)
{
	Drv_Uart_DMAInit(&_tJY901->tUART);

	OCD_JY901_RxBaudConfig(_tJY901);
	OCD_JY901_RxSpeedConfig(_tJY901);
	OCD_JY901_RxTypeConfig(_tJY901);
}

/**
 * @brief JY901数据处理函数,按照数据类型放入相对应的结构体成员中
 * @param _tJY901-JY901句柄指针
* @retval uint8_t 1:处理成功 0:未收到数据
*/
uint8_t OCD_JY901_DataProcess(tagJY901_T *_tJY901)
{
	/* 判断标志位 */
	if(_tJY901->tUART.tRxInfo.ucDMARxCplt)
	{
		/* 遍历数组 */
		for (int i = 0; i < _tJY901->tUART.tRxInfo.usDMARxLength; i++)
		{
			if(_tJY901->tUART.tRxInfo.ucpDMARxCache[i] == JY901_HEAD) /* 如果数据头为0x55 */
			{
				/* 和校验 */
				uint8_t ucSum = 0;
				for(int j = 0;j<10;j++)
				{
					ucSum += _tJY901->tUART.tRxInfo.ucpDMARxCache[i+j];
				}
				/* 和校验成功，为需要的数据组 */
				if(ucSum == _tJY901->tUART.tRxInfo.ucpDMARxCache[i+10])
				{
					/* 根据type拷贝到对应的结构体中 */
					switch (_tJY901->tUART.tRxInfo.ucpDMARxCache[i+1])
					{
						case JY901_TIME:	memcpy(&_tJY901->stcTime,&_tJY901->tUART.tRxInfo.ucpDMARxCache[i+2],8);
											break;
						case JY901_ACCEL:	memcpy(&_tJY901->stcAcc,&_tJY901->tUART.tRxInfo.ucpDMARxCache[i+2],8);
											break;
						case JY901_GYRO:	memcpy(&_tJY901->stcGyro,&_tJY901->tUART.tRxInfo.ucpDMARxCache[i+2],8);
											break;
						case JY901_ANGLE:	memcpy(&_tJY901->stcAngle,&_tJY901->tUART.tRxInfo.ucpDMARxCache[i+2],8);
											break;
						case JY901_MAG:		memcpy(&_tJY901->stcMag,&_tJY901->tUART.tRxInfo.ucpDMARxCache[i+2],8);
											break;
						case JY901_QUATER:  memcpy(&_tJY901->stcQuater,&_tJY901->tUART.tRxInfo.ucpDMARxCache[i+2],8);
											break;
						default:	
											break;
					}
				}
				else continue;
			}
		}
		_tJY901->tUART.tRxInfo.ucDMARxCplt = 0;	/* 标志位清零 */
        return 1;
	}
    return 0;
}

/**
 * @brief JY901数据转换函数,按照数据转换为可读,存储至对应Con成员中
 * @param _tJY901-JY901句柄指针
 * @retval Null
*/
void OCD_JY901_DataConversion(tagJY901_T *_tJY901)
{
	/* 读取加速度 */
	if(_tJY901->tConfig.usType & JY901_OUTPUT_ACCEL)
	{
		_tJY901->stcAcc.ConAccX = (float)_tJY901->stcAcc.AccX /32768*16;
		_tJY901->stcAcc.ConAccY = (float)_tJY901->stcAcc.AccY /32768*16;
		_tJY901->stcAcc.ConAccZ = (float)_tJY901->stcAcc.AccZ /32768*16;
	}
	/* 读取陀螺仪 */
	if(_tJY901->tConfig.usType & JY901_OUTPUT_GYRO)
	{
		_tJY901->stcGyro.ConGyroX = (float)_tJY901->stcGyro.GyroX/32768*2000;
		_tJY901->stcGyro.ConGyroY = (float)_tJY901->stcGyro.GyroY/32768*2000;
		_tJY901->stcGyro.ConGyroZ = (float)_tJY901->stcGyro.GyroZ/32768*2000;
	}
	/* 读取欧拉角 */
	if (_tJY901->tConfig.usType & JY901_OUTPUT_ANGLE)
	{
		_tJY901->stcAngle.ConRoll = (float)_tJY901->stcAngle.Roll/32768*180;
		_tJY901->stcAngle.ConPitch = (float)_tJY901->stcAngle.Pitch/32768*180;
		_tJY901->stcAngle.ConYaw = (float)_tJY901->stcAngle.Yaw/32768*180;
	}
	/* 读取磁场 */
	if (_tJY901->tConfig.usType & JY901_OUTPUT_MAG)
	{
		_tJY901->stcMag.ConMagX = (float)_tJY901->stcMag.MagX;
		_tJY901->stcMag.ConMagY = (float)_tJY901->stcMag.MagY;
		_tJY901->stcMag.ConMagZ = (float)_tJY901->stcMag.MagZ;
	}
	/* 读取四元数 */
	if(_tJY901->tConfig.usType & JY901_OUTPUT_QUATER)
	{
		_tJY901->stcQuater.Conq0 = (float)_tJY901->stcQuater.q0/32768;
		_tJY901->stcQuater.Conq1 = (float)_tJY901->stcQuater.q1/32768;
		_tJY901->stcQuater.Conq2 = (float)_tJY901->stcQuater.q2/32768;
		_tJY901->stcQuater.Conq3 = (float)_tJY901->stcQuater.q3/32768;
	}
}

/**
 * @brief JY901向串口打印处理完成的数据，使用printf或者rt_printf
 * @param _tJY901-JY901句柄指针
 * @retval Null
*/
void OCD_JY901_Printf(tagJY901_T *_tJY901)
{
	/* 打印时间 */
	if(_tJY901->tConfig.usType & JY901_OUTPUT_TIME)		printf("time: %d年%d月%d日%d时%d分%d秒\r\n",_tJY901->stcTime.Year,_tJY901->stcTime.Month,_tJY901->stcTime.Day,_tJY901->stcTime.Hour,_tJY901->stcTime.Min,_tJY901->stcTime.Sec);
	/* 打印加速度 */
	if(_tJY901->tConfig.usType & JY901_OUTPUT_ACCEL)	printf("Acc:  %.3f %.3f %.3f\r\n",_tJY901->stcAcc.ConAccX,_tJY901->stcAcc.ConAccY,_tJY901->stcAcc.ConAccZ);
	/* 打印角速度 */
	if(_tJY901->tConfig.usType & JY901_OUTPUT_GYRO)		printf("Gyro: %.3f %.3f %.3f\r\n",_tJY901->stcGyro.ConGyroX,_tJY901->stcGyro.ConGyroY,_tJY901->stcGyro.ConGyroZ);
	/* 打印欧拉角 */
	if(_tJY901->tConfig.usType & JY901_OUTPUT_ANGLE)	printf("Angle:%.3f %.3f %.3f\r\n",_tJY901->stcAngle.ConRoll,_tJY901->stcAngle.ConPitch,_tJY901->stcAngle.ConYaw);
	/* 打印磁场 */
	if(_tJY901->tConfig.usType & JY901_OUTPUT_MAG)		printf("Mag:  %.3f %.3f %.3f\r\n",_tJY901->stcMag.ConMagX,_tJY901->stcMag.ConMagY,_tJY901->stcMag.ConMagZ);
	/* 打印四元数 */
	if(_tJY901->tConfig.usType & JY901_OUTPUT_QUATER)	printf("Quater:  %.3f %.3f %.3f %.3f\r\n",_tJY901->stcQuater.Conq0,_tJY901->stcQuater.Conq1,_tJY901->stcQuater.Conq2,_tJY901->stcQuater.Conq3);
}
