//-----------------------------------------------------------------
// 程序描述: 
//		STM32内部ADC，lmt70测室温程序
// 作    者: 凌智电子
// 开始日期: 2020-09-08
// 完成日期: 2020-09-08
// 修改日期:
// 版    本: V1.0
// 调试工具: 凌智STM32核心开发板、0.96OLED显示、LZE_ST_LINK2、lmt70,室温温度计
// 说    明:
//					 (1) STM32核心板和LMT70模块的连接
//					 STM32核心板		LMT70模块
//						PA1						TAO
//						3V3						VCC
//						GND						GND
//					 (2) STM32核心板和OLED模块的连接
//					 STM32核心板		OLED模块
//						PD4						SCL
//						PD5						SDA
//						5V						VCC
//						GND						GND
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include <stm32f10x.h>
#include "lmt70_ad.h"
#include "oled.h"
#include "delay.h"
#include <stdio.h> 
//-----------------------------------------------------------------
// 主函数
//-----------------------------------------------------------------
int main(void)
{
	 u8 buf[8];
	 temp=receive=0;
	 OLED_Init();               
	 OLED_Display_On();         
	 OLED_Clear();             
	 ADC_RegularChannel_Configuration(ADC1);  //初始化
	 Delay_5ms(5); 
	while(1)
	{
    //计算温度
    caculation();     
		Delay_5ms(25);
    //显示电压
		OLED_ShowString(0,0,"ADC mV:");
		sprintf((char *)buf,"%.3f",receive);  
		OLED_ShowString(0,2,(uint8_t *)buf);
    //显示温度
    OLED_ShowString(0,4,"temperature:");
		sprintf((char *)buf,"%.3f",temp);    
  	OLED_ShowString(0,6,(uint8_t *)buf);
	}
}

//-----------------------------------------------------------------
// End Of File
//-------------------
