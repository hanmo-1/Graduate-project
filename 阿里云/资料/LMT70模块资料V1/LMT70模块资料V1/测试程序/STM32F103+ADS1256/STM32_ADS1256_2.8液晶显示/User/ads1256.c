//-----------------------------------------------------------------
// ��������:
//      ADS1256��ͨ��8·�����4·����
// ��    ��: ���ǵ���
// ��ʼ����: 2017-12-15
// �������: 2017-01-04
// �޸�����: 
// ��ǰ�汾: V1.0.0
// ���Թ���: ����STM32+FPGA����ϵͳ��ƿ����塢2.8��Һ������LZE_ST_LINK2��ADS1256_ADC_V1.0
// ˵    ��:

//-----------------------------------------------------------------
#include "stm32f10x.h"
#include "ads1256.h"
#include "Delay.h"
#include "lze_lcd.h"


int32_t AdcNow[8];		//8·ADC�ɼ������ʵʱ���з�����
uint8_t Channel;			//��ǰͨ�� 
uint8_t ScanMode;			//ɨ��ģʽ��0��ʾ����8·�� 1��ʾ���4·

/*
*********************************************************************************************************
*	�� �� ��: ADS1256_Send8Bit
*	����˵��: ��SPI���߷���8��bit���ݡ� ����CS���ơ�
*	��    ��: _data : ����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ADS1256_Send8Bit(uint8_t data)
{
	uint8_t i;

	/* �������Ͷ���ֽ�ʱ����Ҫ�ӳ�һ�� */
	Delay_ns(250);
	Delay_ns(250);
// 	Delay_1us(1);
	
	/*��ADS1256 Ҫ�� SCL�ߵ�ƽ�͵͵�ƽ����ʱ����С 200ns  */
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
		SCLK_L;			/* <----  ADS1256 ����SCK�½��ز���DIN����, ���ݱ���ά�� 50nS */
		Delay_ns(250);
// 		Delay_1us(1);
	}
}

