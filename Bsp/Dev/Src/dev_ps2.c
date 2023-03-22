/****************************************************************************

* Sigma团队

* 文件名: dev_ps2.c

* 内容简述：PS2手柄驱动模块文件

* 文件历史：

* 版本号		   日期		   作者		   说明
*  2.1    		2023-03-22	  鲍程璐	 变更文件位置

*  1.2  		2022-10-27	  鲍程璐	 创建该文件

****************************************************************************/
#include "dev_ps2.h"

uint16_t usHandkey;	            	/* 按键值读取，零时存储 */ 
uint8_t ucaCMD[2] = {0x01,0x42};	/* 开始命令。请求数据 */
uint8_t ucaPS2Data[9] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; /* 数据存储数组 */
                    /*ucaPS2Data[0]    空 
                      ucaPS2Data[1]    0x73 红灯模式 0x41 灭灯模式
                      ucaPS2Data[2]    0x5A 返回数据预告
                      ucaPS2Data[3]    低8位按键状态 0按下
                      ucaPS2Data[4]    高8位按键状态 0按下
                      ucaPS2Data[5]    PSS_RX
                      ucaPS2Data[6]    PSS_RY
                      ucaPS2Data[7]    PSS_LX
                      ucaPS2Data[8]    PSS_LY
                    */
uint16_t usaMASK[] =
{
    PSB_SELECT,
    PSB_L3,
    PSB_R3,
    PSB_START,
    PSB_PAD_UP,
    PSB_PAD_RIGHT,
    PSB_PAD_DOWN,
    PSB_PAD_LEFT,
    PSB_L2,
    PSB_R2,
    PSB_L1,
    PSB_R1 ,
    PSB_GREEN,
    PSB_RED,
    PSB_BLUE,
    PSB_PINK
};

/**
 * @brief 向手柄发送指令函数
 * @param _tPS2-PS2句柄指针
 * @param _ucCMD-发送的指令，十六进制
 * @retval Null
*/
static void S_PS2_Cmd(tagPS2_T *_tPS2,uint8_t _ucCMD)
{
	volatile uint16_t ref=0x01;
	ucaPS2Data[1] = 0;

	for(ref=0x01;ref<0x0100;ref<<=1)
	{
		if(ref&_ucCMD)
		{
			DO_H;                   /* 输出一位控制位 */
		}
		else DO_L;

		CLK_H;                      /* 时钟拉高 */
		DELAY_TIME;
		CLK_L;
		DELAY_TIME;
		CLK_H;
		if(DI)
			ucaPS2Data[1] = ref|ucaPS2Data[1];
	}
	Drv_Delay_Us(16);
}

/**
 * @brief 手柄配置初始化
 * @param _tPS2-PS2句柄指针
 * @retval Null
*/
static void S_PS2_ShortPoll(tagPS2_T *_tPS2)
{
	CS_L;
	Drv_Delay_Us(16);
	S_PS2_Cmd(_tPS2,ucaCMD[0]);  
	S_PS2_Cmd(_tPS2,ucaCMD[1]);  
	S_PS2_Cmd(_tPS2,0X00);
	S_PS2_Cmd(_tPS2,0x00);
	S_PS2_Cmd(_tPS2,0x00);
	CS_H;
	Drv_Delay_Us(16);	
}

/**
 * @brief 手柄进入配置
 * @param _tPS2-PS2句柄指针
 * @retval Null
*/
static void S_PS2_EnterConfing(tagPS2_T *_tPS2)
{
    CS_L;
	Drv_Delay_Us(16);
	S_PS2_Cmd(_tPS2,0x01);  
	S_PS2_Cmd(_tPS2,0x43);  
	S_PS2_Cmd(_tPS2,0X00);
	S_PS2_Cmd(_tPS2,0x01);
	S_PS2_Cmd(_tPS2,0x00);
	S_PS2_Cmd(_tPS2,0X00);
	S_PS2_Cmd(_tPS2,0X00);
	S_PS2_Cmd(_tPS2,0X00);
	S_PS2_Cmd(_tPS2,0X00);
	CS_H;
	Drv_Delay_Us(16);
}

