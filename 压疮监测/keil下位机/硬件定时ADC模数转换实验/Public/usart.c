#include "usart.h"




UART_HandleTypeDef UART1_Handler; //UART句柄
#define RXBUFFERSIZE   1 //缓存大小
u8 aRxBuffer[RXBUFFERSIZE];//HAL库使用的串口接收缓冲


//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8)ch;      
	return ch;
}


//初始化IO 串口1 
//bound:波特率
void USART1_Init(u32 bound)
{	
	//UART 初始化设置
	UART1_Handler.Instance=USART1;					    //USART1
	UART1_Handler.Init.BaudRate=bound;				    //波特率
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&UART1_Handler);					    //HAL_UART_Init()会使能UART1
	
	//不使用HAL库中的HAL_UART_Receive_IT函数，使能接收中断函数
	__HAL_UART_ENABLE_IT(&UART1_Handler,UART_IT_RXNE);
	
//	HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
  
}

//UART底层初始化，时钟使能，引脚配置，中断配置
//此函数会被HAL_UART_Init()调用
//huart:串口句柄

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if(huart->Instance==USART1)//如果是串口1，进行串口1 MSP初始化
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOA时钟
		__HAL_RCC_USART1_CLK_ENABLE();			//使能USART1时钟
	
		GPIO_InitStructure.Pin=GPIO_PIN_9|GPIO_PIN_10;			//PA9
		GPIO_InitStructure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_InitStructure.Pull=GPIO_PULLUP;			//上拉
		GPIO_InitStructure.Speed=GPIO_SPEED_FAST;		//高速
		GPIO_InitStructure.Alternate=GPIO_AF7_USART1;	//复用为USART1
		HAL_GPIO_Init(GPIOA,&GPIO_InitStructure);	   	//初始化PA9
		
		HAL_NVIC_EnableIRQ(USART1_IRQn);		//使能USART1中断通道
		HAL_NVIC_SetPriority(USART1_IRQn,3,3);	//抢占优先级3，子优先级3

	}

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	
	if(huart->Instance==USART1)//如果是串口1
	{
		HAL_UART_Transmit(&UART1_Handler,(u8 *)aRxBuffer,RXBUFFERSIZE,1000);	//发送接收到的数据
		while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);
		__HAL_UART_CLEAR_FLAG(&UART1_Handler,UART_FLAG_TC);
	
	}
}
 

//串口1中断服务程序
void USART1_IRQHandler(void)                	
{ 
	u32 timeout=0;
	u32 maxDelay=0x1FFFF;

	
	HAL_UART_IRQHandler(&UART1_Handler);	//调用HAL库中断处理公用函数
	
	timeout=0;
    while (HAL_UART_GetState(&UART1_Handler) != HAL_UART_STATE_READY)//等待就绪
	{
		timeout++;////超时处理
		if(timeout>maxDelay) break;		
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
		timeout++; //超时处理
		if(timeout>maxDelay) break;	
	}
}  

/*下面代码我们直接把中断控制逻辑写在中断服务函数内部。
 说明：采用HAL库处理逻辑，效率不高。*/
/*
void USART1_IRQHandler(void)                	
{ 
	u8 Res;
	
	if((__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_RXNE)!=RESET))
	{
		HAL_UART_Receive(&UART1_Handler,&Res,1,1000); 
		HAL_UART_Transmit(&UART1_Handler,&Res,1,1000);	//发送接收到的数据
		while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);
		__HAL_UART_CLEAR_FLAG(&UART1_Handler,UART_FLAG_TC);
	}
}
*/
