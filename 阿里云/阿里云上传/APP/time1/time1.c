#include "time1.h"
#include "led.h"
#include "adc.h"
#include "mqtt.h"          //包含需要的头文件
#include "wifi.h"	       //包含需要的头文件
#include "usart.h"
#include "main.h"
#include "string.h"



TIM_HandleTypeDef TIM1_Base_Handler;      //定时器句柄 

//通用定时器3中断初始化
//per：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!(定时器3挂在APB1上，时钟为HCLK/2)
//目的：给输出ADC输出信号以一定的定时，显示出来
void TIM2_Init(u16 per,u16 psc)
{  
    TIM2_Base_Handler.Instance=TIM2;                          //通用定时器3
    TIM2_Base_Handler.Init.Prescaler=psc;                     //分频系数
    TIM2_Base_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM2_Base_Handler.Init.Period=per;                        //自动装载值
    TIM2_Base_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM2_Base_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM2_Base_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE   
}





//定时器底层驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();            //使能TIM3时钟
		HAL_NVIC_SetPriority(TIM2_IRQn,1,1);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启ITM3中断   
	}
}


//定时器2中断服务函数
void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM2_Base_Handler);
}



//回调函数，定时器中断服务函数调用
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	 if(htim==(&TIM2_Base_Handler))
	 {
		if((__HAL_TIM_GET_FLAG(&TIM2_Base_Handler,TIM_IT_UPDATE)!=RESET)){  //如果TIM_IT_Update置位，表示TIM2溢出中断，进入if	
			TempHumi_State();
//			TIM_ClearITPendingBit(TIM2, TIM_IT_Update);     //清除TIM2溢出中断标志 
      __HAL_TIM_CLEAR_FLAG(&TIM2_Base_Handler, TIM_IT_UPDATE);			
		}
	 }	
	 
	
//	u16 value0,value1,value2,value3,value4,value5,value6,value7,value8,value9,value10,value11,value12,value13,value14,value15;
//	float vol0,vol1,vol2,vol3,vol4,vol5,vol6,vol7,vol8,vol9,vol10,vol11,vol12,vol13,vol14,vol15;
//    if(htim==(&TIM3_Base_Handler))
//    {
////        LED2=!LED2;        //LED1反转
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

