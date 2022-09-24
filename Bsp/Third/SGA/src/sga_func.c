/****************************************************************************

* Sigma团队

* 文件名: sga_func.c

* 内容简述：SGA额外常用函数文件

* 文件历史：

* 版本号		日期	  作者				说明
* 1.1.6 	2022-09-24	鲍程璐		增加Hex至Dec的转换函数
* 1.0.1a 	2020-03-15	李环宇		新增SGA_StringToHex函数
									修改SGA_Function_Query_Sub执行逻辑、参数和返回值
* 1.0.0a 	2020-02-22	李环宇		创建该文件


****************************************************************************/
#include "sga_func.h"

/**
 * @brief 查询一个字符串a中是否存在另一个字符串b
 * @param *_ucpStrA---字符串a地址指针
 * @param _iSrclenth---字符串a的长度
 * @param _ucpStrB---字符串b地址指针
 * @param _ucpStrBlenth---字符串b的长度
 * @retval -1---不存在字符串b；其它---字符串b在字符串a的起始位置
*/
tag_StringInfo SGA_Function_Query_Sub(uint8_t *_ucpStrA, uint8_t *_ucpStrB)
{
	uint8_t *ucpSrc; 
	uint8_t *ucpSub;
	uint16_t usNum = 1;
	tag_StringInfo pos = {0,0};
	
	if(!_ucpStrA||!_ucpStrB)
	{
	   return pos;
	}
	/* 遍历src字符串 */
	while(*_ucpStrA)
	{ 
		/* 用来遍历子串 */
		ucpSrc = _ucpStrA;
		ucpSub = _ucpStrB;
		pos.ulIndex++;
		while(*ucpSrc++ == *ucpSub++)
		{
			usNum += 1;
			if(*ucpSub == ':')/*到了sub的结束位置，返回src位置*/
			{
				pos.ucpAddr = ucpSrc + 2;
				pos.ulIndex += usNum;
				return pos;
			}
			
		}
		usNum = 1;
		_ucpStrA++	;
		
	}
	
	return pos; 
}

/**
 * @brief 向字符串1中的指定位置插入字符串2(注意长度)
 * @param *_ucpStrA---字符串1地址指针
 * @param *_ucpStrB---字符串2地址指针
 * @param usOffset---偏移量
 * @retval char*---字符串1的地址
*/
#define SGA_INSERT_LENGTH	30
static char ucpInsertBuffer[SGA_INSERT_LENGTH] = {0};
void SGA_Function_Insert_Sub(uint8_t *_ucpStrA, uint8_t *_ucpStrB, uint16_t usOffset)
{
	uint8_t ucLen = 0;
	
	ucLen = strlen((char*)_ucpStrB);
	
	if(usOffset)
	{
		memcpy(ucpInsertBuffer, _ucpStrA, usOffset);
		memcpy(ucpInsertBuffer + usOffset, _ucpStrB, ucLen);
		memcpy(ucpInsertBuffer + usOffset + ucLen, _ucpStrA + usOffset, strlen((char*)_ucpStrA) - usOffset);
	}
	else
	{
		memcpy(ucpInsertBuffer, _ucpStrB, ucLen);
		memcpy(ucpInsertBuffer + ucLen, _ucpStrA, strlen((char*)_ucpStrA));
	}
	
	memcpy(_ucpStrA, ucpInsertBuffer, strlen((char*)_ucpStrA) + ucLen);
	memset(ucpInsertBuffer, 0, SGA_INSERT_LENGTH);
}

/* CRC16校验 start*/
static void S_SGA_Function_InvertUint8(uint8_t *_ucpBuffer, uint8_t *_ucpSrcBuffer)  
{  
    int i;  
    uint8_t ucpTmp[4];  
    ucpTmp[0] = 0;  
    for(i=0;i< 8;i++)  
    {  
      if(_ucpSrcBuffer[0]& (1 << i))  
        ucpTmp[0] |= 1<<(7-i);  
    }  
    _ucpBuffer[0] = ucpTmp[0];       
}  

static void S_SGA_Function_InvertUint16(uint16_t *_ucpBuffer, uint16_t *_ucpSrcBuffer)  
{  
    int i;  
    uint16_t ucpTmp[4];  
    ucpTmp[0] = 0;  
    for(i=0;i< 16;i++)  
    {  
      if(_ucpSrcBuffer[0] & (1 << i))  
        ucpTmp[0] |= 1<<(15 - i);  
    }  
    _ucpBuffer[0] = ucpTmp[0];  
} 

/**
 * @brief Crc16位校验（modbus）
 * @param _ucpMsg---需要校验的数据数组地址指针
 * @param _usDataLen---需要校验的数据数组长度
 * @retval 2字节CRC校验结果
*/
uint16_t SGA_Function_Crc16_Modbus(uint8_t *_ucpMsg, uint16_t _usDataLen)
{
	uint16_t usCRCin = 0xFFFF;  
	uint16_t usCPoly = 0x8005;  
	uint8_t ucChar = 0;  
		
	while (_usDataLen--)     
	{  
		ucChar = *(_ucpMsg++);  
		S_SGA_Function_InvertUint8(&ucChar,&ucChar);  
		usCRCin ^= (ucChar << 8); 
		
		for(int i = 0;i < 8;i++)  
		{  
			if(usCRCin & 0x8000)  
				usCRCin = (usCRCin << 1) ^ usCPoly;  
			else  
				usCRCin = usCRCin << 1;  
		}  
	}  
	
	S_SGA_Function_InvertUint16(&usCRCin,&usCRCin); 
	
	return (usCRCin);
}
/* CRC16校验 end*/

/**
 * @brief 字符串转Hex
 * @param *_ucpStrA---需要转换的字符串
 * @param _ucLen---转换长度
 * @retval uint32_t---转换后的结果
*/
uint32_t SGA_StringToHex(uint8_t *_ucpStrA, uint8_t _ucLen )
{
	uint8_t index;
	uint32_t ulRes = 0;

	for(index = 0; index < _ucLen; index++)
		ulRes += (_ucpStrA[index] - '0') << ((_ucLen - index - 1) * 4);

	return ulRes;
}

/**
 * @brief 十进制转Hex显示（如23转换为0x23）
 * @param _ucDec---需要转换的十进制数
 * @retval uint8_t---转换后的十六进制数
*/
uint8_t SGA_DecToHex(uint8_t _ucDec)
{
	uint8_t res;
	res = _ucDec + (_ucDec / 10) * 6;	

	return res;
}

/**
 * @brief 十六进制转Dec显示（如0x37转换为55）
 * @param hex---需要转换的十六进制数
 * @retval uint8_t---转换后的十进制数
*/
uint8_t SGA_HexToDec(uint8_t hex)
{
	uint8_t dec;
	uint8_t num1 = hex >> 4;
	uint8_t num2 = hex & 0x0F;
	dec = num1*16 + num2;
	return dec;
}
