//-----------------------------------------------------------------
// 程序描述:
//      ADS1256单通道8路，差分4路测试
// 作    者: 凌智电子
// 开始日期: 2017-12-15
// 完成日期: 2017-01-04
// 修改日期: 
// 当前版本: V1.0.0
// 调试工具: 凌智STM32+FPGA电子系统设计开发板、2.8寸液晶屏、LZE_ST_LINK2、ADS1256_ADC_V1.0
// 说    明:

//-----------------------------------------------------------------
#include "stm32f10x.h"
#include "ads1256.h"
#include "Delay.h"
#include "lze_lcd.h"


int32_t AdcNow[8];		//8路ADC采集结果（实时）有符号数
uint8_t Channel;			//当前通道 
uint8_t ScanMode;			//扫描模式，0表示单端8路， 1表示差分4路

/*
*********************************************************************************************************
*	函 数 名: ADS1256_Send8Bit
*	功能说明: 向SPI总线发送8个bit数据。 不带CS控制。
*	形    参: _data : 数据
*	返 回 值: 无
*********************************************************************************************************
*/
void ADS1256_Send8Bit(uint8_t data)
{
	uint8_t i;

	/* 连续发送多个字节时，需要延迟一下 */
	Delay_ns(250);
	Delay_ns(250);
// 	Delay_1us(1);
	
	/*　ADS1256 要求 SCL高电平和低电平持续时间最小 200ns  */
	for(i = 0; i < 8; i++)
	{
		if (data & 0x80)
		{
			DIN_H;
		}
		else
		{
			DIN_L;
		}
		SCLK_H;
		Delay_ns(250);
// 		Delay_1us(1);
		data <<= 1;
		SCLK_L;			/* <----  ADS1256 是在SCK下降沿采样DIN数据, 数据必须维持 50nS */
		Delay_ns(250);
// 		Delay_1us(1);
	}
}

/*********************************************************************************************************
*	函 数 名: ADS1256_Recive8Bit
*	功能说明: 从SPI总线接收8个bit数据。 不带CS控制。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************/
uint8_t ADS1256_Recive8Bit(void)
{
	uint8_t i;
	uint8_t read = 0;

	Delay_ns(250);
	Delay_ns(250);
// 	Delay_1us(1);
	/*　ADS1256 要求 SCL高电平和低电平持续时间最小 200ns  */
	for (i = 0; i < 8; i++)
	{
		SCLK_H;
		Delay_ns(250);
// 		Delay_1us(1);
		read = read<<1;
		SCLK_L;
		if (DO_IS_H)
		{
			read++;
		}
		Delay_ns(250);
// 		Delay_1us(1);
	}
	return read;
}

/*********************************************************************************************************
*	函 数 名: ADS1256_WriteReg
*	功能说明: 写指定的寄存器
*	形    参:  _RegID : 寄存器ID
*			  _RegValue : 寄存器值
*	返 回 值: 无
*********************************************************************************************************/
void ADS1256_WriteReg(uint8_t RegID, uint8_t RegValue)
{
	CS_L;	/* SPI片选 = 0 */
	ADS1256_Send8Bit(CMD_WREG | RegID);	/* 写寄存器的命令, 并发送寄存器地址 */
	ADS1256_Send8Bit(0x00);		/* 寄存器个数 - 1, 此处写1个寄存器 */
	ADS1256_Send8Bit(RegValue);	/* 发送寄存器值 */
	CS_H;	/* SPI片选 = 1 */
}

/*********************************************************************************************************
*	函 数 名: ADS1256_ReadReg
*	功能说明: 读指定的寄存器
*	形    参:  _RegID : 寄存器ID
*			  _RegValue : 寄存器值。
*	返 回 值: 读到的寄存器值。
*********************************************************************************************************/
uint8_t ADS1256_ReadReg(uint8_t _RegID)
{
	uint8_t read;

	CS_L;	/* SPI片选 = 0 */
	ADS1256_Send8Bit(CMD_RREG | _RegID);	/* 写寄存器的命令, 并发送寄存器地址 */
	ADS1256_Send8Bit(0x00);	/* 寄存器个数 - 1, 此处读1个寄存器 */

	Delay_1us(1);	/* 必须延迟才能读取芯片返回数据 */

	read = ADS1256_Recive8Bit();	/* 读寄存器值 */
	CS_H;	/* SPI片选 = 1 */

	return read;
}

