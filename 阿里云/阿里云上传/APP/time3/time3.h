#ifndef _TIME3_H
#define _TIME3_H

#include "system.h"


extern TIM_HandleTypeDef  TIM3_Base_Handler;      //��ʱ����� 


void TIM3_Init(u16 per,u16 psc);


#endif
