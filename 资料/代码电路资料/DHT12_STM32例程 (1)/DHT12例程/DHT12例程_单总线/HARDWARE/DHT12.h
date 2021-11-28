#ifndef _DHT12_H_
#define _DHT12_H_
#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "stdio.h"

//IO��������
#define SDA_IN()  {GPIOB->CRH&=0XF0FFFFFF;GPIOB->CRH|=0x08000000;}//����������
#define SDA_OUT() {GPIOB->CRH&=0XF0FFFFFF;GPIOB->CRH|=0x03000000;}//ͨ���������


//IO��������
#define IIC_SCL    PBout(13) //SCL���	
#define SEND_SDA   PBout(14) //SDA���	 
#define READ_SDA   PBin(14)  //SDA���� 



void DHT12_Init(void);//��ʼ��SDA(PB14),CLK(PB13)
u8 DHT12_Rdata(void);
u8 DHT12_READ(void);




#endif	//_DHT12_H_
