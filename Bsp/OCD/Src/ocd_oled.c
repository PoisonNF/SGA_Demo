/****************************************************************************

* Sigma团队

* 文件名: ocd_oled.c

* 内容简述：4线OLED驱动（SSD1306）
			从I/O中任选两个作为SCL和SDA，实现OLED的驱动。包含字库6*8以及8*16.
			6*8字符占一页，即y = 1
			8*16字符占两页，即y = 2 
* 文件历史：

* 版本号	日期		作者		说明

* 1.1.5  2022-09-08   鲍程璐	创建该文件

****************************************************************************/
#include "ocd_oled.h"
#include "ocd_oled_font.h"

/**
 * @brief 	往SSD1306中写入一个字节数据或者命令
 * @param   *_tOLED-OLED结构体
 * @param 	dat-数据
 * @param 	cmd-数据/命令   OLED_CMD(0)命令;OLED_DATA(1)数据;
 * @retval	NULL
 */
static void OLED_WR_Byte(tagOLED_T *_tOLED,uint8_t dat,uint8_t cmd)
{
    Drv_IICSoft_Start(&_tOLED->tIIC);
    Drv_IICSoft_SendByte(&_tOLED->tIIC,0x78);
    Drv_IICSoft_WaitAck(&_tOLED->tIIC);
    if(cmd)
    {
        /* 写入数据 */
        Drv_IICSoft_SendByte(&_tOLED->tIIC,0x40);
    }
    else
    {
        /* 写入命令 */
        Drv_IICSoft_SendByte(&_tOLED->tIIC,0x00);
    }
    Drv_IICSoft_WaitAck(&_tOLED->tIIC);
    Drv_IICSoft_SendByte(&_tOLED->tIIC,dat);
    Drv_IICSoft_WaitAck(&_tOLED->tIIC);
    Drv_IICSoft_Stop(&_tOLED->tIIC);
}


/******************************************* 以下是OLED操作相关函数 ************************************************/
/**
 * @brief 	OLED显示开启函数
 * @param   *_tOLED-OLED结构体
 * @param 	NULL
 * @retval	NULL
 */
