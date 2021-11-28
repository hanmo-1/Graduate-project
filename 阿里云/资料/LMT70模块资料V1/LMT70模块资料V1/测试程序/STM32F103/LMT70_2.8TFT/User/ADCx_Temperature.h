//-----------------------------------------------------------------
// ����ADC��ʼ��ͷ�ļ�
// ͷ�ļ���: ADCx_Temperature.h
// ��    ��: ���ǵ���
// ��дʱ��: 2014-02-17
// �޸�����:
//-----------------------------------------------------------------

#ifndef _ADCx_Temperature_H
#define _ADCx_Temperature_H

//-----------------------------------------------------------------
// �궨�壺�����ַ
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
// ��׼��ѹ
#define VREF                  3300
// �¶ȴ�����ģ������ת����ʽ
#define V25                   1430
#define Vdd_convert_value     0xfff
#define Avg_Slope             43.5
#define Vtemp_sensor ( ADCConvertedValue *VREF /Vdd_convert_value)
//#define Temperature ((V25  -Vtemp_sensor)/Avg_Slope+25)

//-----------------------------------------------------------------
// �ⲿ��������
//-----------------------------------------------------------------
extern __IO uint16_t ADCConvertedValue;

//-----------------------------------------------------------------
// �ⲿ��������
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
