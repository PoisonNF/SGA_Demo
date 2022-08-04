#ifndef __OCD_JY901B_H_
#define __OCD_JY901B_H_

#include "drv_hal_conf.h"

typedef struct
{
	tagUART_T 	tUART;
}tagJY901_T;

#define JY901_OUTPUT_TIME			0x0100	/* 时间 */
#define JY901_OUTPUT_ACCE			0x0200	/* 加速度 */
#define JY901_OUTPUT_ANGULAR		0x0400	/* 角速度 */
#define JY901_OUTPUT_ANGLE			0x0800	/* 角度 */
#define	JY901_OUTPUT_MAG			0x1000	/* 磁场 */	
#define	JY901_OUTPUT_DIO			0x2000	/* 端口状态 */
#define	JY901_OUTPUT_PREANDHEIGHT	0x4000	/* 气压和高度 */
#define	JY901_OUTPUT_LONGANDLAT		0x8000	/* 经纬度 */
#define	JY901_OUTPUT_GROUND			0x0001	/* 地速 */
#define	JY901_OUTPUT_FOURELE		0x0002	/* 四元素 */
#define	JY901_OUTPUT_GPS			0x0004	/* 卫星定位精度 */

#define JY901_RX_0_1HZ		0x01
#define JY901_RX_0_5HZ		0x02
#define JY901_RX_1HZ		0x03
#define JY901_RX_2HZ		0x04
#define JY901_RX_5HZ		0x05
#define JY901_RX_10HZ		0x06
#define JY901_RX_20HZ		0x07
#define JY901_RX_50HZ		0x08
#define JY901_RX_100HZ		0x09
#define JY901_RX_NULL		0x0a
#define JY901_RX_200HZ		0x0b
#define JY901_RX_ONCE		0x0c

#define JY901_RXBAUD_2400		0x00
#define JY901_RXBAUD_4800		0x01
#define JY901_RXBAUD_9600		0x02
#define JY901_RXBAUD_19200		0x03
#define JY901_RXBAUD_38400		0x04
#define JY901_RXBAUD_57600		0x05
#define JY901_RXBAUD_115200		0x06
#define JY901_RXBAUD_230400		0x07
#define JY901_RXBAUD_460800		0x08
#define JY901_RXBAUD_921600		0x09

void OCD_JY901B_Init(tagJY901_T *_tJY901b);
void OCD_JY901B_RxTypeConfig(tagJY901_T *_tJY901b, uint16_t _ucType);
void OCD_JY901B_RxSpeedConfig(tagJY901_T *_tJY901b, uint8_t _ucFreq);
void OCD_JY901B_RxBaudConfig(tagJY901_T *_tJY901b, uint8_t _ucBaud);
void OCD_JY901B_GyroAutoCorrect(tagJY901_T *_tJY901b, uint8_t _ucMode);
void OCD_JY901B_Sleep(tagJY901_T *_tJY901b);
void OCD_JY901B_Correct(tagJY901_T *_tJY901b, uint8_t _ucMode);
void OCD_JY901B_OutputOnce(tagJY901_T *_tJY901b);

#endif
