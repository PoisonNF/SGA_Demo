/****************************************************************************

* Sigma团队

* 文件名: drv_hal_spi_soft.c

* 内容简述：软件spi模拟驱动文件

* 文件历史：

* 版本号		日期		作者		说明
*  1.2		2023-2-22	  鲍程璐     按照句柄资源举例，统一初始化顺序

* 1.1.8 	2022-10-25     李豪		 创建该文件

****************************************************************************/
#include "drv_hal_conf.h"

#ifdef DRV_HAL_SPI_ENABLE

/**
 * @brief SPICLK 输出
 * @param _tSPISoft-软件spi结构体指针
 * @param _ucLevel-电平 0-低 1-高  
 * @retval NULL
*/
static void S_SPI_CLK(tagSPISoft_T *_tSPISoft, uint8_t _ucLevel)
{
	HAL_GPIO_WritePin(_tSPISoft->tSPISoft[0].tGPIOPort, _tSPISoft->tSPISoft[0].tGPIOInit.Pin, _ucLevel?GPIO_PIN_SET:GPIO_PIN_RESET);
}

/**
 * @brief SPIMISO 输出
 * @param _tSPISoft-软件spi结构体指针
 * @retval GPIO_PinState-读到的电平
*/
static GPIO_PinState S_SPI_MISO(tagSPISoft_T *_tSPISoft)
{
	return HAL_GPIO_ReadPin(_tSPISoft->tSPISoft[1].tGPIOPort, _tSPISoft->tSPISoft[1].tGPIOInit.Pin);
}

/**
 * @brief SPIMOSI 输出
 * @param _tSPISoft-软件spi结构体指针
 * @param _ucLevel-电平 0-低 1-高  
 * @retval NULL
*/
static void S_SPI_MOSI(tagSPISoft_T *_tSPISoft, uint8_t _ucLevel)
{
	HAL_GPIO_WritePin(_tSPISoft->tSPISoft[2].tGPIOPort, _tSPISoft->tSPISoft[2].tGPIOInit.Pin, _ucLevel?GPIO_PIN_SET:GPIO_PIN_RESET);
}

/**
 * @brief SPI片选
 * @param _tSPISoft-软件spi结构体指针
 * @param _ucLevel-点评 0-低 1-高  
 * @retval NULL
*/
void Drv_SPI_NSS(tagSPISoft_T *_tSPISoft,uint8_t _ucLevel)
{
	HAL_GPIO_WritePin(_tSPISoft->tSPISoft[3].tGPIOPort, _tSPISoft->tSPISoft[3].tGPIOInit.Pin, _ucLevel?GPIO_PIN_SET:GPIO_PIN_RESET);
}

/**
 * @brief 读写一个字节
 * @param _tSPISoft-软件spi结构体指针
 * @param _ucData-写入数据  
 * @retval 读取的数据
*/
uint8_t Drv_SPI_WriteByte_Soft(tagSPISoft_T *_tSPISoft,uint8_t _ucData)
{
	uint8_t i = 0;
	uint8_t temp = 0;
	uint8_t read_data = 0xFF;
	
	for(i=0;i<8;i++) 
	{
		temp = ((_ucData&0x80)==0x80)? 1:0;
		_ucData = _ucData<<1;
		read_data = read_data<<1;
		S_SPI_CLK(_tSPISoft,0);
		S_SPI_MOSI(_tSPISoft,temp);
		SPI_Delay
		S_SPI_CLK(_tSPISoft,1);
		SPI_Delay
		if(S_SPI_MISO(_tSPISoft)==1) {read_data = read_data + 1; } 
	}
	S_SPI_CLK(_tSPISoft,0);
	return read_data;
}

/**
 * @brief 模拟SPI初始化
 * @param _tSPISoft-SPI结构体指针
 * @retval Null 
*/
void Drv_SPISoft_Init(tagSPISoft_T *_tSPISoft)
{
	Drv_GPIO_Init(_tSPISoft->tSPISoft, 4);
}


#endif

