#include "usart.h"
#include "mqtt.h"
#include "time.h"
#include "string.h"
#include "stdarg.h"




UART_HandleTypeDef UART1_Handler,UART2_Handler,UART3_Handler; //UART���
#define RXBUFFERSIZE   1 //�����С
u8 aRxBuffer[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���

#if  USART3_RX_ENABLE                   //���ʹ�ܽ��չ���
char Usart3_RxCompleted = 0;            //����һ������ 0����ʾ����δ��� 1����ʾ������� 
unsigned int Usart3_RxCounter = 0;      //����һ����������¼����2�ܹ������˶����ֽڵ�����
char Usart3_RxBuff[USART3_RXBUFF_SIZE]; //����һ�����飬���ڱ��洮��2���յ�������   	
#endif


//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������,���ڴ���1�Ĵ�ӡ  
	USART1->DR = (u8)ch;
//  while((USART3->SR&0X40)==0);//ѭ������,ֱ��������ϣ����ڴ���3�Ĵ�ӡ  
//	USART3->DR = (u8)ch;	
	return ch;
}


//��ʼ��IO ����1 
//bound:������
void USART1_Init(u32 bound)
{	
	//UART ��ʼ������
	UART1_Handler.Instance=USART1;					    //USART1
	UART1_Handler.Init.BaudRate=bound;				    //������
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART1_Handler);					    //HAL_UART_Init()��ʹ��UART1
	
	//��ʹ��HAL���е�HAL_UART_Receive_IT������ʹ�ܽ����жϺ���
	__HAL_UART_ENABLE_IT(&UART1_Handler,UART_IT_RXNE);
	
//	HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
  
}

void USART3_Init(u32 bound)
{	
	//UART ��ʼ������
	UART3_Handler.Instance=USART3;					    //USART3
	UART3_Handler.Init.BaudRate=bound;				    //������
	UART3_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART3_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART3_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART3_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART3_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART3_Handler);					    //HAL_UART_Init()��ʹ��UART1
	
	//��ʹ��HAL���е�HAL_UART_Receive_IT������ʹ�ܽ����жϺ���
	__HAL_UART_ENABLE_IT(&UART3_Handler,UART_IT_RXNE);
	
//	HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
  
}

//UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж�����
//�˺����ᱻHAL_UART_Init()����
//huart:���ھ��

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if(huart->Instance==USART1)//����Ǵ���1�����д���1 MSP��ʼ��
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_USART1_CLK_ENABLE();			//ʹ��USART1ʱ��
	
		GPIO_InitStructure.Pin=GPIO_PIN_9|GPIO_PIN_10;			//PA9
		GPIO_InitStructure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_InitStructure.Pull=GPIO_PULLUP;			//����
		GPIO_InitStructure.Speed=GPIO_SPEED_FAST;		//����
		GPIO_InitStructure.Alternate=GPIO_AF7_USART1;	//����ΪUSART1
		HAL_GPIO_Init(GPIOA,&GPIO_InitStructure);	   	//��ʼ��PA9
		
		HAL_NVIC_EnableIRQ(USART1_IRQn);		//ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(USART1_IRQn,3,3);	//��ռ���ȼ�3�������ȼ�3

	}
	if(huart->Instance==USART3)//����Ǵ���3�����д���3 MSP��ʼ��
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_USART3_CLK_ENABLE();			//ʹ��USART1ʱ��
	
		GPIO_InitStructure.Pin=GPIO_PIN_10|GPIO_PIN_11;			//PB9
		GPIO_InitStructure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_InitStructure.Pull=GPIO_PULLUP;			//����
		GPIO_InitStructure.Speed=GPIO_SPEED_FAST;		//����
		GPIO_InitStructure.Alternate=GPIO_AF7_USART3;	//����ΪUSART1
		HAL_GPIO_Init(GPIOB,&GPIO_InitStructure);	   	//��ʼ��PA9
		
		HAL_NVIC_EnableIRQ(USART3_IRQn);		//ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(USART3_IRQn,3,2);	//��ռ���ȼ�3�������ȼ�3

	}

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	
	if(huart->Instance==USART1)//����Ǵ���1
	{
		HAL_UART_Transmit(&UART1_Handler,(u8 *)aRxBuffer,RXBUFFERSIZE,1000);	//���ͽ��յ�������
		while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);
		__HAL_UART_CLEAR_FLAG(&UART1_Handler,UART_FLAG_TC);
	
	}
