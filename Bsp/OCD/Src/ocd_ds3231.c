/****************************************************************************

* Sigma团队

* 文件名: ut_ds3231.c

* 内容简述：ds3231模块文件

* 文件历史：

* 版本号	日期		作者		说明

* 1.0.0a 	2020-02-22	李环宇		创建该文件
* 1.0.1a 	2020-03-10	李环宇		修复OCD_DS3231_TimeGetASCII函数的逻辑错误所导致的指针跑飞问题
									修改OCD_DS3231_TimeGetASCII和OCD_DS3231_TimeGetDec接口以及执行逻辑
									修改部分函数接口名称，由十进制描述修改为正确的十六进制描述
									修改全部函数的首参数类型，全部改用DS3231的自结构体

****************************************************************************/
#include "ocd_ds3231.h"

#define DS3231_IIC_ADDR 	0XD0

#define DS3231_WriteAddress 0xD0    /* 器件写地址 */
#define DS3231_ReadAddress  0xD1    /* 器件读地址 */

#define DS3231_SECOND       0x00    /* 秒 */
#define DS3231_MINUTE       0x01    /* 分 */
#define DS3231_HOUR         0x02    /* 时 */
#define DS3231_WEEK         0x03    /* 星期 */
#define DS3231_DAY          0x04    /* 日 */
#define DS3231_MONTH        0x05    /* 月 */
#define DS3231_YEAR         0x06    /* 年 */

/* 闹铃1寄存器 */            
#define DS3231_ALARM1SECOND 0x07    /* 秒 */
#define DS3231_ALARM1MINUTE 0x08    /* 分 */
#define DS3231_ALARM1HOUR   0x09    /* 时 */
#define DS3231_ALARM1WEEK   0x0A    /* 星期/日 */

/* 闹铃2寄存器 */
#define DS3231_ALARM2MINUTE 0x0b    /* 分 */
#define DS3231_ALARM2HOUR   0x0c    /* 时 */
#define DS3231_ALARM2WEEK   0x0d    /* 星期/日 */

#define DS3231_CONTROL      0x0e    /* 控制寄存器 */
#define DS3231_STATUS       0x0f    /* 状态寄存器 */
#define BSY                 2       /* 忙 */
#define OSF                 7       /* 振荡器停止标志 */
#define DS3231_XTAL         0x10    /* 晶体老化寄存器 */
#define DS3231_TEMPERATUREH 0x11    /* 温度寄存器高字节(8位) */
#define DS3231_TEMPERATUREL 0x12    /* 温度寄存器低字节(高2位) */

/**
 * @brief DS3231内部延时函数
 * @param 	_ulUs-延时（单位：us）
 * @retval	NULL
 */
static void S_DS3231_DelayUs(uint8_t _ulUs)
{
	int i,j;
	for(i = 0; i < _ulUs; i++)
		for(j = 0; j < 12; j++);
}

/**
 * @brief 	向DS3231读数据
 * @param 	*_tDS3231-ds3231结构体指针
 * @param 	_ucDevAddr-地址
 * @param	_ucReg-寄存器
 * @retval	0-读失败；其它-寄存器数据
 */
static uint8_t S_DS3231_ReadByte(tagDS3231_T *_tDS3231, uint8_t _ucDevAddr, uint8_t _ucReg)
{
	uint8_t res = 0;
	
	Drv_IICSoft_Start(&_tDS3231->tIICSoft);
	
	Drv_IICSoft_SendByte(&_tDS3231->tIICSoft,_ucDevAddr);
	if(Drv_IICSoft_WaitAck(&_tDS3231->tIICSoft)!=0)
		goto cmd_fail;
	
	Drv_IICSoft_SendByte(&_tDS3231->tIICSoft,_ucReg);
	if(Drv_IICSoft_WaitAck(&_tDS3231->tIICSoft)!=0)
		goto cmd_fail;
	
	Drv_IICSoft_Start(&_tDS3231->tIICSoft);
	S_DS3231_DelayUs(1);
	Drv_IICSoft_SendByte(&_tDS3231->tIICSoft,_ucDevAddr|0x01);
	if(Drv_IICSoft_WaitAck(&_tDS3231->tIICSoft)!=0)
		goto cmd_fail;
	
	res = Drv_IICSoft_ReadByte(&_tDS3231->tIICSoft,1);
	
	Drv_IICSoft_Stop(&_tDS3231->tIICSoft);
	
	return res;
	
	cmd_fail:
	
	Drv_IICSoft_Stop(&_tDS3231->tIICSoft);
	
	return 0;
}

/**
 * @brief 向DS3231写数据
 * @param 	*_tDS3231-ds3231结构体指针
 * @param 	_ucDevAddr-地址
 * @param	_ucReg-寄存器
 * @param	cData-数据
 * @retval	0-写失败；1-写成功
 */
