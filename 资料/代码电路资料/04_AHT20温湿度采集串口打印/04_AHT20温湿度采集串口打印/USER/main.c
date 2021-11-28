#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	 
#include "math.h"			
#include "stdio.h"
#include "stm32f10x_flash.h"
#include "stdlib.h"
#include "string.h"
#include "wdg.h"
#include "adc.h"
#include "AHT20.h"
 int main(void)
 {	
	  float adcvalue;
	  uint8_t ret = 0;
	  uint32_t CT_data[2];
	  int  c1,t1;
    delay_init();	    	 //延时函数初始化	  
    NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    LED_Init();		  		//初始化与LED连接的硬件接口
		ADC_Config();
	  uart_init(115200);
	  I2C_Bus_Init();
	  ret = AHT20_Init();
    if(ret == 0)
    {
			    while(1)
					{     
						printf("AHT20传感器初始化错误\n");
					  delay_ms(300);
					}

 
    }
   while(1)
    {  
       /* 读取 AHT20 传感器数据*/
        while(AHT20_Read_Cal_Enable() == 0)
        {
            AHT20_Init();//如果为0再使能一次
            delay_ms(30);
        }
        AHT20_Read_CTdata(CT_data);  //读取温度和湿度
        c1 = CT_data[0] * 1000 / 1024 / 1024;  //计算得到湿度值（放大了10倍,如果c1=523，表示现在湿度为52.3%）
        t1 = CT_data[1] * 200 *10 / 1024 / 1024 - 500;//计算得到温度值（放大了10倍，如果t1=245，表示现在温度为24.5℃）

        /* 读取 BMP280 传感器数据*/
       // BMP280GetData(&P, &T, &ALT);

        printf("***************************\n");
        printf("AHT20温湿度传感器测试数据:\n");
        printf("温度: %d.%d ℃\n",(t1/10),(t1%10));
        printf("湿度: %d.%d %%\n",(c1/10),(c1%10));
        printf("\n");
      delay_ms(300);
			LED1=!LED1;
    }	 
}






