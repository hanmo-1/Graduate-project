#ifndef lmt70_ad
#define lmt70_ad

#include <stm32f10x.h>
//-----------------------------------------------------------------
// �궨�壺�����ַ
//-----------------------------------------------------------------
#define ADC1_DR_Address       0x4001244C 
#define Channel 1
// ��׼��ѹ
#define VREF                  3300
// ģ��������ת����ʽ, ��λmV
#define AnalogVoltage(n) ((float)ADC_RegularConvertedValueTab[n]*VREF/4095*1000)
extern double temp,receive;
//-----------------------------------------------------------------
// �ⲿ��������
//-----------------------------------------------------------------
extern __IO uint16_t ADC_RegularConvertedValueTab[Channel];
//-----------------------------------------------------------------
// �ⲿ��������
//-----------------------------------------------------------------
void ADC_Initialization(ADC_TypeDef *ADCx);
void ADC_Calibration(ADC_TypeDef *ADCx);
void DMA1_ADC_Configuration(void);
void ADC_RegularChannel_Configuration(ADC_TypeDef *ADCx);
void caculation(void);
void average(void);




#endif

