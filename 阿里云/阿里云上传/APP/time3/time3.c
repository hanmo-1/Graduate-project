/*定时器3用于ping报文的发送，防止服务器会解除连接*/



#include "time3.h"
#include "led.h"
#include "adc.h"
#include "mqtt.h"          //包含需要的头文件
#include "wifi.h"	       //包含需要的头文件
#include "usart.h"
#include "main.h"
#include "string.h"




TIM_HandleTypeDef TIM3_Base_Handler;        //定时器句柄 




//通用定时器3中断初始化
//per：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!(定时器3挂在APB1上，时钟为HCLK/2)
//目的：给输出ADC输出信号以一定的定时，显示出来
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



