/*用于串口3接收数据的定时 定时30ms
当串口缓冲区有数据后，定时器30ms，这30ms当中，足够把数据接收到，从而在定时中断中将数据拷贝到发送缓冲区中
*/



#include "time4.h"
#include "time3.h"        //包含需要的头文件
#include "led.h"
#include "adc.h"
#include "mqtt.h"          //包含需要的头文件
#include "wifi.h"	       //包含需要的头文件
#include "usart.h"
#include "main.h"
#include "string.h"



TIM_HandleTypeDef TIM4_Base_Handler;      //定时器句柄 




//通用定时器4中断初始化
//per：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!(定时器3挂在APB1上，时钟为HCLK/2)
//目的：用来进行wifi传输的时候的定时
void TIM4_Init(u16 per,u16 psc)
{  
    TIM4_Base_Handler.Instance=TIM4;                          //通用定时器3
    TIM4_Base_Handler.Init.Prescaler=psc;                     //分频系数
    TIM4_Base_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM4_Base_Handler.Init.Period=per;                        //自动装载值
    TIM4_Base_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM4_Base_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM4_Base_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE
    __HAL_TIM_DISABLE(&TIM4_Base_Handler);	
}




