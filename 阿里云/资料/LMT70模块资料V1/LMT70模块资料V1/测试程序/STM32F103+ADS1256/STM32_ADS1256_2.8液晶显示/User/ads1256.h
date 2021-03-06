#ifndef _ads1256_H
#define _ads1256_H


/* 寄存器地址， 后面是复位后缺省值 */
#define REG_STATUS	(0)	// x1H		状态寄存器
#define	REG_MUX    	(1) // 01H		输入多路复用器控制寄存器
#define	REG_ADCON   (2) // 20H		A/D控制寄存器
#define	REG_DRATE   (3) // F0H		A/D数据速率
#define	REG_IO      (4) // E0H		GPIO控制寄存器
#define	REG_OFC0    (5) // xxH		偏移校准字节0，最低有效字节
#define	REG_OFC1    (6) // xxH		偏移校准字节1
#define	REG_OFC2    (7) // xxH		偏移校准字节2，最高有效字节
#define	REG_FSC0    (8) // xxH		满量程校准字节0，最低有效字节
#define	REG_FSC1    (9) // xxH		满量程校准字节1
#define	REG_FSC2    (10)// xxH		满量程校准字节2，最高有效字节

/* 命令定义： TTable 24. Command Definitions --- ADS1256数据手册第34页 */
#define	CMD_WAKEUP		(0x00)	// Completes SYNC and Exits Standby Mode 0000  0000 (00h)		完成SYNC并退出待机状态
#define	CMD_RDATA   	(0x01) 	// Read Data 0000  0001 (01h)		读取数据
#define	CMD_RDATAC  	(0x03) 	// Read Data Continuously 0000   0011 (03h)		连续读取数据
#define	CMD_SDATAC  	(0x0F) 	// Stop Read Data Continuously 0000   1111 (0Fh)		停止连续读取数据
#define	CMD_RREG    	(0x10) 	// Read from REG rrr 0001 rrrr (1xh)		从REG读取
#define	CMD_WREG    	(0x50) 	// Write to REG rrr 0101 rrrr (5xh)			写到REG
#define	CMD_SELFCAL 	(0xF0) 	// Offset and Gain Self-Calibration 1111    0000 (F0h)		偏移和增益自校准
#define	CMD_SELFOCAL	(0xF1) 	// Offset Self-Calibration 1111    0001 (F1h)		偏移自校准
#define	CMD_SELFGCAL	(0xF2) 	// Gain Self-Calibration 1111    0010 (F2h)			增益自校准
#define	CMD_SYSOCAL 	(0xF3) 	// System Offset Calibration 1111   0011 (F3h)		系统偏移校准
#define	CMD_SYSGCAL 	(0xF4) 	// System Gain Calibration 1111    0100 (F4h)			系统增益校准
#define	CMD_SYNC    	(0xFC) 	// Synchronize the A/D Conversion 1111   1100 (FCh)		同步A/D转换
#define	CMD_STANDBY 	(0xFD) 	// Begin Standby Mode 1111   1101 (FDh)		开始待机模式
#define	CMD_RESET   	(0xFE) 	// Reset to Power-Up Values 1111   1110 (FEh)		重置为开机值

#define PGA_1            0x00
#define PGA_2            0x01
#define PGA_4            0x02
#define PGA_8            0x03
#define PGA_16           0x04
#define PGA_32           0x05
#define PGA_64           0x06

#define POSITIVE_AIN0       (0X00)
#define POSITIVE_AIN1       (0X10)
#define POSITIVE_AIN2       (0X20)
#define POSITIVE_AIN3       (0X30)
#define POSITIVE_AIN4       (0X40)
#define POSITIVE_AIN5       (0X50)
#define POSITIVE_AIN6       (0X60)
#define POSITIVE_AIN7       (0X70)
#define POSITIVE_AINCOM     (0X80)        

#define NEGTIVE_AIN0         0X00
#define NEGTIVE_AIN1         0X01
#define NEGTIVE_AIN2         0X02
#define NEGTIVE_AIN3         0X03
#define NEGTIVE_AIN4         0X04
#define NEGTIVE_AIN5         0X05
#define NEGTIVE_AIN6         0X06
#define NEGTIVE_AIN7         0X07
#define NEGTIVE_AINCOM       0X08

