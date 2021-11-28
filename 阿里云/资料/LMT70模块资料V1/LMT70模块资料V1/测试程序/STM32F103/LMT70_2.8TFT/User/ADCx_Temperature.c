//-----------------------------------------------------------------
// ��������:
// ����ADC��������
// ��������: ���ǵ���
// ��ʼ����: 2014-02-17
// �������: 2014-02-17
// �޸�����: 2014-02-17
// �桡����: V1.0
// ��- V1.0:
// ���Թ���: ����STM32+FPGA����ϵͳ��ƿ����塢2.8��Һ������
//            LZE_ST_LINK2
// ˵������:
//
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include <stm32f10x.h>
#include "ADCx_Temperature.h"

//-----------------------------------------------------------------
// ȫ�ֱ�������
//-----------------------------------------------------------------
__IO uint16_t ADCConvertedValue;

//-----------------------------------------------------------------
// ��ʼ��������
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// void GPIO_ADC_Configuration(ADC_TypeDef* ADCx)
//-----------------------------------------------------------------
//
// ��������: ADC�������ú���
// ��ڲ���: ��
// ���ز���: ��
// ȫ�ֱ���: ��
// ����ģ��: ��
// ע������: ��
//
//-----------------------------------------------------------------
void GPIO_ADC_Configuration(ADC_TypeDef *ADCx)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//-----------------------------------------------------------------
// void ADC_Initialization(ADC_TypeDef* ADCx)
//-----------------------------------------------------------------
//
// ��������: ADC��ʼ��
// ��ڲ���: ��
// ���ز���: ��
// ȫ�ֱ���: ��
// ����ģ��: ��
// ע������: ��
//
//-----------------------------------------------------------------
void ADC_Initialization(ADC_TypeDef *ADCx)
{
  ADC_InitTypeDef ADC_InitStructure;
  // ADC1��ADC2�����ڶ���ģʽ
  ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;
  // ��ֹ��ͨ��
  ADC_InitStructure.ADC_ScanConvMode       = DISABLE;
  // ����ת��ģʽ
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  // ���������ת��
  ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
  // �����Ҷ���
  ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;
  // �涨������ת����ͨ����Ŀ
  ADC_InitStructure.ADC_NbrOfChannel       = 1;
  ADC_Init(ADCx, &ADC_InitStructure);
}

//-----------------------------------------------------------------
// void ADC_Calibration(ADC_TypeDef * ADCx)
//-----------------------------------------------------------------
//
// ��������: ADC�Զ�У׼����
// ��ڲ���: ��
// ���ز���: ��
// ȫ�ֱ���: ��
// ����ģ��: ADC_ResetCalibration(); DC_GetResetCalibrationStatus();
//          ADC_StartCalibration();ADC_GetCalibrationStatus();
// ע������:
//
//-----------------------------------------------------------------
void ADC_Calibration(ADC_TypeDef *ADCx)
{
  // ��λADC��У׼
  ADC_ResetCalibration(ADCx);
  // ���ADC��λУ׼������
  while (ADC_GetResetCalibrationStatus(ADCx))
    ;
  // ����ADC1��У׼����
  ADC_StartCalibration(ADCx);
  // ���ADC��У׼������
  while (ADC_GetCalibrationStatus(ADCx))
    ;
}

//-----------------------------------------------------------------
// void DMA1_ADC_Configuration(void)
//-----------------------------------------------------------------
//
// ��������: ADC��DMAͨ������
// ��ڲ���: ��
// ���ز���: ��
// ȫ�ֱ���: ��
// ����ģ��: ��
// ע������: ��
//
//-----------------------------------------------------------------
void DMA1_ADC_Configuration(void)
{
  DMA_InitTypeDef DMA_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  //DMAͨ��1����
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)&ADCConvertedValue;
  DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize         = 1;
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  // ʹ��DMA1ͨ��1
  DMA_Cmd(DMA1_Channel1, ENABLE);
}

//-----------------------------------------------------------------
// void ADC_Temperature_Configuration(ADC_TypeDef* ADCx)
//-----------------------------------------------------------------
//
// ��������: ADC���ú���
// ��ڲ���: ��
// ���ز���: ��
// ȫ�ֱ���: ��
// ����ģ��: ��
// ע������: ��
//
//-----------------------------------------------------------------
void ADC_Temperature_Configuration(ADC_TypeDef *ADCx)
{
  // ��ADCʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  // ADCx IO������
  GPIO_ADC_Configuration(ADCx);
  // ����ADC��DMA1�е�ͨ��
  DMA1_ADC_Configuration();
  // ADCx��ʼ��
  ADC_Initialization(ADCx);
  // ѡ��ADCxͨ��16�¶ȴ�����ģ����Ϊ����
  ADC_RegularChannelConfig(ADCx, ADC_Channel_16, 1, ADC_SampleTime_55Cycles5);
  // ʹ��ADCx��DMAͨ��
  ADC_DMACmd(ADCx, ENABLE);
  // ʹ���¶ȴ�����
  ADC_TempSensorVrefintCmd(ENABLE);
  // ʹ��ADCx
  ADC_Cmd(ADCx, ENABLE);
  // ADCx�Զ�У׼
  ADC_Calibration(ADCx);
  // ����ADCx���ת��
  ADC_SoftwareStartConvCmd(ADCx, ENABLE);
}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
