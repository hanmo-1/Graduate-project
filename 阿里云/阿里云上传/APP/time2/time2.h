#ifndef _TIME2_H
#define _TIME2_H

#include "system.h"


extern TIM_HandleTypeDef TIM2_Base_Handler;      //��ʱ����� 

void TIM2_Init(u16 per,u16 psc);

#endif
