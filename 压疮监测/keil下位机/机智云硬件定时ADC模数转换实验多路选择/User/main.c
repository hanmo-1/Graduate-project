/*******************************************************************************
*                 
*                 		       ADC采集压力和温度和湿度
--------------------------------------------------------------------------------
* 实 验 名		 : ADC模数转换
* 实验说明       : 
* 连接方式       : 
* 注    意		 : ADC驱动在adc.c内
*******************************************************************************/

#include "system.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "usart3.h"
#include "adc.h"
#include "time.h"
#include "mux.h"
#include "gizwits_product.h" 


/*******************************************************************************
* 函 数 名         : main
* 函数功能		   : 主函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

/* 用户区当前设备状态结构体*/
//dataPoint_t currentDataPoint;  //在product.h中用extern进行声明了


//WIFI连接状态
//wifi_sta 0: 断开
//         1: 已连接
u8 wifi_sta=0;





//Gizwits协议初始化
void Gizwits_Init(void)
{
	TIM3_Init(10-1,9000-1); //1MS系统定时 
	uart3_init(9600);//WIFI初始化 
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//设备状态结构体初始化
	gizwitsInit();//缓冲区初始化
	
}

//数据采集
void userHandle(void)
{
	if(wifi_sta==1)
	{
		for(int i=0; i<143; i++)
				{
					if(i<=127)
					{
						selectMuxPin(i);  // 选择一个多路管脚
						currentDataPoint.valuePressure_signal[i] = Get_Adc(ADC_CHANNEL_0);  // 存储adc的值在value中
					}
					if((127<i)&&(i<=135))
					{
						selectMuxPin(i);  // 选择一个多路管脚
						currentDataPoint.valueTemperature_signal[i] = Get_Adc(ADC_CHANNEL_0);  // 存储adc的值在value中
					}
					else
					{
						selectMuxPin(i);  // 选择一个多路管脚
						currentDataPoint.valueHumidity[i] = Get_Adc(ADC_CHANNEL_0);  // 存储adc的值在value中
					}
				}
	}
}


int main()
{
	u8 i=0;
	u16 value0=0,value1=0;
	float vol0,vol1;
	int key;
	
	HAL_Init();                     //初始化HAL库 
	SystemClock_Init(25,360,2,8);   //设置时钟,180Mhz
	delay_init(180);
	USART1_Init(115200);
	MUX_Init();
	LED_Init();                     //初始化LED 
	
  ADCx_Init();
	TIM3_Init(1000-1,9000-1);   //这里是定时20ms也就是90Mhz 9000分频，然后自动装载值为200（减1是因为分频系数是从1不是0开始，自动装载值也是从0开始
	Gizwits_Init();         //协议初始化
	printf("--------机智云IOT-温度采集实验----------\r\n");
	printf("KEY1:AirLink接入模式\t KEY_UP:复位\r\n\r\n");                      
    while(1)
    {
      	userHandle();//用户采集
         
        gizwitsHandle((dataPoint_t *)&currentDataPoint);//协议处理
 		
	    key = KEY_Scan(0);
		if(key==KEY1_PRES)//KEY1按键
		{
			printf("WIFI进入AirLink连接模式\r\n");
			gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-link模式接入
		}			
		if(key==WKUP_PRES)//KEY_UP按键
		{  
			printf("WIFI复位，请重新配置连接\r\n");
			gizwitsSetMode(WIFI_RESET_MODE);//WIFI复位
		}
		delay_ms(200);
		LED1=!LED1;
    }
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

