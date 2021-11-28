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
    delay_init();	    	 //��ʱ������ʼ��	  
    NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
		ADC_Config();
	  uart_init(115200);
	  I2C_Bus_Init();
	  ret = AHT20_Init();
    if(ret == 0)
    {
			    while(1)
					{     
						printf("AHT20��������ʼ������\n");
					  delay_ms(300);
					}

 
    }
   while(1)
    {  
       /* ��ȡ AHT20 ����������*/
        while(AHT20_Read_Cal_Enable() == 0)
        {
            AHT20_Init();//���Ϊ0��ʹ��һ��
            delay_ms(30);
        }
        AHT20_Read_CTdata(CT_data);  //��ȡ�¶Ⱥ�ʪ��
        c1 = CT_data[0] * 1000 / 1024 / 1024;  //����õ�ʪ��ֵ���Ŵ���10��,���c1=523����ʾ����ʪ��Ϊ52.3%��
        t1 = CT_data[1] * 200 *10 / 1024 / 1024 - 500;//����õ��¶�ֵ���Ŵ���10�������t1=245����ʾ�����¶�Ϊ24.5�棩

        /* ��ȡ BMP280 ����������*/
       // BMP280GetData(&P, &T, &ALT);

        printf("***************************\n");
        printf("AHT20��ʪ�ȴ�������������:\n");
        printf("�¶�: %d.%d ��\n",(t1/10),(t1%10));
        printf("ʪ��: %d.%d %%\n",(c1/10),(c1%10));
        printf("\n");
      delay_ms(300);
			LED1=!LED1;
    }	 
}






