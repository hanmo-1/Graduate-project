#include "stm32f10x.h"
#include "I2C.h"
#include "RCC.h"

void RCC_Configuration(void);
void delay_us(uint32_t time);
volatile int RH=0,TH=0;
uint32_t CT_data[2];
//****************************************************************************************
int main(void)
{ 
	RCC_Configuration();//48MHZ主频
	GPIO_config();
  JH_Init();//AHT10初始化
  while (1)
  {	
// 		while(JH_Read_Cal_Enable()==0) //等待状态Bit[3]=1才能读数据
// 		{
// 			JH_Init();    //AHT10初始化
// 			delay_ms(100);
// // 		}
//     JH_Read_CTdata(CT_data);//读取温度和湿度数据?
// 	  RH=CT_data[0]*1000/1024/1024;//计算得到的湿度值   放大了10倍
// 	  TH=CT_data[1]*200*10/1024/1024-500;//计算得到的温度值  放大了10倍
// 		delay_ms(500); //间隔2S读一次
   }
}
//***********************************************
void delay_us(uint32_t time)
{    
 time=time*48;
 while(time)
  time--;
}


