#include "key.h"
#include "SysTick.h"


void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    
    KEY_UP_RCC_CLK_ENABLE;
    KEY_LEFT_RCC_CLK_ENABLE;
    KEY_RIGHT_RCC_CLK_ENABLE;
	KEY_DOWN_RCC_CLK_ENABLE;
    
    GPIO_InitStructure.Pin=KEY_UP_PIN;            
    GPIO_InitStructure.Mode=GPIO_MODE_INPUT;      //����
    GPIO_InitStructure.Pull=GPIO_PULLDOWN;        //����
    GPIO_InitStructure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(KEY_UP_PORT,&GPIO_InitStructure);
    
    GPIO_InitStructure.Pin=KEY_LEFT_PIN;           
    GPIO_InitStructure.Mode=GPIO_MODE_INPUT;      //����
    GPIO_InitStructure.Pull=GPIO_PULLUP;          //����
    GPIO_InitStructure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(KEY_LEFT_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.Pin=KEY_RIGHT_PIN;           
    HAL_GPIO_Init(KEY_RIGHT_PORT,&GPIO_InitStructure);
    
    GPIO_InitStructure.Pin=KEY_DOWN_PIN;           
    HAL_GPIO_Init(KEY_DOWN_PORT,&GPIO_InitStructure);
}

/*******************************************************************************
* �� �� ��         : KEY_Scan
* ��������		   : ����ɨ����
* ��    ��         : mode=0:���ΰ��°���
					 mode=1���������°���
* ��    ��         : 0��δ�а�������
					 KEY_UP��K_UP������
					 KEY_DOWN��K_DOWN������
					 KEY_LEFT��K_LEFT������
					 KEY_RIGHT��K_RIGHT������
*******************************************************************************/
u8 KEY_Scan(u8 mode)
{
	static u8 key=1;
	if(key==1&&(K_UP==1||K_DOWN==0||K_LEFT==0||K_RIGHT==0)) //����һ����������
	{
		delay_ms(10);  //����
		key=0;
		if(K_UP==1)
		{
			return KEY_UP; 
		}
		else if(K_DOWN==0)
		{
			return KEY_DOWN; 
		}
		else if(K_LEFT==0)
		{
			return KEY_LEFT; 
		}
		else
		{
			return KEY_RIGHT;
		}
	}
	else if(K_UP==0&&K_DOWN==1&&K_LEFT==1&&K_RIGHT==1)    //�ް�������
	{
		key=1;
	}
	if(mode==1) //������������
	{
		key=1;
	}
	return 0;
}
