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
	RCC_Configuration();//48MHZ��Ƶ
	GPIO_config();
  JH_Init();//AHT10��ʼ��
  while (1)
  {	
// 		while(JH_Read_Cal_Enable()==0) //�ȴ�״̬Bit[3]=1���ܶ�����
// 		{
// 			JH_Init();    //AHT10��ʼ��
// 			delay_ms(100);
// // 		}
//     JH_Read_CTdata(CT_data);//��ȡ�¶Ⱥ�ʪ�����ݣ
// 	  RH=CT_data[0]*1000/1024/1024;//����õ���ʪ��ֵ   �Ŵ���10��
// 	  TH=CT_data[1]*200*10/1024/1024-500;//����õ����¶�ֵ  �Ŵ���10��
// 		delay_ms(500); //���2S��һ��
   }
}
//***********************************************
void delay_us(uint32_t time)
{    
 time=time*48;
 while(time)
  time--;
}


