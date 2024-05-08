/****************************************************************************

* Sigma�Ŷ�

* �ļ���: drv_hal_spi.c

* ���ݼ�����spi�����ļ�

* �ļ���ʷ��

* �汾��		����		����		˵��
*  3.2 	 	2024-04-18	 �����		Drv_SPI_Transmit�������ӷ���ֵ

*  3.0	 	2024-01-26	 �����		����STM32F4ϵ��

* 1.1.7 	2022-10-11   �����		�Ż�ִ��˳��

* 1.1.4 	2022-09-03   �����		����SPI������ӳ�����

* 1.0.0a 	2020-02-22	 ���		�������ļ�

****************************************************************************/
#include "drv_hal_conf.h"

#ifdef DRV_HAL_SPI_ENABLE

/**
 * @brief SPIʱ��ʹ�ܺ���
 * @param _tSPI-spi�ṹ��ָ��
 * @retval Null
*/
static void S_SPI_CLKEnable(tagSPI_T *_tSPI)
{
	if(_tSPI->tSPIHandle.Instance == SPI1)
	{
		__HAL_RCC_SPI1_CLK_ENABLE();
	}
	else if(_tSPI->tSPIHandle.Instance == SPI2)
	{
		__HAL_RCC_SPI2_CLK_ENABLE();
	}
	else if(_tSPI->tSPIHandle.Instance == SPI3)
	{
		__HAL_RCC_SPI3_CLK_ENABLE();
	}
}

/**
 * @brief SPI��GPIO���ú���
 * @param _tSPI-spi�ṹ��ָ��
 * @retval Null
*/
static void S_SPI_GPIOConfig(tagSPI_T *_tSPI)
{
#ifdef STM32F1_SGA_ENABLE
	/* ��������ģʽʱ�� */
	__HAL_RCC_AFIO_CLK_ENABLE();

	/* ���ݲ�ͬSPI��ucAFMode������Ӧ����ӳ�䣬��ӳ�����drv_hal_spi.h�� */
	if(_tSPI->tSPIHandle.Instance == SPI1)
	{
		if(_tSPI->tGPIO->ucAFMode == NO_REMAP)				__HAL_AFIO_REMAP_SPI1_DISABLE();
		else if(_tSPI->tGPIO->ucAFMode == FULL_REMAP)		__HAL_AFIO_REMAP_SPI1_ENABLE();		
	}
#endif

#ifdef STM32F4_SGA_ENABLE
	if(_tSPI->tSPIHandle.Instance == SPI1)
	{
		_tSPI->tGPIO[0].tGPIOInit.Alternate = GPIO_AF5_SPI1;
		_tSPI->tGPIO[1].tGPIOInit.Alternate = GPIO_AF5_SPI1;
		_tSPI->tGPIO[2].tGPIOInit.Alternate = GPIO_AF5_SPI1;
	}
	else if(_tSPI->tSPIHandle.Instance == SPI2)
	{
		_tSPI->tGPIO[0].tGPIOInit.Alternate = GPIO_AF5_SPI2;
		_tSPI->tGPIO[1].tGPIOInit.Alternate = GPIO_AF5_SPI2;
		_tSPI->tGPIO[2].tGPIOInit.Alternate = GPIO_AF5_SPI2;
	}
	else if(_tSPI->tSPIHandle.Instance == SPI3)
	{
		_tSPI->tGPIO[0].tGPIOInit.Alternate = GPIO_AF6_SPI3;
		_tSPI->tGPIO[1].tGPIOInit.Alternate = GPIO_AF6_SPI3;
		_tSPI->tGPIO[2].tGPIOInit.Alternate = GPIO_AF6_SPI3;
	}
#endif
	Drv_GPIO_Init(_tSPI->tGPIO, 4);	/* GPIO��ʼ�� */
}

/**
 * @brief spi�������ú���
 * @param _tSPI-spi�ṹ��ָ��
 * @retval Null
*/
static void S_SPI_ParamConfig(tagSPI_T *_tSPI)
{
	if (HAL_SPI_Init(&_tSPI->tSPIHandle) != HAL_OK)
	{
		Drv_HAL_Error(__FILE__, __LINE__);
	}
}

/**
 * @brief Ӳ��SPI�ٶ�����
 * @param _tSPI-spi�ṹ��ָ��
 * @param _ucSpeed- 1-���٣�0-����
 * @retval Null 
*/
void Drv_SPI_SpeedConfig(tagSPI_T *_tSPI, uint8_t _ucSpeed)
{
	switch(_ucSpeed)
	{
		case SPI_SPEED_HIGH:
			_tSPI->tSPIHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
		break;
			
		case SPI_SPEED_LOW:
			_tSPI->tSPIHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
		break;
			
		default:
			Drv_HAL_Error(__FILE__, __LINE__);
		break;
	}
}

/**
 * @brief Ӳ��SPI���ͺͽ���
 * @param _tSPI-spi�ṹ��ָ��
 * @param _ucTxData-��Ҫ���͵�һ���ֽ�����
 * @retval uint8_t-���յ���һ���ֽ�����
*/
uint8_t Drv_SPI_TransmitReceive(tagSPI_T *_tSPI, uint8_t _ucTxData)
{
	uint8_t ucRxData = 0;
	
	HAL_SPI_TransmitReceive(&_tSPI->tSPIHandle,&_ucTxData,&ucRxData,BYTE_SIZE,SPI_TIME_OUT);
	
	return ucRxData;
}

/**
 * @brief Ӳ��SPI����
 * @param _tSPI-spi�ṹ��ָ��
 * @param _ucTxData-��Ҫ���͵�һ���ֽ�����
* @retval uint8_t ����״̬
*/
uint8_t Drv_SPI_Transmit(tagSPI_T *_tSPI, uint8_t _ucTxData)
{
    uint8_t ucRet;
    
	ucRet = HAL_SPI_Transmit(&_tSPI->tSPIHandle,&_ucTxData,BYTE_SIZE,SPI_TIME_OUT);
    
    return ucRet;
}

/**
 * @brief Ӳ��SPI��ʼ��
 * @param _tSPI-spi�ṹ��ָ��
 * @retval null
*/
void Drv_SPI_Init(tagSPI_T *_tSPI)
{
	S_SPI_CLKEnable(_tSPI);
	S_SPI_GPIOConfig(_tSPI);
	S_SPI_ParamConfig(_tSPI);
}

#endif
