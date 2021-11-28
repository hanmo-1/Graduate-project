#ifndef _usart_H
#define _usart_H


#include "system.h"
#include "stdio.h"
#include "mqtt.h"

#define USART3_RX_ENABLE     1      //�Ƿ������չ���  1������  0���ر�
#define USART3_TXBUFF_SIZE   1024   //���崮��2 ���ͻ�������С 1024�ֽ�

#if  USART3_RX_ENABLE                          //���ʹ�ܽ��չ���
#define USART3_RXBUFF_SIZE   1024              //���崮��2 ���ջ�������С 1024�ֽ�
extern char Usart3_RxCompleted ;               //�ⲿ�����������ļ����Ե��øñ���
extern unsigned int Usart3_RxCounter;          //�ⲿ�����������ļ����Ե��øñ���
extern char Usart3_RxBuff[USART3_RXBUFF_SIZE]; //�ⲿ�����������ļ����Ե��øñ���
#endif


void USART1_Init(u32 bound);
void USART3_Init(u32 bound);
void u3_printf(char*,...) ; 
void u3_TxData(unsigned char *data);


#endif
