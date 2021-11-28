//-----------------------------------------------------------------
// ��������:
// ����LMT70���³���
// ��������: ���ǵ���
// ��ʼ����: 2020-09-05
// �������: 2020-09-09
// �޸�����: 2020-09-09
// �桡����: V1.0
// ��- V1.0:  
// ���Թ���: ����STM32���Ŀ����塢2.8��Һ������LZE_ST_LINK2
// ˵������: 
//					 STM32���İ��LMT70ģ�������
//					 STM32���İ�		LMT70ģ��
//						PC4						TAO
//						3V3						VCC
//						GND						GND
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include <stm32f10x.h>
#include "lze_lcd.h"
#include "Delay.h"
#include "ADCx_RegularChannel.h"
#include "PeripheralInit.h"

//-----------------------------------------------------------------
// ������
//-----------------------------------------------------------------
int main(void)
{ 
	u16 buf[8];
	u8 i;
	float T;
	float a=-1.809628;                     //���״��ݺ������ò���
	float b=-3.325395;
	float c=-1.814103;
	float d=2.055894;


  PeripheralInit();                      // �����ʼ��
  LCD_Clear(LCD_COLOR_BLACK);            // ����
  // ��ʾ��ʾ��
  LCD_WriteString ( 10,   0,   LCD_COLOR_YELLOW, LCD_COLOR_BLACK, (uint8_t *)"STM32 LMT70 Temperature"  );
  LCD_WriteString ( 10,  20,   LCD_COLOR_BLUE, LCD_COLOR_BLACK, (uint8_t *)"ADC Ch0  Voltage:         mV"   );
  LCD_WriteString ( 45,  40,   LCD_COLOR_BLUE, LCD_COLOR_BLACK, (uint8_t *)"Temperature:          'C"   );     
  while (1)
  {
     u16 temp = 0;
     u32 sum = 0;
		 u16 MAX = 0;
     u16 MIN = 0xffff;
		 float Covert=0;
		for(i=0;i<100;i++)                    // ȡƽ��ֵ
		{
			temp=AnalogVoltage(0);
			sum+=temp;
	    if( temp > MAX ) MAX = temp;
      if( temp < MIN ) MIN = temp;
			Delay_250us(1);
		}
	  sum -= MAX ; 												 // ȥ�����ֵ
    sum -= MIN ; 												 // ȥ����Сֵ
	  Covert=(float)sum/98.0;
    T=a*Covert*Covert*Covert/1000000000+b*Covert*Covert/1000000+c*Covert/10+d*100;// �¶ȵ����׺���ת����ʽ
		sprintf((char *)buf,"%.3f", Covert);	//��ѹ��ʾ
		LCD_WriteString(150, 20, LCD_COLOR_RED, LCD_COLOR_BLACK, (uint8_t *)buf);
		sprintf((char *)buf,"%.3f",T);	     	// �¶���ʾ
		LCD_WriteString(150, 40, LCD_COLOR_RED, LCD_COLOR_BLACK, (uint8_t *)buf); 
    Delay_50ms(4);
  }
}



//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
