#include "I2C.h"
u8 Byte[6];
u8 flag_ACK[20];
u32 SO_RH=0,SO_TH=0;
uint8_t val = 0;
uint8_t Byte_first;	
extern void delay_us(uint32_t time);//毫秒延时
//*************************************************************
void GPIO_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//****SCL******开漏输出*************************
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_PIN7_OPP(); //输出模式
}
//****SDA输出*************************************************
void GPIO_PIN7_OPP(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
//****SDA输出************************************************
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
//*********I2C开始信号*****************************************
void I2C_Start(void)  
{	 
	  SDA_OUT; //输出模式
	
	  SDA_H;
	  I2C_DELAY(8);
	  SCL_H;
 	  I2C_DELAY(8);
	  SDA_L;
    I2C_DELAY(8);
		SCL_L;
		I2C_DELAY(8);
} 
//***********I2C停止信号***************************
void I2C_Stop(void)
{
	  SDA_OUT; //输出模式
	
	  SDA_L;
	  I2C_DELAY(8);
		SCL_H;
		I2C_DELAY(8);
  	SDA_H;
	  I2C_DELAY(8);
}
//***********************************************************
uint8_t Receive_ACK(void) //从机返回应答
{
	u16 CNT;
	
	SDA_IN;      //输入模式  
	SCL_L;
	I2C_DELAY(8);
  SCL_H;
	I2C_DELAY(8);
	while((SDA_READ)&&(CNT<100))
		CNT++;
	if(CNT>=100)  //应答超时
	{
		CNT=0;
		return 0;
	}
	SCL_L;
	I2C_DELAY(8);
	return 1;    //应答OK
}
//*********I2C发送一个字节**************************************
void I2C_SendByte(uint8_t SendByte)   
{  
 uint8_t i = 8; 
 SDA_OUT; //输出模式
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
//********I2C接收一个字节**************************************
uint8_t I2C_ReceiveByte(void)    
{   
 uint8_t i = 8;  
 uint8_t ReceiveByte = 0;	
 SDA_IN;       //输入模式
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
void Send_ACK()        //主机回复应答ACK信号
{
	SDA_OUT; //输出模式
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
void Send_NOT_ACK(void)	//主机不回复应答
{
	SDA_OUT; //输出模式
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
//************查询Cal ENABLE 位有没有使能**********************************
u8 JH_Read_Cal_Enable(void)
{
  val = JH_Read_Status();
	 if((val & 0x68)==0x08)  //判断NOR模式和校准输出是否有效
		 return 1;
   else 
		 return 0;
 }
//************************************************************************
u8 JH_Read_Status(void)//读取AHT10的状态寄存器
{
	I2C_Start();
	I2C_SendByte(0x71);//发送0x71
	Receive_ACK();
	Byte_first = I2C_ReceiveByte();
	Send_NOT_ACK();
	I2C_Stop();
	return Byte_first;
}
//***********************************************************************
void JH_Send_BA(void)//向AHT发送BA命令
{
	I2C_Start();
	I2C_SendByte(0x70);//规格书发的是0x80规格书写错了
	Receive_ACK();
	I2C_SendByte(0xba);//发送复位命令0XBA
	Receive_ACK();
	I2C_Stop();
}
//*********************************************************************
void JH_Read_CTdata(uint32_t *ct)//读取AHT10的温度和湿度数据
{
	uint16_t cnt = 0;

	I2C_Start();
	I2C_SendByte(0x70); //发送0X70
  Receive_ACK();//等待从机I2C应答	
  I2C_SendByte(0xac); //发送0Xac
  Receive_ACK();//等待从机I2C应答	
	I2C_SendByte(0x33); //发送0X33
  Receive_ACK();//等待从机I2C应答	
	I2C_SendByte(0x00); //发送0X00
  Receive_ACK();//等待从机I2C应答	
	I2C_Stop();
	
	delay_us(75000);
	while(((JH_Read_Status()&0x80)==0x80))//Bit[3]=1,已经使能校准过了
	{
		delay_us(1000);
		if(cnt++>=100)
			break;
 	}
	I2C_Start();
	I2C_SendByte(0x71);//发送读命令
	Receive_ACK();//等待从机I2C应答	
	Byte[0]=I2C_ReceiveByte();
	Send_ACK();  //主机回复应答ACK信号
	Byte[1]=I2C_ReceiveByte();
	Send_ACK();  //主机回复应答ACK信号
  Byte[2]=I2C_ReceiveByte();
	Send_ACK();  //主机回复应答ACK信号
	Byte[3]=I2C_ReceiveByte();
	Send_ACK();  //主机回复应答ACK信号
	Byte[4]=I2C_ReceiveByte();
	Send_ACK();  //主机回复应答ACK信号
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
	SO_TH=SO_TH&0x0fffff;    //屏蔽高4位	
	ct[1]=SO_TH;
}
//*******************************************************
void JH_Init(void) //初始化
{
	delay_us(11038);//11038us
	I2C_Start();
	I2C_SendByte(0x70); //发送0X70
	Receive_ACK();//等待从机I2C应答	
	I2C_SendByte(0xe1);//写系统配置寄存器 初始化命令E1
	Receive_ACK();//等待从机I2C应答	
	I2C_SendByte(0x08);       //这个指令规格书漏写
	Receive_ACK();//等待从机I2C应答	
	I2C_SendByte(0x00);
  Receive_ACK();//等待从机I2C应答	
	I2C_Stop();
	delay_us(500000);//延时0.5s
	while(JH_Read_Cal_Enable()==0) //等待状态Bit[3]=1才能读数据，如果不等一直发0XBA重新初始化ATH10 直到Bit[3]=1
	{
		JH_Send_BA(); //复位
		delay_us(100000);
		delay_us(11038);//11038us
		
		I2C_Start();
	  I2C_SendByte(0x70); //发送0X70
	  Receive_ACK();//等待从机I2C应答	
	  I2C_SendByte(0xe1);//写系统配置寄存器  初始化命令E1
	  Receive_ACK();//等待从机I2C应答	
	  I2C_SendByte(0x08);
	  Receive_ACK();//等待从机I2C应答	
	  I2C_SendByte(0x00);
    Receive_ACK();//等待从机I2C应答	
	  I2C_Stop();
	  delay_us(500000);
	}
}


















