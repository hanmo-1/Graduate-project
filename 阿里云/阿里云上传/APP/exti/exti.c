#include "exti.h"
#include "key.h"
#include "SysTick.h"
#include "led.h"
#include "beep.h"


//外部中断初始化
void EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    KEY_UP_RCC_CLK_ENABLE;
    KEY_LEFT_RCC_CLK_ENABLE;
    KEY_RIGHT_RCC_CLK_ENABLE;
	KEY_DOWN_RCC_CLK_ENABLE;
    
    GPIO_Initure.Pin=KEY_UP_PIN;               
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING;      //上升沿触发
    GPIO_Initure.Pull=GPIO_PULLDOWN;
    HAL_GPIO_Init(KEY_UP_PORT,&GPIO_Initure);
    
    GPIO_Initure.Pin=KEY_LEFT_PIN;              
    GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;     //下降沿触发
    GPIO_Initure.Pull=GPIO_PULLUP;
    HAL_GPIO_Init(KEY_LEFT_PORT,&GPIO_Initure);
    
    GPIO_Initure.Pin=KEY_RIGHT_PIN;     
    HAL_GPIO_Init(KEY_RIGHT_PORT,&GPIO_Initure);
	
	GPIO_Initure.Pin=KEY_DOWN_PIN;     
    HAL_GPIO_Init(KEY_DOWN_PORT,&GPIO_Initure);
    
    //中断线0-PA0
    HAL_NVIC_SetPriority(EXTI0_IRQn,2,0);       //抢占优先级为2，子优先级为0
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);             //使能中断线0
    
    //中断线2-PH2
    HAL_NVIC_SetPriority(EXTI2_IRQn,2,1);       //抢占优先级为2，子优先级为1
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);             //使能中断线2
    
    //中断线3-PH3
    HAL_NVIC_SetPriority(EXTI3_IRQn,2,2);       //抢占优先级为2，子优先级为2
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);             //使能中断线2
    
    //中断线13-PC13
    HAL_NVIC_SetPriority(EXTI15_10_IRQn,2,3);   //抢占优先级为2，子优先级为3
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);         //使能中断线13  
}


//中断服务函数
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);//调用中断处理公用函数
}

void EXTI2_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);//调用中断处理公用函数
}

void EXTI3_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);//调用中断处理公用函数
}

void EXTI15_10_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);//调用中断处理公用函数
}

//中断服务程序中需要做的事情
//在HAL库中所有的外部中断服务函数都会调用此函数
//GPIO_Pin:中断引脚号
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    delay_ms(100);      //消抖
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