/*********************************************************************************************************
*	函 数 名: ADS1256_WriteCmd
*	功能说明: 发送单字节命令
*	形    参:  _cmd : 命令
*	返 回 值: 无
*********************************************************************************************************/
void ADS1256_WriteCmd(uint8_t cmd)
{
	CS_L;	/* SPI片选 = 0 */
	ADS1256_Send8Bit(cmd);
	CS_H;	/* SPI片选 = 1 */
}

/*********************************************************************************************************
*	函 数 名: ADS1256_ReadChipID
*	功能说明: 读芯片ID, 读状态寄存器中的高4bit
*	形    参: 无
*	返 回 值: 8bit状态寄存器值的高4位
*********************************************************************************************************/
uint8_t ADS1256_ReadChipID(void)
{
	uint8_t id;

	ADS1256_WaitDRDY();
	id = ADS1256_ReadReg(REG_STATUS);
	return (id >> 4);
}

/*********************************************************************************************************
*	函 数 名: ADS1256_SetChannal
*	功能说明: 配置通道号。多路复用。AIN- 固定接地（ACOM).
*	形    参: _ch : 通道号， 0-7
*	返 回 值: 无
*********************************************************************************************************/
void ADS1256_SetChannal(uint8_t ch)
{
	/*
	位7-4 PSEL3, PSEL2, PSEL1, PSEL0: 正向输入通道 (AINP) 选择
		0000 = AIN0 (default)
		0001 = AIN1
		0010 = AIN2 (ADS1256 only)
		0011 = AIN3 (ADS1256 only)
		0100 = AIN4 (ADS1256 only)
		0101 = AIN5 (ADS1256 only)
		0110 = AIN6 (ADS1256 only)
		0111 = AIN7 (ADS1256 only)
		1xxx = AINCOM (when PSEL3 = 1, PSEL2, PSEL1, PSEL0 are “don’t care”)

	位 3-0 NSEL3, NSEL2, NSEL1, NSEL0:反向输入通道 (AINN)选择
		0000 = AIN0
		0001 = AIN1 (default)
		0010 = AIN2 (ADS1256 only)
		0011 = AIN3 (ADS1256 only)
		0100 = AIN4 (ADS1256 only)
		0101 = AIN5 (ADS1256 only)
		0110 = AIN6 (ADS1256 only)
		0111 = AIN7 (ADS1256 only)
		1xxx = AINCOM (when NSEL3 = 1, NSEL2, NSEL1, NSEL0 are “don’t care”)
	*/
	if (ch > 7)
	{
		return;
	}
	ADS1256_WriteReg(REG_MUX, (ch << 4) | (1 << 3));	/* Bit3 = 1, AINN 固定接 AINCOM */
}

