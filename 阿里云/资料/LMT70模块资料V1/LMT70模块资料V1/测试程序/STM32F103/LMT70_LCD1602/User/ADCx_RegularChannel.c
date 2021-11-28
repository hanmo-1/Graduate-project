//-----------------------------------------------------------------
// ��������:
// ����ADC��������
// ��������: ���ǵ���
// ��ʼ����: 2014-02-18
// �������: 2014-02-18
// �޸�����: 2014-02-18
// �桡����: V1.0
// ��- V1.0:
// ���Թ���: ����STM32���Ŀ����塢2.8��Һ������LZE_ST_LINK2
// ˵������:
//
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include <stm32f10x.h>
#include "ADCx_RegularChannel.h"

//-----------------------------------------------------------------
// ȫ�ֱ�������
//-----------------------------------------------------------------
__IO uint16_t ADC_RegularConvertedValueTab[Channel];

//-----------------------------------------------------------------
// ��ʼ��������
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// void GPIO_ADC_Configuration(void)
//-----------------------------------------------------------------
//
// ��������: ADCͨ��GPIO����
// �������: ��
// �������: ��
// �� �� ֵ: ��
// ȫ�ֱ���: ��
// ���ú���: ��
// ע������: ������ͨ�����16����ע����ͨ�����4��
//          PC0-->ADCͨ��10,ģ������
//           PC1-->ADCͨ��11,ģ������
//          PC2-->ADCͨ��12,ģ������
//          PC3-->ADCͨ��13,ģ������
//          PC4-->ADCͨ��14,ģ������
//          PC5-->ADCͨ��15,ģ������
//          PB0-->ADCͨ��8 ,ģ������
//          PB1-->ADCͨ��9 ,ģ������
//
//-----------------------------------------------------------------
void GPIO_ADC_Configuration(ADC_TypeDef *ADCx)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
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
  // ���� ADC ģʽ
  ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;
  // ��ͨ��ģʽ
  ADC_InitStructure.ADC_ScanConvMode       = DISABLE;
  // ����ת��ģʽ
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  // ʹ���ⲿ��������������ͨ����ģ��ת��
  ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
  // �ɼ������Ҷ���
  ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;
  // �涨��˳����й���ת����ADCͨ������Ŀ:1
  ADC_InitStructure.ADC_NbrOfChannel       = Channel;
  // ����ADC1�ļĴ���
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
  // ����ADC��У׼����
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
  // ʹ������DMA1ʱ��
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  // ��λDMA
  DMA_DeInit(DMA1_Channel1);
  // ����DMA����ADC����ַ
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  // ����DMA�ڴ����ַ
  DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)ADC_RegularConvertedValueTab;
  // �涨����������Ϊ���ݴ����Ŀ�ĵػ�����Դ�����ݴ��������
  DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
  // ����ָ��DMAͨ����DMA����Ĵ�С,��λΪ���ݵ�λ
  DMA_InitStructure.DMA_BufferSize         = Channel;
  // �趨�����ַ�Ĵ����������
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
  // �趨�ڴ��ַ�Ĵ����������
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
  // �趨���������ݿ�� �����ݿ��Ϊ16λ,ADCΪ12λ�ֱ���
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  // �趨���ڴ����ݿ��  16
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
  // ������DMA�Ĺ���ģʽ: ������ѭ������ģʽ
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
  // �趨 DMAͨ�� x ��������ȼ�:�����ȼ�
  DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
  // ʧ��DMAͨ�����ڴ浽�ڴ洫��
  DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
  // ��ʼ��DMA�Ĵ���
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  // ʹ��DMAͨ��1
  DMA_Cmd(DMA1_Channel1, ENABLE);
}

//-----------------------------------------------------------------
// void ADC_RegularChannel_Configuration(ADC_TypeDef* ADCx)
//-----------------------------------------------------------------
//
// ��������: ADC���������ú���
// ��ڲ���: ��
// ���ز���: ��
// ȫ�ֱ���: ��
// ����ģ��:
// ע������: ADC���ʱ�Ӳ�����14MHz,��������Ϊϵͳʱ�ӵ�6��Ƶ����72M/6=12MHz
//
//-----------------------------------------------------------------
void ADC_RegularChannel_Configuration(ADC_TypeDef *ADCx)
{
  // ��ADC1ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 |RCC_APB2Periph_ADC2, ENABLE);
  RCC_ADCCLKConfig(RCC_PCLK2_Div8); // ���� ADCʱ�ӣ�ΪPCLK2��8��Ƶ,
  GPIO_ADC_Configuration(ADCx);     // ADCx IO����
  DMA1_ADC_Configuration();         // ����ADC��DMA1�е�ͨ��
  ADC_Initialization(ADCx);         // ADCx��ʼ��
  // ѡ��ADC1ͨ��14�����ڵ�һλ�ɼ�������ʱ��Ϊ239.5����:����LMT70�����ѹ
  ADC_RegularChannelConfig(ADCx, ADC_Channel_14, 1, ADC_SampleTime_239Cycles5);
  ADC_DMACmd(ADCx, ENABLE);               // ʹ��ADCx��DMAͨ��
  ADC_TempSensorVrefintCmd(ENABLE);       // ʹ�ܻ�׼Դͨ��17
  ADC_Cmd(ADCx, ENABLE);                  // ʹ��ADCx
  ADC_Calibration(ADCx);                  // ADCx�Զ�У׼
  ADC_SoftwareStartConvCmd(ADCx, ENABLE); // ����ADCx���ת��
}


//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
