/****************************************************************************

* Sigma团队

* 文件名: ocd_at24c02.c

* 内容简述：at24c02模块文件

* 文件历史：

* 版本号	日期		作者		说明

* 1.0.0a 	2020-03-14	李环宇		创建该文件
* 1.1.0a 	2020-03-29	李环宇		修改部分文本，修改OCD_AT24CXX_WriteLenByte和OCD_AT24CXX_ReadLenByte参数

****************************************************************************/
#include "ocd_at24cxx.h"

#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767  

#define EE_TYPE AT24C02

/**
 * @brief 	AT24CXX指定地址开始读一个字节数据
 * @param 	*_tAT24CXX-AT24CXX结构体
 * @param 	_usAddr-地址
 * @retval	uint8_t-读到的数据
 */
uint8_t S_AT24CXX_ReadOneByte(tagAT24CXX_T *_tAT24CXX, uint16_t _usAddr)
{				  
	uint8_t temp=0;		  	    																 
    Drv_IICSoft_Start(&_tAT24CXX->tIIC);  
	if(EE_TYPE>AT24C16)
	{
		Drv_IICSoft_SendByte(&_tAT24CXX->tIIC, 0XA0);	/* 发送写命令 */
		Drv_IICSoft_WaitAck(&_tAT24CXX->tIIC);
		Drv_IICSoft_SendByte(&_tAT24CXX->tIIC, _usAddr>>8);	/* 发送高地址 */    
	}
	else 
		Drv_IICSoft_SendByte(&_tAT24CXX->tIIC, 0XA0+((_usAddr/256)<<1));   //发送器件地址0XA0,写数据
	
	Drv_IICSoft_WaitAck(&_tAT24CXX->tIIC);
    Drv_IICSoft_SendByte(&_tAT24CXX->tIIC, _usAddr % 256);   //发送低地址
	Drv_IICSoft_WaitAck(&_tAT24CXX->tIIC);	    
	Drv_IICSoft_Start(&_tAT24CXX->tIIC);  	 	   
	Drv_IICSoft_SendByte(&_tAT24CXX->tIIC, 0XA1);           //进入接收模式			   
	Drv_IICSoft_WaitAck(&_tAT24CXX->tIIC);	 
    temp = Drv_IICSoft_ReadByte(&_tAT24CXX->tIIC, 0);		   
    Drv_IICSoft_Stop(&_tAT24CXX->tIIC);//产生一个停止条件	
    
	return temp;
}

/**
 * @brief 	AT24CXX延时
 * @param 	_ucMs-延时时间
 * @retval	Null
 */
static void S_AT24CXX_Delay(uint8_t _ucMs)
{
	Drv_Delay_Ms(_ucMs);
}

/**
 * @brief 	AT24CXX指定地址开始写入一个字节数据
 * @param 	*_tAT24CXX-AT24CXX结构体
 * @param 	_usAddr-地址
 * @param 	_ucData-写入的撒胡菊
 * @retval	Null
 */
static void S_AT24CXX_WriteOneByte(tagAT24CXX_T *_tAT24CXX, uint16_t _usAddr, uint8_t _ucData)
{				   	  	    																 
    Drv_IICSoft_Start(&_tAT24CXX->tIIC); 
	if(EE_TYPE>AT24C16)
	{
		Drv_IICSoft_SendByte(&_tAT24CXX->tIIC, 0XA0);	    //发送写命令
		Drv_IICSoft_WaitAck(&_tAT24CXX->tIIC);
		Drv_IICSoft_SendByte(&_tAT24CXX->tIIC, _usAddr>>8);//发送高地址	  
	}
	else 
		Drv_IICSoft_SendByte(&_tAT24CXX->tIIC, 0XA0 + ((_usAddr/256)<<1));   //发送器件地址0XA0,写数据 	
	
	Drv_IICSoft_WaitAck(&_tAT24CXX->tIIC);	   
    Drv_IICSoft_SendByte(&_tAT24CXX->tIIC, _usAddr % 256);   //发送低地址
	Drv_IICSoft_WaitAck(&_tAT24CXX->tIIC); 	 										  		   
	Drv_IICSoft_SendByte(&_tAT24CXX->tIIC, _ucData);     //发送字节							   
	Drv_IICSoft_WaitAck(&_tAT24CXX->tIIC);  		    	   
    Drv_IICSoft_Stop(&_tAT24CXX->tIIC);//产生一个停止条件 
	S_AT24CXX_Delay(10);	 
}