/**
 * @brief 手柄发送模式设置
 * @param _tPS2-PS2句柄指针
 * @note 默认为模拟量发送模式，不可以通过按键更改。如需更改在函数体里面修改
 * @retval Null
*/
static void S_PS2_TurnOnAnalogMode(tagPS2_T *_tPS2)
{
	CS_L;
	S_PS2_Cmd(_tPS2,0x01);  
	S_PS2_Cmd(_tPS2,0x44);  
	S_PS2_Cmd(_tPS2,0X00);
	S_PS2_Cmd(_tPS2,0x01); /* analog=0x01;digital=0x00  软件设置发送模式 */
	S_PS2_Cmd(_tPS2,0x03); /* Ox03锁存设置，即不可通过按键“MODE”设置模式。*/
				           /* 0xEE不锁存软件设置，可通过按键“MODE”设置模式。*/
	S_PS2_Cmd(_tPS2,0X00);
	S_PS2_Cmd(_tPS2,0X00);
	S_PS2_Cmd(_tPS2,0X00);
	S_PS2_Cmd(_tPS2,0X00);
	CS_H;
	Drv_Delay_Us(16);
}

/**
 * @brief 手柄完成并保存配置
 * @param _tPS2-PS2句柄指针
 * @retval Null
*/
static void S_PS2_ExitConfing(tagPS2_T *_tPS2)
{
    CS_L;
	Drv_Delay_Us(16);
	S_PS2_Cmd(_tPS2,0x01);  
	S_PS2_Cmd(_tPS2,0x43);  
	S_PS2_Cmd(_tPS2,0X00);
	S_PS2_Cmd(_tPS2,0x00);
	S_PS2_Cmd(_tPS2,0x5A);
	S_PS2_Cmd(_tPS2,0x5A);
	S_PS2_Cmd(_tPS2,0x5A);
	S_PS2_Cmd(_tPS2,0x5A);
	S_PS2_Cmd(_tPS2,0x5A);
	CS_H;
	Drv_Delay_Us(16);
}

/**
 * @brief 从手柄处接收数据函数
 * @param _tPS2-PS2句柄指针
 * @note 单独使用时需提前调用Dev_PS2_ClearData
 * @retval Null
*/
void Dev_PS2_ReadData(tagPS2_T *_tPS2)
{
	volatile uint8_t byte=0;
	volatile uint16_t ref=0x01;

	CS_L;
	S_PS2_Cmd(_tPS2,ucaCMD[0]);  		/* 开始命令 */
	S_PS2_Cmd(_tPS2,ucaCMD[1]);  		/* 请求数据 */

	/* 开始接收数据 */
	for(byte=2;byte<9;byte++)    
	{
		for(ref=0x01;ref<0x100;ref<<=1)
		{
			CLK_H;
			DELAY_TIME;
			CLK_L;
			DELAY_TIME;
			CLK_H;
		    if(DI)
		    ucaPS2Data[byte] = ref|ucaPS2Data[byte];
		}
        Drv_Delay_Us(16);
	}
	CS_H;
}

/**
 * @brief 清除缓冲区数据
 * @param Null
 * @retval Null
*/
void Dev_PS2_ClearData(void)
{
	uint8_t a;

	for(a=0;a<9;a++)
		ucaPS2Data[a]=0x00;
}

/**
 * @brief 对读出来的PS2的数据进行处理,返回按键状态
 * @param _tPS2-PS2句柄指针
 * @retval uint8_t-按键的序号
*/
uint8_t Dev_PS2_DataKey(tagPS2_T *_tPS2)
{
	uint8_t index;

	Dev_PS2_ClearData();
	Dev_PS2_ReadData(_tPS2);

	usHandkey = (ucaPS2Data[4]<<8)|ucaPS2Data[3];     /* 这是16个按键  按下为0，未按下为1 */
	for(index = 0;index < 16;index++)
	{	    
		if((usHandkey&(1<<(usaMASK[index]-1)))==0)
		return index+1;		/* 按键的序号 */
	}
	return 0;           	/* 没有任何按键按下 */
}

