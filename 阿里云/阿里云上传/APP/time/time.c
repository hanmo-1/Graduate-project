#include "time.h"
#include "time2.h"
#include "time3.h"
#include "time4.h"
#include "led.h"
#include "adc.h"
#include "mqtt.h"          //������Ҫ��ͷ�ļ�
#include "wifi.h"	       //������Ҫ��ͷ�ļ�
#include "usart.h"
#include "main.h"
#include "string.h"



//TIM_HandleTypeDef TIM2_Base_Handler, TIM3_Base_Handler, TIM4_Base_Handler;      //��ʱ����� 


////ͨ�ö�ʱ��3�жϳ�ʼ��
////per���Զ���װֵ��
////psc��ʱ��Ԥ��Ƶ��
////��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
////Ft=��ʱ������Ƶ��,��λ:Mhz
////����ʹ�õ��Ƕ�ʱ��3!(��ʱ��3����APB1�ϣ�ʱ��ΪHCLK/2)
////Ŀ�ģ������ADC����ź���һ���Ķ�ʱ����ʾ����
//void TIM2_Init(u16 per,u16 psc)
//{  
//    TIM2_Base_Handler.Instance=TIM2;                          //ͨ�ö�ʱ��3
//    TIM2_Base_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
//    TIM2_Base_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
//    TIM2_Base_Handler.Init.Period=per;                        //�Զ�װ��ֵ
//    TIM2_Base_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
//    HAL_TIM_Base_Init(&TIM2_Base_Handler);
//    
//    HAL_TIM_Base_Start_IT(&TIM2_Base_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE   
//}


////ͨ�ö�ʱ��3�жϳ�ʼ��
////per���Զ���װֵ��
////psc��ʱ��Ԥ��Ƶ��
////��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
////Ft=��ʱ������Ƶ��,��λ:Mhz
////����ʹ�õ��Ƕ�ʱ��3!(��ʱ��3����APB1�ϣ�ʱ��ΪHCLK/2)
////Ŀ�ģ������ADC����ź���һ���Ķ�ʱ����ʾ����
//void TIM3_Init(u16 per,u16 psc)
//{  
//    TIM3_Base_Handler.Instance=TIM3;                          //ͨ�ö�ʱ��3
//    TIM3_Base_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
//    TIM3_Base_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
//    TIM3_Base_Handler.Init.Period=per;                        //�Զ�װ��ֵ
//    TIM3_Base_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
//    HAL_TIM_Base_Init(&TIM3_Base_Handler);
//    
//    HAL_TIM_Base_Start_IT(&TIM3_Base_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE   
//}

////ͨ�ö�ʱ��4�жϳ�ʼ��
////per���Զ���װֵ��
////psc��ʱ��Ԥ��Ƶ��
////��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
////Ft=��ʱ������Ƶ��,��λ:Mhz
////����ʹ�õ��Ƕ�ʱ��3!(��ʱ��3����APB1�ϣ�ʱ��ΪHCLK/2)
////Ŀ�ģ���������wifi�����ʱ��Ķ�ʱ
//void TIM4_Init(u16 per,u16 psc)
//{  
//    TIM4_Base_Handler.Instance=TIM4;                          //ͨ�ö�ʱ��3
//    TIM4_Base_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
//    TIM4_Base_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
//    TIM4_Base_Handler.Init.Period=per;                        //�Զ�װ��ֵ
//    TIM4_Base_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
//    HAL_TIM_Base_Init(&TIM4_Base_Handler);
//    
//    HAL_TIM_Base_Start_IT(&TIM4_Base_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE   
//}


//��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
//�˺����ᱻHAL_TIM_Base_Init()��������
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();            //ʹ��TIM3ʱ��
		HAL_NVIC_SetPriority(TIM2_IRQn,2,1);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM2_IRQn);          //����ITM3�ж�   
	}
  if(htim->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();            //ʹ��TIM3ʱ��
		HAL_NVIC_SetPriority(TIM3_IRQn,2,1);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM3_IRQn);          //����ITM3�ж�   
	}
	if(htim->Instance==TIM4)
	{
		__HAL_RCC_TIM4_CLK_ENABLE();            //ʹ��TIM3ʱ��
		HAL_NVIC_SetPriority(TIM4_IRQn,1,0);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM4_IRQn);          //����ITM3�ж�   
	}
}


//��ʱ��2�жϷ�����
void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM2_Base_Handler);
}
//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM3_Base_Handler);
}
//��ʱ��4�жϷ�����
void TIM4_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM4_Base_Handler);
}


