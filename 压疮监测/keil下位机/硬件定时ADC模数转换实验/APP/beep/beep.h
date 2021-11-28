#ifndef _beep_H
#define _beep_H


#include "system.h"

#define BEEP_RCC_CLK_ENABLE		__HAL_RCC_GPIOB_CLK_ENABLE()
#define BEEP_PORT	GPIOB
#define BEEP_PIN	GPIO_PIN_5


#define BEEP	PBout(5)

void BEEP_Init(void);


#endif