/**
 * @brief 获取一个遥杆X轴或者Y轴的模拟量
 * @param _ucButton-摇杆存储位置
 * 			PSS_RX 5	右摇杆X轴数据
			PSS_RY 6	右摇杆Y轴数据
			PSS_LX 7	左摇杆X轴数据
			PSS_LY 8	左摇杆Y轴数据
 * @retval uint8_t-模拟量 范围0~256
*/
uint8_t Dev_PS2_AnologData(uint8_t _ucButton)
{
	return ucaPS2Data[_ucButton];
}

/**
 * @brief 判断是否为红灯（模拟量）模式，0x41=绿灯，0x73=红灯
 * @param _tPS2-PS2句柄指针
 * @retval uint8_t-0，红灯模式；1，绿灯模式
*/
uint8_t Dev_PS2_RedLight(tagPS2_T *_tPS2)
{
	CS_L;
	S_PS2_Cmd(_tPS2,ucaCMD[0]);  /* 开始命令 */
	S_PS2_Cmd(_tPS2,ucaCMD[1]);  /* 请求数据 */
	CS_H;
	if(ucaPS2Data[1] == 0X73)   return 0 ;
	else return 1;
}

/**
 * @brief 手柄配置震动函数
 * @param _tPS2-PS2句柄指针
 * @param _ucMotor1-右侧小震动电机 0x00关，其他开
 * @param _ucMotor2-左侧大震动电机 0x40~0xFF 电机开，值越大 震动越大
 * @retval Null
*/
void Dev_PS2_Vibration(tagPS2_T *_tPS2,uint8_t _ucMotor1, uint8_t _ucMotor2)
{
	CS_L;
	Drv_Delay_Us(16);
    S_PS2_Cmd(_tPS2,ucaCMD[0]);  /* 开始命令 */
	S_PS2_Cmd(_tPS2,ucaCMD[1]);  /* 请求数据 */
	S_PS2_Cmd(_tPS2,0X00);
	S_PS2_Cmd(_tPS2,_ucMotor1);
	S_PS2_Cmd(_tPS2,_ucMotor2);
	S_PS2_Cmd(_tPS2,0X00);
	S_PS2_Cmd(_tPS2,0X00);
	S_PS2_Cmd(_tPS2,0X00);
	S_PS2_Cmd(_tPS2,0X00);
	CS_H;
	Drv_Delay_Us(16);  
}

/**
 * @brief 手柄开启震动模式
 * @param _tPS2-PS2句柄指针
 * @retval Null
*/
void Dev_PS2_VibrationMode(tagPS2_T *_tPS2)
{
	CS_L;
	Drv_Delay_Us(16);
	S_PS2_Cmd(_tPS2,0x01);  
	S_PS2_Cmd(_tPS2,0x4D);  
	S_PS2_Cmd(_tPS2,0X00);
	S_PS2_Cmd(_tPS2,0x00);
	S_PS2_Cmd(_tPS2,0X01);
	CS_H;
	Drv_Delay_Us(16);	
}

/**
 * @brief 手柄初始化函数
 * @param _tPS2-PS2句柄指针
 * @retval Null
*/
void Dev_PS2_Init(tagPS2_T *_tPS2)
{
    Drv_GPIO_Init(_tPS2->tGPIO,4);	/* 初始化所用I/O口 */

	S_PS2_ShortPoll(_tPS2);
	S_PS2_ShortPoll(_tPS2);
	S_PS2_ShortPoll(_tPS2);
	S_PS2_EnterConfing(_tPS2);		/* 进入配置模式 */
	S_PS2_TurnOnAnalogMode(_tPS2);	/* “红绿灯”配置模式，并选择是否保存 */
	//PS2_VibrationMode();			/* 开启震动模式 */
	S_PS2_ExitConfing(_tPS2);		/* 完成并保存配置 */
}