/*********************************************************************************************************
*	函 数 名: ADS1256_SetDiffChannal
*	功能说明: 配置差分通道号。多路复用。
*	形    参: _ch : 通道号,0-3；共4对
*	返 回 值: 8bit状态寄存器值的高4位
*********************************************************************************************************/
void ADS1256_SetDiffChannal(uint8_t ch)
{
	/*
	位7-4 PSEL3, PSEL2, PSEL1, PSEL0: 正向输入通道 (AINP) 选择
		0000 = AIN0 (default)
		0001 = AIN1
		0010 = AIN2 (ADS1256 only)
		0011 = AIN3 (ADS1256 only)
		0100 = AIN4 (ADS1256 only)
		0101 = AIN5 (ADS1256 only)
		0110 = AIN6 (ADS1256 only)
		0111 = AIN7 (ADS1256 only)
		1xxx = AINCOM (when PSEL3 = 1, PSEL2, PSEL1, PSEL0 are “don’t care”)

	位 3-0 NSEL3, NSEL2, NSEL1, NSEL0:反向输入通道 (AINN)选择
		0000 = AIN0
		0001 = AIN1 (default)
		0010 = AIN2 (ADS1256 only)
		0011 = AIN3 (ADS1256 only)
		0100 = AIN4 (ADS1256 only)
		0101 = AIN5 (ADS1256 only)
		0110 = AIN6 (ADS1256 only)
		0111 = AIN7 (ADS1256 only)
		1xxx = AINCOM (when NSEL3 = 1, NSEL2, NSEL1, NSEL0 are “don’t care”)
	*/
	if (ch == 0)
	{
		ADS1256_WriteReg(REG_MUX, (0 << 4) | 1);	/* 差分输入 AIN0， AIN1 */
	}
	else if (ch == 1)
	{
		ADS1256_WriteReg(REG_MUX, (2 << 4) | 3);	/* 差分输入 AIN2， AIN3 */
	}
	else if (ch == 2)
	{
		ADS1256_WriteReg(REG_MUX, (4 << 4) | 5);	/* 差分输入 AIN4， AIN5 */
	}
	else if (ch == 3)
	{
		ADS1256_WriteReg(REG_MUX, (6 << 4) | 7);	/* 差分输入 AIN6， AIN7 */
	}
}

/*********************************************************************************************************
*	函 数 名: ADS1256_WaitDRDY
*	功能说明: 等待内部操作完成。 自校准时间较长，需要等待。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************/
void ADS1256_WaitDRDY(void)
{
	uint32_t i;

	for (i = 0; i < 40000000; i++)
	{
		if (DRDY_IS_L)
		{
			break;
		} 
	}
	if (i >= 40000000)
	{
		LCD_WriteString(40,10,LCD_COLOR_BLUE,LCD_COLOR_BLACK,"ADS1256_WaitDRDY() Time Out ...");
	}
}
union date
{
	uint32_t read;
	uint8_t read1[4];
};
/*********************************************************************************************************
*	函 数 名: ADS1256_ReadData
*	功能说明: 读ADC数据
*	形    参: 无
*	返 回 值: 采样结果 read
*********************************************************************************************************/
int32_t ADS1256_ReadData(void)
{
	union date Adc;
	
// 	uint32_t read = 0;

	CS_L;	/* SPI片选 = 0 */

	ADS1256_Send8Bit(CMD_RDATA);	/* 读数据的命令 */

	Delay_1us(1);	/* 必须延迟才能读取芯片返回数据 */
	
	Adc.read1[3] = 0;	
	Adc.read1[2] = ADS1256_Recive8Bit();
	Adc.read1[1] = ADS1256_Recive8Bit();
	Adc.read1[0] = ADS1256_Recive8Bit();
	
	/* 读采样结果，3个字节，高字节在前 */
// 	read = (uint32_t)ADS1256_Recive8Bit() << 16;
// 	read += ((uint32_t)ADS1256_Recive8Bit() << 8);
// 	read += ADS1256_Recive8Bit();

	CS_H;	/* SPI片选 = 1 */

	/* 负数进行扩展。24位有符号数扩展为32位有符号数 */
// 	if (read & 0x800000)
// 	{
// 		read -= 0x800000;
// 		read += 0x80000000;
// 	}

	return Adc.read;
}

