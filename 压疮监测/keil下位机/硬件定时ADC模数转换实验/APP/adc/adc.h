#ifndef _adc_H
#define _adc_H

#include "system.h"

void ADCx_Init(void);
u16 Get_ADC_Value(u32 ch,u8 times);
u16 Get_Adc(u32 ch);


#endif