//�ص���������ʱ���жϷ���������
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	 if(htim==(&TIM2_Base_Handler))
	 {
		if((__HAL_TIM_GET_IT_SOURCE(&TIM2_Base_Handler,TIM_IT_UPDATE)!=RESET)){  //���TIM_IT_Update��λ����ʾTIM2����жϣ�����if	
			TempHumi_State();
//			TIM_ClearITPendingBit(TIM2, TIM_IT_Update);     //���TIM2����жϱ�־ 
      __HAL_TIM_CLEAR_FLAG(&TIM2_Base_Handler, TIM_IT_UPDATE);			
		}
	 }	

	 
	 if(htim==(&TIM3_Base_Handler))
	 {
//		if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){   //���TIM_IT_Update��λ����ʾTIM3����жϣ�����if	
		if((__HAL_TIM_GET_IT_SOURCE(&TIM3_Base_Handler,TIM_IT_UPDATE)!=RESET)){
			switch(Ping_flag){                               //�ж�Ping_flag��״̬
			case 0:										 //���Ping_flag����0����ʾ����״̬������Ping����  
					MQTT_PingREQ(); 					 //���Ping���ĵ����ͻ�����  
					break;
			case 1:										 //���Ping_flag����1��˵����һ�η��͵���ping���ģ�û���յ��������ظ�������1û�б����Ϊ0�������������쳣������Ҫ��������pingģʽ
//					TIM3_ENABLE_2S(); 					 //���ǽ���ʱ��6����Ϊ2s��ʱ,���ٷ���Ping����
			    TIM3_Init(20000-1,9000-1);
					MQTT_PingREQ();  					 //���Ping���ĵ����ͻ�����  
					break;
			case 2:										 //���Ping_flag����2��˵����û���յ��������ظ�
			case 3:				                         //���Ping_flag����3��˵����û���յ��������ظ�
			case 4:				                         //���Ping_flag����4��˵����û���յ��������ظ�	
					MQTT_PingREQ();  					 //���Ping���ĵ����ͻ����� 
					break;
			case 5:										 //���Ping_flag����5��˵�����Ƿ����˶��ping�����޻ظ���Ӧ�������������⣬������������
					Connect_flag = 0;                    //����״̬��0����ʾ�Ͽ���û���Ϸ�����
//					TIM_Cmd(TIM3,DISABLE);               //��TIM3 
          __HAL_TIM_DISABLE(&TIM3_Base_Handler);			
					break;			
		}
		Ping_flag++;           		             		 //Ping_flag����1����ʾ�ַ�����һ��ping���ڴ��������Ļظ�
		__HAL_TIM_CLEAR_FLAG(&TIM3_Base_Handler, TIM_IT_UPDATE);      //���TIM3����жϱ�־ 	
	  }
	 } 
	 
	 
	 if(htim==(&TIM4_Base_Handler))
	 {
//		if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){                //���TIM_IT_Update��λ����ʾTIM4����жϣ�����if	
		if((__HAL_TIM_GET_IT_SOURCE(&TIM4_Base_Handler,TIM_IT_UPDATE)!=RESET)){	
			memcpy(&MQTT_RxDataInPtr[2],Usart3_RxBuff,Usart3_RxCounter);  //�������ݵ����ջ�����
			MQTT_RxDataInPtr[0] = Usart3_RxCounter/256;                   //��¼���ݳ��ȸ��ֽ�
			MQTT_RxDataInPtr[1] = Usart3_RxCounter%256;                   //��¼���ݳ��ȵ��ֽ�
			MQTT_RxDataInPtr+=BUFF_UNIT;                                  //ָ������
			if(MQTT_RxDataInPtr==MQTT_RxDataEndPtr)                       //���ָ�뵽������β����
				MQTT_RxDataInPtr = MQTT_RxDataBuf[0];                       //ָ���λ����������ͷ
			Usart3_RxCounter = 0;                                         //����2������������������
//			TIM_SetCounter(TIM3, 0);                                    //���㶨ʱ��6�����������¼�ʱping������ʱ��
			__HAL_TIM_SET_COUNTER(&TIM3_Base_Handler,0);
//			TIM_Cmd(TIM4, DISABLE);                        				  //�ر�TIM4��ʱ��
			__HAL_TIM_DISABLE(&TIM4_Base_Handler);
//			TIM_SetCounter(TIM4, 0);                        			  //���㶨ʱ��4������
			__HAL_TIM_SET_COUNTER(&TIM4_Base_Handler,0);
			__HAL_TIM_CLEAR_FLAG(&TIM4_Base_Handler, TIM_IT_UPDATE);     			  //���TIM4����жϱ�־ 	
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

