/****************************************************************************

* Sigma团队

* 文件名: drv_hal_dac.c

* 内容简述：DAC驱动文件

* 文件历史：

* 版本号	日期		作者		说明

*  2.5 	 2023-06-01	  鲍程璐	创建该文件

****************************************************************************/
#include "drv_hal_conf.h"

#ifdef DRV_HAL_DAC_ENABLE

/**
 * @brief DAC时钟使能函数
 * @param _tDAC-DAC结构体指针
 * @retval Null
*/
static void S_DAC_CLKEnable(tagDAC_T *_tDAC)
{
    /* 使能DAC时钟 */
    __HAL_RCC_DAC_CLK_ENABLE();
}

/**
 * @brief DAC参数配置函数
 * @param _tDAC-DAC结构体指针
 * @retval Null
*/
static void S_DAC_ParamConfig(tagDAC_T *_tDAC)
{
    _tDAC->tDACHandle.Instance = DAC;

    /* DAC句柄初始化 */
    if(HAL_DAC_Init(&_tDAC->tDACHandle) != HAL_OK)
    {
        Drv_HAL_Error(__FILE__, __LINE__);
    }

    /* DAC通道功能初始化 */
    if(HAL_DAC_ConfigChannel(&_tDAC->tDACHandle,&_tDAC->tDACChannel,_tDAC->ulDACChannelSelect) != HAL_OK)
    {
        Drv_HAL_Error(__FILE__, __LINE__);
    }
}

/**
 * @brief DAC GPIO配置函数
 * @param _tDAC-DAC结构体指针
 * @retval Null
*/
static void S_DAC_GPIOConfig(tagDAC_T *_tDAC)
{
    /* 如果是通道1，对PA4进行配置 */
    if(_tDAC->ulDACChannelSelect == DAC_CHANNEL_1)
    {
        _tDAC->tGPIO.tGPIOInit.Pin  = GPIO_PIN_4;
        _tDAC->tGPIO.tGPIOInit.Mode = GPIO_MODE_ANALOG;
        _tDAC->tGPIO.tGPIOInit.Pull = GPIO_NOPULL;
        _tDAC->tGPIO.tGPIOPort      = GPIOA;
    }

    /* 如果是通道2，对PA4进行配置 */
    if(_tDAC->ulDACChannelSelect == DAC_CHANNEL_2)
    {
        _tDAC->tGPIO.tGPIOInit.Pin  = GPIO_PIN_5;
        _tDAC->tGPIO.tGPIOInit.Mode = GPIO_MODE_ANALOG;
        _tDAC->tGPIO.tGPIOInit.Pull = GPIO_NOPULL;
        _tDAC->tGPIO.tGPIOPort      = GPIOA;
    }

	Drv_GPIO_Init(&_tDAC->tGPIO, 1);
}

/**
 * @brief DAC初始化函数
 * @param _tDAC-DAC结构体指针
 * @param _ucNum-初始化DAC个数,个数上限为2
 * @retval Null
*/
void Drv_DAC_Init(tagDAC_T *_tDAC, uint8_t _ucNum)
{
	uint8_t index;

    /* 因为DAC只有两个，初始化数量大于2报错 */
	if(_ucNum > 2)
    {
        Drv_HAL_Error(__FILE__, __LINE__);
        while(1);
    }

	for(index = 0; index < _ucNum; index++)
	{
		S_DAC_CLKEnable(&_tDAC[index]);		/* DAC时钟使能 */
		S_DAC_GPIOConfig(&_tDAC[index]);	/* DAC的GPIO配置 */
		S_DAC_ParamConfig(&_tDAC[index]);	/* DAC参数配置 */
	}
}

/**
 * @brief DAC设置直接值函数
 * @param _tDAC-DAC结构体指针
 * @param _ulData-设置直接值，范围[0,4095]
 * @retval Null
*/
void Drv_DAC_SetData(tagDAC_T *_tDAC, uint32_t _ulData)
{
    if(_ulData > 4095)
    {
        Drv_HAL_Error(__FILE__, __LINE__);
        while(1);
    }

    /* 开启DAC输出 */
    HAL_DAC_Start(&_tDAC->tDACHandle,_tDAC->ulDACChannelSelect);

    /* 设置DAC输出值 */
    HAL_DAC_SetValue(&_tDAC->tDACHandle,_tDAC->ulDACChannelSelect,DAC_ALIGN_12B_R,_ulData);
}

/**
 * @brief DAC设置实际电压值函数
 * @param _tDAC-DAC结构体指针
 * @param _ulValue-设置实际电压值，范围[0,3.3]V
 * @retval Null
*/
void Drv_DAC_SetValue(tagDAC_T *_tDAC, float _fValue)
{
    uint32_t ulDACData = 0;

    if(_fValue < 0 || _fValue > 3.3)
    {
        Drv_HAL_Error(__FILE__, __LINE__);
        while(1);
    }

    /* 转换为直接值 */
    ulDACData = (uint32_t)(_fValue / 3.3 * 4095);

    /* 开启DAC输出 */
    HAL_DAC_Start(&_tDAC->tDACHandle,_tDAC->ulDACChannelSelect);

    /* 设置DAC输出值 */
    HAL_DAC_SetValue(&_tDAC->tDACHandle,_tDAC->ulDACChannelSelect,DAC_ALIGN_12B_R,ulDACData);
}

#endif