#define DATARATE_30K         0xF0
#define DATARATE_15K         0xE0
#define DATARATE_7_5K        0xD0
#define DATARATE_3_7_5K      0xC0
#define DATARATE_2K          0xB0
#define DATARATE_1K          0xA0
#define DATARATE_500         0x92
#define DATARATE_100         0x82
#define DATARATE_60        	 0x72
#define DATARATE_50        	 0x63
#define DATARATE_30        	 0x53
#define DATARATE_25        	 0x43
#define DATARATE_15        	 0x33
#define DATARATE_10        	 0x23
#define DATARATE_5        	 0x13
#define DATARATE_2_5     			0x02

#define	RST				(GPIOC,GPIO_Pin_13)
#define RST_L			GPIO_ResetBits(GPIOC,GPIO_Pin_13)
#define RST_H			GPIO_SetBits(GPIOC,GPIO_Pin_13)

#define	CS				(GPIOC,GPIO_Pin_11)
#define	CS_L			GPIO_ResetBits(GPIOC,GPIO_Pin_11)
#define	CS_H			GPIO_SetBits(GPIOC,GPIO_Pin_11)

#define	DRDY			(GPIOC,GPIO_Pin_9)
#define	DRDY_L		GPIO_ResetBits(GPIOC,GPIO_Pin_9)
#define	DRDY_H		GPIO_SetBits(GPIOC,GPIO_Pin_9)
#define DRDY_IS_L	(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9) == Bit_RESET)

#define	DOUT			(GPIOC,GPIO_Pin_12)
#define	DOUT_L		GPIO_ResetBits(GPIOC,GPIO_Pin_12)
#define	DOUT_H		GPIO_SetBits(GPIOC,GPIO_Pin_12)
#define DO_IS_H		(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12) == Bit_SET)

#define	DIN				(GPIOC,GPIO_Pin_10)
#define	DIN_L			GPIO_ResetBits(GPIOC,GPIO_Pin_10)
#define	DIN_H			GPIO_SetBits(GPIOC,GPIO_Pin_10)

#define	SCLK			(GPIOC,GPIO_Pin_8)
#define	SCLK_L		GPIO_ResetBits(GPIOC,GPIO_Pin_8)
#define	SCLK_H		GPIO_SetBits(GPIOC,GPIO_Pin_8)

///*STATUS REGISTER*/
//#define MSB_FRIST                (0x00<<3)
//#define LSB_FRIST                (0x01<<3)
//#define ACAL_OFF                 (0x00<<2)
//#define ACAL_ON                  (0x01<<2)
//#define BUFEN_OFF                (0x00<<1)
//#define BUFEN_ON                 (0x01<<1)

///*ADCON REGISTER*/
//#define CLKOUT_OFF               (0x00<<5)
//#define CLKOUT_CLKIN             (0x01<<5)
//#define DETECT_OFF               (0x00<<3)
//#define DETECT_ON_2UA            (0x02<<3)
extern void bsp_InitADS1256(void);
extern void ADS1256_Send8Bit(uint8_t data);
extern uint8_t ADS1256_Recive8Bit(void);
extern void ADS1256_WriteReg(uint8_t RegID, uint8_t RegValue);
extern uint8_t ADS1256_ReadReg(uint8_t _RegID);
extern void ADS1256_WriteCmd(uint8_t cmd);
extern uint8_t ADS1256_ReadChipID(void);
extern void ADS1256_SetChannal(uint8_t ch);
extern void ADS1256_SetDiffChannal(uint8_t ch);
extern void ADS1256_WaitDRDY(void);
extern int32_t ADS1256_ReadData(void);
extern int32_t ADS1256_ReadAdc(uint8_t ch);
extern void ADS1256_StartScan(uint8_t ucScanMode);
extern void ADS1256_StopScan(void);
extern int32_t ADS1256_GetAdc(uint8_t ch);
extern void TIM1_UP_IRQHandler(void);
extern void EXTI15_IRQHandler(void);
extern void ADS1256_CfgADC(uint8_t gain, uint8_t drate);
extern void ADS1256_ResetHard(void);
extern void ADS1256_StartScan_1(void);

//void ADS1256_Write_Byte(unsigned char d);
//unsigned char ADS1256_Read_Byte(void);
//void  ADS1256_Write_Reg(unsigned char reg_name, unsigned char reg_data);
//extern unsigned char  ADS1256_Read_one_Reg(unsigned char reg_name);
//extern void initads1256_bak(void);
//extern unsigned long result;
//extern unsigned long readad(void);
//extern void initads1256(void);

#endif

