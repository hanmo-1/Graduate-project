//****************************************************************//
//	本例程适用于AHT21B，AHT25，AM2301B，AM2108，AM2315C，DHT20,
//单片机 ：STC89C52RC 
// 功能  ：串口发送温湿度数据  波特率 9600 
// 晶振  ：11.0592Mhz (用户系统时钟如不是12M 请更改相关宏定义及注释的延时时间)
// 编译环境:  Keil5
// 公司  ：奥松电子    
//****************************************************************//

#include "stdio.h"
#include "reg52.h"
#include <intrins.h>

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long int u32;


//定义IIC引脚
sbit SDA=P0^0;
sbit SCL=P0^1;

char buffer[100]={0};//串口打印的缓存
u8  CTDATA[7]={0};//用于CRC传递数组
u8 CRC_WrongFlag=0;//CRC错误标志位   1:Wrong,0:True
u8 ACK_Flag=0;//应答标志位   1:ACK,0:NACK



/**************************延时部分*****************************
用逻辑分析仪测得:1个nop:6us, 2个Nop:7us, 3个Nop:8.5us, 4个Nop:9.5us,
								5个Nop:10.5us, 6个Nop:11.5us
****************************************************************/
void delay_10us()//延时10us
{
  _nop_(); _nop_(); _nop_(); 
  _nop_(); _nop_();
}

//**********************************************
void delay_1ms()		//延时1ms，用逻辑分析仪测得i=84时，延时为1ms
{
	u8 i;
	for(i=84;i>0;i--)
	{
		delay_10us();
	}
}



//**********************************************
void delay_ms(u16 i)	//延时 i ms，不是十分精准，当i大于58时误差会超过1ms
{
	for(;i>0;i--)
	{
		delay_1ms();
	}
}






/**************************串口部分*****************************/

void UART_init(void)	//串口初始化
{
	//下面代码设置定时器 1
	TMOD = 0x20;		 //0010 0000 定时器 1 工作于方式 2（8 位自动重装方式）
	TH1 = 0xFD; 		//波特率： 9600 /11.0592MHZ
	TL1 = 0xFD; 		//波特率： 9600 /11.0592MHZ
	TR1 = 1;
	
	//下面代码设置定串口
	SCON = 0x50; 		//01010 0000 SM0.SM1=01(最普遍的 8 位通信） ,REN=1（允许接受）
	ES = 1; 				//关键：开启了中断就必须编写相应的中断函数，哪怕是中断空函数，
									//但必须有,否则程序进入中断入口地址后（这里是 0023H)不能跳出，必然出错
	EA = 1;
	
}


//**********************************************
//串口发送
void UARTSend(char UCHAR)
{
   SBUF=UCHAR;
  while(TI==0);
  TI=0;
}



//**********************************************
void UARTRead(void) interrupt 4
{
  char temp;
  if(RI)
  {
    RI=0;
    temp = SBUF;
  }
}


//**********************************************
void UART_PutString(unsigned char *buf )//串口发送字符串
{
	while(*buf)
      UARTSend(*buf++);
} 





/**********************************************
//串口打印IIC接收到的7个字节数据（16进制数）
**********************************************/
void UART_Data(unsigned char *buf ,unsigned char *bufdata)
{
	unsigned char a[2],i,j;
	
	UART_PutString("IIC_data: ");
	for(i=0;i<7;i++)
	{
		a[0] = bufdata[i]/16; 
		a[1] = bufdata[i]%16;
		for(j=0;j<2;j++)
		{
		  if(a[j]>9)
		  {
		    a[j] = (a[j]-10)+'A';
		  }
		  else
		  {
		    a[j] = a[j]+'0';
		  }
		  UARTSend(a[j]);
		}
		UARTSend(' ');
	} 
		UARTSend('\n');
}






/**************************IIC部分*****************************/

//送起始位 sda=1->0
void I2C_Start()
{
  SDA=1;
  SCL=1;
  delay_10us();
  SDA=0;
  delay_10us();
  SCL=0; 
}
//送停止位 sda=0->1
void Stop_I2C()
{
  SDA=0;
  SCL=1;
  delay_10us();
  SDA=1;
}
//主机发送ACK(包含ack:sda=0)
void Send_ACK(void)
{  //设置SDA 口为输出
   SDA=0;
   SCL=0;
   delay_10us();
   SCL=1;
   delay_10us();	
   SCL=0;
   SDA=1;
}
//主机发送NACK(no_ack:sda=1）
void Send_NOT_ACK(void)
{  //设置SDA 口为输出
   SDA=1;  
   SCL=0;
   delay_10us();
   SCL=1;
   delay_10us();
   SDA=1;
   SCL=0;
}