static uint8_t S_DS3231_WriteByte(tagDS3231_T *_tDS3231, uint8_t _ucDevAddr, uint8_t _ucReg, uint8_t _ucData)
{
	Drv_IICSoft_Start(&_tDS3231->tIICSoft);
	_ucDevAddr &= 0xFE;
		
	Drv_IICSoft_SendByte(&_tDS3231->tIICSoft, _ucDevAddr);
	if(Drv_IICSoft_WaitAck(&_tDS3231->tIICSoft)!=0)
		goto cmd_fail;
		
	Drv_IICSoft_SendByte(&_tDS3231->tIICSoft, _ucReg);
	if(Drv_IICSoft_WaitAck(&_tDS3231->tIICSoft)!=0)
		goto cmd_fail;
	
	Drv_IICSoft_SendByte(&_tDS3231->tIICSoft, _ucData);
	if(Drv_IICSoft_WaitAck(&_tDS3231->tIICSoft)!=0)
		goto cmd_fail;
	
	Drv_IICSoft_Stop(&_tDS3231->tIICSoft);
	
	return(1);     
	
	cmd_fail:
	Drv_IICSoft_Stop(&_tDS3231->tIICSoft);
	return(0);
}

/**
 * @brief 向DS3231时间设置（十六进制）
 * @param 	*_tDS3231-ds3231结构体指针
 * @param	*_tTime-时间结构体地址指针
 * @retval	0-设置失败；1-设置成功
 */
uint8_t OCD_DS3231_TimeSetHex(tagDS3231_T *_tDS3231, tagDS3231Time_T *_tTime)
{
	do
	{
		/* 设置年 */
		if(S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_YEAR,(_tTime->ucYear)) == 0)
			return 0;
		
		/* 设置月 */
		if(S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_MONTH,(_tTime->ucMonth)) == 0)
			return 0;
		
		/* 设置日 */
		if(S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_DAY,(_tTime->ucDate)) == 0)
			return 0;
		
		/* 设置时 */
		if(S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_HOUR,(_tTime->ucHour)) == 0)
			return 0;
		
		/* 设置分 */
		if(S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_MINUTE,(_tTime->ucMinute)) == 0)
			return 0;
		
		/* 设置秒 */
		if(S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_SECOND,(_tTime->ucSecond)) == 0)
			return 0;
		
		/* 设置星期几 */
		if(S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_WEEK,(_tTime->ucWeek)) == 0)
			return 0;
	}while(0);
	
	return 1;
}

//static uint32_t S_DS3231_StringToHex(uint8_t *_ucpSrc, uint8_t _ucLen)
//{
//	uint8_t index;
//	uint32_t ulRes;

//	for(index = 0; index < _ucLen; index++)
//		ulRes += (_ucpSrc[index] - '0') << (_ucLen - index);

//	return ulRes;
//}

/**
 * @brief 向DS3231时间设置（ASCII）
 * @param 	*_tDS3231-ds3231结构体指针
 * @param	*_tTime-时间结构体地址指针
 * @retval	0-设置失败；1-设置成功
 */
uint8_t OCD_DS3231_TimeSetASCII(tagDS3231_T *_tDS3231, tagDS3231TimeASCII_T *_tTime)
{
//	tagDS3231Time_T tTime;
//	
//	tTime.ucYear = S_DS3231_StringToHex(_tTime->ucYear, 2);
//	tTime.ucMonth = S_DS3231_StringToHex(_tTime->ucMonth, 2);
//	tTime.ucDate = S_DS3231_StringToHex(_tTime->ucDate, 2);
//	tTime.ucHour = S_DS3231_StringToHex(_tTime->ucHour, 2);
//	tTime.ucMinute = S_DS3231_StringToHex(_tTime->ucMinute, 2);
//	tTime.ucSecond = S_DS3231_StringToHex(_tTime->ucSecond, 2);
	
	return 1;
}

/**
 * @brief 向DS3231时间读取（十六进制）
 * @param 	*_tDS3231-ds3231结构体指针
 * @param	*_tTime-时间结构体地址指针
 * @retval	0-设置失败；1-设置成功
 */
uint8_t OCD_DS3231_TimeGetHex(tagDS3231_T *_tDS3231, tagDS3231Time_T *_tTime)
{	
	_tTime->ucYear = S_DS3231_ReadByte(_tDS3231, DS3231_IIC_ADDR, DS3231_YEAR);		/* 年 */
	_tTime->ucMonth = S_DS3231_ReadByte(_tDS3231, DS3231_IIC_ADDR, DS3231_MONTH);		/* 月 */
	_tTime->ucDate = S_DS3231_ReadByte(_tDS3231, DS3231_IIC_ADDR, DS3231_DAY);			/* 日 */
	_tTime->ucHour = S_DS3231_ReadByte(_tDS3231, DS3231_IIC_ADDR, DS3231_HOUR);		/* 时 */
	_tTime->ucMinute = S_DS3231_ReadByte(_tDS3231, DS3231_IIC_ADDR, DS3231_MINUTE);	/* 分 */
	_tTime->ucSecond = S_DS3231_ReadByte(_tDS3231, DS3231_IIC_ADDR, DS3231_SECOND);	/* 秒 */
	_tTime->ucWeek = S_DS3231_ReadByte(_tDS3231, DS3231_IIC_ADDR, DS3231_WEEK);		/* 星期几 */
	
	return 1;
}

