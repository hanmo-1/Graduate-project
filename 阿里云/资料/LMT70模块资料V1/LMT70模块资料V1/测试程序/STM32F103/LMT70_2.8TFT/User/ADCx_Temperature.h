//-----------------------------------------------------------------
// 外设ADC初始化头文件
// 头文件名: ADCx_Temperature.h
// 作    者: 凌智电子
// 编写时间: 2014-02-17
// 修改日期:
//-----------------------------------------------------------------

#ifndef _ADCx_Temperature_H
#define _ADCx_Temperature_H

//-----------------------------------------------------------------
// 宏定义：外设地址
//-----------------------------------------------------------------
#define SPI2_DR_Address       0x4000380C
#define ADC1_DR_Address       0x4001244C
#define ADC3_DR_Address       0x40013C4C  
#define DAC_DHR12RD_Address   0x40007420
#define DAC_DHR8R1_Address    0x40007410
#define TIM1_CCR1_Address     0x40012C34
#define TIM1_CCR3_Address     0x40012C3C
#define USART1_DR_Base        0x40013804
#define USART2_DR_Base        0x40004404
// 基准电压
#define VREF                  3300
// 温度传感器模拟数字转换公式
#define V25                   1430
#define Vdd_convert_value     0xfff
#define Avg_Slope             43.5
#define Vtemp_sensor ( ADCConvertedValue *VREF /Vdd_convert_value)
//#define Temperature ((V25  -Vtemp_sensor)/Avg_Slope+25)

//-----------------------------------------------------------------
// 外部变量声明
//-----------------------------------------------------------------
extern __IO uint16_t ADCConvertedValue;

//-----------------------------------------------------------------
// 外部函数声明
//-----------------------------------------------------------------
extern void ADC_Temperature_Configuration(ADC_TypeDef* ADCx);
extern void DMA1_ADC_Configuration(void);
extern void ADC_Calibration(ADC_TypeDef * ADCx);
extern void ADC_Initialization(ADC_TypeDef* ADCx);
extern void ADC_Configuration(ADC_TypeDef* ADCx);

#endif

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
