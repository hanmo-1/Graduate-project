//-----------------------------------------------------------------
// ��������:
// 		 ADC��������
// ��    ��: ���ǵ���
// ��ʼ����: 2018-08-04
// �������: 2018-08-04
// �޸�����: 
// ��ǰ�汾: V1.0
// ��ʷ�汾:
//  - V1.0: (2018-08-04)ADC��ʼ��
// ���Թ���: ����STM32F429+Cyclone IV����ϵͳ��ƿ����塢LZE_ST_LINK2
// ˵    ��: 
//    
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include "adc.h"
#include "delay.h"
//-----------------------------------------------------------------

ADC_HandleTypeDef ADC_Handler;//ADC���

//-----------------------------------------------------------------
// void MY_ADC_Init(void)
//-----------------------------------------------------------------
//
// ��������: ��ʼ��ADC
// ��ڲ���: ��
// ���ز���: ��
// ע������: ��
//
//-----------------------------------------------------------------
void MY_ADC_Init(void)
{ 
	ADC_Handler.Instance=ADC_Instance;
	ADC_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   // 4��Ƶ��ADCCLK=PCLK2/4=90/4=22.5MHZ
	ADC_Handler.Init.Resolution=ADC_RESOLUTION_12B;             // 12λģʽ
	ADC_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             // �Ҷ���
	ADC_Handler.Init.ScanConvMode=DISABLE;                      // ��ɨ��ģʽ
	ADC_Handler.Init.EOCSelection=DISABLE;                      // �ر�EOC�ж�
	ADC_Handler.Init.ContinuousConvMode=DISABLE;                // �ر�����ת��
	ADC_Handler.Init.NbrOfConversion=1;                         // 1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_Handler.Init.DiscontinuousConvMode=DISABLE;             // ��ֹ����������ģʽ
	ADC_Handler.Init.NbrOfDiscConversion=0;                     // ����������ͨ����Ϊ0
	ADC_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       // �������
	ADC_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;// ʹ���������
	ADC_Handler.Init.DMAContinuousRequests=DISABLE;             // �ر�DMA����
	HAL_ADC_Init(&ADC_Handler);                                 // ��ʼ�� 
}

//-----------------------------------------------------------------
// void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
//-----------------------------------------------------------------
//
// ��������: ADC�ײ��������������ã�ʱ��ʹ��
// ��ڲ���: ADC_HandleTypeDef* hadc��ADC���
// ���ز���: ��
// ע������: �˺����ᱻHAL_ADC_Init()����
//
//-----------------------------------------------------------------
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
	GPIO_InitTypeDef GPIO_Initure;
	ADC_CLK_ENABLE();         					 // ʹ��ADCʱ��
	ADC_GPIO_CLK_ENABLE();				 			 // ����ADC��GPIOʱ��

	GPIO_Initure.Pin=ADC_PIN;         	 // PA5
	GPIO_Initure.Mode=GPIO_MODE_ANALOG;  // ģ��
	GPIO_Initure.Pull=GPIO_NOPULL;       // ����������
	HAL_GPIO_Init(ADC_GPIO_PORT,&GPIO_Initure);
}

//-----------------------------------------------------------------
// u16 Get_Adc(u32 ch)
//-----------------------------------------------------------------
//
// ��������: ���ADCֵ
// ��ڲ���: u32 ch��ͨ��ֵ 0~16��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
// ���ز���: ת�����
// ע������: �˺����ᱻHAL_ADC_Init()����
//
//-----------------------------------------------------------------
u16 Get_Adc(u32 ch)   
{
	ADC_ChannelConfTypeDef ADC1_ChanConf;
	
	ADC1_ChanConf.Channel=ch;                            // ͨ��
	ADC1_ChanConf.Rank=1;                                // ��1�����У�����1
	ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES; // ����ʱ��
	ADC1_ChanConf.Offset=0;                 
	HAL_ADC_ConfigChannel(&ADC_Handler,&ADC1_ChanConf);  // ͨ������

	HAL_ADC_Start(&ADC_Handler);                         // ����ADC

	HAL_ADC_PollForConversion(&ADC_Handler,10);          // ��ѯת��
 
	return (u16)HAL_ADC_GetValue(&ADC_Handler);	         // �������һ��ADC1�������ת�����
}

//-----------------------------------------------------------------
// u16 Get_Adc_Average(u32 ch,u8 times)
//-----------------------------------------------------------------
//
// ��������: ��ȡָ��ͨ����ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
// ��ڲ���: u32 ch��ͨ��ֵ 0~16��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
//					 u8 times����ȡ����
// ���ز���: ͨ��ch��times��ת�����ƽ��ֵ
// ע������: �˺����ᱻHAL_ADC_Init()����
//
//-----------------------------------------------------------------
u16 Get_Adc_Average(u32 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
