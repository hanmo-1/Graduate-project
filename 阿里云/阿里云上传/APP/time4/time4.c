/*���ڴ���3�������ݵĶ�ʱ ��ʱ30ms
�����ڻ����������ݺ󣬶�ʱ��30ms����30ms���У��㹻�����ݽ��յ����Ӷ��ڶ�ʱ�ж��н����ݿ��������ͻ�������
*/



#include "time4.h"
#include "time3.h"        //������Ҫ��ͷ�ļ�
#include "led.h"
#include "adc.h"
#include "mqtt.h"          //������Ҫ��ͷ�ļ�
#include "wifi.h"	       //������Ҫ��ͷ�ļ�
#include "usart.h"
#include "main.h"
#include "string.h"



TIM_HandleTypeDef TIM4_Base_Handler;      //��ʱ����� 




//ͨ�ö�ʱ��4�жϳ�ʼ��
//per���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!(��ʱ��3����APB1�ϣ�ʱ��ΪHCLK/2)
//Ŀ�ģ���������wifi�����ʱ��Ķ�ʱ
void TIM4_Init(u16 per,u16 psc)
{  
    TIM4_Base_Handler.Instance=TIM4;                          //ͨ�ö�ʱ��3
    TIM4_Base_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    TIM4_Base_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM4_Base_Handler.Init.Period=per;                        //�Զ�װ��ֵ
    TIM4_Base_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&TIM4_Base_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM4_Base_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE
    __HAL_TIM_DISABLE(&TIM4_Base_Handler);	
}