// 检测 SDA是否回ACK（ack:sda=1；Nack:sda=0）
u8 Receive_ACK(void)
{  //设置SDA 口为输入
	u8 cnt=0;
   SCL=0;
   delay_10us();
   SCL=1;
   delay_10us();
	while((SDA==1)&&cnt<100)
		cnt++;
	if(cnt==100)
	{
		ACK_Flag=0;//nack
	}else
	{
		ACK_Flag=1;//ack
	}
	SCL=0;
	delay_10us();
	 return ACK_Flag;
}






void AHT20_WR_Byte(u8 Byte) //往AHT20写一个字节
{
	u8 Data,N,i;	
	Data=Byte;
	i = 0x80;
	for(N=0;N<8;N++)
	{
		SCL=0; 
		delay_10us();	
		if(i&Data)
		{
			SDA=1;
		}
		else
		{
			SDA=0;
		}	
		SCL=1;
		delay_10us();
		Data <<= 1;
		 
	}
	SCL=0;
	delay_10us();   
}	


u8 AHT20_RD_Byte(void)//从AHT20读取一个字节
{
	u8 Byte,i,a;
	Byte = 0;
	
	for(i=0;i<8;i++)
	{
		SCL=0;
		delay_10us();
		SCL=1;
		delay_10us();
		a=0;
		if(SDA==1)a=1;
		Byte=(Byte<<1);
		Byte|=a;
		SCL=0;
		
	}
	return Byte;
	
	
	
}




u8 AHT20_Read_Status(void)//读取AHT20的状态寄存器
{

	u8 Byte_first;	
	I2C_Start();
	AHT20_WR_Byte(0x71);
	
	Receive_ACK();
	Byte_first = AHT20_RD_Byte();
	Send_NOT_ACK();
	Stop_I2C();
	return Byte_first;
}

u8 AHT20_Read_Cal_Enable(void)  //查询cal enable位有没有使能
{
	u8 val = 0;//ret = 0,
  val = AHT20_Read_Status();
	 if((val & 0x68)==0x08)
		 return 1;
   else  return 0;
 }

void AHT20_SendAC(void) //向AHT20发送AC命令
{

	I2C_Start();
	AHT20_WR_Byte(0x70);
	Receive_ACK();
	AHT20_WR_Byte(0xac);//0xAC采集命令
	Receive_ACK();
	AHT20_WR_Byte(0x33);
	Receive_ACK();
	AHT20_WR_Byte(0x00);
	Receive_ACK();
	Stop_I2C();

}


//CRC校验类型：CRC8/MAXIM
//多项式：X8+X5+X4+1
//Poly：0011 0001  0x31
//高位放到后面就变成 1000 1100 0x8c
//C现实代码：
u8 Calc_CRC8(u8 *message,u8 Num)
{
        u8 i;
        u8 byte;
        u8 crc=0xFF;
  for(byte=0; byte<Num; byte++)
  {
    crc^=(message[byte]);
    for(i=8;i>0;--i)
    {
      if(crc&0x80) crc=(crc<<1)^0x31;
      else crc=(crc<<1);
    }
  }
        return crc;
}





