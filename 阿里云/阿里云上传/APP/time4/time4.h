#ifndef _TIME4_H
#define _TIME4_H

#include "system.h"


extern TIM_HandleTypeDef TIM4_Base_Handler;      //��ʱ����� 


void TIM4_Init(u16 per,u16 psc);

#endif
