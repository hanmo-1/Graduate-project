//-----------------------------------------------------------------
// 程序描述: 串口函数
//		在Keil uVision4开发平台下基于STM32的程序模板
// 作    者: 凌智电子
// 开始日期: 2018-04-09
// 完成日期: 2014-04-24
// 修改日期:
// 版    本: V1.0
//   - V1.0: 初步模板
// 调试工具: 凌智STM32+FPGA电子系统设计开发板、LZE_ST LINK2、2.8寸液晶屏
// 说    明:
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include <stm32f10x.h>
// #include <stdio.h>
#include <stdarg.h>
#include "Delay.h"
#include "USART.h"


//-----------------------------------------------------------------
// void USART_Configuration(void) 
//-----------------------------------------------------------------
// 
// 函数功能: 串口配置
// 入口参数: 无 
// 返 回 值: 无
// 全局变量: 无
// 调用模块: 无
// 注意事项: 无
//-----------------------------------------------------------------
void USART_Configuration(void)               //LED引脚配置
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	//使能IO口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);
// 	// 使能AFIO功能的时钟
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;                  // TX引脚
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;            // 复用推挽输出	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;                 // RX引脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;      // 浮空输入
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//使能串口
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
// USART1模式配置	
	USART_InitStructure.USART_BaudRate = 115200;                         // 波特率设置为115200；
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;          // 8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;               // 1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;                  // 禁止校验控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;      // 模式为发送与接收
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;       // 禁止CTS,RTS硬件流控制
	USART_Init(USART1, &USART_InitStructure);
	
// 	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                       //中断接收配置使能
	USART_Cmd(USART1, ENABLE);                                           //使能串口1
// 	USART_ClearFlag(USART1, USART_FLAG_TC);                              //清除串口发送标志位	
}

//-----------------------------------------------------------------
// int fputc(int ch, FILE *f)
//-----------------------------------------------------------------
// 
// 函数功能: 重定向c库函数printf 到 USART1
// 入口参数: 无
// 返 回 值: 无
// 全局变量: 无
// 注意事项: 是被 printf 函数调用，使用时必须将Options -> Taget中的Use MicroLIB 打钩
//-----------------------------------------------------------------
int fputc(int ch, FILE *f)
{
/* 将Printf内容发往串口 */
  USART_SendData(USART1, (unsigned char) ch);
  while (!(USART1->SR & USART_FLAG_TXE));
 
  return (ch);
}

//-----------------------------------------------------------------
// static char *itoa(int value, char *string, int radix)
//-----------------------------------------------------------------
// 函数功能: 将整型数据转换成字符串
// 入口参数:  value -> 要转换的整型数
//            string -> 转换后的字符串
//            radix = 10 -> 表示十进制 
// 返 回 值: 无
// 全局变量: 无
// 调用模块: 无
// 注意事项: 无
//-----------------------------------------------------------------
static char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;                //字符串的首地址

    /* This implementation only works for decimal numbers. */
    if (radix != 10)                    
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */

//-----------------------------------------------------------------
// void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...)
//-----------------------------------------------------------------
// 函数功能: 格式化输出，类似C库的printf，但这里没有用到C库
// 入口参数:  USARTx -> 串口通道
//            Data -> 发送到串口内容的指针
//            ... -> 其他参数
// 返 回 值: 无
// 全局变量: 无
// 调用模块: 无
// 注意事项: 无
//-----------------------------------------------------------------
void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...)
{
	const char *s;
  int d;   
  char buf[16];

  va_list ap;
  va_start(ap, Data);

	while ( *Data != 0)     // 判断是否到达字符串结束符
	{				                          
		if ( *Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //回车符
					USART_SendData(USARTx, 0x0d);
					Data ++;
					break;

				case 'n':							          //换行符
					USART_SendData(USARTx, 0x0a);	
					Data ++;
					break;
				
				default:
					Data ++;
				    break;
			}			 
		}
		else if ( *Data == '%')
		{									  //
			switch ( *++Data )
			{				
				case 's':										  //字符串
					s = va_arg(ap, const char *);
          for ( ; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
          }
					Data++;
          break;

        case 'd':										//十进制
          d = va_arg(ap, int);
          itoa(d, buf, 10);
          for (s = buf; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
          }
					Data++;
          break;
				 default:
						Data++;
				    break;
			}		 
		} /* end of else if */
		else USART_SendData(USARTx, *Data++);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
