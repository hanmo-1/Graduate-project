#include "RCC.h"
//*******************************************************************************
void RCC_Configuration(void)
{
   RCC_DeInit(); 
	 RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_12); //4X12=48MH /* PLLCLK =4MHz * 12 = 48 MHz 设置时钟为48M*/
	 RCC_HSICmd(ENABLE);//使能内部时钟
	 while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET); /* Wait till PLL is ready 等待内部时钟工作稳定*/
	
	 RCC_PLLCmd(ENABLE);/* Enable PLL 使能PLL*/  
	 while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); /* Wait till PLL is ready 等待PLL工作稳定*/
	 RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);    /* Select PLL as system clock source 选择PLL做为系统时钟源*/    
	 while(RCC_GetSYSCLKSource() != 0x08);  /* Wait till PLL is used as system clock source 准备就绪，开始干活*/  	
	 RCC_HCLKConfig(RCC_SYSCLK_Div1);/* HCLK = SYSCLK 设置系统设置*/	 
	
   RCC_PCLK2Config(RCC_HCLK_Div1); /* PCLK2 = HCLK PCLK2时钟=主时钟*/
   RCC_PCLK1Config(RCC_HCLK_Div2);/* PCLK1 = HCLK/2 PCLK1时钟为主时钟1/2*/	
	 FLASH_SetLatency(FLASH_Latency_2);
 	 FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
}

  
