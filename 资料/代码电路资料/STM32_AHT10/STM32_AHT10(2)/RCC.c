#include "RCC.h"
//*******************************************************************************
void RCC_Configuration(void)
{
   RCC_DeInit(); 
	 RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_12); //4X12=48MH /* PLLCLK =4MHz * 12 = 48 MHz ����ʱ��Ϊ48M*/
	 RCC_HSICmd(ENABLE);//ʹ���ڲ�ʱ��
	 while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET); /* Wait till PLL is ready �ȴ��ڲ�ʱ�ӹ����ȶ�*/
	
	 RCC_PLLCmd(ENABLE);/* Enable PLL ʹ��PLL*/  
	 while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); /* Wait till PLL is ready �ȴ�PLL�����ȶ�*/
	 RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);    /* Select PLL as system clock source ѡ��PLL��Ϊϵͳʱ��Դ*/    
	 while(RCC_GetSYSCLKSource() != 0x08);  /* Wait till PLL is used as system clock source ׼����������ʼ�ɻ�*/  	
	 RCC_HCLKConfig(RCC_SYSCLK_Div1);/* HCLK = SYSCLK ����ϵͳ����*/	 
	
   RCC_PCLK2Config(RCC_HCLK_Div1); /* PCLK2 = HCLK PCLK2ʱ��=��ʱ��*/
   RCC_PCLK1Config(RCC_HCLK_Div2);/* PCLK1 = HCLK/2 PCLK1ʱ��Ϊ��ʱ��1/2*/	
	 FLASH_SetLatency(FLASH_Latency_2);
 	 FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
}

  