//#if 0
/*********************************************************************************************************
*	函 数 名: ADS1256_ReadAdc
*	功能说明: 读指定通道的ADC数据
*	形    参: 读取的通道 ch
*	返 回 值: 采样结果 read
*********************************************************************************************************/
int32_t ADS1256_ReadAdc(uint8_t ch)
{
	/* ADS1256 数据手册第21页 */

//#if 0	/* 对于30Ksps 采样速率 */
	int32_t read;

	while (DRDY_IS_L);	/* 等待 DRDY 高 */
	while (!DRDY_IS_L);	/* 等待 DRDY 低 */

	ADS1256_SetChannal(ch);	/* 切换模拟通道 */
	Delay_1us(5);

	ADS1256_WriteCmd(CMD_SYNC);	/*同步A/D转换*/
	Delay_1us(5);

	ADS1256_WriteCmd(CMD_WAKEUP);  /* 正常情况下，这个时候 DRDY 已经为高 */
	Delay_1us(25);

	read =  (int32_t)ADS1256_ReadData();

	while (DRDY_IS_L);	/* 等待 DRDY 高 */
	while (!DRDY_IS_L);	/* 等待 DRDY 低 */

	read =  (int32_t)ADS1256_ReadData();

	return read;
//#else
//	//while (DRDY_IS_LOW());

//	/* ADS1256 数据手册第21页 */
//	ADS1256_WaitDRDY();		/* 等待 DRDY = 0 */

//	ADS1256_SetChannal(ch);	/* 切换模拟通道 */
//	Delay_1us(5);

//	ADS1256_WriteCmd(CMD_SYNC);
//	Delay_1us(5);

//	ADS1256_WriteCmd(CMD_WAKEUP);
//	Delay_1us(25);

//	//ADS1256_WaitDRDY();		/* 等待 DRDY = 0 */

//	return (int32_t)ADS1256_ReadData();
//#endif
}
//#endif

/*********************************************************************************************************
*	函 数 名: ADS1256_StartScan
*	功能说明: 将 DRDY引脚 （PC15 ）配置成外部中断触发方式， 中断服务程序中扫描8个通道的数据。
*	形    参: _ucDiffMode : 0 表示单端模式（扫描8路）； 1表示差分模式，扫描4路
*	返 回 值: 无
*********************************************************************************************************/
void ADS1256_StartScan(uint8_t ucScanMode)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	ScanMode = ucScanMode;
	/* 开始扫描前, 清零结果缓冲区 */
	{
		uint8_t i;
		Channel = 0;
		for (i = 0; i < 8; i++)
		{
			AdcNow[i] = 0;
		}
	}

	/* Enable AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	/* Connect EXTI15 Line to PC15 pin */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource9);

	/* Configure EXTI15 line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  /* 下降沿 */
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI3 Interrupt  priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*********************************************************************************************************
*	函 数 名: ADS1256_StartScan
*	功能说明: 将 DRDY引脚 （PC15 ）配置成外部中断触发方式， 中断服务程序中扫描8个通道的数据。
*	形    参: _ucDiffMode : 0 表示单端模式（扫描8路）； 1表示差分模式，扫描4路
*	返 回 值: 无
*********************************************************************************************************/
void ADS1256_StartScan_1(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	/* Enable AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	/* Connect EXTI15 Line to PC15 pin */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource9);

	/* Configure EXTI15 line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  /* 下降沿 */
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI3 Interrupt  priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*********************************************************************************************************
*	函 数 名: ADS1256_StopScan
*	功能说明: 停止 DRDY 中断
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************/
void ADS1256_StopScan(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	/* 配置 EXTI LineXXX */
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	/* 下降沿(等待 DRDY 由1变0的时刻) */
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;		/* 禁止 */
	EXTI_Init(&EXTI_InitStructure);

	/* 中断优先级配置 最低优先级 这里一定要分开的设置中断，不能够合并到一个里面设置 */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;		/* 禁止 */
	NVIC_Init(&NVIC_InitStructure);
}

/*********************************************************************************************************
*	函 数 名: ADS1256_GetAdc
*	功能说明: 从缓冲区读取ADC采样结果。采样结构是由中断服务程序填充的。
*	形    参: _ch 通道号 (0 - 7)
*	返 回 值: ADC采集结果（有符号数）
*********************************************************************************************************/
int32_t ADS1256_GetAdc(uint8_t ch)  //主函数直接调用这个函数
{
	int32_t iTemp;

	if (ch > 7)
	{
		return 0;
	}

	ADS1256_StopScan();	/* 禁止中断 */

	iTemp = AdcNow[ch];

	ADS1256_StartScan_1();	/* 使能中断 */

	return iTemp;
}