void OCD_OLED_DisplayON(tagOLED_T *_tOLED)
{
    OLED_WR_Byte(_tOLED,0x8D,OLED_CMD);//SET DCDC命令
    OLED_WR_Byte(_tOLED,0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(_tOLED,0XAF,OLED_CMD);  //DISPLAY ON
}

/**
 * @brief 	OLED显示关闭函数
 * @param   *_tOLED-OLED结构体
 * @param 	NULL
 * @retval	NULL
 */
void OCD_OLED_DisplayOFF(tagOLED_T *_tOLED)
{
    OLED_WR_Byte(_tOLED,0x8D,OLED_CMD);//SET DCDC命令
    OLED_WR_Byte(_tOLED,0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(_tOLED,0XAE,OLED_CMD);  //DISPLAY OFF
}

/**
 * @brief 	OLED清屏函数
 * @param   *_tOLED-OLED结构体
 * @param 	NULL
 * @retval	NULL
 */
void OCD_OLED_Clear(tagOLED_T *_tOLED)
{
    uint8_t i,n;
    for(i=0;i<8;i++)
    {	
		OLED_WR_Byte(_tOLED,0xb0+i,OLED_CMD);
		OLED_WR_Byte(_tOLED,0x00,OLED_CMD);
		OLED_WR_Byte(_tOLED,0x10,OLED_CMD);
        for(n=0;n<128;n++)  OLED_WR_Byte(_tOLED,0,OLED_DATA);
    }
}

/**
 * @brief 	OLED设置光标位置
 * @param   *_tOLED-OLED结构体
 * @param 	x	横坐标	x(0-127)
 * @param 	y	纵坐标	y(0-127)
 * @retval	NULL
 */
void OLED_SetCursorAddrese(tagOLED_T *_tOLED,uint8_t x,uint8_t y)
{
		OLED_WR_Byte(_tOLED,0xB0+y,OLED_CMD); 					//设置页地址
		OLED_WR_Byte(_tOLED,(x&0xF0)>>4|0x10,OLED_CMD);//设置列高起始地址(半字节)
		OLED_WR_Byte(_tOLED,(x&0x0F)|0x00,OLED_CMD);   //设置列低起始地址(半字节)			
}

/**
 * @brief 	OLED按页清屏函数
 * @param   *_tOLED-OLED结构体
 * @param 	page	(0-7)
 * @retval	NULL
 */
void OCD_OLED_PageClear(tagOLED_T *_tOLED,uint8_t page)
{
		uint8_t j;
		OLED_WR_Byte(_tOLED,0xB0+page,OLED_CMD); //设置页地址
		OLED_WR_Byte(_tOLED,0x10,OLED_CMD);      //设置列高起始地址(半字节)
		OLED_WR_Byte(_tOLED,0x00,OLED_CMD);      //设置列低起始地址(半字节)
		for(j=0;j<128;j++)
		{
				OLED_WR_Byte(_tOLED,0,OLED_DATA); //写数据
		}
}

/**
 * @brief 	OLED写一个字符
 * @param   *_tOLED-OLED结构体
 * @param 	x	横坐标	x(0-127)
 * @param 	y	纵坐标	y(0-127)
 * @param 	str	所写字符
 * @param	Char_Size	显示大小 可选8或者16
 * @retval	NULL
 */
void OLED_ShowChar(tagOLED_T *_tOLED,uint8_t x,uint8_t y,uint8_t str,uint8_t Char_Size)
{      	
	uint8_t addr=0,i=0;
	uint8_t *strflag = &str;			
	if(*strflag >= ' '&& *strflag <= '~') //显示英文
	{
		addr= *strflag-' '; //取模从空格开始的，计算下标	
		if(Char_Size == 16)
		{
			//写8*16ASCII字符的上半部分
			OLED_SetCursorAddrese(_tOLED,x,y); //设置光标的位置
			for(i=0;i<Char_Size/2;i++)      //横向写width列
			{
				OLED_WR_Byte(_tOLED,ASCII_8_16[addr][i],OLED_DATA); 
			}
			//写8*16ASCII字符的下半部分
			OLED_SetCursorAddrese(_tOLED,x,y+1); //设置光标的位置
			for(i=0;i<Char_Size/2;i++)        //横向写width列
			{
				OLED_WR_Byte(_tOLED,ASCII_8_16[addr][i+Char_Size/2],OLED_DATA); 
			}
			strflag++;//继续显示下一个字符	
		}
		if((Char_Size == 8))
		{
			//写6*8ASCII字符
			OLED_SetCursorAddrese(_tOLED,x,y); //设置光标的位置
			for(i=0;i<6;i++)      //横向写width列
			{
				OLED_WR_Byte(_tOLED,ASCII_6_8[addr][i],OLED_DATA); 
			}
			strflag++;//继续显示下一个字符	
		}
      
	}
}

/**
 * @brief 	OLED写字符串
 * @param   *_tOLED-OLED结构体
 * @param 	x	横坐标	x(0-127)
 * @param 	y	纵坐标	y(0-127)
 * @param 	*str	字符串首地址，输入显示字符串即可
 * @param	Char_Size	显示大小 可选8或者16
 * @retval	NULL
 */
void OCD_OLED_ShowString(tagOLED_T *_tOLED,uint8_t x,uint8_t y,uint8_t *str,uint8_t Char_Size)
{
	uint8_t i=0;
	while(str[i]!='\0') //连续显示
	{
		OLED_ShowChar(_tOLED,x,y,str[i],Char_Size);
        if(Char_Size == 16)	//如果是8*16字体
		{
			x+= Char_Size/2;
			if((x+Char_Size/2) > Max_Column)  {x=0;y+=2;}	//溢出则换行
		}
		if(Char_Size == 8) //如果是6*8字体
		{
			x+= 6;
			if((x+6) > Max_Column)  {x=0;y+=1;}	//溢出则换行
		}
		i++;
	}
}

/**
 * @brief 	OLED指数函数
 * @param 	m	底数
 * @param 	n	指数
 * @retval	m^n的结果
 */
uint32_t OLED_Pow(uint8_t m,uint8_t n)
{
	uint32_t result = 1;
	while(n--)	result *= m;
	return result;
}

/**
 * @brief 	OLED显示数字
 * @param   *_tOLED-OLED结构体
 * @param 	x	横坐标	x(0-127)
 * @param 	y	纵坐标	y(0-127)
 * @param 	num	需显示数字
 * @param	len 需显示数字长度
 * @param	Char_Size	显示大小 可选8或者16
 * @retval	NULL
 */
void OCD_OLED_ShowNum(tagOLED_T *_tOLED,uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t Char_Size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/OLED_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(Char_Size == 16)	OLED_ShowChar(_tOLED,x+(Char_Size/2)*t,y,' ',Char_Size);
				if(Char_Size == 8)	OLED_ShowChar(_tOLED,x+Char_Size*t,y,' ',Char_Size);
				continue;
			}else enshow=1; 
		 	 
		}
	 	if(Char_Size == 16)	OLED_ShowChar(_tOLED,x+(Char_Size/2)*t,y,temp+'0',Char_Size);
		if(Char_Size == 8)	OLED_ShowChar(_tOLED,x+6*t,y,temp+'0',Char_Size);
	}
} 

/**
 * @brief 	OLED显示浮点数
 * @param   *_tOLED-OLED结构体
 * @param 	x	横坐标	x(0-127)
 * @param 	y	纵坐标	y(0-127)
 * @param 	Fnum	需显示浮点数
 * @param	Char_Size	显示大小 可选8或者16
 * @retval	NULL
 */
