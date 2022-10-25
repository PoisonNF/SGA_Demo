/****************************************************************************

* Sigma团队

* 文件名: drv_hal_iic_soft.c

* 内容简述：软件iic模拟驱动文件

* 文件历史：

* 版本号	日期		作者		说明

* 1.0.0a 	2020-02-22	李环宇		创建该文件
* 1.0.1a 	2020-02-22	李环宇		修复Drv_IICSoft_ReadByte函数中ACK和NACK错误的问题

****************************************************************************/
#include "drv_hal_conf.h"

#define TYPE_NUM		8	/* 字节位数 */
#define IIC_GPIO_NUM	2	/* IIC所需的GPIO数量 */

/**
 * @brief IIC的微秒延时函数
 * @param _ucUs 需要延时的数值
 * @retval Null
*/
static void S_IIC_DelayUs(uint8_t _ucUs)
{
	Drv_Delay_Us(_ucUs);
}

/**
 * @brief IIC的SCL线拉高
 * @param _tIIC-IIC结构体指针
 * @retval Null
*/
static void S_IICSoft_SCL_H(tagIICSoft_T *_tIIC)
{
	HAL_GPIO_WritePin(_tIIC->tIICSoft[0].tGPIOPort,_tIIC->tIICSoft[0].tGPIOInit.Pin,GPIO_PIN_SET);
}

/**
 * @brief IIC的SCL线拉低
 * @param _tIIC-IIC结构体指针
 * @retval Null
*/
static void S_IICSoft_SCL_L(tagIICSoft_T *_tIIC)
{
	HAL_GPIO_WritePin(_tIIC->tIICSoft[0].tGPIOPort,_tIIC->tIICSoft[0].tGPIOInit.Pin,GPIO_PIN_RESET);
}

/**
 * @brief IIC的SDA线拉高
 * @param _tIIC-IIC结构体指针
 * @retval Null
*/
static void S_IICSoft_SDA_H(tagIICSoft_T *_tIIC)
{
	HAL_GPIO_WritePin(_tIIC->tIICSoft[1].tGPIOPort,_tIIC->tIICSoft[1].tGPIOInit.Pin,GPIO_PIN_SET);
}

/**
 * @brief IIC的SDA线拉低
 * @param _tIIC-IIC结构体指针
 * @retval Null
*/
static void S_IICSoft_SDA_L(tagIICSoft_T *_tIIC)
{
	HAL_GPIO_WritePin(_tIIC->tIICSoft[1].tGPIOPort,_tIIC->tIICSoft[1].tGPIOInit.Pin,GPIO_PIN_RESET);
}

/**
 * @brief IIC的SDA线读取
 * @param _tIIC-IIC结构体指针
 * @retval Null
*/
static GPIO_PinState S_IICSoft_READ_SDA(tagIICSoft_T *_tIIC)
{
	return HAL_GPIO_ReadPin(_tIIC->tIICSoft[1].tGPIOPort,_tIIC->tIICSoft[1].tGPIOInit.Pin);
}

/**
 * @brief IIC的SDA线输出模式
 * @param _tIIC-IIC结构体指针
 * @retval Null
*/
static void IICSoft_SDA_OutputMode(tagIICSoft_T *_tIIC)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	GPIO_InitStruct.Pin = _tIIC->tIICSoft[1].tGPIOInit.Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(_tIIC->tIICSoft[1].tGPIOPort, &GPIO_InitStruct);
}

/**
 * @brief IIC的SDA线输入模式
 * @param _tIIC-IIC结构体指针
 * @retval Null
*/
static void IICSoft_SDA_InputMode(tagIICSoft_T *_tIIC)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	GPIO_InitStruct.Pin = _tIIC->tIICSoft[1].tGPIOInit.Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(_tIIC->tIICSoft[1].tGPIOPort,&GPIO_InitStruct);
}

/**
 * @brief 模拟IIC产生起始信号
 * @param _tIIC---IIC结构体指针
 * @retval Null 
*/
void Drv_IICSoft_Start(tagIICSoft_T *_tIIC)
{
	IICSoft_SDA_OutputMode(_tIIC);	/* sda线输出 */
	
	S_IICSoft_SDA_H(_tIIC);	  	  
	S_IICSoft_SCL_H(_tIIC);
	S_IIC_DelayUs(4);
	
 	S_IICSoft_SDA_L(_tIIC);
	S_IIC_DelayUs(4);
	S_IICSoft_SCL_L(_tIIC);
}	

/**
 * @brief 模拟IIC产生停止信号
 * @param _tIIC---IIC结构体指针
 * @retval Null 
*/
void Drv_IICSoft_Stop(tagIICSoft_T *_tIIC)
{
	IICSoft_SDA_OutputMode(_tIIC);
	
	S_IICSoft_SCL_L(_tIIC);
	S_IICSoft_SDA_L(_tIIC);
 	S_IIC_DelayUs(4);
	
	S_IICSoft_SCL_H(_tIIC); 
	S_IICSoft_SDA_H(_tIIC);
	S_IIC_DelayUs(4);							   	
}

