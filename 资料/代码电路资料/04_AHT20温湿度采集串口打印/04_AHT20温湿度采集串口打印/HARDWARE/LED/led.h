#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

void LED_Init(void);//≥ı ºªØ
void BEEP_Init(void);
#define LED1     PBout(3)
#define LED2  	 PBout(4)
#define BEEP  	 PAout(15)


#endif
