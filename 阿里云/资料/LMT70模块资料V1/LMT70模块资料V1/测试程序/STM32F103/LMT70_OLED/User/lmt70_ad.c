//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include "stm32f10x.h"
#include "lmt70_ad.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"
#include "delay.h"
//-----------------------------------------------------------------
// ȫ�ֱ�������
//-----------------------------------------------------------------
double temp,receive;          //temp�����¶ȣ�receive���յ�ѹ

__IO uint16_t ADC_RegularConvertedValueTab[Channel];

//-----------------------------------------------------------------
// void GPIO_ADC_Configuration(void)
//-----------------------------------------------------------------
//
// ��������: ADCͨ��GPIO����
// �������: ��
// �������: ��
// �� �� ֵ: ��
// ע������: PA1-->ADCͨ��1,ģ������                 
//-----------------------------------------------------------------
void GPIO_ADC_Configuration()
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}
//-----------------------------------------------------------------
// void ADC_Initialization(ADC_TypeDef* ADCx)
//-----------------------------------------------------------------
//
// ��������: ADC��ʼ��
// ��ڲ���: ��
// ���ز���: ��
// ע������: ��
//
//-----------------------------------------------------------------
void ADC_Initialization(ADC_TypeDef *ADCx)
{
  ADC_InitTypeDef ADC_InitStructure;
  // ����ÿ��ADC�����ڶ���ģʽ
  ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;
  // �涨��ģ��ת��������ͨ��ģʽ
  ADC_InitStructure.ADC_ScanConvMode       = DISABLE;
  // �涨��ģ��ת������������ת��ģʽ
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  // ����ͨ��
  ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
  // �Ҷ���
  ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;
  //��Ŀ��1
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
// ע������:
//
//-----------------------------------------------------------------
void ADC_Calibration(ADC_TypeDef *ADCx)
{
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
// ע������: ��
//
//-----------------------------------------------------------------
void DMA1_ADC_Configuration(void)
{
  DMA_InitTypeDef DMA_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

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
  //ѭ������
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
  // �趨 DMAͨ�� x ��������ȼ�:�����ȼ�
  DMA_InitStructure.DMA_Priority           = DMA_Priority_High;

  DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
  // ��ʼ��DMA�Ĵ���
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  DMA_Cmd(DMA1_Channel1, ENABLE);
}

//-----------------------------------------------------------------
// void ADC_RegularChannel_Configuration(ADC_TypeDef* ADCx)
//-----------------------------------------------------------------
//
// ��������: ADC���������ú���
// ��ڲ���: ��
// ���ز���: ��
// ע������: ADC���ʱ�Ӳ�����14MHz,��������Ϊϵͳʱ�ӵ�6��Ƶ����72M/6=12MHz
//
//-----------------------------------------------------------------
void ADC_RegularChannel_Configuration(ADC_TypeDef *ADCx)
{
  // ��ADC1ʱ��
	GPIO_ADC_Configuration();          // ADCx IO����
	DMA1_ADC_Configuration();          // ����ADC��DMA1�е�ͨ��
  ADC_Calibration(ADCx);             // ADCx��ʼ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
  RCC_ADCCLKConfig(RCC_PCLK2_Div6); // ���� ADCʱ��
  ADC_Initialization(ADCx);         
  // ѡ��ADC1ͨ��1�����ڵ�һλ�ɼ�������ʱ��Ϊ41.5����:����DAͨ��1�����ѹ
  ADC_RegularChannelConfig(ADCx, ADC_Channel_1, 1, ADC_SampleTime_41Cycles5);
  ADC_DMACmd(ADCx, ENABLE);               
  ADC_TempSensorVrefintCmd(ENABLE);       
  ADC_Cmd(ADCx, ENABLE);                  // ʹ��ADCx
  ADC_Calibration(ADCx);                  // ADCx�Զ�У׼
  ADC_SoftwareStartConvCmd(ADCx, ENABLE); // ����ADCx���ת��
}
//-----------------------------------------------------------------
// void average(void)
//-----------------------------------------------------------------
//
// ��������: ȡ100�������¶�ȡƽ������
// ��ڲ���: ��
// ���ز���: ��
// ע������: 
//
//-----------------------------------------------------------------
void average(void)
{
 u8 i;
 u32 tem[200];
	Delay_5ms(5);
	for(i=0;i<200;i++)
	{
     tem[i]=AnalogVoltage(0);		 
		 receive+=tem[i];
	
  }
		 receive/=200000;
   
}


//-----------------------------------------------------------------
// void caculation(void)
//-----------------------------------------------------------------
//
// ��������: �¶ȼ��㺯��
// ��ڲ���: ��
// ���ز���: ��
// ע������: 
//
//-----------------------------------------------------------------
void caculation(void)
{
	   average();
   //�����¶�
    temp=204.6398-receive*receive*7.857923/1000000-receive*0.1777501;
	
	       
}


//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------



