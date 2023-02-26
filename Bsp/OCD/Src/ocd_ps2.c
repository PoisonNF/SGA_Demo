/****************************************************************************

* Sigma�Ŷ�

* �ļ���: ocd_ps2.c

* ���ݼ�����PS2�ֱ�����ģ���ļ�

* �ļ���ʷ��

* �汾��		   ����		   ����		   ˵��
*  1.2  		2022-10-27	  �����	 �������ļ�

****************************************************************************/
#include "ocd_ps2.h"

/**
 * @brief ���ֱ�����ָ���
 * @param *_tPS2-PS2���ָ��
 * @param CMD ���͵�ָ�ʮ������
 * @retval Null
*/
static void S_PS2_Cmd(tagPS2_T *_tPS2,uint8_t CMD)
{
	volatile uint16_t ref=0x01;
	PS2Data[1] = 0;
	for(ref=0x01;ref<0x0100;ref<<=1)
	{
		if(ref&CMD)
		{
			DO_H;                   //���һλ����λ
		}
		else DO_L;

		CLK_H;                      //ʱ������
		DELAY_TIME;
		CLK_L;
		DELAY_TIME;
		CLK_H;
		if(DI)
			PS2Data[1] = ref|PS2Data[1];
	}
	Drv_Delay_Us(16);
}

/**
 * @brief ���ֱ����������ݺ���
 * @param *_tPS2-PS2���ָ��
 * @note ����ʹ��ʱ����ǰ����PS2_ClearData
 * @retval Null
*/
void OCD_PS2_ReadData(tagPS2_T *_tPS2)
{
	volatile uint8_t byte=0;
	volatile uint16_t ref=0x01;
	CS_L;
	S_PS2_Cmd(_tPS2,Comd[0]);  //��ʼ����
	S_PS2_Cmd(_tPS2,Comd[1]);  //��������
	for(byte=2;byte<9;byte++)         //��ʼ��������
	{
		for(ref=0x01;ref<0x100;ref<<=1)
		{
			CLK_H;
			DELAY_TIME;
			CLK_L;
			DELAY_TIME;
			CLK_H;
		    if(DI)
		    PS2Data[byte] = ref|PS2Data[byte];
		}
        Drv_Delay_Us(16);
	}
	CS_H;
}

/**
 * @brief �������������
 * @param Null
 * @retval Null
*/
static void S_PS2_ClearData(void)
{
	uint8_t a;
	for(a=0;a<9;a++)
		PS2Data[a]=0x00;
}

/**
 * @brief �Զ�������PS2�����ݽ��д���,���ذ���״̬
 * @param *_tPS2-PS2���ָ��
 * @retval ���������
*/
uint8_t OCD_PS2_DataKey(tagPS2_T *_tPS2)
{
	uint8_t index;

	S_PS2_ClearData();
	OCD_PS2_ReadData(_tPS2);

	Handkey=(PS2Data[4]<<8)|PS2Data[3];     //����16������  ����Ϊ0�� δ����Ϊ1
	for(index=0;index<16;index++)
	{	    
		if((Handkey&(1<<(MASK[index]-1)))==0)
		return index+1;	//���������
	}
	return 0;           //û���κΰ�������
}

/**
 * @brief ��ȡһ��ң��X�����Y���ģ����
 * @param button ҡ�˴洢λ��
 * 			PSS_RX 5	��ҡ��X������
			PSS_RY 6	��ҡ��Y������
			PSS_LX 7	��ҡ��X������
			PSS_LY 8	��ҡ��Y������
 * @retval ģ���� ��Χ0~256
*/
uint8_t OCD_PS2_AnologData(uint8_t button)
{
	return PS2Data[button];
}

/**
 * @brief �ж��Ƿ�Ϊ��ƣ�ģ������ģʽ��0x41=�̵ƣ�0x73=���
 * @param *_tPS2-PS2���ָ��
 * @retval 0�����ģʽ��1���̵�ģʽ
*/
uint8_t OCD_PS2_RedLight(tagPS2_T *_tPS2)
{
	CS_L;
	S_PS2_Cmd(_tPS2,Comd[0]);  //��ʼ����
	S_PS2_Cmd(_tPS2,Comd[1]);  //��������
	CS_H;
	if( PS2Data[1] == 0X73)   return 0 ;
	else return 1;
}

