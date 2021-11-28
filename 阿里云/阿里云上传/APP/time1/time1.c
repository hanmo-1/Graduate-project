#include "time1.h"
#include "led.h"
#include "adc.h"
#include "mqtt.h"          //������Ҫ��ͷ�ļ�
#include "wifi.h"	       //������Ҫ��ͷ�ļ�
#include "usart.h"
#include "main.h"
#include "string.h"



TIM_HandleTypeDef TIM1_Base_Handler;      //��ʱ����� 

//ͨ�ö�ʱ��3�жϳ�ʼ��
//per���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!(��ʱ��3����APB1�ϣ�ʱ��ΪHCLK/2)
//Ŀ�ģ������ADC����ź���һ���Ķ�ʱ����ʾ����
void TIM2_Init(u16 per,u16 psc)
{  
    TIM2_Base_Handler.Instance=TIM2;                          //ͨ�ö�ʱ��3
    TIM2_Base_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    TIM2_Base_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM2_Base_Handler.Init.Period=per;                        //�Զ�װ��ֵ
    TIM2_Base_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&TIM2_Base_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM2_Base_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE   
}





//��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
//�˺����ᱻHAL_TIM_Base_Init()��������
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();            //ʹ��TIM3ʱ��
		HAL_NVIC_SetPriority(TIM2_IRQn,1,1);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM2_IRQn);          //����ITM3�ж�   
	}
}


//��ʱ��2�жϷ�����
void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM2_Base_Handler);
}



//�ص���������ʱ���жϷ���������
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	 if(htim==(&TIM2_Base_Handler))
	 {
		if((__HAL_TIM_GET_FLAG(&TIM2_Base_Handler,TIM_IT_UPDATE)!=RESET)){  //���TIM_IT_Update��λ����ʾTIM2����жϣ�����if	
			TempHumi_State();
//			TIM_ClearITPendingBit(TIM2, TIM_IT_Update);     //���TIM2����жϱ�־ 
      __HAL_TIM_CLEAR_FLAG(&TIM2_Base_Handler, TIM_IT_UPDATE);			
		}
	 }	
	 
	
//	u16 value0,value1,value2,value3,value4,value5,value6,value7,value8,value9,value10,value11,value12,value13,value14,value15;
//	float vol0,vol1,vol2,vol3,vol4,vol5,vol6,vol7,vol8,vol9,vol10,vol11,vol12,vol13,vol14,vol15;
//    if(htim==(&TIM3_Base_Handler))
//    {
////        LED2=!LED2;        //LED1��ת
//		value0 = Get_Adc(ADC_CHANNEL_0);
//		value1 = Get_Adc(ADC_CHANNEL_1);
//		value2 = Get_Adc(ADC_CHANNEL_2);
//		value3 = Get_Adc(ADC_CHANNEL_3);
//		value4 = Get_Adc(ADC_CHANNEL_4);
//		value5 = Get_Adc(ADC_CHANNEL_5);
//		value6 = Get_Adc(ADC_CHANNEL_6);
//		value7 = Get_Adc(ADC_CHANNEL_7);
//		value8 = Get_Adc(ADC_CHANNEL_8);
//		value9 = Get_Adc(ADC_CHANNEL_9);
//		value10 = Get_Adc(ADC_CHANNEL_10);
//		value11 = Get_Adc(ADC_CHANNEL_11);
//		value12 = Get_Adc(ADC_CHANNEL_12);
//		value13 = Get_Adc(ADC_CHANNEL_13);
//		value14 = Get_Adc(ADC_CHANNEL_14);
//		value15 = Get_Adc(ADC_CHANNEL_15);
//		vol0=(float)value0*(3.3/4096);
//		vol1=(float)value1*(3.3/4096);
//		vol2=(float)value2*(3.3/4096);
//		vol3=(float)value3*(3.3/4096);
//		vol4=(float)value4*(3.3/4096);
//		vol5=(float)value5*(3.3/4096);
//		vol6=(float)value6*(3.3/4096);
//		vol7=(float)value7*(3.3/4096);
//		vol8=(float)value8*(3.3/4096);
//		vol9=(float)value9*(3.3/4096);
//		vol10=(float)value10*(3.3/4096);
//		vol11=(float)value11*(3.3/4096);
//		vol12=(float)value12*(3.3/4096);
//		vol13=(float)value13*(3.3/4096);
//		vol14=(float)value14*(3.3/4096);
//		vol15=(float)value15*(3.3/4096);
//		
//		printf("%.2f%.2f%.2f%.2f%.2f%.2f%.2f%.2f%.2f%.2f%.2f%.2f%.2f%.2f%.2f%.2f",vol0,vol1,vol2,vol3,vol4,vol5,vol6,vol7,vol8,vol9,vol10,vol11,vol12,vol13,vol14,vol15);
//		
//    }
}

