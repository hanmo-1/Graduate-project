#include "exti.h"
#include "key.h"
#include "SysTick.h"
#include "led.h"
#include "beep.h"


//�ⲿ�жϳ�ʼ��
void EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    KEY_UP_RCC_CLK_ENABLE;
    KEY_LEFT_RCC_CLK_ENABLE;
    KEY_RIGHT_RCC_CLK_ENABLE;
	KEY_DOWN_RCC_CLK_ENABLE;
    
    GPIO_Initure.Pin=KEY_UP_PIN;               
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING;      //�����ش���
    GPIO_Initure.Pull=GPIO_PULLDOWN;
    HAL_GPIO_Init(KEY_UP_PORT,&GPIO_Initure);
    
    GPIO_Initure.Pin=KEY_LEFT_PIN;              
    GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;     //�½��ش���
    GPIO_Initure.Pull=GPIO_PULLUP;
    HAL_GPIO_Init(KEY_LEFT_PORT,&GPIO_Initure);
    
    GPIO_Initure.Pin=KEY_RIGHT_PIN;     
    HAL_GPIO_Init(KEY_RIGHT_PORT,&GPIO_Initure);
	
	GPIO_Initure.Pin=KEY_DOWN_PIN;     
    HAL_GPIO_Init(KEY_DOWN_PORT,&GPIO_Initure);
    
    //�ж���0-PA0
    HAL_NVIC_SetPriority(EXTI0_IRQn,2,0);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ0
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);             //ʹ���ж���0
    
    //�ж���2-PH2
    HAL_NVIC_SetPriority(EXTI2_IRQn,2,1);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ1
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);             //ʹ���ж���2
    
    //�ж���3-PH3
    HAL_NVIC_SetPriority(EXTI3_IRQn,2,2);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ2
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);             //ʹ���ж���2
    
    //�ж���13-PC13
    HAL_NVIC_SetPriority(EXTI15_10_IRQn,2,3);   //��ռ���ȼ�Ϊ2�������ȼ�Ϊ3
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);         //ʹ���ж���13  
}


//�жϷ�����
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);//�����жϴ����ú���
}

void EXTI2_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);//�����жϴ����ú���
}

void EXTI3_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);//�����жϴ����ú���
}

void EXTI15_10_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);//�����жϴ����ú���
}

//�жϷ����������Ҫ��������
//��HAL�������е��ⲿ�жϷ�����������ô˺���
//GPIO_Pin:�ж����ź�
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    delay_ms(100);      //����
    switch(GPIO_Pin)
    {
        case GPIO_PIN_0:
            if(K_UP==1) 
            {
				LED1=0;
            }
            break;
        case GPIO_PIN_2:
            if(K_DOWN==0) 
            {
                LED2=1;    
            }
            break;
        case GPIO_PIN_3:
            if(K_RIGHT==0)  
            {
                BEEP=0;
            }
            break;

        case GPIO_PIN_13:
            if(K_LEFT==0)  
            {
				BEEP=1;
            }
            break;
    }
}

