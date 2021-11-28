#include "usart.h"
#include "mqtt.h"
#include "time.h"
#include "string.h"
#include "stdarg.h"




UART_HandleTypeDef UART1_Handler,UART2_Handler,UART3_Handler; //UART句柄
#define RXBUFFERSIZE   1 //缓存大小
u8 aRxBuffer[RXBUFFERSIZE];//HAL库使用的串口接收缓冲

#if  USART3_RX_ENABLE                   //如果使能接收功能
char Usart3_RxCompleted = 0;            //定义一个变量 0：表示接收未完成 1：表示接收完成 
unsigned int Usart3_RxCounter = 0;      //定义一个变量，记录串口2总共接收了多少字节的数据
char Usart3_RxBuff[USART3_RXBUFF_SIZE]; //定义一个数组，用于保存串口2接收到的数据   	
#endif


//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕,用于串口1的打印  
	USART1->DR = (u8)ch;
//  while((USART3->SR&0X40)==0);//循环发送,直到发送完毕，用于串口3的打印  
//	USART3->DR = (u8)ch;	
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

void USART3_Init(u32 bound)
{	
	//UART 初始化设置
	UART3_Handler.Instance=USART3;					    //USART3
	UART3_Handler.Init.BaudRate=bound;				    //波特率
	UART3_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	UART3_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	UART3_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	UART3_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	UART3_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&UART3_Handler);					    //HAL_UART_Init()会使能UART1
	
	//不使用HAL库中的HAL_UART_Receive_IT函数，使能接收中断函数
	__HAL_UART_ENABLE_IT(&UART3_Handler,UART_IT_RXNE);
	
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
	if(huart->Instance==USART3)//如果是串口3，进行串口3 MSP初始化
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();			//使能GPIOA时钟
		__HAL_RCC_USART3_CLK_ENABLE();			//使能USART1时钟
	
		GPIO_InitStructure.Pin=GPIO_PIN_10|GPIO_PIN_11;			//PB9
		GPIO_InitStructure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_InitStructure.Pull=GPIO_PULLUP;			//上拉
		GPIO_InitStructure.Speed=GPIO_SPEED_FAST;		//高速
		GPIO_InitStructure.Alternate=GPIO_AF7_USART3;	//复用为USART1
		HAL_GPIO_Init(GPIOB,&GPIO_InitStructure);	   	//初始化PA9
		
		HAL_NVIC_EnableIRQ(USART3_IRQn);		//使能USART1中断通道
		HAL_NVIC_SetPriority(USART3_IRQn,3,2);	//抢占优先级3，子优先级3

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
//	if(huart->Instance==USART3)//如果是串口3
//	{
//		HAL_UART_Transmit(&UART3_Handler,(u8 *)aRxBuffer,RXBUFFERSIZE,1000);	//发送接收到的数据
//		while(__HAL_UART_GET_FLAG(&UART3_Handler,UART_FLAG_TC)!=SET);
//		__HAL_UART_CLEAR_FLAG(&UART3_Handler,UART_FLAG_TC);
//	}
	if(huart->Instance==USART2)//如果是串口2
	{
		HAL_UART_Transmit(&UART2_Handler,(u8 *)aRxBuffer,RXBUFFERSIZE,1000);	//发送接收到的数据
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

void USART3_IRQHandler(void)                	
{ 
	
	if((__HAL_UART_GET_IT_SOURCE(&UART3_Handler,UART_IT_RXNE)!=RESET))   //如果USART_IT_RXNE标志位，表示数据收到了
		if(Connect_flag==0){                                //如果Connect_flag等于0，当前还没有连接服务器，处于指令配置状态
			if(USART3->DR){                                 //处于指令配置状态时，非零值才保存到缓冲区	
				Usart3_RxBuff[Usart3_RxCounter]=USART3->DR; //保存到缓冲区	
				Usart3_RxCounter ++;                        //每接收1个字节的数据，Usart2_RxCounter加1，表示接收的数据总量+1 
			}		
		}else{		                                        //反之Connect_flag等于1，连接上服务器了	
			Usart3_RxBuff[Usart3_RxCounter] = USART3->DR;   //把接收到的数据保存到Usart2_RxBuff中				
			if(Usart3_RxCounter == 0){    					//如果Usart2_RxCounter等于0，表示是接收的第1个数据，进入if分支				
//				TIM_Cmd(TIM4,ENABLE);
        __HAL_TIM_ENABLE(&TIM4_Base_Handler);				
			}else{                        					//else分支，表示果Usart2_RxCounter不等于0，不是接收的第一个数据
//				TIM_SetCounter(TIM4,0);
        __HAL_TIM_SET_COUNTER(&TIM4_Base_Handler,0);				
			}	
			Usart3_RxCounter ++;         				    //每接收1个字节的数据，Usart2_RxCounter加1，表示接收的数据总量+1 
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



/*-------------------------------------------------*/
/*函数名：串口2 printf函数                         */
/*参  数：char* fmt,...  格式化输出字符串和参数    */
/*返回值：无                                       */
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
/*函数名：串口3发送缓冲区中的数据                  */
/*参  数：data：数据                               */
/*返回值：无                                       */
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
