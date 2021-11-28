#ifndef _key_H
#define _key_H


#include "system.h"


#define KEY_UP_RCC_CLK_ENABLE		__HAL_RCC_GPIOA_CLK_ENABLE()
#define KEY_UP_PORT					GPIOA
#define KEY_UP_PIN					GPIO_PIN_0

#define KEY_LEFT_RCC_CLK_ENABLE		__HAL_RCC_GPIOC_CLK_ENABLE()
#define KEY_LEFT_PORT				GPIOC
#define KEY_LEFT_PIN				GPIO_PIN_13

#define KEY_RIGHT_RCC_CLK_ENABLE	__HAL_RCC_GPIOH_CLK_ENABLE()
#define KEY_RIGHT_PORT				GPIOH
#define KEY_RIGHT_PIN				GPIO_PIN_3

#define KEY_DOWN_RCC_CLK_ENABLE		__HAL_RCC_GPIOH_CLK_ENABLE()
#define KEY_DOWN_PORT				GPIOH
#define KEY_DOWN_PIN				GPIO_PIN_2


//使用位操作定义
#define K_UP PAin(0)
#define K_DOWN PHin(2)
#define K_LEFT PCin(13)
#define K_RIGHT PHin(3)


//定义各个按键值  
#define KEY_UP 1
#define KEY_DOWN 2
#define KEY_LEFT 3
#define KEY_RIGHT 4  


void KEY_Init(void);
u8 KEY_Scan(u8 mode);

#endif