void AHT20_Read_CTdata(u32 *ct) //没有CRC校验，直接读取AHT20的温度和湿度数据
{
	volatile u8  Byte_1th=0;
	volatile u8  Byte_2th=0;
	volatile u8  Byte_3th=0;
	volatile u8  Byte_4th=0;
	volatile u8  Byte_5th=0;
	volatile u8  Byte_6th=0;
	volatile u8  Byte_7th=0;
	 u32 RetuData = 0;
	u16 cnt = 0;
	AHT20_SendAC();//向AHT10发送AC命令
	delay_ms(80);//延时80ms左右	
    cnt = 0;
	while(((AHT20_Read_Status()&0x80)==0x80))//直到状态bit[7]为0，表示为空闲状态，若为1，表示忙状态
	{
		delay_ms(2);
		if(cnt++>=100)
		{
		 break;
		 }
	}
	I2C_Start();
	AHT20_WR_Byte(0x71);
	Receive_ACK();
	CTDATA[0]=Byte_1th = AHT20_RD_Byte();//状态字，查询到状态为0x98,表示为忙状态，bit[7]为1；状态为0x1C，或者0x0C，或者0x08表示为空闲状态，bit[7]为0
	Send_ACK();
	CTDATA[1]=Byte_2th = AHT20_RD_Byte();//湿度
	Send_ACK();
	CTDATA[2]=Byte_3th = AHT20_RD_Byte();//湿度
	Send_ACK();
	CTDATA[3]=Byte_4th = AHT20_RD_Byte();//湿度/温度
	Send_ACK();
	CTDATA[4]=Byte_5th = AHT20_RD_Byte();//温度
	Send_ACK();
	CTDATA[5]=Byte_6th = AHT20_RD_Byte();//温度
	Send_NOT_ACK();
	Stop_I2C();

	RetuData = (RetuData|Byte_2th)<<8;
	RetuData = (RetuData|Byte_3th)<<8;
	RetuData = (RetuData|Byte_4th);
	RetuData =RetuData >>4;
	ct[0] = RetuData;//湿度
	RetuData = 0;
	RetuData = (RetuData|Byte_4th)<<8;
	RetuData = (RetuData|Byte_5th)<<8;
	RetuData = (RetuData|Byte_6th);
	RetuData = RetuData&0xfffff;
	ct[1] =RetuData; //温度
	

}


void AHT20_Read_CTdata_crc(u32 *ct) //CRC校验后，读取AHT20的温度和湿度数据
{
	volatile u8  Byte_1th=0;
	volatile u8  Byte_2th=0;
	volatile u8  Byte_3th=0;
	volatile u8  Byte_4th=0;
	volatile u8  Byte_5th=0;
	volatile u8  Byte_6th=0;
	volatile u8  Byte_7th=0;
	 u32 RetuData = 0;
	 u16 cnt = 0;

	
	AHT20_SendAC();//向AHT10发送AC命令
	delay_ms(80);//延时80ms左右	
    cnt = 0;
	while(((AHT20_Read_Status()&0x80)==0x80))//直到状态bit[7]为0，表示为空闲状态，若为1，表示忙状态
	{
		delay_ms(1);
		if(cnt++>=100)
		{
		 break;
		}
	}
	
	I2C_Start();

	AHT20_WR_Byte(0x71);
	Receive_ACK();
	CTDATA[0]=Byte_1th = AHT20_RD_Byte();//状态字，查询到状态为0x98,表示为忙状态，bit[7]为1；状态为0x1C，或者0x0C，或者0x08表示为空闲状态，bit[7]为0
	Send_ACK();
	CTDATA[1]=Byte_2th = AHT20_RD_Byte();//湿度
	Send_ACK();
	CTDATA[2]=Byte_3th = AHT20_RD_Byte();//湿度
	Send_ACK();
	CTDATA[3]=Byte_4th = AHT20_RD_Byte();//湿度/温度
	Send_ACK();
	CTDATA[4]=Byte_5th = AHT20_RD_Byte();//温度
	Send_ACK();
	CTDATA[5]=Byte_6th = AHT20_RD_Byte();//温度
	Send_ACK();
	CTDATA[6]=Byte_7th = AHT20_RD_Byte();//CRC数据
	Send_NOT_ACK();                           //注意: 最后是发送NAK
	Stop_I2C();
	
	if(Calc_CRC8(CTDATA,6)==Byte_7th)
	{
	CRC_WrongFlag=0;
		
	RetuData = (RetuData|Byte_2th)<<8;
	RetuData = (RetuData|Byte_3th)<<8;
	RetuData = (RetuData|Byte_4th);
	RetuData =RetuData >>4;
	ct[0] = RetuData;//湿度
	RetuData = 0;
	RetuData = (RetuData|Byte_4th)<<8;
	RetuData = (RetuData|Byte_5th)<<8;
	RetuData = (RetuData|Byte_6th);
	RetuData = RetuData&0xfffff;
	ct[1] =RetuData; //温度
		
	}
	else
	{
//		ct[0]=0x00;
//		ct[1]=0x00;//校验错误返回值，客户可以根据自己需要更改
		CRC_WrongFlag=1;//CRC错误标志位

	}//CRC数据
}


