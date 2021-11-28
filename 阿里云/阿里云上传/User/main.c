/*******************************************************************************
*                 
*                 		       ADC�ɼ�ѹ�����¶Ⱥ�ʪ��
--------------------------------------------------------------------------------
* ʵ �� ��		 : ADCģ��ת��ʵ��
* ʵ��˵��       : 
* ���ӷ�ʽ       : 
* ע    ��		 : ADC������adc.c��
*******************************************************************************/

#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "adc.h"
#include "time.h"
#include "mux.h"
#include "key.h"
#include "wifi.h"	      //������Ҫ��ͷ�ļ�
#include "mqtt.h"       //������Ҫ��ͷ�ļ�
#include "string.h"       //������Ҫ��ͷ�ļ�	
#include "main.h"
#include "dht11.h"
#include "myiic.h"
#include "time2.h"
#include "time3.h"
#include "time4.h"

#include "AHT20.h"
//#include "temhum.h"




extern TIM_HandleTypeDef TIM2_Base_Handler,TIM3_Base_Handler,TIM4_Base_Handler;      //��ʱ�����
extern ADC_HandleTypeDef ADC1_Handler;//ADC���

struct m_AHT20 AHT20;


/*******************************************************************************
* �� �� ��         : main
* ��������		   : ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
int main()
{
	int i;
	
	
	HAL_Init();                     //��ʼ��HAL�� 
	SystemClock_Init(25,360,2,8);   //����ʱ��,180Mhz
	SysTick_Init(180);
	MUX_Init();	                    //��ʼ��8·�ܽ�
	KEY_Init(); //������ʼ��
	USART1_Init(115200);     //���ڴ�ӡ����
	USART3_Init(115200);     // ���ڴ�������
  LED_Init();
  ADCx_Init();                    //ADC��ʼ��
	TIM4_Init(100-1,9000-1);        //�����Ƕ�ʱ30msҲ����90Mhz 9000��Ƶ��Ȼ���Զ�װ��ֵΪ200����1����Ϊ��Ƶϵ���Ǵ�1����0��ʼ���Զ�װ��ֵҲ�Ǵ�0��ʼ
	WiFi_ResetIO_Init();            //��ʼ��WiFi�ĸ�λIO
	MQTT_Buff_Init();               //��ʼ������,����,�������ݵ� ������ �Լ���״̬����
	AliIoT_Parameter_Init();	      //��ʼ�����Ӱ�����IoTƽ̨MQTT�������Ĳ���


	IIC_Init();							//IIC�ܽų�ʼ��
	AHT20.alive=!AHT20_Init();	        //AHT20��ʪ�ȴ�������ʼ��
//  temphum_init();     //ATH20��ʼ�� 

	

	
	
	
//	TempHumi_State();             //������Ϊ�˵���ADC��û����
	while(1)                        //��ѭ��
	{		
		/*--------------------------------------------------------------------*/
		/*   Connect_flag=1ͬ����������������,���ǿ��Է������ݺͽ���������    */
		/*--------------------------------------------------------------------*/
		if(Connect_flag==1){     
			/*-------------------------------------------------------------*/
			/*                     �����ͻ���������                      */
			/*-------------------------------------------------------------*/
				if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr){                //if�����Ļ���˵�����ͻ�������������
				//3������ɽ���if
				//��1�֣�0x10 ���ӱ���
				//��2�֣�0x82 ���ı��ģ���ConnectPack_flag��λ����ʾ���ӱ��ĳɹ�
				//��3�֣�SubcribePack_flag��λ��˵�����ӺͶ��ľ��ɹ����������Ŀɷ�
				if((MQTT_TxDataOutPtr[2]==0x10)||((MQTT_TxDataOutPtr[2]==0x82)&&(ConnectPack_flag==1))||(SubcribePack_flag==1)){    
					printf("��������:0x%x\r\n",MQTT_TxDataOutPtr[2]);     //������ʾ��Ϣ
					MQTT_TxData(MQTT_TxDataOutPtr);                       //��������
					MQTT_TxDataOutPtr += BUFF_UNIT;                       //ָ������
					if(MQTT_TxDataOutPtr==MQTT_TxDataEndPtr)              //���ָ�뵽������β����
						MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];            //ָ���λ����������ͷ
				} 				
			}//�����ͻ��������ݵ�else if��֧��β
			
			/*-------------------------------------------------------------*/
			/*                     ������ջ���������                      */
			/*-------------------------------------------------------------*/
			if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr){  //if�����Ļ���˵�����ջ�������������														
				printf("���յ�����:");
				/*-----------------------------------------------------*/
				/*                    ����CONNACK����                  */
				/*-----------------------------------------------------*/				
				//if�жϣ������һ���ֽ���0x20����ʾ�յ�����CONNACK����
				//��������Ҫ�жϵ�4���ֽڣ�����CONNECT�����Ƿ�ɹ�
				if(MQTT_RxDataOutPtr[2]==0x20){             			
				    switch(MQTT_RxDataOutPtr[5]){					
						case 0x00 : printf("CONNECT���ĳɹ�\r\n");                            //���������Ϣ	
								    ConnectPack_flag = 1;                                        //CONNECT���ĳɹ������ı��Ŀɷ�
									break;                                                       //������֧case 0x00                                              
						case 0x01 : printf("�����Ѿܾ�����֧�ֵ�Э��汾��׼������\r\n");     //���������Ϣ
									Connect_flag = 0;                                            //Connect_flag���㣬��������
									break;                                                       //������֧case 0x01   
						case 0x02 : printf("�����Ѿܾ������ϸ�Ŀͻ��˱�ʶ����׼������\r\n"); //���������Ϣ
									Connect_flag = 0;                                            //Connect_flag���㣬��������
									break;                                                       //������֧case 0x02 
						case 0x03 : printf("�����Ѿܾ�������˲����ã�׼������\r\n");         //���������Ϣ
									Connect_flag = 0;                                            //Connect_flag���㣬��������
									break;                                                       //������֧case 0x03
						case 0x04 : printf("�����Ѿܾ�����Ч���û��������룬׼������\r\n");   //���������Ϣ
									Connect_flag = 0;                                            //Connect_flag���㣬��������						
									break;                                                       //������֧case 0x04
						case 0x05 : printf("�����Ѿܾ���δ��Ȩ��׼������\r\n");               //���������Ϣ
									Connect_flag = 0;                                            //Connect_flag���㣬��������						
									break;                                                       //������֧case 0x05 		
						default   : printf("�����Ѿܾ���δ֪״̬��׼������\r\n");             //���������Ϣ 
									Connect_flag = 0;                                            //Connect_flag���㣬��������					
									break;                                                       //������֧case default 								
					}				
				}			
				//if�жϣ���һ���ֽ���0x90����ʾ�յ�����SUBACK����
				//��������Ҫ�ж϶��Ļظ��������ǲ��ǳɹ�
				else if(MQTT_RxDataOutPtr[2]==0x90){ 
						switch(MQTT_RxDataOutPtr[6]){					
						case 0x00 :
						case 0x01 : printf("���ĳɹ�\r\n");            //���������Ϣ
							        SubcribePack_flag = 1;                //SubcribePack_flag��1����ʾ���ı��ĳɹ����������Ŀɷ���
											Ping_flag = 0;						//Ping_flag����
											TIM3_Init(60000-1,45000-1);                    //����30s��PING��ʱ��
//   									TIM3_ENABLE_30S();                    //����30s��PING��ʱ��
//										TIM2_ENABLE_30S();                    //����30s���ϴ����ݵĶ�ʱ��
											TIM2_Init(5000-1,45000-1);                 //����30s���ϴ����ݵĶ�ʱ��
						          TempHumi_State();                     //�ȷ�һ������
									break;                                //������֧                                             
						default   : printf("����ʧ�ܣ�׼������\r\n");  //���������Ϣ 
									Connect_flag = 0;                     //Connect_flag���㣬��������
									break;                                //������֧ 								
					}					
				}
				//if�жϣ���һ���ֽ���0xD0����ʾ�յ�����PINGRESP����
				else if(MQTT_RxDataOutPtr[2]==0xD0){ 
					printf("PING���Ļظ�\r\n"); 		  //���������Ϣ 
					if(Ping_flag==1){                     //���Ping_flag=1����ʾ��һ�η���
						 Ping_flag = 0;    				  //Ҫ���Ping_flag��־
					}else if(Ping_flag>1){ 				  //���Ping_flag>1����ʾ�Ƕ�η����ˣ�������2s����Ŀ��ٷ���
						Ping_flag = 0;     				  //Ҫ���Ping_flag��־
						TIM3_Init(60000-1,45000-1); 				  //PING��ʱ���ػ�30s��ʱ��
					}				
				}	
				//if�жϣ������һ���ֽ���0x30����ʾ�յ����Ƿ�������������������
				//����Ҫ��ȡ��������
				else if((MQTT_RxDataOutPtr[2]==0x30)){ 
					printf("�������ȼ�0����\r\n"); 		   //���������Ϣ 
					MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);  //����ȼ�0��������
				}				
								
				MQTT_RxDataOutPtr += BUFF_UNIT;                     //ָ������
				if(MQTT_RxDataOutPtr==MQTT_RxDataEndPtr)            //���ָ�뵽������β����
					MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];          //ָ���λ����������ͷ                        
			}//������ջ��������ݵ�else if��֧��β
			
			/*-------------------------------------------------------------*/
			/*                     ���������������                      */
			/*-------------------------------------------------------------*/
			if(MQTT_CMDOutPtr != MQTT_CMDInPtr){                             //if�����Ļ���˵�����������������			       
				printf("����:%s\r\n",&MQTT_CMDOutPtr[2]);                 //���������Ϣ
				
				MQTT_CMDOutPtr += BUFF_UNIT;                             	 //ָ������
				if(MQTT_CMDOutPtr==MQTT_CMDEndPtr)           	             //���ָ�뵽������β����
					MQTT_CMDOutPtr = MQTT_CMDBuf[0];          	             //ָ���λ����������ͷ				
			}//��������������ݵ�else if��֧��β	
		}//Connect_flag=1��if��֧�Ľ�β
		
		/*--------------------------------------------------------------------*/
		/*      Connect_flag=0ͬ�������Ͽ�������,����Ҫ�������ӷ�����         */
		/*--------------------------------------------------------------------*/
		else{ 
			printf("��Ҫ���ӷ�����\r\n");                 //���������Ϣ
//			TIM_Cmd(TIM4,DISABLE);                           //�ر�TIM4
			__HAL_TIM_DISABLE(&TIM4_Base_Handler);			
//			TIM_Cmd(TIM3,DISABLE);                           //�ر�TIM3 
			__HAL_TIM_DISABLE(&TIM3_Base_Handler);
			WiFi_RxCounter=0;                                //WiFi������������������                        
			memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);          //���WiFi���ջ����� 
			if(WiFi_Connect_IoTServer()==0){   			     //���WiFi�����Ʒ�������������0����ʾ��ȷ������if
				printf("����TCP���ӳɹ�\r\n");            //���������Ϣ
				Connect_flag = 1;                            //Connect_flag��1����ʾ���ӳɹ�	
				WiFi_RxCounter=0;                            //WiFi������������������                        
				memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);      //���WiFi���ջ����� 
				MQTT_Buff_ReInit();                          //���³�ʼ�����ͻ�����                    
			}				
		}
	}
	i++;
		if(i%20==0)
		{
			LED1=!LED1;
		}
}
/*-------------------------------------------------*/
/*���������ɼ���ʪ�ȣ���������������               */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TempHumi_State(void)
{
	u32 CT_data[2]={0};
  volatile float  hum=0,tem=0; 
	u16 adcx;
	float adcx_buffer[8];
	float T[8];    	//�¶Ȳ���
//	char humidity=0.0;           //����һ������������ʪ��ֵ
//	float val;        //����һ�������������¶�ֵ
	char temp[256];                  
	
//	for(int i=0; i<8; i++)
//				{
//					
//						selectMuxPin(i);  // ѡ��һ����·�ܽ�		
//						adcx_buffer[i]= Get_ADC_Value(ADC_CHANNEL_5,200);  // �洢adc��ֵ��value��
//						printf ("Voltage=%3.3fmV",adcx_buffer[i]*(33000/4096));	// ���ڷ��������Ϣ	
////						selectMuxPin(i);  // ѡ��һ����·�ܽ�
////						adcx_buffer[i]= Get_ADC_Value(ADC_CHANNEL_5,200);  // �洢adc��ֵ��value��
//////					HAL_ADC_Stop(&ADC1_Handler);                     // ������Բ���ֹͣ
////				 		adcx_buffer[i]=(float)adcx_buffer[i]*(3.3000/4096);      		// ��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ
//////						T[i]=(-0.193)*adcx_buffer[i]+212.009;								// �����¶�ֵ
////						printf ("Voltage=%3.3fmV  Temperature=%3.3fC\r\n",T[i],adcx_buffer[i]);	// ���ڷ��������Ϣ
//				}
//	 sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"humidity\":%3.3f,\"temperature\":%3.3f},\"version\":\"1.0.0\"}",adcx_buffer[7],T[7]);  //�����ظ�ʪ���¶�����
//	 MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //������ݣ�������������	

				
						selectMuxPin(0);  // ѡ��һ����·�ܽ�		
						adcx_buffer[0]= Get_ADC_Value(ADC_CHANNEL_5,200);  // �洢adc��ֵ��value��
						printf ("Voltage=%3.3fV",adcx_buffer[0]*(3.3/4096)*10);	// ���ڷ��������Ϣ			
				
	if(AHT20.alive)// ���AHT20���������ڣ����ȡ��ʪ������
		{
			//��ȡAHT20�� 20Bitԭʼ����
			AHT20.flag = AHT20_ReadHT(AHT20.HT);
			//ʵ�ʱ�׼��λת��
			StandardUnitCon(&AHT20);
		}
	 sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"humidity\":%3.3f,\"temperature\":%3.3f,\"pressure\":%3.3f},\"version\":\"1.0.0\"}",AHT20.RH,AHT20.Temp,adcx_buffer[0]*(3.3/4096)*10);  //�����ظ�ʪ���¶�����
	 MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //������ݣ�������������			


	
//	DHT11_Read_Data(&T,&humidity);	               //��ȡ��ʪ��ֵ
//	  adcx=Get_ADC_Value(ADC_CHANNEL_1,100);
//		val=(float)adcx*(3300.0/4096);      		// ��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ
//		T=(-0.193)*val+212.009;								// �����¶�ֵ
//		printf ("Voltage=%3.3fmV  Temperature=%3.3fC\r\n", val,T);	// ���ڷ��������Ϣ
//	printf("�¶ȣ�%d  ʪ�ȣ�%d\r\n",T,humidity);  //���������Ϣ
	
//	sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"humidity\":%d,\"temperature\":%d},\"version\":\"1.0.0\"}",humidity,T);  //�����ظ�ʪ���¶�����
//	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //������ݣ�������������	
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

