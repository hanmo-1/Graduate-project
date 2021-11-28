#ifndef _mux_H
#define _mux_H


#include "system.h"


#define MUX1	PBout(6)
#define MUX2	PBout(7)
#define MUX3	PBout(8)
#define MUX4	PBout(9)
#define MUX_EN	PBout(12)

void MUX_Init(void);
void selectMuxPin(int pin);

#endif