/*********************************************************************************************************
*	函 数 名: ADS1256_ISR
*	功能说明: 定时采集中断服务程序
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************/
void ADS1256_ISR(void)    //Y  中断服务程序中调用该程序；
{
	if (ScanMode == 0)	/* 0 表示单端8路扫描，1表示差分4路扫描 */
	{
		/* 读取采集结构，保存在全局变量 */
		ADS1256_SetChannal(Channel);	/* 切换模拟通道 */
// 		Delay_1us(1);
		Delay_ns(250);

		ADS1256_WriteCmd(CMD_SYNC);
// 		Delay_1us(1);
		Delay_ns(250);

		ADS1256_WriteCmd(CMD_WAKEUP);
// 		Delay_1us(1);
		Delay_ns(250);

		if (Channel == 0)
		{
			AdcNow[7] = ADS1256_ReadData();	/* 注意保存的是上一个通道的数据 */
		}
		else
		{
			AdcNow[Channel-1] = ADS1256_ReadData();	/* 注意保存的是上一个通道的数据 */
		}

		if (++Channel >= 8)
		{
			Channel = 0;
		}
	}
	else	/* 差分4路扫描 */
	{
		/* 读取采集结构，保存在全局变量 */
		ADS1256_SetDiffChannal(Channel);	/* 切换模拟通道 */
		Delay_1us(5);

		ADS1256_WriteCmd(CMD_SYNC);
		Delay_1us(5);

		ADS1256_WriteCmd(CMD_WAKEUP);
		Delay_1us(25);

		if (Channel == 0)
		{
			AdcNow[3] = ADS1256_ReadData();	/* 注意保存的是上一个通道的数据 */
		}
		else
		{
			AdcNow[Channel-1] = ADS1256_ReadData();	/* 注意保存的是上一个通道的数据 */
		}

		if (++Channel >= 4)
		{
			Channel = 0;
		}
	}
}

/*********************************************************************************************************
*	函 数 名: EXTI3_IRQHandler
*	功能说明: 外部中断服务程序.
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************/
void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line9) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line9);		/* 清除中断标志位 */

		ADS1256_ISR();

		/* 执行上面的代码完毕后，再次清零中断标志 */
		EXTI_ClearITPendingBit(EXTI_Line9);		/* 清除中断标志位 */
	}
}

