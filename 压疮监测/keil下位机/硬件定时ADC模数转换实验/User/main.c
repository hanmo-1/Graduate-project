/*******************************************************************************
*                 
*                 		       ADC采集压力和温度和湿度
--------------------------------------------------------------------------------
* 实 验 名		 : ADC模数转换实验
* 实验说明       : 
* 连接方式       : 
* 注    意		 : ADC驱动在adc.c内
*******************************************************************************/

#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "adc.h"
#include "time.h"


/*******************************************************************************
* 函 数 名         : main
* 函数功能		   : 主函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
int main()
{
	u8 i=0;
	u16 value0=0,value1=0;
	float vol0,vol1;
	
	HAL_Init();                     //初始化HAL库 
	SystemClock_Init(25,360,2,8);   //设置时钟,180Mhz
	SysTick_Init(180);
	USART1_Init(115200);
//	LED_Init();
  ADCx_Init();
	TIM3_Init(1000-1,9000-1);   //这里是定时20ms也就是90Mhz 9000分频，然后自动装载值为200（减1是因为分频系数是从1不是0开始，自动装载值也是从0开始
	
	while(1)
	{
		i++;
		if(i%20==0)
		{
			LED1=!LED1;
		}
//		
//		if(i%50==0)
//		{
//			value0=Get_ADC_Value(ADC_CHANNEL_0,20);
//			value1=Get_ADC_Value(ADC_CHANNEL_5,20);
//			printf("检测通道0AD值为：%d\r\n",value0);
//	        printf("检测通道5AD值为：%d\r\n",value1);
//			vol0=(float)value0*(3.3/4096);
////				USART1->DR = vol0;

//			
//			

//			printf("%.2f",vol0);
//			printf("%.2f\r\n",vol0);

//		}
//		delay_ms(10);
	}
}