void OCD_OLED_ShowFNum(tagOLED_T *_tOLED,uint8_t x,uint8_t y,float Fnum,uint8_t Char_Size)
{
	char Data[] = "";
	sprintf(Data,"%.3f",Fnum);
	uint8_t i=0;
	while(Data[i]!='\0') //连续显示
	{
		OLED_ShowChar(_tOLED,x,y,Data[i],Char_Size);
        if(Char_Size == 16)	//如果是8*16字体
		{
			x+= Char_Size/2;
			if((x+Char_Size/2) > Max_Column)  {x=0;y+=2;}	//溢出则换行
		}
		if(Char_Size == 8) //如果是6*8字体
		{
			x+= 6;
			if((x+6) > Max_Column)  {x=0;y+=1;}	//溢出则换行
		}
		i++;
	}
}

/**
 * @brief 	OLED显示BMP位图
 * @param   *_tOLED-OLED结构体
 * @param 	x0	起始横坐标	横坐标	x(0-127)
 * @param 	y0	起始纵坐标	纵坐标	y(0-127)
 * @param	x1	终止横坐标	横坐标	x(0-127)
 * @param	y1	终止纵坐标	纵坐标	y(0-127)
 * @param 	BMP	bmp解析数组
 * @retval	NULL
 */
void OLED_DrawBMP(tagOLED_T *_tOLED,uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1,uint8_t BMP[])
{ 	
 unsigned int j=0;
 uint8_t x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetCursorAddrese(_tOLED,x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(_tOLED,BMP[j++],OLED_DATA);	    	
	    }
	}
} 

/**
 * @brief 	OLED初始化函数
 * @param   *_tOLED-OLED结构体
 * @param 	NULL
 * @retval	NULL
 */
void OCD_OLED_Init(tagOLED_T *_tOLED)
{
    Drv_IICSoft_Init(&_tOLED->tIIC);
    Drv_Delay_Ms(500);
    OLED_WR_Byte(_tOLED,0xAE,OLED_CMD); //关闭显示
	OLED_WR_Byte(_tOLED,0x00,OLED_CMD);//设置低列地址
	OLED_WR_Byte(_tOLED,0x10,OLED_CMD);//设置高列地址
	OLED_WR_Byte(_tOLED,0x40,OLED_CMD);//设置起始行地址,集映射RAM显示起始行(0x00~0x3F)
	OLED_WR_Byte(_tOLED,0x81,OLED_CMD);//设置对比度控制寄存器
	OLED_WR_Byte(_tOLED,0xCF,OLED_CMD);//设置SEG输出电流亮度
	OLED_WR_Byte(_tOLED,0xA1,OLED_CMD);//段重定义设置,bit0:0,0->0;1,0->127; 0xa0左右反置 0xa1正常
	OLED_WR_Byte(_tOLED,0xC8,OLED_CMD);//设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数   0xc0上下反置 0xc8正常
	OLED_WR_Byte(_tOLED,0xA6,OLED_CMD);//设置正常显示(设置显示方式;bit0:1,反相显示;0,正常显示	)
	OLED_WR_Byte(_tOLED,0xA8,OLED_CMD);//设置驱动路数 设置多路复用比(1比64)
	OLED_WR_Byte(_tOLED,0x3F,OLED_CMD);//1/64 duty(默认0X3F(1/64))
	OLED_WR_Byte(_tOLED,0xD3,OLED_CMD);//设置显示偏移位移映射RAM计数器(0x00~0x3F)
	OLED_WR_Byte(_tOLED,0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(_tOLED,0xD5,OLED_CMD);//设置显示时钟分频比/振荡器频率
	OLED_WR_Byte(_tOLED,0x80,OLED_CMD);//设置分频比，设置时钟为100帧/秒
	OLED_WR_Byte(_tOLED,0xD9,OLED_CMD);//设置预充电周期
	OLED_WR_Byte(_tOLED,0xF1,OLED_CMD);//设置预充15个时钟，放电1个时钟([3:0],PHASE 1;[7:4],PHASE 2;)
	OLED_WR_Byte(_tOLED,0xDA,OLED_CMD);//设置COM硬件引脚配置
	OLED_WR_Byte(_tOLED,0x12,OLED_CMD);//[5:4]配置
	OLED_WR_Byte(_tOLED,0xDB,OLED_CMD);//设置VCOMH 电压倍率
	OLED_WR_Byte(_tOLED,0x40,OLED_CMD);//Set VCOM 释放电压([6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;)
	OLED_WR_Byte(_tOLED,0x20,OLED_CMD);//设置页面寻址模式(0x00/0x01/0x02)
	OLED_WR_Byte(_tOLED,0x02,OLED_CMD);//[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
	OLED_WR_Byte(_tOLED,0x8D,OLED_CMD);//设置充电泵启用/禁用
	OLED_WR_Byte(_tOLED,0x14,OLED_CMD);//设置(0x10禁用,0x14启用)
	OLED_WR_Byte(_tOLED,0xA4,OLED_CMD);// 全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏) (0xa4/0xa5)
	OLED_WR_Byte(_tOLED,0xA6,OLED_CMD);// 设置显示方式;bit0:1,反相显示;0,正常显示 (0xa6/a7) 
	OLED_WR_Byte(_tOLED,0xAF,OLED_CMD);//开启显示	
	OCD_OLED_Clear(_tOLED);
	OLED_SetCursorAddrese(_tOLED,0,0);
	//printf("OLED Init OK !\n");

}
