#include "adc.h"
#define ADC1_DR_Address               ((uint32_t)0x4001244c)//
__IO uint16_t RegularConvData_Tab[2];//�洢2����ѹֵ
__IO uint16_t  ADC1ConvertedValue = 0, ADC1ConvertedVoltage = 0;
__IO uint16_t VREFINT_CAL;
__IO uint16_t VREFINT_DATA;
__IO float Vbat_value;
void ADC_Config(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_ADC1,ENABLE);	
 
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;		 
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;//ADC��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&RegularConvData_Tab; //�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //����(�����赽�ڴ�)
	DMA_InitStructure.DMA_BufferSize = 1; //�������ݵĴ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable; //�ڴ��ַ�̶�
	DMA_InitStructure.DMA_PeripheralDataSize = 
	DMA_PeripheralDataSize_HalfWord ; //�������ݵ�λ
	DMA_InitStructure.DMA_MemoryDataSize = 
	DMA_MemoryDataSize_HalfWord ;    //�ڴ����ݵ�λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular  ; //DMAģʽ��ѭ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_High ; //���ȼ�����
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;   //��ֹ�ڴ浽�ڴ�Ĵ���
	
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);  //����DMA1��1ͨ��
	DMA_Cmd(DMA1_Channel1,ENABLE);
 
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //����ADCģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;  //��ֹɨ�跽ʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//��������ת��ģʽ 
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //��ʹ���ⲿ����ת��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //�ɼ������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1; //Ҫת����ͨ����Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);//����ADCʱ�ӣ�ΪPCLK2��8��Ƶ����9Hz
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5);//����ADC1ͨ��1Ϊ55.5���������� 
	
	ADC_DMACmd(ADC1,ENABLE);
	ADC_Cmd(ADC1,ENABLE);
 
	ADC_ResetCalibration(ADC1);//��λУ׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));//�ȴ�У׼�Ĵ�����λ���
 
	ADC_StartCalibration(ADC1);//ADCУ׼
	while(ADC_GetCalibrationStatus(ADC1));//�ȴ�У׼���
 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//����û�в����ⲿ����������ʹ���������ADCת��

}
/***************��ȡ��׼��ѹֵ*****************/
float Get_VREFINT(void)
{
     /* Test DMA1 TC flag */
    if((DMA_GetFlagStatus(DMA1_FLAG_TC1)) )
    {
    /* Clear DMA TC flag */
    DMA_ClearFlag(DMA1_FLAG_TC1);
 
    } 
      VREFINT_DATA=RegularConvData_Tab[0];//��ȡ��ADCֵ
      Vbat_value =((3.3*VREFINT_DATA)/4096.0);//�ȴ����� �����ѹ��
    return Vbat_value;//���ص�ǰֵ  
}