/**
 * @brief �ֱ������𶯺���
 * @param *_tPS2-PS2���ָ��
 * @param motor1 �Ҳ�С�𶯵�� 0x00�أ�������
 * @param motor2 �����𶯵�� 0x40~0xFF �������ֵԽ�� ��Խ��
 * @retval Null
*/
void OCD_PS2_Vibration(tagPS2_T *_tPS2,uint8_t motor1, uint8_t motor2)
{
	CS_L;
	Drv_Delay_Us(16);
    S_PS2_Cmd(_tPS2,Comd[0]);  //��ʼ����
	S_PS2_Cmd(_tPS2,Comd[1]);  //��������
	S_PS2_Cmd(_tPS2,0X00);
	S_PS2_Cmd(_tPS2,motor1);
	S_PS2_Cmd(_tPS2,motor2);
	S_PS2_Cmd(_tPS2,0X00);
	S_PS2_Cmd(_tPS2,0X00);
	S_PS2_Cmd(_tPS2,0X00);
	S_PS2_Cmd(_tPS2,0X00);
	CS_H;
	Drv_Delay_Us(16);  
}

/**
 * @brief �ֱ����ó�ʼ��
 * @param *_tPS2-PS2���ָ��
 * @retval Null
*/
static void S_PS2_ShortPoll(tagPS2_T *_tPS2)
{
	CS_L;
	Drv_Delay_Us(16);
	S_PS2_Cmd(_tPS2,Comd[0]);  
	S_PS2_Cmd(_tPS2,Comd[1]);  
	S_PS2_Cmd(_tPS2,0X00);
	S_PS2_Cmd(_tPS2,0x00);
	S_PS2_Cmd(_tPS2,0x00);
	CS_H;
	Drv_Delay_Us(16);	
}

/**
 * @brief �ֱ���������
 * @param *_tPS2-PS2���ָ��
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
 * @brief �ֱ�����ģʽ����
 * @param *_tPS2-PS2���ָ��
 * @note Ĭ��Ϊģ��������ģʽ��������ͨ���������ġ���������ں����������޸�
 * @retval Null
*/
static void S_PS2_TurnOnAnalogMode(tagPS2_T *_tPS2)
{
	CS_L;
	S_PS2_Cmd(_tPS2,0x01);  
	S_PS2_Cmd(_tPS2,0x44);  
	S_PS2_Cmd(_tPS2,0X00);
	S_PS2_Cmd(_tPS2,0x01); //analog=0x01;digital=0x00  �������÷���ģʽ
	S_PS2_Cmd(_tPS2,0x03); //Ox03�������ã�������ͨ��������MODE������ģʽ��
				           //0xEE�������������ã���ͨ��������MODE������ģʽ��
	S_PS2_Cmd(_tPS2,0X00);
	S_PS2_Cmd(_tPS2,0X00);
	S_PS2_Cmd(_tPS2,0X00);
	S_PS2_Cmd(_tPS2,0X00);
	CS_H;
	Drv_Delay_Us(16);
}

/**
 * @brief �ֱ�������ģʽ
 * @param *_tPS2-PS2���ָ��
 * @retval Null
*/
void OCD_PS2_VibrationMode(tagPS2_T *_tPS2)
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
 * @brief �ֱ���ɲ���������
 * @param *_tPS2-PS2���ָ��
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
 * @brief �ֱ���ʼ������
 * @param *_tPS2-PS2���ָ��
 * @retval Null
*/
void OCD_PS2_Init(tagPS2_T *_tPS2)
{
    Drv_GPIO_Init(_tPS2->tGPIO,4);	//��ʼ������I/O��

	S_PS2_ShortPoll(_tPS2);
	S_PS2_ShortPoll(_tPS2);
	S_PS2_ShortPoll(_tPS2);
	S_PS2_EnterConfing(_tPS2);		//��������ģʽ
	S_PS2_TurnOnAnalogMode(_tPS2);	//�����̵ơ�����ģʽ����ѡ���Ƿ񱣴�
	//PS2_VibrationMode();			//������ģʽ
	S_PS2_ExitConfing(_tPS2);			//��ɲ���������

}