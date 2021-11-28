//-----------------------------------------------------------------
// ��������:
// ����LMT70���³���
// ��������: ���ǵ���
// ��ʼ����: 2020-09-05
// �������: 2020-09-09
// �޸�����: 2020-09-09
// �桡����: V1.0
// ��- V1.0:  
// ���Թ���: ����STM32���Ŀ����塢2.8��Һ������LZE_ST_LINK2��ADS1256ģ��
// ˵������: 
//       (1) ADS1256ģ���STM32���İ������
//							ADS1256ģ��    	STM32���İ�   
//     					 GND   -------  GND       
//     					 DRDY  ------>  PC9       
//     					 CS    <------  PC11    
//     					 DIN   <------  PC10     
//     					 DOUT  ------>  PC12      
//    					 SCLK  <------  PC8     
//     					 GND   -------  GND           
//     					 RST   <------  PC13
//							 VCC	 -------  5V
//			 (1) ADS1256ģ���LMT70ģ�������
//					 		ADS1256ģ��		LMT70ģ��
//							IN0						TAO
//							VCC						VCC
//							GND						GND

//-----------------------------------------------------------------

#include "stm32f10x.h"
#include "Delay.h"
#include "PeripheralInit.h"
#include "ads1256.h"
#include "lze_lcd.h"
#include "stdio.h"
#include "USART.h"
//-----------------------------------------------------------------
// ȫ�ֱ���
//-----------------------------------------------------------------
// ���ճɹ���־
extern u8 Resive_Flag;
// ���ջ���
// extern uint8_t USB_Rx_Buffer[VIRTUAL_COM_PORT_DATA_SIZE];

u8 IO_TAG=1;

//-----------------------------------------------------------------
// void USB_Send_string(u8* data_buffer)
//-----------------------------------------------------------------
//
// ��������:	USB�����ַ���
// ��ڲ���:  �����ַ����׵�ַ
// �� �� ֵ:  ��
// ע������:
//-----------------------------------------------------------------

// void USB_Send_string(u8* data_buffer)
// {  
//   // �����û����ݵ��˵�1�ķ��ͻ�����
//   UserToPMABufferCopy(data_buffer, ENDP1_TXADDR, 39); 
// 	// ���ö˵�1�������ݳ���
//   SetEPTxCount(ENDP1, 39); 
// 	// ���ö˵�1�ķ���״̬ΪValid
//   SetEPTxValid(ENDP1);		
// }

int main(void)
{
	u8 dis_buf[40];									// ��ʾ������
	int32_t adc[8];									// �������
	int32_t adc_1[8];								//
	float volt[8];									// ʵ�ʵ�ѹֵ
	float T;
//	uint8_t i;
// uint8_t ch_num=8;									//ͨ����
	
	PeripheralInit();								// ���ֳ�ʼ��
	LCD_Clear(LCD_COLOR_BLACK); 		// ���ñ���ɫ
	
	ADS1256_CfgADC(PGA_1, DATARATE_100);	//����ADC������ ����1:1, ����������� 15Hz 
	
	ADS1256_StartScan(0);	//�����ж�ɨ��ģʽ. 0��ʾ����8·��1��ʾ���4·

	LCD_WriteString(10,10,LCD_COLOR_YELLOW,LCD_COLOR_BLACK,(uint8_t *)"STM32 LMT70 Temperature Test:");
	LCD_WriteString(10,50,LCD_COLOR_BLUE,LCD_COLOR_BLACK,(uint8_t *)"ADC Ch0 Voltage:          mV");
	LCD_WriteString(10,90,LCD_COLOR_BLUE,LCD_COLOR_BLACK,(uint8_t *)"    Temperature:          C");

		
	while(1)
	{
//		for (i = 0; i < ch_num; i++)
//		{
//			adc[i] = (int32_t)ADS1256_GetAdc(i);		                 //��ȫ�ֻ�������ȡ��������� ������������жϷ�������ж�ȡ��
//			adc_1[i] = (adc[i] & 0x7FFFFFFF);				                 //4194303 = 2.5V , ��������ֵ��ʵ�ʿ��Ը���2.5V��׼��ʵ��ֵ���й�ʽ���� 

//			if(adc[i]&0x80000000)
//				volt[i] =((((float)adc_1[i] * 5000) / 8388608))-5000;	//����ʵ�ʵ�ѹֵ�����ƹ���ģ�������׼ȷ�������У׼ 
//			else
//				volt[i] =((((float)adc_1[i] * 5000) / 8388608)-0.15);			//����ʵ�ʵ�ѹֵ�����ƹ���ģ�������׼ȷ�������У׼ 
//		}
	

			adc[0] = (int32_t)ADS1256_GetAdc(0);		                 //��ȫ�ֻ�������ȡ��������� ������������жϷ�������ж�ȡ��
			adc_1[0] = (adc[0] & 0x7FFFFFFF);				                 //4194303 = 2.5V , ��������ֵ��ʵ�ʿ��Ը���2.5V��׼��ʵ��ֵ���й�ʽ���� 

			if(adc[0]&0x80000000)
				volt[0] =((((float)adc_1[0] * 5000) / 8388608))-5000;	//����ʵ�ʵ�ѹֵ�����ƹ���ģ�������׼ȷ�������У׼ 
			else
				volt[0] =((((float)adc_1[0] * 5000) / 8388608)-0.15);			//����ʵ�ʵ�ѹֵ�����ƹ���ģ�������׼ȷ�������У׼ 

		
		LCD_WriteString(190,140,LCD_COLOR_BLUE,LCD_COLOR_BLACK,(u8 *)"         ");
		LCD_WriteString(170,140,LCD_COLOR_BLUE,LCD_COLOR_BLACK,(u8 *)"         ");

		sprintf((char*)dis_buf,"%.3f", (float)volt[0]);		//Һ����ʾ
		LCD_WriteString(155,50,LCD_COLOR_RED,LCD_COLOR_BLACK,dis_buf );   
		T=(-0.193)*volt[0]+212.009;
		sprintf((char*)dis_buf,"%.2f", (float)T);
		LCD_WriteString(155,90,LCD_COLOR_RED,LCD_COLOR_BLACK,dis_buf );   
		Delay_5ms(60);
	}
}


//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