//	if(huart->Instance==USART3)//����Ǵ���3
//	{
//		HAL_UART_Transmit(&UART3_Handler,(u8 *)aRxBuffer,RXBUFFERSIZE,1000);	//���ͽ��յ�������
//		while(__HAL_UART_GET_FLAG(&UART3_Handler,UART_FLAG_TC)!=SET);
//		__HAL_UART_CLEAR_FLAG(&UART3_Handler,UART_FLAG_TC);
//	}
	if(huart->Instance==USART2)//����Ǵ���2
	{
		HAL_UART_Transmit(&UART2_Handler,(u8 *)aRxBuffer,RXBUFFERSIZE,1000);	//���ͽ��յ�������
		while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);
		__HAL_UART_CLEAR_FLAG(&UART1_Handler,UART_FLAG_TC);
	}
}
 

//����1�жϷ������
void USART1_IRQHandler(void)                	
{ 
	u32 timeout=0;
	u32 maxDelay=0x1FFFF;

	
	HAL_UART_IRQHandler(&UART1_Handler);	//����HAL���жϴ����ú���
	
	timeout=0;
    while (HAL_UART_GetState(&UART1_Handler) != HAL_UART_STATE_READY)//�ȴ�����
	{
		timeout++;////��ʱ����
		if(timeout>maxDelay) break;		
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
		timeout++; //��ʱ����
		if(timeout>maxDelay) break;	
	}
}

void USART3_IRQHandler(void)                	
{ 
	
	if((__HAL_UART_GET_IT_SOURCE(&UART3_Handler,UART_IT_RXNE)!=RESET))   //���USART_IT_RXNE��־λ����ʾ�����յ���
		if(Connect_flag==0){                                //���Connect_flag����0����ǰ��û�����ӷ�����������ָ������״̬
			if(USART3->DR){                                 //����ָ������״̬ʱ������ֵ�ű��浽������	
				Usart3_RxBuff[Usart3_RxCounter]=USART3->DR; //���浽������	
				Usart3_RxCounter ++;                        //ÿ����1���ֽڵ����ݣ�Usart2_RxCounter��1����ʾ���յ���������+1 
			}		
		}else{		                                        //��֮Connect_flag����1�������Ϸ�������	
			Usart3_RxBuff[Usart3_RxCounter] = USART3->DR;   //�ѽ��յ������ݱ��浽Usart2_RxBuff��				
			if(Usart3_RxCounter == 0){    					//���Usart2_RxCounter����0����ʾ�ǽ��յĵ�1�����ݣ�����if��֧				
//				TIM_Cmd(TIM4,ENABLE);
        __HAL_TIM_ENABLE(&TIM4_Base_Handler);				
			}else{                        					//else��֧����ʾ��Usart2_RxCounter������0�����ǽ��յĵ�һ������
//				TIM_SetCounter(TIM4,0);
        __HAL_TIM_SET_COUNTER(&TIM4_Base_Handler,0);				
			}	
			Usart3_RxCounter ++;         				    //ÿ����1���ֽڵ����ݣ�Usart2_RxCounter��1����ʾ���յ���������+1 
		}
}

/*�����������ֱ�Ӱ��жϿ����߼�д���жϷ������ڲ���
 ˵��������HAL�⴦���߼���Ч�ʲ��ߡ�*/
/*
void USART1_IRQHandler(void)                	
{ 
	u8 Res;
	
	if((__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_RXNE)!=RESET))
	{
		HAL_UART_Receive(&UART1_Handler,&Res,1,1000); 
		HAL_UART_Transmit(&UART1_Handler,&Res,1,1000);	//���ͽ��յ�������
		while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);
		__HAL_UART_CLEAR_FLAG(&UART1_Handler,UART_FLAG_TC);
	}
}
*/



/*-------------------------------------------------*/
/*������������2 printf����                         */
/*��  ����char* fmt,...  ��ʽ������ַ����Ͳ���    */
/*����ֵ����                                       */
/*-------------------------------------------------*/

__align(8) char USART3_TxBuff[USART3_TXBUFF_SIZE];  

void u3_printf(char* fmt,...) 
{  
	unsigned int i,length;
	
	va_list ap;
	va_start(ap,fmt);
	vsprintf(USART3_TxBuff,fmt,ap);
	va_end(ap);	
	
	length=strlen((const char*)USART3_TxBuff);		
	while((USART3->SR&0X40)==0);
	for(i = 0;i < length;i ++)
	{			
		USART3->DR = USART3_TxBuff[i];
		while((USART3->SR&0X40)==0);	
	}	
}


/*-------------------------------------------------*/
/*������������3���ͻ������е�����                  */
/*��  ����data������                               */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void u3_TxData(unsigned char *data)
{
	int	i;	
	while((USART3->SR&0X40)==0);
	for(i = 1;i <= (data[0]*256+data[1]);i ++){			
		USART3->DR = data[i+1];
		while((USART3->SR&0X40)==0);	
	}
}
