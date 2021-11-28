/*******************************************************************************
*                 
*                 		       ADC采集压力和温度和湿度
--------------------------------------------------------------------------------
* 实 验 名		 : ADC模数转换实验
* 实验说明       : 
* 连接方式       : 
* 注    意		 : ADC驱动在adc.c内
*******************************************************************************/

#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "adc.h"
#include "time.h"
#include "mux.h"
#include "key.h"
#include "wifi.h"	      //包含需要的头文件
#include "mqtt.h"       //包含需要的头文件
#include "string.h"       //包含需要的头文件	
#include "main.h"
#include "dht11.h"
#include "myiic.h"
#include "time2.h"
#include "time3.h"
#include "time4.h"

#include "AHT20.h"
//#include "temhum.h"




extern TIM_HandleTypeDef TIM2_Base_Handler,TIM3_Base_Handler,TIM4_Base_Handler;      //定时器句柄
extern ADC_HandleTypeDef ADC1_Handler;//ADC句柄

struct m_AHT20 AHT20;


/*******************************************************************************
* 函 数 名         : main
* 函数功能		   : 主函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
int main()
{
	int i;
	
	
	HAL_Init();                     //初始化HAL库 
	SystemClock_Init(25,360,2,8);   //设置时钟,180Mhz
	SysTick_Init(180);
	MUX_Init();	                    //初始化8路管脚
	KEY_Init(); //按键初始化
	USART1_Init(115200);     //用于打印数据
	USART3_Init(115200);     // 用于传输数据
  LED_Init();
  ADCx_Init();                    //ADC初始化
	TIM4_Init(100-1,9000-1);        //这里是定时30ms也就是90Mhz 9000分频，然后自动装载值为200（减1是因为分频系数是从1不是0开始，自动装载值也是从0开始
	WiFi_ResetIO_Init();            //初始化WiFi的复位IO
	MQTT_Buff_Init();               //初始化接收,发送,命令数据的 缓冲区 以及各状态参数
	AliIoT_Parameter_Init();	      //初始化连接阿里云IoT平台MQTT服务器的参数


	IIC_Init();							//IIC管脚初始化
	AHT20.alive=!AHT20_Init();	        //AHT20温湿度传感器初始化
//  temphum_init();     //ATH20初始化 

	

	
	
	
//	TempHumi_State();             //这里是为了调试ADC有没问题
	while(1)                        //主循环
	{		
		/*--------------------------------------------------------------------*/
		/*   Connect_flag=1同服务器建立了连接,我们可以发布数据和接收推送了    */
		/*--------------------------------------------------------------------*/
		if(Connect_flag==1){     
			/*-------------------------------------------------------------*/
			/*                     处理发送缓冲区数据                      */
			/*-------------------------------------------------------------*/
				if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr){                //if成立的话，说明发送缓冲区有数据了
				//3种情况可进入if
				//第1种：0x10 连接报文
				//第2种：0x82 订阅报文，且ConnectPack_flag置位，表示连接报文成功
				//第3种：SubcribePack_flag置位，说明连接和订阅均成功，其他报文可发
				if((MQTT_TxDataOutPtr[2]==0x10)||((MQTT_TxDataOutPtr[2]==0x82)&&(ConnectPack_flag==1))||(SubcribePack_flag==1)){    
					printf("发送数据:0x%x\r\n",MQTT_TxDataOutPtr[2]);     //串口提示信息
					MQTT_TxData(MQTT_TxDataOutPtr);                       //发送数据
					MQTT_TxDataOutPtr += BUFF_UNIT;                       //指针下移
					if(MQTT_TxDataOutPtr==MQTT_TxDataEndPtr)              //如果指针到缓冲区尾部了
						MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];            //指针归位到缓冲区开头
				} 				
			}//处理发送缓冲区数据的else if分支结尾
			
			/*-------------------------------------------------------------*/
			/*                     处理接收缓冲区数据                      */
			/*-------------------------------------------------------------*/
			if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr){  //if成立的话，说明接收缓冲区有数据了														
				printf("接收到数据:");
				/*-----------------------------------------------------*/
				/*                    处理CONNACK报文                  */
				/*-----------------------------------------------------*/				
				//if判断，如果第一个字节是0x20，表示收到的是CONNACK报文
				//接着我们要判断第4个字节，看看CONNECT报文是否成功
				if(MQTT_RxDataOutPtr[2]==0x20){             			
				    switch(MQTT_RxDataOutPtr[5]){					
						case 0x00 : printf("CONNECT报文成功\r\n");                            //串口输出信息	
								    ConnectPack_flag = 1;                                        //CONNECT报文成功，订阅报文可发
									break;                                                       //跳出分支case 0x00                                              
						case 0x01 : printf("连接已拒绝，不支持的协议版本，准备重启\r\n");     //串口输出信息
									Connect_flag = 0;                                            //Connect_flag置零，重启连接
									break;                                                       //跳出分支case 0x01   
						case 0x02 : printf("连接已拒绝，不合格的客户端标识符，准备重启\r\n"); //串口输出信息
									Connect_flag = 0;                                            //Connect_flag置零，重启连接
									break;                                                       //跳出分支case 0x02 
						case 0x03 : printf("连接已拒绝，服务端不可用，准备重启\r\n");         //串口输出信息
									Connect_flag = 0;                                            //Connect_flag置零，重启连接
									break;                                                       //跳出分支case 0x03
						case 0x04 : printf("连接已拒绝，无效的用户名或密码，准备重启\r\n");   //串口输出信息
									Connect_flag = 0;                                            //Connect_flag置零，重启连接						
									break;                                                       //跳出分支case 0x04
						case 0x05 : printf("连接已拒绝，未授权，准备重启\r\n");               //串口输出信息
									Connect_flag = 0;                                            //Connect_flag置零，重启连接						
									break;                                                       //跳出分支case 0x05 		
						default   : printf("连接已拒绝，未知状态，准备重启\r\n");             //串口输出信息 
									Connect_flag = 0;                                            //Connect_flag置零，重启连接					
									break;                                                       //跳出分支case default 								
					}				
				}			
				//if判断，第一个字节是0x90，表示收到的是SUBACK报文
				//接着我们要判断订阅回复，看看是不是成功
				else if(MQTT_RxDataOutPtr[2]==0x90){ 
						switch(MQTT_RxDataOutPtr[6]){					
						case 0x00 :
						case 0x01 : printf("订阅成功\r\n");            //串口输出信息
							        SubcribePack_flag = 1;                //SubcribePack_flag置1，表示订阅报文成功，其他报文可发送
											Ping_flag = 0;						//Ping_flag清零
											TIM3_Init(60000-1,45000-1);                    //启动30s的PING定时器
//   									TIM3_ENABLE_30S();                    //启动30s的PING定时器
//										TIM2_ENABLE_30S();                    //启动30s的上传数据的定时器
											TIM2_Init(5000-1,45000-1);                 //启动30s的上传数据的定时器
						          TempHumi_State();                     //先发一次数据
									break;                                //跳出分支                                             
						default   : printf("订阅失败，准备重启\r\n");  //串口输出信息 
									Connect_flag = 0;                     //Connect_flag置零，重启连接
									break;                                //跳出分支 								
					}					
				}
				//if判断，第一个字节是0xD0，表示收到的是PINGRESP报文
				else if(MQTT_RxDataOutPtr[2]==0xD0){ 
					printf("PING报文回复\r\n"); 		  //串口输出信息 
					if(Ping_flag==1){                     //如果Ping_flag=1，表示第一次发送
						 Ping_flag = 0;    				  //要清除Ping_flag标志
					}else if(Ping_flag>1){ 				  //如果Ping_flag>1，表示是多次发送了，而且是2s间隔的快速发送
						Ping_flag = 0;     				  //要清除Ping_flag标志
						TIM3_Init(60000-1,45000-1); 				  //PING定时器重回30s的时间
					}				
				}	
				//if判断，如果第一个字节是0x30，表示收到的是服务器发来的推送数据
				//我们要提取控制命令
				else if((MQTT_RxDataOutPtr[2]==0x30)){ 
					printf("服务器等级0推送\r\n"); 		   //串口输出信息 
					MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);  //处理等级0推送数据
				}				
								
				MQTT_RxDataOutPtr += BUFF_UNIT;                     //指针下移
				if(MQTT_RxDataOutPtr==MQTT_RxDataEndPtr)            //如果指针到缓冲区尾部了
					MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];          //指针归位到缓冲区开头                        
			}//处理接收缓冲区数据的else if分支结尾
			
			/*-------------------------------------------------------------*/
			/*                     处理命令缓冲区数据                      */
			/*-------------------------------------------------------------*/
			if(MQTT_CMDOutPtr != MQTT_CMDInPtr){                             //if成立的话，说明命令缓冲区有数据了			       
				printf("命令:%s\r\n",&MQTT_CMDOutPtr[2]);                 //串口输出信息
				
				MQTT_CMDOutPtr += BUFF_UNIT;                             	 //指针下移
				if(MQTT_CMDOutPtr==MQTT_CMDEndPtr)           	             //如果指针到缓冲区尾部了
					MQTT_CMDOutPtr = MQTT_CMDBuf[0];          	             //指针归位到缓冲区开头				
			}//处理命令缓冲区数据的else if分支结尾	
		}//Connect_flag=1的if分支的结尾
		
		/*--------------------------------------------------------------------*/
		/*      Connect_flag=0同服务器断开了连接,我们要重启连接服务器         */
		/*--------------------------------------------------------------------*/
		else{ 
			printf("需要连接服务器\r\n");                 //串口输出信息
//			TIM_Cmd(TIM4,DISABLE);                           //关闭TIM4
			__HAL_TIM_DISABLE(&TIM4_Base_Handler);			
//			TIM_Cmd(TIM3,DISABLE);                           //关闭TIM3 
			__HAL_TIM_DISABLE(&TIM3_Base_Handler);
			WiFi_RxCounter=0;                                //WiFi接收数据量变量清零                        
			memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);          //清空WiFi接收缓冲区 
			if(WiFi_Connect_IoTServer()==0){   			     //如果WiFi连接云服务器函数返回0，表示正确，进入if
				printf("建立TCP连接成功\r\n");            //串口输出信息
				Connect_flag = 1;                            //Connect_flag置1，表示连接成功	
				WiFi_RxCounter=0;                            //WiFi接收数据量变量清零                        
				memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);      //清空WiFi接收缓冲区 
				MQTT_Buff_ReInit();                          //重新初始化发送缓冲区                    
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
/*函数名：采集温湿度，并发布给服务器               */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TempHumi_State(void)
{
	u32 CT_data[2]={0};
  volatile float  hum=0,tem=0; 
	u16 adcx;
	float adcx_buffer[8];
	float T[8];    	//温度参数
//	char humidity=0.0;           //定义一个变量，保存湿度值
//	float val;        //定义一个变量，保存温度值
	char temp[256];                  
	
//	for(int i=0; i<8; i++)
//				{
//					
//						selectMuxPin(i);  // 选择一个多路管脚		
//						adcx_buffer[i]= Get_ADC_Value(ADC_CHANNEL_5,200);  // 存储adc的值在value中
//						printf ("Voltage=%3.3fmV",adcx_buffer[i]*(33000/4096));	// 串口发送相关信息	
////						selectMuxPin(i);  // 选择一个多路管脚
////						adcx_buffer[i]= Get_ADC_Value(ADC_CHANNEL_5,200);  // 存储adc的值在value中
//////					HAL_ADC_Stop(&ADC1_Handler);                     // 这里可以不用停止
////				 		adcx_buffer[i]=(float)adcx_buffer[i]*(3.3000/4096);      		// 获取计算后的带小数的实际电压值
//////						T[i]=(-0.193)*adcx_buffer[i]+212.009;								// 计算温度值
////						printf ("Voltage=%3.3fmV  Temperature=%3.3fC\r\n",T[i],adcx_buffer[i]);	// 串口发送相关信息
//				}
//	 sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"humidity\":%3.3f,\"temperature\":%3.3f},\"version\":\"1.0.0\"}",adcx_buffer[7],T[7]);  //构建回复湿度温度数据
//	 MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器	

				
						selectMuxPin(0);  // 选择一个多路管脚		
						adcx_buffer[0]= Get_ADC_Value(ADC_CHANNEL_5,200);  // 存储adc的值在value中
						printf ("Voltage=%3.3fV",adcx_buffer[0]*(3.3/4096)*10);	// 串口发送相关信息			
				
	if(AHT20.alive)// 如果AHT20传感器存在，则读取温湿度数据
		{
			//读取AHT20的 20Bit原始数据
			AHT20.flag = AHT20_ReadHT(AHT20.HT);
			//实际标准单位转换
			StandardUnitCon(&AHT20);
		}
	 sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"humidity\":%3.3f,\"temperature\":%3.3f,\"pressure\":%3.3f},\"version\":\"1.0.0\"}",AHT20.RH,AHT20.Temp,adcx_buffer[0]*(3.3/4096)*10);  //构建回复湿度温度数据
	 MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器			


	