/**
 * @brief 模拟IIC等待应答
 * @param _tIIC---IIC结构体指针
 * @retval Null 
*/
uint8_t Drv_IICSoft_WaitAck(tagIICSoft_T *_tIIC)
{
	uint8_t ucErrTime=0;

	IICSoft_SDA_InputMode(_tIIC);              //SDA设置为输入  
	
	S_IICSoft_SDA_H(_tIIC);
	S_IIC_DelayUs(1);	
	S_IICSoft_SCL_H(_tIIC);
	S_IIC_DelayUs(1);	 /*SCL为高 进行读取操作 */  

	while(S_IICSoft_READ_SDA(_tIIC))
	{
		ucErrTime++;
		if(ucErrTime > 250)
		{
			Drv_IICSoft_Stop(_tIIC);
			return 1;
		}
	}

	S_IICSoft_SCL_L(_tIIC);//时钟输出0 	  	
	return 0;  
} 


/**
 * @brief 模拟IIC产生应答
 * @param _tIIC---IIC结构体指针
 * @retval Null 
*/
static void IICSoft_SendAck(tagIICSoft_T *_tIIC)
{
	S_IICSoft_SCL_L(_tIIC);
	
	IICSoft_SDA_OutputMode(_tIIC);
	
	S_IICSoft_SDA_L(_tIIC);
	S_IIC_DelayUs(2);
	
	S_IICSoft_SCL_H(_tIIC);
	S_IIC_DelayUs(2);
	S_IICSoft_SCL_L(_tIIC);
}

/**
 * @brief 模拟IIC不产生应答
 * @param _tIIC---IIC结构体指针
 * @retval Null 
*/
static void IICSoft_SendNAck(tagIICSoft_T *_tIIC)
{
	S_IICSoft_SCL_L(_tIIC);
	
	IICSoft_SDA_OutputMode(_tIIC);
	S_IICSoft_SDA_H(_tIIC);
	S_IIC_DelayUs(2);
	S_IICSoft_SCL_H(_tIIC);
	S_IIC_DelayUs(2);
	S_IICSoft_SCL_L(_tIIC);
}					 				     

/**
 * @brief 模拟IIC发送一个字节
 * @param _tIIC---IIC结构体指针
 * @param txdata---发送得到字节数据
 * @retval Null 
*/
void Drv_IICSoft_SendByte(tagIICSoft_T *_tIIC, uint8_t _ucTxData)
{                        
    uint8_t index;   
	IICSoft_SDA_OutputMode(_tIIC); 	    
    S_IICSoft_SCL_L(_tIIC);//拉低时钟开始数据传输
	
    for(index = 0;index < TYPE_NUM;index++)
    {     
		if(_ucTxData & 0x80)
			S_IICSoft_SDA_H(_tIIC);
		else
			S_IICSoft_SDA_L(_tIIC);
			
		_ucTxData <<= 1; 	  
		S_IIC_DelayUs(2);   //对TEA5767这三个延时都是必须的
			
		S_IICSoft_SCL_H(_tIIC);
		S_IIC_DelayUs(2); 
		S_IICSoft_SCL_L(_tIIC);	
		S_IIC_DelayUs(2);
    }	 
} 	    

/**
 * @brief 模拟IIC读取一个字节
 * @param _tIIC---IIC结构体指针
 * @param ack---ack=1时，发送NACK，ack=0，发送ACK
 * @retval Null 
*/
uint8_t Drv_IICSoft_ReadByte(tagIICSoft_T *_tIIC, uint8_t _ucAck)
{
	uint8_t index,ucRxData=0;
	IICSoft_SDA_InputMode(_tIIC);//SDA设置为输入
   
	for(index = 0;index < TYPE_NUM;index++ )
	{
		S_IICSoft_SCL_L(_tIIC); 
		S_IIC_DelayUs(2);
		S_IICSoft_SCL_H(_tIIC);
		
		ucRxData <<= 1;
		
		if(S_IICSoft_READ_SDA(_tIIC))
		   ucRxData++;
//		else
//			receive=receive&0xFE;
		
		S_IIC_DelayUs(1); 
    }	

    if (_ucAck)
        IICSoft_SendAck(_tIIC); //发送ACK  
    else
		IICSoft_SendNAck(_tIIC);//发送nACK

    return ucRxData;
}

/**
 * @brief 模拟IIC初始化
 * @param _tIIC---IIC结构体指针
 * @retval Null 
*/
void Drv_IICSoft_Init(tagIICSoft_T *_tIIC)
{
	Drv_GPIO_Init(_tIIC->tIICSoft, IIC_GPIO_NUM);
}