/**
 * @brief 	AT24CXX指定地址开始写入长度为Len的数据
 * @param 	*_tAT24CXX-AT24CXX结构体
 * @param 	_usAddr-地址
 * @param 	_ulAddr-数据的首地址
 * @param 	_ucLen-长度
 * @retval	Null
 */
void OCD_AT24CXX_WriteLenByte(tagAT24CXX_T *_tAT24CXX, uint16_t _usAddr, uint8_t *_ulpData, uint8_t _ucLen)
{  	
	uint8_t index;
	for(index = 0; index < _ucLen; index++)
	{
		S_AT24CXX_WriteOneByte(_tAT24CXX, _usAddr + index, _ulpData[index]);
	}												    
}

/**
 * @brief 	AT24CXX指定地址开始读出长度为Len的数据
 * @param 	*_tAT24CXX-AT24CXX结构体
 * @param 	_usAddr-地址
 * @param 	_ucLen-长度
 * @retval	Null
 */
void OCD_AT24CXX_ReadLenByte(tagAT24CXX_T *_tAT24CXX, uint16_t _usAddr, uint8_t *_ulpData, uint8_t _ucLen)
{  	
	uint8_t index;
	
	for(index = 0; index < _ucLen; index++)
	{
		_ulpData[index] = S_AT24CXX_ReadOneByte(_tAT24CXX, _usAddr + index); 	 				   
	}											    
}

/**
 * @brief 	AT24CXX检查
 * @param 	*_tAT24CXX-AT24CXX结构体
 * @retval	0-检测失败；1-检测成功
 */
uint8_t OCD_AT24CXX_Check(tagAT24CXX_T *_tAT24CXX)
{
	uint8_t ucTemp;
	
	ucTemp = S_AT24CXX_ReadOneByte(_tAT24CXX, 255);		/* 避免每次开机都写AT24CXX */		   
	if(ucTemp == 0X55)
		return 1;
	else	/* 排除第一次初始化的情况 */
	{
		S_AT24CXX_WriteOneByte(_tAT24CXX, 255,0X55);
	    ucTemp = S_AT24CXX_ReadOneByte(_tAT24CXX, 255);
		
		if(ucTemp == 0X55)
			return 1;
	}
	
	return 0;											  
}

/**
 * @brief 	AT24CXX指定地址开始读出指定个数的数据
 * @param 	*_tAT24CXX-AT24CXX结构体
 * @param	_usAddr-地址
 * @param	*_ucpBuffer-缓存地址
 * @param	_usNum-数量
 * @retval	Null
 */
void OCD_AT24CXX_Read(tagAT24CXX_T *_tAT24CXX, uint16_t _usAddr, uint8_t *_ucpBuffer, uint16_t _usNum)
{
	while(_usNum)
	{
		*_ucpBuffer ++= S_AT24CXX_ReadOneByte(_tAT24CXX, _usAddr++);	
		_usNum--;
	}
}  

/**
 * @brief 	AT24CXX指定地址开始写入指定个数的数据
 * @param 	*_tAT24CXX-AT24CXX结构体
 * @param	_usAddr-地址
 * @param	*_ucpBuffer-缓存地址
 * @param	_usNum-数量
 * @retval	Null
 */
void OCD_AT24CXX_Write(tagAT24CXX_T *_tAT24CXX, uint16_t _usAddr, uint8_t *_ucpBuffer, uint16_t _usNum)
{
	while(_usNum--)
	{
		S_AT24CXX_WriteOneByte(_tAT24CXX, _usAddr,*_ucpBuffer);
		_usAddr++;
		_ucpBuffer++;
	}
}

/**
 * @brief 	AT24CXX初始化
 * @param 	*_tAT24CXX-AT24CXX结构体
 * @retval	Null
 */
void OCD_AT24CXX_Init(tagAT24CXX_T *_tAT24CXX)
{
	Drv_IICSoft_Init(&_tAT24CXX->tIIC);
}
