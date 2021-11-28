#include "usart.h"




UART_HandleTypeDef UART1_Handler; //UART���
#define RXBUFFERSIZE   1 //�����С
u8 aRxBuffer[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���


//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8)ch;      
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

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	
	if(huart->Instance==USART1)//����Ǵ���1
	{
		HAL_UART_Transmit(&UART1_Handler,(u8 *)aRxBuffer,RXBUFFERSIZE,1000);	//���ͽ��յ�������
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