/*********************************************************************************************************
*	函 数 名: ADS1256_CfgADC
*	功能说明: 配置ADC参数，增益和数据输出速率
*	形    参: gain : 增益
*			  _		drate : 数据输出速率
*	返 回 值: 无
*********************************************************************************************************/
void ADS1256_CfgADC(uint8_t gain, uint8_t drate)   //Y
{
	ADS1256_StopScan();			/* 暂停CPU中断 */

	ADS1256_ResetHard();		/* 硬件复位 */
	
	ADS1256_WriteCmd(CMD_RESET);
	
	ADS1256_WriteReg(REG_STATUS,0xf4);

  ADS1256_WriteCmd(CMD_SELFCAL);

	ADS1256_WaitDRDY();

	{
		uint8_t buf[4];		/* 暂存ADS1256 寄存器配置参数，之后连续写4个寄存器 */

		/* 状态寄存器定义
			Bits 7-4 ID3, ID2, ID1, ID0  Factory Programmed Identification Bits (Read Only)

			Bit 3 ORDER: Data Output Bit Order
				0 = Most Significant Bit First (default)
				1 = Least Significant Bit First
			Input data  is always shifted in most significant byte and bit first. Output data is always shifted out most significant
			byte first. The ORDER bit only controls the bit order of the output data within the byte.

			Bit 2 ACAL : Auto-Calibration
				0 = Auto-Calibration Disabled (default)
				1 = Auto-Calibration Enabled
			When Auto-Calibration is enabled, self-calibration begins at the completion of the WREG command that changes
			the PGA (bits 0-2 of ADCON register), DR (bits 7-0 in the DRATE register) or BUFEN (bit 1 in the STATUS register)
			values.

			Bit 1 BUFEN: Analog Input Buffer Enable
				0 = Buffer Disabled (default)
				1 = Buffer Enabled

			Bit 0 DRDY :  Data Ready (Read Only)
				This bit duplicates the state of the DRDY pin.

			ACAL=1使能自校准功能。当 PGA，BUFEEN, DRATE改变时会启动自校准
		*/
		buf[0] = (0 << 3) | (1 << 2) | (0 << 1);
		//ADS1256_WriteReg(REG_STATUS, (0 << 3) | (1 << 2) | (1 << 1));

		buf[1] = 0x08;	/* 高四位0表示AINP接 AIN0,  低四位8表示 AINN 固定接 AINCOM */

		/*	ADCON: A/D Control Register (Address 02h)
			Bit 7 Reserved, always 0 (Read Only)
			Bits 6-5 CLK1, CLK0 : D0/CLKOUT Clock Out Rate Setting
				00 = Clock Out OFF
				01 = Clock Out Frequency = fCLKIN (default)
				10 = Clock Out Frequency = fCLKIN/2
				11 = Clock Out Frequency = fCLKIN/4
				When not using CLKOUT, it is recommended that it be turned off. These bits can only be reset using the RESET pin.

			Bits 4-2 SDCS1, SCDS0: Sensor Detect Current Sources
				00 = Sensor Detect OFF (default)
				01 = Sensor Detect Current = 0.5 μ A
				10 = Sensor Detect Current = 2 μ A
				11 = Sensor Detect Current = 10μ A
				The Sensor Detect Current Sources can be activated to verify  the integrity of an external sensor supplying a signal to the
				ADS1255/6. A shorted sensor produces a very small signal while an open-circuit sensor produces a very large signal.

			Bits 2-0 PGA2, PGA1, PGA0: Programmable Gain Amplifier Setting
				000 = 1 (default)
				001 = 2
				010 = 4
				011 = 8
				100 = 16
				101 = 32
				110 = 64
				111 = 64
		*/
		buf[2] = (0 << 5) | (0 << 3) | (gain << 0);
		//ADS1256_WriteReg(REG_ADCON, (0 << 5) | (0 << 2) | (GAIN_1 << 0));	/* 选择1;1增益, 输入正负5V */

		/* 因为切换通道和读数据耗时 123uS, 因此扫描中断模式工作时，最大速率 = DRATE_1000SPS */
		buf[3] = drate;	// DRATE_10SPS;	/* 选择数据输出速率 */

		CS_L;	/* SPI片选 = 0 */
		ADS1256_Send8Bit(CMD_WREG | 0);	/* 写寄存器的命令, 并发送寄存器地址 */
		ADS1256_Send8Bit(0x03);			/* 寄存器个数 - 1, 此处3表示写4个寄存器 */

		ADS1256_Send8Bit(buf[0]);	/* 设置状态寄存器 */
		ADS1256_Send8Bit(buf[1]);	/* 设置输入通道参数 */
		ADS1256_Send8Bit(buf[2]);	/* 设置ADCON控制寄存器，增益 */
		ADS1256_Send8Bit(buf[3]);	/* 设置输出数据速率 */

		CS_H;	/* SPI片选 = 1 */
	}

	Delay_1us(50);
}

/*********************************************************************************************************
*	函 数 名: ADS1256_ResetHard
*	功能说明: 硬件复位 ADS1256芯片.低电平复位。最快4个时钟，也就是 4x0.13uS = 0.52uS
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************/
void ADS1256_ResetHard(void)
{
	Delay_1us(5);
}
