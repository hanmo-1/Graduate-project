//-----------------------------------------------------------------
// 程序描述:
// 　　LMT70测温程序
// 作　　者: 凌智电子
// 开始日期: 2020-09-09
// 完成日期: 2020-09-09
// 修改日期: 2020-09-09
// 版　　本: V1.0
// 　- V1.0:  
// 调试工具: 凌智STM32核心开发板、1602液晶屏、LZE_ST_LINK2
// 说    明:
//					 STM32核心板和LMT70模块的连接
//					 STM32核心板		LMT70模块
//						PC4						TAO
//						3V3						VCC
//						GND						GND
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include <stm32f10x.h>
#include "CharLCD.h"
#include "Delay.h"
#include "ADCx_RegularChannel.h"
#include "PeripheralInit.h"

//-----------------------------------------------------------------
// 主程序
//-----------------------------------------------------------------
int main(void)
{ 
	u16 buf[8];          
	u8 i;
	float T;
	float a=-1.809628;                    //三阶传递函数所用参数
	float b=-3.325395;
	float c=-1.814103;
	float d=2.055894;


  PeripheralInit();                     // 外设初始化
  WrCLcdC(0x01);                        // LCM清屏 
  WriteString( 1, 1, "V:          mV") ;// 显示提示符
  WriteString( 2, 1, "T:          'C");     
  while (1)
  {
     u16 temp = 0;
     u32 sum = 0;
		 u16 MAX = 0;
     u16 MIN = 0xffff;
		 float Covert=0;
	
		for(i=0;i<100;i++)                  	//取平均值
		{
			temp=AnalogVoltage(0);
			sum+=temp;
	    if( temp > MAX ) MAX = temp;
      if( temp < MIN ) MIN = temp;
			Delay_250us(1);
		}
	    sum -= MAX ; 												//去掉最大值
      sum -= MIN ; 												//去掉最小值
	    Covert=(float)sum/98.0;
    T=a*Covert*Covert*Covert/1000000000+b*Covert*Covert/1000000+c*Covert/10+d*100; // 温度的三阶函数转换公式
		sprintf((char *)buf,"%.3f", Covert); //电压显示
		WriteString(1, 4,(uint8_t *)buf);
		sprintf((char *)buf,"%.3f",T);       //温度显示
		WriteString(2, 4, (uint8_t *)buf); 
    Delay_50ms(4);                       //延时0.2s
  }
}



//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
