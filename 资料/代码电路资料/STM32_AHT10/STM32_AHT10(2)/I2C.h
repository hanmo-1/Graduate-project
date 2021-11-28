#include "stm32f10x.h"
#include <stdbool.h>

#define I2C_CLR (GPIOB->BSRR = GPIO_Pin_5)
#define I2C_EN (GPIOB->BRR = GPIO_Pin_5)

#define SCL_H  (GPIOB->BSRR = GPIO_Pin_6)
#define SCL_L  (GPIOB->BRR = GPIO_Pin_6)

#define SDA_H  (GPIOB->BSRR = GPIO_Pin_7)
#define SDA_L  (GPIOB->BRR = GPIO_Pin_7)

#define SCL_READ (GPIOB->IDR & GPIO_Pin_6)
#define SDA_READ (GPIOB->IDR & GPIO_Pin_7)

#define SDA_OUT (GPIO_PIN7_OPP())
#define SDA_IN  (GPIO_PIN7_FLOATING())

void GPIO_PIN7_OPP(void);
void GPIO_PIN_UIP(void);
void GPIO_config(void);
void I2C_DELAY(uint16_t i); 
void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendByte(uint8_t SendByte);//I2C·¢ËÍÊý¾Ý
u8 I2C_ReceiveByte(void);  //I2C·µ»ØÊý¾Ý
u8 Receive_ACK(void);     //´Ó»úI2CÓ¦´ð
void Send_NOT_ACK(void);	//Ö÷»ú²»»Ø¸´Ó¦´ð
void Send_ACK(void);     //Ö÷»ú»Ø¸´Ó¦´ðACKÐÅºÅ
void JH_Init(void);      //AHT10³õÊ¼»¯
u8 JH_Read_Cal_Enable(void);
u8 JH_Read_Status(void);//¶ÁÈ¡AHT10µÄ×´Ì¬¼Ä´æÆ
void JH_Send_BA(void);//ÏòAHT·¢ËÍBAÃüÁî
void JH_Read_CTdata(uint32_t *ct);//¶ÁÈ¡AHT10µÄÎÂ¶ÈºÍÊª¶ÈÊý¾Ý






