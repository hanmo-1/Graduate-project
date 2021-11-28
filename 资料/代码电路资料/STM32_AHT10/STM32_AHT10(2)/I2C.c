#include "I2C.h"
u8 Byte[6];
u8 flag_ACK[20];
u32 SO_RH=0,SO_TH=0;
uint8_t val = 0;
uint8_t Byte_first;	
extern void delay_us(uint32_t time);//������ʱ
//*************************************************************
void GPIO_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//****SCL******��©���*************************
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_PIN7_OPP(); //���ģʽ
}
//****SDA���*************************************************
void GPIO_PIN7_OPP(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
//****SDA���************************************************
void GPIO_PIN7_FLOATING(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
//*************************************************************
void I2C_DELAY(uint16_t i)  //53KHZ
{ 
 i=i*15;
 while(i)      
  i--;   
}
//*********I2C��ʼ�ź�*****************************************
void I2C_Start(void)  
{	 
	  SDA_OUT; //���ģʽ
	
	  SDA_H;
	  I2C_DELAY(8);
	  SCL_H;
 	  I2C_DELAY(8);
	  SDA_L;
    I2C_DELAY(8);
		SCL_L;
		I2C_DELAY(8);
} 
//***********I2Cֹͣ�ź�***************************
void I2C_Stop(void)
{
	  SDA_OUT; //���ģʽ
	
	  SDA_L;
	  I2C_DELAY(8);
		SCL_H;
		I2C_DELAY(8);
  	SDA_H;
	  I2C_DELAY(8);
}
//***********************************************************
uint8_t Receive_ACK(void) //�ӻ�����Ӧ��
{
	u16 CNT;
	
	SDA_IN;      //����ģʽ  
	SCL_L;
	I2C_DELAY(8);
  SCL_H;
	I2C_DELAY(8);
	while((SDA_READ)&&(CNT<100))
		CNT++;
	if(CNT>=100)  //Ӧ��ʱ
	{
		CNT=0;
		return 0;
	}
	SCL_L;
	I2C_DELAY(8);
	return 1;    //Ӧ��OK
}
//*********I2C����һ���ֽ�**************************************
void I2C_SendByte(uint8_t SendByte)   
{  
 uint8_t i = 8; 
 SDA_OUT; //���ģʽ
 while(i--)  
 {   
	SCL_L;
	I2C_DELAY(3);  
  if(SendByte & 0x80)
    SDA_H;
  else  
    SDA_L;
  SCL_H;  
  I2C_DELAY(3);  
	
	SendByte <<= 1; 
 } 
  SCL_L;
	I2C_DELAY(8);  
} 
//********I2C����һ���ֽ�**************************************
uint8_t I2C_ReceiveByte(void)    
{   
 uint8_t i = 8;  
 uint8_t ReceiveByte = 0;	
 SDA_IN;       //����ģʽ
 SCL_L;
 I2C_DELAY(8);
 while(i--)  
 {
	SCL_H;  
	I2C_DELAY(3);   
	ReceiveByte<<=1;
	if(SDA_READ)  
   {  
     ReceiveByte|=0x01;
    } 
  SCL_L;
  I2C_DELAY(3);
  }	
  return ReceiveByte;  
}

//***********************************************************************
void Send_ACK()        //�����ظ�Ӧ��ACK�ź�
{
	SDA_OUT; //���ģʽ
	SCL_L;
  I2C_DELAY(8);
	SDA_L;
	I2C_DELAY(8);
	SCL_H;
  I2C_DELAY(8);
	SCL_L;
  I2C_DELAY(8);
}
//************************************************************************
void Send_NOT_ACK(void)	//�������ظ�Ӧ��
{
	SDA_OUT; //���ģʽ
	SCL_L;
  I2C_DELAY(8);
	SDA_H;
	I2C_DELAY(8);
	SCL_H;
  I2C_DELAY(8);
	SCL_L;
  I2C_DELAY(8);
	SDA_L;
	I2C_DELAY(8);
}
//************��ѯCal ENABLE λ��û��ʹ��**********************************
u8 JH_Read_Cal_Enable(void)
{
  val = JH_Read_Status();
	 if((val & 0x68)==0x08)  //�ж�NORģʽ��У׼����Ƿ���Ч
		 return 1;
   else 
		 return 0;
 }
//************************************************************************
u8 JH_Read_Status(void)//��ȡAHT10��״̬�Ĵ���
{
	I2C_Start();
	I2C_SendByte(0x71);//����0x71
	Receive_ACK();
	Byte_first = I2C_ReceiveByte();
	Send_NOT_ACK();
	I2C_Stop();
	return Byte_first;
}
//***********************************************************************
void JH_Send_BA(void)//��AHT����BA����
{
	I2C_Start();
	I2C_SendByte(0x70);//����鷢����0x80�����д����
	Receive_ACK();
	I2C_SendByte(0xba);//���͸�λ����0XBA
	Receive_ACK();
	I2C_Stop();
}
//*********************************************************************
void JH_Read_CTdata(uint32_t *ct)//��ȡAHT10���¶Ⱥ�ʪ������
{
	uint16_t cnt = 0;

	I2C_Start();
	I2C_SendByte(0x70); //����0X70
  Receive_ACK();//�ȴ��ӻ�I2CӦ��	
  I2C_SendByte(0xac); //����0Xac
  Receive_ACK();//�ȴ��ӻ�I2CӦ��	
	I2C_SendByte(0x33); //����0X33
  Receive_ACK();//�ȴ��ӻ�I2CӦ��	
	I2C_SendByte(0x00); //����0X00
  Receive_ACK();//�ȴ��ӻ�I2CӦ��	
	I2C_Stop();
	
	delay_us(75000);
	while(((JH_Read_Status()&0x80)==0x80))//Bit[3]=1,�Ѿ�ʹ��У׼����
	{
		delay_us(1000);
		if(cnt++>=100)
			break;
 	}
	I2C_Start();
	I2C_SendByte(0x71);//���Ͷ�����
	Receive_ACK();//�ȴ��ӻ�I2CӦ��	
	Byte[0]=I2C_ReceiveByte();
	Send_ACK();  //�����ظ�Ӧ��ACK�ź�
	Byte[1]=I2C_ReceiveByte();
	Send_ACK();  //�����ظ�Ӧ��ACK�ź�
  Byte[2]=I2C_ReceiveByte();
	Send_ACK();  //�����ظ�Ӧ��ACK�ź�
	Byte[3]=I2C_ReceiveByte();
	Send_ACK();  //�����ظ�Ӧ��ACK�ź�
	Byte[4]=I2C_ReceiveByte();
	Send_ACK();  //�����ظ�Ӧ��ACK�ź�
	Byte[5]=I2C_ReceiveByte();
	Send_NOT_ACK();
	I2C_Stop();
	
	SO_RH=(SO_RH|Byte[1])<<8;
	SO_RH=(SO_RH|Byte[2])<<8;
	SO_RH= SO_RH|Byte[3];
	SO_RH=SO_RH>>4; 
	ct[0]=SO_RH;
	
	SO_TH=(SO_TH|Byte[3])<<8;
	SO_TH=(SO_TH|Byte[4])<<8;
	SO_TH=SO_TH|Byte[5];
	SO_TH=SO_TH&0x0fffff;    //���θ�4λ	
	ct[1]=SO_TH;
}
//*******************************************************
void JH_Init(void) //��ʼ��
{
	delay_us(11038);//11038us
	I2C_Start();
	I2C_SendByte(0x70); //����0X70
	Receive_ACK();//�ȴ��ӻ�I2CӦ��	
	I2C_SendByte(0xe1);//дϵͳ���üĴ��� ��ʼ������E1
	Receive_ACK();//�ȴ��ӻ�I2CӦ��	
	I2C_SendByte(0x08);       //���ָ������©д
	Receive_ACK();//�ȴ��ӻ�I2CӦ��	
	I2C_SendByte(0x00);
  Receive_ACK();//�ȴ��ӻ�I2CӦ��	
	I2C_Stop();
	delay_us(500000);//��ʱ0.5s
	while(JH_Read_Cal_Enable()==0) //�ȴ�״̬Bit[3]=1���ܶ����ݣ��������һֱ��0XBA���³�ʼ��ATH10 ֱ��Bit[3]=1
	{
		JH_Send_BA(); //��λ
		delay_us(100000);
		delay_us(11038);//11038us
		
		I2C_Start();
	  I2C_SendByte(0x70); //����0X70
	  Receive_ACK();//�ȴ��ӻ�I2CӦ��	
	  I2C_SendByte(0xe1);//дϵͳ���üĴ���  ��ʼ������E1
	  Receive_ACK();//�ȴ��ӻ�I2CӦ��	
	  I2C_SendByte(0x08);
	  Receive_ACK();//�ȴ��ӻ�I2CӦ��	
	  I2C_SendByte(0x00);
    Receive_ACK();//�ȴ��ӻ�I2CӦ��	
	  I2C_Stop();
	  delay_us(500000);
	}
}


















