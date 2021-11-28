#include "mux.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//ADG706��������	   
//��������ѧ�����
//��������:2021/04/23
//�汾��V1.0
//��Ȩ���У�����ؾ���
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//��ʼ��PB1Ϊ���.��ʹ��ʱ��	    
//MUX IO��ʼ��
/*����PB5��ȥ������Ϊʹ�õ����п������е�PB5�����˷��������͵�ƽ��Ч��*/
const int selectPins[8] = {GPIO_PIN_3, GPIO_PIN_4, /*GPIO_PIN_5*/ GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10}; //�������������ڽ���ѡ��ͨ����ֻ����ѡ���ͨ��PD3-PD10


void MUX_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOB_CLK_ENABLE();           //����GPIOBʱ��
	  __HAL_RCC_GPIOD_CLK_ENABLE();           //����GPIODʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_6; //PB3-PB10, PD11���ڶ�·ѡ������ʹ��
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_NOPULL;          //����������
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
	  GPIO_Initure.Pin=GPIO_PIN_11; // PD11���ڶ�·ѡ������ʹ��
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_NOPULL;          //����������
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);


	
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_11,GPIO_PIN_RESET);	//PD11��0��Ĭ�ϳ�ʼ��ʹ�ܴ�	(7HC40��·ѡ����Ĭ��E�ǵ͵�ƽʹ�ܣ���������Բ��ã�
		for (int i=0; i<3; i++)
			{
				HAL_GPIO_WritePin(GPIOB, selectPins[i], GPIO_PIN_SET);     //Ĭ�ϳ�ʼ���ر�   ��i=0ʱ����������
//				PBout(i) = GPIO_PIN_SET;
			}

//				PDout(11) = GPIO_PIN_RESET;
}


void selectMuxPin(int pin)
{
  for (int i=0; i<3; i++)
  {
    if (pin & (1<<i))
      HAL_GPIO_WritePin(GPIOB, selectPins[i], GPIO_PIN_SET);
//				PBout(i) = GPIO_PIN_SET;
    else
      HAL_GPIO_WritePin(GPIOB, selectPins[i], GPIO_PIN_RESET);
//				PBout(i) = GPIO_PIN_RESET;
  }
}