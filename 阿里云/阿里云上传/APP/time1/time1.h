#ifndef _TIME1_H
#define _TIME1_H

#include "system.h"


extern TIM_HandleTypeDef TIM1_Base_Handler;      //¶¨Ê±Æ÷¾ä±ú 

void TIM1_Init(u16 per,u16 psc);


#endif
