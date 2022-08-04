/****************************************************************************

* Sigma团队

* 文件名: ocd_jy901.c

* 内容简述：jy901模块文件

* 文件历史：

* 版本号	日期		作者		说明

* 1.0.0a 	2020-03-14	李环宇		创建该文件
* 1.1.0a 	2020-03-29	李环宇		修改部分文本

****************************************************************************/
#include "ocd_jy901.h"

#define SAVE_NOW		0x00
#define SAVE_DEFAULT	0x01

/**
 * @brief JY901发送解锁指令函数
 * @param *_tJY901b-JY901句柄指针
 * @retval Null
*/
const uint8_t ucpUnlockCmd[] = {0xff, 0xaa, 0x69, 0x88, 0xb5}; 
static void S_JY901B_UnLock(tagJY901_T *_tJY901b)
{
	Drv_Uart_Transmit(&_tJY901b->tUART, (uint8_t*)ucpUnlockCmd, sizeof(ucpUnlockCmd));
}

/**
 * @brief JY901延时函数
 * @param Null
 * @retval Null
*/
static void S_JY901B_Delay(void)
{
	Drv_Delay_Ms(20);
}

/**
 * @brief JY901保存配置函数
 * @param *_tJY901b-JY901句柄指针
 * @param _ucSet-1:恢复默认；0-保存当前
 * @retval Null
*/
static void S_JY901B_SaveConfig(tagJY901_T *_tJY901b, uint8_t _ucSet)
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
		
		default:
			
		break;
	}
	Drv_Uart_Transmit(&_tJY901b->tUART, ucpWrite, sizeof(ucpWrite));
}

/**
 * @brief JY901回传内容配置
 * @param *_tJY901b-JY901句柄指针
 * @param _ucType-回传内容地址
 * @retval Null
*/
void OCD_JY901B_RxTypeConfig(tagJY901_T *_tJY901b, uint16_t _ucType)
{
	uint8_t ucpWrite[] = {0xff, 0xaa, 0x02, 0x00, 0x00};
	
	ucpWrite[3] = _ucType >> 8;
	ucpWrite[4] = _ucType;
	
	S_JY901B_UnLock(_tJY901b);
	S_JY901B_Delay();
	
	Drv_Uart_Transmit(&_tJY901b->tUART, ucpWrite, sizeof(ucpWrite));
	S_JY901B_Delay();
	
	S_JY901B_SaveConfig(_tJY901b, SAVE_NOW);		
}

/**
 * @brief JY901校准函数
 * @param *_tJY901b-JY901句柄指针
 * @param _ucMode-校准模式		0-退出校准模式；1-进入加速度计校准模式；2-进入磁场校准模式；3-高度置0		
 * @retval Null
*/
void OCD_JY901B_Correct(tagJY901_T *_tJY901b, uint8_t _ucMode)
{
	uint8_t ucpWrite[] = {0xff, 0xaa, 0x01, 0x00, 0x00};
	
	ucpWrite[3] = _ucMode;
	
	S_JY901B_UnLock(_tJY901b);
	S_JY901B_Delay();
	
	Drv_Uart_Transmit(&_tJY901b->tUART, ucpWrite, sizeof(ucpWrite));
	S_JY901B_Delay();
	
	S_JY901B_SaveConfig(_tJY901b, SAVE_NOW);
}

/**
 * @brief JY901休眠函数（休眠后再次调用为解除休眠）
 * @param *_tJY901b-JY901句柄指针
 * @retval Null
*/
const uint8_t ucpSleepCmd[] = {0xff, 0xaa, 0x22, 0x01, 0x00}; 
void OCD_JY901B_Sleep(tagJY901_T *_tJY901b)
{
	S_JY901B_UnLock(_tJY901b);
	S_JY901B_Delay();
	
	Drv_Uart_Transmit(&_tJY901b->tUART, (uint8_t*)ucpSleepCmd, sizeof(ucpSleepCmd));
	S_JY901B_Delay();
	
	S_JY901B_SaveConfig(_tJY901b, SAVE_NOW);
}

/**
 * @brief JY901回传速率配置
 * @param *_tJY901b-JY901句柄指针
 * @param _ucFreq-回传频率
 * @retval Null
*/
void OCD_JY901B_RxSpeedConfig(tagJY901_T *_tJY901b, uint8_t _ucFreq)
{
	uint8_t ucpWrite[] = {0xff, 0xaa, 0x03, 0x00, 0x00};
	
	ucpWrite[3] = _ucFreq;
	
	S_JY901B_UnLock(_tJY901b);
	S_JY901B_Delay();
	
	Drv_Uart_Transmit(&_tJY901b->tUART, ucpWrite, sizeof(ucpWrite));
	S_JY901B_Delay();
	
	S_JY901B_SaveConfig(_tJY901b, SAVE_NOW);		
}

/**
 * @brief JY901回传波特率配置
 * @param *_tJY901b-JY901句柄指针
 * @param _ucBaud-回传波特率地址
 * @retval Null
*/
void OCD_JY901B_RxBaudConfig(tagJY901_T *_tJY901b, uint8_t _ucBaud)
{
	uint8_t ucpWrite[] = {0xff, 0xaa, 0x04, 0x00, 0x00};
	
	ucpWrite[3] = _ucBaud;
	
	S_JY901B_UnLock(_tJY901b);
	S_JY901B_Delay();
	
	Drv_Uart_Transmit(&_tJY901b->tUART, ucpWrite, sizeof(ucpWrite));
	S_JY901B_Delay();
	
	S_JY901B_SaveConfig(_tJY901b, SAVE_NOW);		
}

/**
 * @brief JY901陀螺仪自动校准函数
 * @param *_tJY901b-JY901句柄指针
 * @param _ucMode-1-禁用自动校准;0-启用自动校准
 * @retval Null
*/
void OCD_JY901B_GyroAutoCorrect(tagJY901_T *_tJY901b, uint8_t _ucMode)
{
	uint8_t ucpWrite[] = {0xff, 0xaa, 0x63, 0x00, 0x00};
	
	ucpWrite[3] = _ucMode;
	
	S_JY901B_UnLock(_tJY901b);
	S_JY901B_Delay();
	
	Drv_Uart_Transmit(&_tJY901b->tUART, ucpWrite, sizeof(ucpWrite));
	S_JY901B_Delay();
	
	S_JY901B_SaveConfig(_tJY901b, SAVE_NOW);
}

/**
 * @brief JY901单次输出（需设置回传速率为单次输出）
 * @param *_tJY901b-JY901句柄指针
 * @retval Null
*/
const uint8_t ucpOutputOnceCmd[] = {0xff, 0xaa, 0x03, 0x0c, 0x00}; 
void OCD_JY901B_OutputOnce(tagJY901_T *_tJY901b)
{
	S_JY901B_UnLock(_tJY901b);
	S_JY901B_Delay();
	
	Drv_Uart_Transmit(&_tJY901b->tUART, (uint8_t*)ucpOutputOnceCmd, sizeof(ucpOutputOnceCmd));
	S_JY901B_Delay();
	
	S_JY901B_SaveConfig(_tJY901b, SAVE_NOW);
}

/**
 * @brief JY901初始化函数
 * @param *_tJY901b-JY901句柄指针
 * @retval Null
*/
void OCD_JY901B_Init(tagJY901_T *_tJY901b)
{
	Drv_Uart_ITInit(&_tJY901b->tUART);
}