/**
 * @brief 向DS3231时间读取（ASCII）
 * @param 	*_tDS3231-ds3231结构体指针
 * @param	*_tTime-时间结构体地址指针
 * @retval	0-设置失败；1-设置成功
 */
uint8_t OCD_DS3231_TimeGetASCII(tagDS3231_T *_tDS3231, tagDS3231TimeASCII_T *_tTime)
{
	tagDS3231Time_T tTime;

	OCD_DS3231_TimeGetHex(_tDS3231, &tTime);
	
	_tTime->ucYear[0] = (tTime.ucYear >> 4) + '0';
	_tTime->ucYear[1] = (tTime.ucYear & 0x0f) + '0';
	_tTime->ucMonth[0] = (tTime.ucMonth >> 4) + '0';
	_tTime->ucMonth[1] = (tTime.ucMonth & 0x0f) + '0';
	_tTime->ucDate[0] = (tTime.ucDate >> 4) + '0';
	_tTime->ucDate[1] = (tTime.ucDate & 0x0f) + '0';
	_tTime->ucHour[0] = (tTime.ucHour >> 4) + '0';
	_tTime->ucHour[1] = (tTime.ucHour & 0x0f) + '0';
	_tTime->ucMinute[0] = (tTime.ucMinute >> 4) + '0';
	_tTime->ucMinute[1] = (tTime.ucMinute & 0x0f) + '0';
	_tTime->ucSecond[0] = (tTime.ucSecond >> 4) + '0';
	_tTime->ucSecond[1] = (tTime.ucSecond & 0x0f) + '0';
	_tTime->ucWeek[0] = (tTime.ucWeek >> 4) + '0';
	_tTime->ucWeek[1] = (tTime.ucWeek & 0x0f) + '0';
	
	return 1;
}

/**
 * @brief 	DS3231闹钟设置（ASCII）
 * @param 	*_tDS3231-ds3231结构体指针
 * @param	*_tTime-时间结构体地址指针
 * @retval	0-设置失败；1-设置成功
 */
void OCD_DS3231_Alarm1ConfigASCII(tagDS3231_T *_tDS3231, tagDS3231TimeASCII_T *_tTime)
{	
	uint8_t ucRes;
	uint8_t ucHour, ucMin, ucSec = 0;
	
	ucHour = ((_tTime->ucHour[0] - '0') << 4) | (_tTime->ucHour[1] - '0');
	ucMin = ((_tTime->ucMinute[0] - '0') << 4) | (_tTime->ucMinute[1] - '0');
	ucSec = ((_tTime->ucSecond[0] - '0') << 4) | (_tTime->ucSecond[1] - '0');
	
	S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_ALARM1WEEK,0X80);
	S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_ALARM1HOUR,(ucHour));
	S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_ALARM1MINUTE,(ucMin));
	S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_ALARM1SECOND,(ucSec));
	
	ucRes = S_DS3231_ReadByte(_tDS3231, DS3231_IIC_ADDR, DS3231_CONTROL);
	S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_CONTROL, ucRes|0x05);
	
	ucRes = S_DS3231_ReadByte(_tDS3231, DS3231_IIC_ADDR, DS3231_STATUS);
	S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_STATUS, ucRes&0xfC);////闹钟1、2标志位清零
	
	//DS3231_Interrupt_ENABLE(Interrupt_EXTI_X_IRQn,ENABLE);
}

/**
 * @brief	DS3231闹钟设置（Hex）
 * @param 	*_tDS3231-iic结构体指针
 * @param	*_tTime-时间结构体地址指针
 * @retval	0-设置失败；1-设置成功
 */
void OCD_DS3231_Alarm1ConfigHex(tagDS3231_T *_tDS3231, tagDS3231Time_T *_tTime)
{	
	uint8_t ucRes;
	
	S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_ALARM1WEEK,0X80);
	S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_ALARM1HOUR,(_tTime->ucHour));
	S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_ALARM1MINUTE,(_tTime->ucMinute));
	S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_ALARM1SECOND,(_tTime->ucSecond));
	
	ucRes = S_DS3231_ReadByte(_tDS3231, DS3231_IIC_ADDR, DS3231_CONTROL);
	S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_CONTROL, ucRes|0x05);
	
	ucRes = S_DS3231_ReadByte(_tDS3231, DS3231_IIC_ADDR, DS3231_STATUS);
	S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_STATUS, ucRes&0xfC);////闹钟1、2标志位清零
	
	//DS3231_Interrupt_ENABLE(Interrupt_EXTI_X_IRQn,ENABLE);
}

/**
 * @brief DS3231初始化
 * @param _tDS3231-ds3231结构体指针
 * @retval None
 */
void OCD_DS3231_Init(tagDS3231_T *_tDS3231)
{
	Drv_IICSoft_Init(&_tDS3231->tIICSoft);
	Drv_Delay_Ms(200);
	
	S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_CONTROL, 0x04);
	Drv_Delay_Ms(200);
	
	S_DS3231_WriteByte(_tDS3231, DS3231_IIC_ADDR, DS3231_STATUS, 0);
	Drv_Delay_Ms(200);
}
