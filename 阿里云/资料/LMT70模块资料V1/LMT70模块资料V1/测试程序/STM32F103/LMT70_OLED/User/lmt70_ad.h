#ifndef lmt70_ad
#define lmt70_ad

#include <stm32f10x.h>
//-----------------------------------------------------------------
// 宏定义：外设地址
//-----------------------------------------------------------------
#define ADC1_DR_Address       0x4001244C 
#define Channel 1
// 基准电压
#define VREF                  3300
// 模拟数字量转换公式, 单位mV
#define AnalogVoltage(n) ((float)ADC_RegularConvertedValueTab[n]*VREF/4095*1000)
extern double temp,receive;
//-----------------------------------------------------------------
// 外部变量声明
//-----------------------------------------------------------------
extern __IO uint16_t ADC_RegularConvertedValueTab[Channel];
//-----------------------------------------------------------------
// 外部函数声明
//-----------------------------------------------------------------
void ADC_Initialization(ADC_TypeDef *ADCx);
void ADC_Calibration(ADC_TypeDef *ADCx);
void DMA1_ADC_Configuration(void);
void ADC_RegularChannel_Configuration(ADC_TypeDef *ADCx);
void caculation(void);
void average(void);




#endif

