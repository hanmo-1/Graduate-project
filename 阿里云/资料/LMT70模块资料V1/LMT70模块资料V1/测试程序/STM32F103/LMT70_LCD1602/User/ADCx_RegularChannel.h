//-----------------------------------------------------------------
// ����ADC��ʼ��ͷ�ļ�
// ͷ�ļ���: ADCx_RegularChannel.h
// ��    ��: ���ǵ���
// ��дʱ��: 2014-02-18
// �޸�����:
//-----------------------------------------------------------------

#ifndef _ADCx_RegularChannel_H
#define _ADCx_RegularChannel_H

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
// ģ��������ת����ʽ
#define AnalogVoltage(n) (ADC_RegularConvertedValueTab[n]*VREF/4095)
// ���������ת��ͨ��
#define Channel  1
//// ƽ����ѹģ��������ת����ʽ
//#define Vtemp_sensor (float)(Covert*VREF/4095.0)
	
//-----------------------------------------------------------------
// �ⲿ��������
//-----------------------------------------------------------------
extern __IO uint16_t ADC_RegularConvertedValueTab[Channel];

//-----------------------------------------------------------------
// �ⲿ��������
//-----------------------------------------------------------------
extern void GPIO_ADC_Configuration(ADC_TypeDef* ADCx);
extern void DMA1_ADC_Configuration(void);
extern void ADC_Calibration(ADC_TypeDef * ADCx);
extern void ADC_Initialization(ADC_TypeDef* ADCx);
extern void ADC_RegularChannel_Configuration(ADC_TypeDef* ADCx);

#endif

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
