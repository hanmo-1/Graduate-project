//-----------------------------------------------------------------
// 程序描述:
// 		 ADC驱动程序头文件
// 作    者: 凌智电子
// 开始日期: 2018-08-04
// 完成日期: 2018-08-04
// 修改日期: 
// 当前版本: V1.0
// 历史版本:
//  - V1.0: (2018-08-04)ADC初始化
// 调试工具: 凌智STM32F429+Cyclone IV电子系统设计开发板、LZE_ST_LINK2
// 说    明: 
//    
//-----------------------------------------------------------------
#ifndef __ADC_H
#define __ADC_H
#include "stm32f429_winner.h"
//-----------------------------------------------------------------
// 宏定义
//-----------------------------------------------------------------
#define ADC_Vref	3.0			// ADC基准电压

//-----------------------------------------------------------------
// ADC引脚定义
//-----------------------------------------------------------------
#define ADC_Instance													 ADC1
#define ADC_PIN                                GPIO_PIN_5
#define ADC_GPIO_PORT                          GPIOA
#define ADC_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOA_CLK_ENABLE()  
#define ADC_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOA_CLK_DISABLE() 
#define ADC_CLK_ENABLE()                  		 __HAL_RCC_ADC1_CLK_ENABLE()  

//-----------------------------------------------------------------
// 函数声明
//-----------------------------------------------------------------
extern void MY_ADC_Init(void); 								// ADC通道初始化
extern u16  Get_Adc(u32 ch); 		        			// 获得某个通道值 
extern u16 Get_Adc_Average(u32 ch,u8 times);	// 得到某个通道给定次数采样的平均值
#endif 
//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
