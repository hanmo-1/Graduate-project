#include "wkup.h"


/*******************************************************************************
* �� �� ��         : Enter_Standby_Mode
* ��������		   : �������ģʽ
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Enter_Standby_Mode(void)
{
	__HAL_RCC_PWR_CLK_ENABLE();         //ʹ��PWRʱ��

	
	//STM32F4,��������RTC����жϺ�,�����ȹر�RTC�ж�,�����жϱ�־λ,Ȼ����������
	//RTC�ж�,�ٽ������ģʽ�ſ�����������,�����������.	
//    __HAL_RCC_BACKUPRESET_FORCE();      //��λ��������
//    HAL_PWR_EnableBkUpAccess();         //���������ʹ��	
//    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
//    __HAL_RTC_WRITEPROTECTION_DISABLE(&RTC_Handler);//�ر�RTCд����
    
    //�ر�RTC����жϣ�������RTCʵ�����
//    __HAL_RTC_WAKEUPTIMER_DISABLE_IT(&RTC_Handler,RTC_IT_WUT);
//    __HAL_RTC_TIMESTAMP_DISABLE_IT(&RTC_Handler,RTC_IT_TS);
//    __HAL_RTC_ALARM_DISABLE_IT(&RTC_Handler,RTC_IT_ALRA|RTC_IT_ALRB);
    
    //���RTC����жϱ�־λ
//    __HAL_RTC_ALARM_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_ALRAF|RTC_FLAG_ALRBF);
//    __HAL_RTC_TIMESTAMP_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_TSF); 
//    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_WUTF);
	
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);//���Wake-up ��־
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);//����WKUP���ڻ���
    HAL_PWR_EnterSTANDBYMode(); //�������ģʽ 
}



