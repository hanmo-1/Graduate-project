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
    GPIO_InitStructure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_InitStructure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_InitStructure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(KEY_UP_PORT,&GPIO_InitStructure);
    
    GPIO_InitStructure.Pin=KEY_LEFT_PIN;           
    GPIO_InitStructure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_InitStructure.Pull=GPIO_PULLUP;          //上拉
    GPIO_InitStructure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(KEY_LEFT_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.Pin=KEY_RIGHT_PIN;           
    HAL_GPIO_Init(KEY_RIGHT_PORT,&GPIO_InitStructure);
    
    GPIO_InitStructure.Pin=KEY_DOWN_PIN;           
    HAL_GPIO_Init(KEY_DOWN_PORT,&GPIO_InitStructure);
}

/*******************************************************************************
* 函 数 名         : KEY_Scan
* 函数功能		   : 按键扫描检测
* 输    入         : mode=0:单次按下按键
					 mode=1：连续按下按键
* 输    出         : 0：未有按键按下
					 KEY_UP：K_UP键按下
					 KEY_DOWN：K_DOWN键按下
					 KEY_LEFT：K_LEFT键按下
					 KEY_RIGHT：K_RIGHT键按下
*******************************************************************************/
u8 KEY_Scan(u8 mode)
{
	static u8 key=1;
	if(key==1&&(K_UP==1||K_DOWN==0||K_LEFT==0||K_RIGHT==0)) //任意一个按键按下
	{
		delay_ms(10);  //消抖
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
	else if(K_UP==0&&K_DOWN==1&&K_LEFT==1&&K_RIGHT==1)    //无按键按下
	{
		key=1;
	}
	if(mode==1) //连续按键按下
	{
		key=1;
	}
	return 0;
}