/*********************************************************************************************************
*	�� �� ��: ADS1256_Recive8Bit
*	����˵��: ��SPI���߽���8��bit���ݡ� ����CS���ơ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************/
uint8_t ADS1256_Recive8Bit(void)
{
	uint8_t i;
	uint8_t read = 0;

	Delay_ns(250);
	Delay_ns(250);
// 	Delay_1us(1);
	/*��ADS1256 Ҫ�� SCL�ߵ�ƽ�͵͵�ƽ����ʱ����С 200ns  */
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
*	�� �� ��: ADS1256_WriteReg
*	����˵��: дָ���ļĴ���
*	��    ��:  _RegID : �Ĵ���ID
*			  _RegValue : �Ĵ���ֵ
*	�� �� ֵ: ��
*********************************************************************************************************/
void ADS1256_WriteReg(uint8_t RegID, uint8_t RegValue)
{
	CS_L;	/* SPIƬѡ = 0 */
	ADS1256_Send8Bit(CMD_WREG | RegID);	/* д�Ĵ���������, �����ͼĴ�����ַ */
	ADS1256_Send8Bit(0x00);		/* �Ĵ������� - 1, �˴�д1���Ĵ��� */
	ADS1256_Send8Bit(RegValue);	/* ���ͼĴ���ֵ */
	CS_H;	/* SPIƬѡ = 1 */
}

/*********************************************************************************************************
*	�� �� ��: ADS1256_ReadReg
*	����˵��: ��ָ���ļĴ���
*	��    ��:  _RegID : �Ĵ���ID
*			  _RegValue : �Ĵ���ֵ��
*	�� �� ֵ: �����ļĴ���ֵ��
*********************************************************************************************************/
uint8_t ADS1256_ReadReg(uint8_t _RegID)
{
	uint8_t read;

	CS_L;	/* SPIƬѡ = 0 */
	ADS1256_Send8Bit(CMD_RREG | _RegID);	/* д�Ĵ���������, �����ͼĴ�����ַ */
	ADS1256_Send8Bit(0x00);	/* �Ĵ������� - 1, �˴���1���Ĵ��� */

	Delay_1us(1);	/* �����ӳٲ��ܶ�ȡоƬ�������� */

	read = ADS1256_Recive8Bit();	/* ���Ĵ���ֵ */
	CS_H;	/* SPIƬѡ = 1 */

	return read;
}

/*********************************************************************************************************
*	�� �� ��: ADS1256_WriteCmd
*	����˵��: ���͵��ֽ�����
*	��    ��:  _cmd : ����
*	�� �� ֵ: ��
*********************************************************************************************************/
void ADS1256_WriteCmd(uint8_t cmd)
{
	CS_L;	/* SPIƬѡ = 0 */
	ADS1256_Send8Bit(cmd);
	CS_H;	/* SPIƬѡ = 1 */
}

/*********************************************************************************************************
*	�� �� ��: ADS1256_ReadChipID
*	����˵��: ��оƬID, ��״̬�Ĵ����еĸ�4bit
*	��    ��: ��
*	�� �� ֵ: 8bit״̬�Ĵ���ֵ�ĸ�4λ
*********************************************************************************************************/
uint8_t ADS1256_ReadChipID(void)
{
	uint8_t id;

	ADS1256_WaitDRDY();
	id = ADS1256_ReadReg(REG_STATUS);
	return (id >> 4);
}

/*********************************************************************************************************
*	�� �� ��: ADS1256_SetChannal
*	����˵��: ����ͨ���š���·���á�AIN- �̶��ӵأ�ACOM).
*	��    ��: _ch : ͨ���ţ� 0-7
*	�� �� ֵ: ��
*********************************************************************************************************/
void ADS1256_SetChannal(uint8_t ch)
{
	/*
	λ7-4 PSEL3, PSEL2, PSEL1, PSEL0: ��������ͨ�� (AINP) ѡ��
		0000 = AIN0 (default)
		0001 = AIN1
		0010 = AIN2 (ADS1256 only)
		0011 = AIN3 (ADS1256 only)
		0100 = AIN4 (ADS1256 only)
		0101 = AIN5 (ADS1256 only)
		0110 = AIN6 (ADS1256 only)
		0111 = AIN7 (ADS1256 only)
		1xxx = AINCOM (when PSEL3 = 1, PSEL2, PSEL1, PSEL0 are ��don��t care��)

	λ 3-0 NSEL3, NSEL2, NSEL1, NSEL0:��������ͨ�� (AINN)ѡ��
		0000 = AIN0
		0001 = AIN1 (default)
		0010 = AIN2 (ADS1256 only)
		0011 = AIN3 (ADS1256 only)
		0100 = AIN4 (ADS1256 only)
		0101 = AIN5 (ADS1256 only)
		0110 = AIN6 (ADS1256 only)
		0111 = AIN7 (ADS1256 only)
		1xxx = AINCOM (when NSEL3 = 1, NSEL2, NSEL1, NSEL0 are ��don��t care��)
	*/
	if (ch > 7)
	{
		return;
	}
	ADS1256_WriteReg(REG_MUX, (ch << 4) | (1 << 3));	/* Bit3 = 1, AINN �̶��� AINCOM */
}

/*********************************************************************************************************
*	�� �� ��: ADS1256_SetDiffChannal
*	����˵��: ���ò��ͨ���š���·���á�
*	��    ��: _ch : ͨ����,0-3����4��
*	�� �� ֵ: 8bit״̬�Ĵ���ֵ�ĸ�4λ
*********************************************************************************************************/
void ADS1256_SetDiffChannal(uint8_t ch)
{
	/*
	λ7-4 PSEL3, PSEL2, PSEL1, PSEL0: ��������ͨ�� (AINP) ѡ��
		0000 = AIN0 (default)
		0001 = AIN1
		0010 = AIN2 (ADS1256 only)
		0011 = AIN3 (ADS1256 only)
		0100 = AIN4 (ADS1256 only)
		0101 = AIN5 (ADS1256 only)
		0110 = AIN6 (ADS1256 only)
		0111 = AIN7 (ADS1256 only)
		1xxx = AINCOM (when PSEL3 = 1, PSEL2, PSEL1, PSEL0 are ��don��t care��)

	λ 3-0 NSEL3, NSEL2, NSEL1, NSEL0:��������ͨ�� (AINN)ѡ��
		0000 = AIN0
		0001 = AIN1 (default)
		0010 = AIN2 (ADS1256 only)
		0011 = AIN3 (ADS1256 only)
		0100 = AIN4 (ADS1256 only)
		0101 = AIN5 (ADS1256 only)
		0110 = AIN6 (ADS1256 only)
		0111 = AIN7 (ADS1256 only)
		1xxx = AINCOM (when NSEL3 = 1, NSEL2, NSEL1, NSEL0 are ��don��t care��)
	*/
	if (ch == 0)
	{
		ADS1256_WriteReg(REG_MUX, (0 << 4) | 1);	/* ������� AIN0�� AIN1 */
	}
	else if (ch == 1)
	{
		ADS1256_WriteReg(REG_MUX, (2 << 4) | 3);	/* ������� AIN2�� AIN3 */
	}
	else if (ch == 2)
	{
		ADS1256_WriteReg(REG_MUX, (4 << 4) | 5);	/* ������� AIN4�� AIN5 */
	}
	else if (ch == 3)
	{
		ADS1256_WriteReg(REG_MUX, (6 << 4) | 7);	/* ������� AIN6�� AIN7 */
	}
}

/*********************************************************************************************************
*	�� �� ��: ADS1256_WaitDRDY
*	����˵��: �ȴ��ڲ�������ɡ� ��У׼ʱ��ϳ�����Ҫ�ȴ���
*	��    ��: ��
*	�� �� ֵ: ��
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
*	�� �� ��: ADS1256_ReadData
*	����˵��: ��ADC����
*	��    ��: ��
*	�� �� ֵ: ������� read
*********************************************************************************************************/
int32_t ADS1256_ReadData(void)
{
	union date Adc;
	
// 	uint32_t read = 0;

	CS_L;	/* SPIƬѡ = 0 */

	ADS1256_Send8Bit(CMD_RDATA);	/* �����ݵ����� */

	Delay_1us(1);	/* �����ӳٲ��ܶ�ȡоƬ�������� */
	
	Adc.read1[3] = 0;	
	Adc.read1[2] = ADS1256_Recive8Bit();
	Adc.read1[1] = ADS1256_Recive8Bit();
	Adc.read1[0] = ADS1256_Recive8Bit();
	
	/* �����������3���ֽڣ����ֽ���ǰ */
// 	read = (uint32_t)ADS1256_Recive8Bit() << 16;
// 	read += ((uint32_t)ADS1256_Recive8Bit() << 8);
// 	read += ADS1256_Recive8Bit();

	CS_H;	/* SPIƬѡ = 1 */

	/* ����������չ��24λ�з�������չΪ32λ�з����� */
// 	if (read & 0x800000)
// 	{
// 		read -= 0x800000;
// 		read += 0x80000000;
// 	}

	return Adc.read;
}

//#if 0
/*********************************************************************************************************
*	�� �� ��: ADS1256_ReadAdc
*	����˵��: ��ָ��ͨ����ADC����
*	��    ��: ��ȡ��ͨ�� ch
*	�� �� ֵ: ������� read
*********************************************************************************************************/
int32_t ADS1256_ReadAdc(uint8_t ch)
{
	/* ADS1256 �����ֲ��21ҳ */

//#if 0	/* ����30Ksps �������� */
	int32_t read;

	while (DRDY_IS_L);	/* �ȴ� DRDY �� */
	while (!DRDY_IS_L);	/* �ȴ� DRDY �� */

	ADS1256_SetChannal(ch);	/* �л�ģ��ͨ�� */
	Delay_1us(5);

	ADS1256_WriteCmd(CMD_SYNC);	/*ͬ��A/Dת��*/
	Delay_1us(5);

	ADS1256_WriteCmd(CMD_WAKEUP);  /* ��������£����ʱ�� DRDY �Ѿ�Ϊ�� */
	Delay_1us(25);

	read =  (int32_t)ADS1256_ReadData();

	while (DRDY_IS_L);	/* �ȴ� DRDY �� */
	while (!DRDY_IS_L);	/* �ȴ� DRDY �� */

	read =  (int32_t)ADS1256_ReadData();

	return read;
//#else
//	//while (DRDY_IS_LOW());

//	/* ADS1256 �����ֲ��21ҳ */
//	ADS1256_WaitDRDY();		/* �ȴ� DRDY = 0 */

//	ADS1256_SetChannal(ch);	/* �л�ģ��ͨ�� */
//	Delay_1us(5);

//	ADS1256_WriteCmd(CMD_SYNC);
//	Delay_1us(5);

//	ADS1256_WriteCmd(CMD_WAKEUP);
//	Delay_1us(25);

//	//ADS1256_WaitDRDY();		/* �ȴ� DRDY = 0 */

//	return (int32_t)ADS1256_ReadData();
//#endif
}
//#endif

/*********************************************************************************************************
*	�� �� ��: ADS1256_StartScan
*	����˵��: �� DRDY���� ��PC15 �����ó��ⲿ�жϴ�����ʽ�� �жϷ��������ɨ��8��ͨ�������ݡ�
*	��    ��: _ucDiffMode : 0 ��ʾ����ģʽ��ɨ��8·���� 1��ʾ���ģʽ��ɨ��4·
*	�� �� ֵ: ��
*********************************************************************************************************/
void ADS1256_StartScan(uint8_t ucScanMode)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	ScanMode = ucScanMode;
	/* ��ʼɨ��ǰ, ������������ */
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
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  /* �½��� */
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
*	�� �� ��: ADS1256_StartScan
*	����˵��: �� DRDY���� ��PC15 �����ó��ⲿ�жϴ�����ʽ�� �жϷ��������ɨ��8��ͨ�������ݡ�
*	��    ��: _ucDiffMode : 0 ��ʾ����ģʽ��ɨ��8·���� 1��ʾ���ģʽ��ɨ��4·
*	�� �� ֵ: ��
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
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  /* �½��� */
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
*	�� �� ��: ADS1256_StopScan
*	����˵��: ֹͣ DRDY �ж�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************/
void ADS1256_StopScan(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	/* ���� EXTI LineXXX */
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	/* �½���(�ȴ� DRDY ��1��0��ʱ��) */
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;		/* ��ֹ */
	EXTI_Init(&EXTI_InitStructure);

	/* �ж����ȼ����� ������ȼ� ����һ��Ҫ�ֿ��������жϣ����ܹ��ϲ���һ���������� */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;		/* ��ֹ */
	NVIC_Init(&NVIC_InitStructure);
}

/*********************************************************************************************************
*	�� �� ��: ADS1256_GetAdc
*	����˵��: �ӻ�������ȡADC��������������ṹ�����жϷ���������ġ�
*	��    ��: _ch ͨ���� (0 - 7)
*	�� �� ֵ: ADC�ɼ�������з�������
*********************************************************************************************************/
int32_t ADS1256_GetAdc(uint8_t ch)  //������ֱ�ӵ����������
{
	int32_t iTemp;

	if (ch > 7)
	{
		return 0;
	}

	ADS1256_StopScan();	/* ��ֹ�ж� */

	iTemp = AdcNow[ch];

	ADS1256_StartScan_1();	/* ʹ���ж� */

	return iTemp;
}

/*********************************************************************************************************
*	�� �� ��: ADS1256_ISR
*	����˵��: ��ʱ�ɼ��жϷ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************/
void ADS1256_ISR(void)    //Y  �жϷ�������е��øó���
{
	if (ScanMode == 0)	/* 0 ��ʾ����8·ɨ�裬1��ʾ���4·ɨ�� */
	{
		/* ��ȡ�ɼ��ṹ��������ȫ�ֱ��� */
		ADS1256_SetChannal(Channel);	/* �л�ģ��ͨ�� */
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
			AdcNow[7] = ADS1256_ReadData();	/* ע�Ᵽ�������һ��ͨ�������� */
		}
		else
		{
			AdcNow[Channel-1] = ADS1256_ReadData();	/* ע�Ᵽ�������һ��ͨ�������� */
		}

		if (++Channel >= 8)
		{
			Channel = 0;
		}
	}
	else	/* ���4·ɨ�� */
	{
		/* ��ȡ�ɼ��ṹ��������ȫ�ֱ��� */
		ADS1256_SetDiffChannal(Channel);	/* �л�ģ��ͨ�� */
		Delay_1us(5);

		ADS1256_WriteCmd(CMD_SYNC);
		Delay_1us(5);

		ADS1256_WriteCmd(CMD_WAKEUP);
		Delay_1us(25);

		if (Channel == 0)
		{
			AdcNow[3] = ADS1256_ReadData();	/* ע�Ᵽ�������һ��ͨ�������� */
		}
		else
		{
			AdcNow[Channel-1] = ADS1256_ReadData();	/* ע�Ᵽ�������һ��ͨ�������� */
		}

		if (++Channel >= 4)
		{
			Channel = 0;
		}
	}
}

