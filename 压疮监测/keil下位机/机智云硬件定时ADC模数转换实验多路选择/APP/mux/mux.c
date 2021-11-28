#include "mux.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//ADG706��������	   
//��������ѧ�����
//������̳:www.openedv.com
//��������:2021/04/23
//�汾��V1.0
//��Ȩ���У�����ؾ���
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//��ʼ��PB1Ϊ���.��ʹ��ʱ��	    
//MUX IO��ʼ��
const int selectPins[8] = {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10}; //�������������ڽ���ѡ��ͨ����ֻ����ѡ���ͨ��PD3-PD10


void MUX_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOB_CLK_ENABLE();           //����GPIODʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11; //PD3-PD10, PD11���ڶ�·ѡ������ʹ��
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
		for (int i=0; i<8; i++)
			{
				HAL_GPIO_WritePin(GPIOD, selectPins[i], GPIO_PIN_SET);     //Ĭ�ϳ�ʼ���ر�
			}
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_11,GPIO_PIN_SET);	//PD11��1��Ĭ�ϳ�ʼ���ر�ʹ��
}
void selectMuxPin(int pin)
{
  for (int i=0; i<8; i++)
  {
    if (pin & (1<<i))
      HAL_GPIO_WritePin(GPIOD, selectPins[i], GPIO_PIN_SET);
    else
      HAL_GPIO_WritePin(GPIOD, selectPins[i], GPIO_PIN_RESET);
  }
}