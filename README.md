# SGA Library

- 一个基于STM32F1系列和STM32L4系列的代码库

## 基本说明

- 本代码库由杭州电子科技大学SIGMA团队制作。适用于绝大多数开发板，包括正点原子和野火的板子，最佳适配为IM板。

## 工程目录说明

**Doc** : 工程记录文件

- 工程目录介绍.txt :工程文件目录
- 工程维护记录.txt : 工程文件修改、更新、增加、删除等系列维护记录，最新记录在最上面
- 句柄资源实例.txt : 库函数封装结构体句柄使用实例

**Apply** : 应用层

- **Logic** : 逻辑层——用户逻辑源代码（抽象执行逻辑）
- **Task** : 业务层——用户业务源代码（具体执行实现）

**Bsp**: 板级层

- **Algo** : 算法库——常用函数与控制算法
- **Dev** : 设备驱动——遥感以及电机驱动
- **OCD** : 片外外设——需要使用的片外外设驱动
- **FatFs** : 文件系统——开源文件系统
- **RTOS** : 实时操作系统——各第三方支持RTOS系统

**Driver** : 驱动层（非架构开发者不要轻易修改）

**Hardware**: 硬件抽象层

- **STM32F1XX**
    CMSIS : 
    - core版本: V5.6.0_cm4
    - device F1版本：V4.3.2 
    - STM32F1XX系列MCU的HAL固件库源代码、驱动程序版本: V1.1.7
- **STM32L4XX**
    CMSIS : 
    - core版本: V5.4.0_cm4
    - device L4版本：V1.5.1
    - STM32L4XX系列MCU的HAL固件库源代码、驱动程序版本: V1.10.0 

**Project**		: 存放工程相关文件


## Apply层

一般的STM32开发只需要在此层修改。

在Apply层中有两个文件夹，分别是Logic和Task。

1. Logic中

    - **main.c**  		    最上层逻辑抽象，一般不需要更改

    - **usercode.c**      如果使用裸机模式，可以在/* 用户逻辑代码 */下直接书写代码

    - **threadpool.c**   在使用RTOS时，需要在工程目录中加入此文件，此时usercode文件将作为线程初始化使用，而threadpool中将存放对应的线程入口函数
    
    - **config.c**           该文件存放所有用到的I/O口初始化参数配置，在**句柄资源实例**中有相应的实例
    
2. Task中
    - **task_irq.c**          存放中断处理函数以及中断返回服务函数
    
    - **task_userinit.c** 存放所有的初始化，例如串口，IIC
    
    - **task_sysinit.c**    该文件为hal库启动，不需要做修改
    
        剩下其他外设函数可以自建文件

## BSP/Algo层

存放常用函数与控制算法

目前已有常用函数驱动：func、增量式PID

## BSP/Dev层

存放设备驱动，例如遥感和电机驱动

目前已有ps2手柄驱动：PS2

## BSP/OCD层

存放第三方芯片的驱动代码

目前已有的芯片驱动：ADS1256、AT24xx、DS3231、JY901、RM3100

目前已有的外设驱动：OLED、SDcard

## Doc层

存放工程文档，开发人员可以在**工程维护记录**中写上维护记录

## Driver层

- 目前驱动层已有adc、dac、gpio、iic_soft、pwm、pwr、spi_soft、spi、timer、uart、wdg相关驱动可以直接使用，不排除有隐藏BUG的可能

## RTOS支持

- **RT-Thread** —— 使用时须在**drv_hal_conf.h**中解开`#define RTT_ENABLE`注释，同时在工程中加入kernel文件
- **FreeRTOS** —— 使用时须在**drv_hal_conf.h**中解开`#define FREERTOS_ENABLE`注释，同时在工程中加入kernel文件
- 其他操作系统等待后续使用后加入

## 库函数维护

fork该工程，通过PR来完善库函数。除Apply层之外的所有地方皆可进行维护。

开发人员需在修改代码前阅读Doc中《SGA库代码规范》。

## 库版本更新

- 从GitHub上下载最新版本：[PoisonNF/SGA_Demo: Repair and test of SGA Library. (github.com)](https://github.com/PoisonNF/SGA_Demo)

- 从旧版本更新：

    - 2.0之前的版本，**Doc、Driver、Hardware、Project**可以覆盖，对于**Bsp**文件夹，除bsp（内含bsp_io.c）以外，都可以覆盖

    - 从2.0版本开始，SGA库的更新仅需要覆盖**Bsp、Doc、Driver、Hardware、Project**文件夹（除Apply）即可。

## 声明

- 本软件开源、免费，仅供学习交流使用。

## 制作

- 本README.md由PoisonNF书写
