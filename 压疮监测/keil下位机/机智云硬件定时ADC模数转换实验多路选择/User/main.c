/*******************************************************************************
*                 
*                 		       ADC�ɼ�ѹ�����¶Ⱥ�ʪ��
--------------------------------------------------------------------------------
* ʵ �� ��		 : ADCģ��ת��
* ʵ��˵��       : 
* ���ӷ�ʽ       : 
* ע    ��		 : ADC������adc.c��
*******************************************************************************/

#include "system.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "usart3.h"
#include "adc.h"
#include "time.h"
#include "mux.h"
#include "gizwits_product.h" 


/*******************************************************************************
* �� �� ��         : main
* ��������		   : ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/

/* �û�����ǰ�豸״̬�ṹ��*/
//dataPoint_t currentDataPoint;  //��product.h����extern����������


//WIFI����״̬
//wifi_sta 0: �Ͽ�
//         1: ������
u8 wifi_sta=0;





//GizwitsЭ���ʼ��
void Gizwits_Init(void)
{
	TIM3_Init(10-1,9000-1); //1MSϵͳ��ʱ 
	uart3_init(9600);//WIFI��ʼ�� 
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//�豸״̬�ṹ���ʼ��
	gizwitsInit();//��������ʼ��
	
}

//���ݲɼ�
void userHandle(void)
{
	if(wifi_sta==1)
	{
		for(int i=0; i<143; i++)
				{
					if(i<=127)
					{
						selectMuxPin(i);  // ѡ��һ����·�ܽ�
						currentDataPoint.valuePressure_signal[i] = Get_Adc(ADC_CHANNEL_0);  // �洢adc��ֵ��value��
					}
					if((127<i)&&(i<=135))
					{
						selectMuxPin(i);  // ѡ��һ����·�ܽ�
						currentDataPoint.valueTemperature_signal[i] = Get_Adc(ADC_CHANNEL_0);  // �洢adc��ֵ��value��
					}
					else
					{
						selectMuxPin(i);  // ѡ��һ����·�ܽ�
						currentDataPoint.valueHumidity[i] = Get_Adc(ADC_CHANNEL_0);  // �洢adc��ֵ��value��
					}
				}
	}
}


int main()
{
	u8 i=0;
	u16 value0=0,value1=0;
	float vol0,vol1;
	int key;
	
	HAL_Init();                     //��ʼ��HAL�� 
	SystemClock_Init(25,360,2,8);   //����ʱ��,180Mhz
	delay_init(180);
	USART1_Init(115200);
	MUX_Init();
	LED_Init();                     //��ʼ��LED 
	
  ADCx_Init();
	TIM3_Init(1000-1,9000-1);   //�����Ƕ�ʱ20msҲ����90Mhz 9000��Ƶ��Ȼ���Զ�װ��ֵΪ200����1����Ϊ��Ƶϵ���Ǵ�1����0��ʼ���Զ�װ��ֵҲ�Ǵ�0��ʼ
	Gizwits_Init();         //Э���ʼ��
	printf("--------������IOT-�¶Ȳɼ�ʵ��----------\r\n");
	printf("KEY1:AirLink����ģʽ\t KEY_UP:��λ\r\n\r\n");                      
    while(1)
    {
      	userHandle();//�û��ɼ�
         
        gizwitsHandle((dataPoint_t *)&currentDataPoint);//Э�鴦��
 		
	    key = KEY_Scan(0);
		if(key==KEY1_PRES)//KEY1����
		{
			printf("WIFI����AirLink����ģʽ\r\n");
			gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-linkģʽ����
		}			
		if(key==WKUP_PRES)//KEY_UP����
		{  
			printf("WIFI��λ����������������\r\n");
			gizwitsSetMode(WIFI_RESET_MODE);//WIFI��λ
		}
		delay_ms(200);
		LED1=!LED1;
    }
}
//		
//		if(i%50==0)
//		{
//			value0=Get_ADC_Value(ADC_CHANNEL_0,20);
//			value1=Get_ADC_Value(ADC_CHANNEL_5,20);
//			printf("���ͨ��0ADֵΪ��%d\r\n",value0);
//	        printf("���ͨ��5ADֵΪ��%d\r\n",value1);
//			vol0=(float)value0*(3.3/4096);
////				USART1->DR = vol0;

//			
//			

//			printf("%.2f",vol0);
//			printf("%.2f\r\n",vol0);

//		}
//		delay_ms(10);

