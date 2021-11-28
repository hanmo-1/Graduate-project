//-----------------------------------------------------------------
// ��������: ���ں���
//		��Keil uVision4����ƽ̨�»���STM32�ĳ���ģ��
// ��    ��: ���ǵ���
// ��ʼ����: 2018-04-09
// �������: 2014-04-24
// �޸�����:
// ��    ��: V1.0
//   - V1.0: ����ģ��
// ���Թ���: ����STM32+FPGA����ϵͳ��ƿ����塢LZE_ST LINK2��2.8��Һ����
// ˵    ��:
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ͷ�ļ�����
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
// ��������: ��������
// ��ڲ���: �� 
// �� �� ֵ: ��
// ȫ�ֱ���: ��
// ����ģ��: ��
// ע������: ��
//-----------------------------------------------------------------
void USART_Configuration(void)               //LED��������
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	//ʹ��IO��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);
// 	// ʹ��AFIO���ܵ�ʱ��
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;                  // TX����
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;            // �����������	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;                 // RX����
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;      // ��������
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//ʹ�ܴ���
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
// USART1ģʽ����	
	USART_InitStructure.USART_BaudRate = 115200;                         // ����������Ϊ115200��
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;          // 8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;               // 1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;                  // ��ֹУ�����
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;      // ģʽΪ���������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;       // ��ֹCTS,RTSӲ��������
	USART_Init(USART1, &USART_InitStructure);
	
// 	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                       //�жϽ�������ʹ��
	USART_Cmd(USART1, ENABLE);                                           //ʹ�ܴ���1
// 	USART_ClearFlag(USART1, USART_FLAG_TC);                              //������ڷ��ͱ�־λ	
}

//-----------------------------------------------------------------
// int fputc(int ch, FILE *f)
//-----------------------------------------------------------------
// 
// ��������: �ض���c�⺯��printf �� USART1
// ��ڲ���: ��
// �� �� ֵ: ��
// ȫ�ֱ���: ��
// ע������: �Ǳ� printf �������ã�ʹ��ʱ���뽫Options -> Taget�е�Use MicroLIB ��
//-----------------------------------------------------------------
int fputc(int ch, FILE *f)
{
/* ��Printf���ݷ������� */
  USART_SendData(USART1, (unsigned char) ch);
  while (!(USART1->SR & USART_FLAG_TXE));
 
  return (ch);
}

//-----------------------------------------------------------------
// static char *itoa(int value, char *string, int radix)
//-----------------------------------------------------------------
// ��������: ����������ת�����ַ���
// ��ڲ���:  value -> Ҫת����������
//            string -> ת������ַ���
//            radix = 10 -> ��ʾʮ���� 
// �� �� ֵ: ��
// ȫ�ֱ���: ��
// ����ģ��: ��
// ע������: ��
//-----------------------------------------------------------------
static char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;                //�ַ������׵�ַ

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
// ��������: ��ʽ�����������C���printf��������û���õ�C��
// ��ڲ���:  USARTx -> ����ͨ��
//            Data -> ���͵��������ݵ�ָ��
//            ... -> ��������
// �� �� ֵ: ��
// ȫ�ֱ���: ��
// ����ģ��: ��
// ע������: ��
//-----------------------------------------------------------------
void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...)
{
	const char *s;
  int d;   
  char buf[16];

  va_list ap;
  va_start(ap, Data);

	while ( *Data != 0)     // �ж��Ƿ񵽴��ַ���������
	{				                          
		if ( *Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //�س���
					USART_SendData(USARTx, 0x0d);
					Data ++;
					break;

				case 'n':							          //���з�
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
				case 's':										  //�ַ���
					s = va_arg(ap, const char *);
          for ( ; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
          }
					Data++;
          break;

        case 'd':										//ʮ����
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
