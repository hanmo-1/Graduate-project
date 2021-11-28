#ifndef __AHT20_H
#define __AHT20_H

#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x.h"
#include "bsp_i2c.h"

#define AHT20_SLAVE_ADDRESS    0x38		/* I2C�ӻ���ַ */

//****************************************
// ���� AHT20 �ڲ���ַ
//****************************************
#define	INIT		    0xBE	//��ʼ��
#define	SoftReset		0xBA	//��λ
#define	StartTest		0xAC	//��ʼ����

uint8_t AHT20_Init(void);
uint8_t AHT20_Read_Cal_Enable(void);  //��ѯcal enableλ��û��ʹ��
void AHT20_Read_CTdata(uint32_t *ct); //��ȡAHT10���¶Ⱥ�ʪ������

#endif