//	DHT11_Read_Data(&T,&humidity);	               //读取温湿度值
//	  adcx=Get_ADC_Value(ADC_CHANNEL_1,100);
//		val=(float)adcx*(3300.0/4096);      		// 获取计算后的带小数的实际电压值
//		T=(-0.193)*val+212.009;								// 计算温度值
//		printf ("Voltage=%3.3fmV  Temperature=%3.3fC\r\n", val,T);	// 串口发送相关信息
//	printf("温度：%d  湿度：%d\r\n",T,humidity);  //串口输出信息
	
//	sprintf(temp,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"humidity\":%d,\"temperature\":%d},\"version\":\"1.0.0\"}",humidity,T);  //构建回复湿度温度数据
//	MQTT_PublishQs0(P_TOPIC_NAME,temp,strlen(temp));   //添加数据，发布给服务器	
}
//		
//		if(i%50==0)
//		{
//			value0=Get_ADC_Value(ADC_CHANNEL_0,20);
//			value1=Get_ADC_Value(ADC_CHANNEL_5,20);
//			printf("检测通道0AD值为：%d\r\n",value0);
//	        printf("检测通道5AD值为：%d\r\n",value1);
//			vol0=(float)value0*(3.3/4096);
////				USART1->DR = vol0;

//			
//			

//			printf("%.2f",vol0);
//			printf("%.2f\r\n",vol0);

//		}
//		delay_ms(10);

