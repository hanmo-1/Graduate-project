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
	RCC_Configuration();//48MHZÖ÷Æµ
	GPIO_config();
  JH_Init();//AHT10³õÊ¼»¯
  while (1)
  {	
// 		while(JH_Read_Cal_Enable()==0) //µÈ´ı×´Ì¬Bit[3]=1²ÅÄÜ¶ÁÊı¾İ
// 		{
// 			JH_Init();    //AHT10³õÊ¼»¯
// 			delay_ms(100);
// // 		}
//     JH_Read_CTdata(CT_data);//¶ÁÈ¡ÎÂ¶ÈºÍÊª¶ÈÊı¾İ£
// 	  RH=CT_data[0]*1000/1024/1024;//¼ÆËãµÃµ½µÄÊª¶ÈÖµ   ·Å´óÁË10±¶
// 	  TH=CT_data[1]*200*10/1024/1024-500;//¼ÆËãµÃµ½µÄÎÂ¶ÈÖµ  ·Å´óÁË10±¶
// 		delay_ms(500); //¼ä¸ô2S¶ÁÒ»´Î
   }
}
//***********************************************
void delay_us(uint32_t time)
{    
 time=time*48;
 while(time)
  time--;
}


