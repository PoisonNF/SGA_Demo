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
 * @brief 模拟SPI初始化
 * @param _tSPI---SPI结构体指针
 * @retval Null 
*/
void Drv_SPISoft_Init(tagSPISoft_T *_tSPI)
{
	Drv_GPIO_Init(_tSPI->tSPISoft, 4);
}

/**
 * @brief SPICLK 输出
 * @param id-第几个软件spi 对应于定义时的顺序
 * @param level-电平 0-低 1-高  
 * @retval NULL
*/
void Drv_SPI_CLK(tagSPISoft_T *_tSPI, uint8_t level)
{
	HAL_GPIO_WritePin(_tSPI->tSPISoft[0].tGPIOPort, _tSPI->tSPISoft[0].tGPIOInit.Pin, level?GPIO_PIN_SET:GPIO_PIN_RESET);
}

/**
 * @brief SPIMISO 输出
 * @param id-第几个软件spi 对应于定义时的顺序 
 * @retval 读到的电平
*/

GPIO_PinState Drv_SPI_MISO(tagSPISoft_T *_tSPI)
{
	return HAL_GPIO_ReadPin(_tSPI->tSPISoft[1].tGPIOPort, _tSPI->tSPISoft[1].tGPIOInit.Pin);
}

/**
 * @brief SPIMOSI 输出
 * @param id-第几个软件spi 对应于定义时的顺序
 * @param level-电平 0-低 1-高  
 * @retval NULL
*/
void Drv_SPI_MOSI(tagSPISoft_T *_tSPI, uint8_t level)
{
	HAL_GPIO_WritePin(_tSPI->tSPISoft[2].tGPIOPort, _tSPI->tSPISoft[2].tGPIOInit.Pin, level?GPIO_PIN_SET:GPIO_PIN_RESET);
}

/**
 * @brief SPI片选
 * @param level-点评 0-低 1-高  
 * @param id-第几个软件spi 对应于定义时的顺序
 * @retval NULL
*/
void Drv_SPI_NSS(tagSPISoft_T *_tSPI,uint8_t level)
{
	HAL_GPIO_WritePin(_tSPI->tSPISoft[3].tGPIOPort, _tSPI->tSPISoft[3].tGPIOInit.Pin, level?GPIO_PIN_SET:GPIO_PIN_RESET);
}

/**
 * @brief 读写一个字节
 * @param data-写入数据  
 * @param id-第几个软件spi 对应于定义时的顺序
 * @retval 读取的数据
*/
uint8_t Drv_SPI_WriteByte_soft(tagSPISoft_T *_tSPI,uint8_t data)
{
	uint8_t i = 0;
	uint8_t temp = 0;
	uint8_t read_data = 0xFF;
	for(i=0;i<8;i++) 
	{
		temp = ((data&0x80)==0x80)? 1:0;
		data = data<<1;
		read_data = read_data<<1;
		Drv_SPI_CLK(_tSPI,0);
		Drv_SPI_MOSI(_tSPI,temp);
		SPI_Delay
		Drv_SPI_CLK(_tSPI,1);
		SPI_Delay
		if(Drv_SPI_MISO(_tSPI)==1) {read_data = read_data + 1; } 
	}
	Drv_SPI_CLK(_tSPI,0);
	return read_data;
}

#endif