void AHT20_Init(void)   //初始化AHT20
{	
	I2C_Start();
	AHT20_WR_Byte(0x70);
	Receive_ACK();
	AHT20_WR_Byte(0xa8);//0xA8进入NOR工作模式
	Receive_ACK();
	AHT20_WR_Byte(0x00);
	Receive_ACK();
	AHT20_WR_Byte(0x00);
	Receive_ACK();
	Stop_I2C();

	delay_ms(10);//延时10ms左右

	I2C_Start();
	AHT20_WR_Byte(0x70);
	Receive_ACK();
	AHT20_WR_Byte(0xbe);//0xBE初始化命令，AHT20的初始化命令是0xBE,   AHT10的初始化命令是0xE1
	Receive_ACK();
	AHT20_WR_Byte(0x08);//相关寄存器bit[3]置1，为校准输出
	Receive_ACK();
	AHT20_WR_Byte(0x00);
	Receive_ACK();
	Stop_I2C();
	delay_ms(10);//延时10ms左右
}
void JH_Reset_REG(u8 addr)
{
	
	u8 Byte_first,Byte_second,Byte_third;
	I2C_Start();
	AHT20_WR_Byte(0x70);//原来是0x70
	Receive_ACK();
	AHT20_WR_Byte(addr);
	Receive_ACK();
	AHT20_WR_Byte(0x00);
	Receive_ACK();
	AHT20_WR_Byte(0x00);
	Receive_ACK();
	Stop_I2C();

	delay_ms(5);//延时5ms左右
	I2C_Start();
	AHT20_WR_Byte(0x71);//
	Receive_ACK();
	Byte_first = AHT20_RD_Byte();
	Send_ACK();
	Byte_second = AHT20_RD_Byte();
	Send_ACK();
	Byte_third = AHT20_RD_Byte();
	Send_NOT_ACK();
	Stop_I2C();
	
    delay_ms(10);//延时10ms左右
	I2C_Start();
	AHT20_WR_Byte(0x70);///
	Receive_ACK();
	AHT20_WR_Byte(0xB0|addr);////寄存器命令
	Receive_ACK();
	AHT20_WR_Byte(Byte_second);
	Receive_ACK();
	AHT20_WR_Byte(Byte_third);
	Receive_ACK();
	Stop_I2C();
	
	Byte_second=0x00;
	Byte_third =0x00;
}

void AHT20_Start_Init(void)
{
	JH_Reset_REG(0x1b);
	JH_Reset_REG(0x1c);
	JH_Reset_REG(0x1e);
}







int main()
{
  u32 CT_data[2];
	volatile float  c1,t1;

	UART_init();
	delay_ms(500);//①刚上电，产品芯片内部就绪需要时间，延时100~500ms,建议500ms

	if((AHT20_Read_Status()&0x18)!=0x18)//②上电第一次发0x71读取状态字，判断状态字是否为0x18,如果不是0x18,进行寄存器初始化
	{
	AHT20_Start_Init(); //重新初始化寄存器
	delay_ms(10);
	}
	

	while(1)//③根据客户自己需求发测量命令读取温湿度数据，当前while（1）循环发测量命令读取温湿度数据，仅供参考
	{
//	 AHT20_Read_CTdata(CT_data);       //不经过CRC校验，直接读取AHT20的温度和湿度数据    推荐每隔大于1S读一次
   AHT20_Read_CTdata_crc(CT_data);  //crc校验后，读取AHT20的温度和湿度数据 
	

	 c1 = ((float)CT_data[0]/1024/1024)*100;  //计算得到湿度值c1放大10倍
	 t1 = ((float)CT_data[1]/1024/1024)*200-50;//计算得到温度值t1放大10倍
		
		
		
		
		
	////下一步客户处理显示数据，
		
		if(ACK_Flag==0)//判断是否有应答，0为无应答
		{
		UART_PutString("No Connected! \r\n");
		}
		else if(CRC_WrongFlag==0)//判断CRC校验是否正确
		{
		sprintf(buffer,"温度:%.2f \r\n湿度:%.2f%%\r\n",t1,c1);//串口打印温湿度数据
		UART_PutString(buffer);
		UART_Data(buffer,CTDATA);
		}
		else//CRC校验失败
		{
		UART_PutString("CRC Wrong!\r\n");
		UART_Data(buffer,CTDATA);
		}
		
		delay_ms(2000);
	 }

}











