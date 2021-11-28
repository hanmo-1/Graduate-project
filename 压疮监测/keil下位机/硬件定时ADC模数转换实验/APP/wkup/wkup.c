#include "wkup.h"


/*******************************************************************************
* 函 数 名         : Enter_Standby_Mode
* 函数功能		   : 进入待机模式
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Enter_Standby_Mode(void)
{
	__HAL_RCC_PWR_CLK_ENABLE();         //使能PWR时钟

	
	//STM32F4,当开启了RTC相关中断后,必须先关闭RTC中断,再清中断标志位,然后重新设置
	//RTC中断,再进入待机模式才可以正常唤醒,否则会有问题.	
//    __HAL_RCC_BACKUPRESET_FORCE();      //复位备份区域
//    HAL_PWR_EnableBkUpAccess();         //后备区域访问使能	
//    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
//    __HAL_RTC_WRITEPROTECTION_DISABLE(&RTC_Handler);//关闭RTC写保护
    
    //关闭RTC相关中断，可能在RTC实验打开了
//    __HAL_RTC_WAKEUPTIMER_DISABLE_IT(&RTC_Handler,RTC_IT_WUT);
//    __HAL_RTC_TIMESTAMP_DISABLE_IT(&RTC_Handler,RTC_IT_TS);
//    __HAL_RTC_ALARM_DISABLE_IT(&RTC_Handler,RTC_IT_ALRA|RTC_IT_ALRB);
    
    //清除RTC相关中断标志位
//    __HAL_RTC_ALARM_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_ALRAF|RTC_FLAG_ALRBF);
//    __HAL_RTC_TIMESTAMP_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_TSF); 
//    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_WUTF);
	
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);//清除Wake-up 标志
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);//设置WKUP用于唤醒
    HAL_PWR_EnterSTANDBYMode(); //进入待机模式 
}



