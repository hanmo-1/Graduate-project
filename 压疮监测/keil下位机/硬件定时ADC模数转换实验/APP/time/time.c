#include "time.h"
#include "led.h"
#include "adc.h"



TIM_HandleTypeDef TIM3_Base_Handler;      //定时器句柄 


//通用定时器3中断初始化
//per：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!(定时器3挂在APB1上，时钟为HCLK/2)
void TIM3_Init(u16 per,u16 psc)
{  
    TIM3_Base_Handler.Instance=TIM3;                          //通用定时器3
    TIM3_Base_Handler.Init.Prescaler=psc;                     //分频系数
    TIM3_Base_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM3_Base_Handler.Init.Period=per;                        //自动装载值
    TIM3_Base_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM3_Base_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM3_Base_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE   
}


//定时器底层驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();            //使能TIM3时钟
		HAL_NVIC_SetPriority(TIM3_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM3_IRQn);          //开启ITM3中断   
	}
}


//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM3_Base_Handler);
}


//回调函数，定时器中断服务函数调用
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	u16 value0,value1,value2,value3,value4,value5,value6,value7,value8,value9,value10,value11,value12,value13,value14,value15;
	float vol0,vol1,vol2,vol3,vol4,vol5,vol6,vol7,vol8,vol9,vol10,vol11,vol12,vol13,vol14,vol15;
    if(htim==(&TIM3_Base_Handler))
    {
//        LED2=!LED2;        //LED1反转
		value0 = Get_Adc(ADC_CHANNEL_0);
		value1 = Get_Adc(ADC_CHANNEL_1);
		value2 = Get_Adc(ADC_CHANNEL_2);
		value3 = Get_Adc(ADC_CHANNEL_3);
		value4 = Get_Adc(ADC_CHANNEL_4);
		value5 = Get_Adc(ADC_CHANNEL_5);
		value6 = Get_Adc(ADC_CHANNEL_6);
		value7 = Get_Adc(ADC_CHANNEL_7);
		value8 = Get_Adc(ADC_CHANNEL_8);
		value9 = Get_Adc(ADC_CHANNEL_9);
		value10 = Get_Adc(ADC_CHANNEL_10);
		value11 = Get_Adc(ADC_CHANNEL_11);
		value12 = Get_Adc(ADC_CHANNEL_12);
		value13 = Get_Adc(ADC_CHANNEL_13);
		value14 = Get_Adc(ADC_CHANNEL_14);
		value15 = Get_Adc(ADC_CHANNEL_15);
		vol0=(float)value0*(3.3/4096);
		vol1=(float)value1*(3.3/4096);
		vol2=(float)value2*(3.3/4096);
		vol3=(float)value3*(3.3/4096);
		vol4=(float)value4*(3.3/4096);
		vol5=(float)value5*(3.3/4096);
		vol6=(float)value6*(3.3/4096);
		vol7=(float)value7*(3.3/4096);
		vol8=(float)value8*(3.3/4096);
		vol9=(float)value9*(3.3/4096);
		vol10=(float)value10*(3.3/4096);
		vol11=(float)value11*(3.3/4096);
		vol12=(float)value12*(3.3/4096);
		vol13=(float)value13*(3.3/4096);
		vol14=(float)value14*(3.3/4096);
		vol15=(float)value15*(3.3/4096);
		
		printf("%.2f%.2f%.2f%.2f%.2f%.2f%.2f%.2f%.2f%.2f%.2f%.2f%.2f%.2f%.2f%.2f",vol0,vol1,vol2,vol3,vol4,vol5,vol6,vol7,vol8,vol9,vol10,vol11,vol12,vol13,vol14,vol15);
		
    }
}