/*********************************************************************************************************
*	�� �� ��: EXTI3_IRQHandler
*	����˵��: �ⲿ�жϷ������.
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************/
void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line9) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line9);		/* ����жϱ�־λ */

		ADS1256_ISR();

		/* ִ������Ĵ�����Ϻ��ٴ������жϱ�־ */
		EXTI_ClearITPendingBit(EXTI_Line9);		/* ����жϱ�־λ */
	}
}

/*********************************************************************************************************
*	�� �� ��: ADS1256_CfgADC
*	����˵��: ����ADC����������������������
*	��    ��: gain : ����
*			  _		drate : �����������
*	�� �� ֵ: ��
*********************************************************************************************************/
void ADS1256_CfgADC(uint8_t gain, uint8_t drate)   //Y
{
	ADS1256_StopScan();			/* ��ͣCPU�ж� */

	ADS1256_ResetHard();		/* Ӳ����λ */
	
	ADS1256_WriteCmd(CMD_RESET);
	
	ADS1256_WriteReg(REG_STATUS,0xf4);

  ADS1256_WriteCmd(CMD_SELFCAL);

	ADS1256_WaitDRDY();

	{
		uint8_t buf[4];		/* �ݴ�ADS1256 �Ĵ������ò�����֮������д4���Ĵ��� */

		/* ״̬�Ĵ�������
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

			ACAL=1ʹ����У׼���ܡ��� PGA��BUFEEN, DRATE�ı�ʱ��������У׼
		*/
		buf[0] = (0 << 3) | (1 << 2) | (0 << 1);
		//ADS1256_WriteReg(REG_STATUS, (0 << 3) | (1 << 2) | (1 << 1));

		buf[1] = 0x08;	/* ����λ0��ʾAINP�� AIN0,  ����λ8��ʾ AINN �̶��� AINCOM */

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
				01 = Sensor Detect Current = 0.5 �� A
				10 = Sensor Detect Current = 2 �� A
				11 = Sensor Detect Current = 10�� A
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
		//ADS1256_WriteReg(REG_ADCON, (0 << 5) | (0 << 2) | (GAIN_1 << 0));	/* ѡ��1;1����, ��������5V */

		/* ��Ϊ�л�ͨ���Ͷ����ݺ�ʱ 123uS, ���ɨ���ж�ģʽ����ʱ��������� = DRATE_1000SPS */
		buf[3] = drate;	// DRATE_10SPS;	/* ѡ������������� */

		CS_L;	/* SPIƬѡ = 0 */
		ADS1256_Send8Bit(CMD_WREG | 0);	/* д�Ĵ���������, �����ͼĴ�����ַ */
		ADS1256_Send8Bit(0x03);			/* �Ĵ������� - 1, �˴�3��ʾд4���Ĵ��� */

		ADS1256_Send8Bit(buf[0]);	/* ����״̬�Ĵ��� */
		ADS1256_Send8Bit(buf[1]);	/* ��������ͨ������ */
		ADS1256_Send8Bit(buf[2]);	/* ����ADCON���ƼĴ��������� */
		ADS1256_Send8Bit(buf[3]);	/* ��������������� */

		CS_H;	/* SPIƬѡ = 1 */
	}

	Delay_1us(50);
}

/*********************************************************************************************************
*	�� �� ��: ADS1256_ResetHard
*	����˵��: Ӳ����λ ADS1256оƬ.�͵�ƽ��λ�����4��ʱ�ӣ�Ҳ���� 4x0.13uS = 0.52uS
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************/
void ADS1256_ResetHard(void)
{
	Delay_1us(5);
}
