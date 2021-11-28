#include "mux.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F429开发板
//ADG706驱动代码	   
//华南理工大学电活性
//创建日期:2021/04/23
//版本：V1.0
//版权所有，盗版必究。
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//初始化PB1为输出.并使能时钟	    
//MUX IO初始化
/*这里PB5先去掉是因为使用的普中开发板中的PB5连接了蜂鸣器（低电平有效）*/
const int selectPins[8] = {GPIO_PIN_3, GPIO_PIN_4, /*GPIO_PIN_5*/ GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10}; //放入数组有利于进行选择通道，只放如选择的通道PD3-PD10


void MUX_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOB_CLK_ENABLE();           //开启GPIOB时钟
	  __HAL_RCC_GPIOD_CLK_ENABLE();           //开启GPIOD时钟
	
    GPIO_Initure.Pin=GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_6; //PB3-PB10, PD11用于多路选择器的使能
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_NOPULL;          //不用上下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
	  GPIO_Initure.Pin=GPIO_PIN_11; // PD11用于多路选择器的使能
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_NOPULL;          //不用上下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);


	
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_11,GPIO_PIN_RESET);	//PD11置0，默认初始化使能打开	(7HC40多路选择器默认E是低电平使能（这句代码可以不用）
		for (int i=0; i<3; i++)
			{
				HAL_GPIO_WritePin(GPIOB, selectPins[i], GPIO_PIN_SET);     //默认初始化关闭   当i=0时候蜂鸣器会叫
//				PBout(i) = GPIO_PIN_SET;
			}

//				PDout(11) = GPIO_PIN_RESET;
}


void selectMuxPin(int pin)
{
  for (int i=0; i<3; i++)
  {
    if (pin & (1<<i))
      HAL_GPIO_WritePin(GPIOB, selectPins[i], GPIO_PIN_SET);
//				PBout(i) = GPIO_PIN_SET;
    else
      HAL_GPIO_WritePin(GPIOB, selectPins[i], GPIO_PIN_RESET);
//				PBout(i) = GPIO_PIN_RESET;
  }
}