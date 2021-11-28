#include "beep.h"


void BEEP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	BEEP_RCC_CLK_ENABLE;
	
	GPIO_InitStructure.Pin=BEEP_PIN; 
    GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_InitStructure.Pull=GPIO_PULLUP;          //����
    GPIO_InitStructure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(BEEP_PORT,&GPIO_InitStructure);
	
	BEEP=1;
}
