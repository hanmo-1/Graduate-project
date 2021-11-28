#ifndef _pwm_H
#define _pwm_H


#include "system.h"


void TIM3_CH4_PWM_Init(u16 pre,u16 psc);
void TIM_SetTIM3Compare4(u32 compare);

#endif
