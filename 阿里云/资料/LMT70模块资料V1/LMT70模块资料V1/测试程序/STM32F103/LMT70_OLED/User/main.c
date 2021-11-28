//-----------------------------------------------------------------
// ��������: 
//		STM32�ڲ�ADC��lmt70�����³���
// ��    ��: ���ǵ���
// ��ʼ����: 2020-09-08
// �������: 2020-09-08
// �޸�����:
// ��    ��: V1.0
// ���Թ���: ����STM32���Ŀ����塢0.96OLED��ʾ��LZE_ST_LINK2��lmt70,�����¶ȼ�
// ˵    ��:
//					 (1) STM32���İ��LMT70ģ�������
//					 STM32���İ�		LMT70ģ��
//						PA1						TAO
//						3V3						VCC
//						GND						GND
//					 (2) STM32���İ��OLEDģ�������
//					 STM32���İ�		OLEDģ��
//						PD4						SCL
//						PD5						SDA
//						5V						VCC
//						GND						GND
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include <stm32f10x.h>
#include "lmt70_ad.h"
#include "oled.h"
#include "delay.h"
#include <stdio.h> 
//-----------------------------------------------------------------
// ������
//-----------------------------------------------------------------
int main(void)
{
	 u8 buf[8];
	 temp=receive=0;
	 OLED_Init();               
	 OLED_Display_On();         
	 OLED_Clear();             
	 ADC_RegularChannel_Configuration(ADC1);  //��ʼ��
	 Delay_5ms(5); 
	while(1)
	{
    //�����¶�
    caculation();     
		Delay_5ms(25);
    //��ʾ��ѹ
		OLED_ShowString(0,0,"ADC mV:");
		sprintf((char *)buf,"%.3f",receive);  
		OLED_ShowString(0,2,(uint8_t *)buf);
    //��ʾ�¶�
    OLED_ShowString(0,4,"temperature:");
		sprintf((char *)buf,"%.3f",temp);    
  	OLED_ShowString(0,6,(uint8_t *)buf);
	}
}

//-----------------------------------------------------------------
// End Of File
//-------------------
