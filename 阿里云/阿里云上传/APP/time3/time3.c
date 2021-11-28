/*��ʱ��3����ping���ĵķ��ͣ���ֹ��������������*/



#include "time3.h"
#include "led.h"
#include "adc.h"
#include "mqtt.h"          //������Ҫ��ͷ�ļ�
#include "wifi.h"	       //������Ҫ��ͷ�ļ�
#include "usart.h"
#include "main.h"
#include "string.h"




TIM_HandleTypeDef TIM3_Base_Handler;        //��ʱ����� 




//ͨ�ö�ʱ��3�жϳ�ʼ��
//per���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!(��ʱ��3����APB1�ϣ�ʱ��ΪHCLK/2)
//Ŀ�ģ������ADC����ź���һ���Ķ�ʱ����ʾ����
void TIM3_Init(u16 per,u16 psc)
{  
    TIM3_Base_Handler.Instance=TIM3;                          //ͨ�ö�ʱ��3
    TIM3_Base_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    TIM3_Base_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM3_Base_Handler.Init.Period=per;                        //�Զ�װ��ֵ
    TIM3_Base_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&TIM3_Base_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM3_Base_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE   
}



